#include "MainMenu.h"

#include <memory>

#include "../Game.h"
#include "Core/Logger.h"
#include "State/Game.h"
#include "UI/Button.h"
#include "UI/Frame.h"
#include "UI/Text.h"

// State lifetime
void MainMenuState::enter() {
  DEBUG("entering MainMenuState");
  window.setView(
      sf::View{sf::Vector2f{windowSize} / 2.f, sf::Vector2f{windowSize}});
  auto background_texture =
      resourceManager.retain<sf::Texture>("resources/images/pearto.png");
  background.setTexture(background_texture.get(), sf::Vector2f{windowSize});
  background.setView(window.getView());
  initUI();
}

void MainMenuState::exit() { DEBUG("exiting MainMenuState"); }

// Functionality
void MainMenuState::update(sf::Time dt) {}

void MainMenuState::render() { window.draw(background); }

void MainMenuState::initUI() {
  auto font = resourceManager.retain<sf::Font>("resources/fonts/papyrus.ttf");
  auto mono_font =
      resourceManager.retain<sf::Font>("resources/fonts/DroidSansMono.ttf");

  auto main_frame = std::make_unique<Frame>();

  {
    auto title_text = std::make_unique<Text>(*font, "", 40);
    auto& text = title_text->text;
    text.setPosition({20.f, 20.f});
    text.setFillColor(sf::Color::Black);

    auto& keys = Game::key_storage;
    std::stringstream ss;
    ss << keys.at(keybinds["QUIT"]) << " - exit to menu\n"
       << keys.at(keybinds["MOVE_UP"]) << keys.at(keybinds["MOVE_LEFT"])
       << keys.at(keybinds["MOVE_DOWN"]) << keys.at(keybinds["MOVE_RIGHT"])
       << " - move\n"
       << keys.at(keybinds["INTERACT"]) << " - interact";
    text.setString(ss.str());
    main_frame->addChild(std::move(title_text));
  }

  auto screen_middle = sf::Vector2f{windowSize} * 0.5f;

  {
    auto start_button = std::make_unique<Button>(
        *mono_font, sf::Vector2f{200.f, 70.f}, "Start game");
    start_button->setOrigin(start_button->shape.getGlobalBounds().getCenter());
    start_button->setPosition(screen_middle + sf::Vector2f{0.f, -80.f});
    start_button->onClick().subscribe(
        [this]() { next_state = new GameState{}; });
    main_frame->addChild(std::move(start_button));
  }

  {
    auto settings_button = std::make_unique<Button>(
        *mono_font, sf::Vector2f{200.f, 70.f}, "Settings");
    settings_button->setOrigin(
        settings_button->shape.getGlobalBounds().getCenter());
    settings_button->setPosition(screen_middle + sf::Vector2f{0.f, 0.f});
    main_frame->addChild(std::move(settings_button));
  }

  {
    auto quit_button =
        std::make_unique<Button>(*mono_font, sf::Vector2f{200.f, 70.f}, "Quit");
    quit_button->setOrigin(quit_button->shape.getGlobalBounds().getCenter());
    quit_button->setPosition(screen_middle + sf::Vector2f{0.f, 80.f});
    quit_button->onClick().subscribe([this]() { window.close(); });
    main_frame->addChild(std::move(quit_button));
  }

  uiManager.states["menu"] = std::move(main_frame);
  uiManager.setActiveState("menu");
}
