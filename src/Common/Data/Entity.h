#pragma once

#include <SFML/System/Vector2.hpp>
#include <optional>
#include <variant>

#include "Common/Data/Action.h"

class EntityData {
 public:
  struct Player {
    static constexpr const char* HEADER = "(Player)";
    sf::Vector2u position;
    // animated sprite
  };

  struct Character {
    static constexpr const char* HEADER = "(Character)";
    sf::Vector2u position;
    // animated sprite
    std::optional<ActionData> action;
  };

  struct Prop {
    static constexpr const char* HEADER = "(Prop)";
    sf::Vector2u position;
    // animated sprite
    std::optional<ActionData> action;
  };

  struct Trigger {
    sf::Vector2u position;
    // sprite
    std::optional<ActionData> action;
  };

  template <typename EntityType>
  EntityData(const EntityType& entity) : data(entity) {}

  std::variant<Player, Character, Prop> data;

  static EntityData load(std::istream& in);
  void save(std::ostream& out);
};
