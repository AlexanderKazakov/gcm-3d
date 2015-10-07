#ifndef ANALYTICAL_FINITE_STRAIN_RHEOLOGY_MATRIX_DECOMPOSER_HPP
#define	ANALYTICAL_FINITE_STRAIN_RHEOLOGY_MATRIX_DECOMPOSER_HPP

#include "libgcm/solvers/matrixDecomposers/RheologyMatrixDecomposer.hpp"
#include "libgcm/solvers/matrixDecomposers/AnalyticalRheologyMatrixDecomposer.hpp"
#include "libgcm/solvers/matrixDecomposers/NumericalRheologyMatrixDecomposer.hpp"


namespace gcm
{
	/**
	 * Class for analytical rheology matrix decomposition for 
	 * ideal elastic anisotropic material in model that takes into account 
	 * the finite strain rates
	 */
    class AnalyticalFiniteStrainRheologyMatrixDecomposer: 
	                                          public RheologyMatrixDecomposer
    {
        public:
            AnalyticalFiniteStrainRheologyMatrixDecomposer();
            ~AnalyticalFiniteStrainRheologyMatrixDecomposer();
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

#endif	/* ANALYTICAL_FINITE_STRAIN_RHEOLOGY_MATRIX_DECOMPOSER_HPP */

