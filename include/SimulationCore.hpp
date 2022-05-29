#pragma once
#include "../common/CommonTypes.hpp"
#include "../common/classes/SimpleLogger.hpp"

// comms
#include "../common/MessageTypes.hpp"
#include <mqueue.h>

// sim
#include "../common/PhysicalConstants.hpp"
#include <thread>

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

  std::thread getAndRunRunningSimulationInLoopThread();
  void startSimulation();
  void updateSystemState(const float &dtAsSeconds);
  void updateLinearMotionPartOfSystemState(const float &dtAsSeconds);
  void updateRotationalMotionPartOfSystemState(const float &dtAsSeconds);
  float getAngleInClassicCartesianCoordinateSystem(
      const float &angleInWindowCoordinateSystem);

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