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
#include <string>

namespace electrostatics {

class SolvedElectrostaticSystem : public ElectrostaticSystem{
    protected:
        doubleGrid field;   // Strength of the field
        doubleGrid fieldX;  // Component of field in x direction
        doubleGrid fieldY;  // Component of field in y direction
        double maxE;        // Max value of electric field
        bool fieldFound;    // True if the field has been found

    public:
        /* Constructors */
        SolvedElectrostaticSystem(doubleGrid &potentials, int iMin, int jMin);
        SolvedElectrostaticSystem(int iMin, int iMax, int jMin, int jMax);

        /* Methods */
        void findField();
        void saveFieldGNUPlot(std::string fileName);
};

} // namespace electrostatics
#endif
