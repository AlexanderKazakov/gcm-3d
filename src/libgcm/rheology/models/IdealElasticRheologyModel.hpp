#ifndef GCM_IDEALELASTICRHEOLOGYMODEL_HPP
#define GCM_IDEALELASTICRHEOLOGYMODEL_HPP

#include <string>
#include <vector>

#include "libgcm/rheology/matrixSetters/IsotropicElasticRheologyMatrixSetter.hpp"
#include "libgcm/solvers/matrixDecomposers/IsotropicElasticRheologyMatrixDecomposer.hpp"
#include "libgcm/nodes/IdealElasticNode.hpp"
#include "libgcm/util/Logging.hpp"
#include "libgcm/rheology/models/RheologyModel.hpp"

namespace gcm {

    class IdealElasticRheologyModel : public RheologyModel {
    public:
        IdealElasticRheologyModel();

    private:
        USE_LOGGER;
    };
}

#endif /* GCM_IDEALELASTICRHEOLOGYMODEL_HPP */ 