#include "UnsolvedElectrostaticSystem.h"
#include <stdexcept>
#include <gtest/gtest.h>
#include <Eigen/Dense>

class UnsolvedElectrostaticSystemTest : public ::testing::Test {
    protected:
        electrostatics::UnsolvedElectrostaticSystem* system;

        virtual void SetUp() {
            system = new electrostatics::UnsolvedElectrostaticSystem(-18, 2, -8, 6);
        }

        virtual void TearDown() {
            delete system;
        }
};

TEST_F(UnsolvedElectrostaticSystemTest, BoundaryChecks) {
    ASSERT_THROW(system->isBoundaryConditionIJ(-19, 0), std::out_of_range);
    ASSERT_THROW(system->isBoundaryConditionK(30000), std::out_of_range);
    ASSERT_THROW(system->setBoundaryConditionIJ(-19, 7, true), std::out_of_range);
    ASSERT_THROW(system->setBoundaryConditionK(-1, false), std::out_of_range);

    ASSERT_NO_THROW(system->isBoundaryConditionIJ(-18, 0));
    ASSERT_NO_THROW(system->isBoundaryConditionK(system->getKMax()));
    ASSERT_NO_THROW(system->setBoundaryConditionIJ(2, -8, false));
    ASSERT_NO_THROW(system->setBoundaryConditionK(0, true));
}

TEST_F(UnsolvedElectrostaticSystemTest, IsAndSetBoundary1) {
    system->setBoundaryConditionIJ(1, 5, true);
    ASSERT_EQ(true, system->isBoundaryConditionK(system->ij2k(1, 5)));
    ASSERT_EQ(false, system->isBoundaryConditionIJ(0, -1));
}

TEST_F(UnsolvedElectrostaticSystemTest, IsAndSetBoundary2) {
    int* ij = system->k2ij(31);
    system->setBoundaryConditionK(31, true);
    ASSERT_EQ(true, system->isBoundaryConditionIJ(ij[0], ij[1]));
    ASSERT_EQ(false, system->isBoundaryConditionK(10));
}
