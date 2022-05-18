#include "../include/Switch.hpp"
#include "../../../../common/Common.hpp"

namespace spriteObjects {

Switch::Switch() {
  checkStateAndSetProperSprite();
  setPosition({0, 0});
}

void Switch::checkStateAndSetProperSprite() {

  if (state) {
    _switchTexture.loadFromFile(common::IMG_ABS_PATH + "onSwitch.png");
  } else {
    _switchTexture.loadFromFile(common::IMG_ABS_PATH + "offSwitch.png");
  }

  _switchSprite.setTexture(_switchTexture);
  auto scaleX =
      common::SWITCH_X_MAX_SIZE / _switchSprite.getLocalBounds().width;
  auto scaleY =
      common::SWITCH_Y_MAX_SIZE / _switchSprite.getLocalBounds().height;

  auto scale = std::min(scaleX, scaleY);
  _switchSprite.setScale(scale, scale);
}

const Vector2i Switch::getSize() const {
  const sf::Vector2i spriteSize(
      _switchSprite.getTexture()->getSize().x * _switchSprite.getScale().x,
      _switchSprite.getTexture()->getSize().y * _switchSprite.getScale().y);
  return spriteSize;
}
void Switch::setPosition(const Vector2i &designatedPosition) {
  // origin ignoruje wszystkie tranformacje
  // _switchSprite.setOrigin(
  //     Vector2f(_switchSprite.getTexture()->getSize().x * 0.5,
  //              _switchSprite.getTexture()->getSize().y * 0.5));

  _switchPosition.x = designatedPosition.x;
  _switchPosition.y = designatedPosition.y;
  _switchSprite.setPosition(_switchPosition.x, _switchPosition.y);
}
} // namespace spriteObjects