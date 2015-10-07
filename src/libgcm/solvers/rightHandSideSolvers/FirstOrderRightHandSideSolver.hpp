#ifndef FIRST_ORDER_RIGHT_HAND_SIDE_SOLVER_HPP
#define	FIRST_ORDER_RIGHT_HAND_SIDE_SOLVER_HPP

#include "libgcm/solvers/rightHandSideSolvers/RightHandSideSolver.hpp"


namespace gcm {

/**
 * Class to implement first order of accuracy formula to calculate 
 * the " du/dt = f(u, t) " part of the whole equation. 
 * The whole equation " du/dt + A_x_i * du/dx_i = f(u, t) " is splitted on
 * this part and spatial coordinates parts. 
 */

class FirstOrderRightHandSideSolver : public RightHandSideSolver {
public:
	/**
	 * Solve " du/dt = f(u, t) " with first order of accuracy
	 * @param curNode initial value
	 * @param newNode final value
	 * @param f temporary node to store f(u, t) 
	 * @param tau (final time) minus (initial time)
	 * @param setter pointer to the evaluator of f(u, t)
	 */
	void solve(const Node& curNode, 
	           Node& newNode, Node& f,
	           const gcm::real tau, 
	           const RightHandSideSetter& setter) override;
private:

};
}

#endif	/* FIRST_ORDER_RIGHT_HAND_SIDE_SOLVER_HPP */

