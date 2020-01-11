//! \file    LogicalProcessorLogger.h
//! \author  Henrick Deschamps (henrick.deschamps [at] isae-supaero [dot] fr)
//! \version 1.0.0
//! \date    July, 2016
//! \brief   Logical processor logger
//!
//! This class provides a simple way to log execution

#ifndef LOGICALPROCESSORLOGGER_H
#define LOGICALPROCESSORLOGGER_H

#include <iostream>

#include <map>
#include <memory>
#include <string>

#include <LogicalProcessorCommon.h>

namespace Seaplanes {

//! \brief Explicit type for message.
using Message = std::string;

class Logger final {
public:
  enum class Level : unsigned int {
    INFO,
    NOTICE,
    WARN,
    ERROR,
  };

  ~Logger() = default;
  Logger(const Logger &) = delete;
  void operator=(const Logger &) = delete;
  Logger(Logger &&) = default;
  void operator=(Logger &&) = delete;

  static Logger &get_instance(std::ostream * /* p_log_stream */ = &std::clog);
  static void set_default_level(Level /* level */);
  void log(Level /* level */, const Message & /* message */);
  void operator<<(const Message & /* message */);

private:
  std::ostream *__p_log_stream_;
  static Level __default_level_;
  const std::map<Level, Message> __HEADERS_{
      {Level::INFO, "INFO"},
      {Level::NOTICE, "NOTICE"},
      {Level::WARN, "WARN"},
      {Level::ERROR, "ERROR"},
  };

  explicit Logger(std::ostream * /* p_log_stream */);
};

} // namespace Seaplanes

#endif // LOGICALPROCESSORLOGGER_H
