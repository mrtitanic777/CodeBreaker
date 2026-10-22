#pragma once

#include <cstdint>
#include <map>
#include <mutex>

// Kernel pool (ExAllocatePool*): a first-fit sub-allocator over one region of the system heap.
// Blocks are 16-byte aligned and zero-filled; free neighbours are coalesced.
class GuestPool
{
public:
    void Initialize(uint32_t base, uint32_t size);
    uint32_t Allocate(uint32_t size);   // 0 if out of memory
    bool Free(uint32_t address);
    uint32_t SizeOf(uint32_t address);  // 0 if not an allocation

private:
    std::map<uint32_t, uint32_t> free_; // address -> size
    std::map<uint32_t, uint32_t> used_; // address -> size
    std::mutex mutex_;
};

GuestPool& KernelPool();
