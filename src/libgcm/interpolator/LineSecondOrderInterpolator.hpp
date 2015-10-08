#ifndef GCM_LINE_SECOND_ORDER_INTERPOLATOR_H
#define GCM_LINE_SECOND_ORDER_INTERPOLATOR_H

#include <string>

#include "libgcm/Math.hpp"
#include "libgcm/util/Logging.hpp"
#include "libgcm/interpolator/TetrInterpolator.hpp"


namespace gcm {
    class Node;

    class LineSecondOrderInterpolator : public TetrInterpolator {
    public:
        LineSecondOrderInterpolator();
        ~LineSecondOrderInterpolator();
        void interpolate( Node& node, Node& nodeLeft, Node& nodeCur, Node& nodeRight );
    private:
        USE_LOGGER;
    };
}

#endif    /* GCM_LINE_SECOND_ORDER_INTERPOLATOR_H */

