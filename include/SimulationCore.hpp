#pragma once
#include "../common/CommonTypes.hpp"
#include "../common/classes/SimpleLogger.hpp"

// comms
#include "../common/MessageTypes.hpp"

class SimulationCore : public SimpleLogger {
public:
  SimulationCore(bool isLogInfoEnable = true, bool isLogErrorEnable = true);

  ~SimulationCore();

  void run();

private:
  common::RocketParams _rocketParams;
  common::DestinationParams _destinationParams;

  // CORE
  void startSimulation();

  // COMMUNICATION
  void getCurrentThrustersControl();
  void sendObjectsPosition();
  void sendRocketStatus();

  // COMMUNICATION SETUP
  void openQueues();
  void closeQueues();

  ////HELPERS
  void LogReceicedControl(const msg::ThrustersStateMsg &thrusterStateMsg);
};