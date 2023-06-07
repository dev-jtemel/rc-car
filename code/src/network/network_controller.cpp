#include "rc/network/network_controller.hpp"
#include "rc/io/journal.hpp"

namespace rc {
namespace network {

network_controller::network_controller(std::function<void(int)> termination_handler)
  : base_controller("network_controller"),
    _svr_thread(),
    _svr_thread_pwr(),
    _svr_thread_ori(),
    _termination_handler(termination_handler),
    _rc_state(false) {
}

network_controller::~network_controller() {
}

bool network_controller::init() {
  _svr->Get("/start", [&](const httplib::Request &, httplib::Response &) {
    RC_LOG(info, _tag, "Starting rccar on request");

    _start_handler();
    start_internal_servers();

    _rc_state = true;
  });

  _svr->Get("/stop", [&](const httplib::Request &, httplib::Response &) {
    RC_LOG(info, _tag, "Stopping rccar on request");

    _svr_pwr->stop();
    _svr_ori->stop();
    _stop_handler();

    _rc_state = false;
  });

  _svr->Get("/abort", [&](const httplib::Request &, httplib::Response &) {
    RC_LOG(debug, _tag, "Stopping rccar on request");

    _termination_handler(0);
  });

  _svr_pwr->Get("/power", [&](const httplib::Request &req, httplib::Response &res) {
      power_cb(req, res);
  });

  _svr_ori->Get("/orientation", [&](const httplib::Request &req, httplib::Response &res) {
    orientation_cb(req, res);
  });

  RC_LOG(info, _tag, "Initialized");
  set_state(state::initialized);
  return true;
}

void network_controller::start() {
  _svr_thread = std::thread([&]() {
    RC_LOG(info, _tag, "Server started on " << IP << ":" << PORT);
    _svr->listen(IP, PORT);
  });
  _svr_thread.detach();

  RC_LOG(info, _tag, "Started");
  set_state(state::running);
}

void network_controller::terminate() {
  _svr->stop();
  _svr_pwr->stop();
  _svr_ori->stop();

  RC_LOG(info, _tag, "Terminated");
  set_state(state::terminated);
}

void network_controller::register_stop_handler(std::function<void()> stop_handler) {
  _stop_handler = stop_handler;
}

void network_controller::register_start_handler(std::function<void()> start_handler) {
  _start_handler = start_handler;
}

void network_controller::start_internal_servers() {
  _svr_thread_pwr = std::thread([&]() {
    RC_LOG(info, _tag, "Power server started on " << IP << ":" << PORT_PWR);
    _svr_pwr->listen(IP, PORT_PWR);
  });
  _svr_thread_ori = std::thread([&]() {
    RC_LOG(info, _tag, "Orientation server started on " << IP << ":" << PORT_ORI);
    _svr_ori->listen(IP, PORT_ORI);
  });
  _svr_thread_pwr.detach();
  _svr_thread_ori.detach();
}

void network_controller::power_cb(const httplib::Request &req, httplib::Response &res){
  const auto pwr = std::stod(req.get_param_value("pwr"));
  const auto pwr_seq = std::stod(req.get_param_value("seq"));

  _pwr_lk.lock();
  if (pwr_seq < _pwr_seq) {
    _pwr_lk.unlock();
    RC_LOG(trace, _tag, "Skip out of sync packet: pwr_seq=" << pwr_seq);
  } else {
    _pwr_seq = pwr_seq;
    _pwr_lk.unlock();
    RC_LOG(debug, _tag, "pwr_seq=" << pwr_seq << " " << pwr);
  }
}

void network_controller::orientation_cb(const httplib::Request &req, httplib::Response &res){
  const auto ori_str = req.get_param_value("ori");
  const auto ori = std::stod(ori_str.substr(0, ori_str.length() - 3));
  const auto ori_seq = std::stod(req.get_param_value("seq"));

  _ori_lk.lock();
  if (ori_seq < _ori_seq) {
    _ori_lk.unlock();
    RC_LOG(trace, _tag, "Skip out of sync packet: ori_seq=" << ori_seq);
  } else {
    _ori_seq = ori_seq;
    _ori_lk.unlock();
    RC_LOG(debug, _tag, "ori_seq=" << ori_seq << " " << ori);
  }
}

} // namespace network
} // namespace rc
