#ifndef _GCM_SMOOTH_BORDER_CALCULATOR_H
#define _GCM_SMOOTH_BORDER_CALCULATOR_H  1

#include <gsl/gsl_linalg.h>

#include "libgcm/calc/border/BorderCalculator.hpp"

namespace gcm
{

    class SmoothBorderCalculator : public BorderCalculator
    {
    public:
        SmoothBorderCalculator();
        ~SmoothBorderCalculator();
        void doCalc(Node& cur_node, Node& new_node, RheologyMatrixPtr matrix,
                                std::vector<Node>& previousNodes, bool inner[],
                                float outer_normal[], float scale);

        inline std::string getType() {
            return "SmoothBorderCalculator";
        }

        void setParameters(const xml::Node& params);

    protected:

    private:
        USE_LOGGER;
    };
}

#endif
