
#include "SimpleLogger.hpp"
#include "../CommunicationInfo.hpp"
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <sstream>

void SimpleLogger::LG_INF(const std::string &info) const {
  if (isLogInfoEnable_) {
    std::stringstream log;
    log << infoString << info << std::endl;
    logString(log.str());
  }
}

void SimpleLogger::LG_INF(const std::string &info, const int &intInfo) const {
  if (isLogInfoEnable_) {
    std::stringstream log;
    log << infoString << info << intInfo << std::endl;
    logString(log.str());
  }
}

void SimpleLogger::LG_ERR(const std::string &info) const {
  if (isLogErrorEnable_ and not info.empty()) {
    std::stringstream log;
    log << errorString << info << std::endl;
    logString(log.str());
  }
}

void SimpleLogger::LG_ERR(const std::string &info, const int &intInfo) const {
  if (isLogErrorEnable_ and not info.empty()) {
    std::stringstream log;
    log << errorString << info << intInfo << std::endl;
    logString(log.str());
  }
}

// HELPER
void SimpleLogger::logString(const std::string &log) const {

  int fileDescriptor;
  char buffer[log.length()];

  strcpy(buffer, log.c_str());

  if (isRealTime) {

    // Remove newline from string
    int length = strlen(buffer);
    if (buffer[length - 1] == '\n')
      buffer[length - 1] = '\0';

    // Open FIFO file
    if ((fileDescriptor = open(comm::VIRTUAL_FILE_FOR_LOGGING, O_WRONLY)) ==
        -1) {
      fprintf(stderr, "Cannot open FIFO.\n");
      return;
    }

    // Write a message to FIFO
    if (write(fileDescriptor, buffer, strlen(buffer)) != strlen(buffer)) {
      fprintf(stderr, "Cannot write to FIFO.\n");
      return;
    }

    // Close FIFO
    if (close(fileDescriptor) == -1) {
      fprintf(stderr, "Cannot close FIFO.\n");
      return;
    }
  } else {
    std::cout << log;
  }
}
