#ifndef GCM_TETR_MESH_SECOND_ORDER_H_
#define GCM_TETR_MESH_SECOND_ORDER_H_

#include "libgcm/mesh/tetr/TetrMeshFirstOrder.hpp"
#include "libgcm/elements/TetrahedronSecondOrder.hpp"
#include "libgcm/elements/TriangleSecondOrder.hpp"

#include "libgcm/interpolator/TetrSecondOrderMinMaxInterpolator.hpp"


typedef std::pair <uint, uint> UintPair;

namespace gcm {
    /*
     * Tetrahedral 2nd order mesh.
     */
    class TetrMeshSecondOrder: public TetrMeshFirstOrder {

    //friend class VTK2SnapshotWriter;
    friend class CollisionDetector;
    friend class BruteforceCollisionDetector;

    private:
        /*
         * List of mesh tetrahedrons.
         */
        std::vector<TetrahedronSecondOrder> tetrs2;
        std::vector<TriangleSecondOrder> border2;
        bool secondOrderNodesAreGenerated;
        uint firstOrderNodesNumber;
        uint secondOrderNodesNumber;

        TetrSecondOrderMinMaxInterpolator* interpolator;

    protected:
        uint countSecondOrderNodes(TetrMeshFirstOrder* src);
        void generateSecondOrderNodes();
        void fillSecondOrderNode(Node& newNode, uint nodeIdx1, uint nodeIdx2);

        void verifyTetrahedraVertices();
        void build_volume_reverse_lookups();
        void build_surface_reverse_lookups();
        void build_first_order_border();
        void generateSecondOrderBorder();
        void moveSecondOrderNode(uint nodeIdx, uint nodeIdx1, uint nodeIdx2);

    public:
        TetrMeshSecondOrder();
        ~TetrMeshSecondOrder();
        void copyMesh(TetrMeshFirstOrder* src);
        void copyMesh2(TetrMeshSecondOrder* src);
        void createTetrs(uint number);
        void createTriangles(uint number);
        /*
         * Returns tetr by its index.
         */
        TetrahedronFirstOrder& getTetr(uint index);
        TetrahedronSecondOrder& getTetr2(uint index);

        TetrahedronFirstOrder& getTetrByLocalIndex(uint index);
        TetrahedronSecondOrder& getTetr2ByLocalIndex(uint index);

        void addTetr(TetrahedronFirstOrder& tetr);
        void addTetr2(TetrahedronSecondOrder& tetr);
        void rebuildMaps();

        TriangleFirstOrder& getTriangle(uint index);
        TriangleSecondOrder& getTriangle2(uint index);

        void preProcessGeometry();
        void moveCoords(float tau);

        bool interpolateNode(Node& origin, float dx, float dy, float dz, bool debug,
                                Node& targetNode, bool& isInnerPoint);

        bool interpolateNode(Node& node);
		
		bool belongsToTetr(uint nodeNum, uint tetrNum, uint faceNum);

        virtual const SnapshotWriter& getSnaphotter() const override;
        virtual const SnapshotWriter& getDumper() const override;
    };
}
#endif
