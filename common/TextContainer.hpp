#pragma once
#include "Common.hpp"
#include "Container.hpp"
#include <SFML/Graphics.hpp>

template <int nrOfElements>
class TextContainer : public Container<sf::Text, nrOfElements> {
public:
  void align(sf::Vector2f referencePoint, sf::Vector2f size, float marginX,
             float marginY, bool keepProportion = false) {

    if (!_defaultFont.loadFromFile(common::FONT_ABS_PATH +
                                   "SevenSegment.ttf")) {
      // FIXME:
      //  handle error
      // dlaczego gdy dodajemy override to program sie nie kompiluje, czemu nie
      // widzi wersji oryginalnej align, sygnatura jest identyczna
    }

    float elementWidth = (size.x - 2 * marginX);
    float elementHeigth =
        (size.y - (nrOfElements + 1) * marginY) / nrOfElements;

    float columnPosition = referencePoint.x + marginX + elementWidth / 2;
    float rowPosition = referencePoint.y + marginY + elementHeigth / 2;

    std::cout << "text: " << std::endl;
    int i = 1;
    for (auto &el : this->_elementsContainer) {
      el.setFont(_defaultFont);
      //   el.setCharacterSize(elementHeigth);
      el.setString("Element" + std::to_string(i++));
      el.setFillColor(_defaultColor);
      float textWidth = el.getLocalBounds().width * el.getScale().x;
      float textHeight = el.getLocalBounds().height * el.getScale().y;

      // dlaczego textHEight nie dzielimy przez 2 to nie mam pojecia
      // tylko tak sie odpowiednio alignuje

      el.setOrigin(textWidth * 0.5f, textHeight);

      el.setPosition({columnPosition, rowPosition});
      std::cout << "row pos = " << rowPosition << std::endl;

      rowPosition += marginY + elementHeigth;
    }

    this->_containerDimensions = size;
  }

private:
  sf::Font _defaultFont;
  sf::Color _defaultColor{0, 200, 0};
};