#pragma once
#include "../../../../common/Common.hpp"
#include "../../../../common/CommonTypes.hpp"
#include "../../../../common/IContainerElement.hpp"
#include <SFML/Graphics.hpp>

using namespace sf;

namespace spriteObjects {

class Switch : IContainerElement {
public:
  Switch();

  operator Sprite &() {
    checkStateAndSetProperSprite();
    return _switchSprite;
  }

  operator Drawable &() override {
    checkStateAndSetProperSprite();
    return _switchSprite;
  }

  void click() { state = !state; }
  void setOn() { state = true; }
  void setOff() { state = false; }

  void setPosition(const Vector2f &designatedPosition) override;
  void setSizeAndScale(const Vector2f &size, bool keepProportion) override;
  const Vector2f getPosition() const override {
    return _switchSprite.getPosition();
  }
  const Vector2f getSize() const override;

private:
  bool state{false};
  Texture _switchTexture;
  Sprite _switchSprite;
  Vector2f _switchPosition;
  Vector2f _switchSize{common::SWITCH_X_MAX_SIZE, common::SWITCH_Y_MAX_SIZE};

  void checkStateAndSetProperSprite();
};
} // namespace spriteObjects