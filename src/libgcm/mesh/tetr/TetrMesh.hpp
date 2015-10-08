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

        virtual int getTetrsNumber() = 0;
        virtual int getTriangleNumber() = 0;

        virtual void addTetr(TetrahedronFirstOrder& tetr) = 0;

        virtual TetrahedronFirstOrder& getTetr(unsigned int index) = 0;

        virtual TetrahedronFirstOrder& getTetrByLocalIndex(unsigned int index) = 0;

        virtual TriangleFirstOrder& getTriangle(int index) = 0;

        virtual bool hasTetr(unsigned int index) = 0;

        virtual void createTetrs(int number) = 0;

        virtual void createTriangles(int number) = 0;
		
		virtual bool belongsToTetr(int nodeNum, int tetrNum, int faceNum) = 0;

        /*void preProcessGeometry();

        float getRecommendedTimeStep();

        float getMinH();

        void doNextPartStep(float tau, int stage);

        void checkTopology(float tau);*/
    };
}
#endif
