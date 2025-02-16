#pragma once

#include <filesystem>
#include <map>
#include <optional>
#include <string>

class Config {
 private:
  using map_t = std::map<std::string, std::string>;

 public:
  static Config read(const std::filesystem::path& filename);

  Config(const std::filesystem::path& filename);
  ~Config() = default;

  template <typename T = std::string>
  std::optional<T> get(const std::string& key) const = delete;

  map_t::iterator begin();
  map_t::iterator end();

 private:
  map_t config;
};

template <>
std::optional<std::string> Config::get<std::string>(
    const std::string& key) const;
template <>
std::optional<int> Config::get<int>(const std::string& key) const;
template <>
std::optional<long> Config::get<long>(const std::string& key) const;
template <>
std::optional<long long> Config::get<long long>(const std::string& key) const;
template <>
std::optional<unsigned> Config::get<unsigned>(const std::string& key) const;
template <>
std::optional<unsigned long> Config::get<unsigned long>(
    const std::string& key) const;
template <>
std::optional<unsigned long long> Config::get<unsigned long long>(
    const std::string& key) const;
template <>
std::optional<bool> Config::get<bool>(const std::string& key) const;
template <>
std::optional<float> Config::get<float>(const std::string& key) const;
template <>
std::optional<double> Config::get<double>(const std::string& key) const;
template <>
std::optional<long double> Config::get<long double>(
    const std::string& key) const;
