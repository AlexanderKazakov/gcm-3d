#ifndef _GCM_SIMPLE_VOLUME_CALCULATOR_H
#define _GCM_SIMPLE_VOLUME_CALCULATOR_H  1

#include "libgcm/calc/volume/VolumeCalculator.hpp"
#include "libgcm/rheology/RheologyMatrix.hpp"
#include "libgcm/util/Logging.hpp"


namespace gcm
{
    class SimpleVolumeCalculator : public VolumeCalculator
    {
    public:
        SimpleVolumeCalculator();
        void doCalc(Node& new_node, RheologyMatrixPtr matrix,
                                        std::vector<Node>& previousNodes);
        inline std::string getType() {
            return "SimpleVolumeCalculator";
        }

    protected:
        USE_LOGGER;

    };
}

#endif
