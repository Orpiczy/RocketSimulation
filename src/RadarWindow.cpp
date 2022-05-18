#include "../include/RadarWindow.hpp"
#include "../common/Common.hpp"

RadarWindow::RadarWindow(const Vector2i &mainWindowPosition) {

    _window.create(VideoMode(common::SIDE_WINDOW_X_SIZE, common::SIDE_WINDOW_Y_SIZE),
        _windowName , common::DEFAULT_WINDOW_STYLE);
 
    _backgroundTexture.loadFromFile(common::IMG_ABS_PATH + "background3.jpg");
 
    _backgroundSprite.setTexture(_backgroundTexture);
    _backgroundSprite.setScale(
        common::SIDE_WINDOW_X_SIZE / _backgroundSprite.getLocalBounds().width, 
        common::SIDE_WINDOW_Y_SIZE / _backgroundSprite.getLocalBounds().height);

    Vector2i windowPosition {0,0};
    windowPosition.x = mainWindowPosition.x + common::RADAR_WINDOW_X_RELATIVE_POS;
    windowPosition.y = mainWindowPosition.y + common::RADAR_WINDOW_Y_RELATIVE_POS;
    _window.setPosition(windowPosition);
}

void RadarWindow::start() {

    Clock clock;
    while (_window.isOpen())
    {
        Time dt = clock.restart();
        float dtAsSeconds = dt.asSeconds();
        input();
        update(dtAsSeconds);
        draw();
        sf::Event event;
        _window.pollEvent(event);
    }
}

void RadarWindow::input() {

    if (Keyboard::isKeyPressed(Keyboard::Escape))
    {
        _window.close();
    }
    
}

void RadarWindow::update(float dtAsSeconds) {
    
}

void RadarWindow::draw() {
    _window.clear(Color::White);
    _window.draw(_backgroundSprite);
    _window.display();
}

