#pragma once

#include <SFML/Graphics/Text.hpp>

#include "Action.h"
#include "Level.h"

class TextAction : public Action {
 public:
  TextAction(Level& level,
             std::string text)
      : level(level),
        text(std::move(text)) {}

  void start() override {
    level.setTextFrameActive(true);
    level.textFrameText->setString(text);
    level.textClosed = false;
  }

  bool update(sf::Time dt) override { return !level.textClosed; }

  void end() override {
    level.setTextFrameActive(false);
    level.textFrameText->setString("");
  }

 private:
  Level& level;
  std::string text;
};
