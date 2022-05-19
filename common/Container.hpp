#pragma once
#include "IContainerElement.hpp"
#include <SFML/Graphics.hpp>

template <class T, int nrOfElements> class Container {
public:
  operator std::array<T, nrOfElements> &() { return elementsContainer; }

  void align(sf::Vector2i referencePoint, sf::Vector2f size, float marginX,
             float marginY, bool keepProportion = false) {
    float fixedXSize = (size.x - 2 * marginX);
    float fixedYSize = (size.y - (nrOfElements + 1) * marginY) / nrOfElements;
    sf::Vector2f fixedSize{fixedXSize, fixedYSize};

    float columnXPosition = marginX + fixedXSize / 2;

    for (std::size_t i = 0; i != elementsContainer.size(); i++) {
      elementsContainer.at(i).setSizeAndScale(fixedSize, true);
      auto rowYPosition = marginY + fixedYSize / 2 + (marginY + fixedYSize) * i;
      elementsContainer.at(i).setPosition({columnXPosition, rowYPosition});
    }
    std::cout << "fixedXSize = " << fixedXSize
              << ", fixedYSize = " << fixedYSize << std::endl;
  }

  std::array<T, nrOfElements> &get() { return elementsContainer; }

private:
  std::array<T, nrOfElements> elementsContainer;
  //   std::array<IContainerElement, nrOfElements> elementsContainer {};
};
