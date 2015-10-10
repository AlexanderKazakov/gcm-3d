#include "libgcm/mesh/Mesh.hpp"

#include "libgcm/node/Node.hpp"
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
    nodesNumber = 0;
    nodesStorageSize = 0;
    movable = false;
}

Mesh::Mesh(string _type) : type(_type)
{
    INIT_LOGGER("gcm.Mesh");
    calc = false;
    nodesNumber = 0;
    nodesStorageSize = 0;
    movable = false;
}

Mesh::~Mesh()
{

}

string Mesh::getType()
{
    return type;
}

void Mesh::setId(string id)
{
    this->id = id;
}

string Mesh::getId() const
{
    return id;
}

void Mesh::setCalc(bool calc)
{
    this->calc = calc;
}

bool Mesh::getCalc()
{
    return calc;
}

void Mesh::setMovable(bool movable)
{
    this->movable = movable;
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
        node.bodyId = id;
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

void Mesh::initNewNodes()
{
    for(uint i = 0; i < getNodesNumber(); i++)
    {
        Node& node = getNodeByLocalIndex(i);
        Node& newNode = getNewNode( node.number );
        newNode.coords = node.coords;
        copy(node.PDE, node.PDE + node.getSizeOfPDE(), newNode.PDE);
        copy(node.ODE, node.ODE + node.getSizeOfODE(), newNode.ODE);
        newNode.setMaterialId(node.getMaterialId());
        newNode.setContactConditionId(node.getContactConditionId());
    }
}

void Mesh::preProcess()
{
    LOG_DEBUG("Preprocessing mesh started.");
    initNewNodes();
    calcMinH();
	createOutline();
    preProcessGeometry();
    LOG_DEBUG("Preprocessing mesh done.");
    logMeshStats();
}

void Mesh::createOutline()
{
    int nodesNumber = getNodesNumber();
    if (nodesNumber > 0)
    {
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
    for(uint i = 0; i < getNodesNumber(); i++)
    {
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
    }
}

void Mesh::setContactCondition(Area* area, uint num)
{
    for(uint i = 0; i < getNodesNumber(); i++)
    {
        Node& node = getNodeByLocalIndex(i);
        if( area->isInArea( node ) )
            node.setContactConditionId(num);
    }
}

void Mesh::setRheologyModel(RheologyModel* _model) {
	rheologyModel = _model;
}

RheologyModel* Mesh::getRheologyModel() {
	// TODO@next what to do with rheologyModels, materials, matIds, etc..
	return rheologyModel;
}

void Mesh::setRheology(uchar matId) {
    for(uint i = 0; i < getNodesNumber(); i++)
    {
        Node& node = getNodeByLocalIndex(i);
        node.setMaterialId( matId );
    }
}

void Mesh::setRheology(uchar matId, Area* area) {
    for(uint i = 0; i < getNodesNumber(); i++)
    {
        Node& node = getNodeByLocalIndex(i);
        if( area->isInArea(node) )
        {
            node.setMaterialId( matId );
        }
    }
}

void Mesh::transfer(float x, float y, float z)
{
    for(uint i = 0; i < getNodesNumber(); i++)
    {
        Node& node = getNodeByLocalIndex(i);
        node.coords[0] += x;
        node.coords[1] += y;
        node.coords[2] += z;
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
		float scaleX, float scaleY, float scaleZ)
{
    for(uint i = 0; i < getNodesNumber(); i++)
    {
        Node& node = getNodeByLocalIndex(i);
        node.coords[0] = (node.coords[0] - x0)*scaleX + x0;
        node.coords[1] = (node.coords[1] - y0)*scaleY + y0;
        node.coords[2] = (node.coords[2] - z0)*scaleZ + z0;
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
    for(uint i = 0; i < getNodesNumber(); i++)
    {
        Node& node = getNodeByLocalIndex(i);
        if( node.isLocal() )
            node.clearErrorFlags();
    }
};

void Mesh::clearContactState()
{
    for(uint i = 0; i < getNodesNumber(); i++)
    {
        Node& node = getNodeByLocalIndex(i);
        if( node.isLocal() )
                        node.setInContact(false);
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
    LOG_DEBUG("Moving mesh coords");
    for(uint i = 0; i < getNodesNumber(); i++)
    {
        Node& node = getNodeByLocalIndex(i);
        if( node.isLocal() && node.isFirstOrder() )
        {
            Node& newNode = getNewNode( node.number );
            for(int j = 0; j < 3; j++)
            {
                // Move node
                node.coords[j] += node.PDE[j]*tau;
                newNode.coords[j] = node.coords[j];
                // Move mesh outline if necessary
                // TODO - this does not 'clean' outline areas where there is no nodes anymore
                if(node.coords[j] > outline.max_coords[j])
                    outline.max_coords[j] = node.coords[j];
                if(node.coords[j] < outline.min_coords[j])
                    outline.min_coords[j] = node.coords[j];
                if(node.coords[j] > expandedOutline.max_coords[j])
                    expandedOutline.max_coords[j] = node.coords[j];
                if(node.coords[j] < expandedOutline.min_coords[j])
                    expandedOutline.min_coords[j] = node.coords[j];
            }
        }
    }
    calcMinH();
    LOG_DEBUG("New outline: " << outline);
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
    return nodesNumber;
}

uint Mesh::getNumberOfLocalNodes()
{
    uint num = 0;
    for(uint i = 0; i < getNodesNumber(); i++)
    {
        // FIXME this code seems to be dead
        // node = getNodeByLocalIndex(i);
        if( getNodeByLocalIndex(i).isLocal() )
            num++;
    }
    return num;
}

void Mesh::createNodes(uint number) {
    LOG_DEBUG("Creating nodes storage, size: " << (uint)(number*STORAGE_OVERCOMMIT_RATIO));
    nodes.resize((uint)(number*STORAGE_OVERCOMMIT_RATIO));
    new_nodes.resize((uint)(number*STORAGE_OVERCOMMIT_RATIO));
    nodesStorageSize = number*STORAGE_OVERCOMMIT_RATIO;
}

bool Mesh::hasNode(uint index)
{
    assert_ge(index, 0 );
    unordered_map<uint, uint>::const_iterator itr;
    itr = nodesMap.find(index);
    return itr != nodesMap.end();
}

Node& Mesh::getNode(uint index)
{
    assert_ge(index, 0 );
    unordered_map<uint, uint>::const_iterator itr;
    itr = nodesMap.find(index);
    assert_true(itr != nodesMap.end() );
    return nodes[itr->second];
}

Node& Mesh::getNewNode(uint index) {
    assert_ge(index, 0 );
    unordered_map<uint, uint>::const_iterator itr;
    itr = nodesMap.find(index);
    assert_true(itr != nodesMap.end() );
    return new_nodes[itr->second];
}

Node& Mesh::getNodeByLocalIndex(uint index) {
    assert_ge(index, 0);
    assert_lt(index, nodes.size());
    return nodes[index];
}

uint Mesh::getNodeLocalIndex(uint index) const {
    assert_ge(index, 0 );
    unordered_map<uint, uint>::const_iterator itr;
    itr = nodesMap.find(index);
    return ( itr != nodesMap.end() ? itr->second : -1 );
}

void Mesh::addNode(Node& node) {
    if( nodesNumber == nodesStorageSize )
        // FIXME what is this?
        // why not to use a propper allocator for container?
        createNodes((nodesStorageSize+1)*STORAGE_ONDEMAND_GROW_RATE);
    assert_lt(nodesNumber, nodesStorageSize );
    nodes[nodesNumber] = node;
    nodesMap[node.number] = nodesNumber;
    nodesNumber++;
}

const SnapshotWriter& Mesh::getSnapshotter2() const {
    THROW_UNSUPPORTED("Not implemented");
}

void Mesh::defaultNextPartStep(float tau, int stage)
{
    LOG_DEBUG("Nodes: " << nodesNumber);

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
    for( MapIter itr = nodesMap.begin(); itr != nodesMap.end(); ++itr ) {
        uint i = itr->first;
        Node& node = getNode(i);
        if( node.isLocal() && node.isBorder() )
			method->doNextPartStep( node, getNewNode(i), tau, stage, this );
    }

    // Inner nodes
    LOG_DEBUG("Processing inner nodes");
    for( MapIter itr = nodesMap.begin(); itr != nodesMap.end(); ++itr ) {
        uint i = itr->first;
        Node& node = getNode(i);
        if( node.isLocal() && node.isInner() )
                method->doNextPartStep( node, getNewNode(i), tau, stage, this );
    }
}

void Mesh::copyValues() {
	LOG_DEBUG("Copying PDE");
    for( MapIter itr = nodesMap.begin(); itr != nodesMap.end(); ++itr ) {
        uint i = itr->first;
        Node& node = getNode(i);
        if( node.isLocal() )
            memcpy( node.PDE, getNewNode(i).PDE, node.getSizeOfPDE() * sizeof(float) );
    }
}
