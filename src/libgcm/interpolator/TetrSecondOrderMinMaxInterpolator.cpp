/*
 * File:   TetrSecondOrderMinMaxInterpolator.cpp
 * Author: anganar
 *
 * Created on May 3, 2013, 12:13 AM
 */
#include "libgcm/interpolator/TetrSecondOrderMinMaxInterpolator.hpp"

#include "libgcm/node/Node.hpp"

using namespace gcm;

TetrSecondOrderMinMaxInterpolator::TetrSecondOrderMinMaxInterpolator()
{
    type = "TetrSecondOrderMinMaxInterpolator";
    INIT_LOGGER("gcm.TetrSecondOrderMinMaxInterpolator");
}

TetrSecondOrderMinMaxInterpolator::~TetrSecondOrderMinMaxInterpolator()
{
}

void TetrSecondOrderMinMaxInterpolator::interpolate(Node& node,
		Node& node0, Node& node1, Node& node2, Node& node3,
		Node& addNode0, Node& addNode1, Node& addNode2,
		Node& addNode3, Node& addNode4, Node& addNode5)
{
    LOG_TRACE("Start interpolation");

    float factor[4];

    float Vol = tetrVolume(
                           (node1.coords[0])-(node0.coords[0]),
                           (node1.coords[1])-(node0.coords[1]),
                           (node1.coords[2])-(node0.coords[2]),
                           (node2.coords[0])-(node0.coords[0]),
                           (node2.coords[1])-(node0.coords[1]),
                           (node2.coords[2])-(node0.coords[2]),
                           (node3.coords[0])-(node0.coords[0]),
                           (node3.coords[1])-(node0.coords[1]),
                           (node3.coords[2])-(node0.coords[2])
                           );

    factor[0] = fabs(tetrVolume(
                                (node1.coords[0])-(node.coords[0]),
                                (node1.coords[1])-(node.coords[1]),
                                (node1.coords[2])-(node.coords[2]),
                                (node2.coords[0])-(node.coords[0]),
                                (node2.coords[1])-(node.coords[1]),
                                (node2.coords[2])-(node.coords[2]),
                                (node3.coords[0])-(node.coords[0]),
                                (node3.coords[1])-(node.coords[1]),
                                (node3.coords[2])-(node.coords[2])
                                ) / Vol);

    factor[1] = fabs(tetrVolume(
                                (node0.coords[0])-(node.coords[0]),
                                (node0.coords[1])-(node.coords[1]),
                                (node0.coords[2])-(node.coords[2]),
                                (node2.coords[0])-(node.coords[0]),
                                (node2.coords[1])-(node.coords[1]),
                                (node2.coords[2])-(node.coords[2]),
                                (node3.coords[0])-(node.coords[0]),
                                (node3.coords[1])-(node.coords[1]),
                                (node3.coords[2])-(node.coords[2])
                                ) / Vol);

    factor[2] = fabs(tetrVolume(
                                (node1.coords[0])-(node.coords[0]),
                                (node1.coords[1])-(node.coords[1]),
                                (node1.coords[2])-(node.coords[2]),
                                (node0.coords[0])-(node.coords[0]),
                                (node0.coords[1])-(node.coords[1]),
                                (node0.coords[2])-(node.coords[2]),
                                (node3.coords[0])-(node.coords[0]),
                                (node3.coords[1])-(node.coords[1]),
                                (node3.coords[2])-(node.coords[2])
                                ) / Vol);

    factor[3] = fabs(tetrVolume(
                                (node1.coords[0])-(node.coords[0]),
                                (node1.coords[1])-(node.coords[1]),
                                (node1.coords[2])-(node.coords[2]),
                                (node2.coords[0])-(node.coords[0]),
                                (node2.coords[1])-(node.coords[1]),
                                (node2.coords[2])-(node.coords[2]),
                                (node0.coords[0])-(node.coords[0]),
                                (node0.coords[1])-(node.coords[1]),
                                (node0.coords[2])-(node.coords[2])
                                ) / Vol);

    // If we see potential instability
    if (factor[0] + factor[1] + factor[2] + factor[3] > 1.0) {
        // If it is small - treat instability as minor and just 'smooth' it
        // TODO - think about it more carefully
        //if( point_in_tetr(node.local_num, node.coords[0], node.coords[1], node.coords[2], tetr) )
        if (factor[0] + factor[1] + factor[2] + factor[3] < 1.1) {
            float sum = factor[0] + factor[1] + factor[2] + factor[3];
            for (int i = 0; i < 4; i++)
                factor[i] = factor[i] / sum;
        }
            // If point is not in tetr - throw exception
        else {
            /*            *logger << "\tTetrVol = " < Vol;
             *logger << "\tfactor[0]=" << factor[0] << " factor[1]=" << factor[1] << " factor[2]=" << factor[2]     << " factor[3]=" << factor[3] << " Sum: " < factor[0] + factor[1] + factor[2] + factor[3];

             *logger << "\tnode.coords.x[0]=" << node.coords[0] << " node.coords.x[1]=" << node.coords[1]
                                            << " node.coords.x[2]=" < node.coords[2];
                                    if( node.isFirstOrder() )
             *logger < "First order node";
                                    else if( node.isSecondOrder() )
             *logger < "Second order node";

             *logger << "\tv0.x[0]=" << nodes[tetr.vert[0]].coords[0] << " v0.x[1]=" << nodes[tetr.vert[0]].coords[1] << " v0.x[2]=" < nodes[tetr.vert[0]].coords[2];

             *logger << "\tv1.x[0]=" << nodes[tetr.vert[1]].coords[0] << " v1.x[1]=" << nodes[tetr.vert[1]].coords[1] << " v1.x[2]=" < nodes[tetr.vert[1]].coords[2];

             *logger << "\tv2.x[0]=" << nodes[tetr.vert[2]].coords[0] << " v2.x[1]=" << nodes[tetr.vert[2]].coords[1] << " v2.x[2]=" < nodes[tetr.vert[2]].coords[2];

             *logger << "\tv3.x[0]=" << nodes[tetr.vert[3]].coords[0] << " v3.x[1]=" << nodes[tetr.vert[3]].coords[1] << " v3.x[2]=" < nodes[tetr.vert[3]].coords[2];*/
            THROW_BAD_MESH("Sum of factors is greater than 1.0");
        }
    }

    baseNodes[0] = &node0;
    baseNodes[1] = &node1;
    baseNodes[2] = &node2;
    baseNodes[3] = &node3;

    addNodes[0] = &addNode0;
    addNodes[1] = &addNode1;
    addNodes[2] = &addNode2;
    addNodes[3] = &addNode3;
    addNodes[4] = &addNode4;
    addNodes[5] = &addNode5;

    for (int i = 0; i < baseNodes[0]->getSizeOfPDE(); i++) {
        float min = baseNodes[0]->PDE[i];
        float max = baseNodes[0]->PDE[i];
        for (int z = 1; z < 4; z++) {
            if (baseNodes[z]->PDE[i] < min)
                min = baseNodes[z]->PDE[i];
            if (baseNodes[z]->PDE[i] > max)
                max = baseNodes[z]->PDE[i];
        }
        for (int z = 0; z < 6; z++) {
            if (addNodes[z]->PDE[i] < min)
                min = addNodes[z]->PDE[i];
            if (addNodes[z]->PDE[i] > max)
                max = addNodes[z]->PDE[i];
        }

        node.PDE[i] = (baseNodes[0]->PDE[i] * factor[0] * (2 * factor[0] - 1)
                + baseNodes[1]->PDE[i] * factor[1] * (2 * factor[1] - 1)
                + baseNodes[2]->PDE[i] * factor[2] * (2 * factor[2] - 1)
                + baseNodes[3]->PDE[i] * factor[3] * (2 * factor[3] - 1)
                + addNodes[0]->PDE[i] * 4 * factor[0] * factor[1]
                + addNodes[1]->PDE[i] * 4 * factor[0] * factor[2]
                + addNodes[2]->PDE[i] * 4 * factor[0] * factor[3]
                + addNodes[3]->PDE[i] * 4 * factor[1] * factor[2]
                + addNodes[4]->PDE[i] * 4 * factor[1] * factor[3]
                + addNodes[5]->PDE[i] * 4 * factor[2] * factor[3]
                );

        if (node.PDE[i] < min)
            node.PDE[i] = min;
        if (node.PDE[i] > max)
            node.PDE[i] = max;
    }

//	TODO@next - do smth with rho in interpolators
//    {
//        float min = baseNodes[0]->getRho();
//        float max = baseNodes[0]->getRho();
//        for (int z = 1; z < 4; z++) {
//            if (baseNodes[z]->getRho() < min)
//                min = baseNodes[z]->getRho();
//            if (baseNodes[z]->getRho() > max)
//                max = baseNodes[z]->getRho();
//        }
//        for (int z = 0; z < 6; z++) {
//            if (addNodes[z]->getRho() < min)
//                min = addNodes[z]->getRho();
//            if (addNodes[z]->getRho() > max)
//                max = addNodes[z]->getRho();
//        }
//
//        float rho = (baseNodes[0]->getRho() * factor[0] * (2 * factor[0] - 1)
//                + baseNodes[1]->getRho() * factor[1] * (2 * factor[1] - 1)
//                + baseNodes[2]->getRho() * factor[2] * (2 * factor[2] - 1)
//                + baseNodes[3]->getRho() * factor[3] * (2 * factor[3] - 1)
//                + addNodes[0]->getRho() * 4 * factor[0] * factor[1]
//                + addNodes[1]->getRho() * 4 * factor[0] * factor[2]
//                + addNodes[2]->getRho() * 4 * factor[0] * factor[3]
//                + addNodes[3]->getRho() * 4 * factor[1] * factor[2]
//                + addNodes[4]->getRho() * 4 * factor[1] * factor[3]
//                + addNodes[5]->getRho() * 4 * factor[2] * factor[3]
//                );
//
//        if (rho < min)
//            rho = min;
//        if (rho > max)
//            rho = max;
//
//        node.setRho(rho);
//    }

    node.setMaterialId(baseNodes[0]->getMaterialId());

    LOG_TRACE("Interpolation done");
}
