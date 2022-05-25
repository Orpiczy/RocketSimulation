#pragma once
#include "../../common/CommunicationInfo.hpp"
#include <SFML/Graphics.hpp>
#include <errno.h>
#include <error.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

class LogPublisher {
public:
  LogPublisher() {}

  void start() {

    int fileDescriptor, bytesRead;
    char buffer[512];

    // Create FIFO
    if ((mkfifo(comm::VIRTUAL_FILE_FOR_LOGGING, 0664) == -1) &&
        (errno != EEXIST)) {
      fprintf(stderr, "Cannot create FIFO.\n");
      return;
    }

    // Open FIFO file
    if ((fileDescriptor = open(comm::VIRTUAL_FILE_FOR_LOGGING, O_RDONLY)) ==
        -1) {
      fprintf(stderr, "Cannot open FIFO.\n");
      return;
    }

    while (true) {

      // Clear data bufferer
      memset(buffer, '\0', sizeof(buffer));

      // Read data from FIFO
      if ((bytesRead = read(fileDescriptor, buffer, sizeof(buffer))) == -1) {
        fprintf(stderr, "Something is wrong with FIFO.\n");
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
        printf("%s\n", buffer);
        fflush(stdout);
      }
    }
  }
};