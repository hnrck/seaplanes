//! \file    LogicalProcessorLogger.cpp
//! \author  Henrick Deschamps (henrick.deschamps [at] isae-supaero [dot] fr)
//! \version 1.0.0
//! \date    July, 2016
//! \brief   Rosace Attribute manipulation implementation.

#include <LogicalProcessorLogger.h>

#include <fstream>
#include <ostream>
#include <sstream>

using std::clog;
using std::cout;
using std::endl;
using std::make_unique;
using std::move;
using std::ostream;
using std::stringstream;

namespace Seaplanes {

Logger &Logger::get_instance(ostream *p_log_stream) {
  static auto &&instance = Logger(p_log_stream);
  return (instance);
}

void Logger::set_default_level(Level level) { __default_level_ = level; }

void Logger::log(Level level, const Message &message) {
  if (__p_log_stream_ != nullptr) {
    if (level >= __default_level_) {
      *__p_log_stream_ << __HEADERS_.at(level) << ": " << message << endl;
    }
  }
}

void Logger::operator<<(const Message &message) {
  log(__default_level_, message);
}

Logger::Level Logger::__default_level_ = Logger::Level::INFO;

Logger::Logger(ostream *p_log_stream) : __p_log_stream_(p_log_stream) {}

} // namespace Seaplanes
