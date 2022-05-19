#pragma once
#include "../assets/graphics/SpriteObjects/include/Switch.hpp"
#include "../common/Container.hpp"
#include "../common/IWindow.hpp"
#include <SFML/Graphics.hpp>
using namespace sf;

class ControlWindow : public IWindow {
public:
  ControlWindow() : ControlWindow({0, 0}){};
  ControlWindow(const Vector2i &mainWindowPosition);
  void start();

private:
  RenderWindow _window;
  Texture _backgroundTexture;
  Sprite _backgroundSprite;
  sf::Font font;
  const int MAX_NUMBER_OF_ITEMS{3};
  sf::Text descriptions[3];

  const char *_windowName = "Control Window";

  // spriteObjects::Switch _mainThrusterSwitch;
  // spriteObjects::Switch _leftThrusterSwitch;
  // spriteObjects::Switch _rightThrusterSwitch;

  Container<spriteObjects::Switch, 3> _thrusterSwitches{};

  void input();
  void update(float dtAsSeconds);
  void draw();

  // HELPERS
  void setElementsPosition();
};