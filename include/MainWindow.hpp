#pragma once

#include "../assets/graphics/SpriteObjects/include/Rocket.hpp"
#include "../common/classes/SimpleLogger.hpp"
#include <SFML/Graphics.hpp>

using namespace sf;

class MainWindow : public SimpleLogger {
public:
  MainWindow(const Vector2i &referencePoint = {0, 0},
             bool isLogInfoEnable = true, bool isLogErrorEnable = true);
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