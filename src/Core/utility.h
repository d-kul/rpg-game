#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <istream>
#include <string>
#include <unordered_map>

using keybinds_t = std::unordered_map<std::string, sf::Keyboard::Key>;
using key_index_t = std::unordered_map<std::string, sf::Keyboard::Key>;
using key_storage_t = std::unordered_map<sf::Keyboard::Key, const char*>;

template <class... Ts>
struct overloaded : Ts... {
  using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

template <typename T>
T readOne(std::istream& in) {
  T t;
  in >> t >> std::ws;
  return t;
}
