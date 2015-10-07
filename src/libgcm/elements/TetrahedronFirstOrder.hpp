#ifndef GCM_TETR_H_
#define GCM_TETR_H_

#include <iostream>
#include "libgcm/elements/Element.hpp"

namespace gcm {
    /*
     * Tetrahedron base class to inherit from.
     */
    class TetrahedronFirstOrder : public Element {
    public:
        TetrahedronFirstOrder();
        TetrahedronFirstOrder(uint n, uint v[4]);
        /*
         * List of tetr vertices.
         */
        uint vertices[4];
    };
}

namespace std {
    inline std::ostream& operator<< (std::ostream &os, const gcm::TetrahedronFirstOrder &tetr) {
        os << "\n\tTetrahedron 1st order. Number: " << tetr.number << "\n";
        os << "\tVertices:";
        for( uint i = 0; i < 4; i++ )
            os << " " << tetr.vertices[i];
        return os;
    }
}

#endif