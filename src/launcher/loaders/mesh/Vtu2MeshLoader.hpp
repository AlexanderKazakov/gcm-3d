#ifndef LAUNCHER_VTU2MESHLOADER_H
#define LAUNCHER_VTU2MESHLOADER_H

#include <string>

#include "libgcm/GCMDispatcher.hpp"
#include "libgcm/util/AABB.hpp"
#include "libgcm/util/Singleton.hpp"
#include "libgcm/mesh/tetr/TetrMeshSecondOrder.hpp"
#include "libgcm/mesh/tetr/Vtu2MeshLoader.hpp"

#include "launcher/util/xml.hpp"

#include "launcher/loaders/mesh/MeshLoader.hpp"


namespace launcher
{
    class Vtu2MeshLoader: public gcm::Singleton<launcher::Vtu2MeshLoader>, public launcher::MeshLoader<gcm::TetrMeshSecondOrder>
    {
        protected:
            void parseDesc(const xml::Node& desc, std::string& id, std::string& fileName);
            void loadMesh(const xml::Node& desc, gcm::TetrMeshSecondOrder* mesh) override;
        public:
            void preLoadMesh(const xml::Node& desc, gcm::AABB& aabb, int& sliceDirection, uint& numberOfNodes);

            const static std::string MESH_TYPE;
    };
}
        
#endif
