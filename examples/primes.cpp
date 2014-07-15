#include "../csp.hpp"
#include <iostream>

#define PRIMES 100

[[noreturn]] void generate(csp::Channel<int>*);
[[noreturn]] void filter(csp::Channel<int>*, csp::Channel<int>*, int);

int main() {
  csp::Scheduler scheduler;

  auto ch = new csp::Channel<int>;

  scheduler.spawn(generate, ch);

  for (int i = 0; i < PRIMES; i++) {
    int prime = ch->get();
    std::cout << prime << std::endl;
    auto ch1 = new csp::Channel<int>;
    scheduler.spawn(filter, ch, ch1, prime);
    ch = ch1;
  }

  scheduler.join();
}

void generate(csp::Channel<int>* chan) {
  int i = 2;
  for (;;) {
    chan->put(i);
    i++;
  }
}

void filter(csp::Channel<int>* in, csp::Channel<int>* out, int prime) {
  for (;;) {
    int i = in->get();
    if (i % prime != 0) {
      out->put(i);
    }
  }
}
