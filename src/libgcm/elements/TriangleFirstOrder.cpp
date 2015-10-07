#include "libgcm/elements/TriangleFirstOrder.hpp"

using namespace gcm;

TriangleFirstOrder::TriangleFirstOrder(): TriangleFirstOrder(-1, -1, -1, -1)
{
}

TriangleFirstOrder::TriangleFirstOrder(uint n, uint v[3]): TriangleFirstOrder(n, v[0], v[1], v[2])
{
}

TriangleFirstOrder::TriangleFirstOrder(uint n, uint v1, uint v2, uint v3)
{
    number = n;
    vertices[0] = v1;
    vertices[1] = v2;
    vertices[2] = v3;
}