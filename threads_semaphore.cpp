#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <chrono>
#include <semaphore.h>
#include <fcntl.h>

sem_t* sem;

void hello(int id) {
    sem_wait(sem);
    for (char c : "hello from thread ")
        std::cout << c;
    std::cout << id << "\n";
    sem_post(sem);
}

int main() {
    sem = sem_open("/hello_sem", O_CREAT | O_EXCL, 0644, 1);
    sem_unlink("/hello_sem"); // remove name immediately, semaphore stays open

    std::vector<std::thread> threads;

    for (int i = 0; i < 8; i++)
        threads.emplace_back(hello, i);

    for (auto& t : threads)
        t.join();

    sem_close(sem);
}
