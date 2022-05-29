#include "../include/MainWindow.hpp"
#include "../common/Common.hpp"

// comms
#include "../common/CommunicationInfo.hpp"
#include <errno.h>
#include <mqueue.h>
#include <string.h>

MainWindow::MainWindow(const Vector2i &referencePoint, bool isLogInfoEnable,
                       bool isLogErrorEnable)
    : SimpleLogger(isLogInfoEnable, isLogErrorEnable) {
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

  while (_window.isOpen()) {
    input();
    draw();
    sf::Event event;
    _window.pollEvent(event);
  }
}

void MainWindow::input() {

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

void MainWindow::update() {
  auto newData = getVisualizationData();
  updateElementsState(newData);
}

void MainWindow::draw() {
  _window.clear(Color::White);
  _window.draw(_backgroundSprite);
  _window.draw(_rocket.getSprite());
  _window.display();
}

std::thread MainWindow::getAndRunUpdatingDataThread() {
  auto runningUpdateInLoopLambda = [this]() {
    LG_INF(
        "MAIN WINDOW - ENTERING LOOP - UpdatingData lambda in parallel thread");
    while (_window.isOpen()) {
      update();
    }
    LG_INF(
        "MAIN WINDOW- EXITING LOOP - UpdatingData lambda in parallel thread");
  };
  std::thread updatingThread(runningUpdateInLoopLambda);
  return updatingThread;
}

//// COMMUNICATION SETUP
void MainWindow::openQueues() {
  if ((comm::rocketVisualizationQueue =
           mq_open(comm::ROCKET_VISUALIZATION_QUEUE_FILE, O_CREAT | O_RDWR,
                   0644, &comm::rocketVisualizationQueueAttr)) == -1) {
    printf(" >> ERROR - MAIN WINDOW - FAILED TO OPEN ROCKET VISUALIZATION "
           "QUEUE %s\n",
           strerror(errno));
    return;
  }
}
void MainWindow::closeQueues() { mq_close(comm::rocketVisualizationQueue); }

//// COMMUNICATION

msg::RocketVisualizationContainerMsg MainWindow::getVisualizationData() {

  msg::RocketVisualizationContainerMsg visualizationContainerMsg;
  if (mq_receive(comm::rocketVisualizationQueue,
                 (char *)&visualizationContainerMsg,
                 sizeof(msg::RocketVisualizationContainerMsg), NULL) == -1) {
    LG_ERR("MAIN WINDOW - FAILED TO RECEIVE VISUALIZATION DATA - " +
           std::string(strerror(errno)));
  } else {
    LG_INF("MAIN WINDOW - RECEIVED UPDATE OF VISUALIZATION DATA");
  }
  return visualizationContainerMsg;
}

//// DATA MANAGMENT

void MainWindow::updateElementsState(
    const msg::RocketVisualizationContainerMsg &visualizationContainerMsg) {
  updateRocketState(visualizationContainerMsg);
}

void MainWindow::updateRocketState(
    const msg::RocketVisualizationContainerMsg &visualizationContainerMsg) {
  auto currenRockettAngle = visualizationContainerMsg.angle;
  LG_INF("MAIN WINDOW - RECEIVED ROCKET ANGLE", currenRockettAngle);
  _rocket.setAngle(currenRockettAngle);
}

//// SETUP HELPERS
void MainWindow::setWindowSizeAndPosition(const Vector2i &referencePoint) {
  _window.create(
      VideoMode(common::MAIN_WINDOW_X_SIZE, common::MAIN_WINDOW_Y_SIZE),
      "Simulation Main Window", common::DEFAULT_WINDOW_STYLE);
  _window.setPosition(referencePoint);
}

void MainWindow::setTexturesAndSprites() {

  // BACKGROUND

  _backgroundTexture.loadFromFile(common::IMG_ABS_PATH + "background3.jpg");

  _backgroundSprite.setTexture(_backgroundTexture);
  _backgroundSprite.setScale(
      common::MAIN_WINDOW_X_SIZE / _backgroundSprite.getLocalBounds().width,
      common::MAIN_WINDOW_Y_SIZE / _backgroundSprite.getLocalBounds().height);
}

void MainWindow::setUpElements() {
  /*
    NOTHING YET
  */
}
