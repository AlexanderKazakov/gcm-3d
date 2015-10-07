#include "libgcm/rheology/models/IdealElasticRheologyModel.hpp"

using namespace gcm;

IdealElasticRheologyModel::IdealElasticRheologyModel()
          : RheologyModel("IdealElasticRheologyModel", IDEAL_ELASTIC_NODE_TYPE, 
                         makeSetterPtr<IsotropicElasticRheologyMatrixSetter>(), 
                         makeDecomposerPtr<IsotropicElasticRheologyMatrixDecomposer>()) {
    
}