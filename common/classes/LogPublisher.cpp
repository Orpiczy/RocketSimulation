#include "LogPublisher.hpp"

void LogPublisher::start() {
  int fileDescriptor, bytesRead;
  isLogPublisherNeeded = true;
  char buffer[512];

  // Create FIFO
  if ((mkfifo(comm::VIRTUAL_FILE_FOR_LOGGING, 0664) == -1) &&
      (errno != EEXIST)) {
    fprintf(stderr, " >> ERROR  - LOG PUBLISHER - Cannot create FIFO.\n");
    return;
  }

  // Open FIFO file
  if ((fileDescriptor = open(comm::VIRTUAL_FILE_FOR_LOGGING, O_RDONLY)) == -1) {
    fprintf(stderr, " >> ERROR  - LOG PUBLISHER - Cannot open FIFO.\n");
    return;
  }

  while (isLogPublisherNeeded) {

    // Clear data bufferer
    memset(buffer, '\0', sizeof(buffer));

    // Read data from FIFO
    if ((bytesRead = read(fileDescriptor, buffer, sizeof(buffer))) == -1) {
      fprintf(stderr,
              " >> ERROR  - LOG PUBLISHER - Something is wrong with FIFO.\n");
      return;
    }

    // If there is no data just continue
    if (bytesRead == 0) {
      // TODO: LOGGER CLOSING
      //  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
      //    break;
      //  }
      continue;
    }

    // If there is message print it
    if (bytesRead > 0) {
      printf("%s", buffer);
      fflush(stdout);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) &&
        not isLoggerClosing) {
      isLoggerClosing = true;
      stopPublishingAfterDelay();
    }
  }
  printf(" >> INFO  - LOG PUBLISHER - EXITING LOOP\n");
}

void LogPublisher::stopPublishingAfterDelay() {

  auto stopDelayingLambda = [&]() {
    printf(" >> INFO  - LOG PUBLISHER - ESC WAS PRESSED, COLLECTING LAST "
           "LOGS - PROCESS TERMINATION IN %d s\n",
           stopDelayInSec);
    sleep(stopDelayInSec);
    isLogPublisherNeeded = false;
  };

  /*YOU CAN NOT USE PTHREAD - DOESNT SUPPORT LAMBDA, ONLY SIGNLE VALUE ARG,
   MAYBE STRUCTURE A LOT OF WORK*/
  std::thread stopLoopThread(stopDelayingLambda);
  stopLoopThread.detach();

  /*bez tego gdy wyjdziemy po za scope threadu obj, a jest on joinable to
   * terminuje program*/
}
