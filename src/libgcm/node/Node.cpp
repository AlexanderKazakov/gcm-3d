#include "libgcm/node/Node.hpp"

using namespace gcm;
using std::copy;

Node::Node() {
	ODE = PDE = NULL;
}

Node::Node(uchar sizeOfValuesInPDE,
                   uchar sizeOfValuesInODE,
                   uchar nodeType) :
                   sizeOfPDE(sizeOfValuesInPDE), 
                   sizeOfODE(sizeOfValuesInODE),
                   nodeType(nodeType)
{
	ODE = PDE = NULL;
}

Node::~Node()
{
    // No delete[] calls here, since we use placement new 
}

void Node::clear() {
	memset(PDE, 0, sizeOfPDE * sizeof (gcm::real));
	memset(ODE, 0, sizeOfODE * sizeof (gcm::real));
}

void Node::initMemory(real *buffer, int nodeNum) {
    real* startAddr = buffer + nodeNum * (sizeOfPDE + sizeOfODE);
    PDE = new (startAddr) real[sizeOfPDE];
    ODE = new (startAddr + sizeOfPDE) real[sizeOfODE];
}


void Node::operator=(const Node& orig) {
    assert_true(nodeType == orig.nodeType);
    assert_true(sizeOfPDE == orig.sizeOfPDE);
    assert_true(sizeOfODE == orig.sizeOfODE);
    assert_true(PDE != NULL);
    assert_true(ODE != NULL);
    publicFlags = orig.publicFlags;
    errorFlags = orig.errorFlags;
    borderConditionId = orig.borderConditionId;
    contactConditionId = orig.contactConditionId;
    bodyId = orig.bodyId;
    materialId = orig.materialId;
    rheologyMatrix = orig.rheologyMatrix;
    number = orig.number;
    coords = orig.coords;
    for (int i = 0; i < sizeOfPDE; i++)
        PDE[i] = orig.PDE[i];
    for (int i = 0; i < sizeOfODE; i++)
        ODE[i] = orig.ODE[i];
}

uchar Node::getType() const
{
    return nodeType;
}

uchar Node::getSizeOfPDE() const
{
    return sizeOfPDE;
}

uchar Node::getSizeOfODE() const
{
    return sizeOfODE;
}

uchar Node::getPublicFlags() const
{
    return publicFlags.flags;
}

void Node::setPublicFlags(uchar flags)
{
    publicFlags.flags = flags;
}

uchar Node::getErrorFlags() const
{
    return errorFlags.flags;
}

void Node::setErrorFlags(uchar flags)
{
    errorFlags.flags = flags;
}

void Node::setBorderConditionId(uchar newBorderCondId)
{
    borderConditionId = newBorderCondId;
}

uchar Node::getBorderConditionId() const
{
    return borderConditionId;
}

void Node::setContactConditionId(uchar newContactConditionId)
{
    contactConditionId = newContactConditionId;
}

uchar Node::getContactConditionId() const
{
    return contactConditionId;
}

void Node::setMaterialId(uchar id)
{
    materialId = id;
    //rho = Engine::getInstance().getMaterial(id)->getRho();
}

uchar Node::getMaterialId() const
{
    return materialId;
}

void Node::setRheologyMatrix(RheologyMatrixPtr matrix)
{
    assert_true(matrix.get());
    rheologyMatrix = matrix;
}

RheologyMatrixPtr Node::getRheologyMatrix() const
{
    return rheologyMatrix;
}

/*MaterialPtr CalcNode::getMaterial() const
{
    return Engine::getInstance().getMaterial(materialId);
}*/

bool Node::isUsed() const
{
    return publicFlags.isUsed;
}

void Node::setUsed(bool value)
{
    publicFlags.isUsed = value;
}

bool Node::isHighOrder() const
{
    return publicFlags.isHighOrder == 0;
}

void Node::setHighOrder(bool value)
{
    publicFlags.isHighOrder = value;
}

bool Node::isVirtual() const
{
    return publicFlags.isVirtual;
}

void Node::setVirtual(bool value)
{
    publicFlags.isVirtual = value;
}

bool Node::isBorder() const
{
    return publicFlags.isBorder;
}

bool Node::isInner() const
{
    return !isBorder();
}

void Node::setBorder(bool value)
{
    publicFlags.isBorder = value;
}


bool Node::isInContact() const
{
    return publicFlags.isInContact;
}

void Node::setInContact(bool value)
{
    publicFlags.isInContact = value;
}

bool Node::isDestroyed() const
{
    return publicFlags.isDestroyed;
}

void Node::setDestroyed(bool value)
{
    publicFlags.isDestroyed = value;
}

bool Node::isContactDestroyed() const
{
    return publicFlags.isContactDestroyed;
}

void Node::setContactDestroyed(bool value)
{
    publicFlags.isContactDestroyed = value;
}

bool Node::getCustomFlag(uchar flag) const
{
    switch (flag) {
    case FLAG_1: return publicFlags.customFlag1;
    default: THROW_INVALID_ARG("Invalid flag specified");
    }
}

void Node::setCustomFlag(uchar flag, bool value)
{
    switch (flag) {
    case FLAG_1: publicFlags.customFlag1 = value; break;
    default: THROW_INVALID_ARG("Invalid flag specified");
    }
}

void Node::setBadNeighborsX()
{
    errorFlags.badNeighborsX = true;
}

void Node::setBadNeighborsY()
{
    errorFlags.badNeighborsY = true;
}

void Node::setBadNeighborsZ()
{
    errorFlags.badNeighborsZ = true;
}

void Node::setBadOuterNormal()
{
    errorFlags.badOuterNormal = true;
}

void Node::setBadNeighbors(unsigned int axisNum)
{
    switch (axisNum) {
    case 0: setBadNeighborsX();
        break;
    case 1: setBadNeighborsY();
        break;
    case 2: setBadNeighborsZ();
        break;
    default: THROW_INVALID_ARG("Invalid axis number specified");
    }
}

void Node::clearErrorFlags()
{
    errorFlags.flags = 0;
}