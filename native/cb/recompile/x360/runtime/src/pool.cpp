#include "pool.h"

#include "memory.h"

#include <cstring>

namespace
{
    constexpr uint32_t kAlignment = 16;
}

GuestPool& KernelPool()
{
    static GuestPool pool;
    return pool;
}

void GuestPool::Initialize(uint32_t base, uint32_t size)
{
    std::lock_guard lock(mutex_);
    free_.clear();
    used_.clear();
    free_[base] = size;
}

uint32_t GuestPool::Allocate(uint32_t size)
{
    size = (std::max<uint32_t>(size, 1) + kAlignment - 1) & ~(kAlignment - 1);
    std::lock_guard lock(mutex_);
    for (auto it = free_.begin(); it != free_.end(); ++it)
    {
        if (it->second < size)
            continue;
        const uint32_t address = it->first;
        const uint32_t remaining = it->second - size;
        free_.erase(it);
        if (remaining)
            free_[address + size] = remaining;
        used_[address] = size;
        memset(guest::g_memory.Translate(address), 0, size);
        return address;
    }
    return 0;
}

bool GuestPool::Free(uint32_t address)
{
    std::lock_guard lock(mutex_);
    auto it = used_.find(address);
    if (it == used_.end())
        return false;
    uint32_t start = address, size = it->second;
    used_.erase(it);

    auto next = free_.find(start + size);
    if (next != free_.end())
    {
        size += next->second;
        free_.erase(next);
    }
    auto prev = free_.lower_bound(start);
    if (prev != free_.begin())
    {
        --prev;
        if (prev->first + prev->second == start)
        {
            start = prev->first;
            size += prev->second;
            free_.erase(prev);
        }
    }
    free_[start] = size;
    return true;
}

uint32_t GuestPool::SizeOf(uint32_t address)
{
    std::lock_guard lock(mutex_);
    auto it = used_.find(address);
    return it == used_.end() ? 0 : it->second;
}
