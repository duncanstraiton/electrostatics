#ifndef FINITEDIFFITERATIVE_H
#define FINITEDIFFITERATIVE_H

#include <Eigen/Dense>
#include <Eigen/Sparse>
#include "SolvedElectrostaticSystem.h"
#include "UnsolvedElectrostaticSystem.h"

namespace electrostatics {

/* Uses the finite difference method to solve an UnsolvedElectrostaticsSystem.
 * The result is saved in the SolvedElectrostaticsSystem which is also passed
 * to the finction.
 */

void finiteDiffIterative(const UnsolvedElectrostaticSystem &unsolvedSystem, 
        SolvedElectrostaticSystem &solvedSystem, int maxIterations=10000);

} // namespace electrostatics

#endif
