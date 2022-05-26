#pragma once
#include "../common/CommonTypes.hpp"
#include "../common/classes/SimpleLogger.hpp"

class SimulationCore : public SimpleLogger {
public:
  SimulationCore(bool isLogInfoEnable = true, bool isLogErrorEnable = true)
      : SimpleLogger(isLogInfoEnable, isLogErrorEnable) {}
  void run();

private:
  common::RocketParams rocketParams;
  common::DestinationParams destinationParams;

  // COMMUNICATION
  void updateThrusterState();
};