#ifndef GCM_FailureCriterion_H
#define GCM_FailureCriterion_H

#include <string>

#include "libgcm/Math.hpp"
#include "libgcm/util/Logging.hpp"

#include "libgcm/node/ICalcNode.hpp"


namespace gcm {

    class FailureCriterion {
    public:
        FailureCriterion();
        ~FailureCriterion();
		virtual void checkFailure(ICalcNode& node, const float tau) = 0;
    private:
        USE_LOGGER;
    };
}

#endif

