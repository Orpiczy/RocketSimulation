#include "../include/ControlWindow.hpp"
#include "../common/Common.hpp"
#include <SFML/Graphics.hpp>

ControlWindow::ControlWindow(const Vector2i &mainWindowPosition,
                             bool isLogInfoEnable, bool isLogErrorEnable)
    : SimpleLogger(isLogInfoEnable, isLogErrorEnable) {
  setWindowSizeAndPosition(mainWindowPosition);
  setTexturesAndSprites();
  setUpElements();
  LG_INF("CONTROL WINDOW - CREATED");
}

void ControlWindow::start() {
  LG_INF("CONTROL WINDOW - LOOP HAS STARTED");
  Clock clock;
  while (_window.isOpen()) {
    Time dt = clock.restart();
    float dtAsSeconds = dt.asSeconds();
    input();
    update(dtAsSeconds);
    draw();
    sf::Event event;
    _window.pollEvent(event);
  }
}

void ControlWindow::input() {

  if (Keyboard::isKeyPressed(Keyboard::Escape)) {
    LG_INF("CONTROL WINDOW - ESC WAS PRESSED, CLOSING WINDOW, EXITING LOOP");
    _window.close();
  }

  if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
    LG_INF("CONTROL WINDOW - LMB WAS PRESSED - CHECKING POSITION AND CLICKING "
           "SWITCH IF NEEDED");
    auto mousePosition = sf::Mouse::getPosition(
        _window); // Mouse position relative to the window
    auto globalMousePosition = _window.mapPixelToCoords(mousePosition);
    _thrusterSwitches.click(globalMousePosition);
  }
}

void ControlWindow::update(float dtAsSeconds) {}

void ControlWindow::draw() {
  _window.clear(Color::White);
  _window.draw(_backgroundSprite);

  for (auto &switchElement : _thrusterSwitches.getElements()) {
    _window.draw(switchElement);
  }

  for (auto &description : _descriptions.getElements()) {
    _window.draw(description);
  }

  _window.display();
}

//// HELPERS

void ControlWindow::setWindowSizeAndPosition(
    const Vector2i &mainWindowPosition) {

  auto windowLength{common::CONTROL_WINDOW_Y_SIZE};
  auto windowHeight{common::CONTROL_WINDOW_Y_SIZE};

  _window.create(VideoMode(windowLength, windowHeight), _windowName,
                 common::DEFAULT_WINDOW_STYLE);

  Vector2i absWindowPosition{0, 0};
  absWindowPosition.x =
      mainWindowPosition.x + common::CONTROL_WINDOW_X_RELATIVE_POS;
  absWindowPosition.y =
      mainWindowPosition.y + common::CONTROL_WINDOW_Y_RELATIVE_POS;
  _window.setPosition(absWindowPosition);
}

void ControlWindow::setTexturesAndSprites() {

  // BACKGROUND
  _backgroundTexture.loadFromFile(common::IMG_ABS_PATH +
                                  "blackBackground1.jpg");

  _backgroundSprite.setTexture(_backgroundTexture);
  _backgroundSprite.setScale(common::CONTROL_WINDOW_X_SIZE /
                                 _backgroundSprite.getLocalBounds().width,
                             common::CONTROL_WINDOW_Y_SIZE /
                                 _backgroundSprite.getLocalBounds().height);
}

void ControlWindow::setUpElements() {

  // THRUSTERS
  float switchesXSize = _window.getSize().x / 4;
  float switchesYSize = _window.getSize().y;
  _thrusterSwitches.align({0.0, 0.0}, {switchesXSize, switchesYSize},
                          common::COMMAND_WINDOW_OBJECTS_X_MARGIN,
                          common::COMMAND_WINDOW_OBJECTS_Y_MARGIN);

  // DESCRIPTIONS
  Vector2f descriptionDesignatedPosition{
      _thrusterSwitches.getContainerDimension().x, 0};

  _descriptions.align(descriptionDesignatedPosition,
                      {_window.getSize().x - switchesXSize, switchesYSize},
                      common::COMMAND_WINDOW_OBJECTS_X_MARGIN,
                      common::COMMAND_WINDOW_OBJECTS_Y_MARGIN,
                      LayoutType::columnLayout);
}