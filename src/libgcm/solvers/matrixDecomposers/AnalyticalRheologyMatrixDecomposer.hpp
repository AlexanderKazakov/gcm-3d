#ifndef ANALYTICAL_RHEOLOGY_MATRIX_DECOMPOSER_HPP
#define ANALYTICAL_RHEOLOGY_MATRIX_DECOMPOSER_HPP

#include "libgcm/solvers/matrixDecomposers/RheologyMatrixDecomposer.hpp"
#include "libgcm/util/Math.hpp"

#include "libgcm/util/Exception.hpp"
#include <assert.h>

namespace gcm
{
	/**
	 * Class for analytical rheology matrix decomposition for 
	 * ideal elastic anisotropic material with infinitely small strains
	 */
    class AnalyticalRheologyMatrixDecomposer : public RheologyMatrixDecomposer
    {
		private:
			/**
			 * Find non-trivial solution of M * x = 0, Range(M) = 2
             * @param M matrix of SLE
             * @param x solution
             */
			void findNonZeroSolution(double **M, double *x) const;
			/**
			 * Find eigenvector of matrix A corresponding to eigenvalue l
             * @param eigenVec eigenvector to find
             * @param l eigenvalue
             * @param A rheology matrix which eigenvector is sought
             * @param stage 0 if A_x, 1 if A_y, 2 if A_z
             */
			void findEigenVec(double *eigenVec, double l, 
			                  const GcmMatrix &A, int stage) const;
			/**
			 * Find two non-trivial solutions of M * x = 0, Range(M) = 1
             * @param M matrix of SLE
             * @param x solution1
             * @param y solution2
             */
			void findNonZeroSolution(double **M, double *x, double *y) const;
			/**
			 * Find two eigenvectors of matrix A corresponding to eigenvalue l
             * @param eigenVec1 first eigenvector to find
             * @param eigenVec2 second eigenvector to find
             * @param l eigenvalue
             * @param A rheology matrix which eigenvector is sought
             * @param stage 0 if A_x, 1 if A_y, 2 if A_z
             */
			void findEigenVec(double *eigenVec1, double *eigenVec2, double l, 
			                  const GcmMatrix &A, int stage) const;
			/**
			 * Find 3 roots of third order polynomial which represent squares
			 * of 6 ( + / - sqrt ) non-zero eigenvalues of matrix A  
             * @param A rheology matrix which eigenvalues are sought with help
			 * of solving third order polynomial
             * @param stage 0 if A_x, 1 if A_y, 2 if A_z
             * @param r1 1'st root of the polynomial
             * @param r2 2'nd root of the polynomial
             * @param r3 3'd root of the polynomial
             * @param isMultiple is two roots of the polynomial happen to be
			 * equal
             */
			void findRoots(const GcmMatrix &A, int stage, 
			               double& r1, double& r2, double& r3, 
						   bool& isMultiple) const;
        public:
            AnalyticalRheologyMatrixDecomposer();
            ~AnalyticalRheologyMatrixDecomposer();
            /**
             * Computes decomposition for matrix in X direction.
             *
             * @param a Matrix to decompose.
             * @param u Matrix to store \f$U\f$
             * @param l Matrix to store \f$\Lambda\f$
             * @param u1 Matrix to store \f$U^{-1}\f$
             */
            void decomposeX(const GcmMatrix& a, GcmMatrix& u, 
			                      GcmMatrix& l, GcmMatrix& u1) const;
            /**
             * Computes decomposition for matrix in Y direction.
             *
             * @param a Matrix to decompose.
             * @param u Matrix to store \f$U\f$
             * @param l Matrix to store \f$\Lambda\f$
             * @param u1 Matrix to store \f$U^{-1}\f$
             */
            void decomposeY(const GcmMatrix& a, GcmMatrix& u, 
			                      GcmMatrix& l, GcmMatrix& u1) const;
            /**
             * Computes decomposition for matrix in Z direction.
             *
             * @param a Matrix to decompose.
             * @param u Matrix to store \f$U\f$
             * @param l Matrix to store \f$\Lambda\f$
             * @param u1 Matrix to store \f$U^{-1}\f$
             */
            void decomposeZ(const GcmMatrix& a, GcmMatrix& u, 
			                      GcmMatrix& l, GcmMatrix& u1) const;
    };
}

#endif /* ANALYTICAL_RHEOLOGY_MATRIX_DECOMPOSER_HPP */
