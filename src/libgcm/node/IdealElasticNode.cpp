#include "libgcm/node/IdealElasticNode.hpp"


using namespace gcm;

IdealElasticNode::IdealElasticNode() : Node(9, 0, gcm::IDEAL_ELASTIC_NODE_TYPE)
{
}

IdealElasticNode::IdealElasticNode(uchar numOfPDE, uchar numOfODE, uchar type) :
                                              Node(numOfPDE, numOfODE, type)
{
}

// Read-only get-ters

real IdealElasticNode::getVx() const {
    return PDE[0];
}

real IdealElasticNode::getVy() const {
    return PDE[1];
}

real IdealElasticNode::getVz() const {
    return PDE[2];
}

real IdealElasticNode::getSxx() const {
    return PDE[3];
}

real IdealElasticNode::getSxy() const {
    return PDE[4];
}

real IdealElasticNode::getSxz() const {
    return PDE[5];
}

real IdealElasticNode::getSyy() const {
    return PDE[6];
}

real IdealElasticNode::getSyz() const {
    return PDE[7];
}

real IdealElasticNode::getSzz() const {
    return PDE[8];
}

// Read-write access

real& IdealElasticNode::vx() {
    return PDE[0];
}

real& IdealElasticNode::vy() {
    return PDE[1];
}

real& IdealElasticNode::vz() {
    return PDE[2];
}

real& IdealElasticNode::sxx() {
    return PDE[3];
}

real& IdealElasticNode::sxy() {
    return PDE[4];
}

real& IdealElasticNode::sxz() {
    return PDE[5];
}

real& IdealElasticNode::syy() {
    return PDE[6];
}

real& IdealElasticNode::syz() {
    return PDE[7];
}

real& IdealElasticNode::szz() {
    return PDE[8];
}

// Density from material

/*real IdealElasticNode::getRho() const {
    
}*/

// Different stress representations

real IdealElasticNode::getCompression() const
{
    real compression = 0;
    vector3r s = getPrincipalStresses();

    for (int i = 0; i < 3; i++)
        if (s[i] < compression)
            compression = s[i];

    return fabs(compression);
}

real IdealElasticNode::getTension() const
{
    real tension = 0;
    vector3r s = getPrincipalStresses();

    for (int i = 0; i < 3; i++)
        if (s[i] > tension)
            tension = s[i];

    return tension;
}

// See http://www.toehelp.ru/theory/sopromat/6.html for details
real IdealElasticNode::getShear() const
{
    real shear = 0;
    real t[3];
    vector3r s = getPrincipalStresses();

    t[0] = 0.5 * fabs(s[1] - s[0]);
    t[1] = 0.5 * fabs(s[2] - s[0]);
    t[2] = 0.5 * fabs(s[2] - s[1]);

    for (int i = 0; i < 3; i++)
        if (t[i] > shear)
            shear = t[i];

    return shear;
}
real IdealElasticNode::getStressDeviator() const
{
    return sqrt(((getSxx() - getSyy()) * (getSxx() - getSyy()) + (getSyy() - getSzz()) 
            * (getSyy() - getSzz()) + (getSxx() - getSzz()) * (getSxx() - getSzz())
            + 6 * (getSxy() * getSxy() + getSxz() * getSxz() + getSyz() * getSyz())) / 6);
}

real IdealElasticNode::getPressure() const
{
    return -(getSxx() + getSyy() + getSzz()) / 3;
}

// See http://www.toehelp.ru/theory/sopromat/6.html
vector3r IdealElasticNode::getPrincipalStresses() const
{
    double mainStresses[3];
    double a = -getStressInvariantJ1();
    double b = getStressInvariantJ2();
    double c = -getStressInvariantJ3();
    
	solvePolynomialThirdOrder(a, b, c, mainStresses[0], mainStresses[1], mainStresses[2]);

    return vector3r(mainStresses[0], mainStresses[1], mainStresses[2]);
}

real IdealElasticNode::getStressInvariantJ1() const
{
    return getSxx() + getSyy() + getSzz();
}

real IdealElasticNode::getStressInvariantJ2() const
{
    return getSxx() * getSyy() + getSxx() * getSzz() + getSyy() * getSzz() 
            - (getSxy() * getSxy() + getSxz() * getSxz() + getSyz() * getSyz());
}

real IdealElasticNode::getStressInvariantJ3() const
{
    return getSxx() * getSyy() * getSzz() + 2 * getSxy() * getSxz() * getSyz() 
            - getSxx() * getSyz() * getSyz() - getSyy() * getSxz() * getSxz() 
            - getSzz() * getSxy() * getSxy();
}