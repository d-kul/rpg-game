#include "GameState.h"

#include <memory>

#include "Core/Logger.h"
#include "State/MainMenuState.h"
#include "UI/Button.h"
#include "UI/Frame.h"
#include "UI/Text.h"

GameState::GameState(Game& game) : State(game), level(game) {}

// Lifetime management
void GameState::loadResources() {
  font = game.resourceManager.retain<sf::Font>("resources/fonts/ComicMono.ttf");
  mono_font = game.resourceManager.retain<sf::Font>(
      "resources/fonts/DroidSansMono.ttf");
  music = game.resourceManager.retain<sf::Music>(
      "resources/music/Teto Kasane Teto.ogg");
  music->setLooping(true);
}

void GameState::loadLevel() {
  level.loadFromFile(
      "resources/data/levels/level.txt");  // TODO(des): move initial level name
                                           // somewhere else
}

// State lifetime
void GameState::enter() {
  DEBUG("entering GameState");
  loadResources();
  loadLevel();
  initUI();
  onKeyPressed_cg = game.eventManager.bind<sf::Event::KeyPressed>(
      &GameState::onKeyPressed, this);
  onKeyReleased_cg = game.eventManager.bind<sf::Event::KeyReleased>(
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
  if (!game.uiManager.hasActiveState()) level.update(dt);
  if (next_level) {
    level.loadFromFile(*next_level);
    next_level.reset();
  }
}

void GameState::render() { level.render(game.window); }

// Listeners
void GameState::onKeyPressed(sf::Event::KeyPressed keyPressed) {
  if (game.uiManager.hasActiveState() &&
      game.uiManager.getActiveStateName() == "text" &&
      keyPressed.code == game.keybinds["INTERACT"]) {
    game.uiManager.setActiveState();
  }
}

void GameState::onKeyReleased(sf::Event::KeyReleased keyReleased) {
  if (!game.uiManager.hasActiveState() &&
      keyReleased.code == game.keybinds["QUIT"]) {
    music->pause();
    game.uiManager.setActiveState("menu");
  }
}

void GameState::loadNextLevel(const std::filesystem::path& filename) {
  next_level = filename;
}

void GameState::initUI() {
  {
    auto frame = std::make_unique<Frame>();
    frame->setPosition(sf::Vector2f{game.videoMode.size} / 2.f);
    auto button_size = sf::Vector2f{200.f, 50.f};
    auto text_size = 25;
    auto interval = 10.f;

    constexpr auto style_button = [](Button& button) {
      button.shape.setOutlineThickness(2.f);
      button.shape.setOutlineColor(sf::Color::Black);
    };

    auto& rect = frame->shape;
    rect.setSize({600, 400});
    rect.setOrigin(rect.getGeometricCenter());
    rect.setFillColor(sf::Color(0, 0, 0, 200));
    rect.setOutlineThickness(6.f);
    rect.setOutlineColor(sf::Color::White);

    {
      auto text = std::make_unique<Text>(*mono_font, "Pause menu", text_size);
      text->setOrigin(text->text.getGlobalBounds().getCenter());
      text->setPosition({0, -button_size.y - interval});
      frame->addChild(std::move(text));
    }

    {
      auto button =
          std::make_unique<Button>(*mono_font, button_size, "Close", text_size);
      style_button(*button);
      button->setOrigin(button->shape.getGeometricCenter());
      button->setPosition({0, 0});
      button->onClick().subscribe([this]() {
        music->play();
        game.uiManager.setActiveState();
      });
      frame->addChild(std::move(button));
    }

    {
      auto button = std::make_unique<Button>(*mono_font, button_size,
                                             "Exit to menu", text_size);
      style_button(*button);
      button->setOrigin(button->shape.getGeometricCenter());
      button->setPosition({0, button_size.y + interval});
      button->onClick().subscribe(
          [this]() { next_state = new MainMenuState{game}; });
      frame->addChild(std::move(button));
    }

    {
      auto button = std::make_unique<Button>(*mono_font, button_size,
                                             "test text", text_size);
      style_button(*button);
      button->setOrigin(button->shape.getGeometricCenter());
      button->setPosition({0, 2 * button_size.y + 2 * interval});
      button->onClick().subscribe(
          [this]() { game.uiManager.setActiveState("text"); });
      frame->addChild(std::move(button));
    }

    game.uiManager.states["menu"] = std::move(frame);
  }

  {
    auto frame = std::make_unique<Frame>();
    frame->setPosition(sf::Vector2f{game.videoMode.size} / 2.f);
    float padding = 10.f;

    auto& rect = frame->shape;
    rect.setSize(
        {std::min<float>(game.videoMode.size.x - 2 * padding, 800), 230});
    rect.setFillColor(sf::Color(0, 0, 0, 200));
    rect.setOutlineThickness(6.f);
    rect.setOutlineColor(sf::Color::White);

    {
      auto text =
          std::make_unique<Text>(*font,
                                 "This is a test message. If you see this,\n"
                                 "then it means that I forgot to change it.",
                                 30);
      text->setPosition({padding, padding});
      frame->addChild(std::move(text));
    }

    frame->setPosition({(game.videoMode.size.x - rect.getSize().x) / 2.f,
                        game.videoMode.size.y - rect.getSize().y - padding});

    game.uiManager.states["text"] = std::move(frame);
  }
}
