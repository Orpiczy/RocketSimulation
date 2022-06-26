#pragma once
#include "SchedulingManagment.hpp"

using SchedulingPriority = schedulingManagment::SchedulingPriority;
namespace schedulingInfo {

// I
const static SchedulingPriority initialPriority = SchedulingPriority::min;

// CORE
const static SchedulingPriority simulationPriority = SchedulingPriority::min;
const static SchedulingPriority sendingDataFromCorePriority =
    SchedulingPriority::min;
const static SchedulingPriority updatingCmdInCorePriority =
    SchedulingPriority::medium;

// CONTROL WINDOW
const static SchedulingPriority sendingCmdPriority = SchedulingPriority::medium;

// OTHERS
const static SchedulingPriority updatingVisDataPriority =
    SchedulingPriority::max;

} // namespace schedulingInfo