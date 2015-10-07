#include "libgcm/rheology/rightHandSideSetters/MaxwellViscosityRightHandSideSetter.hpp"

using namespace gcm;


void MaxwellViscosityRightHandSideSetter::setVector(Node& f, 
                                                    const Node& node) const {
	f.clear();
	// property of viscosity - relaxation time
	real tau = 1; // TODO - get tau from Material node.getMaterial();
	const IdealElasticNode &node1 = static_cast<const IdealElasticNode&> (node);
	IdealElasticNode &f1 = static_cast<IdealElasticNode&> (f);
	f1.sxx() = - node1.getSxx() / tau;
	f1.sxy() = - node1.getSxy() / tau;
	f1.sxz() = - node1.getSxz() / tau;
	f1.syy() = - node1.getSyy() / tau;
	f1.syz() = - node1.getSyz() / tau;
	f1.szz() = - node1.getSzz() / tau;
}