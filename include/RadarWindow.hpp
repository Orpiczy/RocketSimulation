#pragma once 
#include <SFML/Graphics.hpp>
#include "../common/ISideWindow.hpp"

using namespace sf;

class RadarWindow : public ISideWindow {
public:
    RadarWindow(const Vector2i &mainWindowPosition);
    void start();

private:
    RenderWindow _window; 
    Texture _backgroundTexture; 
    Sprite _backgroundSprite;

    const int _windowRow = 0;
    const char* _windowName = "Radar Window";

    void input();
    void update(float dtAsSeconds);
    void draw();
};