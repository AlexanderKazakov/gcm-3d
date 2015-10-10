#include "libgcm/elements/TriangleSecondOrder.hpp"

using namespace gcm;

TriangleSecondOrder::TriangleSecondOrder() {
    for( int i = 0; i < 3; i++ )
        addVerts[i] = (uint) -1;
}

TriangleSecondOrder::TriangleSecondOrder(uint n, uint v[3])
{
    for( int i = 0; i < 3; i++ )
        addVerts[i] = (uint) -1;
}

TriangleSecondOrder::TriangleSecondOrder(uint n, uint v[3], uint av[3])
{
    for( int i = 0; i < 3; i++ )
        addVerts[i] = av[i];
}