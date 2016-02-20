#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <iostream>
#include "finiteDiffIterative.h"
#include "SolvedElectrostaticSystem.h"
#include "UnsolvedElectrostaticSystem.h"

namespace electrostatics {

/* Iterative finite difference method.
 *
 * Copies back and forth from solvedSystemA to solvedSystemB for each iteration.
 */
void finiteDiffIterative(const UnsolvedElectrostaticSystem &unsolvedSystem,
        SolvedElectrostaticSystem &solvedSystemA, int maxIterations) {

    int iMin = unsolvedSystem.getIMin();
    int iMax = unsolvedSystem.getIMax();
    int jMin = unsolvedSystem.getJMin();
    int jMax = unsolvedSystem.getJMax();

    electrostatics::SolvedElectrostaticSystem solvedSystemB(iMin, iMax, jMin, jMax);

    // Copy boundary conditions over to solvedSystemA and solvedSystemB
    for(int i=iMin; i<=iMax; i++) {
        for(int j=jMin; j<=jMax; j++) {
            solvedSystemA.setPotentialIJ(i, j, unsolvedSystem.getPotentialIJ(i, j));
            solvedSystemB.setPotentialIJ(i, j, unsolvedSystem.getPotentialIJ(i, j));
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

                // If on an odd iteration, go from A to B
                if(iter%2 == 1) {
                    if(i<iMax) {
                        surroundingPoints += 1;
                        sum += solvedSystemA.getPotentialIJ(i+1, j);
                    }
                    if(i>iMin) {
                        surroundingPoints += 1;
                        sum += solvedSystemA.getPotentialIJ(i-1, j);
                    }
                    if(j<jMax) {
                        surroundingPoints += 1;
                        sum += solvedSystemA.getPotentialIJ(i, j+1);
                    }
                    if(j>jMin) {
                        surroundingPoints += 1;
                        sum += solvedSystemA.getPotentialIJ(i, j-1);
                    }
                    solvedSystemB.setPotentialIJ(i, j, sum/surroundingPoints);
                }

                // Otherwise for even iteration, go from B to A
                else {
                    if(i<iMax) {
                        surroundingPoints += 1;
                        sum += solvedSystemB.getPotentialIJ(i+1, j);
                    }
                    if(i>iMin) {
                        surroundingPoints += 1;
                        sum += solvedSystemB.getPotentialIJ(i-1, j);
                    }
                    if(j<jMax) {
                        surroundingPoints += 1;
                        sum += solvedSystemB.getPotentialIJ(i, j+1);
                    }
                    if(j>jMin) {
                        surroundingPoints += 1;
                        sum += solvedSystemB.getPotentialIJ(i, j-1);
                    }
                    solvedSystemA.setPotentialIJ(i, j, sum/surroundingPoints);
                }
            }   
        }
    }

    // If doing an odd number of iterations, result ends up in B, so copy it into A
    if(maxIterations%2 == 1) {
        for(int i=iMin; i<=iMax; i++) {
            for(int j=jMin; j<=jMax; j++) {
                solvedSystemA.setPotentialIJ(i, j, solvedSystemB.getPotentialIJ(i, j));
            }
        }
    }
}



} // namespace electrostatics
