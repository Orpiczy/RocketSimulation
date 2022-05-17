#include "../include/MainWindow.hpp"
#include "../common/Common.hpp"
// #include <chrono>
// #include <thread>

MainWindow::MainWindow::MainWindow() {
    Vector2f resolution;
    resolution.x = common::MAIN_WINDOW_X_SIZE;
    resolution.y = common::MAIN_WINDOW_Y_SIZE;
 
    _window.create(VideoMode(resolution.x, resolution.y),
        "Simulation Main Window",Style::Resize);
 
    _backgroundTexture.loadFromFile(common::IMG_ABS_PATH + "background3.jpg");
 
    _backgroundSprite.setTexture(_backgroundTexture);
    _backgroundSprite.setScale(
        common::MAIN_WINDOW_X_SIZE / _backgroundSprite.getLocalBounds().width, 
        common::MAIN_WINDOW_Y_SIZE / _backgroundSprite.getLocalBounds().height);
}

Vector2i MainWindow::getPosition(){
    return _window.getPosition();
}
void MainWindow::MainWindow::start() {

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
        
        // FIXME: 
        // using namespace std::chrono_literals;
        // std::this_thread::sleep_for(500ms);
        // std::cout<<"I sleep"<<std::endl;
    }
}

void MainWindow::MainWindow::input() {

    // if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    // {
    //     sf::Vector2i position = sf::Mouse::getPosition();
    //     std::cout<<"Mouse position x = " << position.x << ", y = " << position.y << std::endl;     
    // }

    if (Keyboard::isKeyPressed(Keyboard::Escape))
    {
        _window.close();
    }
 
    if (Keyboard::isKeyPressed(Keyboard::A))
    {
        // _rocket.moveLeft();
    }
    else
    {
        // _rocket.stopLeft();
    }
 
    if (Keyboard::isKeyPressed(Keyboard::D))
    {
        // _rocket.moveRight();
    }
    else
    {
        // _rocket.stopRight();
    }     
}

void MainWindow::MainWindow::update(float dtAsSeconds) {
    _rocket.setAngle(0);
}

void MainWindow::MainWindow::draw() {
    _window.clear(Color::White);
    _window.draw(_backgroundSprite);
    _window.draw(_rocket.getSprite());
    _window.display();
}
