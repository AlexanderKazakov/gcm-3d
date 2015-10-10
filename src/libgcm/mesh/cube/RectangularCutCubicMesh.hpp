#ifndef GCM_RECTANGULARCUT_CUBIC_MESH_H_
#define GCM_RECTANGULARCUT_CUBIC_MESH_H_

#include "libgcm/mesh/cube/BasicCubicMesh.hpp"

namespace gcm
{
    class RectangularCutCubicMesh: public BasicCubicMesh {

    protected:
		
		AABB cutArea;
		
        void preProcessGeometry() override;
		
		uint findNeighbourPoint(Node& node, float dx, float dy, float dz, 
		                       bool debug, float* coords, bool* innerPoint) override;
        USE_LOGGER;

        void findNearestsNodes(const vector3r& coords, uint N, std::vector< std::pair<uint,float> >& result) override;

    public:
		RectangularCutCubicMesh();
		
		void findBorderNodeNormal(const Node& node, float* x, float* y, float* z, bool debug) override;
        bool interpolateBorderNode(real x, real y, real z, 
                                real dx, real dy, real dz, Node& node) override;
        
		void transfer(float x, float y, float z) override;
		
		void setCutArea(const AABB& area) {
			cutArea = area;
		};
    };
}
#endif
