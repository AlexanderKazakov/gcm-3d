#ifndef FINITE_STRAIN_IDEAL_ELASTIC_NODE_HPP
#define FINITE_STRAIN_IDEAL_ELASTIC_NODE_HPP

#include "libgcm/util/Types.hpp"
#include "libgcm/util/NodeTypes.hpp"
#include "libgcm/node/IdealElasticNode.hpp"

namespace gcm {

    /**
     * Node for models with:
     * (a) ideal elasticity;
     * (b) finite deformations;
     * (c) only brittle material failure.
     * Can be used for both isotropic and anisotropic models.
	 * Compared to IdealElasticNode this one contains density (rho) in PDE
     */
    class FiniteStrainIdealElasticNode : public IdealElasticNode {
    public:

		/**
		 * Creates node with: 
		 *     type == FINITE_STRAIN_IDEAL_ELASTIC_NODE_TYPE
		 *     valuesInPDE == 10 (velocity vector and stress tensor)
		 *     valuesInODE == 0
		 */
		FiniteStrainIdealElasticNode();

        
		/**
		 * Measure of damage from 0 to 1
		 */
		gcm::real getRho() const;
		// Read-write access. These methods are slower than get-ters above.
		gcm::real& rho();
    };
	
}

#endif /* FINITE_STRAIN_IDEAL_ELASTIC_NODE_HPP */