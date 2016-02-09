#ifdef biCon

#include <Eigen/Dense>
#include <Eigen/Sparse>
#include "finiteDifferenceSolve.h"
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
 * There may be a better way of doing this - something to look into.
 *
 *
 * The function works by using the formula:
 * P(i, j+1) + P(i, j-1) + P(i+1, j) + P(i-1, j) - 4*P(i, j) = 0
 * where P(i, j) is the potential at point (i, j).
 *
 * A system of simultaneous linear equations is formed by applying this formula
 * to every grid point that is not a boundary condition.
 *
 * For each boundary condition another formula is added to the system of the form
 * P(i, j) = The specified potential
 *
 * Forming a matrix equation of the form  Av = b, where b is the vector of
 * boundary values, and v is the vector of unknown potentials to solve for,
 * and A is the coefficents matrix, the Eigen library is used to solve for v.
 */
void finiteDifferenceSolve(const UnsolvedElectrostaticSystem &unsolvedSystem,
        SolvedElectrostaticSystem &solvedSystem) {
    long kMax = unsolvedSystem.getKMax();
    // Note A could be an int matrix but would need to cast to doubles before solving
    Eigen::SparseMatrix<double, Eigen::RowMajor> A(kMax+1, kMax+1); // Dimension kMax+1 as k counts from zero.
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
                if(k>unsolvedSystem.getLengthI()) {
                    A.insert(k, k-unsolvedSystem.getLengthI()) = 1;
                    surroundingPoints += 1;
                }
                A.insert(k, k) = -surroundingPoints;
            }
        }
    }

    /* Solving the system using the biconjugate gradient stabilised method.
     *
     * Needs to be a matrix of doubles, wont work for ints (I think).
     */
    A.makeCompressed();
    Eigen::BiCGSTAB<Eigen::SparseMatrix<double, Eigen::RowMajor> > solver;
    solver.compute(A);
    Eigen::VectorXd solution = solver.solve(b); // Solve Av = b, v is the solution

    // Set the potentials in the solved system to the ones just calculated
    for(int k=0; k<=kMax; k++) {
        solvedSystem.setPotentialK(k, solution(k));
    }
}

} // namespace electrostatics

#endif //biCon
