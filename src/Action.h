#pragma once

#include <SFML/System/Time.hpp>

class Action {
 public:
  Action() = default;
  virtual ~Action() {}
  virtual void start() {}
  virtual bool update(sf::Time dt) { return false; }
  virtual void end() {}

  static void update(Action*& action, sf::Time dt);

 private:
  Action* next = nullptr;
};
