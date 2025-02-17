#pragma once

#include <iostream>
#include <utility>

class Logger {
 private:
  Logger();

 public:
  enum class Level : int { Debug, Info, Warning, Error };

  static Logger& getInstance();

  template <typename... Args>
  void log(Level level, Args&&... args) {
    if (static_cast<int>(level) < static_cast<int>(this->level)) return;
    (std::clog << ... << args) << '\n';
  }

  template <typename... Args>
  void slog(Level level, const char* sep, Args&&... args) {
    if (static_cast<int>(level) < static_cast<int>(this->level)) return;
    ((std::clog << args << sep), ...) << '\n';
  }

  template <typename... Args>
  void flog(const char* file, int line, const char* func, Level level,
            Args&&... args) {
    if (static_cast<int>(level) < static_cast<int>(this->level)) return;
    statusLine(file, line, func, level);
    log(level, std::forward<Args>(args)...);
  }

  template <typename... Args>
  void fslog(const char* file, int line, const char* func, Level level,
             const char* sep, Args&&... args) {
    if (static_cast<int>(level) < static_cast<int>(this->level)) return;
    statusLine(file, line, func, level);
    slog(level, sep, std::forward<Args>(args)...);
  }

 private:
  void statusLine(const char* file, int line, const char* func, Level level);

  Level level;
  static Logger instance;
};

inline Logger::Level Debug = Logger::Level::Debug;
inline Logger::Level Info = Logger::Level::Info;
inline Logger::Level Warning = Logger::Level::Warning;
inline Logger::Level Error = Logger::Level::Error;

#define LOG(...) \
  Logger::getInstance().flog(__FILE__, __LINE__, __func__, __VA_ARGS__)
#define SLOG(...) \
  Logger::getInstance().fslog(__FILE__, __LINE__, __func__, __VA_ARGS__)
#define DEBUG(...) LOG(Debug, __VA_ARGS__)
#define SDEBUG(...) SLOG(Debug, __VA_ARGS__)
#define INFO(...) LOG(Info, __VA_ARGS__)
#define SINFO(...) SLOG(Info, __VA_ARGS__)
#define WARNING(...) LOG(Warning, __VA_ARGS__)
#define SWARNING(...) SLOG(Warning, __VA_ARGS__)
#define ERROR(...) LOG(Error, __VA_ARGS__)
#define SERROR(...) SLOG(Error, __VA_ARGS__)
