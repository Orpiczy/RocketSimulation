#pragma once

#include "../assets/graphics/SpriteObjects/include/Moon.hpp"
#include "../assets/graphics/SpriteObjects/include/Rocket.hpp"
#include "../assets/graphics/SpriteObjects/include/StarDust.hpp"
#include "../assets/graphics/SpriteObjects/include/StarDustContainer.hpp"
#include "../common/classes/SimpleLogger.hpp"
#include "../common/interfaces/IWindow.hpp"
#include <SFML/Graphics.hpp>

// comms
#include "../common/MessageTypes.hpp"

#include <thread>

using namespace sf;

class MainWindow : public IWindow, public SimpleLogger {
public:
  MainWindow(const Vector2i &referencePoint = {0, 0},
             bool isLogInfoEnable = true, bool isLogErrorEnable = true);
  ~MainWindow();
  void start();
  Vector2i getPosition();

private:
  RenderWindow _window;

  Texture _readyToStartStateBackgroundTexture;
  Texture _activeStateBackgroundTexture;
  Texture _successStateBackgroundTexture;
  Texture _failureStateBackgroundTexture;

  Sprite _backgroundSprite;


  spriteObjects::Rocket _rocket;
  spriteObjects::Moon _moon;
  spriteObjects::StarDustContainer<spriteObjects::StarDust, 10>
      _starDustContainerInForeground;
  spriteObjects::StarDustContainer<spriteObjects::StarDust, 40>
      _starDustContainerInBackground;

  SimulationStatus _simulationState{SimulationStatus::READY_TO_START};

  void input();
  void update() {
    updateVisualData();
    updateSimulationState();
  }

  void checkStateAndUpdateCurrentBackgroundSpriteTexture();
  void updateBackgroundSprite();
  void updateVisualData();
  void updateSimulationState();
  void draw();

  void drawInReadyToStartState();
  void drawInActiveState();
  void drawInSuccessState();
  void drawInFailureState();

  std::thread getAndRunUpdatingDataThread();
  std::thread getAndRunUpdatingSimulationStatusThread();

  // COMMUNICATION SETUP
  void openQueues();
  void closeQueues();

  // COMMUNICATION
  msg::RocketVisualizationContainerMsg getVisualizationData();
  msg::SimulationStatusMsg getSimulationStatusData();

  // DATA MANAGMENT
  void updateElementsState(
      const msg::RocketVisualizationContainerMsg &visualizationContainerMsg);
  void updateRocketState(
      const msg::RocketVisualizationContainerMsg &visualizationContainerMsg);
  void updateStarDust(
      const msg::RocketVisualizationContainerMsg &visualizationContainerMsg);
  void
  updateSimulationState(const msg::SimulationStatusMsg &simulationStatusMsg);

  // SETUP HELPERS
  void setWindowSizeAndPosition(const Vector2i &referencePoint);
  void setTexturesAndSprites();
  void setUpElements();

  // DEBUG 
  void LogSimulationState(const SimulationStatus& status);
};