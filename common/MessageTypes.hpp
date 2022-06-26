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
  RocketAngle angle{0};
};

struct RocketVisualizationContainerMsg {
  sf::Vector2f velocity{0, 0};
  RocketAngle angle{0};
  MainThrusterState mainThrusterState{MainThrusterState::TURN_OFF};
  SideThrusterState sideThrusterState{SideThrusterState::TURN_OFF};
  sf::Vector2f destinationPosition{0, 0};
  sf::Vector2f rockePosition{0, 0};
  StorageTank oxygen{0};
};

struct SimulationStatusMsg {
  SimulationStatus status{SimulationStatus::READY_TO_START};
};

} // namespace msg