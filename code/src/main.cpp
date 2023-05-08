#include <thread>
#include <chrono>
#include "rc/io/journal.hpp"

int main() {
  RC_SEVERITY(debug);

  // Sample logging
  RC_LOG(debug, "motor_controller", "Logging debug...");
  std::this_thread::sleep_for(std::chrono::seconds(1));

  RC_LOG(info, "steering_controller", "Logging info...");
  std::this_thread::sleep_for(std::chrono::seconds(1));

  RC_LOG(warn, "speedometer", "Logging warn...");
  std::this_thread::sleep_for(std::chrono::seconds(1));

  RC_LOG(error, "accelerometer", "Logging error...");
}
