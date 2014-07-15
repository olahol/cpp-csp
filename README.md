# cpp-csp

Minimalistic header-only library for channels and CSP (Communicating
sequential process) in C++11.

## Example

```cpp
#include <iostream>

#include "csp.hpp"

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
```

More examples can be found under `examples/`.

## Contributing

Please run `make tidy` before submitting patches so that the code style is
consistent.
