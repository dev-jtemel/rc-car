#ifndef __RC__IO__JOURNAL_HPP__
#error "Do not include this file directly, instead include rc/io/journal.hpp"
#endif

#ifndef __RC__IO__JOURNAL_IMPL_HPP__
#define __RC__IO__JOURNAL_IMPL_HPP__

#include <sstream>
#include <mutex>

namespace rc {
namespace io {

#define RC_SEVERITY(lvl) \
  rc::io::journal::instance().lock(); \
  rc::io::journal::instance().set_severity(rc::io::journal::severity::lvl); \
  rc::io::journal::instance().unlock();

#define RC_LOG(lvl, tag, str) \
  do { \
    rc::io::journal::instance().lock(); \
    rc::io::journal::instance().stream() << str; \
    rc::io::journal::instance().log(rc::io::journal::severity::lvl, tag); \
    rc::io::journal::instance().unlock(); \
  } while (false)


/**
 * This singleton class handles logging to the console.
 *
 * TODO: Log to file aswell.
 */
class journal {
 public:
  /**
   * The possible severities to log to.
   */
  enum severity {
    trace = 0,
    debug,
    info,
    warn,
    error,

    num_of_severities = 255
  };

  /**
   * Colors for the severities in hex.
   */
  enum colors {
    c_default = 0,
    c_trace = 240,
    c_debug = 247,
    c_info = 15,
    c_warn = 226,
    c_error = 196
  };

  static journal &instance();

  /**
   * Enfore singleton.
   */
  journal(const journal &) = delete;
  journal(journal &&) = delete;


  /**
   * Verify the severity and log the contents in the internal stream to
   * the console under stderr.
   *
   * Internal stream is cleared here.
   */
  void log(severity slvl, const std::string tag);

  void set_severity(severity slvl);

  void lock();
  void unlock();
 
  std::stringstream &stream();
 private:
  journal();

  uint64_t now();
  std::string severity_to_str(severity slvl);
  colors severity_to_color(severity slvl);
 
  severity _slvl; // default is info
  std::stringstream _ss;
  std::mutex _lock;
};

} // namespace io
} // namespace rc

#endif //__RC__IO__JOURNAL_IMPL_HPP__
