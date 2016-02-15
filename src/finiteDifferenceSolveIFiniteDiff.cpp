#ifdef IFiniteDiff`

#include <Eigen/Dense>
#include <Eigen/Sparse>
#include "finiteDifferenceSolve.h"
#include "SolvedElectrostaticSystem.h"
#include "UnsolvedElectrostaticSystem.h"

namespace electrostatics {

/* Iterative finite difference method.
 */
void finiteDifferenceSolve(const UnsolvedElectrostaticSystem &unsolvedSystem,
        SolvedElectrostaticSystem &solvedSystem) {
}

} // namespace electrostatics

#endif // IFiniteDiff
