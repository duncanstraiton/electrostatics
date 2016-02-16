#ifdef IFiniteDiff

#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <iostream>
#include "finiteDifferenceSolve.h"
#include "SolvedElectrostaticSystem.h"
#include "UnsolvedElectrostaticSystem.h"

namespace electrostatics {

/* Iterative finite difference method.
 *
 * Copies back and forth from solvedSystem to unsolvedSystem for each iteration.
 */
void finiteDifferenceSolve(UnsolvedElectrostaticSystem &unsolvedSystem,
        SolvedElectrostaticSystem &solvedSystem, int maxIterations) {

    int iMin = unsolvedSystem.getIMin();
    int iMax = unsolvedSystem.getIMax();
    int jMin = unsolvedSystem.getJMin();
    int jMax = unsolvedSystem.getJMax();

    // Copy boundary conditions over to solvedSystem
    for(int i=iMin; i<=iMax; i++) {
        for(int j=jMin; j<=jMax; j++) {
            solvedSystem.setPotentialIJ(i, j, unsolvedSystem.getPotentialIJ(i, j));
        }
    }

    // Loop for the required number of iterations
    for(int iter=1; iter<=maxIterations; iter++) {
        // Loop over all the points in the system
        for(int i=iMin; i<=iMax; i++) {
            for(int j=jMin; j<=jMax; j++) {

                // If the point is a boundary condition, leave it alone
                if(unsolvedSystem.isBoundaryConditionIJ(i, j)) {
                    continue;
                }

                int surroundingPoints = 0;  // Number of points surrounding (i, j)
                double sum = 0;             // Sum of potentials around (i, j)

                // If on an odd iteration, go from unsolved to solved
                if(iter%2 == 1) {
                    if(i<iMax) {
                        surroundingPoints += 1;
                        sum += unsolvedSystem.getPotentialIJ(i+1, j);
                    }
                    if(i>iMin) {
                        surroundingPoints += 1;
                        sum += unsolvedSystem.getPotentialIJ(i-1, j);
                    }
                    if(j<jMax) {
                        surroundingPoints += 1;
                        sum += unsolvedSystem.getPotentialIJ(i, j+1);
                    }
                    if(j>jMin) {
                        surroundingPoints += 1;
                        sum += unsolvedSystem.getPotentialIJ(i, j-1);
                    }
                    solvedSystem.setPotentialIJ(i, j, sum/surroundingPoints);
                }

                // Otherwise for even iteration, go from solved to unsolved
                else {
                    if(i<iMax) {
                        surroundingPoints += 1;
                        sum += solvedSystem.getPotentialIJ(i+1, j);
                    }
                    if(i>iMin) {
                        surroundingPoints += 1;
                        sum += solvedSystem.getPotentialIJ(i-1, j);
                    }
                    if(j<jMax) {
                        surroundingPoints += 1;
                        sum += solvedSystem.getPotentialIJ(i, j+1);
                    }
                    if(j>jMin) {
                        surroundingPoints += 1;
                        sum += solvedSystem.getPotentialIJ(i, j-1);
                    }
                    unsolvedSystem.setPotentialIJ(i, j, sum/surroundingPoints);
                }
            }   
        }
    }
}



} // namespace electrostatics

#endif // IFiniteDiff
