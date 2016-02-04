#include <Eigen/Dense>
#include <stdexcept>
#include "ElectrostaticSystem.h"
#include "SolvedElectrostaticSystem.h"

namespace electrostatics {

/* Constructors */

SolvedElectrostaticSystem::SolvedElectrostaticSystem(
        doubleGrid &potentials, int iMin, int jMin) :
    ElectrostaticSystem(potentials, iMin, jMin) {
}

SolvedElectrostaticSystem::SolvedElectrostaticSystem(int iMin, int iMax, int jMin, int jMax) :
    ElectrostaticSystem(iMin, iMax, jMin, jMax) {
}

} // namespace electrostatics
