#include "GameState.h"

#include <memory>

#include "Core/Logger.h"
#include "State/MainMenuState.h"
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
  level.loadFromFile(
      "resources/data/levels/level.txt");  // TODO(des): move initial level name
                                           // somewhere else
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
  if (next_level) {
    level.loadFromFile(*next_level);
    next_level.reset();
  }
}

void GameState::render() { level.render(window); }

// Listeners
void GameState::onKeyReleased(sf::Event::KeyReleased keyReleased) {
  if (keyReleased.code == keybinds["QUIT"]) {
    music->pause();
    uiManager.setActiveState("menu");
  }
}

void GameState::loadNextLevel(const std::filesystem::path& filename) {
  next_level = filename;
}

void GameState::initUI() {
  auto main_frame = std::make_unique<Frame>();
  main_frame->setPosition({100, 100});

  auto& rect = main_frame->shape;
  rect.setSize({600, 400});
  rect.setFillColor(sf::Color(0, 0, 0, 180));
  rect.setOutlineThickness(6.f);
  rect.setOutlineColor(sf::Color::White);

  {
    auto text = std::make_unique<Text>(*mono_font, "Pause menu", 20);
    text->setOrigin(text->text.getGlobalBounds().getCenter());
    text->setPosition(rect.getGeometricCenter());
    text->move({0, -40});
    main_frame->addChild(std::move(text));
  }

  {
    auto button = std::make_unique<Button>(*mono_font, sf::Vector2f{200, 30},
                                           "Close", 15);
    button->setOrigin(button->shape.getGeometricCenter());
    button->setPosition(rect.getGeometricCenter());
    button->move({0, 0});
    button->onClick().subscribe([this]() {
      music->play();
      uiManager.setActiveState();
    });
    main_frame->addChild(std::move(button));
  }

  {
    auto button = std::make_unique<Button>(*mono_font, sf::Vector2f{200, 30},
                                           "Exit to menu", 15);
    button->setOrigin(button->shape.getGeometricCenter());
    button->setPosition(rect.getGeometricCenter());
    button->move({0, +40});
    button->onClick().subscribe([this]() { next_state = new MainMenuState{}; });
    main_frame->addChild(std::move(button));
  }

  uiManager.states["menu"] = std::move(main_frame);
}
