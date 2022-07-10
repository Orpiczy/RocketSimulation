#include "../include/StarDust.hpp"
#include "../../../../common/CommonDisplayInfo.hpp"
#include <random>

namespace spriteObjects {

StarDust::StarDust(bool isLogInfoEnable, bool isLogErrorEnable)
    : SimpleLogger(isLogInfoEnable, isLogErrorEnable) {
  setTextureAndSprite();
  // DEFAULTS ARE LIKE THIS TO MAKE MOON SPAWN OUTSIDE SCREEN
  setNewAbsolutePositionAndDisplay(
      {common::MAIN_WINDOW_X_SIZE * 2, common::MAIN_WINDOW_Y_SIZE * 2}, {0, 0});
}

// HELPERS
void StarDust::setTextureAndSprite() {
  _starDustTexture.loadFromFile(common::IMG_ABS_PATH + getTextureFilename());
  _starDustSprite.setTexture(_starDustTexture);

  auto scaleX =
      common::STAR_DUST_X_MAX_SIZE / _starDustSprite.getLocalBounds().width;
  auto scaleY =
      common::STAR_DUST_Y_MAX_SIZE / _starDustSprite.getLocalBounds().height;

  auto scale = std::min(scaleX, scaleY);
  _starDustSprite.setScale(scale, scale);

  // origin ignoruje wszystkie tranformacje
  _starDustSprite.setOrigin(
      Vector2f(_starDustSprite.getTexture()->getSize().x * 0.5,
               _starDustSprite.getTexture()->getSize().y * 0.5));
}

void StarDust::setNewAbsolutePositionAndDisplay(
    const sf::Vector2f &starDustRelativeToRocketPosition,
    const sf::Vector2f &rocketAbsolutePosition) {

  _starDustAbsolutePosition.x =
      rocketAbsolutePosition.x + starDustRelativeToRocketPosition.x;
  _starDustAbsolutePosition.y =
      rocketAbsolutePosition.y + starDustRelativeToRocketPosition.y;

  updateDisplayPosition(rocketAbsolutePosition);
}

void StarDust::updateDisplayPosition(
    const sf::Vector2f &rocketAbsolutePosition) {
  Vector2f relativePosition;
  relativePosition.x = _starDustAbsolutePosition.x - rocketAbsolutePosition.x;
  relativePosition.y = _starDustAbsolutePosition.y - rocketAbsolutePosition.y;

  Vector2f displayPosition;
  // MAIN WINDOW / 2 BO W CENTRUN USTYWUOWANA JEST RAKIETA
  displayPosition.x =
      floor(common::MAIN_WINDOW_X_SIZE / 2 + relativePosition.x);
  displayPosition.y =
      floor(common::MAIN_WINDOW_Y_SIZE / 2 - relativePosition.y);
  _starDustSprite.setPosition(displayPosition);
}

std::string StarDust::getTextureFilename() {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> dist(1, 2);
  std::string textureFileName = {"stone/stone"};
  textureFileName += std::to_string(dist(rng));
  textureFileName += ".png";
  return textureFileName;
}

float StarDust::getDistance(const sf::Vector2f &rocketAbsolutePosition) {
  float x = _starDustAbsolutePosition.x - rocketAbsolutePosition.x;
  float y = _starDustAbsolutePosition.y - rocketAbsolutePosition.y;
  float distance = sqrt(x * x + y * y);
  return distance;
}
} // namespace spriteObjects