#include "libgcm/BruteforceCollisionDetector.hpp"

#include "launcher/loaders/mesh/BasicCubicMeshLoader.hpp"
#include "libgcm/node/Node.hpp"
#include "libgcm/Engine.hpp"
#include "launcher/loaders/mesh/RectangularCutCubicMeshLoader.hpp"

using namespace gcm;
using std::string;
using std::vector;

BruteforceCollisionDetector::BruteforceCollisionDetector() {
    INIT_LOGGER("gcm.BruteforceCollisionDetector");
}

BruteforceCollisionDetector::~BruteforceCollisionDetector() {
}

string BruteforceCollisionDetector::getType () const
{
    return "BruteforceCollisionDetector";
}

void BruteforceCollisionDetector::find_collisions(NodeStorage &virtNodeStorage) {
    Engine& e = Engine::getInstance();
    AABB intersection;
    vector<Node> local_nodes;

    LOG_DEBUG("Processing local/local collisions");

    // process collisions between local nodes and local faces
    // we start both cycles from zero because collision should be 'symmetric'
    for (int i = 0; i < e.getNumberOfBodies(); i++)
    {
        for (int j = 0; j < e.getNumberOfBodies(); j++) 
        {
            Mesh* mesh1 = e.getBody(i)->getMeshes();
            Mesh* mesh2 = e.getBody(j)->getMeshes();
            AABB outl1 = mesh1->getExpandedOutline();
            AABB outl2 = mesh2->getExpandedOutline();

            LOG_DEBUG("Local mesh #" << mesh1->getId() << " with outline " << outl1
                      << " and local mesh #" << mesh2->getId() << " with outline " << outl2);

            if ((i != j) && (find_intersection(outl1, outl2, intersection))) 
            {
                LOG_DEBUG("Collision detected between local mesh zone #" << mesh1->getId()
                          << " and local mesh zone #" << mesh2->getId() << ". "
                          << "Intersection: " << intersection);
                // find local nodes inside intersection
                if(mesh1->getType() == launcher::BasicCubicMeshLoader::MESH_TYPE ||
				   mesh1->getType() == launcher::RectangularCutCubicMeshLoader::MESH_TYPE)
                	find_nodes_in_intersection(dynamic_cast<BasicCubicMesh*>(mesh1),
					                           intersection, local_nodes);
                else
                	find_nodes_in_intersection(mesh1, intersection, local_nodes);

                LOG_DEBUG("Got " << local_nodes.size() << " nodes");

                LOG_DEBUG("Virt nodes size before processing: " << virtNodeStorage.getSize());

                // process collisions
                float direction[3];
                for (unsigned int k = 0; k < local_nodes.size(); k++) 
                {
                    if (k % 10000 == 0)
                        LOG_DEBUG("Nodes processed: " << k);

                    // Check axis directions
                    for (int m = 0; m < 3; m++) 
                    {
						local_nodes[k].contactDirection = m;
                        mesh1->findBorderNodeNormal(local_nodes[k],
                                        &direction[0], &direction[1], &direction[2], false);

                        if (direction[m] > 0)
                            direction[m] = 1;
                        else
                            direction[m] = -1;
                        for (int z = 0; z < 3; z++)
                            if (z != m)
                                direction[z] = 0;

	                    // TODO@next - conception of virtNodeStorage for virtNodes does not work.
	                    // What if nodes from different meshes has different sizeOfPDE? etc..
                        Node new_node;
                        if( mesh2->interpolateBorderNode(
                                local_nodes[k].coords[0], local_nodes[k].coords[1], local_nodes[k].coords[2],
                                direction[0] * get_threshold(), direction[1] * get_threshold(),
						        direction[2] * get_threshold(), new_node) )
                        {
                            new_node.setBorder(true);
                            new_node.setInContact(true);
                            new_node.contactNodeNum = j;
                            new_node.setCustomFlag(Node::FLAG_1, 1);
							new_node.contactDirection = m;
                            (mesh1->getNodeByGlobalIndex(local_nodes[k].number)).setInContact(true);
                            (mesh1->getNodeByGlobalIndex(local_nodes[k].number)).contactNodeNum =
		                            virtNodeStorage.getSize();
                            (mesh1->getNodeByGlobalIndex(local_nodes[k].number)).contactDirection = m;
                            virtNodeStorage.addNode(new_node);
                            break;
                        }
                    }
                }
            }

            LOG_DEBUG("Virt nodes size after processing: " << virtNodeStorage.getSize());

            // clear
            local_nodes.clear();
        }
    }
    
    LOG_DEBUG("Local/local collisions processed");

}
