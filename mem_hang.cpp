#include <iostream>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdint>

uint64_t ONE = 1ULL;

static uintptr_t virt_to_phys(void* vaddr) {
    int fd = open("/proc/self/pagemap", O_RDONLY);
    if (fd < 0) return 0;

    uintptr_t virt = (uintptr_t)vaddr;
    uint64_t entry;
    off_t offset = (virt / 4096) * 8;

    // read the page table entry, if it's not present, return 0
    if (pread(fd, &entry, 8, offset) != 8) {
        close(fd);
        return 0;
    }
    close(fd);

    if (!(entry & (ONE << 63))) return 0; // page not present

    uint64_t pfn = entry & ((ONE << 55) - 1);
    return (pfn * 4096) + (virt % 4096);
}

int main(int argc, char* argv[]) {
    void* addr = mmap((void*)0x400000, 1 << 12, PROT_READ | PROT_WRITE,
                      MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (addr == MAP_FAILED) {
        std::cerr << "mmap failed\n";
        return 1;
    }

    // Touch the page so the OS creates the physical mapping
    *static_cast<volatile char*>(addr) = 0;

    uintptr_t phys = virt_to_phys(addr);

    std::cout << "virtual address:  " << addr << "\n";
    if (phys)
        std::cout << "physical address: 0x" << std::hex << phys << "\n";
    else
        std::cout << "physical address: unavailable (need root?)\n";

    // pause();
}
