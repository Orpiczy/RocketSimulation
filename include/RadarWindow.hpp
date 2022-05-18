#pragma once
#include "../common/IWindow.hpp"
#include <SFML/Graphics.hpp>

using namespace sf;

class RadarWindow : public IWindow {
public:
  RadarWindow() : RadarWindow({0, 0}){};
  RadarWindow(const Vector2i &mainWindowPosition);

  void start();

private:
  RenderWindow _window;
  Texture _backgroundTexture;
  Sprite _backgroundSprite;

  const int _windowRow = 0;
  const char *_windowName = "Radar Window";

  void input();
  void update(float dtAsSeconds);
  void draw();
};