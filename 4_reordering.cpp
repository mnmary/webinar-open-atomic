
#include <mutex>

std::mutex mutex;
int * instance = nullptr;

int * getInstance() {
    if (instance == nullptr) {

        std::lock_guard<std::mutex> guard{mutex};

        if (instance == nullptr) {

            instance = new int(42);

        }

    }

}


int * getInstance() {
    if (instance == nullptr) {

        std::lock_guard<std::mutex> guard{mutex};

        if (instance == nullptr) {

            void * memory = operator new(sizeof(int));    // 1

            new (memory) int(42);                         // 2

            instance = static_cast<int*>(memory);         // 3

            // 2 and 3 could be reordered

        }

    }

}



int main() {

    int a = 0;
    int b = 0;
    int c = 0;

    a = 5;            // 1
    b = a;            // 2
    a = 42;           // 3
    c = b + 7;        // 4 - could be reordered with 3, because b is already in register
}