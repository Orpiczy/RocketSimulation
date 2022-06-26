#include "../include/StatusWindow.hpp"
#include "../common/Common.hpp"

// comms
#include "../common/CommunicationInfo.hpp"
#include <mqueue.h>
#include <string.h>

// scheduling
#include "../common/SchedulingInfo.hpp"
#include "../common/SchedulingManagment.hpp"
using SchedulingPriority = schedulingManagment::SchedulingPriority;

StatusWindow::StatusWindow(const Vector2i &mainWindowPosition,
                           bool isLogInfoEnable, bool isLogErrorEnable)
    : SimpleLogger(isLogInfoEnable, isLogErrorEnable) {

  schedulingManagment::setAndLogSchedulingPolicyAndPriority(
      "StatusWindow::StatusWindow", SCHED_FIFO,
      schedulingInfo::initialPriority);

  setWindowSizeAndPosition(mainWindowPosition);
  setTexturesAndSprites();
  updateDisplayedGaugeValues({2, 23, 67, 53});
  updateDescription();
  setUpElements();
  openQueues();
}

StatusWindow::~StatusWindow() { closeQueues(); }

void StatusWindow::start() {

  LG_INF("STATUS WINDOW - LOOP HAS STARTED");

  auto updatingDataInBackgroundThread = getAndRunUpdatingDataThread();
  while (_window.isOpen()) {
    input();
    draw();
    sf::Event event;
    _window.pollEvent(event);
  }
}

void StatusWindow::input() {

  if (Keyboard::isKeyPressed(Keyboard::Escape)) {
    LG_INF("STATUS WINDOW - ESC WAS PRESSED, CLOSING WINDOW, EXITING LOOP");
    _window.close();
  }
}

void StatusWindow::update() {
  auto newData = getStatusData();
  updateElementsState(newData);
}

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

std::thread StatusWindow::getAndRunUpdatingDataThread() {

  auto runningUpdateInLoopLambda = [this]() {
    schedulingManagment::setAndLogSchedulingPolicyAndPriority(
        "StatusWindow::getAndRunUpdatingDataThread()", SCHED_FIFO,
        schedulingInfo::updatingVisDataPriority);

    LG_INF("STATUS WINDOW - ENTERING LOOP - UpdatingData lambda in parallel "
           "thread");
    while (_window.isOpen()) {
      update();
    }
    LG_INF("STATUS WINDOW - EXITING LOOP - UpdatingData lambda in parallel "
           "thread");
  };
  std::thread updatingThread(runningUpdateInLoopLambda);
  return updatingThread;
}

//// COMMUNICATION SETUP
void StatusWindow::openQueues() {
  if ((comm::rocketStatusQueue =
           mq_open(comm::ROCKET_STATUS_QUEUE_FILE, O_CREAT | O_RDWR, 0644,
                   &comm::rocketStatusQueueAttr)) == -1) {
    printf(
        " >> ERROR - Simulation Core - FAILED TO OPEN ROCKET STATUS QUEUE %s\n",
        strerror(errno));
    return;
  }
}
void StatusWindow::closeQueues() {
  if (mq_close(comm::rocketStatusQueue) == -1) {
    printf(" >> ERROR - STATUS WINDOW - FAILED TO CLOSE ROCKET STATUS QUEUE "
           "%s\n",
           strerror(errno));
  }
}

//// COMMUNICATION
msg::RocketStatusMsg StatusWindow::getStatusData() {
  msg::RocketStatusMsg rocketStatusMsg;
  if (mq_receive(comm::rocketStatusQueue, (char *)&rocketStatusMsg,
                 sizeof(msg::RocketStatusMsg), NULL) == -1) {

    LG_ERR("STATUS WINDOW - FAILED TO RECEIVE DATA - " +
           std::string(strerror(errno)));
  } else {
    LG_INF("STATUS WINDOW - RECEIVED UPDATE OF DATA");
  }
  return rocketStatusMsg;
}

//// DATA MANAGMENT
void StatusWindow::updateElementsState(
    const msg::RocketStatusMsg &rocketStatusMsg) {

  int fuel = rocketStatusMsg.fuel;
  int oxygen = rocketStatusMsg.oxygen;
  int velocity = sqrt(rocketStatusMsg.velocity.x * rocketStatusMsg.velocity.x +
                      rocketStatusMsg.velocity.y * rocketStatusMsg.velocity.y);
  int angle = rocketStatusMsg.angle;
  std::vector<int> newData{fuel, oxygen, velocity, angle};

  updateDisplayedGaugeValues(newData);
}

//// HELPERS
void StatusWindow::updateDescription() {
  auto &rawDescriptions = _descriptions.getElements();
  for (std::size_t i = 0; i < rawDescriptions.size(); i++) {
    rawDescriptions.at(i).setString(_descriptionLookUpTable.at(i));
  }
}

void StatusWindow::updateDisplayedGaugeValues(
    const std::vector<int> &gaugeValues) {

  std::string unit;

  for (std::size_t i = 0; i < gaugeValues.size(); i++) {

    switch (i) {
    case 2: // velocity
      unit = "";
      break;
    case 3:      // angle
      unit = ""; // procent "\370" ale nie ma w twojej czcionce
      break;
    default:
      unit = "%";
      break;
    }

    std::string stringValue = std::to_string(gaugeValues.at(i)) + unit;
    _gaugeValues.getElements().at(i).setString(stringValue);
  }
}

void StatusWindow::setWindowSizeAndPosition(
    const Vector2i &mainWindowPosition) {

  auto windowLength{common::STATUS_WINDOW_X_SIZE};
  auto windowHeight{common::STATUS_WINDOW_Y_SIZE};

  _window.create(VideoMode(windowLength, windowHeight), _windowName,
                 common::DEFAULT_WINDOW_STYLE);

  Vector2i windowPosition{0, 0};
  windowPosition.x =
      mainWindowPosition.x + common::STATUS_WINDOW_X_RELATIVE_POS;
  windowPosition.y =
      mainWindowPosition.y + common::STATUS_WINDOW_Y_RELATIVE_POS;
  _window.setPosition(windowPosition);

  LG_INF("STATUS WINDOW - CREATED");
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
