#include "MainMenuState.h"

#include <SFML/Window/Mouse.hpp>
#include <memory>
#include <sstream>

#include "Core/Logger.h"
#include "Game.h"
#include "State/GameState.h"
#include "UI/Button.h"
#include "UI/Combo.h"
#include "UI/Frame.h"
#include "UI/Text.h"

MainMenuState::MainMenuState(Game& game) : State(game) {}

// State lifetime
void MainMenuState::enter() {
  DEBUG("entering MainMenuState");
  game.window.setView(sf::View{sf::Vector2f{game.videoMode.size} / 2.f,
                               sf::Vector2f{game.videoMode.size}});
  auto background_texture =
      game.resourceManager.retain<sf::Texture>("resources/images/pearto.png");
  background.setTexture(background_texture.get(),
                        sf::Vector2f{game.videoMode.size});
  background.setView(game.window.getView());
  initUI();
}

void MainMenuState::exit() { DEBUG("exiting MainMenuState"); }

// Functionality
void MainMenuState::update(sf::Time dt) {}

void MainMenuState::render() { game.window.draw(background); }

void MainMenuState::initUI() {
  auto font =
      game.resourceManager.retain<sf::Font>("resources/fonts/papyrus.ttf");
  auto mono_font = game.resourceManager.retain<sf::Font>(
      "resources/fonts/DroidSansMono.ttf");
  auto screen_middle = sf::Vector2f{game.videoMode.size} / 2.f;
  auto button_size = sf::Vector2f{200.f, 50.f};
  auto text_size = 25;
  auto interval = 10.f;

  constexpr auto style_button = [](Button& button) {
    button.shape.setOutlineThickness(2.f);
    button.shape.setOutlineColor(sf::Color::Black);
  };

  auto main_frame = std::make_unique<Frame>();

  {
    auto title_text = std::make_unique<Text>(*font, "", 40);
    auto& text = title_text->text;
    text.setPosition({20.f, 20.f});
    text.setFillColor(sf::Color::Black);

    auto& keys = Game::key_storage;
    std::stringstream ss;
    ss << keys.at(game.keybinds["QUIT"]) << " - exit to menu\n"
       << keys.at(game.keybinds["MOVE_UP"])
       << keys.at(game.keybinds["MOVE_LEFT"])
       << keys.at(game.keybinds["MOVE_DOWN"])
       << keys.at(game.keybinds["MOVE_RIGHT"]) << " - move\n"
       << keys.at(game.keybinds["INTERACT"]) << " - interact";
    text.setString(ss.str());
    main_frame->addChild(std::move(title_text));
  }

  {
    auto middle_frame = std::make_unique<Frame>();
    mainMiddle = middle_frame.get();
    middle_frame->setPosition(screen_middle);
    {
      auto start_button = std::make_unique<Button>(*mono_font, button_size,
                                                   "Start game", text_size);
      start_button->setOrigin(button_size / 2.f);
      start_button->setPosition(sf::Vector2f{0.f, -button_size.y - interval});
      start_button->onClick().subscribe(
          [this]() { next_state = new GameState{game}; });
      style_button(*start_button);
      middle_frame->addChild(std::move(start_button));
    }

    {
      auto settings_button = std::make_unique<Button>(*mono_font, button_size,
                                                      "Settings", text_size);
      settings_button->setOrigin(button_size / 2.f);
      settings_button->setPosition(sf::Vector2f{0.f, 0.f});
      settings_button->onClick().subscribe(
          [this]() { game.uiManager.setActiveState("settings"); });
      style_button(*settings_button);
      middle_frame->addChild(std::move(settings_button));
    }

    {
      auto quit_button =
          std::make_unique<Button>(*mono_font, button_size, "Quit", text_size);
      quit_button->setOrigin(button_size / 2.f);
      quit_button->setPosition(sf::Vector2f{0.f, button_size.y + interval});
      quit_button->onClick().subscribe([this]() { game.window.close(); });
      style_button(*quit_button);
      middle_frame->addChild(std::move(quit_button));
    }

    main_frame->addChild(std::move(middle_frame));
  }

  game.uiManager.states["menu"] = std::move(main_frame);

  auto settings_frame = std::make_unique<Frame>();

  {
    auto settings_text = std::make_unique<Text>(*font, "settings", 40);
    auto& text = settings_text->text;
    text.setPosition({20.f, 20.f});
    text.setFillColor(sf::Color::Black);
    settings_frame->addChild(std::move(settings_text));
  }

  {
    auto middle_frame = std::make_unique<Frame>();
    settingsMiddle = middle_frame.get();
    middle_frame->setPosition(screen_middle);
    {
      auto main_button = std::make_unique<Button>(*mono_font, button_size,
                                                  "Main menu", text_size);
      main_button->setOrigin(button_size / 2.f);
      main_button->setPosition(sf::Vector2f{0.f, button_size.y + interval});
      main_button->onClick().subscribe(
          [this]() { game.uiManager.setActiveState("menu"); });
      style_button(*main_button);
      middle_frame->addChild(std::move(main_button));
    }

    {
      auto fullscreen_text =
          std::make_unique<Text>(*mono_font, "Fullscreen:", text_size);
      auto& text = fullscreen_text->text;
      text.setOrigin(
          {text.getLocalBounds().size.x, text.getLocalBounds().getCenter().y});
      text.setPosition({-interval / 2.f, -button_size.y - interval});
      text.setFillColor(sf::Color::Black);
      middle_frame->addChild(std::move(fullscreen_text));
    }

    {
      bool fullscreen = game.fullscreen;
      auto fullscreen_button = std::make_unique<Button>(
          *mono_font, sf::Vector2f{button_size.y, button_size.y}, "x",
          text_size);
      fullscreen_button->text.setString(fullscreen ? "x" : "");
      fullscreen_button->setOrigin(sf::Vector2f{0.f, button_size.y / 2.f});
      fullscreen_button->setPosition(
          {interval / 2.f, -button_size.y - interval});
      fullscreen_button->onClick().subscribe(
          [&game = this->game, &fullscreen = game.fullscreen,
           fullscreen_button = fullscreen_button.get()]() {
            game.setFullscreen(!fullscreen);
            fullscreen_button->text.setString(fullscreen ? "x" : "");
          });
      style_button(*fullscreen_button);
      middle_frame->addChild(std::move(fullscreen_button));
    }

    {
      auto resolution_text =
          std::make_unique<Text>(*mono_font, "Resolution:", text_size);
      auto& text = resolution_text->text;
      text.setOrigin(
          {text.getLocalBounds().size.x, text.getLocalBounds().getCenter().y});
      text.setPosition({-interval / 2.f, 0});
      text.setFillColor(sf::Color::Black);
      middle_frame->addChild(std::move(resolution_text));
    }

    {
      std::vector<sf::String> options;
      bool first = true;
      unsigned bpp;
      for (auto& videoMode : sf::VideoMode::getFullscreenModes()) {
        if (first) {
          first = false;
          bpp = videoMode.bitsPerPixel;
        } else if (bpp != videoMode.bitsPerPixel) {
          break;
        }
        options.emplace_back(std::to_string(videoMode.size.x) + "x" +
                             std::to_string(videoMode.size.y));
      }
      DEBUG("options size: ", options.size());

      auto resolution_combo = std::make_unique<Combo>(
          *mono_font, button_size, text_size, std::move(options), -1, 6);
      resolution_combo->setOrigin({0.f, button_size.y / 2.f});
      resolution_combo->setPosition(sf::Vector2f{interval / 2.f, 0});
      resolution_combo->onSelect().subscribe([&](int optionIdx) {
        game.videoMode = sf::VideoMode::getFullscreenModes()[optionIdx];
        game.updateSize();
        if (game.fullscreen) game.createWindow();
        resizeUpdateUI();
      });
      auto& combo_button =
          static_cast<Button&>(*resolution_combo->getChildren().back());
      style_button(combo_button);
      combo_button.text.setString(std::to_string(game.videoMode.size.x) + "x" +
                                  std::to_string(game.videoMode.size.y));
      combo_button.text.setOrigin(
          combo_button.text.getLocalBounds().getCenter());
      for (auto& child : *resolution_combo->popup.options) {
        style_button(static_cast<Button&>(*child));
      }
      middle_frame->addChild(std::move(resolution_combo));
    }
    settings_frame->addChild(std::move(middle_frame));
  }

  game.uiManager.states["settings"] = std::move(settings_frame);

  game.uiManager.setActiveState("menu");
}

void MainMenuState::resizeUpdateUI() {
  background.setTextureSize(sf::Vector2f{game.videoMode.size});
  background.setView(game.window.getView());
  auto screen_middle = sf::Vector2f{game.videoMode.size} / 2.f;
  assert(mainMiddle && settingsMiddle);
  mainMiddle->setPosition(screen_middle);
  settingsMiddle->setPosition(screen_middle);
}
