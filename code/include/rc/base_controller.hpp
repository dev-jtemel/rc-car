#ifndef __RC__BASE_CONTROLLER_HPP__
#define __RC__BASE_CONTROLLER_HPP__

#include <string>

namespace rc {

class base_controller {
 public:
  enum state {
    uninitialized,
    initialized,
    running,
    terminated
  };

  base_controller(std::string tag) : _tag(tag), _state(state::uninitialized) {
  }

  virtual ~base_controller() {
  }

  virtual bool init() = 0;
  virtual void start() = 0;
  virtual void terminate() = 0;

  void set_state(state s) {
    _state = s;
  }

 protected:
  std::string _tag;
  state _state;
};

} // namespace rc

#endif //__RC__BASE_CONTROLLER_HPP__
