#pragma once

#include "../assets/graphics/SpriteObjects/include/Rocket.hpp"
#include "../common/classes/SimpleLogger.hpp"
#include <SFML/Graphics.hpp>

// comms
#include "../common/MessageTypes.hpp"

using namespace sf;

class MainWindow : public SimpleLogger {
public:
  MainWindow(const Vector2i &referencePoint = {0, 0},
             bool isLogInfoEnable = true, bool isLogErrorEnable = true);
  ~MainWindow();
  void start();
  Vector2i getPosition();

private:
  RenderWindow _window;
  Texture _backgroundTexture;
  Sprite _backgroundSprite;

  spriteObjects::Rocket _rocket;

  void input();
  void update();
  void draw();

  // COMMUNICATION SETUP
  void openQueues();
  void closeQueues();

  //// COMMUNICATION
  msg::RocketVisualizationContainerMsg getVisualizationData();

  //// DATA MANAGMENT
  void updateElementsState(
      const msg::RocketVisualizationContainerMsg &visualizationContainerMsg);
  void updateRocketState(
      const msg::RocketVisualizationContainerMsg &visualizationContainerMsg);

  //// SETUP HELPERS
  void setWindowSizeAndPosition(const Vector2i &referencePoint);
  void setTexturesAndSprites();
  void setUpElements();
};