/*
 * File:   TetrFirstOrderInterpolator.h
 * Author: anganar
 *
 * Created on May 3, 2013, 12:13 AM
 */

#ifndef GCM_TETR_FIRST_ORDER_INTERPOLATOR_H
#define    GCM_TETR_FIRST_ORDER_INTERPOLATOR_H

#include <string>

#include "libgcm/Math.hpp"
#include "libgcm/util/Logging.hpp"
#include "libgcm/interpolator/TetrInterpolator.hpp"


namespace gcm {
    class Node;

    class TetrFirstOrderInterpolator : public TetrInterpolator {
    public:
        TetrFirstOrderInterpolator();
        ~TetrFirstOrderInterpolator();
        void interpolate( Node& node,
                Node& node0, Node& node1, Node& node2, Node& node3 );
    private:
        USE_LOGGER;
    };
}

#endif    /* GCM_TETR_FIRST_ORDER_INTERPOLATOR_H */

