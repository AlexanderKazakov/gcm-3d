#ifndef GCM_TETR_SECOND_ORDER_H_
#define GCM_TETR_SECOND_ORDER_H_

#include "libgcm/elements/TetrahedronFirstOrder.hpp"

namespace gcm {
    /*
     * Tetrahedron with additional points on edges.
     */
    class TetrahedronSecondOrder : public TetrahedronFirstOrder {
    public:
        TetrahedronSecondOrder();
        TetrahedronSecondOrder(uint n, uint v[4]);
        TetrahedronSecondOrder(uint n, uint v[4], uint av[6]);
        /*
         * List of additional vertices on edges
         */
        uint secondaryVertices[6];
    };
}

namespace std {
    inline std::ostream& operator<< (std::ostream &os, const gcm::TetrahedronSecondOrder &tetr) {
        os << "\n\tTetrahedron 2nd order. Number: " << tetr.number << "\n";
        os << "\tVertices:";
        for( uint i = 0; i < 4; i++ )
            os << " " << tetr.vertices[i];
        os << "\n\tAdditional vertices:";
        for( uint i = 0; i < 6; i++ )
            os << " " << tetr.secondaryVertices[i];
        return os;
    }
}

#endif