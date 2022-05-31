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

  void
  setPosition(const sf::Vector2f &starDustRelativeToRocketInCenterPosition);
  float getDistance();

private:
  Texture _starDustTexture;
  Sprite _starDustSprite;
  Vector2f _starDustPosition;

  void setTextureAndSprite();

  // HELPER
  std::string getTextureFilename();
};

} // end namespace spriteObjects