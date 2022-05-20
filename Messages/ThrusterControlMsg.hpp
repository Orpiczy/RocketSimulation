#pragma once
#include "../common/CommonTypes.hpp"

using namespace common;

namespace msg {
struct ThrusterControlMsg {
  MainThrusterState mainThrusterState{MainThrusterState::TURN_OFF};
  SideThrusterState sideThrusterState{SideThrusterState::TURN_OFF};
};

} // namespace msg