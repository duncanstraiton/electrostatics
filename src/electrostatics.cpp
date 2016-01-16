#include "finiteDifferenceSolve.h"
#include "SolvedElectrostaticSystem.h"
#include "UnsolvedElectrostaticSystem.h"
#include <iostream>
#include <cmath>

/* Currently set to solve the second problem from the project guide,
 * however this is just as a temporary test to check that things are working
 * and to get some initial output.
 *
 * It will need to be generalised / changed to allow solving of arbitrary electrostatic
 * systems.
 */
int main() {
    // Set up the unsolved system
    int iMin = -50;
    int iMax = 50;
    int jMin = -50;
    int jMax = 50;
    electrostatics::UnsolvedElectrostaticSystem unsolvedSystem(iMin, iMax, jMin, jMax);

    /* Second problem from group project description:
     * parallel plates to left and right, left at +v,
     * right at -v, circle (radius r) in the middle at v=0.
     */
    int v1 = 5;      // Voltage at left hand plate
    int v2 = 0;      // Voltage of circle in middle
    int v3 = -5;     // Voltage of right hand plate
    int radius = 10; // Radius of the circle

    // Set points as required
    for(int i=iMin; i<=iMax; i++) {
        for(int j=jMin; j<=jMax; j++) {
            
            // Set left hand plate as boundary condition
            if(i==iMin) {
                unsolvedSystem.setPotentialIJ(i, j, v1);
                unsolvedSystem.setBoundaryConditionIJ(i, j, true);
            }

            // Set right hand plate as boundary condition
            else if(i==iMax) {
                unsolvedSystem.setPotentialIJ(i, j, v3);
                unsolvedSystem.setBoundaryConditionIJ(i, j, true);
            }

            // Set circle as boundary condition
            else if(sqrt(pow(i, 2) + pow(j ,2)) <= radius) {
                unsolvedSystem.setPotentialIJ(i, j, v2);
                unsolvedSystem.setBoundaryConditionIJ(i, j, true);
            }
        }
    }

    // Create the solved system
    electrostatics::SolvedElectrostaticSystem solvedSystem(iMin, iMax, jMin, jMax);

    // Pass the unsolved system to the finite difference solving function along with 
    // the new solved system object just created for it to store the results in
    electrostatics::finiteDifferenceSolve(unsolvedSystem, solvedSystem);

    // Output the system as as a list of the following form:  iValue  jValue  potentialAt(i,j)
    for(int i=iMin; i<=iMax; i++) {
        for(int j=jMin; j<=jMax; j++) {
            std::cout << i << " " << j << " " << solvedSystem.getPotentialIJ(i, j) << "\n";
        }
        std::cout << "\n";;
    }
}
