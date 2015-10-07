#include "libgcm/elements/TetrahedronSecondOrder.hpp"

using namespace gcm;

TetrahedronSecondOrder::TetrahedronSecondOrder() {
    for( int j = 0; j < 6; j++ )
        secondaryVertices[j] = -1;
}

TetrahedronSecondOrder::TetrahedronSecondOrder(uint n, uint v[4]) {
    number = n;
    for( int j = 0; j < 4; j++ )
        vertices[j] = v[j];
    for( int j = 0; j < 6; j++ )
        secondaryVertices[j] = -1;
}

TetrahedronSecondOrder::TetrahedronSecondOrder(uint n, uint v[4], uint av[6]) {
    number = n;
    for( int j = 0; j < 4; j++ )
        vertices[j] = v[j];
    for( int j = 0; j < 6; j++ )
        secondaryVertices[j] = av[j];
}