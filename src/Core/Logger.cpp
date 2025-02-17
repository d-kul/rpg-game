#include "Logger.h"

#ifndef NDEBUG
Logger::Logger() : level{Logger::Level::Debug} {}
#else
Logger::Logger() : level{Logger::Level::Warning} {}
#endif

Logger& Logger::getInstance() { return instance; }

void Logger::statusLine(const char* file, int line, const char* func,
                        Level level) {
  switch (level) {
    case Level::Debug:
      std::clog << "[DEBUG] ";
      break;
    case Level::Info:
      std::clog << "[INFO] ";
      break;
    case Level::Warning:
      std::clog << "[WARNING] ";
      break;
    case Level::Error:
      std::clog << "[ERROR] ";
      break;
  }
  std::clog << file << ":" << line << ":" << func << " ";
}

Logger Logger::instance{};
