#ifndef FINITEDIFFERENCESOLVE_H
#define FINITEDIFFERENCESOLVE_H

#include <Eigen/Dense>
#include <Eigen/Sparse>
#include "SolvedElectrostaticSystem.h"
#include "UnsolvedElectrostaticSystem.h"

namespace electrostatics {

/* Uses the finite difference method to solve an UnsolvedElectrostaticsSystem.
 * The result is saved in the SolvedElectrostaticsSystem which is also passed
 * to the finction.
 */

#ifdef IFiniteDiff
void finiteDifferenceSolve(UnsolvedElectrostaticSystem &unsolvedSystem, 
        SolvedElectrostaticSystem &solvedSystem, int maxIterations=10000);
#endif // IFiniteDiff

#ifndef IFiniteDiff
void finiteDifferenceSolve(const UnsolvedElectrostaticSystem &unsolvedSystem, 
        SolvedElectrostaticSystem &solvedSystem);
#endif // Not IFiniteDiff

} // namespace electrostatics

#endif
