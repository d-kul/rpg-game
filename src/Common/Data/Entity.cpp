#include "Entity.h"

#include <type_traits>

#include "Core/Logger.h"

EntityData EntityData::load(std::istream& in) {
  std::string line;
  std::getline(in, line);
  if (line == EntityData::Player::HEADER) {
    EntityData::Player player;
    // TODO(des): sprite data...
    in >> player.position.x >> player.position.y >> std::ws;
    return EntityData(player);
  } else if (line == EntityData::Character::HEADER) {
    EntityData::Character character;
    // TODO(des): sprite data...
    bool hasAction;
    in >> character.position.x >> character.position.y >> hasAction >> std::ws;
    if (hasAction) {
      character.action = ActionData::load(in);
    }
    return EntityData(character);
  } else if (line == EntityData::Prop::HEADER) {
    EntityData::Prop prop;
    // TODO(des): sprite data...
    bool hasAction;
    in >> prop.position.x >> prop.position.y >> hasAction >> std::ws;
    if (hasAction) {
      prop.action = ActionData::load(in);
    }
    return EntityData(prop);
  } else {
    ERROR("invalid entity header");
    throw std::runtime_error("invalid entity header");
  }
}

namespace {
struct Save {
  explicit Save(std::ostream& out) : out(out) {}
  std::ostream& out;
  void operator()(EntityData::Player& player) {
    out << player.position.x << ' ' << player.position.y << '\n';
  }

  void operator()(EntityData::Character& character) {
    out << character.position.x << ' ' << character.position.y << ' '
        << character.action.has_value() << '\n';
    if (character.action) {
      character.action->save(out);
    }
  }

  void operator()(EntityData::Prop& prop) {
    out << prop.position.x << ' ' << prop.position.y << ' '
        << prop.action.has_value() << '\n';
    if (prop.action) {
      prop.action->save(out);
    }
  }
};

}  // namespace

void EntityData::save(std::ostream& out) {
  std::visit(
      [&](auto&& data) { out << std::decay_t<decltype(data)>::HEADER << '\n'; },
      data);
  std::visit(Save(out), data);
}
