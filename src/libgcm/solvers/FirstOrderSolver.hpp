#ifndef GCM_FIRSTORDERSOLVER_HPP
#define	GCM_FIRSTORDERSOLVER_HPP

#include "libgcm/solvers/Solver.hpp"

namespace gcm {
	class FirstOrderSolver : public Solver
	{
	public:
		FirstOrderSolver();
		~FirstOrderSolver();
		void doNextTimeStep(Mesh *mesh) override;

	};
}
#endif	/* GCM_FIRSTORDERSOLVER_HPP */

