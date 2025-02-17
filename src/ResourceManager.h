#pragma once

#include <cassert>
#include <list>
#include <map>
#include <memory>
#include <string>
#include <typeindex>
#include <utility>

#include "Core/Logger.h"

class ResourceManager {
 private:
  using retained_t = std::list<std::shared_ptr<void>>;
  using resources_t =
      std::map<std::pair<std::type_index, std::string>, std::shared_ptr<void>>;

 public:
  using retained_iter_t = retained_t::iterator;

 public:
  template <typename Resource, typename... Args>
  std::shared_ptr<Resource> load(std::string name, Args&&... args) {
    auto [it, loaded] =
        get_or_emplace<Resource>(std::move(name), std::forward<Args>(args)...);
    DEBUG(loaded ? "loaded" : "cached");
    return {it->second, reinterpret_cast<Resource*>(it->second.get())};
  }

  template <typename Resource, typename... Args>
  std::pair<std::shared_ptr<Resource>, retained_iter_t> retain(std::string name,
                                                               Args&&... args) {
    auto [it, loaded] =
        get_or_emplace<Resource>(std::move(name), std::forward<Args>(args)...);
    DEBUG(loaded ? "loaded" : "cached");
    auto retained_iter = retained.insert(retained.end(), it->second);
    std::shared_ptr<Resource> p = {
        it->second, reinterpret_cast<Resource*>(it->second.get())};
    return {p, retained_iter};
  }

  template <typename Resource, typename... Args>
  std::shared_ptr<Resource> hold(std::string name, Args&&... args) {
    auto [it, loaded] =
        get_or_emplace<Resource>(std::move(name), std::forward<Args>(args)...);
    DEBUG(loaded ? "loaded" : "cached");
    retained.push_back(it->second);
    std::shared_ptr<Resource> p = {
        it->second, reinterpret_cast<Resource*>(it->second.get())};
    return p;
  }

  void release(retained_iter_t it) {
    assert(it != retained.end());
    retained.erase(it);
    DEBUG("released");
  }

 private:
  template <typename Resource, typename... Args>
  std::pair<resources_t::iterator, bool> get_or_emplace(std::string name,
                                                        Args&&... args) {
    bool loaded = false;
    auto key =
        std::make_pair(std::type_index(typeid(Resource)), std::move(name));
    auto it = resources.find(key);
    if (it == resources.end()) {
      loaded = true;
      it =
          resources
              .insert({key,
                       std::make_shared<Resource>(std::forward<Args>(args)...)})
              .first;
    }
    return {it, loaded};
  }

  retained_t retained;
  resources_t resources;
};
