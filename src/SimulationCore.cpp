#include "../include/SimulationCore.hpp"
#include "../common/classes/LogPublisher.hpp"
#include "../include/ControlWindow.hpp"
#include "../include/MainWindow.hpp"
#include "../include/RadarWindow.hpp"
#include "../include/StatusWindow.hpp"

// multiprocess
#include <sys/wait.h>
#include <unistd.h>

void SimulationCore::run() {

  pid_t pid;
  uint8_t childProcessNr{1};
  uint8_t nrOfNeededChildProcesses{5};

  /* Process spawning */
  while (childProcessNr < nrOfNeededChildProcesses) {
    pid = fork();
    switch (pid) {
    case 0:
      childProcessNr++;
      continue;
    case -1:
      printf("Error while creating new process");
      return;

    default:
      printf("Successful process spawn of process, pid = %d, my parent pid =  "
             "%d, childProcessNumber %d\n",
             getpid(), getppid(), childProcessNr);
      goto spawnPoint;
    }
  }

  /* Task distribution */
spawnPoint:
  switch (childProcessNr) {
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

  waitpid(pid, NULL, 0); // waits for logger
}

// COMMUNICATION
void SimulationCore::updateThrusterState() {}
