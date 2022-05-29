#include <algorithm>
#include <math.h>

#include "../../../../common/Common.hpp"
#include "../include/Rocket.hpp"

namespace spriteObjects {

Rocket::Rocket(bool isLogInfoEnable, bool isLogErrorEnable)
    : SimpleLogger(isLogInfoEnable, isLogErrorEnable) {
  setTextureAndSpriteBaseOnRocketState();
}

Sprite Rocket::getSprite() {
  setTextureAndSpriteBaseOnRocketState();
  return _rocketSprite;
}

void Rocket::updateSideThrusterState(common::SideThrusterState thrusterState) {
  _sideThrustersState = thrusterState;
}

void Rocket::updateMainThrusterState(common::MainThrusterState thrusterState) {
  _mainThrusterState = thrusterState;
}

void Rocket::setAngle(const float &angleInDegree) {
  _rocketSprite.setRotation(angleInDegree);
}

const Vector2f Rocket::getRocketSpriteSize() const {

  const sf::Vector2f spriteSize(
      _rocketSprite.getTexture()->getSize().x * _rocketSprite.getScale().x,
      _rocketSprite.getTexture()->getSize().y * _rocketSprite.getScale().y);

  return spriteSize;
}

//// HELPERS
void Rocket::setTextureAndSpriteBaseOnRocketState() {
  _rocketTexture.loadFromFile(common::IMG_ABS_PATH +
                              getImageNameBaseOnRocketState());
  _rocketSprite.setTexture(_rocketTexture);

  auto scaleX =
      common::ROCKET_X_MAX_SIZE / _rocketSprite.getLocalBounds().width;
  auto scaleY =
      common::ROCKET_Y_MAX_SIZE / _rocketSprite.getLocalBounds().height;
  auto scale = std::min(scaleX, scaleY);
  _rocketSprite.setScale(scale, scale);

  // origin ignoruje wszystkie tranformacje
  _rocketSprite.setOrigin(
      Vector2f(_rocketSprite.getTexture()->getSize().x * 0.5,
               _rocketSprite.getTexture()->getSize().y * 0.5));

  _rocketPosition.x = floor(common::MAIN_WINDOW_X_SIZE / 2);
  _rocketPosition.y = floor(common::MAIN_WINDOW_Y_SIZE / 2);
  _rocketSprite.setPosition(_rocketPosition.x, _rocketPosition.y);
}

const std::string Rocket::getImageNameBaseOnRocketState() {
  std::string imageName{_rocketModelFilename};

  switch (_mainThrusterState) {
  case common::MainThrusterState::TURN_ON:
    imageName += "Mon";
    break;
  case common::MainThrusterState::TURN_OFF:
    imageName += "Moff";
    break;
  default:
    imageName += "Moff";
    break;
  }

  switch (_sideThrustersState) {
  case common::SideThrusterState::LEFT_ON:
    imageName += "Sl";
    break;
  case common::SideThrusterState::RIGHT_ON:
    imageName += "Sr";
    break;
  case common::SideThrusterState::TURN_OFF:
    imageName += "Soff";
    break;
  default:
    imageName += "Soff";
    break;
  }

  imageName += _rocketImageFormat;

  return imageName;
}

} // end namespace spriteObjects
