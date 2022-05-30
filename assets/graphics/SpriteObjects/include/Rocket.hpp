#pragma once
#include "../../../../common/CommonTypes.hpp"
#include "../../../../common/classes/SimpleLogger.hpp"
#include <SFML/Graphics.hpp>

using namespace sf;

namespace spriteObjects {

class Rocket : public SimpleLogger {
public:
  Rocket(bool isLogInfoEnable = true, bool isLogErrorEnable = true);

  Sprite getSprite();

  void updateSideThrusterState(common::SideThrusterState thrusterState);
  void updateMainThrusterState(common::MainThrusterState thrusterState);
  void setAngle(const float &angleInDegree);

private:
  Texture _rocketTexture;
  Sprite _rocketSprite;
  Vector2f _rocketPosition;
  const Vector2f getRocketSpriteSize() const;

  common::MainThrusterState _mainThrusterState{
      common::MainThrusterState::TURN_OFF};
  common::SideThrusterState _sideThrustersState{
      common::SideThrusterState::TURN_OFF};

  // HELPERS
  void setTextureAndSpriteBaseOnRocketState();
  const std::string getImageNameBaseOnRocketState();

  // Dictates which picture should you load
  const std::string _rocketModelFilename{"rocket/saturnRocketBasic"};
  const std::string _sideThrustersStateFilenamePart{"Soff"};
  const std::string _mainThrusterStateFilenamePart{"Moff"};
  const std::string _rocketImageFormat{".png"};
};

} // end namespace spriteObjects