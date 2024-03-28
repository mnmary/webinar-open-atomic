#include <atomic>
#include <cassert>
#include <iostream>
#include <thread>

std::atomic<bool> x, y;
std::atomic<int> z;

void write_z_y_x() {
  z.store(42, std::memory_order_seq_cst);
  y.store(true, std::memory_order_seq_cst);
  x.store(true, std::memory_order_seq_cst);
}

void read_x_then_y_then_z() {
  while (!x.load(std::memory_order_seq_cst))
    ;
  assert(y.load(std::memory_order_seq_cst));
  assert(z.load(std::memory_order_seq_cst) == 42);
}

void test() {
  x = false;
  y = false;
  z = 0;

  std::thread a(read_x_then_y_then_z);
  std::thread b(write_z_y_x);

  a.join();
  b.join();

  assert(z.load() != 0);
}

int main() {
  test();
  return 0;
}