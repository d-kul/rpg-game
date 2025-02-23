#include "Game.h"

#include "State/MainMenu.h"

// Lifetime management
void GameState::loadResources() {
  pearto_texture =
      resourceManager.retain<sf::Texture>("resources/images/pearto.png");
  font = resourceManager.retain<sf::Font>("resources/fonts/papyrus.ttf");
  mono_font =
      resourceManager.retain<sf::Font>("resources/fonts/DroidSansMono.ttf");
  music =
      resourceManager.retain<sf::Music>("resources/music/Teto Kasane Teto.ogg");
  music->setLooping(true);
}

void GameState::loadAssets() {
  level.loadFromFile("resources/data/levels/level.txt");

  float tileSize = 64.f;
  auto test_entity =
      std::make_unique<Interactible>(sf::Vector2i{3, 3}, tileSize);

  auto test_shape = std::make_unique<sf::RectangleShape>();
  test_shape->setSize({tileSize, tileSize});
  test_shape->setTexture(pearto_texture.get());
  test_shape->setOrigin({tileSize / 2, tileSize / 2});
  test_shape->setPosition({tileSize / 2, tileSize / 2});

  auto test_shape_ptr = test_shape.get();
  test_entity->drawable = std::move(test_shape);

  auto test_entity_collider = std::make_unique<RectCollider>();
  test_entity_collider->setRect(
      {test_entity->getPosition(), {tileSize, tileSize}});

  auto& vine_boom = *resourceManager.retain<sf::SoundBuffer>(
      "resources/sounds/vine_boom.wav");

  test_entity->action = [this, vine_boom, test_shape_ptr]() {
    audioManager.playSound(vine_boom);
    test_shape_ptr->scale({0.99f, 0.99f});
    level.loadFromFile("resources/data/levels/test_indoors.txt");
  };

  level.entities.push_back(std::move(test_entity));  // TODO: remove it later
  level.colliders.push_back(
      std::move(test_entity_collider));  // TODO: remove it later
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
  level.update(dt);
}

void GameState::render() { level.render(window); }

// Listeners
void GameState::onKeyReleased(sf::Event::KeyReleased keyReleased) {
  if (keyReleased.code == keybinds["QUIT"]) {
    next_state = new MainMenuState{};
  }
}
