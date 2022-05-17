#pragma once
#include <SFML/Graphics.hpp>

class ISideWindow {
public:

    virtual void start() = 0;

private:
    virtual void input() = 0 ;
    virtual void update(float dtAsSeconds) = 0;
    virtual void draw() = 0;
};