#pragma once
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include <cstdarg>
#include <functional>
#include <condition_variable>

namespace csp {
  template <class TItem>
  class Channel {
   private:
    std::queue<TItem> queue;
    std::mutex m;
    std::condition_variable cv;
    unsigned int buffer;

   public:
    Channel() : buffer(0) {}
    Channel(unsigned int n) : buffer(n) {}

    void put(TItem t) {
      std::unique_lock<std::mutex> lock(m);

      queue.push(t);
      cv.notify_one();
      cv.wait(lock, [&]() { return queue.size() < this->buffer + 1; });
    }

    TItem get() {
      std::unique_lock<std::mutex> lock(m);
      cv.wait(lock, [&]() { return !queue.empty(); });

      TItem e = queue.front();
      queue.pop();
      cv.notify_one();
      return e;
    }

    bool empty() {
      std::unique_lock<std::mutex> lock(m);
      return queue.empty();
    }
  };

  template <typename TItem>
  inline int which(int n, Channel<TItem>& chan) {
    if (chan.empty()) return -1;
    return n;
  }

  template <typename TItem1, typename... TItem2>
  inline int which(int n, Channel<TItem1>& chan, Channel<TItem2>&... chans) {
    if (chan.empty()) return which(n + 1, chans...);
    return n;
  }

  template <typename... TItem>
  inline int select(Channel<TItem>&... chans) {
    int n = -1;
    while (n == -1) {
      n = which(0, chans...);
    }
    return n;
  }

  class Scheduler {
   private:
    std::vector<std::thread*> threads;

   public:
    template <typename TFunction, typename... TArgs>
    void spawn(TFunction func, TArgs... args) {
      auto thread = new std::thread([=]() { func(args...); });

      threads.emplace_back(thread);
    }

    void join() {
      for (auto thread : threads) {
        if (thread->joinable()) {
          thread->join();
        }
      }
    }

    ~Scheduler() {
      for (auto thread : threads) delete thread;
    }
  };
}
