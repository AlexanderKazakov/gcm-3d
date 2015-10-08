#ifndef _GCM_EXTERNAL_VALUES_CALCULATOR_H
#define _GCM_EXTERNAL_VALUES_CALCULATOR_H  1

#include <gsl/gsl_linalg.h>

#include "libgcm/calc/border/BorderCalculator.hpp"

namespace gcm
{
    class ExternalValuesCalculator : public BorderCalculator
    {
    public:
        ExternalValuesCalculator();
        ~ExternalValuesCalculator();
        void doCalc(Node& cur_node, Node& new_node, RheologyMatrixPtr matrix,
                                std::vector<Node>& previousNodes, bool inner[],
                                float outer_normal[], float scale) override;
        inline std::string getType() {
            return "ExternalValuesCalculator";
        }
        void setParameters(const xml::Node& params);
        void set_parameters(int vars[], float vals[]);

    protected:

    private:
        int vars_index[3];
        float vars_values[3];

        // Used for border calculation
        gsl_matrix *U_gsl;
        gsl_vector *om_gsl;
        gsl_vector *x_gsl;
        gsl_permutation *p_gsl;
    };
}
#endif
