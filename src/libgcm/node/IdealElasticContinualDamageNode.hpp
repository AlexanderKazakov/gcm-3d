#ifndef IDEAL_ELASTIC_CONTINUAL_DAMAGE_NODE_HPP
#define IDEAL_ELASTIC_CONTINUAL_DAMAGE_NODE_HPP

#include "libgcm/util/Types.hpp"
#include "libgcm/util/NodeTypes.hpp"
#include "libgcm/node/IdealElasticNode.hpp"

namespace gcm {

    /**
     * Node for models with:
     * (a) ideal (?) elasticity;
     * (b) infinitely small deformations;
     * (c) only brittle (?) material failure.
     * Can be used for both isotropic and anisotropic models.
	 * Compared to IdealElasticNode this one contain scalar damageMeasure
	 * parameter. Represents the most stupid model of damage.
     */
    class IdealElasticContinualDamageNode : public IdealElasticNode {
    public:

		/**
		 * Creates node with: 
		 *     type == IDEAL_ELASTIC_NODE_TYPE
		 *     valuesInPDE == 9 (velocity vector and stress tensor)
		 *     valuesInODE == 1
		 */
		IdealElasticContinualDamageNode();

        
		/**
		 * Measure of damage from 0 to 1
		 */
		gcm::real getDamageMeasure() const;
		// Read-write access. These methods are slower than get-ters above.
		gcm::real& damageMeasure();
    };
	
}

#endif /* IDEAL_ELASTIC_CONTINUAL_DAMAGE_NODE_HPP */