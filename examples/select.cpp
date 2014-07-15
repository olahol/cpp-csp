#include "../csp.hpp"
#include <iostream>

int main() {
  csp::Scheduler scheduler;
  csp::Channel<int> ch1(10), ch2(0);

  scheduler.spawn([&]() {
    std::chrono::milliseconds dura(100);
    for (;;) {
      int n = csp::select(ch2, ch1);
      std::cout << "got chan " << n << std::endl;
      switch (n) {
        case 0:
          ch2.get();
          std::cout << "exit" << std::endl;
          return;
        case 1:
          ch1.get();
          std::this_thread::sleep_for(dura);
          break;
      }
    }
  });

  scheduler.spawn([&]() {
    std::chrono::milliseconds dura(10);
    std::this_thread::sleep_for(dura);
    ch2.put(2);
  });

  ch1.put(1);
  ch1.put(1);
  ch1.put(1);
  ch1.put(1);

  std::cout << "join" << std::endl;
  scheduler.join();
}
