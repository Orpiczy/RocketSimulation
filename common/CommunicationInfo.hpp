#pragma once

#include "MessageTypes.hpp"
#include <iostream>
#include <mqueue.h>

namespace comm {
/*FIFO*/
static const char *VIRTUAL_FILE_FOR_LOGGING{"logging_info_fifo"};

/* MSG QUEUES */

// CMD -> CORE
static mqd_t thrustersControlQueue;
static struct mq_attr thrustersControlQueueAttr {
  .mq_maxmsg = 10, .mq_msgsize = sizeof(msg::ThrustersStateMsg)
};
static const char *THRUSTERS_CONTROL_QUEUE_FILE{"/thrusters_control_queue"};

// CORE -> RADAR
static mqd_t objectsPositionQueue;
static struct mq_attr objectsPositionQueueAttr {
  .mq_maxmsg = 10, .mq_msgsize = sizeof(msg::ObjectsPositionMsg)
};
static const char *OBJECTS_POSITION_QUEUE_FILE{"/objects_position_queue"};

// CORE -> STATUS
static mqd_t rocketStatusQueue;
static struct mq_attr rocketStatusQueueAttr {
  .mq_maxmsg = 10, .mq_msgsize = sizeof(msg::RocketStatusMsg)
};
static const char *ROCKET_STATUS_QUEUE_FILE{"/rocket_status_queue"};

} // namespace comm