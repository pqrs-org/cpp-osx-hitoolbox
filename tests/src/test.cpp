#include <boost/ut.hpp>
#include <chrono>
#include <memory>
#include <pqrs/osx/hitoolbox.hpp>
#include <thread>

int main() {
  using namespace boost::ut;
  using namespace boost::ut::literals;

  "secure_event_input_monitor_lifetime"_test = [] {
    auto time_source = std::make_shared<pqrs::dispatcher::hardware_time_source>();
    auto dispatcher = std::make_shared<pqrs::dispatcher::dispatcher>(time_source);

    {
      pqrs::osx::hitoolbox::secure_event_input_monitor monitor(dispatcher,
                                                               std::chrono::milliseconds(1));
      expect(monitor.attached());
      monitor.async_start();
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
      monitor.async_stop();
    }

    dispatcher->terminate();
  };

  return 0;
}
