#ifndef _GCM_CONTACT_SLIDING_CALCULATOR_H
#define _GCM_CONTACT_SLIDING_CALCULATOR_H  1

#include <gsl/gsl_linalg.h>

#include "libgcm/calc/contact/ContactCalculator.hpp"
#include "libgcm/calc/border/FreeBorderCalculator.hpp"
#include "libgcm/Math.hpp"
#include "libgcm/util/Logging.hpp"

namespace gcm
{
    class SlidingContactCalculator : public ContactCalculator
    {
    public:
        SlidingContactCalculator();
        ~SlidingContactCalculator();
        void doCalc(Node& cur_node, Node& new_node, Node& virt_node,
                                RheologyMatrixPtr matrix, std::vector<Node>& previousNodes, bool inner[],
                                RheologyMatrixPtr virt_matrix, std::vector<Node>& virtPreviousNodes, bool virt_inner[],
                                float outer_normal[], float scale);
    private:
        USE_LOGGER;
        FreeBorderCalculator *fbc;
        // Used for border calculation
        gsl_matrix *U_gsl;
        gsl_vector *om_gsl;
        gsl_vector *x_gsl;
        gsl_permutation *p_gsl;
		bool isFreeBorder(Node& node, Node& virt_node, float outer_normal[]);
    };
}

#endif
