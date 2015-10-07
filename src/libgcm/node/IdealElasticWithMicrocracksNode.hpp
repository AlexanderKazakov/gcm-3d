#ifndef IDEAL_ELASTIC_WITH_MICROCRACKS_NODE_HPP
#define IDEAL_ELASTIC_WITH_MICROCRACKS_NODE_HPP

#include "libgcm/node/IdealElasticNode.hpp"
#include "libgcm/util/Types.hpp"

namespace gcm {

    /**
     * Node for models with:
     * (a) ideal elasticity;
     * (b) infinitely small deformations;
     * (c) only brittle material failure.
     * Can be used for both isotropic and anisotropic models.
     * Compared to IdealElasticNode this node adds micro cracks (separate crack in each node).
     */
    class IdealElasticWithMicrocracksNode : public IdealElasticNode {
    public:

        /**
         * Creates node with: 
         *     type == IDEAL_ELASTIC_WITH_MICROCRACKS_NODE_TYPE
         *     valuesInPDE == 9 (velocity vector and stress tensor)
         *     valuesInODE == 3 (vector for crack orientation)
         */
        IdealElasticWithMicrocracksNode(); //: CalcNode(9, 3, IDEAL_ELASTIC_WITH_MICROCRACKS_NODE_TYPE);

        // TODO@amisto Review, cleanup and document the methods.

        const vector3r& getCrackDirection() const;
        void setCrackDirection(const vector3r& crack);
        void setCrackDirection(real x, real y, real z);
        void createCrack(int direction);
        // TODO@amisto Rename it - 'excise' stands for 'акциз' mostly.
        void exciseByCrack();
        void cleanStressByDirection(const vector3r& direction);

    };
}

#endif /* IDEAL_ELASTIC_WITH_MICROCRACKS_NODE_HPP */