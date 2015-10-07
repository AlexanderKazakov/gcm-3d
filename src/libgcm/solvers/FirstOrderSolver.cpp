#include "libgcm/solvers/FirstOrderSolver.hpp"

using namespace gcm;


FirstOrderSolver::FirstOrderSolver() {
	type = "FirstOrderSolver";
}

FirstOrderSolver::~FirstOrderSolver() {
}

void FirstOrderSolver::doNextTimeStep(Mesh *mesh) {
	std::cout << "doNextTimeStep. Worker: " << MPI::COMM_WORLD.Get_rank()
	          << " type of solver: " << type 
	          << " meshId: " << mesh->getId()
	          << " meshRank: " << mesh->getRank()
	          << " meshType: " << mesh->getType()
	          << std::endl;
	
	mesh->stageX();
	
	auto correctors = mesh->getRheologyModel()->getCorrectors();
	for(auto corrector = correctors.begin(); corrector != correctors.end(); corrector++) {
		(*corrector)->apply(mesh);
	}
}
