#pragma once
#include "../../common/PipeInfo.hpp"
#include <SFML/Graphics.hpp>
#include <unistd.h>

class LogPublisher {
public:
  LogPublisher() {  }

  void start() {
    if (pipe(commonPipeInfo::LOG_PIPE_ENDS_DESCRIPTOR) == -1) {
      fprintf(stderr, "Cannot create pipe.\n");
      return;
    }
    // closing  writing descriptor
    close(commonPipeInfo::LOG_PIPE_ENDS_DESCRIPTOR[1]);
    while (true) {
      char message_str[100];
      read(commonPipeInfo::LOG_PIPE_ENDS_DESCRIPTOR[0], message_str, 100);
      printf("PIPE MESSAGE: %s\n", message_str);
    }
    // closing reading descriptor
    close(commonPipeInfo::LOG_PIPE_ENDS_DESCRIPTOR[1]);
  }

private:
};