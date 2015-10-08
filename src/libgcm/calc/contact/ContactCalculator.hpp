/*
 * File:   ContactCalculator.h
 * Author: anganar
 *
 * Created on April 30, 2013, 3:18 PM
 */

#ifndef GCM_CONTACT_CALCULATOR_H_
#define GCM_CONTACT_CALCULATOR_H_  1

#include <string>
#include <vector>

#include "libgcm/rheology/RheologyMatrix.hpp"
#include "libgcm/node/Node.hpp"


namespace gcm
{
    class Node;
    /*
     * Base class for inner points calculators
     */
    class ContactCalculator {
    protected:
    	std::string type;
    public:
        /*
         * Destructor
         */
        virtual ~ContactCalculator() = 0;
        /*
         * Calculate next state for the given node
         */
        virtual void doCalc(Node& cur_node, Node& new_node, Node& virt_node,
                            RheologyMatrixPtr matrix, std::vector<Node>& previousNodes, bool inner[],
                            RheologyMatrixPtr virt_matrix, std::vector<Node>& virtPreviousNodes, bool virt_inner[],
                            float outer_normal[], float scale) = 0;
        /*
         * Returns type of the calculator
         */
        std::string getType();

    };

}

#endif    /* GCM_CONTACT_CALCULATOR_H_ */
