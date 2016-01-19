/**
 * A class to represent an unsolved electrostatic system.
 *
 * Inherits from ElectrostaticSystem, adding a matrix (from eigen, dense) to
 * specify which points of the grid are boundary conditions and which are 
 * unspecified points to calculate the potential at.
 * 
 * eg:
 *
 * potentials:      and         boundaryCondition:
 * 6 7 8                    1 1 1
 * 3 4 5                    0 1 0
 * 0 1 2                    0 0 1
 *
 * would mean that positions 2, 4, 6, 7 and 8 of the potentials are boundary 
 * conditions, while the other positions are unknown points.
 *
 * Note: In actual use all the edge positions of the grid should be 
 * defined as boundary conditions, unlike in the above example.
 */

#ifndef UNSOLVEDELECTROSTATICSYSTEM_H
#define UNSOLVEDELECTROSTATICSYSTEM_H

#include <Eigen/Dense>
#include "ElectrostaticSystem.h"

namespace electrostatics {

class UnsolvedElectrostaticSystem : public ElectrostaticSystem{
    protected:
        /* Boolean matrix as described above to mark the positions of the boundary conditions. */
        Eigen::Matrix<bool, Eigen::Dynamic, Eigen::Dynamic>  boundaryConditionPositions; 

    public:
        /* Constructors. */
        UnsolvedElectrostaticSystem(int iMin, int iMax, int jMin, int jMax);
        UnsolvedElectrostaticSystem(Eigen::MatrixXd &potentials,
                Eigen::Matrix<bool, Eigen::Dynamic, Eigen::Dynamic> &boundaryConditionPositions,
                int iMin, int jMin);

        
        /* Test if position (i, j) or (k) is a boundary condition. */
        bool isBoundaryConditionIJ(int i, int j) const;
        bool isBoundaryConditionK(long k) const;

        /* Set/unset position (i, j) or (k) as a boundary condition. */
        void setBoundaryConditionIJ(int i, int j, bool isBoundaryCondition);
        void setBoundaryConditionK(long k, bool isBoundaryCondition);
};

} // namespace electrostatics
#endif
