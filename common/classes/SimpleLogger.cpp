
#include "SimpleLogger.hpp"
#include <cstring>
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
  if (isRealTime) {

    // opening pipe
    if (pipe(commonPipeInfo::LOG_PIPE_ENDS_DESCRIPTOR) == -1) {
      fprintf(stderr, "Cannot create pipe.\n");
      return;
    }

    // close reading descriptor
    // close(commonPipeInfo::LOG_PIPE_ENDS_DESCRIPTOR[0]);

    // writing
    write(commonPipeInfo::LOG_PIPE_ENDS_DESCRIPTOR[1], log.c_str(),
          strlen(log.c_str()) + 1);

    // close writing
    // close(commonPipeInfo::LOG_PIPE_ENDS_DESCRIPTOR[1]);
  } else {
    std::cout << log;
  }
}
