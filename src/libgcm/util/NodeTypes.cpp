#include "libgcm/util/NodeTypes.hpp"

#include "libgcm/nodes/Node.hpp"
#include "libgcm/nodes/IdealElasticNode.hpp"
#include "libgcm/nodes/IdealElasticWithMicrocracksNode.hpp"
#include "libgcm/nodes/FiniteStrainIdealElasticNode.hpp"
#include "libgcm/nodes/IdealElasticContinualDamageNode.hpp"


#include "libgcm/util/Exception.hpp"

namespace gcm
{
    Node getNewNode(uchar nodeType)
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