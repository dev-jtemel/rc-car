#ifndef __RC__IO__JOURNAL_HPP__
#define __RC__IO__JOURNAL_HPP__

#include "rc/io/journal_impl.hpp"

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

} // namesapce io
} // namesapce rc

#endif //__RC__IO__JOURNAL_HPP__
