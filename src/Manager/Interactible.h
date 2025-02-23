#pragma once

#include <SFML/System/Vector2.hpp>
#include <map>

class InteractibleManager;

#include "Entity/Interactible.h"

class InteractibleManager {
 public:
  void addInteractible(sf::Vector2i position, Interactible& i);
  void removeInteractible(sf::Vector2i position);
  void interact(sf::Vector2i position);

 private:
  struct CoordCmp {
    bool operator()(const sf::Vector2i& a, const sf::Vector2i& b) const {
      return a.x == b.x ? a.y < b.y : a.x < b.x;
    }
  };

  std::map<sf::Vector2i, Interactible*, CoordCmp> interactibles;
};
