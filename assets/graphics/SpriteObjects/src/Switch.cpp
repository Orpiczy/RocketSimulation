#include "../include/Switch.hpp"
#include "../../../../common/CommonDisplayInfo.hpp"

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
  // origin ignoruje wszystkie tranformacje
  _switchSprite.setOrigin(_switchSprite.getTexture()->getSize().x * 0.5,
                          _switchSprite.getTexture()->getSize().y * 0.5);

  setSizeAndScale(_switchSize, true);
}

const Vector2f Switch::getSize() const {
  const sf::Vector2f spriteSize(
      _switchSprite.getTexture()->getSize().x * _switchSprite.getScale().x,
      _switchSprite.getTexture()->getSize().y * _switchSprite.getScale().y);
  return spriteSize;
}

void Switch::setPosition(const Vector2f &designatedPosition) {

  _switchPosition.x = designatedPosition.x;
  _switchPosition.y = designatedPosition.y;
  _switchSprite.setPosition(_switchPosition.x, _switchPosition.y);
}

void Switch::setSizeAndScale(const Vector2f &size, bool keepProportion) {
  _switchSize = size;
  auto scaleX = _switchSize.x / _switchSprite.getLocalBounds().width;
  auto scaleY = _switchSize.y / _switchSprite.getLocalBounds().height;
  if (keepProportion) {
    auto sameScaleToKeepProportion = std::min(scaleX, scaleY);
    _switchSprite.setScale(sameScaleToKeepProportion,
                           sameScaleToKeepProportion);
  } else {
    _switchSprite.setScale(scaleX, scaleY);
  }
}

} // namespace spriteObjects