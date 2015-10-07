#ifndef GCM_TRIANGLE_SECOND_ORDER_H_
#define GCM_TRIANGLE_SECOND_ORDER_H_

#include "libgcm/elements/TriangleFirstOrder.hpp"

namespace gcm {
    /*
     * Triangle with additional points on edges.
     */
    class TriangleSecondOrder : public TriangleFirstOrder {
    public:
        TriangleSecondOrder();
        TriangleSecondOrder(uint n, uint v[3]);
        TriangleSecondOrder(uint n, uint v[3], uint av[3]);
        /*
         * List of additional vertices on edges
         */
        uint secondaryVertices[3];
    };
}

namespace std {
    inline std::ostream& operator<< (std::ostream &os, const gcm::TriangleSecondOrder &tri) {
        os << "\n\tTriangle 2nd order. Number: " << tri.number << "\n";
        os << "\tVertices:";
        for( uint i = 0; i < 3; i++ )
            os << " " << tri.vertices[i];
        os << "\n\tAdditional vertices:";
        for( uint i = 0; i < 3; i++ )
            os << " " << tri.secondaryVertices[i];
        return os;
    }
}

#endif