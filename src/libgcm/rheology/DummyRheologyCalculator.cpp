#include "libgcm/rheology/DummyRheologyCalculator.hpp"

#include "libgcm/node/Node.hpp"

using namespace gcm;

DummyRheologyCalculator::DummyRheologyCalculator() {
    INIT_LOGGER("gcm.DummyRheologyCalculator");
}

void DummyRheologyCalculator::doCalc(Node& src, Node& dst)
{
    // do nothing
}
