#pragma once

#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <list>
#include <memory>

#include "Entity.h"
#include "State.h"

class GameState : public State {
 private:
  // Initialization
  void initAssets();
  void initKeybinds();

 public:
  // Constructors, destructor
  GameState(std::map<std::string, sf::Keyboard::Key>& keybinds);
  virtual ~GameState();

  // Functionality
  void endState() override;
  void onEvent(const sf::Event& event) override;
  void updateInput(sf::Time dt);
  void update(sf::Time dt) override;
  void render(sf::RenderTarget& target) override;

 private:
  // Members
  std::unique_ptr<Entity> entity;

  sf::Texture background_texture{"resources/test.jpg", false,
                                 sf::IntRect({0, 2}, {190, 162})};
  sf::Sprite background_sprite{background_texture};
  sf::Font font{"resources/papyrus.ttf"};
  sf::Text text{font, "game design is my passion", 40};
  sf::Music music{"resources/teto scatman.ogg"};
  sf::SoundBuffer sound_buffer{"resources/vine_boom.wav"};
  std::list<sf::Sound> sounds;
  sf::Text soundsText{font};
  sf::Clock clock;
  static constexpr sf::Time PRESS_FREQ = sf::milliseconds(70);
  static constexpr size_t MAX_SOUNDS = 2000u;
};
