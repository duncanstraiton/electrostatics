#ifndef FINITEDIFFMATRIX_H
#define FINITEDIFFMATRIX_H

#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <string>
#include "SolvedElectrostaticSystem.h"
#include "UnsolvedElectrostaticSystem.h"

namespace electrostatics {

/* Uses the finite difference method to solve an UnsolvedElectrostaticsSystem.
 * The result is saved in the SolvedElectrostaticsSystem which is also passed
 * to the finction. This header is used for all methods that use a matrix of 
 * simultanious equations to solve the system.
 *
 * Method can be the following:
 * "eigenbicon" - Biconjugate gradient stabalized method from eigen
 * "eigensparselu" - Eigen sparse LU module
 * "viennabicon" - Biconjugate gradient method from vienna library - (will run
 * on gpu if opencl or cuda flag is set and required libraries are installed)
 */

void finiteDiffMatrix(const UnsolvedElectrostaticSystem &unsolvedSystem, 
        SolvedElectrostaticSystem &solvedSystem, std::string method);

} // namespace electrostatics

#endif
