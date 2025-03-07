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
  dialogFont =
      game.resourceManager.retain<sf::Font>("resources/fonts/ComicMono.ttf");
  menuFont = game.resourceManager.retain<sf::Font>(
      "resources/fonts/DroidSansMono.ttf");
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
  initUI();
  loadLevel();
}

void GameState::exit() { DEBUG("exiting GameState"); }

// Functionality
void GameState::update(sf::Time dt) {
  level.update(dt);
  if (level.nextLevel) {
    level.loadFromFile(*level.nextLevel);
    level.nextLevel.reset();
  }
}

void GameState::render() { level.render(game.window); }

void GameState::initUI() {
  auto frame = std::make_unique<Frame>();
  mainFrame = frame.get();
  initImageFrame();
  initTextFrame();
  initMenuFrame();
  game.uiManager.states["main"] = std::move(frame);
  game.uiManager.setActiveState("main");
}

void GameState::initImageFrame() {
  auto frame = std::make_unique<Frame>();
  frame->setActive(false);
  level.imageFrame = frame.get();
  auto& shape = frame->shape;
  shape.setPosition(sf::Vector2f{game.videoMode.size} / 2.f);
  level.imageFrameShape = &shape;

  mainFrame->addChild(std::move(frame));
}

void GameState::initTextFrame() {
  auto frame = std::make_unique<Frame>();
  frame->setActive(false);
  level.textFrame = frame.get();
  frame->setPosition(sf::Vector2f{game.videoMode.size} / 2.f);
  float padding = 20.f;

  auto& rect = frame->shape;
  rect.setSize(
      {std::min<float>(game.videoMode.size.x - 2 * padding, 800), 230});
  rect.setFillColor(sf::Color(0, 0, 0, 200));
  rect.setOutlineThickness(6.f);
  rect.setOutlineColor(sf::Color::White);

  {
    auto text = std::make_unique<Text>(*dialogFont, "", 30);
    text->setPosition({padding, padding});
    level.textFrameText = &text->text;
    frame->addChild(std::move(text));
  }

  {
    auto arrow = std::make_unique<Frame>();
    auto texture = game.resourceManager.retain<sf::Texture>(
        "resources/images/down_triangle.png");
    arrow->shape.setTexture(texture.get());
    arrow->shape.setSize(sf::Vector2f{texture->getSize()});
    arrow->setPosition(rect.getSize() - arrow->shape.getSize() -
                       sf::Vector2f{padding, padding});
    frame->addChild(std::move(arrow));
  }

  frame->setPosition({(game.videoMode.size.x - rect.getSize().x) / 2.f,
                      game.videoMode.size.y - rect.getSize().y - padding});

  mainFrame->addChild(std::move(frame));
}

void GameState::initMenuFrame() {
  auto frame = std::make_unique<Frame>();
  frame->setActive(false);
  level.menuFrame = frame.get();
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
    auto text = std::make_unique<Text>(*menuFont, "Pause", text_size);
    text->setOrigin(text->text.getGlobalBounds().getCenter());
    text->setPosition({0, -button_size.y - interval});
    frame->addChild(std::move(text));
  }

  {
    auto button =
        std::make_unique<Button>(*menuFont, button_size, "Close", text_size);
    style_button(*button);
    button->setOrigin(button->shape.getGeometricCenter());
    button->setPosition({0, 0});
    button->onClick().subscribe([this]() { level.setMenuFrameActive(false); });
    frame->addChild(std::move(button));
  }

  {
    auto button = std::make_unique<Button>(*menuFont, button_size,
                                           "Exit to menu", text_size);
    style_button(*button);
    button->setOrigin(button->shape.getGeometricCenter());
    button->setPosition({0, button_size.y + interval});
    button->onClick().subscribe(
        [this]() { next_state = new MainMenuState{game}; });
    frame->addChild(std::move(button));
  }

  mainFrame->addChild(std::move(frame));
}
