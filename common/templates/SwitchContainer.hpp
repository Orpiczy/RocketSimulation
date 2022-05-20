#pragma once
#include "../../assets/graphics/SpriteObjects/include/Switch.hpp"
#include "Container.hpp"

template <int nrOfElements>
class SwitchContainer : public Container<spriteObjects::Switch, nrOfElements> {
public:
  void alignInColumn(sf::Vector2f referencePoint, sf::Vector2f size,
                     float marginX, float marginY,
                     bool keepProportion) override {

    float elementWidth = (size.x - 2 * marginX);
    float elementHeight =
        (size.y - (nrOfElements + 1) * marginY) / nrOfElements;

    float columnPosition = referencePoint.x + marginX + elementWidth / 2;
    float rowPosition = referencePoint.y + marginY + elementHeight / 2;

    // std::cout << "Switch: " << std::endl;
    for (auto &el : this->_elementsContainer) {
      el.setSizeAndScale({elementWidth, elementHeight}, true);
      el.setPosition({columnPosition, rowPosition});
      // std::cout << "row pos = " << rowPosition << std::endl;
      rowPosition += marginY + elementHeight;
    }

    this->_containerDimensions = size;
  }

  void alignInRow(sf::Vector2f referencePoint, sf::Vector2f size, float marginX,
                  float marginY, bool keepProportion) override {}

  std::array<bool, nrOfElements> getSwitchesState() {
    std::array<bool, nrOfElements> switchesState{0};
    for (std::size_t i = 0; i != this->_elementsContainer.size(); i++) {
      //"this" wymagane bo probuje za szybko zasiegnac,
      // chyba przed specjalizacja templatu i wszystko sie sypie
      switchesState.at(i) = this->_elementsContainer.at(i).getState();
    }
    return switchesState;
  }
};