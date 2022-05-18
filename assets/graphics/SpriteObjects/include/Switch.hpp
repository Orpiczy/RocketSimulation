#pragma once
#include "../../../../common/CommonTypes.hpp"
#include <SFML/Graphics.hpp>

using namespace sf;

namespace spriteObjects {

class Switch {
public:
  Switch();

  operator Sprite() {
    checkStateAndSetProperSprite();
    return _switchSprite;
  }

  void click() { state = !state; }
  void setOn() { state = true; }
  void setOff() { state = false; }

  void setPosition(const Vector2i &designatedPosition);
  const Vector2i getPosition() const {
    return static_cast<Vector2i>(_switchSprite.getPosition());
  }
  const Vector2i getSize() const;

private:
  bool state{false};
  Texture _switchTexture;
  Sprite _switchSprite;
  Vector2f _switchPosition;

  void checkStateAndSetProperSprite();
};
} // namespace spriteObjects