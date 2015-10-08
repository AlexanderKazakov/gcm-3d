#ifndef _GCM_CONTACT_ADHESION_D_CALCULATOR_H
#define _GCM_CONTACT_ADHESION_D_CALCULATOR_H  1

#include <gsl/gsl_linalg.h>

#include "libgcm/calc/contact/ContactCalculator.hpp"
#include "libgcm/calc/contact/AdhesionContactCalculator.hpp"
#include "libgcm/calc/contact/SlidingContactCalculator.hpp"


namespace gcm
{
    class AdhesionContactDestroyCalculator : public ContactCalculator
    {
    public:
        AdhesionContactDestroyCalculator();
        ~AdhesionContactDestroyCalculator();
        void doCalc(Node& cur_node, Node& new_node, Node& virt_node,
                                RheologyMatrixPtr matrix, std::vector<Node>& previousNodes, bool inner[],
                                RheologyMatrixPtr virt_matrix, std::vector<Node>& virtPreviousNodes, bool virt_inner[],
                                float outer_normal[], float scale);
    private:
        SlidingContactCalculator* scc;
        AdhesionContactCalculator* acc;
    };
}
#endif
