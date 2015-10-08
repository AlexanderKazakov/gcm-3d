#include "libgcm/BorderCondition.hpp"

#include "libgcm/util/forms/PulseForm.hpp"
#include "libgcm/node/Node.hpp"

using namespace gcm;
using std::vector;

BorderCondition::BorderCondition() {
    area = NULL;
    form = NULL;
    calc = NULL;
}

BorderCondition::BorderCondition(Area* _area, PulseForm* _form, BorderCalculator* _calc) {
    area = _area;
    form = _form;
    calc = _calc;
}

BorderCondition::~BorderCondition() {

}

void BorderCondition::doCalc(float time, Node& cur_node, Node& new_node, RheologyMatrixPtr matrix,
                            vector<Node>& previousNodes, bool inner[], float outer_normal[])
{
    calc->doCalc(cur_node, new_node, matrix, previousNodes, inner, outer_normal,
                    form->calcMagnitudeNorm(time, cur_node.coords, area) );
};
