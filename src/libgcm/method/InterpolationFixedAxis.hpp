#ifndef GCM_INTERPOLATION_FIXED_AXIS_H
#define    GCM_INTERPOLATION_FIXED_AXIS_H

#include "libgcm/mesh/tetr/TetrMeshFirstOrder.hpp"
#include "libgcm/mesh/tetr/TetrMeshSecondOrder.hpp"
#include "libgcm/method/NumericalMethod.hpp"
#include "libgcm/mesh/Mesh.hpp"
//#include "libgcm/util/AnisotropicMatrix3D.hpp"
////#include "libgcm/util/ElasticMatrix3D.hpp"
#include "libgcm/util/Types.hpp"
#include "libgcm/node/Node.hpp"
#include "libgcm/util/Logging.hpp"
#include "libgcm/Exception.hpp"


namespace gcm
{

    class InterpolationFixedAxis : public NumericalMethod {
    public:
        InterpolationFixedAxis();
        virtual ~InterpolationFixedAxis();
        int getNumberOfStages();
        void doNextPartStep(Node& cur_node, Node& new_node, float time_step, int stage, Mesh* mesh);
        std::string getType();
    protected:
        int prepare_node(Node& cur_node, RheologyMatrixPtr rheologyMatrix,
                float time_step, int stage, Mesh* mesh,
                float* dksi, bool* inner, std::vector<Node>& previous_nodes,
                float* outer_normal, bool debug);
        int prepare_node(Node& cur_node, RheologyMatrixPtr rheologyMatrix,
                float time_step, int stage, Mesh* mesh,
                float* dksi, bool* inner, std::vector<Node>& previous_nodes,
                float* outer_normal);
        int find_nodes_on_previous_time_layer(Node& cur_node, int stage, Mesh* mesh,
                float dksi[], bool inner[], std::vector<Node>& previous_nodes,
                float outer_normal[], bool debug);
        int find_nodes_on_previous_time_layer(Node& cur_node, int stage, Mesh* mesh,
                float dksi[], bool inner[], std::vector<Node>& previous_nodes,
                float outer_normal[]);
        
        void __doNextPartStep(Node& cur_node, Node& new_node, float time_step, int stage, Mesh* mesh);

        USE_LOGGER;
    };
}

#endif    /* GCM_INTERPOLATION_FIXED_AXIS_H */

