#ifndef GCM_IDEALPLASTICRHEOLOGYMODEL_HPP
#define GCM_IDEALPLASTICRHEOLOGYMODEL_HPP

#include <string>
#include <vector>

#include "libgcm/rheology/matrixSetters/IsotropicElasticRheologyMatrixSetter.hpp"
#include "libgcm/solvers/matrixDecomposers/IsotropicElasticRheologyMatrixDecomposer.hpp"
#include "libgcm/solvers/correctors/IdealPlasticFlowCorrector.hpp"
#include "libgcm/node/IdealElasticNode.hpp"
#include "libgcm/util/Logging.hpp"
#include "libgcm/rheology/models/RheologyModel.hpp"

namespace gcm {

    class IdealPlasticRheologyModel : public RheologyModel {
    public:
        IdealPlasticRheologyModel();

    private:
        USE_LOGGER;
    };
}

#endif /* GCM_IDEALPLASTICRHEOLOGYMODEL_HPP */