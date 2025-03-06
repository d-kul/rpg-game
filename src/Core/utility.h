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

template <typename T, typename... Ts>
struct is_one_of {
  static constexpr bool value = false;
};

template <typename T, typename... Ts>
struct is_one_of<T, T, Ts...> {
  static constexpr bool value = true;
};

template <typename T, typename U, typename... Ts>
struct is_one_of<T, U, Ts...> : is_one_of<T, Ts...> {};

template <typename T, typename... Ts>
constexpr bool is_one_of_v = is_one_of<T, Ts...>::value;
