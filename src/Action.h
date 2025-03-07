#pragma once

#include <SFML/System/Time.hpp>

class Action {
 public:
  Action() = default;
  virtual ~Action() {}
  virtual void start() {}
  virtual bool update(sf::Time dt) { return false; }
  virtual void end() {}
  void setNext(Action* next) { this->next = next; }

  static void update(Action*& action, sf::Time dt);

 private:
  Action* next = nullptr;
};
