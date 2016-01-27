#include "finiteDifferenceSolve.h"
#include "SolvedElectrostaticSystem.h"
#include "UnsolvedElectrostaticSystem.h"
#include <iostream>
#include <cmath>
#include <string>
#include <cctype>

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
    std::string answer;    // To hold users response to yes/no questions
    std::string fileName;  // To hold file names input by the user
    double radius;         // To hold the radius of the current point while looping over points
    bool plotNumerical;    // True if numerical solution is plotted
    bool plotAnalytical;   // True if analytical solution is plotted

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

    // Create variables to store the solutions
    electrostatics::SolvedElectrostaticSystem solvedSystemNumerical(iMin, iMax, jMin, jMax);
    electrostatics::SolvedElectrostaticSystem systemAnalytical(iMin, iMax, jMin, jMax);

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


    // Solve and save plot data for the numerical solution if desired
    std::cout << "\nDo you want to solve numerically and save numerical solution plot data? (y/n): ";
    std::cin >> answer;
    plotNumerical = std::toupper(answer.front()) == 'Y';
    if(plotNumerical) {
        // Setup the unsolved system with the boundary conditions, and then solve it
        electrostatics::UnsolvedElectrostaticSystem unsolvedSystem(iMin, iMax, jMin, jMax);
        unsolvedSystem.setBoundaryCircle(0, 0, radiusA, potentialA);
        unsolvedSystem.setBoundaryRing(0, 0, radiusB, potentialB);
 
        electrostatics::finiteDifferenceSolve(unsolvedSystem, solvedSystemNumerical);

        std::cout << "Enter the file name for the numerical solution data: ";
        std::cin >> fileName;
        solvedSystemNumerical.saveFileGNUPlot(fileName);
    }


    // Save plot data for the analytical solution if desired
    std::cout << "\nDo you want to save analytical solution plot data? (y/n): ";
    std::cin >> answer;
    plotAnalytical = std::toupper(answer.front()) == 'Y';
    if(plotAnalytical) {
        // Calculate the potential at each point using the analytical solution
        systemAnalytical.setPotentialCircle(0, 0, radiusA, potentialA);
        for(int i=iMin; i<=iMax; i++) {
            for(int j=jMin; j<=jMax; j++) {
                radius = std::sqrt(std::pow(i, 2) + std::pow(j, 2));
                systemAnalytical.setPotentialIJ(i, j, potentialA + 
                        ((potentialB-potentialA) / std::log(radiusB/radiusA)) * std::log(radius/radiusA));
            }
        }

        std::cout << "Enter the file name for the analytical solution data: ";
        std::cin >> fileName;
        systemAnalytical.saveFileGNUPlot(fileName);
    }


    // Save plot data for the difference between analytical and numerical solutions if desired
    if(plotAnalytical && plotNumerical) {
        std::cout << "\nDo you want to save plot data for the difference between analytical" << 
            " and numerical soultions? (y/n): ";
        std::cin >> answer;
        if(std::toupper(answer.front()) == 'Y') {
            electrostatics::SolvedElectrostaticSystem solutionComparison(iMin, iMax, jMin, jMax);

            // Calculate the absolute value of the difference at each point
            for(int i=iMin; i<=iMax; i++) {
                for(int j=jMin; j<=jMax; j++) {
                    solutionComparison.setPotentialIJ(i, j, std::abs(solvedSystemNumerical.getPotentialIJ(i, j) - 
                                systemAnalytical.getPotentialIJ(i, j)));
                }
            }

            std::cout << "Enter the file name for the solution comparison data: ";
            std::cin >> fileName;
            solutionComparison.saveFileGNUPlot(fileName);
        }
    }
}


void solveProblem2() {
    /* TO DO! */
    std::cout << "No solution for system 2 yet.";
}
