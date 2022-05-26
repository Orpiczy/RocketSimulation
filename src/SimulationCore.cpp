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
#include "../common/MessageTypes.hpp"
#include <mqueue.h>

SimulationCore::SimulationCore(bool isLogInfoEnable, bool isLogErrorEnable)
    : SimpleLogger(isLogInfoEnable, isLogErrorEnable) {

  /* Create Message Queue */
  if ((comm::thrustersControlQueue =
           mq_open(comm::THRUSTERS_CONTROL_QUEUE_FILE, O_CREAT | O_RDWR, 0644,
                   &comm::thrustersControlQueueAttr)) == -1) {
    printf(" >> ERROR - Simulation Core - FAILED TO OPEN THE QUEUE %s\n",
           strerror(errno));
    return;
  }
}

SimulationCore::~SimulationCore() {
  /* Close Message Queue */
  mq_close(comm::thrustersControlQueue);
}
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

// CORE
void SimulationCore::startSimulation() {
  LG_INF("SIMULATION CORE - STARTING SIMULATION");
  while (true) {
    updateThrustersInformation();
  }
  LG_INF("SIMULATION CORE - ENDING SIMULATION");
}
// COMMUNICATION
void SimulationCore::updateThrustersInformation() {
  msg::ThrustersStateMsg thrusterStateMsg;
  mq_receive(comm::thrustersControlQueue, (char *)&thrusterStateMsg,
             sizeof(msg::ThrustersStateMsg), NULL);
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
