#include "libgcm/rheology/models/RheologyModel.hpp"

using namespace gcm;

RheologyModel::RheologyModel(std::string modelType, uchar nodeType, 
	SetterPtr matrixSetter, DecomposerPtr matrixDecomposer) :
	modelType(modelType), nodeType(nodeType), matrixSetter(matrixSetter),
	matrixDecomposer(matrixDecomposer)
{
}

RheologyModel::~RheologyModel() {
}

std::string RheologyModel::getType() const {
    return modelType;
}

const SetterPtr RheologyModel::getRheologyMatrixSetter() const {
    return matrixSetter;
}

const DecomposerPtr RheologyModel::getRheologyMatrixDecomposer() const {
    return matrixDecomposer;
}

const std::vector<Corrector*>& RheologyModel::getCorrectors() const {
	return correctors;
}

uchar RheologyModel::getNodeType() const {
    return nodeType;
}

uchar RheologyModel::getSizeOfValuesInPDE() const {
    Node tmpNode = getNodeOfTheType(nodeType);
    return tmpNode.getSizeOfPDE();
}

uchar RheologyModel::getSizeOfValuesInODE() const {
    Node tmpNode = getNodeOfTheType(nodeType);
    return tmpNode.getSizeOfODE();
}