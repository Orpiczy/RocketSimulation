#pragma once
#include "../interfaces/IContainerElement.hpp"
#include <SFML/Graphics.hpp>
#include <optional>

enum class LayoutType { rowLayout, columnLayout };

template <class T, int nrOfElements> class Container {
public:
  operator std::array<T, nrOfElements> &() { return _elementsContainer; }

  const sf::Vector2f getContainerDimension() { return _containerDimensions; }

  void align(sf::Vector2f referencePoint, sf::Vector2f size, float marginX,
             float marginY, LayoutType layout = LayoutType::columnLayout,
             bool keepProportion = false) {

    switch (layout) {

    case LayoutType::columnLayout:
      alignInColumn(referencePoint, size, marginX, marginY, keepProportion);
      break;

    case LayoutType::rowLayout:
      alignInRow(referencePoint, size, marginX, marginY, keepProportion);
      break;

    default:
      break;
    }
  }

  virtual void alignInColumn(sf::Vector2f referencePoint, sf::Vector2f size,
                             float marginX, float marginY,
                             bool keepProportion) = 0;

  virtual void alignInRow(sf::Vector2f referencePoint, sf::Vector2f size,
                          float marginX, float marginY,
                          bool keepProportion) = 0;

  int click(const sf::Vector2f &globalMousePosition) {
    if (_timer.getElapsedTime() < _keyPressDelay) {
      return -1;
    }

    int elementId {0};
    for (auto &el : _elementsContainer) {
      if (static_cast<sf::Sprite>(el).getGlobalBounds().contains(
              globalMousePosition)) {
        el.click();
        _timer.restart();
        return elementId;
      }
      elementId ++;
    }

    return -1;
  }

  std::array<T, nrOfElements> &getElements() { return _elementsContainer; }

protected:
  std::array<T, nrOfElements> _elementsContainer;
  sf::Time _keyPressDelay{sf::milliseconds(300)};
  sf::Clock _timer;
  sf::Vector2f _containerDimensions{0, 0};
};
