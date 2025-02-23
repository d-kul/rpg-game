#include "Game.h"

#include <memory>

#include "Entity/RectCollider.h"
#include "State/MainMenu.h"
#include "UI/Button.h"
#include "UI/Frame.h"
#include "UI/Text.h"

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
  initUI();
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
  if (!uiManager.hasActiveState()) level.update(dt);
}

void GameState::render() { level.render(window); }

// Listeners
void GameState::onKeyReleased(sf::Event::KeyReleased keyReleased) {
  if (keyReleased.code == keybinds["QUIT"]) {
    music->pause();
    uiManager.setActiveState("menu");
  }
}

void GameState::initUI() {
  auto main_frame = std::make_unique<Frame>();

  auto& rect = main_frame->shape;
  rect.setSize({600, 400});
  rect.setFillColor(sf::Color(0, 0, 0, 180));
  rect.setOutlineThickness(6.f);
  rect.setOutlineColor(sf::Color::White);

  main_frame->setPosition({100, 100});

  {
    auto button = std::make_unique<Button>(*mono_font, sf::Vector2f{200, 30},
                                           "Close", 15);
    button->setOrigin(button->shape.getGeometricCenter());
    button->setPosition(rect.getGeometricCenter());
    button->move({0, -20});
    button->onClick().subscribe([this]() {
      music->play();
      uiManager.unsetActiveState();
    });
    main_frame->addChild(std::move(button));
  }

  {
    auto button = std::make_unique<Button>(*mono_font, sf::Vector2f{200, 30},
                                           "Exit to menu", 15);
    button->setOrigin(button->shape.getGeometricCenter());
    button->setPosition(rect.getGeometricCenter());
    button->move({0, +20});
    button->onClick().subscribe([this]() { next_state = new MainMenuState{}; });
    main_frame->addChild(std::move(button));
  }

  uiManager.states["menu"] = std::move(main_frame);
}
