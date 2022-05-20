#pragma once
#include "../common/interfaces/IWindow.hpp"
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

  Texture _rocketTexture;
  Sprite _rocketSprite;

  Texture _moonTexture;
  Sprite _moonSprite;

  sf::Vector2f relativeDestinationPosition{0, 0};

  float _moonIconScale = 0.05;
  float _rocketIconScale = 0.065;
  float _maxRadarSpriteDistane{0.0};

  const char *_windowName = "Radar Window";

  void input();
  void update(float dtAsSeconds);
  void draw();

  //// HELPERS
  void setWindowSizeAndPosition(const Vector2i &mainWindowPosition);
  void setMaxRadarSpriteDistane();
  void setTexturesAndSprites();
};