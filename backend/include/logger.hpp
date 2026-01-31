#ifndef LOGGER_HPP_
#define LOGGER_HPP_
#include <mutex>
#include <ostream>
#include <sstream>
#pragma once

class ILog : public std::stringstream {
public:
  virtual ~ILog() {};
};

class TimedLog : public ILog {
public:
  TimedLog(const char *format = "%Y:%m:%d -- %H:%M:%S");
  ~TimedLog();

private:
  static std::mutex mutex_;
  const char *format_;
};

#endif // !LOGGER_HPP_
