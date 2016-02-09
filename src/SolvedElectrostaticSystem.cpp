#include <Eigen/Dense>
#include <stdexcept>
#include "ElectrostaticSystem.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include "SolvedElectrostaticSystem.h"

namespace electrostatics {

/* Constructors */

SolvedElectrostaticSystem::SolvedElectrostaticSystem(
        doubleGrid &potentials, int iMin, int jMin) :
    ElectrostaticSystem(potentials, iMin, jMin) {
        fieldFound = false;
}

SolvedElectrostaticSystem::SolvedElectrostaticSystem(int iMin, int iMax, int jMin, int jMax) :
    ElectrostaticSystem(iMin, iMax, jMin, jMax) {
        fieldFound = false;
}


/* Methods */

void SolvedElectrostaticSystem::findField() {
    fieldX = doubleGrid::Zero(iMax-iMin+1, jMax-jMin+1);
    fieldY = doubleGrid::Zero(iMax-iMin+1, jMax-jMin+1);
    field = doubleGrid::Zero(iMax-iMin+1, jMax-jMin+1);
    for(int i=iMin; i<=iMax; i++) {
        for(int j=jMin; j<=jMax; j++) {

            // Components in i direction
            int surroundingPointsX = 0;
            if(i<iMax) {
                fieldX(i-iMin, j-jMin) += getPotentialIJ(i+1, j);
                surroundingPointsX += 1;
            } else fieldX(i-iMin, j-jMin) += getPotentialIJ(i, j);
            if(i>iMin) {
                fieldX(i-iMin, j-jMin) -= getPotentialIJ(i-1, j);
                surroundingPointsX += 1;
            } else fieldX(i-iMin, j-jMin) -= getPotentialIJ(i, j);
            fieldX(i-iMin, j-jMin) /= surroundingPointsX;

            // Components in j direction
            int surroundingPointsY = 0;
            if(j<jMax) {
                fieldY(i-iMin, j-jMin) += getPotentialIJ(i, j+1);
                surroundingPointsY += 1;
            } else fieldY(i-iMin, j-jMin) += getPotentialIJ(i, j);
            if(j>jMin) {
                fieldY(i-iMin, j-jMin) -= getPotentialIJ(i, j-1);
                surroundingPointsY += 1;
            } else fieldY(i-iMin, j-jMin) -= getPotentialIJ(i, j);
            fieldY(i-iMin, j-jMin) /= surroundingPointsY;

            field(i-iMin, j-jMin) = sqrt( pow(fieldX(i-iMin, j-jMin),2) + pow(fieldY(i-iMin,j-jMin),2) );
            if(field(i-iMin, j-jMin) > maxE) maxE = field(i-iMin, j-jMin);

            fieldFound = true;
        }
    }
}

void SolvedElectrostaticSystem::saveFieldGNUPlot(std::string fileName) {
    if(!fieldFound) findField();
    std::ofstream outputFile;
    outputFile.open(fileName.c_str());
    for(int i=iMin; i<=iMax; i++) {
        for(int j=jMin; j<=jMax; j++) {
            outputFile << i << " " << j << " " << field(i-iMin, j-jMin) << "\n";
        }
        outputFile << "\n";
    }
    outputFile.close();
}

} // namespace electrostatics
