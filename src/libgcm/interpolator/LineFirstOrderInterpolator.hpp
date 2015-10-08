#ifndef GCM_LINE_FIRST_ORDER_INTERPOLATOR_H
#define    GCM_LINE_FIRST_ORDER_INTERPOLATOR_H

#include <string>

#include "libgcm/Math.hpp"
#include "libgcm/util/Logging.hpp"
#include "libgcm/interpolator/TetrInterpolator.hpp"


namespace gcm {
    class Node;

    class LineFirstOrderInterpolator : public TetrInterpolator {
    public:
        LineFirstOrderInterpolator();
        ~LineFirstOrderInterpolator();
        void interpolate( Node& node, Node& node0, Node& node1 );
    private:
        USE_LOGGER;
    };
}

#endif    /* GCM_LINE_FIRST_ORDER_INTERPOLATOR_H */

