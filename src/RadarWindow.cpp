#include "../include/RadarWindow.hpp"
#include "../common/Common.hpp"

RadarWindow::RadarWindow(const Vector2i &mainWindowPosition) {

    Vector2f resolution;
    resolution.x = common::SIDE_WINDOW_X_SIZE;
    resolution.y = common::SIDE_WINDOW_Y_SIZE;
 
    _window.create(VideoMode(resolution.x, resolution.y),
        _windowName , Style::None);
 
    _backgroundTexture.loadFromFile(common::IMG_ABS_PATH + "background3.jpg");
 
    _backgroundSprite.setTexture(_backgroundTexture);
    _backgroundSprite.setScale(
        common::SIDE_WINDOW_X_SIZE / _backgroundSprite.getLocalBounds().width, 
        common::SIDE_WINDOW_Y_SIZE / _backgroundSprite.getLocalBounds().height);

    Vector2i windowPosition {0,0};
    windowPosition.x = mainWindowPosition.x + common::MAIN_WINDOW_X_SIZE + common::SIDE_WINDOW_X_MARGIN_SIZE;
    windowPosition.y = mainWindowPosition.y + (common::SIDE_WINDOW_Y_SIZE + common::SIDE_WINDOW_Y_MARGIN_SIZE) * _windowRow ;
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

