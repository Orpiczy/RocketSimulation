#include "../include/RadarWindow.hpp"
#include "../common/CommonDisplayInfo.hpp"

// comms
#include "../common/CommunicationInfo.hpp"
#include "../common/MessageTypes.hpp"
#include <mqueue.h>
#include <string.h>

// scheduling
#include "../common/SchedulingInfo.hpp"
#include "../common/SchedulingManagment.hpp"
using SchedulingPriority = schedulingManagment::SchedulingPriority;

RadarWindow::RadarWindow(const Vector2i &mainWindowPosition,
                         bool isLogInfoEnable, bool isLogErrorEnable)
    : SimpleLogger(isLogInfoEnable, isLogErrorEnable) {

  schedulingManagment::setAndLogSchedulingPolicyAndPriority(
      "RadarWindow::RadarWindow", schedulingInfo::queueDefaultType,
      schedulingInfo::initialPriority);

  setWindowSizeAndPosition(mainWindowPosition);
  setMaxRadarSpriteDistane();
  setTexturesAndSprites();
  setScaleAndRange();
  openQueues();
  LG_INF("RADAR WINDOW - CREATED");
}

RadarWindow::~RadarWindow() { closeQueues(); }

void RadarWindow::start() {
  LG_INF("RADAR WINDOW - LOOP HAS STARTED");

  // jak wyjdzie ze scopa to po prostu go zterminuje
  auto updatingDataInBackgroundThread = getAndRunUpdatingDataThread();

  while (_window.isOpen()) {
    input();
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

void RadarWindow::update() {
  auto newData{getPositionData()};
  updateElementsState(newData);
}

void RadarWindow::draw() {
  _window.clear(Color::White);
  _window.draw(_backgroundSprite);
  _window.draw(_rocketSprite);
  _window.draw(_moonSprite);
  _window.display();
}

std::thread RadarWindow::getAndRunUpdatingDataThread() {
  auto runningUpdateInLoopLambda = [this]() {
    schedulingManagment::setAndLogSchedulingPolicyAndPriority(
        "RadarWindow::getAndRunUpdatingDataThread",
        schedulingInfo::queueDefaultType,
        schedulingInfo::updatingVisDataPriority);
    LG_INF("RADAR WINDOW - ENTERING LOOP - UpdatingData lambda in parallel "
           "thread");
    while (_window.isOpen()) {
      update();
    }
    LG_INF(
        "RADAR WINDOW - EXITING LOOP - UpdatingData lambda in parallel thread");
  };
  std::thread updatingThread(runningUpdateInLoopLambda);
  return updatingThread;
}

//// COMMUNICATION SETUP
void RadarWindow::openQueues() {
  if ((comm::objectsPositionQueue =
           mq_open(comm::OBJECTS_POSITION_QUEUE_FILE, O_CREAT | O_RDWR, 0644,
                   &comm::objectsPositionQueueAttr)) == -1) {
    printf(" >> ERROR - RADAR WINDOW  - FAILED TO OPEN POSITION QUEUE %s\n",
           strerror(errno));
    return;
  }
}
void RadarWindow::closeQueues() {
  if (mq_close(comm::objectsPositionQueue) == -1) {
    printf(" >> ERROR - RADAR WINDOW - FAILED TO CLOSE POSITION QUEUE %s\n",
           strerror(errno));
  }
}

//// COMMUNICATION
msg::ObjectsPositionMsg RadarWindow::getPositionData() {
  msg::ObjectsPositionMsg objectPositionMsg;
  if (mq_receive(comm::objectsPositionQueue, (char *)&objectPositionMsg,
                 sizeof(msg::ObjectsPositionMsg), NULL) == -1) {

    LG_ERR("RADAR WINDOW - FAILED TO RECEIVE DATA - " +
           std::string(strerror(errno)));
  } else {
    // LG_INF("RADAR WINDOW - RECEIVED UPDATE OF DATA");
  }
  return objectPositionMsg;
}

//// DATA MANAGMENT
void RadarWindow::updateElementsState(
    const msg::ObjectsPositionMsg &objectsPositionMsg) {
  updateRadar(objectsPositionMsg);
}

void RadarWindow::updateRadar(
    const msg::ObjectsPositionMsg &objectsPositionMsg) {

  float moonPositionInRadarScaleX = (objectsPositionMsg.destinationPosition.x -
                                     objectsPositionMsg.rockePosition.x) *
                                    _scaleRealToRadar;

  float moonPositionInRadarScaleY = (objectsPositionMsg.destinationPosition.y -
                                     objectsPositionMsg.rockePosition.y) *
                                    _scaleRealToRadar;

  // coefficient that makes moon fit in a circle of a radar if moon is too far()
  float R = std::sqrt(moonPositionInRadarScaleX * moonPositionInRadarScaleX +
                      moonPositionInRadarScaleY * moonPositionInRadarScaleY);

  float alfa{1}; // only when its too far
  if (R > _radarMaxRangeInScale) {
    alfa = _radarMaxRangeInScale / R;
  }

  // this is basically relative to the center
  float moonScaledPositionInRadarScaleX = moonPositionInRadarScaleX * alfa;
  float moonScaledPositionInRadarScaleY = moonPositionInRadarScaleY * alfa;

  // now we have to move to window coord
  float moonWindowPositionX =
      _window.getSize().x / 2 + moonScaledPositionInRadarScaleX;
  float moonWindowPositionY =
      _window.getSize().y / 2 -
      moonScaledPositionInRadarScaleY; // we have minus because moving down is
                                       // incrasing y for some crazy reason

  _moonSprite.setPosition(moonWindowPositionX, moonWindowPositionY);
}

//// HELPERS

void RadarWindow::setMaxRadarSpriteDistane() {
  _maxRadarSpriteDistane = _window.getSize().y / 1;
}
void RadarWindow::setWindowSizeAndPosition(const Vector2i &mainWindowPosition) {

  auto windowLength{common::RADAR_WINDOW_X_SIZE};
  auto windowHeight{common::RADAR_WINDOW_Y_SIZE};

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

void RadarWindow::setScaleAndRange() {
  float moonIconRealXSize =
      _moonSprite.getTexture()->getSize().x * _moonSprite.getScale().x;
  float moonIconRealYSize =
      _moonSprite.getTexture()->getSize().y * _moonSprite.getScale().y;

  _radarMaxRangeInScale =
      _window.getSize().y / 2 - std::max(moonIconRealXSize, moonIconRealYSize);

  // I want moon to fit on a screen
  _scaleRealToRadar = _radarMaxRangeInScale / _radarMaxRange;
}