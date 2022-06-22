#pragma once
#include "../../../../common/CommonTypes.hpp"
#include "../../../../common/classes/SimpleLogger.hpp"
#include <SFML/Graphics.hpp>

using namespace sf;

namespace spriteObjects {

class Moon : public SimpleLogger {
public:
  Moon(bool isLogInfoEnable = true, bool isLogErrorEnable = true);

  Sprite getSprite() { return _moonSprite; }

  void setPositon(const sf::Vector2f &moonRelativeToRocketInCenterPosition);

private:
  Texture _moonTexture;
  Sprite _moonSprite;
  Vector2f _moonPosition;

  void setTextureAndSprite();
};

} // end namespace spriteObjects