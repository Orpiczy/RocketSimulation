#pragma once
#include "../assets/graphics/SpriteObjects/include/Switch.hpp"
#include "../common/interfaces/IWindow.hpp"
#include "../common/templates/SwitchContainer.hpp"
#include "../common/templates/TextContainer.hpp"
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

  const char *_windowName = "Control Window";

  // spriteObjects::Switch _mainThrusterSwitch;
  // spriteObjects::Switch _leftThrusterSwitch;
  // spriteObjects::Switch _rightThrusterSwitch;
  TextContainer<3> _descriptions{};
  SwitchContainer<3> _thrusterSwitches{};

  void input();
  void update(float dtAsSeconds);
  void draw();

  // HELPERS
  void setWindowSizeAndPosition(const Vector2i &mainWindowPosition);
  void setTexturesAndSprites();
  void setUpElements();
};