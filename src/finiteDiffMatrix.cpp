#define VIENNACL_WITH_EIGEN 1

#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <viennacl/linalg/bicgstab.hpp>
#include <viennacl/vector.hpp>
#include <viennacl/matrix.hpp>
#include <viennacl/compressed_matrix.hpp>
#include <string>
#include "finiteDiffMatrix.h"
#include "SolvedElectrostaticSystem.h"
#include "UnsolvedElectrostaticSystem.h"

namespace electrostatics {

/* Takes an UnsolvedElectrostaticSystem and and empty SolvedElectrostaticSystem,
 * solves the unsolved system and saves the result in the solved system.
 *
 * Has to be passed the SolvedElectrostaticSystem to fill as if it was created in
 * this function it would not exist after the function returns - 
 * It would be a local variable - can't return a pointer to a local variable
 *
 * The function works by using the formula:
 * P(i, j+1) + P(i, j-1) + P(i+1, j) + P(i-1, j) - 4*P(i, j) = 0
 * where P(i, j) is the potential at point (i, j).
 *
 * A system of simultaneous linear equations is formed by applying this formula
 * to every grid point that is not a boundary condition.
 *
 * For each boundary condition another equation is added to the system of the form
 * P(i, j) = The specified potential
 *
 * Forming a matrix equation of the form  Av = b, where b is the vector of
 * boundary values, and v is the vector of unknown potentials to solve for,
 * and A is the coefficents matrix, the Eigen or ViennaCL library is then
 * used to solve the system as specified by the function call.
 */
void finiteDiffMatrix(const UnsolvedElectrostaticSystem &unsolvedSystem,
        SolvedElectrostaticSystem &solvedSystem, std::string method) {

    long kMax = unsolvedSystem.getKMax();
    Eigen::SparseMatrix<double> A;

    // Dimension kMax+1 as k counts from zero
    // Sparse Lu solve method needs column major storage, others need row major
    if(method == "eigensparselu") {
        A = Eigen::SparseMatrix<double, Eigen::ColMajor>(kMax+1, kMax+1);
    } else {
        A = Eigen::SparseMatrix<double, Eigen::RowMajor>(kMax+1, kMax+1);
    }

    Eigen::VectorXd b(kMax+1);  // Boundary values vector

    // Allocate space for A - it wont have more than 5 elements per row
    A.reserve(Eigen::VectorXi::Constant(kMax+1, 5));

    // Fill the matrix A and boundary values vector b
    for(int j=unsolvedSystem.getJMin(); j<=unsolvedSystem.getJMax(); j++) {
        for(int i=unsolvedSystem.getIMin(); i<=unsolvedSystem.getIMax(); i++) {
            int k = unsolvedSystem.ij2k(i, j);

            /* If (i, j) is a boundary condition, add the corresponding boundary
             * to A and the bounday values vector:
             * (i, j) = boundary value
             */
            if(unsolvedSystem.isBoundaryConditionIJ(i, j)) {
                A.insert(k, k) = 1;
                b(k) = unsolvedSystem.getPotentialIJ(i, j);
            }

            /* If (i,j) is not a boundary condition, add finite difference equation 
             * for (i, j) to A:
             * (i, j+1) + (i, j-1) + (i+1, j) + (i-1, j) - 4(i, j) = 0
             * If (i, j) is on the edge of the system, ignore points that would
             * end up outside and adjust the coefficent for that point (the 
             * 4 in the above equation) accordingly.
             */
            else {
                int surroundingPoints = 0;
                if(i<unsolvedSystem.getIMax()) {
                    A.insert(k, k+1) = 1;
                    surroundingPoints += 1;
                }
                if(i>unsolvedSystem.getIMin()) {
                    A.insert(k, k-1) = 1;
                    surroundingPoints += 1;
                }
                if(j<unsolvedSystem.getJMax()) {
                    A.insert(k, k+unsolvedSystem.getLengthI()) = 1;
                    surroundingPoints += 1;
                }
                if(j>unsolvedSystem.getJMin()) {
                    A.insert(k, k-unsolvedSystem.getLengthI()) = 1;
                    surroundingPoints += 1;
                }
                A.insert(k, k) = -surroundingPoints;
            }
        }
    }
    A.makeCompressed();

    Eigen::VectorXd solution(kMax+1); // Eigen vector to hold the solution
    if(method == "eigenbicon") {
        Eigen::BiCGSTAB<Eigen::SparseMatrix<double, Eigen::RowMajor> > solver;
        solver.compute(A);
        solution = solver.solve(b);
    }
    else if(method == "eigensparselu") {
        Eigen::SparseLU<Eigen::SparseMatrix<double, Eigen::ColMajor> > solver;
        solver.analyzePattern(A);
        solver.factorize(A);
        solution = solver.solve(b);
    }
    else if(method == "viennabicon") {
        // Make variables for viennacl and copy data to them
        viennacl::vector<double> vcl_b(kMax+1);
        viennacl::compressed_matrix<double> vcl_A(kMax+1, kMax+1);
        viennacl::copy(b, vcl_b);
        viennacl::copy(A, vcl_A);
        // Make ViennaCL vector variable for the solution
        viennacl::vector<double> vcl_solution(kMax+1);
        // Solve the system using viennacl's bicgstab method and copy back to eigen vector
        vcl_solution = viennacl::linalg::solve(vcl_A, vcl_b, viennacl::linalg::bicgstab_tag());
        viennacl::copy(vcl_solution, solution);
    }

    // Set the potentials in the solved system to the ones just calculated
    for(int k=0; k<=kMax; k++) {
        solvedSystem.setPotentialK(k, solution(k));
    }
}

} // namespace electrostatics
