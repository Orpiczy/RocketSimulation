#pragma once
#include <SFML/Graphics.hpp>
#include <math.h>

namespace common {

enum class MainThrusterState { TURN_ON, TURN_OFF, DAMAGED, UNKNOWN };

enum class SideThrusterState { LEFT_ON, RIGHT_ON, TURN_OFF, DAMAGED, UNKNOWN };

enum class SimulationStatus { READY_TO_START, ACTIVE, SUCCESS, FAILURE };

class StorageTank {
public:
  StorageTank(const float initialValue) {
    value = getAcceptableValue(initialValue);
  }

  operator float() const { return value; }

  float operator=(float newValue) {
    return value = getAcceptableValue(newValue);
  }

  const float maxValue{100.0};
  const float minValue{0.0};

private:
  float value;

  const float getAcceptableValue(const float &newValue) const {
    bool isTooBig = newValue > maxValue;
    bool isTooSmall = newValue < minValue;

    if (isTooBig) {
      return maxValue;
    }

    if (isTooSmall) {
      return minValue;
    }

    return newValue;
  }
};

class RocketAngle {
public:
  RocketAngle(const float initialValue) {
    value = getAcceptableValue(initialValue);
  }

  operator float() const { return value; }

  float operator=(float newValue) {
    return value = getAcceptableValue(newValue);
  }

  const float maxValue{360.0};
  const float minValue{0.0};

private:
  float value;
  const float getAcceptableValue(const float &newValue) const {
    bool isTooBig = newValue > maxValue;
    bool isTooSmall = newValue < minValue;
    if (isTooBig) {
      return fmod(newValue, maxValue);
    }

    if (isTooSmall) {
      return maxValue - fmod(newValue, maxValue);
    }

    return newValue;
  }
};

struct RocketParams {
  StorageTank oxygen{0};
  StorageTank fuel{0};
  sf::Vector2f position{0, 0};
  sf::Vector2f velocity{0, 0};
  sf::Vector2f acceleration{0, 0};
  RocketAngle angle{0};
  float angularVelocity{0};
  float angularAcceleration{0};
  MainThrusterState mainThrusterState{MainThrusterState::TURN_OFF};
  SideThrusterState sideThrusterState{SideThrusterState::TURN_OFF};
};

struct DestinationParams {
  sf::Vector2f position{0, 0};
  sf::Vector2f velocity{0, 0};
};

} // namespace common
