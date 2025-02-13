#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <entt/entt.hpp>
#include <list>
#include <string>
#include <unordered_map>

using keybinds_t = std::unordered_map<std::string, sf::Keyboard::Key>;
using key_index_t = std::unordered_map<std::string, sf::Keyboard::Key>;
using key_storage_t = std::unordered_map<sf::Keyboard::Key, const char*>;
using retain_handle_t = std::shared_ptr<void>;
using retained_t = std::list<retain_handle_t>;
