#pragma once

#include <cassert>
#include <map>
#include <memory>
#include <string>
#include <typeindex>
#include <utility>

#include "Core/Logger.h"

class ResourceManager {
 private:
  using resources_t =
      std::map<std::pair<std::type_index, std::string>, std::shared_ptr<void>>;

 public:
  template <typename Resource, typename... Args>
  std::shared_ptr<Resource> load(std::string name, Args&&... args) {
    auto it = resources.find({std::type_index(typeid(Resource)), name});
    if (it != resources.end()) {
      SDEBUG(" ", "cached", name);
      return {it->second, reinterpret_cast<Resource*>(it->second.get())};
    }
    SDEBUG(" ", "loaded", name);
    return std::make_shared<Resource>(std::forward<Args>(args)...);
  }

  template <typename Resource, typename... Args>
  std::shared_ptr<Resource> retain(std::string name, Args&&... args) {
    auto key = std::make_pair(std::type_index(typeid(Resource)), name);
    auto it = resources.find(key);
    if (it != resources.end()) {
      SDEBUG(" ", "cached", name);
      return {it->second, reinterpret_cast<Resource*>(it->second.get())};
    }
    SDEBUG(" ", "loaded", name);
    auto res = std::make_shared<Resource>(std::forward<Args>(args)...);
    resources[key] = res;
    return res;
  }

  template <typename Resource>
  void release(std::string name) {
    resources.erase({std::type_index(typeid(Resource)), name});
    SDEBUG(" ", "released", name);
  }

 private:
  resources_t resources;
};
