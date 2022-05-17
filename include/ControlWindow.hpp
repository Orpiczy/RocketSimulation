#pragma once 
#include <SFML/Graphics.hpp>
#include "../common/ISideWindow.hpp"
using namespace sf;

class ControlWindow: public ISideWindow{
public:
    ControlWindow(const Vector2i &mainWindowPosition);
    void start();

private:
    RenderWindow _window; 
    Texture _backgroundTexture; 
    Sprite _backgroundSprite;

    const int _windowRow = 2;
    const char* _windowName = "Control Window";

    void input();
    void update(float dtAsSeconds);
    void draw();
};