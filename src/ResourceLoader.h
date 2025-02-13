#pragma once

#include <entt/entt.hpp>
#include <memory>

template <typename Resource>
class ResourceLoader final {
 public:
  using result_type = std::shared_ptr<Resource>;

  template <typename... Args>
  result_type operator()(Args&&... args) const {
    return std::make_shared<Resource>(std::forward<Args>(args)...);
  }
};

template <typename Resource>
using ResourceCache = entt::resource_cache<Resource, ResourceLoader<Resource>>;
