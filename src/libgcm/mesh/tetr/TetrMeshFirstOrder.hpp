#ifndef GCM_TETR_MESH_FIRST_ORDER_H_
#define GCM_TETR_MESH_FIRST_ORDER_H_

#include <unordered_map>
#include <algorithm>

#include <gsl/gsl_linalg.h>

#include "libgcm/mesh/tetr/TetrMesh.hpp"
#include "libgcm/elements/TetrahedronFirstOrder.hpp"
#include "libgcm/elements/TriangleFirstOrder.hpp"
#include "libgcm/util/AABB.hpp"
#include "libgcm/method/NumericalMethod.hpp"
#include "libgcm/util/Logging.hpp"
#include "libgcm/Exception.hpp"
#include "libgcm/Math.hpp"
#include "libgcm/interpolator/TetrFirstOrderInterpolator.hpp"
#include "libgcm/snapshot/VTKSnapshotWriter.hpp"

#include "libgcm/util/KDTree.hpp"

namespace gcm
{
    class Node;
    /*
     * Tetrahedral 1st order mesh.
     */
    class TetrMeshFirstOrder: public TetrMesh {

    friend class VTKSnapshotWriter;
    friend class CollisionDetector;
    friend class BruteforceCollisionDetector;

    private:
        TetrFirstOrderInterpolator* interpolator;

    protected:
        std::unordered_map<uint, uint> tetrsMap;

        // Cache for characteristics hits
        bool charactCacheAvailable();
        bool checkCharactCache(const Node& node, float dx, float dy, float dz, uint& tetrNum);
        void updateCharactCache(const Node& node, float dx, float dy, float dz, uint tetrNum);
        uint getCharactCacheIndex(const Node& node, float dx, float dy, float dz);
        std::unordered_map<uint, uint> charactCache[18];
        // Spatial index based on KD-Tree
        struct kdtree* kdtree;
        void initSpatialIndex();
        unsigned long long cacheHits;
        unsigned long long cacheMisses;

        /*
         * List of mesh tetrahedrons.
         */
        std::vector<TetrahedronFirstOrder> tetrs1;
        std::vector<TriangleFirstOrder> border1;

        uint tetrsNumber;
        uint tetrsStorageSize;
        uint faceNumber;
        uint faceStorageSize;

        std::vector< std::vector<uint> > volumeElements;
        std::vector< std::vector<uint> > borderElements;
        std::vector<uint>& getVolumeElementsForNode(uint index);
        std::vector<uint>& getBorderElementsForNode(uint index);

        void build_surface_reverse_lookups();
        void build_volume_reverse_lookups();
        void build_border();
        void check_numbering();
        void check_outer_normals();
        void check_unused_nodes();
        void verifyTetrahedraVertices ();

        bool isTriangleBorder(uint v[4], bool* needSwap, bool debug);
        bool isTriangleBorder(uint v[4], bool* needSwap);
        bool isTriangleBorder(uint v[4], bool debug);
        bool isTriangleBorder(uint v[4]);
        /*
         * Adds triangle to border and simultaneously checks its orientation
         */
        TriangleFirstOrder createBorderTriangle(uint v[4], uint number);
        void find_border_elem_normal(uint border_element_index, float* x, float* y, float* z);

        float get_solid_angle(uint node_index, uint tetr_index);
        float tetr_h(uint i);

        void logMeshStats();

        void calcMinH();
		void calcAvgH();
        void calcMaxH();
        void calcMaxEdge();
        // It MUST take into account mesh topology.
        // So, if the mesh will be second order, h = h / 2, etc
        float mesh_min_h;
        float mesh_avg_h;
        float mesh_max_h;
        float mesh_max_edge;

        USE_LOGGER;

        uint expandingScanForOwnerTetr(const Node& node, float dx, float dy, float dz, bool debug, float* coords, bool* innerPoint);
		uint orientedExpandingScanForOwnerTetr (const Node& node, float dx, float dy, float dz, bool debug, float* coords, bool* innerPoint);
		uint virtExpandingScanForOwnerTetr(const Node& _node, float _dx, float _dy, float _dz, bool debug, float* coords, bool* innerPoint);
        uint newExpandingScanForOwnerTetr(const Node& node, float dx, float dy, float dz, bool debug, float* coords, bool* innerPoint);
		void getPotentialOwnerTetrs(float x, float y, float z, std::vector<uint>& tetrs);
        uint fastScanForOwnerTetr(const Node& node, float dx, float dy, float dz, bool debug);
        uint findOwnerTetr(const Node& node, float dx, float dy, float dz, bool debug, float* coords, bool* innerPoint);
        bool isInnerPoint(const Node& node, float dx, float dy, float dz, bool debug);

        gsl_matrix *T;
        gsl_matrix *S;
        gsl_permutation *P;

    public:
        /*
         * Constructor and destructor.
         */
        TetrMeshFirstOrder();
        virtual ~TetrMeshFirstOrder();
        //void copyMesh(TetrMeshFirstOrder* src);

        uint getTetrsNumber();
        uint getTriangleNumber();

        void addTetr(TetrahedronFirstOrder& tetr);
        /*
         * Returns tetr by its index.
         */
        TetrahedronFirstOrder& getTetr(uint index);

        TetrahedronFirstOrder& getTetrByLocalIndex(uint index);

        bool hasTetr(uint index);

        TriangleFirstOrder& getTriangle(uint index);
        // FIXME should two functions belowe be moved outside this class?
        /*
         * Creates tetrahedrons.
         */
        void createTetrs(uint number);

        void createTriangles(uint number);

        void preProcessGeometry();

        float getRecommendedTimeStep();

        // Finds minimum h over mesh
        float getMinH();
        float getAvgH() override;

        float getMaxH();

        float getMaxEdge();

        void doNextPartStep(float tau, int stage);

        void findBorderNodeNormal(const Node& _node, float* x, float* y, float* z, bool debug);
        void checkTopology(float tau);

        bool interpolateNode(Node& origin, float dx, float dy, float dz, bool debug,
                                Node& targetNode, bool& isInnerPoint);

        bool interpolateNode(Node& node);
		
        bool interpolateBorderNode(real x, real y, real z, 
                                real dx, real dy, real dz, Node& node);
		
		bool belongsToTetr(uint nodeNum, uint tetrNum, uint faceNum);

        virtual const SnapshotWriter& getSnaphotter() const override;
        virtual const SnapshotWriter& getDumper() const override;
    };
}
#endif
