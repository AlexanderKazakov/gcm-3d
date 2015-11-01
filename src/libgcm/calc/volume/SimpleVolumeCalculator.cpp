#include "libgcm/calc/volume/SimpleVolumeCalculator.hpp"

#include "libgcm/node/Node.hpp"

using namespace gcm;
using std::vector;

SimpleVolumeCalculator::SimpleVolumeCalculator() {
    INIT_LOGGER("gcm.SimpleVolumeCalculator");
};

void SimpleVolumeCalculator::doCalc(Node& new_node, RheologyMatrixPtr matrix,
	vector<Node>& previousNodes)
{
	uchar sizeOfPDE = new_node.getSizeOfPDE();
    assert_eq(previousNodes.size(), sizeOfPDE);

    LOG_TRACE("Start calc");

    // Here we will store (omega = Matrix_OMEGA * u)
	real* omega = new real[sizeOfPDE];

    // Calculate omega value
    // TODO - should we use U and U^-1 from old or new node??? Most probably, the 1st from old and the 2nd from new.
    for(int i = 0; i < sizeOfPDE; i++)
    {
        // omega on new time layer is equal to omega on previous time layer along characteristic
        omega[i] = 0;
        for(int j = 0; j < sizeOfPDE; j++)
        {
            omega[i] += matrix->getU(i,j) * previousNodes[i].PDE[j];
        }
    }
    // Calculate new PDE
    for(int i = 0; i < sizeOfPDE; i++)
    {
        new_node.PDE[i] = 0;
        for(int j = 0; j < sizeOfPDE; j++)
        {
            new_node.PDE[i] += matrix->getU1(i,j) * omega[j]; }
    }
    LOG_TRACE("Calc done");
	delete[] omega;
};
