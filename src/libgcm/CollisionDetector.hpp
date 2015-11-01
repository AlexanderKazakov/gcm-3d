#ifndef _GCM_COLLISION_DETECTOR
#define _GCM_COLLISION_DETECTOR 1

#include <vector>
#include <string>

#include "libgcm/util/AABB.hpp"
#include "libgcm/mesh/Mesh.hpp"
#include "libgcm/mesh/cube/BasicCubicMesh.hpp"
#include "libgcm/util/Logging.hpp"

namespace gcm {

    class AABB;
    class Node;

    class CollisionDetector    {

    protected:
        USE_LOGGER;
        float threshold;
        bool static_operation;

    public:
        CollisionDetector();
        virtual ~CollisionDetector();
        virtual std::string getType () const = 0;
        void set_threshold(float value);
        float get_threshold();
        virtual void find_collisions(NodeStorage &virtNodeStorage) = 0;
        bool find_intersection(AABB &outline1, AABB &outline2, AABB &intersection);
        // return elements that are in intersection
        virtual void find_nodes_in_intersection(Mesh* mesh, AABB &intersection, std::vector<Node> &result);
        void find_nodes_in_intersection(BasicCubicMesh* mesh, AABB& intersection, std::vector<Node>& result);
        void find_nodes_in_intersection(Mesh* mesh, AABB &intersection, std::vector<int> &result);
        // number returned surface elements (nodes and triangles) sequentially
        //void renumber_surface(std::vector<TriangleFirstOrder> &faces, std::vector<Node> &nodes);
        // returns surface elements (nodes and triangles) renumbered sequentially
        //void renumber_volume(std::vector<Tetrahedron_1st_order> &tetrs, std::vector<Node> &nodes);
        void set_static(bool state);
        bool is_static();
    };

}

#endif
