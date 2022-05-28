#pragma once
#include <SFML/Graphics.hpp>

class IWindow {
public:
  virtual void start() = 0;

private:
  virtual void input() = 0;
  virtual void update() = 0;
  virtual void draw() = 0;
};