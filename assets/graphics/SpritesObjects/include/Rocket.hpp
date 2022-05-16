#pragma once
#include <SFML/Graphics.hpp>
 
using namespace sf;
 
enum class MainThrusterState
{
    TURN_ON,
    TURN_OFF,
    DAMAGED
};

enum class SideThrusterState
{
    LEFT_ON,
    RIGHT_ON,
    TURN_OFF
};
 
class Rocket
{
public:
 
    Rocket();

    Sprite getSprite();
 
    void updateSideThrusterState(SideThrusterState thrusterState);
    void updateMainThrusterState(MainThrusterState thrusterState);
    void setAngle(float angle);
 
 private:
 
    Vector2f rocketPosition_;
    Sprite rocketSprite_;
    struct thrusterState {
        MainThrusterState mainThrusterState;
        SideThrusterState sideThrusterState;
    };
 
};