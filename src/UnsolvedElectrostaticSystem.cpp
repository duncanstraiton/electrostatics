#include <Eigen/Dense>
#include <stdexcept>
#include "ElectrostaticSystem.h"
#include "UnsolvedElectrostaticSystem.h"

namespace electrostatics {

/* Constructors */

UnsolvedElectrostaticSystem::UnsolvedElectrostaticSystem(
        Eigen::MatrixXd &potentials,
        Eigen::Matrix<bool, Eigen::Dynamic, Eigen::Dynamic> &boundaryConditionPositions,
        int iMin, int jMin) :
    ElectrostaticSystem(potentials, iMin, jMin),
    boundaryConditionPositions(boundaryConditionPositions) {
}

UnsolvedElectrostaticSystem::UnsolvedElectrostaticSystem(int iMin, int iMax, int jMin, int jMax) :
    ElectrostaticSystem(iMin, iMax, jMin, jMax) {
        boundaryConditionPositions =
            Eigen::Matrix<bool, Eigen::Dynamic, Eigen::Dynamic>(potentials.rows(), potentials.cols());
        boundaryConditionPositions.fill(false);
}


/* Methods */

Eigen::Matrix<bool, Eigen::Dynamic, Eigen::Dynamic>* UnsolvedElectrostaticSystem::getBoundaryConditionPositions() {
    return &boundaryConditionPositions;
}

bool UnsolvedElectrostaticSystem::isBoundaryConditionIJ(int i, int j) const {
    if(i>iMax || i<iMin || j>jMax || j<jMin) throw std::out_of_range("Error: Trying to get element out of range!");
    return boundaryConditionPositions(i-iMin, j-jMin);
}
bool UnsolvedElectrostaticSystem::isBoundaryConditionK(long k) const {
    if(k>kMax || k<0) throw std::out_of_range("Error: Trying to get element out of range!");
    int* ij = k2ij(k);
    return boundaryConditionPositions(ij[0]-iMin, ij[1]-jMin);
}

void UnsolvedElectrostaticSystem::setBoundaryConditionIJ(int i, int j, bool isBoundaryCondition) {
    if(i>iMax || i<iMin || j>jMax || j<jMin) throw std::out_of_range("Error: Trying to set element out of range!");
    boundaryConditionPositions(i-iMin, j-jMin) = isBoundaryCondition;
}
void UnsolvedElectrostaticSystem::setBoundaryConditionK(long k, bool isBoundaryCondition) {
    if(k>kMax || k<0) throw std::out_of_range("Error: Trying to set element out of range!");
    int* ij = k2ij(k);
    boundaryConditionPositions(ij[0]-iMin, ij[1]-jMin) = isBoundaryCondition;
}

} // namespace electrostatics
