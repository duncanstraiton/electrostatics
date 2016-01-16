/**
 * A class to represent an electrostatic system.
 *
 * The system is represented as a grid with columns i and rows j.
 * i increases from left to right and j increase from bottom to top.
 * i and j do not necessarally start at zero. eg iMin could be negative.
 * Each position is assigned a position number, k, starting from the
 * bottom left as shown:
 *
 * 8  9 10 11
 * 4  5  6  7
 * 0  1  2  3
 *
 * Each position in the grid corresponds to the electric potential at that
 * point.
 *
 * The system is stored as a (dense) matrix from the eigen library.
 * Because eigen matrices are indexed by (row, column), eg:
 * (row 0, col 0)    (row 0, col 1)    (row 0, col 2)
 * (row 1, col 0)    (row 1, col 1)    (row 1, col 2)
 *
 * and the grid is indexed (like an x-y system) by (column, row), eg:
 * (col 0, row 1)    (col 1, row 1)    (col 2, row 1)
 * (col 0, row 0)    (col 1, row 0)    (col 2, row 0)
 *
 * to get from the matrix representaion to the grid representation the matrix 
 * must be transposed and then fliped vertically. 
 */

#ifndef ELECTROSTATICSYSTEM_H
#define ELECTROSTATICSYSTEM_H

#include <Eigen/Dense>

namespace electrostatics{

class ElectrostaticSystem {
    protected:
        int iMin, iMax, jMin, jMax;
        long kMax;  // kMax = (iMax - iMin) * (jMax - jMin)  -- It may be very large
        Eigen::MatrixXd potentials;

    public:
        /* Constructors. */
        ElectrostaticSystem(int iMin, int iMax, int jMin, int jMax);
        ElectrostaticSystem(Eigen::MatrixXd &potentials, int iMin, int jMin);


        /* Get min/max values for i, j, k. kMin is alway 0. */
        int getIMin() const { return iMin; }
        int getIMax() const { return iMax; }
        int getJMin() const { return jMin; }
        int getJMax() const { return jMax; }
        long getKMax() const { return kMax; }

        /* Get the length of the system in i or j direction. Counts from 1. */
        int getLengthI() const { return potentials.rows(); }
        int getLengthJ() const { return potentials.cols(); }


        /* Get the potential at position (i, j) or (k).*/
        double getPotentialIJ(int i, int j) const;
        double getPotentialK(long k) const;


        /* Set the potential at position (i, j) or (k).*/
        void setPotentialIJ(int i, int j, double potential);
        void setPotentialK(long k, double potential);

        /* Get the potential matrix. */
        Eigen::MatrixXd* getPotentials();

        /* Convert between (i, j) coordinates and (k) positions. */
        long ij2k(int i, int j) const;
        int* k2ij(long k) const;

        /* Print out the system potentials in the grid form described above/ */
        void print() const;
};

} // namespace electrostatics
#endif
