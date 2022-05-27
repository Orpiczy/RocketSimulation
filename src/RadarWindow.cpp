#include "../include/RadarWindow.hpp"
#include "../common/Common.hpp"

// comms
#include "../common/CommunicationInfo.hpp"
#include "../common/MessageTypes.hpp"
#include <mqueue.h>
#include <string.h>

RadarWindow::RadarWindow(const Vector2i &mainWindowPosition,
                         bool isLogInfoEnable, bool isLogErrorEnable)
    : SimpleLogger(isLogInfoEnable, isLogErrorEnable) {
  setWindowSizeAndPosition(mainWindowPosition);
  setMaxRadarSpriteDistane();
  setTexturesAndSprites();
  openQueues();
  LG_INF("RADAR WINDOW - CREATED");
}

RadarWindow::~RadarWindow() { closeQueues(); }

void RadarWindow::start() {
  LG_INF("RADAR WINDOW - LOOP HAS STARTED");
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

void RadarWindow::input() {

  if (Keyboard::isKeyPressed(Keyboard::Escape)) {
    LG_INF("RADAR WINDOW - ESC WAS PRESSED, CLOSING WINDOW, EXITING LOOP");
    _window.close();
  }
}

void RadarWindow::update(float dtAsSeconds) {}

void RadarWindow::draw() {
  _window.clear(Color::White);
  _window.draw(_backgroundSprite);
  _window.draw(_rocketSprite);
  _window.draw(_moonSprite);
  _window.display();
}

//// HELPERS

void RadarWindow::setMaxRadarSpriteDistane() {
  _maxRadarSpriteDistane = _window.getSize().y / 1;
}
void RadarWindow::setWindowSizeAndPosition(const Vector2i &mainWindowPosition) {

  auto windowLength{common::SIDE_WINDOW_Y_SIZE};
  auto windowHeight{common::SIDE_WINDOW_Y_SIZE};

  _window.create(VideoMode(windowLength, windowHeight), _windowName,
                 common::DEFAULT_WINDOW_STYLE);

  Vector2i windowPosition{0, 0};
  windowPosition.x = mainWindowPosition.x + common::RADAR_WINDOW_X_RELATIVE_POS;
  windowPosition.y = mainWindowPosition.y + common::RADAR_WINDOW_Y_RELATIVE_POS;
  _window.setPosition(windowPosition);
}

void RadarWindow::setTexturesAndSprites() {

  // BACKGROUND
  _backgroundTexture.loadFromFile(common::IMG_ABS_PATH + "radar.jpg");
  _backgroundSprite.setTexture(_backgroundTexture);
  _backgroundSprite.setScale(
      _window.getSize().x / _backgroundSprite.getLocalBounds().width,
      _window.getSize().y / _backgroundSprite.getLocalBounds().height);

  // ROCKET
  _rocketTexture.loadFromFile(common::IMG_ABS_PATH + "rocketIcon.png");
  _rocketSprite.setTexture(_rocketTexture);
  _rocketSprite.setScale(_rocketIconScale, _rocketIconScale);
  _rocketSprite.setOrigin(_rocketSprite.getTexture()->getSize().x * 0.5,
                          _rocketSprite.getTexture()->getSize().y * 0.5);
  _rocketSprite.setPosition(_window.getSize().x / 2, _window.getSize().y / 2);

  // MOON
  _moonTexture.loadFromFile(common::IMG_ABS_PATH + "moonIcon.png");
  _moonSprite.setTexture(_moonTexture);
  _moonSprite.setScale(_moonIconScale, _moonIconScale);
  _moonSprite.setOrigin(_moonSprite.getTexture()->getSize().x * 0.5,
                        _moonSprite.getTexture()->getSize().y * 0.5);

  // float moonIconRealXSize = _moonSprite.getTexture()->getSize().x *
  // _moonSprite.getScale().x;
  float moonIconRealYSize =
      _moonSprite.getTexture()->getSize().y * _moonSprite.getScale().y;

  _moonSprite.setPosition(_window.getSize().x / 2, moonIconRealYSize / 2);
}

// COMMUNICATION SETUP
void RadarWindow::openQueues() {
  if ((comm::objectsPositionQueue =
           mq_open(comm::OBJECTS_POSITION_QUEUE_FILE, O_CREAT | O_RDWR, 0644,
                   &comm::objectsPositionQueueAttr)) == -1) {
    printf(" >> ERROR - Simulation Core - FAILED TO OPEN POSITION QUEUE %s\n",
           strerror(errno));
    return;
  }
}
void RadarWindow::closeQueues() { mq_close(comm::objectsPositionQueue); }