#ifndef _GCM_CONTACT_CONDITION_H
#define _GCM_CONTACT_CONDITION_H 1

#include "libgcm/util/areas/Area.hpp"
#include "libgcm/util/forms/PulseForm.hpp"
#include "libgcm/calc/contact/ContactCalculator.hpp"

namespace gcm {
    class PulseForm;
    class Node;
	class ContactCalculator;

    class ContactCondition
    {
    protected:
        /** 
         * Parameter of contact condition. It is interpreted by contact calculator.
         * Depending on contact type it can be adhesion treshold, friction coefficient or smth else.
         */
        real conditionParam;
        
    public:
        ContactCondition();
        ContactCondition(Area* _area, PulseForm* _form, ContactCalculator* _calc);
        ~ContactCondition();

        Area* area;
        PulseForm* form;
        ContactCalculator* calc;
        void doCalc(float time, Node& cur_node, Node& new_node, Node& virt_node,
                    RheologyMatrixPtr matrix, std::vector<Node>& previousNodes, bool inner[],
                    RheologyMatrixPtr virt_matrix, std::vector<Node>& virtPreviousNodes, bool virt_inner[],
                    float outer_normal[]);
        
        void setConditionParam(real param);
        real getConditionParam() const;
    };
}

#endif
