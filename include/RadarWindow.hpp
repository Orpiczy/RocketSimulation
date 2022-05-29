#pragma once
#include "../common/classes/SimpleLogger.hpp"
#include "../common/interfaces/IWindow.hpp"
#include <SFML/Graphics.hpp>

// comms
#include "../common/MessageTypes.hpp"

// thread
#include <thread>

using namespace sf;

class RadarWindow : public IWindow, public SimpleLogger {
public:
  RadarWindow(const Vector2i &mainWindowPosition = {0, 0},
              bool isLogInfoEnable = true, bool isLogErrorEnable = true);
  ~RadarWindow();

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

  const char *_windowName{"Radar Window"};
  const int _radarMaxRange{10'000'000}; // meters
  float _scaleRealToRadar{0};
  float _radarMaxRangeInScale{0};

  void input();
  void update();
  void draw();

  std::thread getUpdatingDataInParallelThread();

  // COMMUNICATION SETUP
  void openQueues();
  void closeQueues();

  // HELPERS
  void setWindowSizeAndPosition(const Vector2i &mainWindowPosition);
  void setMaxRadarSpriteDistane();
  void setTexturesAndSprites();
  void setScaleAndRange();

  // COMMUNICATION
  msg::ObjectsPositionMsg getPositionData();

  // DATA MANAGMENT
  void updateElementsState(const msg::ObjectsPositionMsg &objectsPositionMsg);
  void updateRadar(const msg::ObjectsPositionMsg &objectsPositionMsg);
};