#pragma once

#include "MessageTypes.hpp"
#include <iostream>
#include <mqueue.h>
#include <unistd.h>

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
static const long cmdMsgPriority = _SC_MQ_PRIO_MAX;

// CORE -> RADAR
static mqd_t objectsPositionQueue;
static struct mq_attr objectsPositionQueueAttr {
  .mq_maxmsg = 10, .mq_msgsize = sizeof(msg::ObjectsPositionMsg)
};
static const char *OBJECTS_POSITION_QUEUE_FILE{"/objects_position_queue"};
static const long radarMsgPriority = _SC_MQ_PRIO_MAX;

// CORE -> STATUS
static mqd_t rocketStatusQueue;
static struct mq_attr rocketStatusQueueAttr {
  .mq_maxmsg = 10, .mq_msgsize = sizeof(msg::RocketStatusMsg)
};
static const char *ROCKET_STATUS_QUEUE_FILE{"/rocket_status_queue"};
static const long statusMsgPriority = _SC_MQ_PRIO_MAX;

// CORE -> MAIN WINDOW - SIMULATION STATE
static mqd_t simulationStatusQueue;
static struct mq_attr simulationStatusQueueAttr {
  .mq_maxmsg = 10, .mq_msgsize = sizeof(msg::SimulationStatusMsg)
};

static const char *SIMULATION_STATUS_QUEUE_FILE{"/simulation_status_queue"};
static const long simulationStatusMsgPriority = _SC_MQ_PRIO_MAX;

// CORE -> MAIN WINDOW
static mqd_t rocketVisualizationQueue;
static struct mq_attr rocketVisualizationQueueAttr {
  .mq_maxmsg = 10, .mq_msgsize = sizeof(msg::RocketVisualizationContainerMsg)
};
static const char *ROCKET_VISUALIZATION_QUEUE_FILE{
    "/rocket_visualization_queue"};
static const long visualizationMsgPriority = 0; // lowest possible

} // namespace comm