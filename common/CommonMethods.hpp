#pragma once
#include <SFML/Graphics.hpp>
#include <math.h>

namespace common {

static float getDistance(const sf::Vector2f &obj1, const sf::Vector2f &obj2) {
  float x = obj1.x - obj2.x;
  float y = obj1.y - obj2.y;
  float distance = sqrt(x * x + y * y);
  return distance;
}
} // namespace common