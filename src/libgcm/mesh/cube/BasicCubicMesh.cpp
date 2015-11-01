#include "libgcm/mesh/cube/BasicCubicMesh.hpp"

#include "libgcm/node/Node.hpp"
#include "libgcm/snapshot/VTKCubicSnapshotWriter.hpp"
#include "launcher/loaders/mesh/BasicCubicMeshLoader.hpp"

using namespace gcm;
using std::numeric_limits;
using std::pair;
using std::make_pair;
using std::vector;
using std::sort;
using std::max;
using std::min;


BasicCubicMesh::BasicCubicMesh() : Mesh(launcher::BasicCubicMeshLoader::MESH_TYPE)
{
    meshH = numeric_limits<float>::infinity();
    // FIXME - hardcoded name
    numericalMethodType = "InterpolationFixedAxis";
    // FIXME - hardcoded parameter
    numericalMethodOrder = 1;
    INIT_LOGGER("gcm.BasicCubicMesh");
    LOG_DEBUG("Creating mesh");
    interpolator1 = new LineFirstOrderInterpolator();
    interpolator2 = new LineSecondOrderInterpolator();
}

BasicCubicMesh::BasicCubicMesh(std::string _type) : Mesh(_type)
{
    meshH = numeric_limits<float>::infinity();
    // FIXME - hardcoded name
    numericalMethodType = "InterpolationFixedAxis";
    // FIXME - hardcoded parameter
    numericalMethodOrder = 1;
    INIT_LOGGER("gcm.BasicCubicMesh");
    LOG_DEBUG("Creating mesh");
    interpolator1 = new LineFirstOrderInterpolator();
    interpolator2 = new LineSecondOrderInterpolator();
}

BasicCubicMesh::~BasicCubicMesh()
{
    LOG_DEBUG("Destroying mesh '" << getId() << "'");
    delete interpolator1;
    delete interpolator2;
    LOG_DEBUG("Mesh destroyed");
}

void BasicCubicMesh::preProcessGeometry()
{
    LOG_DEBUG("Preprocessing mesh geometry started.");
	for(uint i = 0; i < getNodesNumber(); i++) {
		getNodeByLocalIndex(i).setBorder(false);
		getNewNodeByLocalIndex(i).setBorder(false);
	}
    for(uint i = 0; i < getNodesNumber(); i++) {
        Node& node = getNodeByLocalIndex(i);
        for( uint i = 0; i < 3; i ++) {
            if( ( fabs(node.coords[i] - outline.min_coords[i]) < EQUALITY_TOLERANCE )
             || ( fabs(node.coords[i] - outline.max_coords[i]) < EQUALITY_TOLERANCE ) )
            {
                node.setBorder(true); getNewNodeByLocalIndex(true);
            }
        }
    }
    LOG_DEBUG("Preprocessing mesh geometry done.");
}

void BasicCubicMesh::doNextPartStep(float tau, int stage)
{
    defaultNextPartStep(tau, stage);
};

void BasicCubicMesh::logMeshStats()
{
    if( isinf( getMinH() ) )
    {
        LOG_DEBUG("Mesh is empty");
        return;
    }

    LOG_DEBUG("Number of nodes: " << nodeStorage.getSize());
    LOG_DEBUG("Mesh h: " << getMinH());
    LOG_DEBUG("Mesh outline:" << outline);
    LOG_DEBUG("Mesh expanded outline:" << expandedOutline);
};

void BasicCubicMesh::checkTopology(float tau)
{
    // We do not need it for this mesh type
}

float BasicCubicMesh::getRecommendedTimeStep()
{
    return getMinH() / getMaxEigenvalue();
};

void BasicCubicMesh::calcMinH()
{
    if( getNodesNumber() < 2)
        return;

    Node& base = getNodeByLocalIndex(0);
    float h;

    // We suppose that mesh is uniform
    for(uint i = 1; i < getNodesNumber(); i++)
    {
        Node& node = getNodeByLocalIndex(i);
        h = distance(base.coords, node.coords);
        if( h < meshH )
            meshH = h;
    }

    // TODO - we should auto-scale mesh transparently in this case
    if( meshH < EQUALITY_TOLERANCE )
    {
        LOG_WARN("Mesh minH is too small: minH " << meshH << ", FP tolerance: " << EQUALITY_TOLERANCE);
        LOG_WARN("Fixing it automatically, but it can cause numerous intersting issues");
        meshH = 10 * EQUALITY_TOLERANCE;
    }
};

float BasicCubicMesh::getAvgH() {
	if( isinf( meshH ) )
        calcMinH();
    return meshH;
};

float BasicCubicMesh::getMinH()
{
    if( isinf( meshH ) )
        calcMinH();
    return meshH;
};

void BasicCubicMesh::findBorderNodeNormal(const Node& node, float* x, float* y, float* z, bool debug)
{
    assert_true(node.isBorder() );
	
    float normal[3];
    normal[0] = normal[1] = normal[2] = 0.0;

	uint i = node.contactDirection;
	for( uint counter = 0; counter < 3; counter ++) {
        if( fabs(node.coords[i] - outline.min_coords[i]) < EQUALITY_TOLERANCE ) {
            normal[i] = -1;
            break;
        }
        if( fabs(node.coords[i] - outline.max_coords[i]) < EQUALITY_TOLERANCE ) {
            normal[i] = 1;
            break;
        }
		i = (i + 1) % 3;
    }
    *x = normal[0];
    *y = normal[1];
    *z = normal[2];
};

uint BasicCubicMesh::findNeighbourPoint(Node& node, float dx, float dy, float dz,
	bool debug, float* coords, bool* innerPoint)
{
	uint meshSizeY = 1 + (outline.maxY - outline.minY + meshH * 0.1) / meshH;
	uint meshSizeZ = 1 + (outline.maxZ - outline.minZ + meshH * 0.1) / meshH;

    assert_le(vectorSquareNorm(dx, dy, dz), getMinH() * getMinH() * (1 + EQUALITY_TOLERANCE) );

    coords[0] = node.coords[0] + dx;
    coords[1] = node.coords[1] + dy;
    coords[2] = node.coords[2] + dz;

    if( !outline.isInAABB(coords[0], coords[1], coords[2]) )
    {
        //LOG_INFO("OUT");
        coords[0] = node.coords[0];
        coords[1] = node.coords[1];
        coords[2] = node.coords[2];
        *innerPoint = false;
        return -1;
    }

    uint neighNum = node.number;
    if ( dx > EQUALITY_TOLERANCE )
        neighNum += meshSizeY*meshSizeZ;
    else if ( dx < -EQUALITY_TOLERANCE )
        neighNum -= meshSizeY*meshSizeZ;
    else if ( dy > EQUALITY_TOLERANCE )
        neighNum += meshSizeZ;
    else if ( dy < -EQUALITY_TOLERANCE )
        neighNum -= meshSizeZ;
    else if ( dz > EQUALITY_TOLERANCE )
        neighNum += 1;
    else if ( dz < -EQUALITY_TOLERANCE )
        neighNum -= 1;

    *innerPoint = true;
    return neighNum;
};

bool BasicCubicMesh::interpolateNode(Node& origin, float dx, float dy, float dz, bool debug,
                                Node& targetNode, bool& isInnerPoint)
{
	if(fabs(dx) < EQUALITY_TOLERANCE && fabs(dy) < EQUALITY_TOLERANCE && fabs(dz) < EQUALITY_TOLERANCE)
	{
		targetNode = origin;
		isInnerPoint = true;
		return true;
	}

    uint neighInd = findNeighbourPoint( origin, dx, dy, dz, debug,
                                    targetNode.coords, &isInnerPoint );

    if( neighInd == (uint) -1 )
        return false;

    Node tmpNode;
    uint secondNeighInd = findNeighbourPoint( origin, -dx, -dy, -dz, debug,
                                    tmpNode.coords, &isInnerPoint );

    //interpolator1->interpolate( targetNode, origin, getNodeByGlobalIndex( neighInd ) );
    //return true;
    
    if( secondNeighInd == (uint) -1 )
    {
        interpolator1->interpolate( targetNode, origin, getNodeByGlobalIndex(neighInd) );
    }
    else
    {
        uint leftInd, rightInd;
        if( dx + dy + dz > 0 )
        {
            leftInd = secondNeighInd;
            rightInd = neighInd;
        }
        else
        {
            rightInd = secondNeighInd;
            leftInd = neighInd;
        }
        interpolator2->interpolate( targetNode, getNodeByGlobalIndex(leftInd), origin, getNodeByGlobalIndex(rightInd) );
    }
    return true;
};

bool BasicCubicMesh::interpolateNode(Node& node)
{
    // Not implemented
    return false;
};

bool BasicCubicMesh::interpolateBorderNode_old(real x, real y, real z,
                                real dx, real dy, real dz, Node& node)
{
    //uint meshSizeX = 1 + (outline.maxX - outline.minX + meshH * 0.1) / meshH;
    float coords[3];
    float tx = coords[0] = x + dx;
    float ty = coords[1] = y + dy;
    float tz = coords[2] = z + dz;

    if( outline.isInAABB(tx, ty, tz) != outline.isInAABB(x, y, z) )
    {
        // FIXME_ASAP
        float minH = std::numeric_limits<float>::infinity();
        uint num = -1;
        for(uint i = 1; i < getNodesNumber(); i++) {
            Node& node = getNodeByLocalIndex(i);
			if(node.isBorder()) {
				float h = distance(coords, node.coords);
				if( h < minH ) {
					minH = h;
					num = i;
				}
			}
        }
        node = getNodeByLocalIndex(num);
        
        return true;
    }
	
    return false;
};

void BasicCubicMesh::findNearestsNodes(const vector3r& coords, uint N, vector< pair<uint,float> >& result)
{
	int n = 0;//floor( pow( (float)(N), 1.0 / 3.0 ) );

	uint i_min =	max( int( (coords[0] - outline.minX) / meshH ) - n, 0);
	uint i_max =	min( int( (coords[0] - outline.minX) / meshH ) + 1 + n, (int) numX);
	uint j_min =	max( int( (coords[1] - outline.minY) / meshH ) - n, 0);
	uint j_max =	min( int( (coords[1] - outline.minY) / meshH ) + 1 + n, (int) numY);
	uint k_min =	max( int( (coords[2] - outline.minZ) / meshH ) - n, 0);
	uint k_max =	min( int( (coords[2] - outline.minZ) / meshH ) + 1 + n, (int) numZ);

	uint num;
	for( uint k = k_min; k <= k_max; k++ )
		for( uint j = j_min; j <= j_max; j++ )
			for( uint i = i_min; i <= i_max; i++ )
	        {
				num = i * (numY + 1) * (numZ + 1) + j * (numZ + 1) + k + getNodeByLocalIndex(0).number;
				Node& node = getNodeByGlobalIndex(num);
				result.push_back( make_pair(node.number, (coords - node.coords).length()) );
	        }
}

bool BasicCubicMesh::interpolateBorderNode(real x, real y, real z,
        					real dx, real dy, real dz, Node& node)
{
	// One cube
	const int N = 8;
	vector3r coords = vector3r(x + dx, y + dy, z + dz);

	if( !outline.isInAABB(coords[0], coords[1], coords[2]) ) 
		return false;
	
	if( !outline.isInAABB(x, y, z) )
	{
		vector< pair<uint,float> > result;

		findNearestsNodes(coords, N, result);

		// Sorting nodes by distance
		sort(result.begin(), result.end(), sort_pred());

		for(uint i = 0; i < result.size(); i++) {
			Node& node1 = getNodeByGlobalIndex(result[i].first);
			if( node1.isBorder() )
			{
				node = node1;
				return true;
			}
		}
	}

    return false;
};

void BasicCubicMesh::setNumX(uint _numX)
{
	numX = _numX;
}

uint BasicCubicMesh::getNumX() const
{
	return numX;
}

void BasicCubicMesh::setNumY(uint _numY)
{
	numY = _numY;
}

uint BasicCubicMesh::getNumY() const
{
	return numY;
}

void BasicCubicMesh::setNumZ(uint _numZ)
{
	numZ = _numZ;
}

uint BasicCubicMesh::getNumZ() const
{
	return numZ;
}

float BasicCubicMesh::getH() const
{
	return meshH;
}

const SnapshotWriter& BasicCubicMesh::getSnaphotter() const
{
    return VTKCubicSnapshotWriter::getInstance();
}

const SnapshotWriter& BasicCubicMesh::getDumper() const
{
    return getSnaphotter();
}
