#include <iostream>
#include <unistd.h>

//#include "rc/io/string.hpp"

// NOTE:
// We need to define this here to avoid the error message of a direct include
// of the journal implementation. This is the only exception. We undefine it
// immediately after.
#define __RC__IO__JOURNAL_HPP__
#include "rc/io/journal_impl.hpp"
#undef __RC__IO__JOURNAL_HPP__

namespace rc {
namespace io {

journal &journal::instance() {
  static journal j;
  return j;
}

std::stringstream &journal::stream() {
  return _ss;
}

void journal::log(severity slvl, const std::string tag) {
  static const auto console = isatty(2);

  if (slvl >= _slvl) {
    if (console) { // logging to console
      std::cerr << "\e[38;5;" << severity_to_color(slvl) << "m";
    }

    std::cerr << "[" << now() << "] " << severity_to_str(slvl) << " [" << tag << "] "
      << _ss.str() << std::endl;

    if (console) { // logging to console
      std::cerr << "\e[0m";
    }
  }
  _ss.str("");
}

void journal::set_severity(severity slvl) {
  _slvl = slvl;
}

void journal::lock() {
  _lock.lock();
}

void journal::unlock() {
  _lock.unlock();
}

journal::journal() : _slvl(info), _ss(), _lock() {
}

uint64_t journal::now() {
   return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch())
          .count();
}

std::string journal::severity_to_str(severity slvl) {
  switch(slvl) {
    case trace: return "[trace]";
    case debug: return "[debug]";
    case info:  return "[ info]";
    case warn:  return "[ warn]";
    case error: return "[error]";
    default:    return "[     ]";
  }
}

journal::colors journal::severity_to_color(severity slvl) {
  switch(slvl) {
    case trace: return c_trace;
    case debug: return c_debug;
    case info:  return c_info;
    case warn:  return c_warn;
    case error: return c_error;
    default:    return c_default;
  }
}

} // namespace io
} // namespace rc
