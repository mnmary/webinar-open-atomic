#include <atomic>
#include <cassert>
#include <iostream>
#include <thread>

int notAtomic;
std::atomic<bool> x, y;
std::atomic<int> z;

void write_x_then_y() {
  notAtomic = 42;
  x.store(true, std::memory_order_relaxed);
  y.store(true, std::memory_order_release);
}

void read_y_then_x() {
  while (!y.load(std::memory_order_acquire))
    ;
  if (x.load(std::memory_order_relaxed)) ++z;
  assert(notAtomic == 42);
}

void test() {
  notAtomic = 0;
  x = false;
  y = false;
  z = 0;

  std::thread a(read_y_then_x);
  std::thread b(write_x_then_y);

  a.join();
  b.join();

  assert(z.load() != 0);
}

int main() {
  test();
  return 0;
}