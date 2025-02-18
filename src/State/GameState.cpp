#include "GameState.h"

#include <iostream>
#include <sstream>

#include "State/MainMenuState.h"

// Lifetime management
void GameState::loadResources() {
  background_texture = resourceManager.retain<sf::Texture>(
      "textures/teto_plush", "resources/images/teto_plush.png");
  background_texture->setRepeated(true);
  font = resourceManager.retain<sf::Font>("fonts/papyrus",
                                          "resources/fonts/papyrus.ttf");
  mono_font = resourceManager.retain<sf::Font>(
      "fonts/DroidSansMono", "resources/fonts/DroidSansMono.ttf");
  music = resourceManager.retain<sf::Music>(
      "music/Teto Kasane Teto", "resources/music/Teto Kasane Teto.ogg");
  music->setLooping(true);
  tileset = resourceManager.retain<TileSet>("textures/comic mono",
                                            sf::Vector2i{32, 32},
                                            "resources/images/comic mono.png");
}

void GameState::loadAssets() {
  background.setTexture(background_texture);

  main_text =
      std::make_unique<sf::Text>(*font, "game design is my passion", 40);
  main_text->setFillColor(sf::Color::Green);
  main_text->setOutlineColor(sf::Color::Blue);
  main_text->setOutlineThickness(3.f);
  main_text->setStyle(sf::Text::Bold);
  main_text->setPosition({80.f, 400.f});

  sounds_text = std::make_unique<sf::Text>(*font);
  sounds_text->setFillColor(sf::Color::Green);
  sounds_text->setOutlineColor(sf::Color::Blue);
  sounds_text->setOutlineThickness(2.f);

  mouse_text = std::make_unique<sf::Text>(*mono_font, "", 20);
  mouse_text->setFillColor(sf::Color::White);
  mouse_text->setOutlineColor(sf::Color::Black);
  mouse_text->setOutlineThickness(2.f);

  player = std::make_unique<Player>();

  const unsigned tiles[] = {0, 4, 8,  12, 1, 5, 9,  13,
                            2, 6, 10, 14, 3, 7, 11, 15};
  tilemap.load(tileset, tiles, {100, 100}, {4, 4});
}

// State lifetime
void GameState::enter() {
  DEBUG("entering GameState");
  loadResources();
  loadAssets();
  onKeyReleased_cg = eventHandler.bind<sf::Event::KeyReleased>(
      &GameState::onKeyReleased, this);
  music->play();
}

void GameState::exit() {
  DEBUG("exiting GameState");
  music->stop();
  onKeyReleased_cg.disconnect();
}

// Functionality
void GameState::update(sf::Time dt) {
  player->update(dt);
  auto view = window.getView();
  view.setCenter(player->getPosition());
  window.setView(view);

  auto top_left = window.mapPixelToCoords({0, 0});
  sounds_text->setPosition(top_left);
  background.update(dt);

  auto pos_window = sf::Mouse::getPosition(window);
  auto pos_view = window.mapPixelToCoords(pos_window);
  mouse_text->setPosition(pos_view);
  std::stringstream ss;
  ss << std::setw(7) << pos_window.x << "," << std::setw(7) << pos_window.y
     << "\n"
     << std::setw(7) << std::setprecision(5) << pos_view.x << ","
     << std::setw(7) << std::setprecision(5) << pos_view.y;
  mouse_text->setString(ss.str());

  sounds_text->setString("things: " +
                         std::to_string(audioManager.playingSounds()));
}

void GameState::render() {
  window.draw(background);
  window.draw(tilemap);
  window.draw(*player);
  window.draw(*main_text);
  window.draw(*sounds_text);
}

// Listeners
void GameState::onKeyReleased(sf::Event::KeyReleased keyReleased) {
  if (keyReleased.code == keybinds["QUIT"]) {
    next_state = new MainMenuState{};
  }
}
