#include <atomic>
#include <chrono>
#include <ctime>
#include <iostream>
#include <random>
#include <thread>

class Stock {
 private:
  std::atomic<int> m_numberOfTickets;

 public:
  explicit Stock(int numberOfTickets) : m_numberOfTickets(numberOfTickets) {}

  void sell() {
    for (;;) {
      auto numberOfTicketsWas = m_numberOfTickets.load();
      if (numberOfTicketsWas <= 0) {
        break;
      }
      introduceCpuLag();

      // or compare_exchange_strong better?
      if (!m_numberOfTickets.compare_exchange_weak(numberOfTicketsWas,
                                                   numberOfTicketsWas - 1)) {
        continue;
      }
      makeSale();
    }
  }

  int numberOfTickets() const { return m_numberOfTickets; }

 private:
  void makeSale() {
    std::cout << "One more sold! Left: " << numberOfTickets() << std::endl;
  }

  void introduceCpuLag() {
    std::this_thread::sleep_for(
        std::chrono::milliseconds(m_sleepTimeDistribution(m_randGenerator)));
  }

  std::default_random_engine m_randGenerator{static_cast<unsigned int>(
      std::chrono::system_clock::now().time_since_epoch().count())};
  std::uniform_int_distribution<int> m_sleepTimeDistribution{1, 100};
};

int main() {
  const auto numberOfTickets = 50;
  const auto numberOfThreads = 20;

  std::vector<std::thread> threads;
  threads.reserve(numberOfThreads);

  Stock stock{numberOfTickets};
  const auto startTime = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < numberOfThreads; ++i) {
    threads.emplace_back([&stock] { stock.sell(); });
  }

  for (auto &thread : threads) {
    thread.join();
  }

  const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
      std::chrono::high_resolution_clock::now() - startTime);

  std::cout << "=============\n"
            << "All tickets sold!"
            << "\n"
            << "Number of tickets left: " << stock.numberOfTickets() << "\n"
            << "Time: " << duration.count() << " ms"
            << "\n"
            << std::endl;
  return 0;
}