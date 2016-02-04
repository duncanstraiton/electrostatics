/**
 * A class to represent a solved electrostatic system.
 *
 * Inherits from ElectrostaticSystem, adding methods that wouldn't
 * make sense for an unsolved system, eg plotting, finding equipotentials etc.
 *
 * This extra functionallity has not yet been implemented, so it is currently
 * the exact same as ElectrostaticSystem.
 */

#ifndef SOLVEDELECTROSTATICSYSTEM_H
#define SOLVEDELECTROSTATICSYSTEM_H

#include <Eigen/Dense>
#include "ElectrostaticSystem.h"

namespace electrostatics {

class SolvedElectrostaticSystem : public ElectrostaticSystem{
    public:
        /* Constructors. */
        SolvedElectrostaticSystem(doubleGrid &potentials, int iMin, int jMin);
        SolvedElectrostaticSystem(int iMin, int iMax, int jMin, int jMax);
};

} // namespace electrostatics
#endif
