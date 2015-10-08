#include "libgcm/interpolator/LineSecondOrderInterpolator.hpp"

#include "libgcm/node/Node.hpp"

using namespace gcm;

LineSecondOrderInterpolator::LineSecondOrderInterpolator()
{
    type = "LineSecondOrderInterpolator";
    INIT_LOGGER("gcm.LineSecondOrderInterpolator");
}

LineSecondOrderInterpolator::~LineSecondOrderInterpolator()
{
}

void LineSecondOrderInterpolator::interpolate(Node& node, Node& nodeLeft, Node& nodeCur, Node& nodeRight)
{
    LOG_TRACE("Start interpolation");

    float lenLeft = distance(nodeCur.coords, nodeLeft.coords);
    float lenRight = distance(nodeCur.coords, nodeRight.coords);
    if( fabs(lenLeft - lenRight) > EQUALITY_TOLERANCE * (lenLeft + lenRight) * 0.5 )
        THROW_BAD_MESH("LineSecondOrderInterpolator can work on uniform grid only");
    
    float h = (lenLeft + lenRight) * 0.5;
    // FIXME_ASAP
    float x = (node.coords.x - nodeCur.coords.x) + (node.coords.y - nodeCur.coords.y) + (node.coords.z - nodeCur.coords.z);

    for (int i = 0; i < node.getSizeOfPDE(); i++) {
        float rVal = nodeRight.PDE[i];
        float lVal = nodeLeft.PDE[i];
        float cVal = nodeCur.PDE[i];
        float a = (rVal - 2 * cVal + lVal) / (2 * h * h);
        float b = (rVal - lVal) / (2 * h);
        float c = cVal;
        node.PDE[i] = a * x * x + b * x + c;
        float max = ( rVal > cVal ? (rVal > lVal ? rVal : lVal) : (cVal > lVal ? cVal : lVal) );
        float min = ( rVal < cVal ? (rVal < lVal ? rVal : lVal) : (cVal < lVal ? cVal : lVal) );
        if(node.PDE[i] > max)
        {
            node.PDE[i] = max;
        }
        if(node.PDE[i] < min)
        {
            node.PDE[i] = min;
        }
    }

//	TODO@next - do smth with rho in interpolators
    node.setMaterialId(nodeCur.getMaterialId());

    LOG_TRACE("Interpolation done");
}
