#ifndef MARKEREDMESHNG_HPP
#define MARKEREDMESHNG_HPP 

#include <unordered_map>

#include "libgcm/mesh/markers/MarkeredSurface.hpp"
#include "libgcm/node/CalcNode.hpp"
#include "libgcm/util/Types.hpp"
#include "libgcm/mesh/Mesh.hpp"
#include "libgcm/Logging.hpp"

namespace gcm
{
    class MarkeredMeshNG: public Mesh
    {
        protected:
            LineFirstOrderInterpolator interpolator;
            MarkeredSurface surface;            
            uint meshElems;
            gcm::real elemSize;

            vector3r pivot;
            std::unordered_map<uint, uint> borderFacesMap;
            
            

            void findBorderCells();
            void fillInterior();

            void getCellCoords(vector3r p, int& i, int& j, int&k);
            
            void logMeshStats();
            void calcMinH();
            void preProcessGeometry();            
            
            USE_LOGGER;

        public:
            MarkeredMeshNG(const MarkeredSurface& surface, uint meshElems);
            MarkeredMeshNG();
            ~MarkeredMeshNG();
            
            uint getCellLocalIndex(const uint i, const uint j, const uint k) const;
            void getCellIndexes(uint num, uint& i, uint& j, uint &k) const;
            CalcNode& getCellByLocalIndex(const uint i, const uint j, const uint k);
            
            const vector3r& getPivot() const;
            gcm::real getElemSize() const;
            uint getMeshElemes() const;
            
            const MarkeredSurface& getMarkeredSurafce() const;
            
            void generateMesh();            
            
            float getRecommendedTimeStep();
            float getMinH();
            void doNextPartStep(float tau, int stage);
            void checkTopology(float tau);

            void findBorderNodeNormal(int border_node_index, float* x, float* y, float* z, bool debug);
            bool interpolateNode(CalcNode& origin, float dx, float dy, float dz, bool debug,
                                    CalcNode& targetNode, bool& isInnerPoint);
            bool interpolateNode(CalcNode& node);
            bool interpolateBorderNode(gcm::real x, gcm::real y, gcm::real z, 
                                    gcm::real dx, gcm::real dy, gcm::real dz, CalcNode& node);

            void setMeshElems(uint meshElems);

            void setSurface(MarkeredSurface surface);
            
    };
};
#endif /* MARKEREDMESHNG_HPP */
