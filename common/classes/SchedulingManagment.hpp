#include "SimpleLogger.hpp"

namespace schedulingManagment {

enum class SchedulingPriority { min, medium, max };

static bool setSchedulingPolicyAndPriority(int policy,
                                           SchedulingPriority priority) {

  /* Structure of other thread parameters */
  struct sched_param param;
  switch (priority) {

  case SchedulingPriority::min:
    param.sched_priority = sched_get_priority_min(policy);
    break;

  case SchedulingPriority::medium: {
    auto min_priority = sched_get_priority_min(policy);
    auto max_priority = sched_get_priority_max(policy);
    param.sched_priority =
        static_cast<int>(abs((max_priority - min_priority) / 2));
  } break;

  case SchedulingPriority::max:
    param.sched_priority = sched_get_priority_max(policy);
    break;

  default:
    printf(">> ERROR - schedulingManagment - "
           "setFifoSchedulingPolicyAndPriorityUnknown - Unknown priority");
    return false;
  }

  /* Read modify and set new thread priority */
  int result = pthread_setschedparam(pthread_self(), policy, &param);
  if (result != 0) {
    printf(">> ERROR - SchedulingManagment - "
           "setFifoSchedulingPolicyAndMinPriority FAILED");
    return false;
  }
  return true;
}

}; // namespace schedulingManagment