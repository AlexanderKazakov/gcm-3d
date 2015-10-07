#ifndef RightHandSideSetter_HPP
#define RightHandSideSetter_HPP 

#include "libgcm/rheology/Material.hpp"
#include "libgcm/nodes/IdealElasticNode.hpp"
#include "libgcm/nodes/Node.hpp"

namespace gcm
{
/**
 * Base class to fill in the right-hand side f(u, t) of the equation 
 * " du/dt + A_x_i * du/dx_i = f(u, t) "
 */
class RightHandSideSetter
{
public:
	/**
	 * Fill in the right-hand side f(u, t) of the equation 
	 * " du/dt + A_x_i * du/dx_i = f(u, t) "
	 * @param f f(u, t) vectors u and f have the same size so values 
	 * f(i) are stored at u(i) in this help node f 
	 * @param material pointer to material of the node
	 * @param node node to fill in right-hand side for
	 */
	virtual void setVector(Node& f, const Node& node) const = 0;
};
}

#endif /* RightHandSideSetter_HPP */