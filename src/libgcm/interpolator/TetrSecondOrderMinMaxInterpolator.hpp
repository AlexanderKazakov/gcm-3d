/*
 * File:   TetrSecondOrderMinMaxInterpolator.h
 * Author: anganar
 *
 * Created on May 3, 2013, 12:13 AM
 */

#ifndef GCM_TETR_SECOND_ORDER_MINMAX_INTERPOLATOR_H
#define    GCM_TETR_SECOND_ORDER_MINMAX_INTERPOLATOR_H

#include <string>

#include "libgcm/Math.hpp"
#include "libgcm/util/Logging.hpp"
#include "libgcm/interpolator/TetrInterpolator.hpp"


namespace gcm {
    class Node;

    class TetrSecondOrderMinMaxInterpolator : public TetrInterpolator {
    public:
        TetrSecondOrderMinMaxInterpolator();
        ~TetrSecondOrderMinMaxInterpolator();
        void interpolate( Node& node,
                Node& node0, Node& node1, Node& node2, Node& node3,
                Node& addNode0, Node& addNode1, Node& addNode2,
                Node& addNode3, Node& addNode4, Node& addNode5 );
    private:
        Node* baseNodes[4];
        Node* addNodes[6];
        USE_LOGGER;
    };
}

#endif    /* GCM_TETR_FIRST_ORDER_INTERPOLATOR_H */

