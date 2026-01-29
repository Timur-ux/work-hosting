#ifndef LOGGER_HPP_
#define LOGGER_HPP_
#include <mutex>
#include <ostream>
#include <sstream>
#pragma once
class ILog : public std::stringstream {
public:
  virtual void flush() = 0;
  virtual ~ILog() {};
};

class TimedLog : public ILog {
public:
  TimedLog(std::ostream &os, const char *format = "%Y:%m:%d -- %H:%M:%S");
  ~TimedLog();

  void flush() override;

private:
  static std::mutex mutex_;
  std::ostream &os_;
  const char *format_;
};

struct FastLog {
  FastLog(ILog &logger) : logger_(logger) {}
  ~FastLog() { logger_.flush(); }

  template <typename T> std::ostream &operator<<(const T &data) {
    return logger_ << data;
  }

private:
  ILog &logger_;
};

#endif // !LOGGER_HPP_
