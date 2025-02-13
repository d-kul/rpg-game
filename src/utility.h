#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <string>
#include <unordered_map>

using keybinds_t = std::unordered_map<std::string, sf::Keyboard::Key>;
using key_index_t = std::unordered_map<std::string, sf::Keyboard::Key>;
using key_storage_t = std::unordered_map<sf::Keyboard::Key, const char*>;
