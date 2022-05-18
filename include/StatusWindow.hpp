#pragma once
#include "../common/IWindow.hpp"
#include <SFML/Graphics.hpp>

using namespace sf;

class StatusWindow : public IWindow {
public:
  StatusWindow() : StatusWindow({0, 0}){};
  StatusWindow(const Vector2i &mainWindowPosition);
  void start();

private:
  RenderWindow _window;
  Texture _backgroundTexture;
  Sprite _backgroundSprite;

  const int _windowRow = 1;
  const char *_windowName = "Status Window";

  void input();
  void update(float dtAsSeconds);
  void draw();
};