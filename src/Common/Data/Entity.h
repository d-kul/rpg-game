#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <filesystem>
#include <variant>

class EntityData {
 public:
  struct Sprite {
    std::filesystem::path textureFile;
    bool isAnimated;
    union {
      sf::IntRect rect;
      struct {
        int spriteSheet;
        int startAnimation;
      } anim;
    };
  };

  struct Player {
    static constexpr const char* HEADER = "(Player)";
    // Sprite sprite;
  };

  struct Character {
    static constexpr const char* HEADER = "(Character)";
    // Sprite sprite;
    int action = -1;
  };

  struct Prop {
    static constexpr const char* HEADER = "(Prop)";
    // Sprite sprite;
    int action = -1;
  };

  struct Trigger {
    static constexpr const char* HEADER = "(Trigger)";
    int action = -1;
  };

  template <typename EntityType>
  EntityData(sf::Vector2i position, const EntityType& entity)
      : position(position), data(entity) {}

  sf::Vector2i position;
  std::variant<Player, Character, Prop> data;

  static EntityData load(std::istream& in);
  void save(std::ostream& out);
};
