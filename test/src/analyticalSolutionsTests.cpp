#include "analyticalSolutions.h"
#include <gtest/gtest.h>

TEST(analyticalSolutionsTest, Problem1) {
    double radiusA = 5.23;
    double radiusB = 20.7;
    double potentialA = 9;
    double potentialB = 18;
    ASSERT_EQ(9, electrostatics::analyticalProblem1(3, 2, radiusA, radiusB, potentialA, potentialB));
    ASSERT_EQ(18, electrostatics::analyticalProblem1(20, 21, radiusA, radiusB, potentialA, potentialB));
    ASSERT_NEAR(16.171476080924844, 
            electrostatics::analyticalProblem1(7, 14, radiusA, radiusB, potentialA, potentialB),
            0.0000000001);
    ASSERT_NEAR(13.890807251564116, 
            electrostatics::analyticalProblem1(-1, -11, radiusA, radiusB, potentialA, potentialB),
            0.0000000001);
}

TEST(analyticalSolutionsTest, Problem2) {
    double cylinderRadius = 5.23;
    double uniformField = 20;
    ASSERT_EQ(0, electrostatics::analyticalProblem2(3, 2, cylinderRadius, uniformField));
    ASSERT_NEAR(-124.36977242857142, 
            electrostatics::analyticalProblem2(7, 14, cylinderRadius, uniformField),
            0.000001);
    ASSERT_NEAR(15.5159180327868852, 
            electrostatics::analyticalProblem2(-1, -11, cylinderRadius, uniformField),
            0.0000001);
}

TEST(analyticalSolutionsTest, UniformField) {
    ASSERT_EQ(0.1, electrostatics::uniformField(-50, 50, 5, -5));
}
