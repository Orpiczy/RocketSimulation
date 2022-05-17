#pragma once

#include <SFML/Graphics.hpp>
#include "../assets/graphics/SpriteObjects/include/Rocket.hpp"


using namespace sf;

class MainWindow
{
public:

    MainWindow();
    void start();
    Vector2i getPosition();

private:
 
    RenderWindow _window; 
    Texture _backgroundTexture; 
    Sprite _backgroundSprite;
    
    spriteObjects::Rocket _rocket;
 
    void input();
    void update(float dtAsSeconds);
    void draw();
 
};