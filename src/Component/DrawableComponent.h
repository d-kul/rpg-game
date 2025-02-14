#pragma once

#include <cstddef>

class DrawableComponent {
 public:
  DrawableComponent();

  friend bool operator<(const DrawableComponent&, const DrawableComponent&);

 private:
  std::size_t getIndex();
  std::size_t index;
};
