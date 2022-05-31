#include "../include/StarDustContainer.hpp"
#include "../include/StarDust.hpp"
#include <random>

namespace spriteObjects {
template <class elementType, int nrOfElements>
StarDustContainer<elementType, nrOfElements>::StarDustContainer(
    bool isLogInfoEnable, bool isLogErrorEnable)
    : SimpleLogger(isLogInfoEnable, isLogErrorEnable) {
  initialSpawn();
}

template <class elementType, int nrOfElements>
void StarDustContainer<elementType, nrOfElements>::initialSpawn() {

  int nrOfElementsSpawnAtInit =
      floor(nrOfElements * _percentageOfObjectsSpawnAtInit);

  for (int i = 0; i < nrOfElementsSpawnAtInit; i++) {
    float x = getRandomFloatInRange(-_radiusOfPasivity, _radiusOfPasivity);
    float ySpawnRadius = sqrt(_radiusOfPasivity * _radiusOfPasivity - x * x);
    float y = getRandomFloatInRange(-ySpawnRadius, ySpawnRadius);
    _elementsContainer.at(i).setPosition({x, y});
  }
}

template <class elementType, int nrOfElements>
void StarDustContainer<elementType, nrOfElements>::inFlightRespawn(
    const sf::Vector2f &velocity) {
  for (int i = 0; i < nrOfElements; i++) {
    if (_elementsContainer.at(i).getDistance() > _radiusOfActivity) {
      _elementsContainer.at(i).setPosition(getSpawnPoint(velocity));
    }
  }
}

template <class elementType, int nrOfElements>
sf::Vector2f StarDustContainer<elementType, nrOfElements>::getSpawnPoint(
    const sf::Vector2f &velocity) {

  float x = getRandomFloatInRange(0, _radiusOfActivity);

  float yMinSpawnRadius = sqrt(_radiusOfPasivity * _radiusOfPasivity - x * x);
  float yMaxSpawnRadius = sqrt(_radiusOfActivity * _radiusOfActivity - x * x);
  float y = getRandomFloatInRange(yMinSpawnRadius, yMaxSpawnRadius);

  sf::Vector2f positionInFirstQuadrant{x, y};
  int designatedQuadrant = getQuadrant(velocity);

  sf::Vector2f finalPosition = movePointToDesignatedQuadrant(
      positionInFirstQuadrant, designatedQuadrant);
  return finalPosition;
}

template <class elementType, int nrOfElements>
int StarDustContainer<elementType, nrOfElements>::getQuadrant(
    const sf::Vector2f &velocity) {
  int initialQuadrant = getRandomIntInRange(1, 4);
  int finalQuadrant{initialQuadrant};

  if (velocity.y >= 0) // moving up
  {
    if (velocity.x >= 0) {
      // we are going into 1 quadrant -> 3 quadrant is undesirable
      if (initialQuadrant == 3) {
        finalQuadrant = 1;
      }
    } else {
      if (initialQuadrant == 4) {
        // we are going into 2 quadrant -> 4 quadrant is undesirable
        finalQuadrant = 2;
      }
    }
  } else // moving down
  {
    if (velocity.x >= 0) {
      // we are going into 3 quadrant -> 1 quadrant is undesirable
      if (initialQuadrant == 1) {
        finalQuadrant = 3;
      }
    } else {
      if (initialQuadrant == 1) {
        // we are going into 4 quadrant -> 1 quadrant is undesirable
        finalQuadrant = 4;
      }
    }
  }
  return finalQuadrant;
}

template <class elementType, int nrOfElements>
sf::Vector2f
StarDustContainer<elementType, nrOfElements>::movePointToDesignatedQuadrant(
    const sf::Vector2f &point, const int &quadrant) {
  sf::Vector2f pointInDesignatedQuadrant{0, 0};
  // getting point in right
  switch (quadrant) {
  case 1:
    pointInDesignatedQuadrant.x = point.x;
    pointInDesignatedQuadrant.y = point.y;
    break;
  case 2:
    pointInDesignatedQuadrant.x = -point.x;
    pointInDesignatedQuadrant.y = point.y;
    break;
  case 3:
    pointInDesignatedQuadrant.x = -point.x;
    pointInDesignatedQuadrant.y = -point.y;
    break;
  case 4:
    pointInDesignatedQuadrant.x = point.x;
    pointInDesignatedQuadrant.y = -point.y;
    break;

  default:
    LG_ERR("STAR DUST CONTAINER - movePointToDesignatedQuadrant - INCORRECT "
           "QUADRNAT - ",
           quadrant);
    break;
  }

  return pointInDesignatedQuadrant;
}

template <class elementType, int nrOfElements>
float StarDustContainer<elementType, nrOfElements>::getRandomFloatInRange(
    const float &min, const float &max) {
  // Will be used to obtain a seed for the random number engine
  std::random_device dev;
  // Standard mersenne_twister_engine seeded with dev()
  std::mt19937 rng(dev());
  //[min,max) that's why you add a little sth at the end
  std::uniform_real_distribution<> dis(min, max + 0.0001);
  return dis(rng);
}

template <class elementType, int nrOfElements>
int StarDustContainer<elementType, nrOfElements>::getRandomIntInRange(
    const int &min, const int &max) {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);
  return dist(rng);
}

// TO DO
template class StarDustContainer<StarDust, 40>;
} // namespace spriteObjects