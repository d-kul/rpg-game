#pragma once

#include <SFML/Audio/SoundBuffer.hpp>

#include "../Action.h"

class SoundAction : public Action<SoundAction> {
 public:
  SoundAction(sf::SoundBuffer* buffer = nullptr) : buffer(buffer) {}
  void start() override;

 private:
  sf::SoundBuffer* buffer;
};
