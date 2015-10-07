#ifndef GCM_SOLVER_HPP
#define GCM_SOLVER_HPP

#include <string>
#include <iostream>
#include <mpi.h>

#include "libgcm/meshes/Mesh.hpp"
#include "libgcm/rheology/models/RheologyModel.hpp"

//#include "libgcm/calc/volume/VolumeCalculator.hpp"
//#include "libgcm/calc/border/BorderCalculator.hpp"
//#include "libgcm/calc/contact/ContactCalculator.hpp"
//#include "libgcm/interpolator/TetrInterpolator.hpp"
//#include "libgcm/interpolator/TetrFirstOrderInterpolator.hpp"
//#include "libgcm/interpolator/TetrSecondOrderMinMaxInterpolator.hpp"
//#include "libgcm/interpolator/LineFirstOrderInterpolator.hpp"


namespace gcm {
    /*
     * Numerical method
     */
    class Solver {
	protected:
		std::string type;
    public:
        /*
         * Constructor
         */
        Solver();
        /*
         * Destructor
         */
        virtual ~Solver() = 0;

        /*
         * Computes next state (after the next part step) for the given node
         */
        // TODO: does it mean that solver will contain everything that is in Engine now?
        virtual void doNextTimeStep(Mesh *mesh) = 0;

        std::string getType() {
			return type;
		};

		/*	Comment until it will be realized in derived classes
        int getSpaceOrder();
        int getTimeOrder();

        virtual const RheologyModel& getRheologyModel() const = 0;
        virtual const FailureModel& getFailureModel() const = 0;
        virtual const CrossPointFinder& getCrossPointFinder() const = 0;
        // TODO: Solver provides default decomposer but the model can provide custom one
        virtual const RheologyMatrixDecomposer& getRheologyMatrixDecomposer() const = 0;
        virtual const RightHandSideSolver& getRightHandSideSolver() const = 0;
        // TODO: how can we make Solver working with different meshes?
        virtual const Interpolator& getInterpolator() const = 0;
        virtual const MeshMover& getMeshMover() const = 0;
		*/
    };
}

#endif    /* GCM_SOLVER_HPP */