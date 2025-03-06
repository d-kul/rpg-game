#pragma once

#include <cassert>
#include <filesystem>
#include <map>
#include <memory>
#include <utility>

// #include "Core/Logger.h"

class ResourceManager {
 public:
  struct str_key_t {};
 private:
  using resources_t = std::map<std::string, std::shared_ptr<void>>;

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
  std::shared_ptr<Resource> load(str_key_t, const std::string& key,
                                 Args&&... args) {
    auto it = resources.find(key);
    if (it != resources.end()) {
      return {it->second, reinterpret_cast<Resource*>(it->second.get())};
    }
    return std::make_shared<Resource>(std::forward<Args>(args)...);
  }

  template <typename Resource, typename... Args>
  std::shared_ptr<Resource> retain(const std::filesystem::path& filename,
                                   Args&&... args) {
    auto res = load<Resource>(filename, std::forward<Args>(args)...);
    resources.insert({filename, res});
    return res;
  }

  template <typename Resource, typename... Args>
  std::shared_ptr<Resource> retain(str_key_t, const std::string& key,
                                 Args&&... args) {
    auto res = load<Resource>(str_key_t{}, key, std::forward<Args>(args)...);
    resources.insert({key, res});
    return res;
  }

  template <typename Resource>
  void release(const std::string& key) {
    resources.erase(key);
    // SDEBUG(" ", "released", filename);
  }

 private:
  resources_t resources;
};

constexpr ResourceManager::str_key_t str_key;
