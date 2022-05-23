#pragma once
#include "../common/classes/SimpleLogger.hpp"
#include "../common/interfaces/IWindow.hpp"
#include "../common/templates/TextContainer.hpp"

#include <SFML/Graphics.hpp>

using namespace sf;

class StatusWindow : public IWindow, public SimpleLogger {
public:
  StatusWindow(const Vector2i &mainWindowPosition = {0, 0},
               bool isLogInfoEnable = true, bool isLogErrorEnable = true);
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
  void update(float dtAsSeconds);
  void draw();

  // HELPERS
  void setWindowSizeAndPosition(const Vector2i &mainWindowPosition);
  void setTexturesAndSprites();
  void setUpElements();
  void updateDescription();
  void updateDisplayedGaugeValues(const std::vector<int> &values);
};