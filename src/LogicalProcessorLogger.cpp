/**
 * @file    LogicalProcessorLogger.cpp
 * @author  Henrick Deschamps (henrick.deschamps [at] isae-supaero [dot] fr)
 * @version 1.0.0
 * @date    July, 2016
 * @brief   Rosace Attribute manipulation implementation.
 */

#include <LogicalProcessorLogger.h>

#include <fstream>
#include <iostream>
#include <sstream>

using std::clog;
using std::cout;
using std::endl;
using std::make_unique;
using std::move;
using std::stringstream;

namespace Seaplanes {

Logger &Logger::get_instance(Name filename) {
  static auto &&instance = Logger(move(filename));
  return (instance);
}

void Logger::set_default_level(Level level) { __default_level_ = level; }

void Logger::log(Level level, Message message) {
  // Initializing a string stream.
  auto ss = stringstream();

  // Buffering the log level in the string stream.
  switch (level) {
  case (Level::INFO):
    ss << "INFO";
    break;
  case (Level::NOTICE):
    ss << "NOTICE";
    break;
  case (Level::WARN):
    ss << "WARN";
    break;
  case (Level::ERROR):
    ss << "ERROR";
    break;
  }
  // Buffering the message in the string stream.
  ss << ": " << message;

  /* If trace file set */
  if (__up_trace_file_ != nullptr) {
    (*__up_trace_file_) << ss.str() << endl;
  }

  switch (level) {
  case (Level::INFO):
    // Nothing
    break;
  case (Level::NOTICE):
    cout << ss.str() << endl;
    break;
  case (Level::WARN):
  case (Level::ERROR):
    clog << ss.str() << endl;
    break;
  }
}

void Logger::operator<<(Message message) {
  log(__default_level_, move(message));
}

void Logger::set_trace_filename(Name filename) {
  __trace_filename_ = move(filename);
}

void Logger::open_trace_file() {
  if (!__trace_filename_.empty()) {
    __up_trace_file_ = make_unique<TraceFile>(__trace_filename_);
  } else {
    log(Level::WARN, "Cannot open trace file, file not set.");
  }
}

void Logger::close_trace_file() {
  if (__up_trace_file_ != nullptr) {
    __up_trace_file_->close();
  } else {
    log(Level::WARN, "Cannot close trace file, file not set.");
  }
}

Logger::Level Logger::__default_level_ = Logger::Level::INFO;

Logger::Logger(Name filename)
    : __trace_filename_(move(filename)), __up_trace_file_(nullptr) {}

} // namespace Seaplanes
