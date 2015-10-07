#ifndef GCM_TRIANGLE_H_
#define GCM_TRIANGLE_H_

#include <iostream>
#include "libgcm/util/Types.hpp"

namespace gcm {
    /*
     * Triangle base class to inherit from.
     */
    class TriangleFirstOrder {
    public:
        TriangleFirstOrder();
        TriangleFirstOrder(uint n, uint v[3]);
        TriangleFirstOrder(uint n, uint v1, uint v2, uint v3);
        /*
         * Number of current tetr
         */
        uint number;
        /*
         * List of tetr vertices.
         */
        uint vertices[3];
    };
}

namespace std {
    inline std::ostream& operator<< (std::ostream &os, const gcm::TriangleFirstOrder &tri) {
        os << "\n\tTriangle 1st order. Number: " << tri.number << "\n";
        os << "\tVertices:";
        for( uint i = 0; i < 3; i++ )
            os << " " << tri.vertices[i];
        return os;
    }
}

#endif