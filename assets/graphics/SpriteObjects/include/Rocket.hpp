#pragma once
#include <SFML/Graphics.hpp>
#include "../../../../common/CommonTypes.hpp"

using namespace sf;
 
namespace spriteObjects 
{

class Rocket
{
public:
 
    Rocket();

    Sprite getSprite();
 
    void updateSideThrusterState(common::SideThrusterState thrusterState);
    void updateMainThrusterState(common::MainThrusterState thrusterState);
    void setAngle(float angle);
 
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