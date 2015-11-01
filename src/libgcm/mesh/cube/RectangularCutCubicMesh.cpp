#include "libgcm/mesh/cube/RectangularCutCubicMesh.hpp"

#include "libgcm/Engine.hpp"
#include "libgcm/node/Node.hpp"
#include "libgcm/snapshot/VTKCubicSnapshotWriter.hpp"
#include "launcher/loaders/mesh/RectangularCutCubicMeshLoader.hpp"

using namespace gcm;
using std::numeric_limits;
using std::pair;
using std::make_pair;
using std::vector;
using std::sort;
using std::max;
using std::min;


RectangularCutCubicMesh::RectangularCutCubicMesh() :
	BasicCubicMesh(launcher::RectangularCutCubicMeshLoader::MESH_TYPE)
{
	INIT_LOGGER("gcm.RectangularCutCubicMesh");
};

void RectangularCutCubicMesh::preProcessGeometry()
{
    LOG_DEBUG("Preprocessing mesh geometry started.");
	
	for(uint i = 0; i < getNodesNumber(); i++) {
		Node& node = getNodeByLocalIndex(i);
		node.setBorder(false);
		node.setCustomFlag(1, false);
		if( !cutArea.isOutAABB(node) ) {
			node.setCustomFlag(1, true);
			node.setUsed(false);
		}
		Node& newnode = getNewNodeByLocalIndex(i);
		newnode.setBorder(false);
		newnode.setCustomFlag(1, false);
		if( !cutArea.isOutAABB(newnode) ) {
			newnode.setCustomFlag(1, true);
			newnode.setUsed(false);
		}
	}
	
    // for usual AABB outline
	for(uint i = 0; i < getNodesNumber(); i++)
    {
        Node& node = getNodeByLocalIndex(i);
        for( uint i = 0; i < 3; i ++)
        {
            if( ( fabs(node.coords[i] - outline.min_coords[i]) < EQUALITY_TOLERANCE )
                || ( fabs(node.coords[i] - outline.max_coords[i]) < EQUALITY_TOLERANCE ) )
            {
                node.setBorder(true); getNewNodeByLocalIndex(i).setBorder(true); break;
            }
        }
    }
	// for cutArea
	for(uint i = 0; i < getNodesNumber(); i++)
    {
        Node& node = getNodeByLocalIndex(i);
		if(cutArea.isInAABB(node)) {
			for( uint i = 0; i < 3; i ++)
				if( ( fabs(node.coords[i] - cutArea.min_coords[i]) < EQUALITY_TOLERANCE )
					|| ( fabs(node.coords[i] - cutArea.max_coords[i]) < EQUALITY_TOLERANCE ) )
				{
					node.setBorder(true); getNewNodeByLocalIndex(i).setBorder(true); break;
				}
		}
	}
	
    LOG_DEBUG("Preprocessing mesh geometry done.");
};

void RectangularCutCubicMesh::findBorderNodeNormal(const Node& node, 
	float* x, float* y, float* z, bool debug)
{
    assert_true(node.isBorder() );
    float normal[3];
    normal[0] = normal[1] = normal[2] = 0.0;
	uint i = node.contactDirection;
    for( uint cntr = 0; cntr < 3; cntr++) {
        if( fabs(node.coords[i] - outline.min_coords[i]) < EQUALITY_TOLERANCE ) {
            normal[i] = -1;
            break;
        }
        if( fabs(node.coords[i] - outline.max_coords[i]) < EQUALITY_TOLERANCE ) {
            normal[i] = 1;
            break;
        }
		if(cutArea.isInAABB(node)) {
			if( fabs(node.coords[i] - cutArea.min_coords[i]) < EQUALITY_TOLERANCE ) {
				normal[i] = 1;
				break;
			}
			if( fabs(node.coords[i] - cutArea.max_coords[i]) < EQUALITY_TOLERANCE ) {
				normal[i] = -1;
				break;
			}
		}
		i = (i + 1) % 3;
    }
    *x = normal[0];
    *y = normal[1];
    *z = normal[2];
};

uint RectangularCutCubicMesh::findNeighbourPoint(Node& node, float dx, float dy, float dz,
	bool debug, float* coords, bool* innerPoint)
{
	uint meshSizeY = 1 + (outline.maxY - outline.minY + meshH * 0.1) / meshH;
	uint meshSizeZ = 1 + (outline.maxZ - outline.minZ + meshH * 0.1) / meshH;

    assert_le(vectorSquareNorm(dx, dy, dz), getMinH() * getMinH() * (1 + EQUALITY_TOLERANCE) );

    coords[0] = node.coords[0] + dx;
    coords[1] = node.coords[1] + dy;
    coords[2] = node.coords[2] + dz;

    if( !outline.isInAABB(coords[0], coords[1], coords[2]) || 
		!cutArea.isOutAABB(coords[0], coords[1], coords[2]) )
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

void RectangularCutCubicMesh::findNearestsNodes(const vector3r& coords, uint N, vector< pair<uint,float> >& result)
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
			for( uint i = i_min; i <= i_max; i++ ) {
				num = i * (numY + 1) * (numZ + 1) + j * (numZ + 1) + k + getNodeByLocalIndex(0).number;
				Node& node = getNodeByGlobalIndex(num);
				result.push_back( make_pair(node.number, (coords - node.coords).length()) );
	        }
}

bool RectangularCutCubicMesh::interpolateBorderNode(real x, real y, real z,
        					real dx, real dy, real dz, Node& node)
{
	// One cube
	const int N = 8;
	vector3r coords = vector3r(x + dx, y + dy, z + dz);

	if( !outline.isInAABB(coords[0], coords[1], coords[2]) || 
	    !cutArea.isOutAABB(coords[0], coords[1], coords[2]) )
		return false;
	
	if( ( !outline.isInAABB(x, y, z) ) || ( !cutArea.isOutAABB(x, y, z) ) )
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


void RectangularCutCubicMesh::transfer(float x, float y, float z) {
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
	if( !isinf(cutArea.minX) )
    {
        cutArea.transfer(x, y, z);
    }
	
	
    // TODO@avasyukov - think about additional checks
    Engine::getInstance().transferScene(x, y, z);
};
