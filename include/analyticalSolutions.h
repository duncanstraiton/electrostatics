#ifndef ANALYTICALSOLUTIONS_H
#define ANALYTICALSOLUTIONS_H

#include <cmath>

namespace electrostatics {

/* Analytical solution for problem 1. A is the inner cylinder, B is the outer. */
double analyticalProblem1(int i, int j, double radiusA, double radiusB, double potentialA, double potentialB);

/* Analytical solution for problem 2. uniformField is the value of the field without the cylinder present. */
double analyticalProblem2(int i, int j, double cylinderRadius, double uniformField);

/* Function to find the uniform electric field between two (vertical) parallel plates. */
double uniformField(double leftPosition, double rightPosition, double leftPotential, double rightPotential);

} // namespace electrostatics
#endif
