#include <atomic>
#include <cassert>
#include <iostream>

void test_numbers() {
  std::atomic_int32_t atomicInt{10};

  auto previous = atomicInt.fetch_sub(2);
  assert(previous == 10);
  assert(atomicInt == 8);

  atomicInt++;
  assert(atomicInt == 9);

  atomicInt--;
  assert(atomicInt == 8);

  atomicInt -= 5;  // also atomic
  assert(atomicInt == 3);

  previous = atomicInt.exchange(100);
  assert(previous == 3);

  previous = 99;
  bool isExchanged = atomicInt.compare_exchange_weak(previous, int32_t(2000));
  // isExchanged = atomicInt.compare_exchange_weak(100, int32_t(2000));
  assert(isExchanged == false);
  assert(previous == 100);
  assert(atomicInt == 100);

  previous = 100;
  isExchanged = atomicInt.compare_exchange_weak(previous, int32_t(3000));
  assert(isExchanged == true);
  assert(atomicInt == 3000);
  assert(previous == 100);

  atomicInt &= 10;
  assert(atomicInt == 8);

  atomicInt.fetch_and(2);
  atomicInt.fetch_or(3);
  atomicInt.fetch_xor(3);
  atomicInt &= 2;
  atomicInt |= 2;
  atomicInt ^= 2;

  int64_t value = 10;
  std::atomic<int64_t*> atomicPtr{&value};
  atomicPtr.fetch_sub(5);
  atomicPtr++;
  atomicPtr--;
  atomicPtr -= 1;
  atomicPtr += 1;
  // Compile error - no operator&=
  // atomicPtr &= 1;
  // atomicPtr.fetch_and(2);
  // atomicPtr.fetch_or(3);
  // atomicPtr.fetch_xor(3);
  // atomicPtr &= 2;
  // atomicPtr |= 2;
  // atomicPtr ^= 2;
}

struct Data {
  uint8_t value1;
  uint8_t value2;
  uint8_t value3;
  uint8_t value4;
};
struct Data3 {
  uint8_t value1;
  uint8_t value2;
  uint8_t value3;
};
struct Data5 {
  uint8_t value1;
  uint8_t value2;
  uint8_t value3;
  uint8_t value4;
  uint8_t value5;
};

void test_custom() {
  std::cout << "\n************\ntest_custom\n";

  std::atomic<Data> atomicData{Data{1, 2, 3, 4}};

  std::cout << "is_lock_free Data 4 fields? = " << std::boolalpha
            << atomicData.is_lock_free() << std::endl;

  std::cout << "is_lock_free Data 3 fields? = " << std::boolalpha
            << std::atomic<Data3>().is_lock_free() << std::endl;

  std::cout << "is_lock_free Data 5 fields? = " << std::boolalpha
            << std::atomic<Data5>().is_lock_free() << std::endl;

  // Load
  auto current = atomicData.load();
  assert(current.value1 == 1);
  assert(current.value2 == 2);
  assert(current.value3 == 3);
  assert(current.value4 == 4);

  // Store
  atomicData.store(Data{4, 3, 2, 1});

  current = atomicData;  // just read - also atomic
  assert(current.value1 == 4);
  assert(current.value2 == 3);
  assert(current.value3 == 2);
  assert(current.value4 == 1);

  current.value1 = 5;
  atomicData = current;  // just assign - also atomic
  // assert(atomicData.value1 == 4); ?????

  current = atomicData.exchange(Data{10, 11, 12, 13});
  assert(current.value1 == 5);
  assert(current.value2 == 3);
  assert(current.value3 == 2);
  assert(current.value4 == 1);

  current = atomicData.load();
  assert(current.value1 == 10);
  assert(current.value2 == 11);
  assert(current.value3 == 12);
  assert(current.value4 == 13);
}

int main() {
  test_numbers();
  test_custom();

  return 0;
}