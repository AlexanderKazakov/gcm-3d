#ifndef MeshMover_HPP
#define MeshMover_HPP 

#include "libgcm/util/matrixes.hpp"
#include "libgcm/rheology/Material.hpp"
#include "libgcm/node/Node.hpp"

namespace gcm
{
    class MeshMover
    {
        public:
            virtual ~MeshMover() = 0;
            virtual void move(const Node& curNode, Node& newNode, const gcm::real tau) = 0;
    };
}
#endif /* MeshMover_HPP */