#pragma once
#include "../../../../common/Common.hpp"
#include "../../../../common/classes/SimpleLogger.hpp"
#include <array>

namespace spriteObjects {

template <class elementType, int nrOfElements>
class StarDustContainer : public SimpleLogger {
public:
  StarDustContainer(bool isLogInfoEnable = true, bool isLogErrorEnable = true);
  std::array<elementType, nrOfElements> &getElements() {
    return _elementsContainer;
  }

  void initialSpawn();
  void update(const sf::Vector2f &velocity,
              const sf::Vector2f &rocketAbsolutePosition);

protected:
  std::array<elementType, nrOfElements> _elementsContainer;

private:
  const float _percentageOfObjectsSpawnAtInit{0.4};
  const float _radiusOfPasivity{
      sqrt((common::MAIN_WINDOW_Y_SIZE * common::MAIN_WINDOW_Y_SIZE +
            common::MAIN_WINDOW_X_SIZE * common::MAIN_WINDOW_X_SIZE)) /
      2};
  const float _activityAreaWidth{50};
  const float _radiusOfActivity{_radiusOfPasivity + _activityAreaWidth};

  // HELPERS
  sf::Vector2f getSpawnPoint(const sf::Vector2f &velocity);
  sf::Vector2f movePointToDesignatedQuadrant(const sf::Vector2f &point,
                                             const int &quadrant);
  int getQuadrant(const sf::Vector2f &velocity);
  float getRandomFloatInRange(const float &min, const float &max);
  int getRandomIntInRange(const int &min, const int &max);
};

} // namespace spriteObjects