#include "libgcm/mesh/Mesh.hpp"

#include "libgcm/Engine.hpp"
#include "libgcm/method/InterpolationFixedAxis.hpp"

using namespace gcm;
using std::string;
using std::copy;
using std::numeric_limits;
using std::min;
using std::max;
using std::unordered_map;
using std::function;

Mesh::Mesh()
{
    INIT_LOGGER("gcm.Mesh");
    calc = false;
    movable = false;
	rheologyModel = NULL;
}

Mesh::Mesh(string _type) : type(_type)
{
    INIT_LOGGER("gcm.Mesh");
    calc = false;
    movable = false;
	rheologyModel = NULL;
}

Mesh::~Mesh() {}

void Mesh::allocateMemoryForNodalData() {
	assert_eq(nodeStorage.getSize(), newNodeStorage.getSize());
	nodeStorage.createMemory();
	newNodeStorage.createMemory();
}

void Mesh::createNodes(uint n) {
	nodeStorage.reserve(n);
	newNodeStorage.reserve(n);
}

string Mesh::getType()
{
    return type;
}

string Mesh::getId() const
{
    return id;
}

bool Mesh::getMovable()
{
    return movable;
}

void Mesh::setBody(Body* body)
{
    this->body = body;
}

void Mesh::setBodyNum(uchar id)
{
    for(uint i = 0; i < getNodesNumber(); i++)
    {
        Node& node = getNodeByLocalIndex(i);
	    Node& newNode = getNewNodeByLocalIndex(i);
        newNode.bodyId = node.bodyId = id;
    }
}

Body* Mesh::getBody()
{
    return body;
}

string Mesh::snapshot(int number)
{
    try {
        getSnapshotter2().dump(this, number);
    } catch (Exception &e)
    {
    }

    return getSnaphotter().dump(this, number);
}

string Mesh::dump(int number)
{
    return getDumper().dump(this, number);
}

// FIXME
// should it be const reference instead of copy?
AABB Mesh::getOutline()
{
    return outline;
}

const AABB& Mesh::getExpandedOutline() const
{
    return expandedOutline;
}

void Mesh::preProcess()
{
    LOG_DEBUG("Preprocessing mesh started.");
    calcMinH();
	createOutline();
    preProcessGeometry();
    LOG_DEBUG("Preprocessing mesh done.");
    logMeshStats();
}

void Mesh::createOutline()
{
    int nodesNumber = getNodesNumber();
    if (nodesNumber > 0) {
        LOG_DEBUG("Creating outline");

        // Create outline
        for(int j = 0; j < 3; j++)
        {
            outline.min_coords[j] = numeric_limits<float>::infinity();
            outline.max_coords[j] = - numeric_limits<float>::infinity();
            expandedOutline.min_coords[j] = numeric_limits<float>::infinity();
            expandedOutline.max_coords[j] = - numeric_limits<float>::infinity();
        }

        for(uint i = 0; i < getNodesNumber(); i++)
        {
            Node& node = getNodeByLocalIndex(i);
            if( node.isLocal() )
            {
                for(int j = 0; j < 3; j++) {
                    if(node.coords[j] > outline.max_coords[j])
                        outline.max_coords[j] = node.coords[j];
                    if(node.coords[j] < outline.min_coords[j])
                        outline.min_coords[j] = node.coords[j];
                }
            }
            for(int j = 0; j < 3; j++) 
                if (node.isUsed()) {
                    if(node.coords[j] > expandedOutline.max_coords[j])
                        expandedOutline.max_coords[j] = node.coords[j];
                    if(node.coords[j] < expandedOutline.min_coords[j])
                        expandedOutline.min_coords[j] = node.coords[j];
                }
        }
    } else
    {
        LOG_DEBUG ("Mesh is empty, no outline to create");
    }
}

void Mesh::setInitialState(Area* area, float* PDE)
{
    for(uint i = 0; i < getNodesNumber(); i++) {
        Node& node = getNodeByLocalIndex(i);
        if( area->isInArea( node ) )
            for( uint k = 0; k < node.getSizeOfPDE(); k++ )
                node.PDE[k] = PDE[k];
    }
}
        
void Mesh::setInitialState(Area* area, std::function<void(Node& node)> setter)
{
    for(uint i = 0; i < getNodesNumber(); i++)
    {
        Node& node = getNodeByLocalIndex(i);
        if (area->isInArea(node))
            setter(node);
    }
}

void Mesh::setBorderCondition(Area* area, uint num)
{
    for(uint i = 0; i < getNodesNumber(); i++)
    {
        Node& node = getNodeByLocalIndex(i);
        if( area->isInArea( node ) )
            node.setBorderConditionId(num);
	    Node& newnode = getNewNodeByLocalIndex(i);
	    if( area->isInArea( newnode ) )
		    newnode.setBorderConditionId(num);
    }
}

void Mesh::setContactCondition(Area* area, uint num)
{
    for(uint i = 0; i < getNodesNumber(); i++)
    {
        Node& node = getNodeByLocalIndex(i);
        if( area->isInArea( node ) )
            node.setContactConditionId(num);
	    Node& newnode = getNewNodeByLocalIndex(i);
	    if( area->isInArea( newnode ) )
		    newnode.setContactConditionId(num);
    }
}


RheologyModel* Mesh::getRheologyModel() {
	// TODO@next what to do with rheologyModels, materials, matIds, etc..
	return rheologyModel;
}

void Mesh::setMaterial(uchar matId) {
    for(uint i = 0; i < getNodesNumber(); i++) {
	    getNodeByLocalIndex(i).setMaterialId( matId );
	    getNewNodeByLocalIndex(i).setMaterialId( matId );
    }
}

void Mesh::setMaterial(uchar matId, Area* area) {
    for(uint i = 0; i < getNodesNumber(); i++) {
        Node& node = getNodeByLocalIndex(i);
        if( area->isInArea(node) ) {
            node.setMaterialId( matId );
        }
	    Node& newnode = getNewNodeByLocalIndex(i);
	    if( area->isInArea(newnode) ) {
		    newnode.setMaterialId( matId );
	    }
    }
}

void Mesh::transfer(float x, float y, float z)
{
    for(uint i = 0; i < getNodesNumber(); i++) {
        Node& node = getNodeByLocalIndex(i);
        node.coords[0] += x;
        node.coords[1] += y;
        node.coords[2] += z;
	    Node& newnode = getNewNodeByLocalIndex(i);
	    newnode.coords[0] += x;
	    newnode.coords[1] += y;
	    newnode.coords[2] += z;
    }
    if( !isinf(outline.minX) )
    {
        outline.transfer(x, y, z);
    }
    if( !isinf(expandedOutline.minX) )
    {
        expandedOutline.transfer(x, y, z);
    }
    if( !isinf(syncedArea.minX) )
    {
        syncedArea.transfer(x, y, z);
    }
    if( !isinf(areaOfInterest.minX) )
    {
        areaOfInterest.transfer(x, y, z);
    }

    // TODO@avasyukov - think about additional checks
    Engine::getInstance().transferScene(x, y, z);
}

void Mesh::scale(float x0, float y0, float z0, 
		float scaleX, float scaleY, float scaleZ) {
    for(uint i = 0; i < getNodesNumber(); i++) {
        Node& node = getNodeByLocalIndex(i);
        node.coords[0] = (node.coords[0] - x0)*scaleX + x0;
        node.coords[1] = (node.coords[1] - y0)*scaleY + y0;
        node.coords[2] = (node.coords[2] - z0)*scaleZ + z0;
	    Node& newnode = getNewNodeByLocalIndex(i);
	    newnode.coords[0] = (newnode.coords[0] - x0)*scaleX + x0;
	    newnode.coords[1] = (newnode.coords[1] - y0)*scaleY + y0;
	    newnode.coords[2] = (newnode.coords[2] - z0)*scaleZ + z0;
    }
    if( !isinf(outline.minX) )
    {
        outline.scale(x0, y0, z0, scaleX, scaleY, scaleZ);
    }
    if( !isinf(expandedOutline.minX) )
    {
        expandedOutline.scale(x0, y0, z0, scaleX, scaleY, scaleZ);
    }
    if( !isinf(syncedArea.minX) )
    {
        syncedArea.scale(x0, y0, z0, scaleX, scaleY, scaleZ);
    }
    if( !isinf(areaOfInterest.minX) )
    {
        areaOfInterest.scale(x0, y0, z0, scaleX, scaleY, scaleZ);
    }
    Engine::getInstance().scaleScene(x0, y0, z0, scaleX, scaleY, scaleZ);
}

void Mesh::applyRheology(RheologyCalculator* rc)
{
    for(uint i = 0; i < getNodesNumber(); i++)
    {
        Node& node = getNodeByLocalIndex(i);
        if( node.isLocal() )
            rc->doCalc(node, node);
    }
}

void Mesh::clearNodesState()
{
    for(uint i = 0; i < getNodesNumber(); i++) {
        Node& node = getNodeByLocalIndex(i);
        if( node.isLocal() )
            node.clearErrorFlags();
	    Node& newnode = getNewNodeByLocalIndex(i);
	    if( newnode.isLocal() )
		    newnode.clearErrorFlags();
    }
};

void Mesh::clearContactState()
{
    for(uint i = 0; i < getNodesNumber(); i++) {
        Node& node = getNodeByLocalIndex(i);
        if( node.isLocal() )
                        node.setInContact(false);
	    Node& newnode = getNewNodeByLocalIndex(i);
	    if( newnode.isLocal() )
		    newnode.setInContact(false);
    }
}

//void Mesh::processMaterialFailure(FailureModel* failureModel, const float tau)
//{
//    for(int i = 0; i < getNodesNumber(); i++)
//    {
//        Node& node = getNodeByLocalIndex(i);
//        if( node.isLocal() && !node.isBorder())
//        {
//            failureModel->checkFailure(node, tau);
//            failureModel->applyCorrection(node, tau);
//        }
//    }
//}

void Mesh::applyCorrectors()
{
    for(uint i = 0; i < getNodesNumber(); i++)
    {
        Node& node = getNodeByLocalIndex(i);
        if( node.isLocal() )
        {
            // TODO@next - rheology model has to be here
			//node.getRheologyMatrix()->applyCorrector(node);
        }
    }
}

void Mesh::moveCoords(float tau)
{
	// TODO@next - with nodeStorages it needs to be implemented another
	THROW_UNSUPPORTED("not implemented");
//    LOG_DEBUG("Moving mesh coords");
//    for(uint i = 0; i < getNodesNumber(); i++)
//    {
//        Node& node = getNodeByLocalIndex(i);
//        if( node.isLocal() && node.isFirstOrder() )
//        {
//            Node& newNode = getNewNodeByGlobalIndex(node.number);
//            for(int j = 0; j < 3; j++)
//            {
//                // Move node
//                node.coords[j] += node.PDE[j]*tau;
//                newNode.coords[j] = node.coords[j];
//                // Move mesh outline if necessary
//                // TODO - this does not 'clean' outline areas where there is no nodes anymore
//                if(node.coords[j] > outline.max_coords[j])
//                    outline.max_coords[j] = node.coords[j];
//                if(node.coords[j] < outline.min_coords[j])
//                    outline.min_coords[j] = node.coords[j];
//                if(node.coords[j] > expandedOutline.max_coords[j])
//                    expandedOutline.max_coords[j] = node.coords[j];
//                if(node.coords[j] < expandedOutline.min_coords[j])
//                    expandedOutline.min_coords[j] = node.coords[j];
//            }
//        }
//    }
//    calcMinH();
//    LOG_DEBUG("New outline: " << outline);
};

float Mesh::getMaxEigenvalue()
{
    float maxLambda = 0;
    for(uint i = 0; i < getNodesNumber(); i++)
    {
        Node& node = getNodeByLocalIndex(i);
        if (!node.isUsed())
            continue;
        RheologyMatrixPtr m = node.getRheologyMatrix();
        m->decomposeX(node);
        auto l1 = m->getMaxEigenvalue();
        m->decomposeY(node);
        auto l2 = m->getMaxEigenvalue();
        m->decomposeZ(node);
        auto l3 = m->getMaxEigenvalue();
        maxLambda = max({maxLambda, l1, l2, l3});
    }
    return maxLambda;
}

float Mesh::getMaxPossibleTimeStep()
{
    auto maxLambda = getMaxEigenvalue();
    LOG_DEBUG( "Min H over mesh is " << getMinH() );
    LOG_DEBUG( "Max lambda over mesh is " << maxLambda );
    LOG_DEBUG( "Courant time step is " << getMinH() / maxLambda );
    return getMinH() / maxLambda;
}

uint Mesh::getNodesNumber()
{
	assert_eq(nodeStorage.getSize(), newNodeStorage.getSize());
    return nodeStorage.getSize();
}

uint Mesh::getNumberOfLocalNodes()
{
    uint num = 0;
    for(uint i = 0; i < getNodesNumber(); i++)
    {
        // FIXME this code seems to be dead
        if( getNodeByLocalIndex(i).isLocal() )
            num++;
    }
    return num;
}


bool Mesh::hasNodeWithGlobalIndex(uint index) {
	return nodeStorage.hasNodeWithGlobalIndex(index);
}

Node& Mesh::getNodeByGlobalIndex(uint index)
{
	return nodeStorage.getNodeByGlobalIndex(index);
}

Node& Mesh::getNewNodeByGlobalIndex(uint index) {
	return newNodeStorage.getNodeByGlobalIndex(index);
}

Node& Mesh::getNodeByLocalIndex(uint index) {
	return nodeStorage.getNodeByLocalIndex(index);
}

Node &Mesh::getNewNodeByLocalIndex(uint index) {
	return newNodeStorage.getNodeByLocalIndex(index);
}

uint Mesh::getNodeLocalIndex(uint index) const {
	return nodeStorage.getNodeLocalIndex(index);
}

void Mesh::addNode(Node& node) {
	nodeStorage.addNode(node);
	newNodeStorage.addNode(node);
}

void Mesh::setCalc(bool calc)
{
	this->calc = calc;
}

void Mesh::setId(string id)
{
	this->id = id;
}

void Mesh::setRheologyModel(RheologyModel* _model) {
	rheologyModel = _model;
}

const SnapshotWriter& Mesh::getSnapshotter2() const {
    THROW_UNSUPPORTED("Not implemented");
}

void Mesh::defaultNextPartStep(float tau, int stage)
{
	assert_eq(nodeStorage.getSize(), newNodeStorage.getSize());
    LOG_DEBUG("Nodes: " << nodeStorage.getSize());

    if( stage == 0 )
    {
        LOG_DEBUG("Clear error flags on all nodes");
        clearNodesState();
    }

    NumericalMethod *method = Engine::getInstance().getNumericalMethod(numericalMethodType);
    method->setSpaceOrder(numericalMethodOrder);

    if( Engine::getInstance().getNumberOfWorkers() != 1
            && ! syncedArea.includes( &areaOfInterest ) )
    {
        LOG_ERROR("Area of interest: " << areaOfInterest);
        LOG_ERROR("Synced area: " << syncedArea);
        assert_true(syncedArea.includes( &areaOfInterest ) );
    }

    // Border nodes
    LOG_DEBUG("Processing border nodes");
	for(uint i = 0; i < getNodesNumber(); i++) {
		Node& node = getNodeByLocalIndex(i);
		if( node.isLocal() && node.isBorder() )
			method->doNextPartStep( node, getNewNodeByLocalIndex(i), tau, stage, this );
	}

    // Inner nodes
    LOG_DEBUG("Processing inner nodes");
	for(uint i = 0; i < getNodesNumber(); i++) {
		Node& node = getNodeByLocalIndex(i);
		if( node.isLocal() && node.isInner() )
			method->doNextPartStep( node, getNewNodeByLocalIndex(i), tau, stage, this );
	}

}

void Mesh::copyValues() {
	LOG_DEBUG("Copying values");
	// TODO@next - how to make it beautiful?
//	std::swap(nodeStorage, newNodeStorage);

	uchar sizeOfPDE = getNodeByLocalIndex(0).getSizeOfPDE();
	uchar sizeOfODE = getNodeByLocalIndex(0).getSizeOfODE();
	for(uint i = 0; i < getNodesNumber(); i++) {
		for(uchar j = 0; j < sizeOfPDE; j++) {
			getNodeByLocalIndex(i).PDE[j] = getNewNodeByLocalIndex(i).PDE[j];
		}
		for(uchar j = 0; j < sizeOfODE; j++) {
			getNodeByLocalIndex(i).ODE[j] = getNewNodeByLocalIndex(i).ODE[j];
		}
	}
}
