#include <iostream>
#include <cstdlib>
#include <cstring>

int main() {
    const size_t chunk = 1024 * 1024; // 1 MiB
    size_t total = 0;

    while (true) {
        void* p = malloc(chunk);
        if (!p) {
            std::cerr << "malloc returned null at " << total << " MiB\n";
            return 1;
        }
        memset(p, 0, chunk); // touch every page to actually commit
        total++;
        std::cout << "allocated " << total << " MiB\n";
        std::cout.flush();
    }
}
