/**
 * @file    LogicalProcessorLogger.h
 * @author  Henrick Deschamps (henrick.deschamps [at] isae-supaero [dot] fr)
 * @version 1.0.0
 * @date    July, 2016
 * @brief   Logical processor logger
 *
 * This class provides a simple way to log execution
 */

#ifndef LOGICALPROCESSORLOGGER_H
#define LOGICALPROCESSORLOGGER_H

#include <memory>
#include <string>

#include <LogicalProcessorCommon.h>

namespace Seaplanes {

/**
 * @brief Type for trace file.
 */
using TraceFile = std::ofstream;

/**
 * @brief Type for trace file unique pointer.
 */
using UpTraceFile = std::unique_ptr<TraceFile>;

/**
 * @brief Explicit type for message.
 */
using Message = std::string;

class Logger final {
public:
  enum class Level {
    INFO,
    NOTICE,
    WARN,
    ERROR,
  };

  ~Logger() = default;
  Logger(const Logger &) = delete;
  void operator=(const Logger &) = delete;
  Logger(Logger &&) = default;
  Logger &operator=(Logger &&) = default;

  static Logger &get_instance(Name /* filename */ = Name());
  static void set_default_level(Level /* level */);
  void log(Level /* level */, Message /* message */);
  void operator<<(Message /* message */);

  void set_trace_filename(Name /* filename */);
  void open_trace_file();
  void close_trace_file();

private:
  Name __trace_filename_;
  UpTraceFile __up_trace_file_;
  static Level __default_level_;

  explicit Logger(Name /* filename */ = Name());
};

} // namespace Seaplanes

#endif // LOGICALPROCESSORLOGGER_H
