#include "libgcm/solvers/rightHandSideSolvers/SecondOrderRightHandSideSolver.hpp"

using namespace gcm;

void SecondOrderRightHandSideSolver::solve(const Node& curNode,
                                          Node& newNode, Node& f,
                                          const gcm::real tau, 
                                          const RightHandSideSetter& setter) {
	// second-order "predictor-corrector" formula for ODE
	
	// predictor
	setter.setVector(f, curNode);
	for (int i = 0; i < curNode.getSizeOfPDE(); i++)
		newNode.PDE[i] = curNode.PDE[i]
							   + tau / 2 * f.PDE[i];
	// corrector
	setter.setVector(f, newNode);
	for (int i = 0; i < curNode.getSizeOfPDE(); i++)
		newNode.PDE[i] = curNode.PDE[i]
							   + tau * f.PDE[i];
}
