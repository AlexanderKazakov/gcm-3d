#ifndef _GCM_FREE_BORDER_CALCULATOR_H
#define _GCM_FREE_BORDER_CALCULATOR_H  1

#include <gsl/gsl_linalg.h>

#include "libgcm/calc/border/BorderCalculator.hpp"

namespace gcm
{
    class FreeBorderCalculator : public BorderCalculator
    {
    public:
        FreeBorderCalculator();
        ~FreeBorderCalculator();
        void doCalc(Node& cur_node, Node& new_node, RheologyMatrixPtr matrix,
                                std::vector<Node>& previousNodes, bool inner[],
                                float outer_normal[], float scale);

        inline std::string getType() {
            return "FreeBorderCalculator";
        }

        void setParameters(const xml::Node& params);

    protected:

    private:
        USE_LOGGER;
        // Used for border calculation
        gsl_matrix *U_gsl;
        gsl_vector *om_gsl;
        gsl_vector *x_gsl;
        gsl_permutation *p_gsl;
    };
}
#endif
