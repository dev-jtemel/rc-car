#ifndef __RC__NETWORK_CONTROLLER_HPP__
#define __RC__NETWORK_CONTROLLER_HPP__

#include "rc/base_controller.hpp"
#include "third_party/http.hpp"

namespace rc {
namespace network {

class network_controller : public base_controller {
 public:
  explicit network_controller(std::function<void(int)> termination_handler);
  virtual ~network_controller();

  virtual bool init();
  virtual void start();
  virtual void terminate();

  void register_stop_handler(std::function<void()> stop_handler);
  void register_start_handler(std::function<void()> start_handler);

 private:
  void start_internal_servers();
  void power_cb(const httplib::Request &req, httplib::Response &res);
  void orientation_cb(const httplib::Request &req, httplib::Response &res);

  const std::string IP = "192.168.0.30";
  const uint16_t PORT = 8080U;
  const uint16_t PORT_PWR = 8081U;
  const uint16_t PORT_ORI = 8082U;

  std::unique_ptr<httplib::Server> _svr = std::make_unique<httplib::Server>();
  std::unique_ptr<httplib::Server> _svr_pwr = std::make_unique<httplib::Server>();
  std::unique_ptr<httplib::Server> _svr_ori = std::make_unique<httplib::Server>();
  bool _rc_state;

  std::mutex _pwr_lk;
  uint64_t _pwr_seq;

  std::mutex _ori_lk;
  uint64_t _ori_seq;

  std::thread _svr_thread;
  std::thread _svr_thread_pwr;
  std::thread _svr_thread_ori;

  std::function<void()> _stop_handler;
  std::function<void()> _start_handler;
  std::function<void(int)> _termination_handler;
};

} // namespace network
} // namespace rc

#endif //__RC__NETWORK_CONTROLLER_HPP__

