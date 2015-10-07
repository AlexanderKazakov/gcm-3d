#ifndef GCM_GEO2_MESH_LOADER_H_
#define GCM_GEO2_MESH_LOADER_H_

#include <map>
#include <string>

#include "libgcm/mesh/tetr/TetrMeshSecondOrder.hpp"
#include "libgcm/util/formats/MshTetrFileReader.hpp"
#include "libgcm/util/formats/Vtu2TetrFileReader.hpp"
#include "libgcm/util/Singleton.hpp"
#include "libgcm/snapshot/VTK2SnapshotWriter.hpp"
#include "libgcm/Exception.hpp"
#include "libgcm/util/Logging.hpp"
#include "libgcm/DummyDispatcher.hpp"



#include <gmsh/Gmsh.h>
#include <gmsh/GModel.h>
#include <gmsh/GEntity.h>
#include <gmsh/MElement.h>
#include <gmsh/MVertex.h>
#include <gmsh/MTriangle.h>
#include <gmsh/MTetrahedron.h>


namespace gcm {
    class Geo2MeshLoader: public Singleton<Geo2MeshLoader>
    {
    protected:
        /*
         * Loads mesh from using passed configuration
         */
         USE_LOGGER;
         bool isMshFileCreated(std::string fileName);
         std::string getMshFileName(std::string geoFile);
         std::string getVtkFileName(std::string geoFile);
         void createMshFile(std::string fileName, float tetrSize);
         std::map<std::string,bool> createdFiles;
    public:
        Geo2MeshLoader();
        ~Geo2MeshLoader();
        void preLoadMesh(AABB* scene, int& sliceDirection, int& numberOfNodes, std::string fileName, float tetrSize);
        void loadMesh(TetrMeshSecondOrder* mesh, GCMDispatcher* dispatcher, std::string fileName, float tetrSize);
        void cleanUp();
    };
}

#endif /* GCM_GEO2_MESH_LOADER_H_ */
