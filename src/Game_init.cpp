#include "Game.h"

const keybinds_t Game::default_keybinds = {
    {"QUIT", sf::Keyboard::Key::Escape},
    {"START_GAME", sf::Keyboard::Key::Enter},
    {"MAKE_SOUND", sf::Keyboard::Key::B},
    {"MOVE_UP", sf::Keyboard::Key::W},
    {"MOVE_LEFT", sf::Keyboard::Key::A},
    {"MOVE_DOWN", sf::Keyboard::Key::S},
    {"MOVE_RIGHT", sf::Keyboard::Key::D},
};

const key_index_t Game::key_index = {
    {"A", sf::Keyboard::Key::A},
    {"B", sf::Keyboard::Key::B},
    {"C", sf::Keyboard::Key::C},
    {"D", sf::Keyboard::Key::D},
    {"E", sf::Keyboard::Key::E},
    {"F", sf::Keyboard::Key::F},
    {"G", sf::Keyboard::Key::G},
    {"H", sf::Keyboard::Key::H},
    {"I", sf::Keyboard::Key::I},
    {"J", sf::Keyboard::Key::J},
    {"K", sf::Keyboard::Key::K},
    {"L", sf::Keyboard::Key::L},
    {"M", sf::Keyboard::Key::M},
    {"N", sf::Keyboard::Key::N},
    {"O", sf::Keyboard::Key::O},
    {"P", sf::Keyboard::Key::P},
    {"Q", sf::Keyboard::Key::Q},
    {"R", sf::Keyboard::Key::R},
    {"S", sf::Keyboard::Key::S},
    {"T", sf::Keyboard::Key::T},
    {"U", sf::Keyboard::Key::U},
    {"V", sf::Keyboard::Key::V},
    {"W", sf::Keyboard::Key::W},
    {"X", sf::Keyboard::Key::X},
    {"Y", sf::Keyboard::Key::Y},
    {"Z", sf::Keyboard::Key::Z},
    {"Num0", sf::Keyboard::Key::Num0},
    {"Num1", sf::Keyboard::Key::Num1},
    {"Num2", sf::Keyboard::Key::Num2},
    {"Num3", sf::Keyboard::Key::Num3},
    {"Num4", sf::Keyboard::Key::Num4},
    {"Num5", sf::Keyboard::Key::Num5},
    {"Num6", sf::Keyboard::Key::Num6},
    {"Num7", sf::Keyboard::Key::Num7},
    {"Num8", sf::Keyboard::Key::Num8},
    {"Num9", sf::Keyboard::Key::Num9},
    {"Escape", sf::Keyboard::Key::Escape},
    {"LControl", sf::Keyboard::Key::LControl},
    {"LShift", sf::Keyboard::Key::LShift},
    {"LAlt", sf::Keyboard::Key::LAlt},
    {"LSystem", sf::Keyboard::Key::LSystem},
    {"RControl", sf::Keyboard::Key::RControl},
    {"RShift", sf::Keyboard::Key::RShift},
    {"RAlt", sf::Keyboard::Key::RAlt},
    {"RSystem", sf::Keyboard::Key::RSystem},
    {"Menu", sf::Keyboard::Key::Menu},
    {"LBracket", sf::Keyboard::Key::LBracket},
    {"RBracket", sf::Keyboard::Key::RBracket},
    {"Semicolon", sf::Keyboard::Key::Semicolon},
    {"Comma", sf::Keyboard::Key::Comma},
    {"Period", sf::Keyboard::Key::Period},
    {"Apostrophe", sf::Keyboard::Key::Apostrophe},
    {"Slash", sf::Keyboard::Key::Slash},
    {"Backslash", sf::Keyboard::Key::Backslash},
    {"Grave", sf::Keyboard::Key::Grave},
    {"Equal", sf::Keyboard::Key::Equal},
    {"Hyphen", sf::Keyboard::Key::Hyphen},
    {"Space", sf::Keyboard::Key::Space},
    {"Enter", sf::Keyboard::Key::Enter},
    {"Backspace", sf::Keyboard::Key::Backspace},
    {"Tab", sf::Keyboard::Key::Tab},
    {"PageUp", sf::Keyboard::Key::PageUp},
    {"PageDown", sf::Keyboard::Key::PageDown},
    {"End", sf::Keyboard::Key::End},
    {"Home", sf::Keyboard::Key::Home},
    {"Insert", sf::Keyboard::Key::Insert},
    {"Delete", sf::Keyboard::Key::Delete},
    {"Add", sf::Keyboard::Key::Add},
    {"Subtract", sf::Keyboard::Key::Subtract},
    {"Multiply", sf::Keyboard::Key::Multiply},
    {"Divide", sf::Keyboard::Key::Divide},
    {"Left", sf::Keyboard::Key::Left},
    {"Right", sf::Keyboard::Key::Right},
    {"Up", sf::Keyboard::Key::Up},
    {"Down", sf::Keyboard::Key::Down},
    {"Numpad0", sf::Keyboard::Key::Numpad0},
    {"Numpad1", sf::Keyboard::Key::Numpad1},
    {"Numpad2", sf::Keyboard::Key::Numpad2},
    {"Numpad3", sf::Keyboard::Key::Numpad3},
    {"Numpad4", sf::Keyboard::Key::Numpad4},
    {"Numpad5", sf::Keyboard::Key::Numpad5},
    {"Numpad6", sf::Keyboard::Key::Numpad6},
    {"Numpad7", sf::Keyboard::Key::Numpad7},
    {"Numpad8", sf::Keyboard::Key::Numpad8},
    {"Numpad9", sf::Keyboard::Key::Numpad9},
    {"F1", sf::Keyboard::Key::F1},
    {"F2", sf::Keyboard::Key::F2},
    {"F3", sf::Keyboard::Key::F3},
    {"F4", sf::Keyboard::Key::F4},
    {"F5", sf::Keyboard::Key::F5},
    {"F6", sf::Keyboard::Key::F6},
    {"F7", sf::Keyboard::Key::F7},
    {"F8", sf::Keyboard::Key::F8},
    {"F9", sf::Keyboard::Key::F9},
    {"F10", sf::Keyboard::Key::F10},
    {"F11", sf::Keyboard::Key::F11},
    {"F12", sf::Keyboard::Key::F12},
    {"F13", sf::Keyboard::Key::F13},
    {"F14", sf::Keyboard::Key::F14},
    {"F15", sf::Keyboard::Key::F15},
    {"Pause", sf::Keyboard::Key::Pause},
};

const key_storage_t Game::key_storage = {
    {sf::Keyboard::Key::A, "A"},
    {sf::Keyboard::Key::B, "B"},
    {sf::Keyboard::Key::C, "C"},
    {sf::Keyboard::Key::D, "D"},
    {sf::Keyboard::Key::E, "E"},
    {sf::Keyboard::Key::F, "F"},
    {sf::Keyboard::Key::G, "G"},
    {sf::Keyboard::Key::H, "H"},
    {sf::Keyboard::Key::I, "I"},
    {sf::Keyboard::Key::J, "J"},
    {sf::Keyboard::Key::K, "K"},
    {sf::Keyboard::Key::L, "L"},
    {sf::Keyboard::Key::M, "M"},
    {sf::Keyboard::Key::N, "N"},
    {sf::Keyboard::Key::O, "O"},
    {sf::Keyboard::Key::P, "P"},
    {sf::Keyboard::Key::Q, "Q"},
    {sf::Keyboard::Key::R, "R"},
    {sf::Keyboard::Key::S, "S"},
    {sf::Keyboard::Key::T, "T"},
    {sf::Keyboard::Key::U, "U"},
    {sf::Keyboard::Key::V, "V"},
    {sf::Keyboard::Key::W, "W"},
    {sf::Keyboard::Key::X, "X"},
    {sf::Keyboard::Key::Y, "Y"},
    {sf::Keyboard::Key::Z, "Z"},
    {sf::Keyboard::Key::Num0, "Num0"},
    {sf::Keyboard::Key::Num1, "Num1"},
    {sf::Keyboard::Key::Num2, "Num2"},
    {sf::Keyboard::Key::Num3, "Num3"},
    {sf::Keyboard::Key::Num4, "Num4"},
    {sf::Keyboard::Key::Num5, "Num5"},
    {sf::Keyboard::Key::Num6, "Num6"},
    {sf::Keyboard::Key::Num7, "Num7"},
    {sf::Keyboard::Key::Num8, "Num8"},
    {sf::Keyboard::Key::Num9, "Num9"},
    {sf::Keyboard::Key::Escape, "Escape"},
    {sf::Keyboard::Key::LControl, "LControl"},
    {sf::Keyboard::Key::LShift, "LShift"},
    {sf::Keyboard::Key::LAlt, "LAlt"},
    {sf::Keyboard::Key::LSystem, "LSystem"},
    {sf::Keyboard::Key::RControl, "RControl"},
    {sf::Keyboard::Key::RShift, "RShift"},
    {sf::Keyboard::Key::RAlt, "RAlt"},
    {sf::Keyboard::Key::RSystem, "RSystem"},
    {sf::Keyboard::Key::Menu, "Menu"},
    {sf::Keyboard::Key::LBracket, "LBracket"},
    {sf::Keyboard::Key::RBracket, "RBracket"},
    {sf::Keyboard::Key::Semicolon, "Semicolon"},
    {sf::Keyboard::Key::Comma, "Comma"},
    {sf::Keyboard::Key::Period, "Period"},
    {sf::Keyboard::Key::Apostrophe, "Apostrophe"},
    {sf::Keyboard::Key::Slash, "Slash"},
    {sf::Keyboard::Key::Backslash, "Backslash"},
    {sf::Keyboard::Key::Grave, "Grave"},
    {sf::Keyboard::Key::Equal, "Equal"},
    {sf::Keyboard::Key::Hyphen, "Hyphen"},
    {sf::Keyboard::Key::Space, "Space"},
    {sf::Keyboard::Key::Enter, "Enter"},
    {sf::Keyboard::Key::Backspace, "Backspace"},
    {sf::Keyboard::Key::Tab, "Tab"},
    {sf::Keyboard::Key::PageUp, "PageUp"},
    {sf::Keyboard::Key::PageDown, "PageDown"},
    {sf::Keyboard::Key::End, "End"},
    {sf::Keyboard::Key::Home, "Home"},
    {sf::Keyboard::Key::Insert, "Insert"},
    {sf::Keyboard::Key::Delete, "Delete"},
    {sf::Keyboard::Key::Add, "Add"},
    {sf::Keyboard::Key::Subtract, "Subtract"},
    {sf::Keyboard::Key::Multiply, "Multiply"},
    {sf::Keyboard::Key::Divide, "Divide"},
    {sf::Keyboard::Key::Left, "Left"},
    {sf::Keyboard::Key::Right, "Right"},
    {sf::Keyboard::Key::Up, "Up"},
    {sf::Keyboard::Key::Down, "Down"},
    {sf::Keyboard::Key::Numpad0, "Numpad0"},
    {sf::Keyboard::Key::Numpad1, "Numpad1"},
    {sf::Keyboard::Key::Numpad2, "Numpad2"},
    {sf::Keyboard::Key::Numpad3, "Numpad3"},
    {sf::Keyboard::Key::Numpad4, "Numpad4"},
    {sf::Keyboard::Key::Numpad5, "Numpad5"},
    {sf::Keyboard::Key::Numpad6, "Numpad6"},
    {sf::Keyboard::Key::Numpad7, "Numpad7"},
    {sf::Keyboard::Key::Numpad8, "Numpad8"},
    {sf::Keyboard::Key::Numpad9, "Numpad9"},
    {sf::Keyboard::Key::F1, "F1"},
    {sf::Keyboard::Key::F2, "F2"},
    {sf::Keyboard::Key::F3, "F3"},
    {sf::Keyboard::Key::F4, "F4"},
    {sf::Keyboard::Key::F5, "F5"},
    {sf::Keyboard::Key::F6, "F6"},
    {sf::Keyboard::Key::F7, "F7"},
    {sf::Keyboard::Key::F8, "F8"},
    {sf::Keyboard::Key::F9, "F9"},
    {sf::Keyboard::Key::F10, "F10"},
    {sf::Keyboard::Key::F11, "F11"},
    {sf::Keyboard::Key::F12, "F12"},
    {sf::Keyboard::Key::F13, "F13"},
    {sf::Keyboard::Key::F14, "F14"},
    {sf::Keyboard::Key::F15, "F15"},
    {sf::Keyboard::Key::Pause, "Pause"},
};

Game Game::instance{};
