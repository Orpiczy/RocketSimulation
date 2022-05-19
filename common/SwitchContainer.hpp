#pragma once
#include "../assets/graphics/SpriteObjects/include/Switch.hpp"
#include "Container.hpp"

template <int nrOfElements>
class SwitchContainer : public Container<spriteObjects::Switch, nrOfElements> {
public:
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