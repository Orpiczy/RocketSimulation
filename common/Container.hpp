#pragma once
#include "IContainerElement.hpp"
#include <SFML/Graphics.hpp>
#include <optional>

template <class T, int nrOfElements> class Container {
public:
  operator std::array<T, nrOfElements> &() { return _elementsContainer; }

  const Vector2f getContainerDimension() { return _containerDimensions; }

  void align(sf::Vector2f referencePoint, sf::Vector2f size, float marginX,
             float marginY, bool keepProportion = false) {

    float elementWidth = (size.x - 2 * marginX);
    float elementHeight =
        (size.y - (nrOfElements + 1) * marginY) / nrOfElements;

    float columnPosition = referencePoint.x + marginX + elementWidth / 2;
    float rowPosition = referencePoint.y + marginY + elementHeight / 2;

    std::cout << "Switch: " << std::endl;
    for (auto &el : _elementsContainer) {
      el.setSizeAndScale({elementWidth, elementHeight}, true);
      el.setPosition({columnPosition, rowPosition});
      std::cout << "row pos = " << rowPosition << std::endl;
      rowPosition += marginY + elementHeight;
    }

    _containerDimensions = size;
  }

  bool click(const Vector2f &globalMousePosition) {
    if (_timer.getElapsedTime() < _keyPressDelay) {
      return false;
    }

    for (auto &el : _elementsContainer) {
      if (static_cast<Sprite>(el).getGlobalBounds().contains(
              globalMousePosition)) {
        el.click();
        _timer.restart();
        return true;
      }
    }

    return false;
  }

  std::array<T, nrOfElements> &getElements() { return _elementsContainer; }

protected:
  std::array<T, nrOfElements> _elementsContainer;
  sf::Time _keyPressDelay{sf::milliseconds(100)};
  sf::Clock _timer;
  Vector2f _containerDimensions{0, 0};
};
