#include <math.h>  
#include <algorithm>

#include "../include/Rocket.hpp"
#include "../../../../common/Common.hpp"

namespace spriteObjects 
{

Rocket::Rocket() {
    _rocketTexture.loadFromFile(common::IMG_ABS_PATH + "saturnRocketBasic.png");
    _rocketSprite.setTexture(_rocketTexture);

    auto scaleX = common::ROCKET_X_MAX_SIZE / _rocketSprite.getLocalBounds().width;
    auto scaleY = common::ROCKET_Y_MAX_SIZE / _rocketSprite.getLocalBounds().height;
    auto scale = std::min(scaleX,scaleY);
    _rocketSprite.setScale(scale,scale);

    //origin ignoruje wszystkie tranformacje
    _rocketSprite.setOrigin(Vector2f( _rocketSprite.getTexture()->getSize().x * 0.5, _rocketSprite.getTexture()->getSize().y * 0.5));
   
    _rocketPosition.x = floor(common::MAIN_WINDOW_X_SIZE / 2);
    _rocketPosition.y = floor(common::MAIN_WINDOW_Y_SIZE / 2);
    _rocketSprite.setPosition(_rocketPosition.x,_rocketPosition.y);
}

Sprite Rocket::getSprite() {
    return _rocketSprite;
}

void Rocket::updateSideThrusterState(common::SideThrusterState thrusterState) {
    
}

void Rocket::updateMainThrusterState(common::MainThrusterState thrusterState) {
    
}

void Rocket::setAngle(float angle) {
    
}

const Vector2f Rocket::getRocketSpriteSize() const {

    const sf::Vector2f spriteSize(
        _rocketSprite.getTexture()->getSize().x * _rocketSprite.getScale().x,
        _rocketSprite.getTexture()->getSize().y * _rocketSprite.getScale().y);

    return spriteSize;
}

} // end namespace spriteObjects
