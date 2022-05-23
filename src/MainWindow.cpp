#include "../include/MainWindow.hpp"
#include "../common/Common.hpp"
// #include <chrono>
// #include <thread>

MainWindow::MainWindow(const Vector2i &referencePoint, bool isLogInfoEnable,
                       bool isLogErrorEnable)
    : SimpleLogger(isLogInfoEnable, isLogErrorEnable) {

  _window.create(
      VideoMode(common::MAIN_WINDOW_X_SIZE, common::MAIN_WINDOW_Y_SIZE),
      "Simulation Main Window", common::DEFAULT_WINDOW_STYLE);

  _backgroundTexture.loadFromFile(common::IMG_ABS_PATH + "background3.jpg");

  _backgroundSprite.setTexture(_backgroundTexture);
  _backgroundSprite.setScale(
      common::MAIN_WINDOW_X_SIZE / _backgroundSprite.getLocalBounds().width,
      common::MAIN_WINDOW_Y_SIZE / _backgroundSprite.getLocalBounds().height);

  _window.setPosition(referencePoint);

  LG_INF("MAIN WINDOW - CREATED");
}

Vector2i MainWindow::getPosition() { return _window.getPosition(); }
void MainWindow::MainWindow::start() {

  LG_INF("MAIN WINDOW - LOOP HAS STARTED");
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

void MainWindow::MainWindow::input() {

  // if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
  // {
  //     sf::Vector2i position = sf::Mouse::getPosition();
  //     std::cout<<"Mouse position x = " << position.x << ", y = " <<
  //     position.y << std::endl;
  // }

  if (Keyboard::isKeyPressed(Keyboard::Escape)) {
    LG_INF("MAIN WINDOW - ESC WAS PRESSED, CLOSING WINDOW, EXITING LOOP");
    _window.close();
  }

  if (Keyboard::isKeyPressed(Keyboard::A)) {
    // _rocket.moveLeft();
  } else {
    // _rocket.stopLeft();
  }

  if (Keyboard::isKeyPressed(Keyboard::D)) {
    // _rocket.moveRight();
  } else {
    // _rocket.stopRight();
  }
}

void MainWindow::MainWindow::update(float dtAsSeconds) { _rocket.setAngle(0); }

void MainWindow::MainWindow::draw() {
  _window.clear(Color::White);
  _window.draw(_backgroundSprite);
  _window.draw(_rocket.getSprite());
  _window.display();
}
