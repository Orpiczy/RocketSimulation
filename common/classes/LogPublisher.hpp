#pragma once
#include "../../common/CommunicationInfo.hpp"
#include <SFML/Graphics.hpp>
#include <errno.h>
#include <error.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <thread>
#include <unistd.h>

class LogPublisher {
public:
  void start();
  void stopPublishingAfterDelay();

private:
  bool isLogPublisherNeeded{true};
  bool isLoggerClosing{false};
  uint8_t stopDelayInSec{2};
};
