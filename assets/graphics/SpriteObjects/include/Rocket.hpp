#pragma once
#include "../../../../common/CommonTypes.hpp"
#include <SFML/Graphics.hpp>

using namespace sf;

namespace spriteObjects {

class Rocket {
public:
  Rocket();

  Sprite getSprite();

  void updateSideThrusterState(common::SideThrusterState thrusterState);
  void updateMainThrusterState(common::MainThrusterState thrusterState);
  void setAngle(const float &angleInDegree);

private:
  Texture _rocketTexture;
  Sprite _rocketSprite;
  Vector2f _rocketPosition;

  const Vector2f getRocketSpriteSize() const;

  struct thrusterState {
    common::MainThrusterState mainThrusterState;
    common::SideThrusterState sideThrusterState;
  };
};

} // end namespace spriteObjects