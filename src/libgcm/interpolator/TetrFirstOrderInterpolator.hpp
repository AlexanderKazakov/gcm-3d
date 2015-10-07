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
    class CalcNode;

    class TetrFirstOrderInterpolator : public TetrInterpolator {
    public:
        TetrFirstOrderInterpolator();
        ~TetrFirstOrderInterpolator();
        void interpolate( CalcNode& node,
                CalcNode& node0, CalcNode& node1, CalcNode& node2, CalcNode& node3 );
    private:
        USE_LOGGER;
    };
}

#endif    /* GCM_TETR_FIRST_ORDER_INTERPOLATOR_H */

