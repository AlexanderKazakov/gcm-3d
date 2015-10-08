#include "libgcm/calc/border/SmoothBorderCalculator.hpp"

#include "libgcm/node/Node.hpp"

using namespace gcm;
using std::vector;

SmoothBorderCalculator::SmoothBorderCalculator()
{
    INIT_LOGGER( "gcm.SmoothBorderCalculator" );
};

SmoothBorderCalculator::~SmoothBorderCalculator()
{
};

void SmoothBorderCalculator::setParameters(const xml::Node& params)
{
    
};

void SmoothBorderCalculator::doCalc(Node& cur_node, Node& new_node, RheologyMatrixPtr matrix,
                            vector<Node>& previousNodes, bool inner[],
                            float outer_normal[], float scale)
{
	THROW_UNSUPPORTED("Not implemented");
//    assert_eq(previousNodes.size(), 9);
//
//    LOG_TRACE("Starting calc with SmoothBorderCalculator");
//
//    int inner_node_num = -1;
//    int outer_count = 0;
//    for(int i = 0; i < 9; i++)
//        if( inner[i] )
//            inner_node_num = i;
//        else
//            outer_count++;
//
//    assert_ne(outer_count, 3 );
//
//    for(int j = 0; j < 9; j++)
//        new_node.PDE[j] = previousNodes[inner_node_num].PDE[j];
//
//    LOG_TRACE("Calc done");
};
