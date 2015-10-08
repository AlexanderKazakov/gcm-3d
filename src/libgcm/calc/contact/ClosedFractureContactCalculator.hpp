#ifndef _GCM_CONTACT_CLOSEDFRACTURE_CALCULATOR_H
#define _GCM_CONTACT_CLOSEDFRACTURE_CALCULATOR_H  1

#include "libgcm/calc/contact/AdhesionContactCalculator.hpp"
#include "libgcm/calc/contact/SlidingContactCalculator.hpp"
#include "libgcm/util/areas/Area.hpp"

namespace gcm
{
    class ClosedFractureContactCalculator : public ContactCalculator
    {
    public:
        ClosedFractureContactCalculator();
        ~ClosedFractureContactCalculator();
		void setFracArea(Area* area);
        void doCalc(Node& cur_node, Node& new_node, Node& virt_node,
                    RheologyMatrixPtr matrix, std::vector<Node>& previousNodes, bool inner[],
                    RheologyMatrixPtr virt_matrix, std::vector<Node>& virtPreviousNodes, 
					bool virt_inner[], float outer_normal[], float scale);
    private:
		Area* fracArea;
        AdhesionContactCalculator* adhesionCalc;
		SlidingContactCalculator* slidingCalc;
    };
}

#endif /* _GCM_CONTACT_CLOSEDFRACTURE_CALCULATOR_H */
