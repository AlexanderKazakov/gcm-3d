#ifndef GCM_TETR_MESH_H_
#define GCM_TETR_MESH_H_

#include "libgcm/mesh/Mesh.hpp"
#include "libgcm/elements/TetrahedronFirstOrder.hpp"
#include "libgcm/elements/TriangleFirstOrder.hpp"


namespace gcm
{
    class TetrMesh: public Mesh {

    public:
        TetrMesh();
        virtual ~TetrMesh();

        virtual uint getTetrsNumber() = 0;
        virtual uint getTriangleNumber() = 0;

        virtual void addTetr(TetrahedronFirstOrder& tetr) = 0;

        virtual TetrahedronFirstOrder& getTetr(uint index) = 0;

        virtual TetrahedronFirstOrder& getTetrByLocalIndex(uint index) = 0;

        virtual TriangleFirstOrder& getTriangle(uint index) = 0;

        virtual bool hasTetr(uint index) = 0;

        virtual void createTetrs(uint number) = 0;

        virtual void createTriangles(uint number) = 0;
		
		virtual bool belongsToTetr(uint nodeNum, uint tetrNum, uint faceNum) = 0;

        /*void preProcessGeometry();

        float getRecommendedTimeStep();

        float getMinH();

        void doNextPartStep(float tau, uint stage);

        void checkTopology(float tau);*/
    };
}
#endif
