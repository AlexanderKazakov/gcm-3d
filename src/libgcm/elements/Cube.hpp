#ifndef GCM_CUBE_HPP
#define GCM_CUBE_HPP

#include <iostream>
#include "libgcm/util/Types.hpp"
#include "libgcm/elements/Element.hpp"

namespace gcm {
    /*
     * Cube class
     */
    class Cube : public Element {
    public:
        Cube();
        Cube(uint n, uint v[8]);
		bool operator<(const Cube& other) const;
        /*
         * List of tetr vertices.
         */
        uint vertices[8];
    };
}

namespace std {
    inline std::ostream& operator<< (std::ostream &os, const gcm::Cube &cube) {
        os << "\nCube. Number: " << cube.number << "\n";
        os << "\tVertices:";
        for( uint i = 0; i < 8; i++ )
            os << " " << cube.vertices[i];
        return os;
    }
}

#endif