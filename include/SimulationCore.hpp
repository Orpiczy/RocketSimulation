#pragma once
#include "../common/CommonTypes.hpp"
#include "../common/classes/SimpleLogger.hpp"

//audio
#include <SFML/Audio.hpp>

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
  common::SimulationStatus _simulationStatus{SimulationStatus::READY_TO_START};
  common::RocketParams _rocketParams{.oxygen = 100, .fuel = 100};
  // const float _moonDistanceScale{0.01};
  // common::DestinationParams _destinationParams{
  //     .position = {commonConsts::MOON_DISTANCE * (-0.2) / 100000,
  //                  commonConsts::MOON_DISTANCE *(0.8) / 100000}};
  common::DestinationParams _destinationParams{.position = {2500, 2500}};
  float _minimalMoonDistanceForMissionSuccess{520.0};
  pthread_rwlock_t _simDataLock;

  // SOUND
  sf::SoundBuffer _readyToStartStateSoundBuffer;
  sf::SoundBuffer _activeStateSoundBuffer;
  sf::SoundBuffer _successStateSoundBuffer;
  sf::SoundBuffer _failureStateSoundBuffer;
  void setupSound();

  sf::Sound _backgroundSound;
  void updateBackgroundSoundBasedOnState();


  // CORE
  [[deprecated]] std::thread getAndRunUpdatingSimulationStateInLoopThread();
  std::thread getAndRunRunningSimulationInLoopThread();
  std::thread getAndRunSendingDataInLoopThread();
  std::thread getAndRunUpdatingControlInLoopThread();

  void startSimulation();
  void updateSimulationStatus();
  void updateSimulationStatusInActiveState();
  void updateSystemState(const float &dtAsSeconds);
  void updateLinearMotionPartOfSystemState(const float &dtAsSeconds);
  void updateRotationalMotionPartOfSystemState(const float &dtAsSeconds);
  float getAngleInRadiansInClassicCoordinateSystem(const float &angle);
  void updateIndependentVariables(const float &dtAsSeconds); // O2 and fuel

  // COMMUNICATION
  void sendVisualizationData();
  void sendSimulationStatus();
  void updateCurrentThrustersControl();
  void sendObjectsPosition();
  void sendRocketStatus();

  const std::chrono::milliseconds _updatingSimulationStatePeriod{150};
  const sf::Time _sendingVisualizationDataPeriod{sf::milliseconds(20)}; //~50fps
  const sf::Time _sendingSimulationStatusPeriod{sf::milliseconds(100)};
  const sf::Time _sendingObjectsPositionPeriod{sf::milliseconds(100)};
  const sf::Time _sendingRocketStatusPeriod{sf::milliseconds(100)};

  // COMMUNICATION SETUP
  void openQueues();
  void closeQueues();

  // LOG HELPERS
  void LogReceivedControl(const msg::ThrustersStateMsg &thrusterStateMsg);

  // DEBUG
  void printQueueInfo(const mqd_t &queueFile);
};