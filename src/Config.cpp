#include "Config.h"

#include <fstream>

Config Config::read(const std::filesystem::path& filename) {
  return Config{filename};
}

Config::Config(const std::filesystem::path& filename) {
  std::ifstream stream(filename);
  std::string line;
  while (std::getline(stream, line)) {
    std::istringstream is_line(std::move(line));
    std::string key;
    if (std::getline(is_line, key, '=')) {
      std::string value;
      if (std::getline(is_line, value)) config.insert({key, value});
    }
  }
}

Config::map_t::iterator Config::begin() { return config.begin(); }

Config::map_t::iterator Config::end() { return config.end(); }

template <>
std::optional<std::string> Config::get<std::string>(
    const std::string& key) const {
  if (auto it = config.find(key); it != config.end()) {
    return it->second;
  }
  return std::nullopt;
}

template <>
std::optional<int> Config::get<int>(const std::string& key) const {
  if (auto it = config.find(key); it != config.end()) {
    try {
      return std::stoi(it->second);
    } catch (...) {
      return std::nullopt;
    }
  }
  return std::nullopt;
}

template <>
std::optional<long> Config::get<long>(const std::string& key) const {
  if (auto it = config.find(key); it != config.end()) {
    try {
      return std::stol(it->second);
    } catch (...) {
      return std::nullopt;
    }
  }
  return std::nullopt;
}

template <>
std::optional<long long> Config::get<long long>(const std::string& key) const {
  if (auto it = config.find(key); it != config.end()) {
    try {
      return std::stoll(it->second);
    } catch (...) {
      return std::nullopt;
    }
  }
  return std::nullopt;
}

template <>
std::optional<unsigned> Config::get<unsigned>(const std::string& key) const {
  if (auto it = config.find(key); it != config.end()) {
    try {
      return std::stoul(it->second);
    } catch (...) {
      return std::nullopt;
    }
  }
  return std::nullopt;
}

template <>
std::optional<unsigned long> Config::get<unsigned long>(
    const std::string& key) const {
  if (auto it = config.find(key); it != config.end()) {
    try {
      return std::stoul(it->second);
    } catch (...) {
      return std::nullopt;
    }
  }
  return std::nullopt;
}

template <>
std::optional<unsigned long long> Config::get<unsigned long long>(
    const std::string& key) const {
  if (auto it = config.find(key); it != config.end()) {
    try {
      return std::stoull(it->second);
    } catch (...) {
      return std::nullopt;
    }
  }
  return std::nullopt;
}

template <>
std::optional<float> Config::get<float>(const std::string& key) const {
  if (auto it = config.find(key); it != config.end()) {
    try {
      return std::stof(it->second);
    } catch (...) {
      return std::nullopt;
    }
  }
  return std::nullopt;
}

template <>
std::optional<double> Config::get<double>(const std::string& key) const {
  if (auto it = config.find(key); it != config.end()) {
    try {
      return std::stod(it->second);
    } catch (...) {
      return std::nullopt;
    }
  }
  return std::nullopt;
}

template <>
std::optional<long double> Config::get<long double>(
    const std::string& key) const {
  if (auto it = config.find(key); it != config.end()) {
    try {
      return std::stold(it->second);
    } catch (...) {
      return std::nullopt;
    }
  }
  return std::nullopt;
}
