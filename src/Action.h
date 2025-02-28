#pragma once

#include <SFML/System/Time.hpp>
#include <memory>

class AbstractAction {
 public:
  AbstractAction() {}
  AbstractAction(const AbstractAction &) {}
  AbstractAction(AbstractAction &&) = delete;
  AbstractAction &operator=(const AbstractAction &) = delete;
  AbstractAction &operator=(AbstractAction &&) = delete;

  virtual ~AbstractAction() {}
  virtual void start() {}
  virtual bool update(sf::Time dt) { return false; }
  virtual void end() {}

  static void update(std::unique_ptr<AbstractAction> &action, sf::Time dt);

  std::unique_ptr<AbstractAction> clone() const;

 protected:
  virtual std::unique_ptr<AbstractAction> cloneImpl() const = 0;

 private:
  mutable std::unique_ptr<AbstractAction> next;
};

template <typename T>
class Action : public AbstractAction {
 private:
  std::unique_ptr<AbstractAction> cloneImpl() const override {
    return std::make_unique<T>(static_cast<const T &>(*this));
  }
};
