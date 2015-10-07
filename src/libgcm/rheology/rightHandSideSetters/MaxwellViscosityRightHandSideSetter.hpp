#ifndef MAXWELL_VISCOSITY_RIGHT_HAND_SIDE_SETTER_HPP
#define	MAXWELL_VISCOSITY_RIGHT_HAND_SIDE_SETTER_HPP

#include "libgcm/rheology/rightHandSideSetters/RightHandSideSetter.hpp"

namespace gcm {
/**
 * Class to fill in the right-hand side f(u, t) of the equation 
 * " du/dt + A_x_i * du/dx_i = f(u, t) "
 * according to the Maxwell's model of viscosity.
 */
class MaxwellViscosityRightHandSideSetter : public RightHandSideSetter {
public:
	/**
	 * Fill in the right-hand side f(u, t) of the equation 
	 * " du/dt + A_x_i * du/dx_i = f(u, t) "
	 * @param f f(u, t) vectors u and f have the same size so values 
	 * f(i) are stored at u(i) in this help node f 
	 * @param material pointer to material of the node
	 * @param node node to fill in right-hand side for
	 */
	void setVector(Node& f, const Node& node) const override;
private:

};
}

#endif	/* MAXWELL_VISCOSITY_RIGHT_HAND_SIDE_SETTER_HPP */

