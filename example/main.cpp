#include <csignal>
#include <iostream>
#include <pqrs/osx/hitoolbox.hpp>

namespace {
auto global_wait = pqrs::make_thread_wait();
}

int main(void) {
  std::signal(SIGINT, [](int) {
    global_wait->notify();
  });

  auto time_source = std::make_shared<pqrs::dispatcher::hardware_time_source>();
  auto dispatcher = std::make_shared<pqrs::dispatcher::dispatcher>(time_source);

  auto monitor = std::make_shared<pqrs::osx::hitoolbox::secure_event_input_monitor>(dispatcher);

  monitor->secure_event_input_status_changed.connect([](auto&& enabled) {
    std::cout << "secure_event_input_status_changed enabled: " << enabled << std::endl;
  });

  monitor->async_start();

  std::thread thread([&monitor] {
    global_wait->wait_notice();

    monitor = nullptr;
  });

  thread.join();

  dispatcher->terminate();
  dispatcher = nullptr;

  std::cout << "finished" << std::endl;

  return 0;
}
