#include "../include/ControlWindow.hpp"
#include "../common/Common.hpp"
#include "../common/CommunicationInfo.hpp"
#include "../common/MessageTypes.hpp"
#include <SFML/Graphics.hpp>
#include <errno.h>
#include <string.h>

ControlWindow::ControlWindow(const Vector2i &mainWindowPosition,
                             bool isLogInfoEnable, bool isLogErrorEnable)
    : SimpleLogger(isLogInfoEnable, isLogErrorEnable) {
  setWindowSizeAndPosition(mainWindowPosition);
  setTexturesAndSprites();
  setUpElements();
  openQueues();
  LG_INF("CONTROL WINDOW - CREATED");
}

ControlWindow::~ControlWindow() { closeQueues(); }

void ControlWindow::start() {
  LG_INF("CONTROL WINDOW - LOOP HAS STARTED");
  while (_window.isOpen()) {
    input();
    update();
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
    // LG_INF("CONTROL WINDOW - LMB WAS PRESSED - CHECKING POSITION AND CLICKING
    // "
    //       "SWITCH IF NEEDED");
    auto mousePosition = sf::Mouse::getPosition(
        _window); // Mouse position relative to the window
    auto globalMousePosition = _window.mapPixelToCoords(mousePosition);
    int clickecElementId = _thrusterSwitches.click(globalMousePosition);
    // LG_INF("clickecElementId = ", clickecElementId);

    // update - tak aby dwa side thrustery nigdy nie byly wlaczone razem
    switch (clickecElementId) {
    case 0:

      if (_thrusterSwitches.getElements().at(0).getState()) {
        _thrusterSwitches.getElements().at(2).setOff();
      }
      break;

    case 2:

      if (_thrusterSwitches.getElements().at(2).getState()) {
        _thrusterSwitches.getElements().at(0).setOff();
      }
      break;

    default:
      break;
    }

    if (clickecElementId != -1) {
      publishThrustersControl();
    }
  }
}

void ControlWindow::update() {}

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

//// COMMUNICATION SETUP
void ControlWindow::openQueues() {
  if ((comm::thrustersControlQueue =
           mq_open(comm::THRUSTERS_CONTROL_QUEUE_FILE, O_CREAT | O_RDWR, 0644,
                   &comm::thrustersControlQueueAttr)) == -1) {
    printf(" >> ERROR - Control Window - FAILED TO OPEN THE QUEUE %s\n",
           strerror(errno));
    return;
  }
}
void ControlWindow::closeQueues() { mq_close(comm::thrustersControlQueue); }

//// COMMUNICATION
void ControlWindow::publishThrustersControl() {

  msg::ThrustersStateMsg thrusterStateMsg;
  std::tie(thrusterStateMsg.mainThrusterState,
           thrusterStateMsg.sideThrusterState) = getThrustersState();

  if (mq_send(comm::thrustersControlQueue, (const char *)&thrusterStateMsg,
              sizeof(msg::ThrustersStateMsg), 0) == -1) {
    LG_ERR("Control Window - FAILED TO SEND CONTROL - " +
           std::string(strerror(errno)));
  } else {
    LG_INF("CONTROL WINDOW - SENT THRUSTER CONTROL UPDATE");
  }
}
////GETERS
std::tuple<common::MainThrusterState, common::SideThrusterState>
ControlWindow::getThrustersState() const {

  const std::array<bool, _nrOfElements> switchesState =
      _thrusterSwitches.getSwitchesState();

  return getThrustersStateFromSwitchesState(switchesState);
}

std::tuple<common::MainThrusterState, common::SideThrusterState>
ControlWindow::getThrustersStateFromSwitchesState(
    const std::array<bool, _nrOfElements> switchesState) const {
  // MAIN
  auto mainThrusterState = switchesState.at(1)
                               ? common::MainThrusterState::TURN_ON
                               : common::MainThrusterState::TURN_OFF;
  // LEFT
  auto leftThrusterState = switchesState.at(0)
                               ? common::SideThrusterState::LEFT_ON
                               : common::SideThrusterState::TURN_OFF;

  // RIGHT
  auto sideThrusterState = switchesState.at(2)
                               ? common::SideThrusterState::RIGHT_ON
                               : leftThrusterState;

  return {mainThrusterState, sideThrusterState};
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