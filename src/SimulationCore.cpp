#include "../include/SimulationCore.hpp"
#include "../common/classes/LogPublisher.hpp"
#include "../include/ControlWindow.hpp"
#include "../include/MainWindow.hpp"
#include "../include/RadarWindow.hpp"
#include "../include/StatusWindow.hpp"

// multiprocess
#include <sys/wait.h>
#include <unistd.h>

// comms
#include "../common/CommunicationInfo.hpp"

// sim
#include "../common/PhysicalConstants.hpp"

// debug
#include <sstream>

SimulationCore::SimulationCore(bool isLogInfoEnable, bool isLogErrorEnable)
    : SimpleLogger(isLogInfoEnable, isLogErrorEnable) {
  openQueues();
}

SimulationCore::~SimulationCore() { closeQueues(); }

void SimulationCore::run() {

  pid_t pid;
  uint8_t processNr{0};
  uint8_t nrOfNeededChildProcesses{5};

  /* Process spawning */
  while (processNr < nrOfNeededChildProcesses) {
    pid = fork();
    switch (pid) {
    case 0:
      processNr++;
      continue;
    case -1:
      printf("Error while creating new process");
      return;

    default:
      printf("Successful process spawn of process, pid = %d, my parent pid =  "
             "%d, childProcessNumber %d\n",
             getpid(), getppid(), processNr);
      goto spawnPoint;
    }
  }

  /* Task distribution */
spawnPoint:
  switch (processNr) {
  case 0: {
    startSimulation();
  }
  case 1: {
    MainWindow mainWindow(common::REFERNCE_POINT);
    mainWindow.start();
    break;
  }
  case 2: {
    ControlWindow controlWindow(common::REFERNCE_POINT);
    controlWindow.start();
    break;
  }
  case 3: {
    RadarWindow radarWindow(common::REFERNCE_POINT);
    radarWindow.start();
    break;
  }
  case 4: {
    StatusWindow statusWindow(common::REFERNCE_POINT);
    statusWindow.start();
    break;
  }
  case 5: {
    LogPublisher logPublisher;
    logPublisher.start();
    break;
  }

  default: {
    std::cout << "ERROR TO MANY PROCESSES" << std::endl;
    break;
  }
  }

  waitpid(pid, NULL, 0); // kazdy czeka na siebie nawzajem
}

//// CORE

void SimulationCore::startSimulation() {
  LG_INF("SIMULATION CORE - STARTING SIMULATION");
  auto simulationRunningThread = getAndRunRunningSimulationInLoopThread();
  auto sendingDataThread = getAndRunSendingDataInLoopThread();
  auto updatingControlThread = getAndRunUpdatingControlInLoopThread();

  while (true) {
    if (Keyboard::isKeyPressed(Keyboard::Escape)) {
      break;
    }
  }

  LG_INF("SIMULATION CORE - ENDING SIMULATION");
}

std::thread SimulationCore::getAndRunSendingDataInLoopThread() {

  auto sendingDataInLoopLambda = [this]() {
    LG_INF("SIMULATION CORE - ENTERING LOOP - sending data lambda in "
           "parallel thread");

    Clock sendingVisualizationTimer;
    Clock sendingPositionTimer;
    Clock sendingRocketStatusTimer;

    while (true) {

      if (sendingVisualizationTimer.getElapsedTime() >
          _sendingVisualizationDataPeriod) {
        sendingVisualizationTimer.restart();
        sendVisualizationData();
      }

      if (sendingPositionTimer.getElapsedTime() >
          _sendingObjectsPositionPeriod) {
        sendingPositionTimer.restart();
        sendObjectsPosition();
      }

      if (sendingRocketStatusTimer.getElapsedTime() >
          _sendingRocketStatusPeriod) {
        sendingRocketStatusTimer.restart();
        sendRocketStatus();
      }
    }

    LG_INF("SIMULATION CORE - EXITING LOOP - sending data lambda in "
           "parallel thread");
  };

  std::thread sendingDataThread(sendingDataInLoopLambda);
  return sendingDataThread;
}

std::thread SimulationCore::getAndRunUpdatingControlInLoopThread() {
  auto updatingDataInLoopLambda = [this]() {
    LG_INF("SIMULATION CORE - ENTERING LOOP - receiving control data "
           "lambda in "
           "parallel thread");

    while (true) {
      updateCurrentThrustersControl();
    }

    LG_INF("SIMULATION CORE - EXITING LOOP - receiving control data lambda in "
           "parallel thread");
  };

  std::thread updatingControlThread(updatingDataInLoopLambda);
  return updatingControlThread;
}
std::thread SimulationCore::getAndRunRunningSimulationInLoopThread() {
  auto runningSimulationInLoopLambda = [this]() {
    LG_INF("SIMULATION CORE - ENTERING LOOP - simulation running lambda in "
           "parallel thread");
    Clock clock;
    while (true) {
      Time dt = clock.restart();
      float dtAsSeconds = dt.asSeconds();
      updateSystemState(dtAsSeconds);
    }
    LG_INF("SIMULATION CORE - EXITING LOOP - simulation running lambda in "
           "parallel thread");
  };

  std::thread simulationRunningThread(runningSimulationInLoopLambda);
  return simulationRunningThread;
}

void SimulationCore::updateSystemState(const float &dtAsSeconds) {
  updateLinearMotionPartOfSystemState(dtAsSeconds);
  updateRotationalMotionPartOfSystemState(dtAsSeconds);
}

void SimulationCore::updateLinearMotionPartOfSystemState(
    const float &dtAsSeconds) {

  Vector2f mainEngineForce{0, 0};

  if (_rocketParams.mainThrusterState == MainThrusterState::TURN_ON) {
    mainEngineForce.x =
        cos(getAngleInRadiansInClassicCoordinateSystem(_rocketParams.angle)) *
        commonConsts::MAIN_ENGINE_THRUST; // plus 90 due to rotate coordinate
                                          // system
    mainEngineForce.y =
        sin(getAngleInRadiansInClassicCoordinateSystem(_rocketParams.angle)) *
        commonConsts::MAIN_ENGINE_THRUST;
  }

  // a1 = F / m
  _rocketParams.acceleration.x = mainEngineForce.x / commonConsts::ROCKET_MASS;
  _rocketParams.acceleration.y = mainEngineForce.y / commonConsts::ROCKET_MASS;

  std::ostringstream ss;
  ss << "SIMCORE cos alfa "
     << cos(getAngleInRadiansInClassicCoordinateSystem(_rocketParams.angle))
     << " sin alfa"
     << sin(getAngleInRadiansInClassicCoordinateSystem(_rocketParams.angle))
     << "angle" << _rocketParams.angle << "-  acc y "
     << _rocketParams.acceleration.y << " mainEngineForce.y "
     << mainEngineForce.y << " - acc x " << _rocketParams.acceleration.x
     << " mainEngineForce.x " << mainEngineForce.x << std::endl;

  // debug
  //  if (_rocketParams.acceleration.x != 0) {
  //    LG_INF(ss.str());
  //  }
  //  v1 = a*t
  _rocketParams.velocity.x =
      _rocketParams.velocity.x + _rocketParams.acceleration.x * dtAsSeconds;
  _rocketParams.velocity.y =
      _rocketParams.velocity.y + _rocketParams.acceleration.y * dtAsSeconds;

  // x1 = x + v*t + 0.5*a*t^2
  _rocketParams.position.x =
      _rocketParams.position.x + (_rocketParams.velocity.x * dtAsSeconds) +
      (0.5 * _rocketParams.acceleration.x * dtAsSeconds * dtAsSeconds);
  _rocketParams.position.y =
      _rocketParams.position.y + (_rocketParams.velocity.y * dtAsSeconds) +
      (0.5 * _rocketParams.acceleration.y * dtAsSeconds * dtAsSeconds);
}

float SimulationCore::getAngleInRadiansInClassicCoordinateSystem(
    const float &angle) {
  float translatedAngle = angle + 90;
  float translatedAngleInRadians = translatedAngle * 2 * M_PI / 360;
  return translatedAngleInRadians;
}

void SimulationCore::updateRotationalMotionPartOfSystemState(
    const float &dtAsSeconds) {
  int sideThrusterForce{0};
  switch (_rocketParams.sideThrusterState) {

  case SideThrusterState::LEFT_ON:
    sideThrusterForce = -commonConsts::SIDE_THRUSTERS_THRUST;
    break;
  case SideThrusterState::RIGHT_ON:
    sideThrusterForce = commonConsts::SIDE_THRUSTERS_THRUST;
    break;
  case SideThrusterState::TURN_OFF:
    sideThrusterForce = 0;
    break;
  default:
    sideThrusterForce = 0;
    break;
  }

  float momentOfForce = sideThrusterForce * (0.5 * commonConsts::ROCKET_HEIGHT);

  // Equation down below are angular counterparts for linear motion

  // acceleration
  _rocketParams.angularAcceleration =
      momentOfForce / commonConsts::ROCKET_MOMENT_OF_INERTIA;

  // debug
  std::ostringstream ss;
  ss << "SIMCORE - calculated angular acceleration as "
     << _rocketParams.angularAcceleration << " moment of Force "
     << momentOfForce;

  // if (_rocketParams.angularAcceleration != 0) {
  //   LG_INF(ss.str());
  // }

  // velocity
  _rocketParams.angularVelocity =
      _rocketParams.angularVelocity +
      (_rocketParams.angularAcceleration * dtAsSeconds);

  // angle
  _rocketParams.angle =
      _rocketParams.angle + (_rocketParams.angularVelocity * dtAsSeconds) +
      (0.5 * _rocketParams.angularAcceleration * dtAsSeconds * dtAsSeconds);
}

//// COMMUNICATION SETUP
void SimulationCore::openQueues() {

  // ROCKET VISUALIZATION
  if ((comm::rocketVisualizationQueue =
           mq_open(comm::ROCKET_VISUALIZATION_QUEUE_FILE, O_CREAT | O_RDWR,
                   0644, &comm::rocketVisualizationQueueAttr)) == -1) {
    printf(" >> ERROR - SIMULATION CORE - FAILED TO OPEN ROCKET VISUALIZATION "
           "QUEUE %s\n",
           strerror(errno));
    return;
  }

  // CONTROL
  if ((comm::thrustersControlQueue =
           mq_open(comm::THRUSTERS_CONTROL_QUEUE_FILE, O_CREAT | O_RDWR, 0644,
                   &comm::thrustersControlQueueAttr)) == -1) {
    printf(" >> ERROR - SIMULATION CORE - FAILED TO OPEN CONTROL QUEUE %s\n",
           strerror(errno));
    return;
  }

  // POSITION
  if ((comm::objectsPositionQueue =
           mq_open(comm::OBJECTS_POSITION_QUEUE_FILE, O_CREAT | O_RDWR, 0644,
                   &comm::objectsPositionQueueAttr)) == -1) {
    printf(" >> ERROR - SIMULATION CORE - FAILED TO OPEN POSITION QUEUE %s\n",
           strerror(errno));
    return;
  }

  // ROCKET STATUS
  if ((comm::rocketStatusQueue =
           mq_open(comm::ROCKET_STATUS_QUEUE_FILE, O_CREAT | O_RDWR, 0644,
                   &comm::rocketStatusQueueAttr)) == -1) {
    printf(
        " >> ERROR - SIMULATION CORE - FAILED TO OPEN ROCKET STATUS QUEUE %s\n",
        strerror(errno));
    return;
  }
}

void SimulationCore::closeQueues() {
  if (mq_close(comm::rocketVisualizationQueue) == -1) {
    printf(" >> ERROR - SIMULATION CORE - FAILED TO CLOSE ROCKET VISUALIZATION "
           "QUEUE %s\n",
           strerror(errno));
  }
  if (mq_close(comm::thrustersControlQueue) == -1) {
    printf(" >> ERROR - SIMULATION CORE - FAILED TO CLOSE CONTROL QUEUE %s\n",
           strerror(errno));
  }
  if (mq_close(comm::objectsPositionQueue) == -1) {
    printf(" >> ERROR - SIMULATION CORE - FAILED TO CLOSE POSITION QUEUE %s\n",
           strerror(errno));
  }
  if (mq_close(comm::rocketStatusQueue) == -1) {
    printf(" >> ERROR - SIMULATION CORE - FAILED TO CLOSE ROCKET STATUS QUEUE "
           "%s\n",
           strerror(errno));
  }
}

//// COMMUNICATION

void SimulationCore::sendVisualizationData() {
  // _rocketParams.angle = _rocketParams.angle + 0.36;
  // _rocketParams.angle = 90;
  msg::RocketVisualizationContainerMsg visualizationContainerMsg{
      .velocity = _rocketParams.velocity,
      .angle = _rocketParams.angle,
      .mainThrusterState = _rocketParams.mainThrusterState,
      .sideThrusterState = _rocketParams.sideThrusterState,
      .destinationPosition = _destinationParams.position,
      .rockePosition = _rocketParams.position};

  if (mq_send(comm::rocketVisualizationQueue,
              (char *)&visualizationContainerMsg,
              sizeof(msg::RocketVisualizationContainerMsg), 0) == -1) {
    LG_ERR("SIMULATION CORE - FAILED TO SEND VISUALIZATION DATA - " +
           std::string(strerror(errno)));
  } else {
    // LG_INF("SIMULATION CORE - SENT UPDATE ON VISUALIZATION DATA");
  }
}

void SimulationCore::updateCurrentThrustersControl() {
  msg::ThrustersStateMsg thrusterStateMsg;

  if (mq_receive(comm::thrustersControlQueue, (char *)&thrusterStateMsg,
                 sizeof(msg::ThrustersStateMsg), NULL) == -1) {
    LG_ERR("SIMULATION CORE - FAILED TO RECEIVE CONTROL - " +
           std::string(strerror(errno)));
  } else {
    // LG_INF("SIMULATION CORE - RECEIVED UPDATE OF THRUSTERS CONTROL");
    // LogReceivedControl(thrusterStateMsg);
  }

  _rocketParams.mainThrusterState = thrusterStateMsg.mainThrusterState;
  _rocketParams.sideThrusterState = thrusterStateMsg.sideThrusterState;
}

void SimulationCore::sendObjectsPosition() {

  // TEST
  // _rocketParams.position.x = _rocketParams.position.x + 1000;
  // _rocketParams.position.y = _rocketParams.position.y + 1000;

  msg::ObjectsPositionMsg objectsPositionMsg{
      .destinationPosition = _destinationParams.position,
      .rockePosition = _rocketParams.position};

  if (mq_send(comm::objectsPositionQueue, (char *)&objectsPositionMsg,
              sizeof(msg::ObjectsPositionMsg), 0) == -1) {
    LG_ERR("SIMULATION CORE - FAILED TO SEND OBJECTS POSITION - " +
           std::string(strerror(errno)));
  }

  // LG_INF("SIMULATION CORE - SENT UPDATE ON OBJECTS POSITION");
}

void SimulationCore::sendRocketStatus() {
  _rocketParams.oxygen = _rocketParams.oxygen + 36;
  msg::RocketStatusMsg rocketStatusMsg{.oxygen = _rocketParams.oxygen,
                                       .fuel = _rocketParams.fuel,
                                       .velocity = _rocketParams.velocity,
                                       .angle = _rocketParams.angle};

  if (mq_send(comm::rocketStatusQueue, (char *)&rocketStatusMsg,
              sizeof(msg::RocketStatusMsg), 0) == -1) {
    LG_ERR("SIMULATION CORE - FAILED TO SEND ROCKET STATUS POSITION - " +
           std::string(strerror(errno)));
  } else {
    // LG_INF("SIMULATION CORE - SENT UPDATE ON ROCKET STATUS");
  }
}

//// LOG HELPERS
void SimulationCore::LogReceivedControl(
    const msg::ThrustersStateMsg &thrusterStateMsg) {
  switch (thrusterStateMsg.mainThrusterState) {
  case common::MainThrusterState::TURN_ON:
    LG_INF("SIMULATION CORE - RECEIVED CONTROL - MAIN ENGINE IS ON");
    break;

  case common::MainThrusterState::TURN_OFF:
    LG_INF("SIMULATION CORE - RECEIVED CONTROL - MAIN ENGINE IS OFF");
    break;

  default:
    LG_INF("SIMULATION CORE - DID NOT RECEIVE CONTROL - MAIN ENGINE");
    break;
  }

  switch (thrusterStateMsg.sideThrusterState) {
  case common::SideThrusterState::LEFT_ON:
    LG_INF("SIMULATION CORE - RECEIVED CONTROL - LEFT THRUSTER IS ON");
    break;

  case common::SideThrusterState::RIGHT_ON:
    LG_INF("SIMULATION CORE - RECEIVED CONTROL - RIGHT THRUSTER IS ON");
    break;

  case common::SideThrusterState::TURN_OFF:
    LG_INF("SIMULATION CORE - RECEIVED CONTROL - SIDE THRUSTERS ARE OFF");
    break;

  default:
    LG_INF("SIMULATION CORE - DID NOT RECEIVE CONTROL - SIDE ENGINES");
    break;
  }
}

////DEBUG
void SimulationCore::printQueueInfo(const mqd_t &queueFile) {
  struct mq_attr attr;
  mq_getattr(queueFile, &attr);
  printf("Maximum # of messages on queue:   %ld\n", attr.mq_maxmsg);
  printf("Maximum message size:             %ld\n", attr.mq_msgsize);
}
