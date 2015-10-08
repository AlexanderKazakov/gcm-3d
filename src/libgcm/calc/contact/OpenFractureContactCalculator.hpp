#ifndef _GCM_CONTACT_OPENFRACTURE_CALCULATOR_H
#define _GCM_CONTACT_OPENFRACTURE_CALCULATOR_H  1

#include "libgcm/calc/contact/AdhesionContactCalculator.hpp"
#include "libgcm/calc/border/FreeBorderCalculator.hpp"
#include "libgcm/util/areas/Area.hpp"

namespace gcm
{
    class OpenFractureContactCalculator : public ContactCalculator
    {
    public:
        OpenFractureContactCalculator();
        ~OpenFractureContactCalculator();
		void setFracArea(Area* area);
        void doCalc(Node& cur_node, Node& new_node, Node& virt_node,
                    RheologyMatrixPtr matrix, std::vector<Node>& previousNodes, bool inner[],
                    RheologyMatrixPtr virt_matrix, std::vector<Node>& virtPreviousNodes, 
					bool virt_inner[], float outer_normal[], float scale);
    private:
		Area* fracArea;
        AdhesionContactCalculator* adhesionCalc;
		FreeBorderCalculator* borderCalc;
    };
}

#endif /* _GCM_CONTACT_OPENFRACTURE_CALCULATOR_H */
