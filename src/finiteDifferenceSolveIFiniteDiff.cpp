#ifdef IFiniteDiff

#include <Eigen/Dense>
#include <Eigen/Sparse>
#include "finiteDifferenceSolve.h"
#include "SolvedElectrostaticSystem.h"
#include "UnsolvedElectrostaticSystem.h"

namespace electrostatics {

/* Iterative finite difference method.
 */
void finiteDifferenceSolve(UnsolvedElectrostaticSystem &unsolvedSystem,
        SolvedElectrostaticSystem &solvedSystem) {

	int maxIter=1000;

	for(int iter=1; iter<=maxIter; iter++) {

		for(int i=unsolvedSystem.getIMin();i<=unsolvedSystem.getIMax();i++) {

			for(int j=unsolvedSystem.getJMin();j<=unsolvedSystem.getJMax();j++) {

				if (unsolvedSystem.isBoundaryConditionIJ(i,j)) continue;
					
				if(iter % 2 == 1) {
					
					double sum=0;

					if(i<unsolvedSystem.getIMax()) sum += unsolvedSystem.getPotentialIJ(i+1, j);

					if(i>unsolvedSystem.getIMin()) sum += unsolvedSystem.getPotentialIJ(i-1, j);

					if(j<unsolvedSystem.getJMax()) sum += unsolvedSystem.getPotentialIJ(i, j+1);

					if(j>unsolvedSystem.getJMin()) sum += unsolvedSystem.getPotentialIJ(i, j-1);
					
					solvedSystem.setPotentialIJ(i,j, sum/4);
											

				} else { 

					double sum=0;

					if(i<solvedSystem.getIMax()) sum += solvedSystem.getPotentialIJ(i+1, j);

					if(i>solvedSystem.getIMin()) sum += solvedSystem.getPotentialIJ(i-1, j);

					if(j<solvedSystem.getJMax()) sum += solvedSystem.getPotentialIJ(i, j+1);

					if(j>solvedSystem.getJMin()) sum += solvedSystem.getPotentialIJ(i, j-1);
					
					unsolvedSystem.setPotentialIJ(i,j, sum/4);
				}

			}	
	
		}

	}

}



} // namespace electrostatics

#endif // IFiniteDiff
