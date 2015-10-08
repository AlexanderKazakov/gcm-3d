#include "libgcm/interpolator/LineFirstOrderInterpolator.hpp"

#include "libgcm/node/Node.hpp"

using namespace gcm;

LineFirstOrderInterpolator::LineFirstOrderInterpolator()
{
    type = "LineFirstOrderInterpolator";
    INIT_LOGGER("gcm.LineFirstOrderInterpolator");
}

LineFirstOrderInterpolator::~LineFirstOrderInterpolator()
{
}

void LineFirstOrderInterpolator::interpolate(Node& node, Node& node0, Node& node1)
{
    LOG_TRACE("Start interpolation");

    float lenTotal = distance(node0.coords, node1.coords);

    float factor0 = distance(node.coords, node1.coords) / lenTotal;

    float factor1 = distance(node.coords, node0.coords) / lenTotal;

    // If we see potential instability
    if (factor0 + factor1 > 1.0) {
        // If it is small - treat instability as minor and just 'smooth' it
        if (factor0 + factor1 < 1 + EQUALITY_TOLERANCE) // FIXME@avasyukov
        {
            float sum = factor0 + factor1;
            factor0 = factor0 / sum;
            factor1 = factor1 / sum;
        }
            // Throw exception
        else {
            LOG_ERROR("Requested node: " << node);
            LOG_ERROR("Node #1: " << node0);
            LOG_ERROR("Node #2: " << node1);
            LOG_ERROR("Factor: " << factor0 + factor1);
            THROW_BAD_MESH("Sum of factors is greater than 1.0");
        }
    }

    for (int i = 0; i < node.getSizeOfPDE(); i++) {
        node.PDE[i] = (node0.PDE[i] * factor0 + node1.PDE[i] * factor1);
    }
//	TODO@next - do smth with rho in interpolators
//    node.setRho(node0.getRho() * factor0 + node1.getRho() * factor1);
    node.setMaterialId(node0.getMaterialId());
    LOG_TRACE("Interpolation done");
}
