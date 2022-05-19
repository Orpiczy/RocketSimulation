#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>

namespace common {
static const std::string PROJECT_PATH{
    "/home/black_wraith/Documents/AIR/S8/RealTime/RocketSimulation/"};
static const std::string IMG_ABS_PATH{PROJECT_PATH + "assets/graphics/img/"};
static const std::string FONT_ABS_PATH{PROJECT_PATH + "assets/fonts/"};

static const int RESOLUTION_X{1920};
static const int RESOLUTION_Y{1080};
static const int X_MARGIN_SIZE{10};
static const int Y_MARGIN_SIZE{10};
static const auto DEFAULT_WINDOW_STYLE = sf::Style::Resize;

// Layout z paskiem kontrolnym obok
static const int MAIN_WINDOW_X_SIZE{RESOLUTION_X * 2 / 3};
static const int MAIN_WINDOW_Y_SIZE{RESOLUTION_Y};

static const int SIDE_WINDOW_X_SIZE{RESOLUTION_X - MAIN_WINDOW_X_SIZE -
                                    X_MARGIN_SIZE};
static const int SIDE_WINDOW_Y_SIZE{(RESOLUTION_Y - 2 * Y_MARGIN_SIZE) / 3};
static const int CONTROL_WINDOW_X_SIZE{SIDE_WINDOW_X_SIZE};
static const int CONTROL_WINDOW_Y_SIZE{SIDE_WINDOW_Y_SIZE};

static const int RADAR_WINDOW_X_RELATIVE_POS{MAIN_WINDOW_X_SIZE +
                                             X_MARGIN_SIZE};
static const int RADAR_WINDOW_Y_RELATIVE_POS{0};

static const int STATUS_WINDOW_X_RELATIVE_POS{RADAR_WINDOW_X_RELATIVE_POS};
static const int STATUS_WINDOW_Y_RELATIVE_POS{
    RADAR_WINDOW_Y_RELATIVE_POS + SIDE_WINDOW_Y_SIZE + Y_MARGIN_SIZE};

static const int CONTROL_WINDOW_X_RELATIVE_POS{RADAR_WINDOW_X_RELATIVE_POS};
static const int CONTROL_WINDOW_Y_RELATIVE_POS{
    STATUS_WINDOW_Y_RELATIVE_POS + SIDE_WINDOW_Y_SIZE + Y_MARGIN_SIZE};

// Layout z paskiem kontrolnym na dole
/*
static const int MAIN_WINDOW_X_SIZE{RESOLUTION_X * 2 / 3};
static const int MAIN_WINDOW_Y_SIZE{RESOLUTION_Y * 2 / 3};

static const int SIDE_WINDOW_X_SIZE{RESOLUTION_X - MAIN_WINDOW_X_SIZE -
                                    X_MARGIN_SIZE};
static const int SIDE_WINDOW_Y_SIZE{(RESOLUTION_Y - 2 * Y_MARGIN_SIZE) / 3};
static const int CONTROL_WINDOW_X_SIZE{RESOLUTION_X};
static const int CONTROL_WINDOW_Y_SIZE{SIDE_WINDOW_Y_SIZE};

static const int RADAR_WINDOW_X_RELATIVE_POS{MAIN_WINDOW_X_SIZE +
                                             X_MARGIN_SIZE};
static const int RADAR_WINDOW_Y_RELATIVE_POS{0};

static const int STATUS_WINDOW_X_RELATIVE_POS{RADAR_WINDOW_X_RELATIVE_POS};
static const int STATUS_WINDOW_Y_RELATIVE_POS{
    RADAR_WINDOW_Y_RELATIVE_POS + SIDE_WINDOW_Y_SIZE + Y_MARGIN_SIZE};

static const int CONTROL_WINDOW_X_RELATIVE_POS{0};
static const int CONTROL_WINDOW_Y_RELATIVE_POS{
    STATUS_WINDOW_Y_RELATIVE_POS + SIDE_WINDOW_Y_SIZE + Y_MARGIN_SIZE};

static const float ROCKET_TO_WINDOW_PROPORTION{0.4};
static const int ROCKET_X_MAX_SIZE{
    static_cast<int>(MAIN_WINDOW_X_SIZE * ROCKET_TO_WINDOW_PROPORTION)};
static const int ROCKET_Y_MAX_SIZE{
    static_cast<int>(MAIN_WINDOW_Y_SIZE * ROCKET_TO_WINDOW_PROPORTION)};
*/

// common

static const float ROCKET_TO_WINDOW_PROPORTION{0.4};
static const int ROCKET_X_MAX_SIZE{
    static_cast<int>(MAIN_WINDOW_X_SIZE * ROCKET_TO_WINDOW_PROPORTION)};
static const int ROCKET_Y_MAX_SIZE{
    static_cast<int>(MAIN_WINDOW_Y_SIZE * ROCKET_TO_WINDOW_PROPORTION)};

// SWITCH
static const int SWITCH_X_MAX_SIZE{
    static_cast<int>(CONTROL_WINDOW_X_SIZE * 0.25)};
static const int SWITCH_Y_MAX_SIZE{
    static_cast<int>(CONTROL_WINDOW_Y_SIZE * 0.35)};

static const int COMMAND_WINDOW_OBJECTS_X_MARGIN{5};
static const int COMMAND_WINDOW_OBJECTS_Y_MARGIN{5};

} // namespace common