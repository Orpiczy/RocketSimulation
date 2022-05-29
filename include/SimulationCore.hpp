#pragma once
#include "../common/CommonTypes.hpp"
#include "../common/classes/SimpleLogger.hpp"

// comms
#include "../common/MessageTypes.hpp"
#include <mqueue.h>

// sim
#include "../common/PhysicalConstants.hpp"

class SimulationCore : public SimpleLogger {
public:
  SimulationCore(bool isLogInfoEnable = true, bool isLogErrorEnable = true);

  ~SimulationCore();

  void run();

private:
  common::RocketParams _rocketParams;
  common::DestinationParams _destinationParams{
      .position = {commonConsts::MOON_DISTANCE * (-0.2),
                   commonConsts::MOON_DISTANCE *(0.8)}};

  // CORE
  void startSimulation();

  // COMMUNICATION
  void sendVisualizationData();
  void getCurrentThrustersControl();
  void sendObjectsPosition();
  void sendRocketStatus();

  // COMMUNICATION SETUP
  void openQueues();
  void closeQueues();

  //// LOG HELPERS
  void LogReceivedControl(const msg::ThrustersStateMsg &thrusterStateMsg);

  //// DEBUG
  void printQueueInfo(const mqd_t &queueFile);
};