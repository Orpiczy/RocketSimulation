#pragma once
#include "../common/classes/SimpleLogger.hpp"
#include "../common/interfaces/IWindow.hpp"
#include "../common/templates/TextContainer.hpp"
#include <SFML/Graphics.hpp>

// comms
#include "../common/MessageTypes.hpp"

using namespace sf;

class StatusWindow : public IWindow, public SimpleLogger {
public:
  StatusWindow(const Vector2i &mainWindowPosition = {0, 0},
               bool isLogInfoEnable = true, bool isLogErrorEnable = true);

  ~StatusWindow();
  void start();

private:
  RenderWindow _window;
  Texture _backgroundTexture;
  Sprite _backgroundSprite;

  TextContainer<4> _descriptions{};
  TextContainer<4> _gaugeValues{};
  const char *_windowName = "Status Window";
  const std::vector<std::string> _descriptionLookUpTable{"Fuel", "O2", "Speed",
                                                         "Angle"};

  void input();
  void update();
  void draw();

  // HELPERS
  void setWindowSizeAndPosition(const Vector2i &mainWindowPosition);
  void setTexturesAndSprites();
  void setUpElements();
  void updateDescription();
  void updateDisplayedGaugeValues(const std::vector<int> &values);

  // COMMUNICATION SETUP
  void openQueues();
  void closeQueues();

  // COMMUNICATION
  msg::RocketStatusMsg getStatusData();

  // DATA MANAGMENT
  void updateElementsState(const msg::RocketStatusMsg &rocketStatusMsg);
};