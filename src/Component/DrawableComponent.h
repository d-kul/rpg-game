#pragma once

#include <cstddef>

struct DrawableComponent {
  DrawableComponent();

 private:
  std::size_t getIndex();
  std::size_t index;
  friend bool operator<(const DrawableComponent&, const DrawableComponent&);
};
