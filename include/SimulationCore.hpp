#pragma once
#include "../common/CommonTypes.hpp"

class SimulationCore {
public:
    void run();
private:
    common::RocketParams rocketParams;
    common::DestinationParams destinationParams;
 
};