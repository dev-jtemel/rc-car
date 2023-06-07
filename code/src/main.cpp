#include <thread>
#include <mutex>
#include <chrono>
#include <vector>
#include <csignal>
#include <condition_variable>

#include "rc/io/journal.hpp"

#include "rc/network/network_controller.hpp"

std::mutex lock;
std::condition_variable cv;
volatile bool running = false;

void termination_handler(int signum) {
  RC_LOG(info, "rccar", "Termination signal received");

  std::lock_guard<std::mutex> lk(lock);
  running = false;
  cv.notify_one();
}

int main() {
  RC_SEVERITY(trace);
  signal(SIGINT, termination_handler);

  std::vector<std::unique_ptr<rc::base_controller>> controllers;

  auto nc = std::make_unique<rc::network::network_controller>(termination_handler);
  std::function<void()> stop_handler = [&](){
    for (auto &c : controllers) {
      c->terminate();
    }
  };
  std::function<void()> start_handler = [&](){
    for (auto &c : controllers) {
      c->start();
    }
  };

  nc->register_stop_handler(stop_handler);
  nc->register_start_handler(start_handler);

  for (auto &c : controllers) {
    c->init();
  }
  nc->init();

  nc->start();

  {
    std::unique_lock<std::mutex> lk(lock);
    running = true;
    cv.wait(lk, [&]{ return !running; });
  }

  for (auto &c : controllers) {
    c->terminate();
  }
  nc->terminate();
  return EXIT_SUCCESS;
}
