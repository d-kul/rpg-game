#include "GameState.h"

#include <SFML/Window/Event.hpp>
#include <iostream>
#include <sstream>

#include "State/MainMenuState.h"

// Lifetime management
void GameState::loadResources() {
  background_texture = new sf::Texture("resources/images/teto_plush.png");
  background_texture->setRepeated(true);
  font = new sf::Font("resources/fonts/papyrus.ttf");
  mono_font = new sf::Font("resources/fonts/DroidSansMono.ttf");
  music = new sf::Music("resources/music/Teto Kasane Teto.ogg");
  music->setLooping(true);
}

void GameState::loadAssets() {
  background.setSize(sf::Vector2f{window.getSize()});
  background.setTexture(background_texture);
  background.setTextureRect(
      sf::IntRect{{0u, 0u}, sf::Vector2i{window.getSize()}});

  // TODO: Player...
  // auto player = create_entity();
  // registry.emplace<PlayerComponent>(player);

  main_text = new sf::Text(*font, "game design is my passion", 40);
  main_text->setFillColor(sf::Color::Green);
  main_text->setOutlineColor(sf::Color::Blue);
  main_text->setOutlineThickness(3.f);
  main_text->setStyle(sf::Text::Bold);
  main_text->setPosition({80.f, 400.f});

  sounds_text = new sf::Text(*font);
  sounds_text->setFillColor(sf::Color::Green);
  sounds_text->setOutlineColor(sf::Color::Blue);
  sounds_text->setOutlineThickness(2.f);
  setText();

  mouse_text = new sf::Text(*mono_font, "", 20);
  mouse_text->setFillColor(sf::Color::White);
  mouse_text->setOutlineColor(sf::Color::Black);
  mouse_text->setOutlineThickness(2.f);
}

void GameState::unloadResources() {
  delete music;
  delete mono_font;
  delete font;
  delete background_texture;
}

void GameState::unloadAssets() {
  delete mouse_text;
  delete sounds_text;
  delete main_text;
}

// Constructors, destructor
GameState::GameState(const State& other) : State(other) {}

GameState::GameState(keybinds_t& keybinds, sf::RenderWindow& window,
                     EventHandler& eventHandler)
    : State(keybinds, window, eventHandler) {}

// State lifetime
void GameState::enter() {
  std::cout << "GameState::enter()" << '\n';
  loadResources();
  loadAssets();
  conn = eventHandler.sink<sf::Event::KeyReleased>().subscribe(
      std::bind(&GameState::onKeyReleased, this, std::placeholders::_1));
  // TODO: Event listeners...
  // registry.on_construct<SoundComponent>().connect<&GameState::onSoundConstruct>(
  //     *this);
  // registry.on_destroy<SoundComponent>().connect<&GameState::onSoundDestroy>(
  //     *this);
  music->play();
}

void GameState::exit() {
  std::cout << "GameState::exit()" << '\n';
  music->stop();
  conn.disconnect();
  // TODO: Event listeners...
  // registry.on_construct<SoundComponent>()
  //     .disconnect<&GameState::onSoundConstruct>(*this);
  // registry.on_destroy<SoundComponent>().disconnect<&GameState::onSoundDestroy>(
  //     *this);
  unloadAssets();
  unloadResources();
}

// Functionality
void GameState::update(sf::Time dt) {
  auto top_left = window.mapPixelToCoords({0, 0});
  sounds_text->setPosition(top_left);
  background.setPosition(top_left);
  background.setTextureRect(
      sf::IntRect{sf::FloatRect{top_left, background.getSize()}});

  auto pos_window = sf::Mouse::getPosition(window);
  auto pos_view = window.mapPixelToCoords(pos_window);
  mouse_text->setPosition(pos_view);
  std::stringstream ss;
  ss << std::setw(7) << pos_window.x << "," << std::setw(7) << pos_window.y
     << "\n"
     << std::setw(7) << std::setprecision(5) << pos_view.x << ","
     << std::setw(7) << std::setprecision(5) << pos_view.y;
  mouse_text->setString(ss.str());
}

void GameState::render() {
  window.draw(background);
  window.draw(*main_text);
  window.draw(*sounds_text);
  window.draw(*main_text);
}

// Helpers
void GameState::setText() {
  sounds_text->setString("things: " + std::to_string(sounds));
}

// Listeners
void GameState::onKeyReleased(sf::Event::KeyReleased keyReleased) {
  if (keyReleased.code == keybinds["QUIT"]) {
    next_state = new MainMenuState(*this);
  }
}

// TODO: ???
// void GameState::onSoundConstruct(entt::registry& registry,
//                                  entt::entity entity) {
//   ++sounds;
//   setText();
// }
//
// void GameState::onSoundDestroy(entt::registry& registry, entt::entity entity)
// {
//   --sounds;
//   setText();
// }
