#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <chrono>

void hello(int id) {
    for (char c : "hello from thread ") {
        std::cout << c;
    }
    std::cout << id << "\n";
}

int main() {
    std::vector<std::thread> threads;

    for (int i = 0; i < 5; i++)
        threads.emplace_back(hello, i);

    for (auto& t : threads)
        t.join();
}
