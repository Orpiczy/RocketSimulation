#include "../include/MainWindow.hpp"
#include "../common/CommonDisplayInfo.hpp"

// comms
#include "../common/CommunicationInfo.hpp"
#include <errno.h>
#include <mqueue.h>
#include <string.h>

// debug
#include <sstream>

// scheduling
#include "../common/SchedulingInfo.hpp"
#include "../common/SchedulingManagment.hpp"
using SchedulingPriority = schedulingManagment::SchedulingPriority;

MainWindow::MainWindow(const Vector2i &referencePoint, bool isLogInfoEnable,
                       bool isLogErrorEnable)
    : SimpleLogger(isLogInfoEnable, isLogErrorEnable) {

  schedulingManagment::setAndLogSchedulingPolicyAndPriority(
      "MainWindow::MainWindow", schedulingInfo::queueDefaultType,
      schedulingInfo::initialPriority);

  setWindowSizeAndPosition(referencePoint);
  setTexturesAndSprites();
  setUpElements();
  openQueues();
  LG_INF("MAIN WINDOW - CREATED");
}

MainWindow::~MainWindow() { closeQueues(); }

Vector2i MainWindow::getPosition() { return _window.getPosition(); }

void MainWindow::start() {

  LG_INF("MAIN WINDOW - LOOP HAS STARTED");

  auto updatingDataInBackgroundThread = getAndRunUpdatingDataThread();
  auto updatingSimulationStatusThread =
      getAndRunUpdatingSimulationStatusThread();

  while (_window.isOpen()) {
    input();
    draw();
    sf::Event event;
    _window.pollEvent(event);
  }
}

std::thread MainWindow::getAndRunUpdatingDataThread() {
  auto runningUpdateInLoopLambda = [this]() {
    schedulingManagment::setAndLogSchedulingPolicyAndPriority(
        "MainWindow::getAndRunUpdatingDataThread",
        schedulingInfo::queueDefaultType,
        schedulingInfo::updatingVisDataPriority);

    LG_INF(
        "MAIN WINDOW - ENTERING LOOP - UpdatingData lambda in parallel thread");
    while (_window.isOpen()) {
      updateVisualData();
    }
    LG_INF(
        "MAIN WINDOW- EXITING LOOP - UpdatingData lambda in parallel thread");
  };
  std::thread updatingThread(runningUpdateInLoopLambda);
  return updatingThread;
}

std::thread MainWindow::getAndRunUpdatingSimulationStatusThread() {

  auto runningUpdateOnSimulationStatusInLoopLambda = [this]() {
    schedulingManagment::setAndLogSchedulingPolicyAndPriority(
        "MainWindow::getAndRunUpdatingSimulationStatusThread",
        schedulingInfo::queueDefaultType,
        schedulingInfo::updatingSimStatusPriority);

    LG_INF("MAIN WINDOW - ENTERING LOOP - Updating Status lambda in parallel "
           "thread");
    while (_window.isOpen()) {
      updateSimulationState();
    }
    LG_INF("MAIN WINDOW- EXITING LOOP - Updating Status lambda in parallel "
           "thread");
  };

  std::thread updatingThread(runningUpdateOnSimulationStatusInLoopLambda);
  return updatingThread;
}

void MainWindow::input() {

  if (Keyboard::isKeyPressed(Keyboard::Escape)) {
    LG_INF("MAIN WINDOW - ESC WAS PRESSED, CLOSING WINDOW, EXITING LOOP");
    _window.close();
  }
}

void MainWindow::updateVisualData() {
  auto newData = getVisualizationData();
  updateElementsState(newData);
}

void MainWindow::updateSimulationState() {
  auto newData = getSimulationStatusData();
  updateSimulationState(newData);
}

void MainWindow::draw() {
  _window.clear(Color::White);

  switch (_simulationState) {
  case SimulationStatus::READY_TO_START:
    drawInReadyToStartState();
    break;

  case SimulationStatus::ACTIVE:
    drawInActiveState();
    break;

  case SimulationStatus::SUCCESS:
    drawInSuccessState();
    break;

  case SimulationStatus::FAILURE:
    drawInFailureState();
    break;

  default:
    break;
  }

  _window.display();
}

void MainWindow::drawInReadyToStartState() {
  updateBackgroundSprite();
  _window.draw(_backgroundSprite);
}

void MainWindow::drawInActiveState() {

  updateBackgroundSprite();
  _window.draw(_backgroundSprite);

  for (auto &starDust : _starDustContainerInBackground.getElements()) {
    _window.draw(starDust.getSprite());
  }
  _window.draw(_rocket.getSprite());
  _window.draw(_moon.getSprite());
  for (auto &starDust : _starDustContainerInForeground.getElements()) {
    _window.draw(starDust.getSprite());
  }
}

void MainWindow::drawInSuccessState() {
  updateBackgroundSprite();
  _window.draw(_backgroundSprite);
}
void MainWindow::drawInFailureState() {
  updateBackgroundSprite();
  _window.draw(_backgroundSprite);
}

// COMMUNICATION SETUP
void MainWindow::openQueues() {

  // SIMULATION STATUS
  if ((comm::simulationStatusQueue =
           mq_open(comm::SIMULATION_STATUS_QUEUE_FILE, O_CREAT | O_RDWR, 0644,
                   &comm::simulationStatusQueueAttr)) == -1) {
    printf(" >> ERROR - SIMULATION CORE - FAILED TO OPEN SIMULATION STATUS"
           "QUEUE %s\n",
           strerror(errno));
    return;
  }

  // ROCKET VISUALIZATION
  if ((comm::rocketVisualizationQueue =
           mq_open(comm::ROCKET_VISUALIZATION_QUEUE_FILE, O_CREAT | O_RDWR,
                   0644, &comm::rocketVisualizationQueueAttr)) == -1) {
    printf(" >> ERROR - MAIN WINDOW - FAILED TO OPEN ROCKET VISUALIZATION "
           "QUEUE %s\n",
           strerror(errno));
    return;
  }
}
void MainWindow::closeQueues() {

  // SIMULATION STATUS
  if (mq_close(comm::simulationStatusQueue) == -1) {
    printf(" >> ERROR - MainWindow - FAILED TO CLOSE SIMULATION STATUS "
           "QUEUE %s\n",
           strerror(errno));
  }

  // ROCKET VISUALIZATION
  if (mq_close(comm::rocketVisualizationQueue) == -1) {
    printf(" >> ERROR - MainWindow - FAILED TO CLOSE ROCKET VISUALIZATION "
           "QUEUE %s\n",
           strerror(errno));
  }
}

// COMMUNICATION

msg::RocketVisualizationContainerMsg MainWindow::getVisualizationData() {

  msg::RocketVisualizationContainerMsg visualizationContainerMsg;
  if (mq_receive(comm::rocketVisualizationQueue,
                 (char *)&visualizationContainerMsg,
                 sizeof(msg::RocketVisualizationContainerMsg), NULL) == -1) {
    LG_ERR("MAIN WINDOW - FAILED TO RECEIVE VISUALIZATION DATA - " +
           std::string(strerror(errno)));
  } else {
    // LG_INF("MAIN WINDOW - RECEIVED UPDATE OF VISUALIZATION DATA");
  }
  return visualizationContainerMsg;
}

msg::SimulationStatusMsg MainWindow::getSimulationStatusData() {
  msg::SimulationStatusMsg simulationStatusMsg;
  if (mq_receive(comm::simulationStatusQueue, (char *)&simulationStatusMsg,
                 sizeof(msg::SimulationStatusMsg), NULL) == -1) {
    LG_ERR("MAIN WINDOW - FAILED TO RECEIVE SIMULATION STATUS DATA - " +
           std::string(strerror(errno)));
  }
  return simulationStatusMsg;
}

// DATA MANAGMENT

void MainWindow::updateElementsState(
    const msg::RocketVisualizationContainerMsg &visualizationContainerMsg) {
  updateRocketState(visualizationContainerMsg);
  updateStarDust(visualizationContainerMsg);
}

void MainWindow::updateRocketState(
    const msg::RocketVisualizationContainerMsg &visualizationContainerMsg) {
  // ROCKET
  _rocket.setAngle(visualizationContainerMsg.angle);
  _rocket.updateMainThrusterState(visualizationContainerMsg.mainThrusterState);
  _rocket.updateSideThrusterState(visualizationContainerMsg.sideThrusterState);

  // MOON
  Vector2f moonRelativePosition{0, 0};
  moonRelativePosition.x = visualizationContainerMsg.destinationPosition.x -
                           visualizationContainerMsg.rockePosition.x;
  moonRelativePosition.y = visualizationContainerMsg.destinationPosition.y -
                           visualizationContainerMsg.rockePosition.y;
  _moon.setPositon(moonRelativePosition);

  // debug
  //  std::ostringstream ss;
  //  ss << "moon relative position x = " << moonRelativePosition.x
  //     << " y = " << moonRelativePosition.y;
  //  LG_INF(ss.str());
}

void MainWindow::updateSimulationState(
    const msg::SimulationStatusMsg &simulationStatusMsg) {
  _simulationState = simulationStatusMsg.status;
}

void MainWindow::updateStarDust(
    const msg::RocketVisualizationContainerMsg &visualizationContainerMsg) {
  _starDustContainerInForeground.update(
      visualizationContainerMsg.velocity,
      visualizationContainerMsg.rockePosition);

  _starDustContainerInBackground.update(
      visualizationContainerMsg.velocity,
      visualizationContainerMsg.rockePosition);
}

// SETUP HELPERS
void MainWindow::setWindowSizeAndPosition(const Vector2i &referencePoint) {
  _window.create(
      VideoMode(common::MAIN_WINDOW_X_SIZE, common::MAIN_WINDOW_Y_SIZE),
      "Simulation Main Window", common::DEFAULT_WINDOW_STYLE);
  _window.setPosition(referencePoint);
}

void MainWindow::setTexturesAndSprites() {
  // BACKGROUND
  auto stateVariableBackgroundPath =
      common::IMG_ABS_PATH + "background/stateVariableBackground/";
  LG_INF("MAIN WINDOW IMG PATH -> " + stateVariableBackgroundPath);
  _readyToStartStateBackgroundTexture.loadFromFile(stateVariableBackgroundPath +
                                                   "startBackground1" + ".png");
  _activeStateBackgroundTexture.loadFromFile(stateVariableBackgroundPath +
                                             "activeBackground1" + ".png");
  _successStateBackgroundTexture.loadFromFile(stateVariableBackgroundPath +
                                              "successBackground1" + ".png");
  _failureStateBackgroundTexture.loadFromFile(stateVariableBackgroundPath +
                                              "failureBackground1" + ".png");

  updateBackgroundSprite();
}

void MainWindow::checkStateAndUpdateCurrentBackgroundSpriteTexture() {
  switch (_simulationState) {
  case SimulationStatus::READY_TO_START:
    _backgroundSprite.setTexture(_readyToStartStateBackgroundTexture);
    break;
  case SimulationStatus::ACTIVE:
    _backgroundSprite.setTexture(_activeStateBackgroundTexture);
    break;
  case SimulationStatus::SUCCESS:
    _backgroundSprite.setTexture(_successStateBackgroundTexture);
    break;
  case SimulationStatus::FAILURE:
    _backgroundSprite.setTexture(_failureStateBackgroundTexture);
    break;

  default:
    LG_ERR("MAIN WINDOW - UPDATING BACKGROUND TEXTURE - UNKNOWN STATE");
  }
}

void MainWindow::updateBackgroundSprite() {
  checkStateAndUpdateCurrentBackgroundSpriteTexture();
  _backgroundSprite.setScale(
      common::MAIN_WINDOW_X_SIZE / _backgroundSprite.getLocalBounds().width,
      common::MAIN_WINDOW_Y_SIZE / _backgroundSprite.getLocalBounds().height);
}

void MainWindow::setUpElements() {
  /*
    NOTHING YET
  */
}

// Debug

void MainWindow::LogSimulationState(const SimulationStatus &status) {
  switch (status) {
  case SimulationStatus::READY_TO_START:
    LG_INF("MAIN WINDOW - SIMULATION STATE - READY_TO_START");
    break;
  case SimulationStatus::ACTIVE:
    LG_INF("MAIN WINDOW - SIMULATION STATE - ACTIVE");
    break;
  case SimulationStatus::SUCCESS:
    LG_INF("MAIN WINDOW - SIMULATION STATE - SUCCESS");
    break;
  case SimulationStatus::FAILURE:
    LG_INF("MAIN WINDOW - SIMULATION STATE - FAILURE");
    break;
  default:
    LG_ERR("MAIN WINDOW - SIMULATION STATE - UNKNOWN STATE");
  }
}
