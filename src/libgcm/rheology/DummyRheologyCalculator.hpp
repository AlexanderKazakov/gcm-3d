#ifndef GCM_DUMMY_RHEOLOGY_CALCULATOR_H_
#define GCM_DUMMY_RHEOLOGY_CALCULATOR_H_

#include <string>

#include "libgcm/rheology/RheologyCalculator.hpp"
#include "libgcm/util/Logging.hpp"


namespace gcm {
    class Node;

    class DummyRheologyCalculator : public RheologyCalculator {
    public:
        DummyRheologyCalculator();
        /*
         * Returns rheology calculator type
         */
        inline std::string getType() {
            return "DummyRheologyCalculator";
        }
        void doCalc(Node& src, Node& dst);
    protected:
        USE_LOGGER;
    };
}


#endif
