#include "libgcm/util/NodeTypes.hpp"

#include "libgcm/node/IdealElasticNode.hpp"
#include "libgcm/node/IdealElasticWithMicrocracksNode.hpp"
#include "libgcm/node/FiniteStrainIdealElasticNode.hpp"
#include "libgcm/node/IdealElasticContinualDamageNode.hpp"



namespace gcm
{
    Node getNodeOfTheType(uchar nodeType)
    {
        switch (nodeType)
        {
            case IDEAL_ELASTIC_NODE_TYPE:
                return IdealElasticNode();
//            case IDEAL_ELASTIC_WITH_MICROCRACKS_NODE_TYPE:
//                return IdealElasticWithMicrocracksNode();
			case IDEAL_ELASTIC_CONTINUAL_DAMAGE_NODE_TYPE:
				return IdealElasticContinualDamageNode();
			case FINITE_STRAIN_IDEAL_ELASTIC_NODE_TYPE:
				return FiniteStrainIdealElasticNode();
            default:
                THROW_UNSUPPORTED("Unknown node type requested: " + nodeType);
        }
    }
}