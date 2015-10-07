#include "libgcm/rheology/matrixSetters/FiniteStrainElasticRheologyMatrixSetter.hpp"

using namespace gcm;
unsigned int FiniteStrainElasticRheologyMatrixSetter::getNumberOfStates() const
{
    return 0;
}

unsigned int FiniteStrainElasticRheologyMatrixSetter::
                                    getStateForNode(const Node& node) const
{
    return 0;
}

void FiniteStrainElasticRheologyMatrixSetter::setX(GcmMatrix& a, 
                                                  const MaterialPtr& material, 
                                                  const Node& node)
{
    a.clear();
	
	auto p = material->getRheologyProperties();
	const FiniteStrainIdealElasticNode &node1 = 
	                  static_cast<const FiniteStrainIdealElasticNode&> (node);
	auto rho = node1.getRho();
	auto sxx = node1.getSxx();
	auto sxy = node1.getSxy();
	auto sxz = node1.getSxz();
	auto syy = node1.getSyy();
	auto syz = node1.getSyz();
	auto szz = node1.getSzz();
	
	a(0,3) = a(1,4) = a(2,5) = -1.0/rho;
	
	a(3,0) = -p.c11;  a(3,1) = -p.c16-sxy;          a(3,2) = -p.c15-sxz;
	a(4,0) = -p.c16;  a(4,1) = -p.c66-(syy-sxx)/2;  a(4,2) = -p.c56-syz/2;
	a(5,0) = -p.c15;  a(5,1) = -p.c56-syz/2;        a(5,2) = -p.c55-(szz-sxx)/2;
	a(6,0) = -p.c12;  a(6,1) = -p.c26+sxy;          a(6,2) = -p.c25;
	a(7,0) = -p.c14;  a(7,1) = -p.c46+sxz/2;        a(7,2) = -p.c45+sxy/2;
	a(8,0) = -p.c13;  a(8,1) = -p.c36;              a(8,2) = -p.c35+sxz;
	
	a(9,0) = rho;
}

void FiniteStrainElasticRheologyMatrixSetter::setY(GcmMatrix& a, 
                                                  const MaterialPtr& material, 
                                                  const Node& node)
{
    a.clear();
    
    auto p = material->getRheologyProperties();
	const FiniteStrainIdealElasticNode &node1 = 
	                  static_cast<const FiniteStrainIdealElasticNode&> (node);
	auto rho = node1.getRho();
	auto sxx = node1.getSxx();
	auto sxy = node1.getSxy();
	auto sxz = node1.getSxz();
	auto syy = node1.getSyy();
	auto syz = node1.getSyz();
	auto szz = node1.getSzz();
	
	a(0,4) = a(1,6) = a(2,7) = -1.0/rho;
	
	a(3,0) = -p.c16+sxy;          a(3,1) = -p.c12;  a(3,2) = -p.c14;
	a(4,0) = -p.c66-(sxx-syy)/2;  a(4,1) = -p.c26;  a(4,2) = -p.c46-sxz/2;
	a(5,0) = -p.c56+syz/2;        a(5,1) = -p.c25;  a(5,2) = -p.c45+sxy/2;
	a(6,0) = -p.c26-sxy;          a(6,1) = -p.c22;  a(6,2) = -p.c24-syz;
	a(7,0) = -p.c46-sxz/2;        a(7,1) = -p.c24;  a(7,2) = -p.c44-(szz-syy)/2;
	a(8,0) = -p.c36;              a(8,1) = -p.c23;  a(8,2) = -p.c34+syz;
	
	a(9,1) = rho;
}

void FiniteStrainElasticRheologyMatrixSetter::setZ(GcmMatrix& a, 
                                                  const MaterialPtr& material, 
                                                  const Node& node)
{
    a.clear();
    
    auto p = material->getRheologyProperties();
	const FiniteStrainIdealElasticNode &node1 = 
	                  static_cast<const FiniteStrainIdealElasticNode&> (node);
	auto rho = node1.getRho();
	auto sxx = node1.getSxx();
	auto sxy = node1.getSxy();
	auto sxz = node1.getSxz();
	auto syy = node1.getSyy();
	auto syz = node1.getSyz();
	auto szz = node1.getSzz();
	
	a(0,5) = a(1,7) = a(2,8) = -1.0/rho;
	
	a(3,0) = -p.c15+sxz;          a(3,1) = -p.c14;              a(3,2) = -p.c13;
	a(4,0) = -p.c56+syz/2;        a(4,1) = -p.c46+sxz/2;        a(4,2) = -p.c36;
	a(5,0) = -p.c55-(sxx-szz)/2;  a(5,1) = -p.c45-sxy/2;        a(5,2) = -p.c35;
	a(6,0) = -p.c25;              a(6,1) = -p.c24+syz;          a(6,2) = -p.c23;
	a(7,0) = -p.c45-sxy/2;        a(7,1) = -p.c44-(syy-szz)/2;  a(7,2) = -p.c34;
	a(8,0) = -p.c35-sxz;          a(8,1) = -p.c34-syz;          a(8,2) = -p.c33;
	
	a(9,2) = rho;
}