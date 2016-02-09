#include <Eigen/Dense>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "ElectrostaticSystem.h"

namespace electrostatics {

/* Constructor */

ElectrostaticSystem::ElectrostaticSystem(int iMin, int iMax, int jMin, int jMax) :
    iMin(iMin), iMax(iMax), jMin(jMin), jMax(jMax) {
        potentials = doubleGrid::Zero(iMax-iMin+1, jMax-jMin+1);
        kMax = ((iMax-iMin+1) * (jMax-jMin+1)) - 1;
}


/* Methods */

double ElectrostaticSystem::getPotentialIJ(int i, int j) const {
    if(i>iMax || i<iMin || j>jMax || j<jMin) throw std::out_of_range(
            "Error: Trying to get element out of range!");
    return potentials(i-iMin, j-jMin);
}
double ElectrostaticSystem::getPotentialK(long k) const {
    if(k>kMax || k<0) throw std::out_of_range("Error: Trying to get element out of range!");
    int* ij = k2ij(k);
    return potentials(ij[0]-iMin, ij[1]-jMin);
}

void ElectrostaticSystem::setPotentialIJ(int i, int j, double potential) {
    if(i>iMax || i<iMin || j>jMax || j<jMin) throw std::out_of_range(
            "Error: Trying to set element out of range!");
    potentials(i-iMin, j-jMin) = potential;
}
void ElectrostaticSystem::setPotentialK(long k, double potential) {
    if(k>kMax || k<0) throw std::out_of_range("Error: Trying to set element out of range!");
    int* ij = k2ij(k);
    potentials(ij[0]-iMin, ij[1]-jMin) = potential;
}

long ElectrostaticSystem::ij2k(int i, int j) const {
    if(i>iMax || i<iMin || j>jMax || j<jMin) throw std::out_of_range(
            "Error: Trying to convert to position (k) out of range!");
    i = i - iMin;
    j = j - jMin;
    return i + j*(iMax-iMin+1);
}

void ElectrostaticSystem::setPotentialRing(int centreI, int centreJ, double radius, double potential) {
    int iOffset, jOffset;
    for(iOffset=ceil(radius); iOffset>=0; iOffset--) {
        if(iOffset > radius) jOffset = 0;
        else jOffset = round(sqrt( pow(radius,2)-pow(iOffset,2) ));
        setPotentialIJ(centreI+iOffset, centreJ+jOffset, potential);
        setPotentialIJ(centreI+iOffset, centreJ-jOffset, potential);
        setPotentialIJ(centreI-iOffset, centreJ+jOffset, potential);
        setPotentialIJ(centreI-iOffset, centreJ-jOffset, potential);
    }
    for(jOffset=ceil(radius); jOffset>=0; jOffset--) {
        if(jOffset > radius) iOffset = 0;
        else iOffset = round(sqrt( pow(radius,2)-pow(jOffset,2) ));
        setPotentialIJ(centreI+iOffset, centreJ+jOffset, potential);
        setPotentialIJ(centreI+iOffset, centreJ-jOffset, potential);
        setPotentialIJ(centreI-iOffset, centreJ+jOffset, potential);
        setPotentialIJ(centreI-iOffset, centreJ-jOffset, potential);
    }
}

void ElectrostaticSystem::setPotentialCircle(int centreI, int centreJ, double radius, double potential) {
    for(int i=centreI-ceil(radius); i<=centreI+ceil(radius); i++) {
        for(int j=centreJ-ceil(radius); j<=centreJ+ceil(radius); j++) {
            if( sqrt( pow(i,2)+pow(j,2) ) <= radius) {
                setPotentialIJ(i, j, potential);
            }
        }
    }
}

int* ElectrostaticSystem::k2ij(long k) const {
    if(k>kMax || k<0) throw std::out_of_range(
            "Error: Trying to convert to position (k)  out of range!");
    int* ij = new int[2];
    int j = k / (iMax-iMin+1);
    int i = k - j*(iMax-iMin+1);
    ij[0] = i + iMin;
    ij[1] = j + jMin;
    return ij;
}

/* To convert from the matrix representaion to the grid representation,
 * transpose the matrix and reverse each column
 */
void ElectrostaticSystem::print() const {
    std::cout << potentials.transpose().colwise().reverse();
}

void ElectrostaticSystem::printGNUPlot() const {
    for(int i=iMin; i<=iMax; i++) {
        for(int j=jMin; j<=jMax; j++) {
            std::cout << i << " " << j << " " << getPotentialIJ(i, j) << "\n";
        }
        std::cout << "\n";
    }
}

void ElectrostaticSystem::saveFileGNUPlot(std::string fileName) const {
    std::ofstream outputFile;
    outputFile.open(fileName.c_str());
    for(int i=iMin; i<=iMax; i++) {
        for(int j=jMin; j<=jMax; j++) {
            outputFile << i << " " << j << " " << getPotentialIJ(i, j) << "\n";
        }
        outputFile << "\n";
    }
    outputFile.close();
}

void ElectrostaticSystem::compareTo(const ElectrostaticSystem &otherSystem,
        ElectrostaticSystem &comparisonResults) {
    if(this->getIMin() != otherSystem.getIMin() ||
            this->getIMax() != otherSystem.getIMax() ||
            this->getJMin() != otherSystem.getJMin() ||
            this->getJMax() != otherSystem.getJMax() ||
            this->getIMin() != comparisonResults.getIMin() ||
            this->getIMax() != comparisonResults.getIMax()) {
        throw std::invalid_argument(
                "The dimensions of both systems and the system for the results must match!");
    }

    for(int i=iMin; i<=iMax; i++) {
        for(int j=jMin; j<jMax; j++) {
            comparisonResults.setPotentialIJ(i, j, fabs(this->getPotentialIJ(i,j) -
                        otherSystem.getPotentialIJ(i, j)));
        }
    }
}







} // namespace electrostatics
