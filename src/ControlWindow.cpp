#include "../include/ControlWindow.hpp"
#include "../common/Common.hpp"

ControlWindow::ControlWindow(const Vector2i &mainWindowPosition) {

  _window.create(
      VideoMode(common::CONTROL_WINDOW_X_SIZE, common::CONTROL_WINDOW_Y_SIZE),
      _windowName, common::DEFAULT_WINDOW_STYLE);

  _backgroundTexture.loadFromFile(common::IMG_ABS_PATH + "background3.jpg");

  _backgroundSprite.setTexture(_backgroundTexture);
  _backgroundSprite.setScale(common::CONTROL_WINDOW_X_SIZE /
                                 _backgroundSprite.getLocalBounds().width,
                             common::CONTROL_WINDOW_Y_SIZE /
                                 _backgroundSprite.getLocalBounds().height);

  Vector2i absWindowPosition{0, 0};
  absWindowPosition.x =
      mainWindowPosition.x + common::CONTROL_WINDOW_X_RELATIVE_POS;
  absWindowPosition.y =
      mainWindowPosition.y + common::CONTROL_WINDOW_Y_RELATIVE_POS;
  _window.setPosition(absWindowPosition);

  // setElementsPosition();

  float switchesXSize = _window.getSize().x / 4;
  float switchesYSize = _window.getSize().y;
  _thrusterSwitches.align({0.0, 0.0}, {switchesXSize, switchesYSize},
                          common::COMMAND_WINDOW_OBJECTS_X_MARGIN,
                          common::COMMAND_WINDOW_OBJECTS_Y_MARGIN);

  Vector2f descriptionDesignatedPosition{
      _thrusterSwitches.getContainerDimension().x, 0};
  _descriptions.align(descriptionDesignatedPosition,
                      {_window.getSize().x - switchesXSize, switchesYSize},
                      common::COMMAND_WINDOW_OBJECTS_X_MARGIN,
                      common::COMMAND_WINDOW_OBJECTS_Y_MARGIN);
}

void ControlWindow::setElementsPosition() {
  // Vector2i position = _window.getPosition();
  // _mainThrusterSwitch.setPosition({common::COMMAND_WINDOW_OBJECTS_X_MARGIN,
  //                                  common::COMMAND_WINDOW_OBJECTS_Y_MARGIN});

  // _leftThrusterSwitch.setPosition(
  //     {_mainThrusterSwitch.getPosition().x,
  //      _mainThrusterSwitch.getPosition().y + _mainThrusterSwitch.getSize().y
  //      +
  //          +common::COMMAND_WINDOW_OBJECTS_Y_MARGIN});

  // _rightThrusterSwitch.setPosition(
  //     {_leftThrusterSwitch.getPosition().x,
  //      _leftThrusterSwitch.getPosition().y + _leftThrusterSwitch.getSize().y
  //      +
  //          +common::COMMAND_WINDOW_OBJECTS_Y_MARGIN});
}

void ControlWindow::start() {

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
    _window.close();
  }

  if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
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
