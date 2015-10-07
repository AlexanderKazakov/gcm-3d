#ifndef ANISOTROPIC_ELASTIC_RHEOLOGY_MATRIX_SETTER_HPP
#define ANISOTROPIC_ELASTIC_RHEOLOGY_MATRIX_SETTER_HPP

#include "libgcm/rheology/matrixSetters/RheologyMatrixSetter.hpp"

namespace gcm
{
	/**
	 * Class to fill in rheology matrix for ideal elastic anisotropic material
	 */
    class AnisotropicElasticRheologyMatrixSetter: public RheologyMatrixSetter
    {
    public:
        /**
        * Returns number of matrix states after decompositions. This method 
		* is supposed to be used to
        * preallocate memory for matrices cache.
        *
        * @return Number of possible states (for one space direction)
		* or 0 if unknown.
        */
        unsigned int getNumberOfStates() const;
        /**
        * Returns number of state for specified node.
        *
        * @param node Node to return state for
        *
        * @return Node state
        */
        unsigned int getStateForNode(const Node& node) const;
        /**
         * Fills matrix \f$A_x\f$ using specified material parameters.
         *
         * @param a Matrix to fill
         * @param material Material to use
         * @param node Node to set matrix at
         */
        void setX(GcmMatrix& a, const MaterialPtr& material, 
		                        const Node& node) override;
        /**
         * Fills matrix \f$A_y\f$ using specified material parameters.
         *
         * @param a Matrix to fill
         * @param material Material to use
         * @param node Node to set matrix at
         */
        void setY(GcmMatrix& a, const MaterialPtr& material, 
		                        const Node& node) override;
        /**
         * Fills matrix \f$A_z\f$ using specified material parameters.
         *
         * @param a Matrix to fill
         * @param material Material to use
         * @param node Node to set matrix at
         */
        void setZ(GcmMatrix& a, const MaterialPtr& material, 
		                        const Node& node) override;
    };
}
#endif /* ANISOTROPIC_ELASTIC_RHEOLOGY_MATRIX_SETTER_HPP */