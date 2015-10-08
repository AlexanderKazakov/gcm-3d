#ifndef GCM_TETR_MESH_SECOND_ORDER_H_
#define GCM_TETR_MESH_SECOND_ORDER_H_

#include "libgcm/mesh/tetr/TetrMeshFirstOrder.hpp"
#include "libgcm/elements/TetrahedronSecondOrder.hpp"
#include "libgcm/elements/TriangleSecondOrder.hpp"

#include "libgcm/interpolator/TetrSecondOrderMinMaxInterpolator.hpp"


typedef std::pair <int, int> IntPair;

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
        int firstOrderNodesNumber;
        int secondOrderNodesNumber;

        TetrSecondOrderMinMaxInterpolator* interpolator;

    protected:
        int countSecondOrderNodes(TetrMeshFirstOrder* src);
        void generateSecondOrderNodes();
        void fillSecondOrderNode(Node& newNode, int nodeIdx1, int nodeIdx2);

        void verifyTetrahedraVertices();
        void build_volume_reverse_lookups();
        void build_surface_reverse_lookups();
        void build_first_order_border();
        void generateSecondOrderBorder();
        void moveSecondOrderNode(int nodeIdx, int nodeIdx1, int nodeIdx2);

    public:
        TetrMeshSecondOrder();
        ~TetrMeshSecondOrder();
        void copyMesh(TetrMeshFirstOrder* src);
        void copyMesh2(TetrMeshSecondOrder* src);
        void createTetrs(int number);
        void createTriangles(int number);
        /*
         * Returns tetr by its index.
         */
        TetrahedronFirstOrder& getTetr(unsigned int index);
        TetrahedronSecondOrder& getTetr2(int index);

        TetrahedronFirstOrder& getTetrByLocalIndex(unsigned int index);
        TetrahedronSecondOrder& getTetr2ByLocalIndex(int index);

        void addTetr(TetrahedronFirstOrder& tetr);
        void addTetr2(TetrahedronSecondOrder& tetr);
        void rebuildMaps();

        TriangleFirstOrder& getTriangle(int index);
        TriangleSecondOrder& getTriangle2(int index);

        void preProcessGeometry();
        void moveCoords(float tau);

        bool interpolateNode(Node& origin, float dx, float dy, float dz, bool debug,
                                Node& targetNode, bool& isInnerPoint);

        bool interpolateNode(Node& node);
		
		bool belongsToTetr(int nodeNum, int tetrNum, int faceNum);

        virtual const SnapshotWriter& getSnaphotter() const override;
        virtual const SnapshotWriter& getDumper() const override;
    };
}
#endif
