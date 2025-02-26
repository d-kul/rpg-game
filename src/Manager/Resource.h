#pragma once

#include <cassert>
#include <filesystem>
#include <map>
#include <memory>
#include <utility>

// #include "Core/Logger.h"

class ResourceManager {
 private:
  using resources_t = std::map<std::filesystem::path, std::shared_ptr<void>>;

 public:
  template <typename Resource, typename... Args>
  std::shared_ptr<Resource> load(const std::filesystem::path& filename,
                                 Args&&... args) {
    auto it = resources.find(filename);
    if (it != resources.end()) {
      // SDEBUG(" ", "cached", filename);
      return {it->second, reinterpret_cast<Resource*>(it->second.get())};
    }
    // SDEBUG(" ", "loaded", filename);
    return std::make_shared<Resource>(filename, std::forward<Args>(args)...);
  }

  template <typename Resource, typename... Args>
  std::shared_ptr<Resource> retain(const std::filesystem::path& filename,
                                   Args&&... args) {
    auto res = load<Resource>(filename, std::forward<Args>(args)...);
    resources.insert({filename, res});
    return res;
  }

  template <typename Resource>
  void release(const std::filesystem::path& filename) {
    resources.erase(filename);
    // SDEBUG(" ", "released", filename);
  }

 private:
  resources_t resources;
};
