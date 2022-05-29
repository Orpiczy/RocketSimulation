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
  while (true) {
    // getCurrentThrustersControl();
    // sendVisualizationData();
    // sendRocketStatus();
    sendObjectsPosition();
  }
  LG_INF("SIMULATION CORE - ENDING SIMULATION");
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
    LG_INF("SIMULATION CORE - SENT UPDATE ON VISUALIZATION DATA");
  }
}

void SimulationCore::getCurrentThrustersControl() {
  msg::ThrustersStateMsg thrusterStateMsg;

  if (mq_receive(comm::thrustersControlQueue, (char *)&thrusterStateMsg,
                 sizeof(msg::ThrustersStateMsg), NULL) == -1) {
    LG_ERR("SIMULATION CORE - FAILED TO RECEIVE CONTROL - " +
           std::string(strerror(errno)));
  } else {
    LG_INF("SIMULATION CORE - RECEIVED UPDATE OF THRUSTERS CONTROL");
    LogReceivedControl(thrusterStateMsg);
  }
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
  _rocketParams.oxygen = _rocketParams.oxygen + 0.01;
  msg::RocketStatusMsg rocketStatusMsg{.oxygen = _rocketParams.oxygen,
                                       .fuel = _rocketParams.fuel,
                                       .velocity = _rocketParams.velocity,
                                       .angle = _rocketParams.angle};

  if (mq_send(comm::rocketStatusQueue, (char *)&rocketStatusMsg,
              sizeof(msg::RocketStatusMsg), 0) == -1) {
    LG_ERR("SIMULATION CORE - FAILED TO SEND ROCKET STATUS POSITION - " +
           std::string(strerror(errno)));
  }

  LG_INF("SIMULATION CORE - SENT UPDATE ON ROCKET STATUS");
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
