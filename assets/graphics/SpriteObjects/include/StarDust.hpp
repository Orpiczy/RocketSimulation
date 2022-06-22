#pragma once
#include "../../../../common/CommonTypes.hpp"
#include "../../../../common/classes/SimpleLogger.hpp"
#include <SFML/Graphics.hpp>

using namespace sf;

namespace spriteObjects {

class StarDust : public SimpleLogger {
public:
  StarDust(bool isLogInfoEnable = true, bool isLogErrorEnable = true);

  Sprite getSprite() { return _starDustSprite; }

  void setNewAbsoluteAndDisplayPosition(
      const sf::Vector2f &starDustRelativeToRocketPosition,
      const sf::Vector2f &rocketAbsolutePosition);
  void updateDisplayPosition(const sf::Vector2f &rocketAbsolutePosition);
  float getDistance(const sf::Vector2f &rocketAbsolutePosition);

private:
  Texture _starDustTexture;
  Sprite _starDustSprite;
  Vector2f _starDustAbsolutePosition;

  void setTextureAndSprite();

  // HELPER
  std::string getTextureFilename();
};

} // end namespace spriteObjects