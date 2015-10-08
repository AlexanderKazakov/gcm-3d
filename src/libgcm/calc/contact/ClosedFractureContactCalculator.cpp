#include "libgcm/calc/contact/ClosedFractureContactCalculator.hpp"


using namespace gcm;
using std::vector;

ClosedFractureContactCalculator::ClosedFractureContactCalculator()
{
	type = "ClosedFractureContactCalculator";

    adhesionCalc = new AdhesionContactCalculator();
	slidingCalc = new SlidingContactCalculator();
};

ClosedFractureContactCalculator::~ClosedFractureContactCalculator()
{
    delete adhesionCalc;
	delete slidingCalc;
};

void ClosedFractureContactCalculator::setFracArea(Area* area) {
	fracArea = area;
};

void ClosedFractureContactCalculator::doCalc(Node& cur_node, Node& new_node, 
	Node& virt_node, RheologyMatrixPtr matrix, vector<Node>& previousNodes, 
	bool inner[], RheologyMatrixPtr virt_matrix, vector<Node>& virtPreviousNodes,
	bool virt_inner[], float outer_normal[], float scale) 
{
	if(fracArea->isInArea(cur_node))
		slidingCalc->doCalc(cur_node, new_node, virt_node, matrix, previousNodes, inner,
							 virt_matrix, virtPreviousNodes, virt_inner, outer_normal, scale);
	else
		adhesionCalc->doCalc(cur_node, new_node, virt_node, matrix, previousNodes, inner,
							 virt_matrix, virtPreviousNodes, virt_inner, outer_normal, scale);
};
