#include "libgcm/node/IdealElasticContinualDamageNode.hpp"
#include "libgcm/util/Assertion.hpp"
#include "libgcm/Math.hpp"

using namespace gcm;

IdealElasticContinualDamageNode::IdealElasticContinualDamageNode() : 
		IdealElasticNode(9, 1, gcm::IDEAL_ELASTIC_CONTINUAL_DAMAGE_NODE_TYPE)
{
}

// Read-only get-ters
real IdealElasticContinualDamageNode::getDamageMeasure() const {
    return ODE[0];
}

// Read-write access
real& IdealElasticContinualDamageNode::damageMeasure() {
    return ODE[0];
}