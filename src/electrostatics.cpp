#include "analyticalSolutions.h"
#include "SolvedElectrostaticSystem.h"
#include "UnsolvedElectrostaticSystem.h"
#include "finiteDiffMatrix.h"
#include "finiteDiffIterative.h"
#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <sstream>
#include <unordered_map>
#include <cstdlib>
#include <ctime>

void processLine(std::string &line, std::vector<std::string> &splitLine);

/* Parses a .cfg file
 */
int main(int argc, char* argv[]) {
    std::ofstream plotFile;
    std::ifstream configFile;
    configFile.open(argv[1]);
    if(!configFile) {
        std::cerr << "Error opening file...\n";
        return(1);
    }

    // Systems are indexed by names which the user enters
    std::unordered_map<std::string, electrostatics::UnsolvedElectrostaticSystem> unsolvedSystems;
    std::unordered_map<std::string, electrostatics::SolvedElectrostaticSystem> solvedSystems;
    std::string currentSystem;   // The currently selected (unsolved) system - the one being editied

    // Variable to hold times
    std::unordered_map<std::string, std::clock_t> timers;

    // Variables for reading a line
    std::string line;
    std::vector<std::string> splitLine;

    // Read in and process lines
    while(!configFile.eof()) {
        // Read a line and process it (split at spaces, convert to lowercase etc)
        std::getline(configFile, line);
        if(line == "") continue;        // Blank line
        if(line[0] == '#') continue;    // Comment line
        splitLine.clear();
        processLine(line, splitLine);

        // For creating a new system
        if(splitLine[0] == "new") {
            std::string name = splitLine[1];
            int iMin = std::stoi(splitLine[2]);
            int iMax = std::stoi(splitLine[3]);
            int jMin = std::stoi(splitLine[4]);
            int jMax = std::stoi(splitLine[5]);
            unsolvedSystems.emplace(name, electrostatics::UnsolvedElectrostaticSystem(iMin, iMax, jMin, jMax));
            currentSystem = name;
        }
            
        // For creating problem 1 analytical solution
        else if(splitLine[0] == "analytical1") {
            std::string name = splitLine[1];
            int iMin = std::stoi(splitLine[2]);
            int iMax = std::stoi(splitLine[3]);
            int jMin = std::stoi(splitLine[4]);
            int jMax = std::stoi(splitLine[5]);
            solvedSystems.emplace(name, electrostatics::SolvedElectrostaticSystem(iMin, iMax, jMin, jMax));
            for(int i=iMin; i<= iMax; i++) {
                for(int j=jMin; j<=jMax; j++) {
                    double potential = electrostatics::analyticalProblem1(i, j, std::stod(splitLine[6]),
                            std::stod(splitLine[7]), std::stod(splitLine[8]), std::stod(splitLine[9]));
                    solvedSystems.at(name).setPotentialIJ(i, j, potential);
                }
            }
        }
        // For creating problem 2 analytical solution
        else if(splitLine[0] == "analytical2") {
            std::string name = splitLine[1];
            int iMin = std::stoi(splitLine[2]);
            int iMax = std::stoi(splitLine[3]);
            int jMin = std::stoi(splitLine[4]);
            int jMax = std::stoi(splitLine[5]);
            solvedSystems.emplace(name, electrostatics::SolvedElectrostaticSystem(iMin, iMax, jMin, jMax));
            double uniformField = electrostatics::uniformField(iMin, iMax, std::stod(splitLine[6]),
                    std::stod(splitLine[7]));
            for(int i=iMin; i<= iMax; i++) {
                for(int j=jMin; j<=jMax; j++) {
                    double potential = electrostatics::analyticalProblem2(i, j, std::stod(splitLine[8]), 
                            uniformField);
                    solvedSystems.at(name).setPotentialIJ(i, j, potential);
                }
            }
        }

        // For adding boundary conditions
        else if(splitLine[0] == "point") {
            unsolvedSystems.at(currentSystem).setBoundaryPoint(std::stoi(splitLine[1]), std::stoi(splitLine[2]),
                    std::stod(splitLine[3]));
        }
        else if(splitLine[0] == "ring") {
            unsolvedSystems.at(currentSystem).setBoundaryRing(std::stoi(splitLine[1]), std::stoi(splitLine[2]),
                    std::stod(splitLine[3]), std::stod(splitLine[4]));
        }
        else if(splitLine[0] == "circle") {
            unsolvedSystems.at(currentSystem).setBoundaryCircle(std::stoi(splitLine[1]), std::stoi(splitLine[2]),
                    std::stod(splitLine[3]), std::stod(splitLine[4]));
        }
        else if(splitLine[0] == "line") {
            unsolvedSystems.at(currentSystem).setBoundaryLine(std::stoi(splitLine[1]), std::stoi(splitLine[2]),
                    std::stoi(splitLine[3]), std::stoi(splitLine[4]), std::stod(splitLine[5]));
        }
        else if(splitLine[0] == "left") {
            unsolvedSystems.at(currentSystem).setLeftBoundary(std::stod(splitLine[1]));
        }
        else if(splitLine[0] == "right") {
            unsolvedSystems.at(currentSystem).setRightBoundary(std::stod(splitLine[1]));
        }
        else if(splitLine[0] == "top") {
            unsolvedSystems.at(currentSystem).setTopBoundary(std::stod(splitLine[1]));
        }
        else if(splitLine[0] == "bottom") {
            unsolvedSystems.at(currentSystem).setBottomBoundary(std::stod(splitLine[1]));
        }
        else if(splitLine[0] == "rectangle") {
            unsolvedSystems.at(currentSystem).setBoundaryRectangle(std::stoi(splitLine[1]), std::stoi(splitLine[2]),
                    std::stoi(splitLine[3]), std::stoi(splitLine[4]), std::stod(splitLine[5]));
        }

        // For solving with different methods
        else if(splitLine[0] == "solveviennabicon") {
            int iMin = unsolvedSystems.at(splitLine[1]).getIMin();
            int iMax = unsolvedSystems.at(splitLine[1]).getIMax();
            int jMin = unsolvedSystems.at(splitLine[1]).getJMin();
            int jMax = unsolvedSystems.at(splitLine[1]).getJMax();
            solvedSystems.emplace(splitLine[2], electrostatics::SolvedElectrostaticSystem(iMin, iMax, jMin, jMax));
            electrostatics::finiteDiffMatrix(unsolvedSystems.at(splitLine[1]),
                    solvedSystems.at(splitLine[2]), "viennabicon");
        }
        else if(splitLine[0] == "solveeigenbicon") {
            int iMin = unsolvedSystems.at(splitLine[1]).getIMin();
            int iMax = unsolvedSystems.at(splitLine[1]).getIMax();
            int jMin = unsolvedSystems.at(splitLine[1]).getJMin();
            int jMax = unsolvedSystems.at(splitLine[1]).getJMax();
            solvedSystems.emplace(splitLine[2], electrostatics::SolvedElectrostaticSystem(iMin, iMax, jMin, jMax));
            electrostatics::finiteDiffMatrix(unsolvedSystems.at(splitLine[1]),
                    solvedSystems.at(splitLine[2]), "eigenbicon");
        }
        else if(splitLine[0] == "solveeigensparselu") {
            int iMin = unsolvedSystems.at(splitLine[1]).getIMin();
            int iMax = unsolvedSystems.at(splitLine[1]).getIMax();
            int jMin = unsolvedSystems.at(splitLine[1]).getJMin();
            int jMax = unsolvedSystems.at(splitLine[1]).getJMax();
            solvedSystems.emplace(splitLine[2], electrostatics::SolvedElectrostaticSystem(iMin, iMax, jMin, jMax));
            electrostatics::finiteDiffMatrix(unsolvedSystems.at(splitLine[1]),
                    solvedSystems.at(splitLine[2]), "eigensparselu");
        }
        else if(splitLine[0] == "solveiterative") {
            int iMin = unsolvedSystems.at(splitLine[1]).getIMin();
            int iMax = unsolvedSystems.at(splitLine[1]).getIMax();
            int jMin = unsolvedSystems.at(splitLine[1]).getJMin();
            int jMax = unsolvedSystems.at(splitLine[1]).getJMax();
            solvedSystems.emplace(splitLine[2], electrostatics::SolvedElectrostaticSystem(iMin, iMax, jMin, jMax));
            electrostatics::finiteDiffIterative(unsolvedSystems.at(splitLine[1]),
                    solvedSystems.at(splitLine[2]), std::stoi(splitLine[3]));
        }

        // For comparisons and outputing results
        else if(splitLine[0] == "savesolution") {
            solvedSystems.at(splitLine[1]).saveFile(splitLine[1]);
        }
        // Would be better to implement a method for this with saving directly to a file if there is time
        else if(splitLine[0] == "savecomparison") {
            std::string nameA = splitLine[1];
            std::string nameB = splitLine[2];
            int iMin = solvedSystems.at(nameA).getIMin();
            int iMax = solvedSystems.at(nameA).getIMax();
            int jMin = solvedSystems.at(nameA).getJMin();
            int jMax = solvedSystems.at(nameA).getJMax();
            electrostatics::SolvedElectrostaticSystem comparisonResult(iMin, iMax, jMin, jMax);
            solvedSystems.at(nameA).compareTo(solvedSystems.at(nameB), comparisonResult);
            comparisonResult.saveFile(splitLine[3]);
        }
        else if(splitLine[0] == "savefield") {
            solvedSystems.at(splitLine[1]).saveFieldGNUPlot(splitLine[1] + "field");
        }

        // For timers
        else if(splitLine[0] == "starttimer") {
            timers.emplace(splitLine[1], std::clock());
        }
        else if(splitLine[0] == "stoptimer") {
            double timeElapsed = double(clock() - timers.at(splitLine[1])) / CLOCKS_PER_SEC;
            std::cout << "CPU time elapsed for " << splitLine[1] << ": " << timeElapsed << "s\n";
        }

        // Setup a new plot file
        else if(splitLine[0] == "plotfile") {
            plotFile.open(splitLine[1].c_str());
            plotFile <<
                "#!/#!/usr/bin/gnuplot -persist\n"
                "\n"
                "set style line 1 lt 1 lc rgb \"red\"\n"
                "set palette defined ( 0 '#FFFFD9', 1 '#EDF8B1', 2 '#C7E9B4', 3 '#7FCDBB',\\\n"
                "                      4 '#41B6C4', 5 '#1D91C0', 6 '#225EA8', 7 '#0C2C84' ) \n"
                "\n"
                "set size ratio -1\n"
                "set term postscript color\n"
                "set pm3d map\n"
                "set xlabel \"i\"\n"
                "set ylabel \"j\"\n"
                "set nokey\n"
                "\n"
                "xMin = " << splitLine[2] << "\n"
                "xMax = " << splitLine[3] << "\n"
                "yMin = " << splitLine[4] << "\n"
                "yMax = " << splitLine[5] << "\n"
                "set xrange [xMin : xMax];\n"
                "set yrange [yMin : yMax];\n"
                "\n";
        }
        else if(splitLine[0] == "plot") {
                plotFile <<
                "set title \"" << splitLine[1] << "\"\n"
                "set output \"" << splitLine[1] + ".eps" << "\"\n"
                "splot \"" << splitLine[1] << "\" using ($1+xMin):($2+yMin):3 matrix\n"
                "\n";
        }
        else if(splitLine[0] == "fieldplot") {
            double arrowSpacing = std::stod(splitLine[2]);
            double arrowScaling = 0.85 * arrowSpacing;
            plotFile <<
                "set title \"" << splitLine[1] << "\"\n"
                "set output \"" << splitLine[1] + "field.eps" << "\"\n"
                "splot \"" << splitLine[1] << "\" using ($1+xMin):($2+yMin):3 matrix, \\\n"
                "\"" << splitLine[1] + "field" << "\" every " << arrowSpacing << ":" << arrowSpacing << 
                " using ($1):($2):(0.0):($3*" << arrowScaling << "):($4*" << arrowScaling <<
                "):(0.0) with vectors\n"
                "\n";
        }
        else if(splitLine[0] == "contourplot") {
            plotFile <<
                "set contour base\n"
                "set cntrparam levels auto\n"
                "unset clabel\n"
                "set output \"" << splitLine[1] + "contour.eps" << "\"\n"
                "splot \"" << splitLine[1] << "\" using ($1+xMin):($2+yMin):3 matrix ls 1 lw 3\n"
                "\n";
        }
    }
    configFile.close();
    if(plotFile) plotFile.close();
}


/* Process a line by splitting it at spaces, converting to lowercase, and saving 
 * each part in a vector.
 */
void processLine(std::string &line, std::vector<std::string> &splitLine) {
    std::transform(line.begin(), line.end(), line.begin(), ::tolower);
    std::istringstream lineStream(line);
    std::string word;
    while(std::getline(lineStream, word, ' ')) splitLine.push_back(word);
}
