#include "../include/SimulationCore.hpp"
#include "../include/MainWindow.hpp"
#include "../include/ControlWindow.hpp"
#include "../include/StatusWindow.hpp"
#include "../include/RadarWindow.hpp"

void SimulationCore::run() {
    MainWindow mainWindow;
    ControlWindow controlWindow(mainWindow.getPosition());
    RadarWindow radarWindow(mainWindow.getPosition());
    StatusWindow statusWindow(mainWindow.getPosition());
    
    // mainWindow.start();
    // controlWindow.start();
    // radarWindow.start();
    statusWindow.start();
    
}
