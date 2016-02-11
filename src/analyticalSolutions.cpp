#include "analyticalSolutions.h"
#include <cmath>

namespace electrostatics {

double analyticalProblem1(int i, int j, double radiusA, double radiusB, double potentialA, double potentialB) {
    double radius = sqrt(pow(i, 2) + pow(j, 2));
    if(radius > radiusA && radius <= radiusB) {
        return potentialA + ((potentialB-potentialA) / log(radiusB/radiusA)) * log(radius/radiusA);
    }
    else if(radius > radiusB) {
        return potentialB;
    }
    else {
        return potentialA;
    }
}

double analyticalProblem2(int i, int j, double cylinderRadius, double uniformField) {
    double radius = sqrt(pow(i, 2) + pow(j, 2));
    if(radius > cylinderRadius) {
        double theta = (i==0)? M_PI/2 : atan((double)j/i);
        return ((i>=0)? 1 : -1) * uniformField * cos(theta) * ((pow(cylinderRadius, 2) / radius) - radius);
    }
    else {
        return 0;
    }
}

double uniformField(double leftPosition, double rightPosition, double leftPotential, double rightPotential) {
    return (leftPotential-rightPotential)/(rightPosition-leftPosition);
}

} // namespace electrostatics
