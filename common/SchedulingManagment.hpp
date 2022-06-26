#pragma once

#include <cmath>
#include <errno.h>
#include <optional>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

namespace schedulingManagment {

enum class SchedulingPriority { min, medium, max };
static bool wasWarningDisplayed{false};

//// HELPERS

// [[deprecated]] static int getCurrentThreadSchedulingPolicy() {
//   pid_t tid;
//   tid = syscall(SYS_gettid);
//   int threadPolicy = sched_getscheduler(tid);
//   return threadPolicy;
// }

// [[deprecated]] static bool isDemandedPolicyActive(const int &demandedPolicy)
// {
//   bool isDemandedPolicyActive =
//       getCurrentThreadSchedulingPolicy() == demandedPolicy;

//   // optional logging
//   if (not isDemandedPolicyActive) {
//     printf(" >> INFO - DemandedPolicyActive IS NOT ACTIVE - are you sure that
//     "
//            "you are running code as root? \n");
//   }

//   return isDemandedPolicyActive;
// }

static std::optional<struct sched_param>
getSchedParamWithDemandedPriority(const int policy,
                                  const SchedulingPriority &priority) {
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
    printf(" >> ERROR - SCHEDULING MANAGMENT - "
           "setFifoSchedulingPolicyAndPriorityUnknown - Unknown priority");
    return std::nullopt;
  }
  return param;
}

static std::pair<bool, bool>
areDemandedPolicyAndPriorityActive(const int demandedPolicy,
                                   const SchedulingPriority &demandedPriority) {
  struct sched_param currentParam;
  int currentPolicy;

  if (pthread_getschedparam(pthread_self(), &currentPolicy, &currentParam) !=
      0) {
    printf(
        " >> ERROR - isDemandedPriorityActive - cannot acceess params - %s\n",
        strerror(errno));
  }

  auto demandedParam =
      getSchedParamWithDemandedPriority(currentPolicy, demandedPriority);
  if (not demandedParam.has_value()) {
    printf(
        " >> ERROR - isDemandedPolicyAndPriorityActive - cannot access sched "
        "params");
    return {false, false};
  }

  return {currentPolicy == demandedPolicy,
          currentParam.sched_priority == (*demandedParam).sched_priority};
}

//// LOGGING
static void logDemandedAndCurrentPolicyAndPriorityOnFailure(
    const std::string caller, const int demandedPolicy,
    const SchedulingPriority &demandedPriority) {

  auto policyAndPriorityComparison =
      areDemandedPolicyAndPriorityActive(demandedPolicy, demandedPriority);

  if (not policyAndPriorityComparison.first or
      not policyAndPriorityComparison.second) {

    if (not wasWarningDisplayed) {
      printf(
          " >> ERROR - SCHEDULING MANAGMENT <- %s  - isDemandedPolicyActive = "
          "%d, "
          "isDemandedPriorityActive = %d\n",
          caller.c_str(), policyAndPriorityComparison.first,
          policyAndPriorityComparison.second);
      printf("\n!!! REMEMBER !!!\nYou can manipulate scheduling type and "
             "priority  only as root\n\n");
      wasWarningDisplayed = true;
    }
  }
}

//// CORE

static bool setSchedulingPolicyAndPriority(int policy,
                                           SchedulingPriority priority) {

  auto param = getSchedParamWithDemandedPriority(policy, priority);
  if (not param.has_value()) {
    printf(" >> ERROR - SCHEDULING MANAGMENT - failure to set priority correct "
           "priority in structure\n");
    return false;
  }

  int result = pthread_setschedparam(pthread_self(), policy, &(*param));

  if (result != 0) {
    if (not wasWarningDisplayed) {
      printf(
          " >> ERROR - SCHEDULING MANAGMENT - setting scheduling policy FAILED "
          "- %s\n",
          strerror(errno));
    }
    return false;
  }

  return true;
}

static bool setAndLogSchedulingPolicyAndPriority(
    const std::string caller, const int demandedPolicy,
    const SchedulingPriority &demandedPriority) {
  bool result =
      setSchedulingPolicyAndPriority(demandedPolicy, demandedPriority);
  logDemandedAndCurrentPolicyAndPriorityOnFailure(caller, demandedPolicy,
                                                  demandedPriority);
  return result;
}

}; // namespace schedulingManagment