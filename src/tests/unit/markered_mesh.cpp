#include <cmath>
#include <gtest/gtest.h>

#include "libgcm/mesh/euler/markers/MarkeredMesh.hpp"
#include "libgcm/mesh/euler/markers/MarkeredSurfaceGeoGenerator.hpp"
#include "libgcm/Math.hpp"

using namespace gcm;

TEST(MarkeredSurfaceGeoGenerator, Sphere)
{
    auto& gen = MarkeredSurfaceGeoGenerator::getInstance();
    auto surface = gen.generate("models/sphere.geo");

    float dh = 1e-4;

    AABB outer = {-1-dh, 1+dh, -1-dh, 1+dh, -1-dh, 1+dh};
    AABB inner = {-1+dh, 1-dh, -1+dh, 1-dh, -1+dh, 1-dh};

    ASSERT_TRUE(outer.includes(surface.getAABB()));
    ASSERT_TRUE(surface.getAABB().includes(inner));


    real center[] = {0, 0, 0};

    const auto& nodes = surface.getMarkerNodes();

    for (auto m: nodes)
    {
        auto dist = distance(center, m.coords);
        EXPECT_NEAR(1.0, dist, 1e-4);
    }

    vector3r norm;

    for (auto f: surface.getMarkerFaces())
    {
        findTriangleFaceNormal(nodes[f.vertices[0]].coords, nodes[f.vertices[1]].coords, nodes[f.vertices[2]].coords, &norm.x, &norm.y, &norm.z);

        EXPECT_NEAR(norm*nodes[f.vertices[0]].coords, 1.0, 1e-2);
    }
}

TEST(MarkeredSurfaceGeoGenerator, Cube)
{
    auto& gen = MarkeredSurfaceGeoGenerator::getInstance();
    auto surface = gen.generate("models/cube.geo");


    float dh = 1e-4;

    AABB outer = {-5-dh, 5+dh, -5-dh, 5+dh, -5-dh, 5+dh};
    AABB inner = {-5+dh, 5-dh, -5+dh, 5-dh, -5+dh, 5-dh};

    ASSERT_TRUE(outer.includes(surface.getAABB()));
    ASSERT_TRUE(surface.getAABB().includes(inner));

    const auto& nodes = surface.getMarkerNodes();

    for (auto m: nodes)
    {
        if (fabs(fabs(m.coords.z)-5) < EQUALITY_TOLERANCE)
        {
            EXPECT_LT(m.coords.x, 5.0+EQUALITY_TOLERANCE);
            EXPECT_GT(m.coords.x, -5.0-EQUALITY_TOLERANCE);
            EXPECT_LT(m.coords.y, 5.0+EQUALITY_TOLERANCE);
            EXPECT_GT(m.coords.y, -5.0-EQUALITY_TOLERANCE);
        }
        else if (fabs(fabs(m.coords.y)-5) < EQUALITY_TOLERANCE)
        {
            EXPECT_LT(m.coords.x, 5.0+EQUALITY_TOLERANCE);
            EXPECT_GT(m.coords.x, -5.0-EQUALITY_TOLERANCE);
            EXPECT_LT(m.coords.z, 5.0+EQUALITY_TOLERANCE);
            EXPECT_GT(m.coords.z, -5.0-EQUALITY_TOLERANCE);
        }
        else if (fabs(fabs(m.coords.x)-5) < EQUALITY_TOLERANCE)
        {
            EXPECT_LT(m.coords.y, 5.0+EQUALITY_TOLERANCE);
            EXPECT_GT(m.coords.y, -5.0-EQUALITY_TOLERANCE);
            EXPECT_LT(m.coords.z, 5.0+EQUALITY_TOLERANCE);
            EXPECT_GT(m.coords.z, -5.0-EQUALITY_TOLERANCE);
        }
        else
            ASSERT_TRUE(false);

    }

    vector3r norm, _norm;

    for (auto f: surface.getMarkerFaces())
    {
        findTriangleFaceNormal(nodes[f.vertices[0]].coords, nodes[f.vertices[1]].coords, nodes[f.vertices[2]].coords, &norm.x, &norm.y, &norm.z);

        auto r = (nodes[f.vertices[0]].coords + nodes[f.vertices[1]].coords + nodes[f.vertices[2]].coords)/3;

        if (fabs(fabs(r.z)-5) < EQUALITY_TOLERANCE)
        {
            _norm.x = _norm.y = 0;
            _norm.z = sgn(r.z);
        }
        else if (fabs(fabs(r.y)-5) < EQUALITY_TOLERANCE)
        {
            _norm.x = _norm.z = 0;
            _norm.y = sgn(r.y);
        }
        else if (fabs(fabs(r.x)-5) < EQUALITY_TOLERANCE)
        {
            _norm.y = _norm.z = 0;
            _norm.x = sgn(r.x);
        }
        else
            ASSERT_TRUE(false);

        EXPECT_NEAR(norm*_norm, 1.0, 1e-2);
    }

}

#ifdef CONFIG_ENABLE_ASSERTIONS

TEST(MarkeredMeshGeometry, Sphere)
{
    auto& gen = MarkeredSurfaceGeoGenerator::getInstance();
    auto surface = gen.generate("models/sphere.geo");

    ASSERT_NO_THROW(
        MarkeredMesh mesh(surface, {100, 100, 100}, {0.03, 0.03, 0.03});
        mesh.reconstructBorder();
    );

}

TEST(MarkeredMeshGeometry, Cube)
{
    auto& gen = MarkeredSurfaceGeoGenerator::getInstance();
    auto surface = gen.generate("models/cube.geo");

    ASSERT_NO_THROW(
        MarkeredMesh mesh(surface, {120, 120, 120}, {0.10, 0.10, 0.10});
        mesh.reconstructBorder();
    );
}

#endif
