#include "libgcm/rheology/models/IdealPlasticRheologyModel.hpp"

using namespace gcm;

IdealPlasticRheologyModel::IdealPlasticRheologyModel()
          : RheologyModel("IdealPlasticRheologyModel", IDEAL_ELASTIC_NODE_TYPE, 
                         makeSetterPtr<IsotropicElasticRheologyMatrixSetter>(), 
                         makeDecomposerPtr<IsotropicElasticRheologyMatrixDecomposer>()) {
	correctors.push_back(new IdealPlasticFlowCorrector());
    
}