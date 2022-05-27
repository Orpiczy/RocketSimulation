#pragma once
#include "CommonTypes.hpp"
#include <SFML/Graphics.hpp>

using namespace common;

namespace msg {
struct ThrustersStateMsg {
  MainThrusterState mainThrusterState{MainThrusterState::TURN_OFF};
  SideThrusterState sideThrusterState{SideThrusterState::TURN_OFF};
};

struct ObjectsPositionMsg {
  sf::Vector2f destinationPosition{0, 0};
  sf::Vector2f rockePosition{0, 0};
};

struct RocketStatusMsg {
  StorageTank oxygen{0};
  StorageTank fuel{0};
  sf::Vector2f velocity{0, 0};
  float angle{0};
};

} // namespace msg