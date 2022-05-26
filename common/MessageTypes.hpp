#pragma once
#include "CommonTypes.hpp"

using namespace common;

namespace msg {
struct ThrustersStateMsg {
  MainThrusterState mainThrusterState{MainThrusterState::TURN_OFF};
  SideThrusterState sideThrusterState{SideThrusterState::TURN_OFF};
};

} // namespace msg