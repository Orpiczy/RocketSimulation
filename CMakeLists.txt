cmake_minimum_required(VERSION 3.12)
project(RocketSimulation VERSION 0.1.0)

include(CTest)
enable_testing()

file(GLOB MAIN_FILES CONFIGURE_DEPENDS "include/*.hpp" "src/*.cpp")
file(GLOB COMMON_FILES CONFIGURE_DEPENDS "common/*.hpp" )
file(GLOB COMMON_INTERFACES CONFIGURE_DEPENDS "common/interfaces/*.hpp" )
file(GLOB COMMON_TEMPLATES CONFIGURE_DEPENDS "common/templates/*.hpp")
file(GLOB COMMON_CLASSES CONFIGURE_DEPENDS "common/classes/*.hpp" "common/classes/*.cpp")
file(GLOB SPRITE_OBJ_FILES CONFIGURE_DEPENDS "assets/graphics/SpriteObjects/include/*.hpp" "assets/graphics/SpriteObjects/src/*.cpp")

add_executable(RocketSimulation main.cpp
    ${MAIN_FILES}
    ${COMMON_FILES}
    ${COMMON_CLASSES}
    ${COMMON_INTERFACES}
    ${COMMON_TEMPLATES}
    ${SPRITE_OBJ_FILES}
)

find_package (SFML 2.5 COMPONENTS graphics audio network REQUIRED)
target_link_libraries (${PROJECT_NAME} PUBLIC sfml-system sfml-graphics sfml-audio sfml-network)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
