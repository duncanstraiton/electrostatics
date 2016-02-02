#include "finiteDifferenceSolve.h"
#include "SolvedElectrostaticSystem.h"
#include "UnsolvedElectrostaticSystem.h"
#include <iostream>
#include <cmath>

//double problem1analytical();
//double problem2analytical();
void solveProblem1();
void solveProblem2();

/* Currently asks the user to select problem 1 or 2 and to input the relevant paramters.
 * The output (plot data for the analytical solution, plot data for the numerical solution, 
 * and plot data for the difference between the two solutions) is saved to the specified files.
 *
 * It will need to be generalised / changed to allow solving of arbitrary electrostatic
 * systems.
 */
int main() {
    int problemNumber;

    std::cout << "\nWhich system would you like to generate plot data for?\n" <<
        "1. The system with the two concentical cylinders.\n" <<
        "2. The system with the cylinder between two plates.\n" <<
        "Enter '1' or '2': ";
    std::cin >> problemNumber;
    std::cout << "\n";

    switch(problemNumber) {
        case 1:
            solveProblem1();
            break;
        case 2:
            solveProblem2();
            break;
    }
    std::cout << "\n";
    return 0;
}


void solveProblem1() {
    double radius;         // To hold the radius of the current point while looping over points

    std::cout << "Solving system 1 using grid size ixj.\n";

    // Get the dimensions of the problem
    int iMin;
    int iMax;
    int jMin;
    int jMax;
    std::cout <<"Enter minimum value for i: ";
    std::cin >> iMin;
    std::cout <<"Enter maximum value for i: ";
    std::cin >> iMax;
    std::cout <<"Enter minimum value for j: ";
    std::cin >> jMin;
    std::cout << "Enter maximum value for j: ";
    std::cin >> jMax;

    // Get the necessary parameters - inner cylinder is A, outer is B
    double radiusA;
    double potentialA;
    double radiusB;
    double potentialB;
    std::cout << "Enter radius for inner cylinder: ";
    std::cin >> radiusA;
    std::cout << "Enter potential for inner cylinder: ";
    std::cin >> potentialA; 
    std::cout << "Enter radius for outer cylinder: ";
    std::cin >> radiusB;
    std::cout << "Enter potential for outer cylinder: ";
    std::cin >> potentialB;


    // Numerical Solution
    // Setup the unsolved system with the boundary conditions, and then solve it
    electrostatics::UnsolvedElectrostaticSystem unsolvedSystem(iMin, iMax, jMin, jMax);
    unsolvedSystem.setBoundaryCircle(0, 0, radiusA, potentialA);
    unsolvedSystem.setBoundaryRing(0, 0, radiusB, potentialB);

    electrostatics::SolvedElectrostaticSystem solvedSystemNumerical(iMin, iMax, jMin, jMax);
    electrostatics::finiteDifferenceSolve(unsolvedSystem, solvedSystemNumerical);
    solvedSystemNumerical.saveFileGNUPlot("numericalProblem1");


    // Analytical solution
    electrostatics::SolvedElectrostaticSystem systemAnalytical(iMin, iMax, jMin, jMax);
    // Calculate the potential at each point using the analytical solution
    systemAnalytical.setPotentialCircle(0, 0, radiusA, potentialA);
    for(int i=iMin; i<=iMax; i++) {
        for(int j=jMin; j<=jMax; j++) {
            radius = std::sqrt(std::pow(i, 2) + std::pow(j, 2));
            if(radius > radiusA && radius <= radiusB) {
                systemAnalytical.setPotentialIJ(i, j, potentialA + 
                        ((potentialB-potentialA) / std::log(radiusB/radiusA)) * std::log(radius/radiusA));
            }
            else if(radius > radiusB) {
                systemAnalytical.setPotentialIJ(i, j, potentialB);
            }
        }
    }
    systemAnalytical.saveFileGNUPlot("analyticalProblem1");


    // Difference between analytical and numerical solutions
    electrostatics::SolvedElectrostaticSystem solutionComparison(iMin, iMax, jMin, jMax);
    // Calculate the absolute value of the difference at each point
    for(int i=iMin; i<=iMax; i++) {
        for(int j=jMin; j<=jMax; j++) {
            solutionComparison.setPotentialIJ(i, j, std::abs(solvedSystemNumerical.getPotentialIJ(i, j) - 
                        systemAnalytical.getPotentialIJ(i, j)));
        }
    }
    solutionComparison.saveFileGNUPlot("differenceProblem1");
}


void solveProblem2() {
    // Get the dimensions of the problem
    int iMin;
    int iMax;
    int jMin;
    int jMax;
    std::cout <<"Enter minimum value for i: ";
    std::cin >> iMin;
    std::cout <<"Enter maximum value for i: ";
    std::cin >> iMax;
    std::cout <<"Enter minimum value for j: ";
    std::cin >> jMin;
    std::cout << "Enter maximum value for j: ";
    std::cin >> jMax;

    // Get the necessary parameters
    double cylinderRadius;
    double cylinderPotential;
    double leftPotential;
    double rightPotential;
    std::cout << "Enter radius for cylinder: ";
    std::cin >> cylinderRadius;
    std::cout << "Enter potential for cylinder: ";
    std::cin >> cylinderPotential; 
    std::cout << "Enter potential for left plate: ";
    std::cin >> leftPotential;
    std::cout << "Enter potential for right plate: ";
    std::cin >> rightPotential;


    // Numerical Solution
    // Setup the unsolved system with the boundary conditions, and then solve it
    electrostatics::UnsolvedElectrostaticSystem unsolvedSystem(iMin, iMax, jMin, jMax);
    unsolvedSystem.setBoundaryCircle(0, 0, cylinderRadius, cylinderPotential);
    for(int j=jMin; j<=jMax; j++) {
        unsolvedSystem.setBoundaryPoint(iMin, j, leftPotential);
        unsolvedSystem.setBoundaryPoint(iMax, j, rightPotential);
    }

    electrostatics::SolvedElectrostaticSystem solvedSystemNumerical(iMin, iMax, jMin, jMax);
    electrostatics::finiteDifferenceSolve(unsolvedSystem, solvedSystemNumerical);
    solvedSystemNumerical.saveFileGNUPlot("numericalProblem2");

    // Analytical solution
    electrostatics::SolvedElectrostaticSystem systemAnalytical(iMin, iMax, jMin, jMax);
    // Calculate the potential at each point using the analytical solution
    systemAnalytical.setPotentialCircle(0, 0, cylinderRadius, cylinderPotential);
    double field = (rightPotential-leftPotential)/(iMax-iMin);
    for(int i=iMin; i<=iMax; i++) {
        for(int j=jMin; j<=jMax; j++) {
            double radius = sqrt(i*i + j*j);
            double theta;
            int sign;
            if(i>0) sign = 1;
            else sign = -1;
            if(i==0) theta = M_PI / 2;
            else theta = atan((double)j/i);
            if(radius > cylinderRadius) {
                systemAnalytical.setPotentialIJ(i, j, field * sign *
                    (radius - (pow(cylinderRadius, 2) / radius)) *
                     cos(theta));
            }
        }
    }
    systemAnalytical.saveFileGNUPlot("analyticalProblem2");


    // Difference between analytical and numerical solutions
    electrostatics::SolvedElectrostaticSystem solutionComparison(iMin, iMax, jMin, jMax);
    // Calculate the absolute value of the difference at each point
    for(int i=iMin; i<=iMax; i++) {
        for(int j=jMin; j<=jMax; j++) {
            solutionComparison.setPotentialIJ(i, j, std::abs(solvedSystemNumerical.getPotentialIJ(i, j) - 
                        systemAnalytical.getPotentialIJ(i, j)));
        }
    }
    solutionComparison.saveFileGNUPlot("differenceProblem2");
}
