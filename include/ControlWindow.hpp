#pragma once
#include "../assets/graphics/SpriteObjects/include/Switch.hpp"
#include "../common/CommonTypes.hpp"
#include "../common/classes/SimpleLogger.hpp"
#include "../common/interfaces/IWindow.hpp"
#include "../common/templates/SwitchContainer.hpp"
#include "../common/templates/TextContainer.hpp"
#include <SFML/Graphics.hpp>

#include <thread>
using namespace sf;

class ControlWindow : public IWindow, public SimpleLogger {
public:
  ControlWindow(const Vector2i &mainWindowPosition = {0, 0},
                bool isLogInfoEnable = true, bool isLogErrorEnable = true);

  ~ControlWindow();
  void start();

private:
  RenderWindow _window;
  Texture _backgroundTexture;
  Sprite _backgroundSprite;
  sf::Font font;

  const char *_windowName = "Control Window";
  const static uint8_t _nrOfElements{3};
  const std::map<uint8_t, std::string> _descriptionMap{
      {0, "LEFT"}, {1, "MAIN"}, {2, "RIGHT"}};
  TextContainer<_nrOfElements> _descriptions{};
  SwitchContainer<_nrOfElements> _thrusterSwitches{};

  void input();
  void update();
  void draw();

  [[deprecated]] std::thread getAndRunInputProcessingThread();

  // COMMUNICATION SETUP
  void openQueues();
  void closeQueues();

  // COMMUNICATION
  void publishThrustersControl();

  // GETERS
  std::tuple<common::MainThrusterState, common::SideThrusterState>
  getThrustersState() const;

  std::tuple<common::MainThrusterState, common::SideThrusterState>
  getThrustersStateFromSwitchesState(
      const std::array<bool, _nrOfElements> switchesState) const;

  // HELPERS
  void setWindowSizeAndPosition(const Vector2i &mainWindowPosition);
  void setTexturesAndSprites();
  void setUpElements();
};