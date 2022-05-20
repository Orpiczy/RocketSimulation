#pragma once
#include "../Common.hpp"
#include "Container.hpp"
#include <SFML/Graphics.hpp>

template <int nrOfElements>
class TextContainer : public Container<sf::Text, nrOfElements> {
public:
  TextContainer() {
    for (auto &el : this->_elementsContainer) {
      el.setString("Element");
    }
  }

  void alignInColumn(sf::Vector2f referencePoint, sf::Vector2f size,
                     float marginX, float marginY, bool keepProportion) {

    if (!_defaultFont.loadFromFile(common::FONT_ABS_PATH +
                                   "SevenSegment.ttf")) {
      // FIXME:
      //  handle error
      // dlaczego gdy dodajemy override to program sie nie kompiluje, czemu nie
      // widzi wersji oryginalnej align, sygnatura jest identyczna
      // dobor wielkosci napisow wymaga dopracowania
      //      std::cout << "Text ColumnLayout" << std::endl;
      //      std::cout << "row pos = " << rowPosition
      //         << ", col pos = " << columnPosition << std::endl;
    }

    float elementWidth = (size.x - 2 * marginX);
    float elementHeigth =
        (size.y - (nrOfElements + 1) * marginY) / nrOfElements;

    float columnPosition = referencePoint.x + marginX + elementWidth / 2;
    float rowPosition = referencePoint.y + marginY + elementHeigth / 2;

    for (auto &el : this->_elementsContainer) {
      alignText(el, {elementWidth, elementHeigth},
                {columnPosition, rowPosition});
      rowPosition += marginY + elementHeigth;
    }

    this->_containerDimensions = size;
  }

  void alignInRow(sf::Vector2f referencePoint, sf::Vector2f size, float marginX,
                  float marginY, bool keepProportion) override {

    if (!_defaultFont.loadFromFile(common::FONT_ABS_PATH +
                                   "SevenSegment.ttf")) {
      // FIXME:
      //  handle error
      // dlaczego gdy dodajemy override to program sie nie kompiluje, czemu nie
      // widzi wersji oryginalnej align, sygnatura jest identyczna
    }

    float elementWidth = (size.x - (nrOfElements + 1) * marginX) / nrOfElements;
    float elementHeigth = (size.y - 2 * marginY);
    ;

    float columnPosition = referencePoint.x + marginX + elementWidth / 2;
    float rowPosition = referencePoint.y + marginY + elementHeigth / 2;

    for (auto &el : this->_elementsContainer) {
      alignText(el, {elementWidth, elementHeigth},
                {columnPosition, rowPosition});
      columnPosition += marginX + elementWidth;
    }

    this->_containerDimensions = size;
  }

private:
  sf::Font _defaultFont;
  sf::Color _defaultColor{0, 200, 0};

  //// HELPERS

  void scaleText(sf::Text &el, const sf::Vector2f &designatedSize,
                 bool keepProportion = true) {
    // TODO:
  }

  void setTextOrigin(sf::Text &el) {
    float textWidth = el.getLocalBounds().width * el.getScale().x;
    float textHeight = el.getLocalBounds().height * el.getScale().y;

    // dlaczego textHEight nie dzielimy przez 2 to nie mam pojecia
    // tylko tak sie odpowiednio alignuje

    el.setOrigin(textWidth * 0.5f, textHeight);
  }

  void alignText(sf::Text &el, const sf::Vector2f &elSize,
                 const sf::Vector2f &position) {

    el.setFillColor(_defaultColor);
    el.setFont(_defaultFont);
    scaleText(el, {elSize.x, elSize.y});
    setTextOrigin(el);
    el.setPosition({position.x, position.y});
  }
};