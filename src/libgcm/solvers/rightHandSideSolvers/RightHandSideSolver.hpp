#ifndef RIGHT_HAND_SIDE_SOLVER_HPP
#define RIGHT_HAND_SIDE_SOLVER_HPP 

#include "libgcm/rheology/Material.hpp"
#include "libgcm/node/Node.hpp"
#include "libgcm/rheology/rightHandSideSetters/RightHandSideSetter.hpp"


namespace gcm
{
	
/**
 * Base class to implement calculation of 
 * the " du/dt = f(u, t) " part of the whole equation. 
 * The whole equation " du/dt + A_x_i * du/dx_i = f(u, t) " is splitted on
 * this part and spatial coordinates parts. 
 */

class RightHandSideSolver
{
public:
	/**
	 * Solve " du/dt = f(u, t) "
	 * @param curNode initial value
	 * @param newNode final value
	 * @param f temporary node to store f(u, t) 
	 * @param tau (final time) minus (initial time)
	 * @param setter pointer to the evaluator of f(u, t)
	 */
	virtual void solve(const Node& curNode, 
	                   Node& newNode, Node& f,
					   const gcm::real tau, 
					   const RightHandSideSetter& setter) = 0;
};

}

#endif /* RIGHT_HAND_SIDE_SOLVER_HPP */