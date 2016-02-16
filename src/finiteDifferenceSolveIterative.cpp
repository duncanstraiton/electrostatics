#ifdef IFiniteDiff

#include <Eigen/Dense>
#include <Eigen/Sparse>
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

    // Loop for the required number of iterations
    for(int iter=1; iter<=maxIterations; iter++) {
        // Loop over all the points in the system
        for(int i=unsolvedSystem.getIMin();i<=unsolvedSystem.getIMax();i++) {
            for(int j=unsolvedSystem.getJMin();j<=unsolvedSystem.getJMax();j++) {

                // If the point is a boundary condition, leave it along
                if (unsolvedSystem.isBoundaryConditionIJ(i,j)) continue;

                // If on an odd iteration, go from unsolved to solved
                if(iter%2 == 1) {
                    double sum=0;
                    if(i<unsolvedSystem.getIMax()) sum += unsolvedSystem.getPotentialIJ(i+1, j);
                    if(i>unsolvedSystem.getIMin()) sum += unsolvedSystem.getPotentialIJ(i-1, j);
                    if(j<unsolvedSystem.getJMax()) sum += unsolvedSystem.getPotentialIJ(i, j+1);
                    if(j>unsolvedSystem.getJMin()) sum += unsolvedSystem.getPotentialIJ(i, j-1);
                    solvedSystem.setPotentialIJ(i, j, sum/4);
                }

                // Otherwise for even iteration, go from solved to unsolved
                else {
                    double sum=0;
                    if(i<solvedSystem.getIMax()) sum += solvedSystem.getPotentialIJ(i+1, j);
                    if(i>solvedSystem.getIMin()) sum += solvedSystem.getPotentialIJ(i-1, j);
                    if(j<solvedSystem.getJMax()) sum += solvedSystem.getPotentialIJ(i, j+1);
                    if(j>solvedSystem.getJMin()) sum += solvedSystem.getPotentialIJ(i, j-1);
                    unsolvedSystem.setPotentialIJ(i,j, sum/4);
                }
            }   
        }
    }
}



} // namespace electrostatics

#endif // IFiniteDiff
