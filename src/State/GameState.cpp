#include "GameState.h"

#include <iostream>
#include <sstream>

#include "State/MainMenuState.h"

// Lifetime management
void GameState::loadResources() {
  background_texture = resourceManager.retain<sf::Texture>(
      "textures/space_bg_fumo", "resources/images/space_bg_fumo.png");
  background_texture->setRepeated(true);
  pearto_texture = resourceManager.retain<sf::Texture>(
      "textures/pearto", "resources/images/pearto.png");
  font = resourceManager.retain<sf::Font>("fonts/papyrus",
                                          "resources/fonts/papyrus.ttf");
  mono_font = resourceManager.retain<sf::Font>(
      "fonts/DroidSansMono", "resources/fonts/DroidSansMono.ttf");
  music = resourceManager.retain<sf::Music>(
      "music/Teto Kasane Teto", "resources/music/Teto Kasane Teto.ogg");
  music->setLooping(true);
  tileset = resourceManager.retain<TileSet>("textures/comic mono", 32,
                                            "resources/images/comic mono.png");
}

static const short tiles[] = {
    -1, 4, 8,  12,  //
    1,  5, 9,  13,  //
    2,  6, 10, 14,  //
    3,  7, 11, 15   //
};

static const bool collision[] = {
    1, 1, 1, 1,  //
    1, 0, 0, 1,  //
    1, 0, 0, 0,  //
    1, 1, 1, 1   //
};

void GameState::loadAssets() {
  float tileSize = 64.f;

  background.setMoving(true);
  background.setSize(sf::Vector2f{windowSize});
  background.setTexture(background_texture.get(),
                        sf::Vector2f{background_texture->getSize()} / 2.f,
                        true);

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

  player = std::make_unique<Player>(tileSize);
  player->setPosition(sf::Vector2f{1, 1} * tileSize);

  tilemap.load(tileset.get(), tiles, tileSize, {4, 4});
  tilemap_collider.load(collision, tileSize, {4, 4});

  test_entity = std::make_unique<Interactible>(sf::Vector2i{5, 5}, tileSize);
  auto test_shape = std::make_unique<sf::RectangleShape>();
  test_shape->setSize({tileSize, tileSize});
  test_shape->setTexture(pearto_texture.get());
  test_shape->setOrigin({tileSize / 2, tileSize / 2});
  test_shape->setPosition({tileSize / 2, tileSize / 2});
  auto test_shape_ptr = test_shape.get();
  test_entity->drawable = std::move(test_shape);
  test_entity_collider.setRect(
      {test_entity->getPosition(), {tileSize, tileSize}});

  auto& vine_boom = *resourceManager.retain<sf::SoundBuffer>(
      "sounds/vine_boom", "resources/sounds/vine_boom.wav");

  test_entity->action = [this, vine_boom, test_shape_ptr]() {
    audioManager.playSound(vine_boom);
    test_shape_ptr->scale({0.99f, 0.99f});
  };
}

// State lifetime
void GameState::enter() {
  DEBUG("entering GameState");
  loadResources();
  loadAssets();
  onKeyReleased_cg = eventManager.bind<sf::Event::KeyReleased>(
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
  window.draw(*test_entity);
  window.draw(*main_text);
  window.draw(*sounds_text);
}

// Listeners
void GameState::onKeyReleased(sf::Event::KeyReleased keyReleased) {
  if (keyReleased.code == keybinds["QUIT"]) {
    next_state = new MainMenuState{};
  }
}
