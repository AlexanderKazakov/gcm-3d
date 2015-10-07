#ifndef IDEAL_ELASTIC_NODE_HPP
#define IDEAL_ELASTIC_NODE_HPP

#include "libgcm/util/Types.hpp"
#include "libgcm/util/NodeTypes.hpp"
#include "libgcm/node/Node.hpp"
#include "libgcm/util/Math.hpp"
#include "libgcm/util/Assertion.hpp"


namespace gcm {

    /**
     * Node for models with:
     * (a) ideal elasticity;
     * (b) infinitely small deformations;
     * (c) only brittle material failure.
     * Can be used for both isotropic and anisotropic models.
     */
    class IdealElasticNode : public Node {
    public:

        /**
         * Creates node with: 
         *     type == IDEAL_ELASTIC_NODE_TYPE
         *     valuesInPDE == 9 (velocity vector and stress tensor)
         *     valuesInODE == 0
         */
        IdealElasticNode();

		/**
         * Creates node with: 
         *     type == type
         *     valuesInPDE == numOfPDE
         *     valuesInODE == numOfODE
		 * 
		 * Used in constructors of nodes derived from IdealElasticNode.
		 * 
         * @param numOfPDE number of values in vector from PDE
         * @param numOfODE number of values calculated by ODE's
         * @param type NODE_TYPE
         */
		IdealElasticNode(uchar numOfPDE, uchar numOfODE, uchar type);
        /**
         * Velocity vector x component.
         */
        gcm::real getVx() const;
        /**
         * Velocity vector y component.
         */
        gcm::real getVy() const;
        /**
         * Velocity vector z component.
         */
        gcm::real getVz() const;

        /**
         * Stress tensor xx component.
         */
        gcm::real getSxx() const;
        /**
         * Stress tensor xy component.
         */
        gcm::real getSxy() const;
        /**
         * Stress tensor xz component.
         */
        gcm::real getSxz() const;
        /**
         * Stress tensor yy component.
         */
        gcm::real getSyy() const;
        /**
         * Stress tensor yz component.
         */
        gcm::real getSyz() const;
        /**
         * Stress tensor zz component.
         */
        gcm::real getSzz() const;
        
        // Read-write access. These methods are slower than get-ters above.
        gcm::real& vx();
        gcm::real& vy();
        gcm::real& vz();
        gcm::real& sxx();
        gcm::real& sxy();
        gcm::real& sxz();
        gcm::real& syy();
        gcm::real& syz();
        gcm::real& szz();
        
        /**
         * Density
         */
        // FIXME@avasyukov - return it back after getMaterial() is back in CalcNode
        //gcm::real getRho() const;

        /**
         * Returns compression.
         *
         * @return Compression value.
         */
        gcm::real getCompression() const;
        /**
         * Returns compression.
         *
         * @return Compression value.
         */
        gcm::real getTension() const;
        /**
         * Returns shear.
         *
         * @return Shear value.
         */
        gcm::real getShear() const;
        /**
         * Returns stress deviator.
         *
         * @return Stress deviator value.
         */
        gcm::real getStressDeviator() const;
        /**
         * Returns hydrostatic pressure.
         *
         * @return Hydrostatic pressure value.
         */
        gcm::real getPressure() const;

        /**
         * Returns principal stresses for the node.
         * See the math at http://en.wikipedia.org/wiki/Cauchy_stress_tensor#Principal_stresses_and_stress_invariants
         *
         * @return Vector of principal stresses.
         */
        vector3r getPrincipalStresses() const;
        /**
         * Returns the first stress invariant for the node.
         * See the math at http://en.wikipedia.org/wiki/Cauchy_stress_tensor#Principal_stresses_and_stress_invariants
         *
         * @return J1 value.
         */
        gcm::real getStressInvariantJ1() const;
        /**
         * Returns the second stress invariant for the node.
         * See the math at http://en.wikipedia.org/wiki/Cauchy_stress_tensor#Principal_stresses_and_stress_invariants
         *
         * @return J2 value.
         */
        gcm::real getStressInvariantJ2() const;
        /**
         * Returns the third stress invariant for the node.
         * See the math at http://en.wikipedia.org/wiki/Cauchy_stress_tensor#Principal_stresses_and_stress_invariants
         *
         * @return J3 value.
         */
        gcm::real getStressInvariantJ3() const;
    };
}

#endif /* IDEAL_ELASTIC_NODE_HPP */