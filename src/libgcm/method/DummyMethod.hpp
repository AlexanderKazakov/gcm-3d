#ifndef GCM_DUMMY_METHOD_H
#define    GCM_DUMMY_METHOD_H

#include "libgcm/method/NumericalMethod.hpp"

namespace gcm {
    /*
     * Numerical method
     */
    class DummyMethod : public NumericalMethod {
    public:
        /*
         * Constructor
         */
        DummyMethod();
        /*
         * Returns number of stages (part steps) this particular method requires
         */
        int getNumberOfStages();
        /*
         * Computes next state (after the next part step) for the given node
         */
        void doNextPartStep(Node& cur_node, Node& new_node, float time_step, int stage, Mesh* mesh);

        std::string getType();

    protected:
        USE_LOGGER;
    };
}

#endif    /* GCM_DUMMY_METHOD_H */
