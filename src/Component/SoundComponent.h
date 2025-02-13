#pragma once

#include <SFML/Audio/Sound.hpp>

class SoundComponent : public sf::Sound {
 public:
  static constexpr auto in_place_delete = true;
  using sf::Sound::Sound;
};
