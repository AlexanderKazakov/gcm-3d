#include "libgcm/elements/TetrahedronFirstOrder.hpp"
#include "libgcm/mesh/tetr/TetrMeshFirstOrder.hpp"

using namespace gcm;

TetrahedronFirstOrder::TetrahedronFirstOrder() {
    for( int j = 0; j < 4; j++ )
        vertices[j] = (uint) -1;
}

TetrahedronFirstOrder::TetrahedronFirstOrder(uint n, uint v[4]) {
    number = n;
    for( int j = 0; j < 4; j++ )
        vertices[j] = v[j];
}