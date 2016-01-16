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
void finiteDifferenceSolve(const UnsolvedElectrostaticSystem &unsolvedSystemi, 
        SolvedElectrostaticSystem &solvedSystem);

} // namespace electrostatics
#endif
