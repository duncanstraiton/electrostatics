#include <Eigen/Dense>
#include <stdexcept>
#include <cmath>
#include "ElectrostaticSystem.h"
#include "UnsolvedElectrostaticSystem.h"

namespace electrostatics {

/* Constructors */

UnsolvedElectrostaticSystem::UnsolvedElectrostaticSystem(
        doubleGrid &potentials, boolGrid &boundaryConditionPositions, int iMin, int jMin) :
    ElectrostaticSystem(potentials, iMin, jMin),
    boundaryConditionPositions(boundaryConditionPositions) {
}

UnsolvedElectrostaticSystem::UnsolvedElectrostaticSystem(int iMin, int iMax, int jMin, int jMax) :
    ElectrostaticSystem(iMin, iMax, jMin, jMax) {
        boundaryConditionPositions = boolGrid(potentials.rows(), potentials.cols());
        boundaryConditionPositions.fill(false);
}


/* Methods */

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

void UnsolvedElectrostaticSystem::setBoundaryPoint(int i, int j, double potential) {
    setBoundaryConditionIJ(i, j, true);
    setPotentialIJ(i, j, potential);
}

void UnsolvedElectrostaticSystem::setBoundaryRing(int centreI, int centreJ, double radius, double potential) {
    int iOffset, jOffset;
    for(iOffset=ceil(radius); iOffset>=0; iOffset--) {
        if(iOffset > radius) jOffset = 0;
        else jOffset = round(sqrt( pow(radius,2)-pow(iOffset,2) ));
        setBoundaryPoint(centreI+iOffset, centreJ+jOffset, potential);
        setBoundaryPoint(centreI+iOffset, centreJ-jOffset, potential);
        setBoundaryPoint(centreI-iOffset, centreJ+jOffset, potential);
        setBoundaryPoint(centreI-iOffset, centreJ-jOffset, potential);
    }
    for(jOffset=ceil(radius); jOffset>=0; jOffset--) {
        if(jOffset > radius) iOffset = 0;
        else iOffset = round(sqrt( std::pow(radius,2)-pow(jOffset,2) ));
        setBoundaryPoint(centreI+iOffset, centreJ+jOffset, potential);
        setBoundaryPoint(centreI+iOffset, centreJ-jOffset, potential);
        setBoundaryPoint(centreI-iOffset, centreJ+jOffset, potential);
        setBoundaryPoint(centreI-iOffset, centreJ-jOffset, potential);
    }
}

void UnsolvedElectrostaticSystem::setBoundaryCircle(int centreI, int centreJ, double radius, double potential) {
    for(int i=centreI-ceil(radius); i<=centreI+ceil(radius); i++) {
        for(int j=centreJ-ceil(radius); j<=centreJ+ceil(radius); j++) {
            if(sqrt( pow(i,2)+pow(j,2) ) <=radius) {
                setBoundaryPoint(i, j, potential);
            }
        }
    }
}

} // namespace electrostatics
