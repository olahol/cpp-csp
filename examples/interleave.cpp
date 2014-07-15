#include "../csp.hpp"
#include <iostream>

int main() {
  csp::Channel<int> chan;
  csp::Scheduler scheduler;

  scheduler.spawn([&]() {
    for (int i = 1; i < 11; i++) {
      std::cout << "put " << i << std::endl;
      chan.put(i);
    }
    chan.put(0);
  });

  scheduler.spawn([&]() {
    while (true) {
      auto i = chan.get();
      if (i == 0) break;
      std::cout << "get " << i << std::endl;
    }
  });

  scheduler.join();

  return 0;
}
