#include "libgcm/mesh/tetr/TetrMeshSecondOrder.hpp"

#include "libgcm/snapshot/VTK2SnapshotWriter.hpp"
#include "libgcm/snapshot/VTKSnapshotWriter.hpp"

#include "libgcm/node/Node.hpp"

using namespace gcm;
using std::unordered_map;
using std::make_pair;
using std::vector;

TetrMeshSecondOrder::TetrMeshSecondOrder()
{
    secondOrderNodesAreGenerated = false;
    numericalMethodOrder = 2;
    INIT_LOGGER("gcm.TetrMeshSecondOrder");
    interpolator = new TetrSecondOrderMinMaxInterpolator();
}

TetrMeshSecondOrder::~TetrMeshSecondOrder()
{
    delete interpolator;
}

void TetrMeshSecondOrder::createTetrs(uint number)
{
    LOG_DEBUG("Creating second order tetrs storage, size: " << (uint) (number * STORAGE_OVERCOMMIT_RATIO));
    tetrs2.resize(number * STORAGE_OVERCOMMIT_RATIO);
    tetrsStorageSize = number*STORAGE_OVERCOMMIT_RATIO;
}

void TetrMeshSecondOrder::createTriangles(uint number)
{
    LOG_DEBUG("Creating second order border storage, size: " << number);
    // TODO - make border working through addTriangle() / faceNumber++ / etc
    border2.resize(number);
    faceNumber = number;
    faceStorageSize = number;
}

TetrahedronFirstOrder& TetrMeshSecondOrder::getTetr(uint index)
{
    assert_ge(index, 0);
    unordered_map<uint, uint>::const_iterator itr;
    itr = tetrsMap.find(index);
    assert_true(itr != tetrsMap.end());
    return tetrs2[itr->second];
}

TetrahedronSecondOrder& TetrMeshSecondOrder::getTetr2(uint index)
{
    assert_ge(index, 0);
    unordered_map<uint, uint>::const_iterator itr;
    itr = tetrsMap.find(index);
    assert_true(itr != tetrsMap.end());
    return tetrs2[itr->second];
}

TetrahedronFirstOrder& TetrMeshSecondOrder::getTetrByLocalIndex(uint index)
{
    assert_ge(index, 0);
    return tetrs2[index];
}

TetrahedronSecondOrder& TetrMeshSecondOrder::getTetr2ByLocalIndex(uint index)
{
    assert_ge(index, 0);
    return tetrs2[index];
}

void TetrMeshSecondOrder::rebuildMaps()
{
    nodesMap.clear();
    for (uint i = 0; i < nodesNumber; i++)
        nodesMap[getNodeByLocalIndex(i).number] = i;
    tetrsMap.clear();
    for (uint i = 0; i < tetrsNumber; i++)
        tetrsMap[getTetr2ByLocalIndex(i).number] = i;
}

void TetrMeshSecondOrder::addTetr(TetrahedronFirstOrder& tetr)
{
    if (tetrsNumber == tetrsStorageSize)
        createTetrs(tetrsStorageSize * STORAGE_ONDEMAND_GROW_RATE);
    assert_lt(tetrsNumber, tetrsStorageSize);
    tetrs2[tetrsNumber].number = tetr.number;
    memcpy(tetrs2[tetrsNumber].vertices, tetr.vertices, 4 * sizeof (uint));
    tetrsMap[tetr.number] = tetrsNumber;
    tetrsNumber++;
}

void TetrMeshSecondOrder::addTetr2(TetrahedronSecondOrder& tetr)
{
    if (tetrsNumber == tetrsStorageSize)
        createTetrs(tetrsStorageSize * STORAGE_ONDEMAND_GROW_RATE);
    assert_lt(tetrsNumber, tetrsStorageSize);
    tetrs2[tetrsNumber] = tetr;
    tetrsMap[tetr.number] = tetrsNumber;
    tetrsNumber++;
}

TriangleFirstOrder& TetrMeshSecondOrder::getTriangle(uint index)
{
    assert_ge(index, 0);
    return border2[index];
}

TriangleSecondOrder& TetrMeshSecondOrder::getTriangle2(uint index)
{
    assert_ge(index, 0);
    return border2[index];
}

bool TetrMeshSecondOrder::belongsToTetr(uint nodeNum, uint tetrNum, uint faceNum)
{
    // Move second order nodes
    UintPair combinations[6];
    combinations[0] = make_pair(0, 1);
    combinations[1] = make_pair(0, 2);
    combinations[2] = make_pair(0, 3);
    combinations[3] = make_pair(1, 2);
    combinations[4] = make_pair(1, 3);
    combinations[5] = make_pair(2, 3);

    uint i1 = (0+faceNum) % 4;
    uint i2 = (1+faceNum) % 4;
    uint i3 = (2+faceNum) % 4;
    
    uint ai1, ai2, ai3;
    for(uint j = 0; j < 6; j++) {
        uint a = combinations[j].first;
        uint b = combinations[j].second;
        if( ( (a == i1) && (b == i2) ) || ( (a == i2) && (b == i1) ) )
            ai1 = j;
    }
    for(uint j = 0; j < 6; j++) {
        uint a = combinations[j].first;
        uint b = combinations[j].second;
        if( ( (a == i1) && (b == i3) ) || ( (a == i3) && (b == i1) ) )
            ai2 = j;
    }
    for(uint j = 0; j < 6; j++) {
        uint a = combinations[j].first;
        uint b = combinations[j].second;
        if( ( (a == i2) && (b == i3) ) || ( (a == i3) && (b == i2) ) )
            ai3 = j;
    }
    
    TetrahedronSecondOrder& tetr = getTetr2(tetrNum);
    if( (nodeNum == tetr.vertices[i1]) || (nodeNum == tetr.vertices[i2]) || (nodeNum == tetr.vertices[i3])
                || (nodeNum == tetr.addVerts[ai1]) || (nodeNum == tetr.addVerts[ai2]) || (nodeNum == tetr.addVerts[ai3]))
        return true;
    else
        return false;
}

void TetrMeshSecondOrder::copyMesh(TetrMeshFirstOrder* src)
{
    LOG_DEBUG("Creating mesh using copy");
    firstOrderNodesNumber = src->getNodesNumber();
    secondOrderNodesNumber = countSecondOrderNodes(src);

    LOG_DEBUG("Copying first order nodes");

    createNodes(firstOrderNodesNumber + secondOrderNodesNumber);
    for (uint i = 0; i < firstOrderNodesNumber; i++)
        addNode(src->getNodeByLocalIndex(i));

    LOG_DEBUG("Copying first order tetrs");

    createTetrs(src->getTetrsNumber());
    //for( uint i = 0; i < tetrsNumber; i++ )
    //for( MapIter itr = tetrsMap.begin(); itr != tetrsMap.end(); ++itr ) {
    //    uint i = itr->first;
    //    addTetr( src->getTetr(i) );
    //}
    for (uint i = 0; i < src->getTetrsNumber(); i++) {
        addTetr(src->getTetrByLocalIndex(i));
    }

    LOG_DEBUG("Generating second order nodes");
    generateSecondOrderNodes();
}

void TetrMeshSecondOrder::copyMesh2(TetrMeshSecondOrder* src)
{
    LOG_DEBUG("Creating second order mesh using copy");

    LOG_DEBUG("Nodes: " << src->getNodesNumber());
    createNodes(src->getNodesNumber());
    for (uint i = 0; i < src->getNodesNumber(); i++)
        addNode(src->getNodeByLocalIndex(i));

    LOG_DEBUG("Tetrs: " << src->getTetrsNumber());
    createTetrs(src->getTetrsNumber());
    //for( uint i = 0; i < tetrsNumber; i++ )
    for (MapIter itr = tetrsMap.begin(); itr != tetrsMap.end(); ++itr) {
        uint i = itr->first;
        addTetr2(src->getTetr2(i));
    }
    for (uint i = 0; i < src->getTetrsNumber(); i++) {
        addTetr2(src->getTetr2ByLocalIndex(i));
    }
}

void TetrMeshSecondOrder::preProcessGeometry()
{
    LOG_DEBUG("Preprocessing second order mesh started");

    calcMaxH();
    calcAvgH();
    calcMaxEdge();
    mesh_min_h *= 0.5;
    mesh_max_h *= 0.5;
    mesh_avg_h *= 0.5;

    verifyTetrahedraVertices();
    build_volume_reverse_lookups();

    check_unused_nodes();
    build_first_order_border();
    generateSecondOrderBorder();
    build_surface_reverse_lookups();

    check_numbering();
    check_outer_normals();
    LOG_DEBUG("Preprocessing mesh done.");
}

void TetrMeshSecondOrder::verifyTetrahedraVertices()
{
    LOG_DEBUG("Verifying second order tetrahedra vertices");
    for (uint tetrInd = 0; tetrInd < tetrsNumber; tetrInd++) {
        TetrahedronSecondOrder& tetr = getTetr2ByLocalIndex(tetrInd);
        for (uint vertInd = 0; vertInd < 4; vertInd++) {
            uint nodeNum = tetr.vertices[vertInd];
            assert_ge(nodeNum, 0);
            assert_true(getNode(nodeNum).isFirstOrder());
        }

        for (uint addVertInd = 0; addVertInd < 6; addVertInd++) {
            uint addNodeNum = tetr.addVerts[addVertInd];
            assert_ge(addNodeNum, 0);
            assert_true(getNode(addNodeNum).isSecondOrder());
        }
    }
}

void TetrMeshSecondOrder::build_volume_reverse_lookups()
{
    LOG_DEBUG("Building volume reverse lookups for second order mesh");

    // Init vectors for "reverse lookups" of tetrahedrons current node is a member of.
    //for(uint i = 0; i < nodesNumber; i++) {
    for (MapIter itr = nodesMap.begin(); itr != nodesMap.end(); ++itr) {
        uint i = itr->first;
        getVolumeElementsForNode(i).clear();
    }

    // Go through all the tetrahedrons
    //for(uint i = 0; i < tetrsNumber; i++) {
    for (MapIter itr = tetrsMap.begin(); itr != tetrsMap.end(); ++itr) {
        uint i = itr->first;
        // For all verticles
        for (uint j = 0; j < 4; j++) {
            TetrahedronFirstOrder& tetr = getTetr(i);
            uint nodeInd = tetr.vertices[j];
            assert_true(getNode(nodeInd).isFirstOrder());
            // Push to data of nodes the number of this tetrahedron
            getVolumeElementsForNode(nodeInd).push_back(tetr.number);
        }
        for (uint j = 0; j < 6; j++) {
            TetrahedronSecondOrder& tetr2 = getTetr2(i);
            uint nodeInd = tetr2.addVerts[j];
            assert_true(getNode(nodeInd).isSecondOrder());
            // Push to data of nodes the number of this tetrahedron
            getVolumeElementsForNode(nodeInd).push_back(tetr2.number);
        }
    }

    for (MapIter itr = nodesMap.begin(); itr != nodesMap.end(); ++itr) {
        Node& node = getNode(itr->first);
        uint num = getVolumeElementsForNode(itr->first).size();
        if (num <= 0)
            LOG_DEBUG("Node is not a part of volumes. Node: " << node);
    }
}

void TetrMeshSecondOrder::build_first_order_border()
{
    // Prepare border data

    float solid_angle;
    float solid_angle_part;

    LOG_DEBUG("Looking for border nodes using angles");
    uint nodeCount = 0;

    // Check border using solid angle comparation with 4*PI

    //for( uint i = 0; i < nodesNumber; i++ ) {
    for (MapIter itr = nodesMap.begin(); itr != nodesMap.end(); ++itr) {
        uint i = itr->first;
        Node& node = getNode(i);
        node.setBorder(false);
        if (/*node.isLocal() &&*/ node.isFirstOrder()) {
            solid_angle = 0;
            vector<uint>& elements = getVolumeElementsForNode(i);
            for (unsigned j = 0; j < elements.size(); j++) {
                solid_angle_part = get_solid_angle(i, elements[j]);
                if (solid_angle_part < 0)
                    for (unsigned z = 0; z < elements.size(); z++)
                        LOG_DEBUG("Element: " << elements[z]);
                assert_ge(solid_angle_part, 0);
                solid_angle += solid_angle_part;
            }
            if (fabs(4 * M_PI - solid_angle) > M_PI * EQUALITY_TOLERANCE) {
                node.setBorder(true);
                nodeCount++;
            }
        }
    }
    LOG_DEBUG("Found " << nodeCount << " border nodes");

    LOG_DEBUG("Constructing border triangles");

    uint faceCount = 0;
    uint number = 0;

    // FIXME TODO - make faces uniq!

    // Check all tetrs and construct border triangles
    //for(uint i = 0; i < tetrsNumber; i++) {
    for (MapIter itr = tetrsMap.begin(); itr != tetrsMap.end(); ++itr) {
        uint i = itr->first;
        for (uint j = 0; j < 4; j++) {
            if (isTriangleBorder(getTetr(i).vertices))
                faceCount++;
            shiftArrayLeft(getTetr(i).vertices, 4);
        }
    }

    LOG_DEBUG("Found " << faceCount << " border triangles");

    createTriangles(faceCount);

    LOG_DEBUG("Triangles storage allocated");
    
    // WA for bruteforce collision detector
    createOutline();

    //for(uint i = 0; i < tetrsNumber; i++) {
    for (MapIter itr = tetrsMap.begin(); itr != tetrsMap.end(); ++itr) {
        uint i = itr->first;
        for (uint j = 0; j < 4; j++) {
            if (isTriangleBorder(getTetr(i).vertices)) {
                getTriangle(number) = createBorderTriangle(getTetr(i).vertices, number);
                number++;
            }
            shiftArrayLeft(getTetr(i).vertices, 4);
        }
    }
    
    //if( number != faceCount )
    //    LOG_WARN("Number: " << number << " FaceCount: " << faceCount);
    assert_eq(number, faceCount);
    LOG_DEBUG("Created " << faceNumber << " triangles");
}

void TetrMeshSecondOrder::generateSecondOrderBorder()
{
    LOG_DEBUG("Faces: " << faceNumber);
    bool debug = false;

    LOG_DEBUG("Generating second order border");
    UintPair combinations[3];
    combinations[0] = make_pair(0, 1);
    combinations[1] = make_pair(0, 2);
    combinations[2] = make_pair(1, 2);

    uint v1, v2, ind;
    uint v1pos, v2pos, l;
    uint curTInd;
    uint minI, maxI;
    for (uint i = 0; i < faceNumber; i++) {
        for (uint j = 0; j < 3; j++) {
            v1 = getTriangle(i).vertices[ combinations[j].first ];
            v2 = getTriangle(i).vertices[ combinations[j].second ];

            //if( ( v1 == 0 && v2 == 420 ) || ( v1 == 420 && v2 == 0 ) )
            //    debug = true;

            ind = -1;
            vector<uint>& elements = getVolumeElementsForNode(v1);
            l = elements.size();
            for (uint k = 0; k < l; k++) {
                v1pos = -1;
                v2pos = -1;
                curTInd = elements[k];
                for (uint z = 0; z < 4; z++) {
                    if (getTetr(curTInd).vertices[z] == v1)
                        v1pos = z;
                    if (getTetr(curTInd).vertices[z] == v2)
                        v2pos = z;
                }
                if (v1pos >= 0 && v2pos >= 0) {
                    minI = (v1pos > v2pos ? v2pos : v1pos);
                    maxI = (v1pos > v2pos ? v1pos : v2pos);
                    if (minI == 0 && maxI == 1)
                        ind = getTetr2(curTInd).addVerts[0];
                    else if (minI == 0 && maxI == 2)
                        ind = getTetr2(curTInd).addVerts[1];
                    else if (minI == 0 && maxI == 3)
                        ind = getTetr2(curTInd).addVerts[2];
                    else if (minI == 1 && maxI == 2)
                        ind = getTetr2(curTInd).addVerts[3];
                    else if (minI == 1 && maxI == 3)
                        ind = getTetr2(curTInd).addVerts[4];
                    else if (minI == 2 && maxI == 3)
                        ind = getTetr2(curTInd).addVerts[5];
                    if (debug) {
                        LOG_DEBUG("Tetr: " << curTInd << " Verts: " << v1 << " " << v2);
                        LOG_DEBUG("Found origin for test node");
                        LOG_DEBUG("Positions: " << v1pos << " " << v2pos);
                        LOG_DEBUG("Index: " << ind);
                        LOG_DEBUG("Node: " << getNode(ind));
                    }
                    break;
                }
            }

            assert_ne(ind, (uint) -1);
            assert_true(getNode(ind).isSecondOrder());
            getTriangle2(i).addVerts[j] = ind;
            getNode(ind).setBorder(true);
        }
    }

    LOG_DEBUG("Second order border generated");
}

void TetrMeshSecondOrder::build_surface_reverse_lookups()
{
    LOG_DEBUG("Building surface reverse lookups for second order mesh");

    // Init vectors for "reverse lookups" of border triangles current node is a member of.
    //for(uint i = 0; i < nodesNumber; i++) {
    for (MapIter itr = nodesMap.begin(); itr != nodesMap.end(); ++itr) {
        uint i = itr->first;
        getBorderElementsForNode(i).clear();
    }

    // Go through all the triangles and push to data of nodes the number of this triangle
    for (uint i = 0; i < faceNumber; i++) {
        for (uint j = 0; j < 3; j++) {
            uint nodeInd = getTriangle(i).vertices[j];
            assert_true(getNode(nodeInd).isFirstOrder());
            getBorderElementsForNode(nodeInd).push_back(i);
        }
        for (uint j = 0; j < 3; j++) {
            uint nodeInd = getTriangle2(i).addVerts[j];
            assert_true(getNode(nodeInd).isSecondOrder());
            getBorderElementsForNode(nodeInd).push_back(i);
        }
    }

    for (MapIter itr = nodesMap.begin(); itr != nodesMap.end(); ++itr) {
        Node& node = getNode(itr->first);
        uint num = getBorderElementsForNode(itr->first).size();
        if (node.isBorder() && num <= 0)
            LOG_DEBUG("Border node is not a part of faces. Node: " << node);
    }
}

void TetrMeshSecondOrder::moveCoords(float tau)
{
    // Move first order nodes
    TetrMeshFirstOrder::moveCoords(tau);
    mesh_min_h *= 0.5;

    // Move second order nodes
    UintPair combinations[6];
    combinations[0] = make_pair(0, 1);
    combinations[1] = make_pair(0, 2);
    combinations[2] = make_pair(0, 3);
    combinations[3] = make_pair(1, 2);
    combinations[4] = make_pair(1, 3);
    combinations[5] = make_pair(2, 3);

    uint v1, v2, ind;
    for (MapIter itr = tetrsMap.begin(); itr != tetrsMap.end(); ++itr) {
        uint i = itr->first;
        for (uint j = 0; j < 6; j++) {
            ind = getTetr2(i).addVerts[j];
            v1 = getTetr(i).vertices[ combinations[j].first ];
            v2 = getTetr(i).vertices[ combinations[j].second ];
            moveSecondOrderNode(ind, v1, v2);
        }
    }
}

void TetrMeshSecondOrder::moveSecondOrderNode(uint nodeIdx, uint nodeIdx1, uint nodeIdx2)
{
    Node& node = getNode(nodeIdx);
    Node& newNode = getNewNode(nodeIdx);
    Node& node1 = getNode(nodeIdx1);
    Node& node2 = getNode(nodeIdx2);

    for (uint i = 0; i < 3; i++) {
        node.coords[i] = (node1.coords[i] + node2.coords[i]) * 0.5;
        newNode.coords[i] = node.coords[i];
    }
}

void TetrMeshSecondOrder::fillSecondOrderNode(Node& newNode, uint nodeIdx1, uint nodeIdx2)
{
    Node& node1 = getNode(nodeIdx1);
    Node& node2 = getNode(nodeIdx2);

    for (uint i = 0; i < 3; i++)
        newNode.coords[i] = (node1.coords[i] + node2.coords[i]) * 0.5;

    for (uint i = 0; i < 9; i++)
        newNode.PDE[i] = (node1.PDE[i] + node2.PDE[i]) * 0.5;

//    newNode.setRho((node1.getRho() + node2.getRho()) * 0.5);
    newNode.setMaterialId(node1.getMaterialId());

    newNode.setPlacement(true);
    newNode.setOrder(2);
}

uint TetrMeshSecondOrder::countSecondOrderNodes(TetrMeshFirstOrder* src)
{
    assert_true(src);
    LOG_DEBUG("Counting additional nodes");

    uint firstOrderNodesCount = src->getNodesNumber();

    UintPair combinations[6];
    combinations[0] = make_pair(0, 1);
    combinations[1] = make_pair(0, 2);
    combinations[2] = make_pair(0, 3);
    combinations[3] = make_pair(1, 2);
    combinations[4] = make_pair(1, 3);
    combinations[5] = make_pair(2, 3);

    uint minFirstOrderNum = src->getNodeByLocalIndex(0).number;
    uint maxFirstOrderNum = src->getNodeByLocalIndex(0).number;
    for (uint i = 0; i < firstOrderNodesCount; i++) {
        uint num = src->getNodeByLocalIndex(i).number;
        if (num > maxFirstOrderNum)
            maxFirstOrderNum = num;
        if (num < minFirstOrderNum)
            minFirstOrderNum = num;
    }
    uint firstOrderLength = maxFirstOrderNum - minFirstOrderNum + 1;
    //LOG_DEBUG("Min: " << minFirstOrderNum << " Max: " << maxFirstOrderNum << " Len: " << firstOrderLength);

    vector<UintPair>** processed = new vector<UintPair>*[firstOrderLength];
    for (uint i = 0; i < firstOrderLength; i++)
        processed[i] = new vector<UintPair>;

    uint secondOrderNodesCount = 0;
    uint v1, v2;
    uint ind;
    for (uint i = 0; i < src->getTetrsNumber(); i++) {
        TetrahedronFirstOrder& tetr = src->getTetrByLocalIndex(i);
        //if( body->getEngine()->getRank() == 1 )
        //    LOG_DEBUG("Tetr " << i << " Num: " << tetr.number);
        for (uint j = 0; j < 6; j++) {
            v1 = tetr.vertices[ combinations[j].first ];
            v2 = tetr.vertices[ combinations[j].second ];
            //if( body->getEngine()->getRank() == 1 )
            //    LOG_DEBUG("V1 = " << v1 << " V2 = " << v2);
            ind = -1;

            for (uint z = 0; z < processed[v1 - minFirstOrderNum]->size(); z++)
                if ((processed[v1 - minFirstOrderNum]->at(z)).second == v2)
                    ind = (processed[v1 - minFirstOrderNum]->at(z)).first;

            if (ind == (uint) -1) {
                UintPair in;
                in.first = firstOrderNodesCount + secondOrderNodesCount;
                in.second = v2;
                processed[v1 - minFirstOrderNum]->push_back(in);
                in.second = v1;
                processed[v2 - minFirstOrderNum]->push_back(in);
                secondOrderNodesCount++;
            }
        }
    }

    for (uint i = 0; i < firstOrderLength; i++)
        delete processed[i];
    delete[] processed;

    LOG_DEBUG("We are going to create " << secondOrderNodesCount << " second order nodes");

    return secondOrderNodesCount;
}

void TetrMeshSecondOrder::generateSecondOrderNodes()
{
    Node node;

    if (secondOrderNodesAreGenerated)
        return;

    uint minNodeNum = getNodeByLocalIndex(0).number;

    for (uint i = 0; i < firstOrderNodesNumber; i++) {
        getNodeByLocalIndex(i).setOrder(1);
        if (getNodeByLocalIndex(i).number < minNodeNum)
            minNodeNum = getNodeByLocalIndex(i).number;
    }

    LOG_DEBUG("Creating second order nodes");
    LOG_DEBUG("Number of first order nodes: " << nodesNumber);
    assert_eq(firstOrderNodesNumber, nodesNumber);

    UintPair combinations[6];
    combinations[0] = make_pair(0, 1);
    combinations[1] = make_pair(0, 2);
    combinations[2] = make_pair(0, 3);
    combinations[3] = make_pair(1, 2);
    combinations[4] = make_pair(1, 3);
    combinations[5] = make_pair(2, 3);

    vector<UintPair>** processed = new vector<UintPair>*[minNodeNum + firstOrderNodesNumber];
    for (uint i = 0; i < minNodeNum + firstOrderNodesNumber; i++)
        processed[i] = new vector<UintPair>;

    uint secondOrderNodesCount = 0;
    uint v1, v2;
    uint ind;
    //for( uint i = 0; i < tetrsNumber; i++) {
    for (MapIter itr = tetrsMap.begin(); itr != tetrsMap.end(); ++itr) {
        uint i = itr->first;
        for (uint j = 0; j < 6; j++) {
            v1 = getTetr(i).vertices[ combinations[j].first ];
            v2 = getTetr(i).vertices[ combinations[j].second ];
            ind = -1;

            for (uint z = 0; z < processed[v1]->size(); z++)
                if ((processed[v1]->at(z)).second == v2)
                    ind = (processed[v1]->at(z)).first;

            if (ind == (uint) -1) {
                ind = minNodeNum + firstOrderNodesNumber + secondOrderNodesCount;
                fillSecondOrderNode(node, v1, v2);
                node.number = ind;
                addNode(node);
                UintPair in;
                in.first = ind;
                in.second = v2;
                processed[v1]->push_back(in);
                in.second = v1;
                processed[v2]->push_back(in);
                secondOrderNodesCount++;
            }
            getTetr2(i).addVerts[j] = ind;
        }
    }

    for (uint i = 0; i < minNodeNum + firstOrderNodesNumber; i++)
        delete processed[i];
    delete[] processed;

    LOG_DEBUG("Second order nodes created");

    LOG_DEBUG("Total number of nodes: " << nodesNumber);

    secondOrderNodesAreGenerated = true;
}

bool TetrMeshSecondOrder::interpolateNode(Node& origin, float dx, float dy, float dz, bool debug,
                                               Node& targetNode, bool& isInnerPoint)
{
    uint tetrInd = findOwnerTetr(origin, dx, dy, dz, debug,
                                targetNode.coords, &isInnerPoint);

    if (tetrInd == (uint) -1)
        return false;

    TetrahedronSecondOrder& tmp_tetr = getTetr2(tetrInd);
    interpolator->interpolate(targetNode,
                              getNode(tmp_tetr.vertices[0]), getNode(tmp_tetr.vertices[1]),
                              getNode(tmp_tetr.vertices[2]), getNode(tmp_tetr.vertices[3]),
                              getNode(tmp_tetr.addVerts[0]), getNode(tmp_tetr.addVerts[1]),
                              getNode(tmp_tetr.addVerts[2]), getNode(tmp_tetr.addVerts[3]),
                              getNode(tmp_tetr.addVerts[4]), getNode(tmp_tetr.addVerts[5]));
    return true;
}

// FIXME_ASAP: rewrite it
bool TetrMeshSecondOrder::interpolateNode(Node& node)
{
    for (uint i = 0; i < getTetrsNumber(); i++)
    {
        TetrahedronSecondOrder& t = getTetr2ByLocalIndex(i);
        if ( pointInTetr(node.coords.x, node.coords.y, node.coords.z,
                getNode(t.vertices[0]).coords, getNode(t.vertices[1]).coords,
                getNode(t.vertices[2]).coords, getNode(t.vertices[3]).coords, false) )
        {
            interpolator->interpolate( node,
                    getNode(t.vertices[0]), getNode(t.vertices[1]),
                    getNode(t.vertices[2]), getNode(t.vertices[3]),
                    getNode(t.addVerts[0]), getNode(t.addVerts[1]),
                    getNode(t.addVerts[2]), getNode(t.addVerts[3]),
                    getNode(t.addVerts[4]), getNode(t.addVerts[5]));
            return true;
        }
    }

    return false;
}


const SnapshotWriter& TetrMeshSecondOrder::getSnaphotter() const
{
    return VTKSnapshotWriter::getInstance();
}

const SnapshotWriter& TetrMeshSecondOrder::getDumper() const {
    return VTK2SnapshotWriter::getInstance();
}
