#ifndef _GCM_BORDER_CONDITION_H
#define _GCM_BORDER_CONDITION_H 1

#include "libgcm/util/areas/Area.hpp"
#include "libgcm/calc/border/BorderCalculator.hpp"

namespace gcm {
    class PulseForm;
    class Node;

    class BorderCondition
    {
    public:
        BorderCondition();
        BorderCondition(Area* _area, PulseForm* _form, BorderCalculator* _calc);
        ~BorderCondition();

        Area* area;
        PulseForm* form;
        BorderCalculator* calc;
        void doCalc(float time, Node& cur_node, Node& new_node, RheologyMatrixPtr matrix,
                            std::vector<Node>& previousNodes, bool inner[], float outer_normal[]);
    };
}

#endif
