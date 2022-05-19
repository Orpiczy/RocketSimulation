#pragma once
#include <SFML/Graphics.hpp>

class IContainerElement {
public:
  virtual void setPosition(const sf::Vector2f &designatedPosition) = 0;
  virtual void setSizeAndScale(const sf::Vector2f &size,
                               bool keepProportion) = 0;
  virtual operator sf::Drawable &() = 0;
  virtual const sf::Vector2f getPosition() const = 0;
  virtual const sf::Vector2f getSize() const = 0;
};