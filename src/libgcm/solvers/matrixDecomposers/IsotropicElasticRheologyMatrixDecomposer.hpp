#ifndef ISOTROPIC_ELASTIC_RHEOLOGY_MATRIX_DECOMPOSER_HPP
#define ISOTROPIC_ELASTIC_RHEOLOGY_MATRIX_DECOMPOSER_HPP

#include "libgcm/solvers/matrixDecomposers/RheologyMatrixDecomposer.hpp"

namespace gcm
{
	/**
	 * Class for analytical rheology matrix decomposition for 
	 * ideal elastic isotropic material
	 */
    class IsotropicElasticRheologyMatrixDecomposer: 
	                                    public RheologyMatrixDecomposer
    {
    public:
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

#endif /* ISOTROPIC_ELASTIC_RHEOLOGY_MATRIX_DECOMPOSER_HPP */