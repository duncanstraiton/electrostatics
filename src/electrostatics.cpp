#include "finiteDifferenceSolve.h"
#include "analyticalSolutions.h"
#include "SolvedElectrostaticSystem.h"
#include "UnsolvedElectrostaticSystem.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <cstdlib>
using namespace std;

//double problem1analytical();
//double problem2analytical();
void solveProblem1();
void solveProblem2();
void solveProblem3();
void solveProblem4(string cfgName);
string nocaps(string input);
/* Currently asks the user to select problem 1 or 2 and to input the relevant paramters.
 * The output (plot data for the analytical solution, plot data for the numerical solution, 
 * and plot data for the difference between the two solutions) is saved to the specified files.
 *
 * It will need to be generalised / changed to allow solving of arbitrary electrostatic
 * systems.
 */
int main(int argc, char *argv[]) {
  
   string cfgName = argv[1];
  
    int problemNumber = 4;

  /* std::cout << "\nWhich system would you like to generate plot data for?\n" <<
      "1. The system with the two concentical cylinders.\n" <<
      "2. The system with the cylinder between two plates.\n" <<
      "3. The Multi-wire proportional Chamber. \n" <<
      "4. Usa a config file. \n" <<
      "Enter '1', '2' or '3': ";
    std::cin >> problemNumber;
    std::cout << "\n";*/


    switch(problemNumber) {
    case 1:
      solveProblem1();
      break;
    case 2:
      solveProblem2();
      break;
    case 3:
      solveProblem3();
      break;
    case 4:
      solveProblem4(cfgName);
      break;
    }
    std::cout << "\n";
    return 0;
}

void solveProblem1(){
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
    solvedSystemNumerical.saveFile("numericalProblem1");


    // Analytical solution
    electrostatics::SolvedElectrostaticSystem systemAnalytical(iMin, iMax, jMin, jMax);
    for(int i=iMin; i<=iMax; i++) {
        for(int j=jMin; j<=jMax; j++) {
            systemAnalytical.setPotentialIJ(i, j,
                    electrostatics::analyticalProblem1(i, j, radiusA, radiusB, potentialA, potentialB));
        }
    }
    systemAnalytical.saveFile("analyticalProblem1");


    // Difference between analytical and numerical solutions
    electrostatics::SolvedElectrostaticSystem solutionComparison(iMin, iMax, jMin, jMax);
    solvedSystemNumerical.compareTo(systemAnalytical, solutionComparison);
    solutionComparison.saveFile("differenceProblem1");
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
    cylinderPotential = 0;
    std::cout << "Enter potential for left plate: ";
    std::cin >> leftPotential;
    std::cout << "Enter potential for right plate: ";
    std::cin >> rightPotential;


    // Numerical Solution
    // Setup the unsolved system with the boundary conditions, and then solve it
    electrostatics::UnsolvedElectrostaticSystem unsolvedSystem(iMin, iMax, jMin, jMax);
    unsolvedSystem.setBoundaryCircle(0, 0, cylinderRadius, cylinderPotential);
    unsolvedSystem.setLeftBoundary(leftPotential);
    unsolvedSystem.setRightBoundary(rightPotential);

    electrostatics::SolvedElectrostaticSystem solvedSystemNumerical(iMin, iMax, jMin, jMax);
    electrostatics::finiteDifferenceSolve(unsolvedSystem, solvedSystemNumerical);
    solvedSystemNumerical.saveFile("numericalProblem2");

    // Analytical solution
    electrostatics::SolvedElectrostaticSystem systemAnalytical(iMin, iMax, jMin, jMax);
    // Calculate the potential at each point using the analytical solution
    double uniformField =electrostatics::uniformField(iMin, iMax, leftPotential, rightPotential);
    for(int i=iMin; i<=iMax; i++) {
        for(int j=jMin; j<=jMax; j++) {
            systemAnalytical.setPotentialIJ(i, j, 
                    electrostatics::analyticalProblem2(i, j, cylinderRadius, uniformField));
        }
    }
    systemAnalytical.saveFile("analyticalProblem2");


    // Difference between analytical and numerical solutions
    electrostatics::SolvedElectrostaticSystem solutionComparison(iMin, iMax, jMin, jMax);
    solvedSystemNumerical.compareTo(systemAnalytical, solutionComparison);
    solutionComparison.saveFile("differenceProblem2");
}

void solveProblem3() {
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
    double topPotential;
    double bottomPotential;
    double cylinderSpacing;
    int numCylinder;
    std::cout << "Enter radii for the wires: ";
    std::cin >> cylinderRadius;
    cylinderPotential = 0;
    std::cout << "Enter the total number of wires: ";
    std::cin >> numCylinder; //----------------------------
    std::cout << "Enter the spacing between the wires: ";
    std::cin >> cylinderSpacing;
    std::cout << "Enter potential for the top  plate: ";
    std::cin >> topPotential;
    std::cout << "Enter potential for the bottom plate: ";
    std::cin >> bottomPotential;



    // Numerical Solution
    // Setup the unsolved system with the boundary conditions, and then solve it
    electrostatics::UnsolvedElectrostaticSystem unsolvedSystem(iMin, iMax, jMin, jMax);
    unsolvedSystem.setBoundaryCircle(0, 0, cylinderRadius, cylinderPotential);
    for (int k = 0; k < (numCylinder-1)/2; k++){
      unsolvedSystem.setBoundaryCircle(cylinderSpacing, 0, cylinderRadius, cylinderPotential);
      unsolvedSystem.setBoundaryCircle(-cylinderSpacing, 0, cylinderRadius, cylinderPotential);
      cylinderSpacing += cylinderSpacing;
    }
    unsolvedSystem.setTopBoundary(topPotential);
    unsolvedSystem.setBottomBoundary(bottomPotential);

    electrostatics::SolvedElectrostaticSystem solvedSystemNumerical(iMin, iMax, jMin, jMax);
    electrostatics::finiteDifferenceSolve(unsolvedSystem, solvedSystemNumerical);
    solvedSystemNumerical.saveFile("numericalProblem3");

    //Edit GNUPlot file to handle user-set range and problem number
    ofstream plotfile("../scripts/plotProblemGeneric.plt");
    if (plotfile.is_open())
      {
	plotfile << "#!/usr/bin/gnuplot -persist \n";
	plotfile << "load '../scripts/colormap.pal' \n";
	plotfile << "set size ratio -1 \n";
	plotfile << "set term postscript color \n";
	plotfile << "set pm3d map \n";
	plotfile << "set xlabel \"i\" \n ";
	plotfile << "set ylabel \"j\" \n";
	plotfile << "set nokey \n";

	plotfile << "xMin = " << iMin << " \n";
	plotfile << "yMin = " << jMin << " \n";
	plotfile << "set xrange [" << iMin << ":" << iMax <<"]\n";
	plotfile << "set yrange [" << jMin << ":" << jMax <<"]\n";

	plotfile << "set output \"numericalProblemGeneric.eps\"\n";
	plotfile << "set title \" Numerical Solution for Problem Generic\"\n";
	plotfile << "splot \"../bin/numericalProblem3\" using ($1 +xMin):($2+yMin):3 matrix \n";
	plotfile.close();
      }
}
void solveProblem4(string cfgName){
  string title="";
  
  string line;
  int paramNo=-1; // number of parameters an object has
  int flag=0; //0 title, 1 grid, 2 shapes
  
  int ij[4]={0};//gridsize
  
  char shape='z'; // type of shape:a ring, b circle, c line, d rectangle, e quad, z unknown
  int shapes=0; // number of shapes
  double ring[256][4];// x,y,r,p
  int ringNo = 0; //number of rings
  double circle[256][4];
  int circleNo = 0;
  double sline[256][5];//xmin, xmax. ymin, ymax, p
  int slineNo = 0;
  double rectangle[256][5];
  int rectangleNo = 0;
  double quad[256][9];// x1,y1 , x2,y2 , x3,y3 , x4,y4 , p
  int quadNo = 0;  
  
  
  ifstream cfg;
  cfg.open(cfgName.c_str());//opens the file
  
  //check for errors
  if (cfg.fail()){
    cerr << "Error Opening File" << endl;
    exit(1);
  }
  
  //Read untill the end
  while(!cfg.eof()) {
    cfg >> line;
    
    line = nocaps(line);// removes capitals
    
    if(line[0] == '#'){//checks if line starts with #
      if(line.size() >= 9){
	if(line.substr(1,8)=="gridsize"){
	  flag = 1; //sets flag to 1 (grid size)
	  paramNo = -1; //ensures that first string not stored
	    }
      }
      else {
	flag = 2; // sets flag to 2 (shape)
	shapes++;
	paramNo = -1; //ensures that first string not stored
	shape = 'z'; // set shape type unkown
      }
    }
    
	switch(flag){
	case 0 : //title
	  title += line;
	  break;
	case 1 : //grid size
	  if(paramNo!=-1){
	    ij[paramNo] = atoi(line.c_str());
	  }
	  paramNo++;	
	  break;
	case 2 : // shape
	  if(paramNo!=-1){
	    switch(shape){
	    case 'a':
	      if(paramNo==0){
		ringNo++;//increases the shape number
	      }
	      ring[ringNo-1][paramNo] = atof(line.c_str());//adds shape line by line
	      paramNo++;
	      break;
	    case 'b':
	      if(paramNo==0){
		circleNo++;
	      }
	      circle[circleNo-1][paramNo] = atof(line.c_str());
	      paramNo++;
	      break;
	    case 'c':
	      if(paramNo==0){
		slineNo++;
	      }
	      sline[slineNo-1][paramNo] = atof(line.c_str());
	      paramNo++;
	      break;
	    case 'd':
	      if(paramNo==0){
		rectangleNo++;
	      }
	      rectangle[rectangleNo-1][paramNo] = atof(line.c_str());
	      paramNo++;
	      break;
	    case 'e':
	      if(paramNo==0){
		quadNo++;
	      }
	      quad[quadNo-1][paramNo] = atof(line.c_str());
	      paramNo++;
	      break;
	    }
	  }
	  else if(line[0] != '#' && shape == 'z'){
	    shape = line[0];
	    paramNo++;
	  }
	  break;
	}
  }
  cfg.close();
  
  electrostatics::UnsolvedElectrostaticSystem unsolvedSystem(ij[0],ij[1],ij[2],ij[3]);
  for(int i=0; i<ringNo;i++){ // adds all rings to unsolved problem
    unsolvedSystem.setBoundaryRing(ring[i][0],ring[i][1],ring[i][2],ring[i][3]);
  }
  for(int i=0; i<circleNo;i++){ // adds all circles to unsolved problem
    unsolvedSystem.setBoundaryCircle(circle[i][0],circle[i][1],circle[i][2],circle[i][3]);
  }
  for(int i=0; i<slineNo;i++){ // adds all lines to usolved problem
    unsolvedSystem.setBoundaryLine(sline[i][0],sline[i][1],sline[i][2],sline[i][3],sline[i][4]);
  }
  for(int i=0; i<rectangleNo;i++){ // adds all rectangles to unsolved problem
    unsolvedSystem.setBoundaryRectangle(rectangle[i][0],rectangle[i][2],rectangle[i][3],rectangle[i][1],rectangle[i][4]);
  }
  for(int i=0; i<quadNo;i++){ // quads have not yet been set up in this version
  }
  electrostatics::SolvedElectrostaticSystem solvedSystemNumerical(ij[0], ij[1], ij[2], ij[3]);
  electrostatics::finiteDifferenceSolve(unsolvedSystem, solvedSystemNumerical);
  solvedSystemNumerical.saveFile("numericalProblem4");

  //conors plot scipt
  ofstream plotfile("../scripts/plotProblemGeneric.plt");
    if (plotfile.is_open())
      {
	plotfile << "#!/usr/bin/gnuplot -persist \n";
	plotfile << "load '../scripts/colormap.pal' \n";
	plotfile << "set size ratio -1 \n";
	plotfile << "set term postscript color \n";
	plotfile << "set pm3d map \n";
	plotfile << "set xlabel \"i\" \n ";
	plotfile << "set ylabel \"j\" \n";
	plotfile << "set nokey \n";

	plotfile << "xMin = " << ij[0] << " \n";
	plotfile << "yMin = " << ij[2] << " \n";
	plotfile << "set xrange [" << ij[0] << ":" << ij[1] <<"]\n";
	plotfile << "set yrange [" << ij[2] << ":" << ij[3] <<"]\n";

	plotfile << "set output \"numericalProblemGeneric.eps\"\n";
	plotfile << "set title \" Numerical Solution for Problem Generic\"\n";
	plotfile << "splot \"../bin/numericalProblem4\" using ($1 +xMin):($2+yMin):3 matrix \n";
	plotfile.close();
      }
}

string nocaps(string input){
  int inputlen = input.size(); // gets the length of a string
  for (int i=0;i<inputlen;i++){ // gets rid of capital letters
    if (isupper(input[i])){ 
      input[i]=tolower(input[i]);
    }
  }
  return input;
}
    

