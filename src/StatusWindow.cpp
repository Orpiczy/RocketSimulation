#include "../include/StatusWindow.hpp"
#include "../common/Common.hpp"

StatusWindow::StatusWindow(const Vector2i &mainWindowPosition) {
  setWindowSizeAndPosition(mainWindowPosition);
  setTexturesAndSprites();
  updateDisplayedGaugeValues({2, 23, 67, 53});
  updateDescription();
  setUpElements();
}

void StatusWindow::setWindowSizeAndPosition(
    const Vector2i &mainWindowPosition) {
  auto windowLength{common::SIDE_WINDOW_Y_SIZE};
  auto windowHeight{common::SIDE_WINDOW_Y_SIZE};

  _window.create(VideoMode(windowLength, windowHeight), _windowName,
                 common::DEFAULT_WINDOW_STYLE);

  Vector2i windowPosition{0, 0};
  windowPosition.x =
      mainWindowPosition.x + common::STATUS_WINDOW_X_RELATIVE_POS;
  windowPosition.y =
      mainWindowPosition.y + common::STATUS_WINDOW_Y_RELATIVE_POS;
  _window.setPosition(windowPosition);
}

void StatusWindow::setUpElements() {

  // DESCRIPTIONS
  float descriptionWidth = _window.getSize().x;
  float descriptionHeight = _window.getSize().y * 0.25;
  _descriptions.align({0.0, 0.0}, {descriptionWidth, descriptionHeight},
                      common::STATUS_WINDOW_OBJECTS_X_MARGIN,
                      common::STATUS_WINDOW_OBJECTS_Y_MARGIN,
                      LayoutType::rowLayout);

  // GAUGE VALUES
  Vector2f gaugeValuesDesignatedPosition{
      0, _descriptions.getContainerDimension().y};

  float gaugeValuesWidth = _window.getSize().x;
  float gaugeValuesHeight = _window.getSize().y - descriptionHeight;

  _gaugeValues.align(
      gaugeValuesDesignatedPosition, {gaugeValuesWidth, gaugeValuesHeight},
      common::STATUS_WINDOW_OBJECTS_X_MARGIN,
      common::STATUS_WINDOW_OBJECTS_Y_MARGIN, LayoutType::rowLayout);
}

void StatusWindow::setTexturesAndSprites() {

  // BACKGROUND
  _backgroundTexture.loadFromFile(common::IMG_ABS_PATH +
                                  "blackBackground1.jpg");
  _backgroundSprite.setTexture(_backgroundTexture);
  _backgroundSprite.setScale(
      common::SIDE_WINDOW_X_SIZE / _backgroundSprite.getLocalBounds().width,
      common::SIDE_WINDOW_Y_SIZE / _backgroundSprite.getLocalBounds().height);
}

void StatusWindow::start() {

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

void StatusWindow::input() {

  if (Keyboard::isKeyPressed(Keyboard::Escape)) {
    _window.close();
  }
}

void StatusWindow::update(float dtAsSeconds) {}

void StatusWindow::draw() {
  _window.clear(Color::White);
  _window.draw(_backgroundSprite);
  for (auto &description : _descriptions.getElements()) {
    _window.draw(description);
  }
  for (auto &gaugeValue : _gaugeValues.getElements()) {
    _window.draw(gaugeValue);
  }

  _window.display();
}

// HELPERS
void StatusWindow::updateDescription() {
  auto &rawDescriptions = _descriptions.getElements();
  for (std::size_t i = 0; i < rawDescriptions.size(); i++) {
    rawDescriptions.at(i).setString(_descriptionLookUpTable.at(i));
  }
}

void StatusWindow::updateDisplayedGaugeValues(
    const std::vector<int> &gaugeValues) {

  for (std::size_t i = 0; i < gaugeValues.size(); i++) {
    std::string stringValue = std::to_string(gaugeValues.at(i)) + " %";
    _gaugeValues.getElements().at(i).setString(stringValue);
  }
}
