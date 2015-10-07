#ifndef NUMERICAL_RHEOLOGY_MATRIX_DECOMPOSER_HPP
#define NUMERICAL_RHEOLOGY_MATRIX_DECOMPOSER_HPP 

#include "libgcm/solvers/matrixDecomposers/RheologyMatrixDecomposer.hpp"
#include "libgcm/util/Exception.hpp"

#include <iostream>
#include <gsl/gsl_eigen.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_complex.h>
#include <gsl/gsl_complex_math.h>

namespace gcm
{
	/**
	 * Class for numerical matrix decomposition by gsl-library
	 */
    class NumericalRheologyMatrixDecomposer: public RheologyMatrixDecomposer
    {
	private:
		mutable gsl_vector_complex* eval;
		mutable gsl_matrix_complex* evec;
		mutable gsl_matrix* _a;
		mutable gsl_matrix* _u;
		mutable gsl_matrix* _u1;
		mutable gsl_permutation* perm;
		mutable gsl_eigen_nonsymmv_workspace* w;
		/**
		 * Converts gsl matrix to gcm one.
		 *
		 * @param a Matrix to convert
		 * @param b Matrix to store result
		 */
		void gsl2gcm(const gsl_matrix* a, GcmMatrix& b) const;
		/**
		 * Converts gcm matrix to gsl one.
		 *
		 * @param a Matrix to convert
		 * @param b Matrix to store result
		 */
		void gcm2gsl(const GcmMatrix& a, gsl_matrix* b) const;
		void decompose(const GcmMatrix& a, GcmMatrix& u, GcmMatrix& l,
		                     GcmMatrix& u1, int stage) const;
	public:
		NumericalRheologyMatrixDecomposer();
		~NumericalRheologyMatrixDecomposer();
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
};

#endif /* NUMERICAL_RHEOLOGY_MATRIX_DECOMPOSER_HPP */
