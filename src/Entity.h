#pragma once

#include <entt/entity/fwd.hpp>
#include <entt/entt.hpp>
#include <tuple>

template <typename... Types>
class Entity {
 private:
  using tuple_t = std::tuple<Types&...>;

 public:
  Entity() = default;

  Entity(const Entity& other)
      : entity_(other.registry_->create()),
        registry_(other.registry_),
        refs(other.registry_->template emplace<Types>(other.get<Types>())...) {}

  Entity(Entity&& other)
      : entity_(other.entity_), registry_(other.registry_), refs(other.refs) {
    other.clear();
  }

  Entity& operator=(const Entity& other) {
    registry_ = other.registry_;
    entity_ = registry_->create();
    refs = tuple_t(
        other.registry_->template emplace<Types>(other.get<Types>())...);
  }

  Entity& operator=(Entity&& other) {
    entity_ = other.entity_;
    registry_ = other.registry_;
    refs = other.refs;
    other.clear();
  }

  Entity(entt::registry& registry, const Types&... args)
      : entity_(registry.create()),
        registry_(&registry),
        refs(registry.emplace<Types>(args)...) {}

  template <typename... UTypes>
  Entity(entt::registry& registry, UTypes&&... args)
      : entity_(registry.create()),
        registry_(&registry),
        refs(registry.emplace<Types>(std::forward<UTypes>(args))...) {}

  entt::entity entity() { return entity_; }

  entt::registry* registry() { return registry_; }

  template <typename T>
  T& get() {
    return std::get<T>(refs);
  }

  template <typename T>
  const T& get() const {
    return std::get<T>(refs);
  }

  ~Entity() { clear(); }

  void swap(Entity other) {
    using std::swap;
    swap(entity_, other.entity_);
    swap(registry_, other.registry_);
    swap(refs, other.refs);
  }

 private:
  void clear() {
    if (registry_) registry_->destroy(entity_);
    entity_ = entt::null;
  }

  entt::entity entity_ = entt::null;
  entt::registry* registry_ = nullptr;
  tuple_t refs;
};
