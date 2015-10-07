#include "libgcm/elements/Cube.hpp"

using namespace gcm;

Cube::Cube() {
    for( int j = 0; j < 8; j++ )
        vertices[j] = -1;
}

Cube::Cube(uint n, uint v[8]) {
    number = n;
    for( int j = 0; j < 8; j++ )
        vertices[j] = v[j];
}

bool Cube::operator<(const Cube& other) const {
	return (number < other.number);
}