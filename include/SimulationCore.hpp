#pragma once
#include "../common/CommonTypes.hpp"
#include "../common/classes/SimpleLogger.hpp"

class SimulationCore : public SimpleLogger {
public:
  SimulationCore(bool isLogInfoEnable = true, bool isLogErrorEnable = true);

  ~SimulationCore();

  void run();

private:
  common::RocketParams rocketParams;
  common::DestinationParams destinationParams;

  // CORE
  void startSimulation();
  // COMMUNICATION
  void updateThrustersInformation();
};