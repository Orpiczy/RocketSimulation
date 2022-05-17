#pragma once
#include <iostream>
#include <math.h>

namespace common {
    static const std::string PROJECT_PATH {"/home/black_wraith/Documents/AIR/S8/RealTime/RocketSimulation/"};
    static const std::string IMG_ABS_PATH {PROJECT_PATH + "assets/graphics/img/"};

    static const int MAIN_WINDOW_X_SIZE {1280}; //1920 * 2/3
    static const int MAIN_WINDOW_Y_SIZE {1080}; //1080 * 2/3
    static const int SIDE_WINDOW_X_SIZE {640}; //1920 * 1/3
    static const int SIDE_WINDOW_Y_SIZE {360}; //1080 * 1/3

    static const float ROCKET_TO_WINDOW_PROPORTION {0.4};
    static const int ROCKET_X_MAX_SIZE {static_cast<int>(MAIN_WINDOW_X_SIZE * ROCKET_TO_WINDOW_PROPORTION)};
    static const int ROCKET_Y_MAX_SIZE {static_cast<int>(MAIN_WINDOW_Y_SIZE * ROCKET_TO_WINDOW_PROPORTION)};

    static const int SIDE_WINDOW_X_MARGIN_SIZE {10};
    static const int SIDE_WINDOW_Y_MARGIN_SIZE {0};
}