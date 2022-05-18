#include "../include/SimulationCore.hpp"
#include "../include/ControlWindow.hpp"
#include "../include/MainWindow.hpp"
#include "../include/RadarWindow.hpp"
#include "../include/StatusWindow.hpp"

void SimulationCore::run() {
  // MainWindow mainWindow;
  // ControlWindow controlWindow(mainWindow.getPosition());
  ControlWindow controlWindow;
  // RadarWindow radarWindow(mainWindow.getPosition());
  // StatusWindow statusWindow(mainWindow.getPosition());

  // mainWindow.start();
  controlWindow.start();
  //   radarWindow.start();
  //   statusWindow.start();
}
