#include "ElectrostaticSystem.h"
#include <stdexcept>
#include <gtest/gtest.h>
#include <Eigen/Dense>

class ElectrostaticSystemTest : public ::testing::Test {
    protected:
        electrostatics::ElectrostaticSystem* system;

        virtual void SetUp() {
            system = new electrostatics::ElectrostaticSystem(-18, 2, -8, 6);
        }

        virtual void TearDown() {
            delete system;
        }
};

TEST_F(ElectrostaticSystemTest, getDimensions) {
    ASSERT_EQ(21, system->getLengthI());
    ASSERT_EQ(15, system->getLengthJ());
}

TEST_F(ElectrostaticSystemTest, BoundaryChecks) {
    ASSERT_THROW(system->getPotentialIJ(-19, 0), std::out_of_range);
    ASSERT_THROW(system->getPotentialK(30000), std::out_of_range);
    ASSERT_THROW(system->setPotentialIJ(-19, 7, 432.34), std::out_of_range);
    ASSERT_THROW(system->setPotentialK(-1, 0.24), std::out_of_range);
    ASSERT_THROW(system->ij2k(-20, 3), std::out_of_range);
    ASSERT_THROW(system->k2ij(-3), std::out_of_range);

    ASSERT_NO_THROW(system->getPotentialIJ(-18, 6));
    ASSERT_NO_THROW(system->getPotentialK(system->getKMax()));
    ASSERT_NO_THROW(system->setPotentialIJ(2, -8, 4324.3));
    ASSERT_NO_THROW(system->setPotentialK(0, 93.2343));
    ASSERT_NO_THROW(system->ij2k(-12, 4));
    ASSERT_NO_THROW(system->k2ij(7));
}

TEST_F(ElectrostaticSystemTest, Ij2k) {
    ASSERT_EQ(0, system->ij2k(-18, -8));
    ASSERT_EQ(22, system->ij2k(-17, -7));
    ASSERT_EQ(system->getKMax(), system->ij2k(2, 6));
}

TEST_F(ElectrostaticSystemTest, K2ij) {
    int *ij = system->k2ij(22);
    ASSERT_EQ(-17, ij[0]);
    ASSERT_EQ(-7, ij[1]);
    ij = system->k2ij(system->getKMax());
    ASSERT_EQ(2, ij[0]);
    ASSERT_EQ(6, ij[1]);
}

TEST_F(ElectrostaticSystemTest, MaxK) {
    ASSERT_EQ(314, system->getKMax());
}

TEST_F(ElectrostaticSystemTest, SetAndGetPotential1) {
    double potential = 7.8492l;
    system->setPotentialIJ(1, 5, potential);
    ASSERT_EQ(potential, system->getPotentialK(system->ij2k(1, 5)));
}

TEST_F(ElectrostaticSystemTest, SetAndGetPotential2) {
    double potential = -8.6823;
    int *ij = system->k2ij(16);
    system->setPotentialK(16, potential);
    ASSERT_EQ(potential, system->getPotentialIJ(ij[0], ij[1]));
}

TEST(ElectrostaticSystemTestMatrixConstructor, MatrixConstructor) {
    int iMin = -3;
    int jMin = 4;
    Eigen::MatrixXd potentialsMatrix = Eigen::MatrixXd::Zero(3, 3);
    potentialsMatrix << 1, 2, 3,
                        4, 5, 6,
                        7, 8, 9;
    electrostatics::ElectrostaticSystem system = electrostatics::ElectrostaticSystem(
            potentialsMatrix, iMin, jMin);
    ASSERT_EQ(-3, system.getIMin());
    ASSERT_EQ(4, system.getJMin());
    ASSERT_EQ(-1, system.getIMax());
    ASSERT_EQ(6, system.getJMax());
    ASSERT_EQ(8, system.getKMax());
    ASSERT_EQ(2, system.getPotentialIJ(0+iMin, 1+jMin));
    ASSERT_EQ(9, system.getPotentialK(8));
}
