#include "libgcm/solvers/rightHandSideSolvers/FirstOrderRightHandSideSolver.hpp"

using namespace gcm;

void FirstOrderRightHandSideSolver::solve(const Node& curNode,
                                          Node& newNode, Node& f,
                                          const gcm::real tau, 
                                          const RightHandSideSetter& setter) {
	setter.setVector(f, curNode);
	for (int i = 0; i < curNode.getSizeOfPDE(); i++)
		// usual first-order formula for ODE
		newNode.PDE[i] = curNode.PDE[i]
							   + tau * f.PDE[i];	
}
