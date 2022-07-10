#include "../include/Moon.hpp"
#include "../../../../common/CommonDisplayInfo.hpp"

namespace spriteObjects {

Moon::Moon(bool isLogInfoEnable, bool isLogErrorEnable)
    : SimpleLogger(isLogInfoEnable, isLogErrorEnable) {
  setTextureAndSprite();
  // DEFAULTS LIKE THIS TO MAKE MOON SPAWN OUTSIDE SCREEN
  setPositon({common::MAIN_WINDOW_X_SIZE * 2, common::MAIN_WINDOW_Y_SIZE * 2});
}

//// HELPERS
void Moon::setTextureAndSprite() {
  _moonTexture.loadFromFile(common::IMG_ABS_PATH + "moon1.png");
  _moonSprite.setTexture(_moonTexture);

  auto scaleX = common::MOON_X_MAX_SIZE / _moonSprite.getLocalBounds().width;
  auto scaleY = common::MOON_Y_MAX_SIZE / _moonSprite.getLocalBounds().height;

  auto scale = std::min(scaleX, scaleY);
  _moonSprite.setScale(scale, scale);

  // origin ignoruje wszystkie tranformacje
  _moonSprite.setOrigin(Vector2f(_moonSprite.getTexture()->getSize().x * 0.5,
                                 _moonSprite.getTexture()->getSize().y * 0.5));
}

void Moon::setPositon(
    const sf::Vector2f &moonRelativeToRocketInCenterPosition) {
  _moonPosition.x = floor(common::MAIN_WINDOW_X_SIZE / 2 +
                          moonRelativeToRocketInCenterPosition.x);
  _moonPosition.y = floor(common::MAIN_WINDOW_Y_SIZE / 2 -
                          moonRelativeToRocketInCenterPosition.y);
  _moonSprite.setPosition(_moonPosition.x, _moonPosition.y);
}
} // namespace spriteObjects