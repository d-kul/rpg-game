#include "Entity.h"

#include <type_traits>

#include "Core/Logger.h"
#include "Core/utility.h"

EntityData EntityData::load(std::istream& in) {
  std::string line;
  std::getline(in, line);
  sf::Vector2i position;
  in >> position.x >> position.y;
  if (line == EntityData::Player::HEADER) {
    // TODO(des): sprite data...
    in >> std::ws;
    return EntityData(position, Player{});
  } else if (line == EntityData::Character::HEADER) {
    EntityData::Character character;
    // TODO(des): sprite data...
    in >> character.action >> std::ws;
    return EntityData(position, character);
  } else if (line == EntityData::Prop::HEADER) {
    EntityData::Prop prop;
    // TODO(des): sprite data...
    in >> prop.action >> std::ws;
    return EntityData(position, prop);
  } else if (line == EntityData::Trigger::HEADER) {
    EntityData::Trigger trigger;
    in >> trigger.action >> std::ws;
    return EntityData(position, trigger);
  } else {
    ERROR("invalid entity header");
    throw std::runtime_error("invalid entity header");
  }
}

void EntityData::save(std::ostream& out) {
  std::visit(
      [&](auto&& data) { out << std::decay_t<decltype(data)>::HEADER << '\n'; },
      data);
  out << position.x << ' ' << position.y;
  std::visit(overloaded{[&](EntityData::Player& player) {},
                        [&](EntityData::Character& character) {
                          // TODO(des): sprite data
                          out << ' ' << character.action;
                        },
                        [&](EntityData::Prop& prop) {
                          // TODO(des): sprite data
                          out << ' ' << prop.action;
                        },
                        [&](EntityData::Trigger& trigger) {
                          out << ' ' << trigger.action;
                        }},
             data);
  out << '\n';
}
