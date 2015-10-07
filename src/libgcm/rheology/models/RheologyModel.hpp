#ifndef GCM_RHEOLOGYMODEL_HPP
#define GCM_RHEOLOGYMODEL_HPP

#include <string>
#include <vector>

#include "libgcm/util/NodeTypes.hpp"
#include "libgcm/util/Assertion.hpp"
#include "libgcm/util/Logging.hpp"
#include "libgcm/rheology/RheologyMatrix.hpp"
#include "libgcm/rheology/matrixSetters/RheologyMatrixSetter.hpp"
#include "libgcm/rheology/rightHandSideSetters/RightHandSideSetter.hpp"
#include "libgcm/solvers/matrixDecomposers/RheologyMatrixDecomposer.hpp"
#include "libgcm/solvers/correctors/Corrector.hpp"


namespace gcm {
	class Corrector;
	
    class RheologyModel {
    public:
        RheologyModel(std::string modelType, uchar nodeType, SetterPtr matrixSetter, DecomposerPtr matrixDecomposer);
        ~RheologyModel();
        std::string getType() const;
        uchar getNodeType() const;
        uchar getSizeOfValuesInPDE() const;
        uchar getSizeOfValuesInODE() const;

        // TODO: does it mean that we create Model for each body?
        // BTW, do we need a concept of 'Body' in new code structure?
        // virtual const Material& getMaterial() const = 0;
        // TODO: how do we identify required node type?
        const SetterPtr getRheologyMatrixSetter() const;
        const DecomposerPtr getRheologyMatrixDecomposer() const;
		const std::vector<Corrector*>& getCorrectors() const;
		
		/*	Comment until it will be realized in derived classes
        virtual const RightHandSideSetter& getRightHandSideSetter() const = 0;
		 */
		
    protected:
        std::string modelType;
        uchar nodeType;
        SetterPtr matrixSetter;
        DecomposerPtr matrixDecomposer;
		RheologyMatrixPtr rheologyMatrix;
		std::vector<Corrector*> correctors;

    private:
        USE_LOGGER;
    };
}

#endif /* GCM_RHEOLOGYMODEL_HPP */ 