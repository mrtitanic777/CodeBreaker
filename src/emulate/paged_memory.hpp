// emulate/paged_memory.hpp -- the memory of a 32-bit machine with tens of megabytes of RAM, made cheap to start: the program's loaded pieces are shared (never
// copied) and a run's writes go to 4 KB pages made when first written, so a run that touches a little costs a little and what it changed is the pages it made.
//
//     paged_memory::Memory ram(program_regions);     // the loaded pieces, shared between runs and threads
//     ram.write(0x00200000, 4, 7);
//     ram.read(0x00200000, 4);                       // 7 (an address nothing was written to or loaded at reads zero)
//     ram.changes();                                 // {address, byte} of every byte whose value now differs from what was loaded
//
// A Memory belongs to one thread; the shared regions are read-only.
#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

#include "core/bytes.hpp"

namespace cb::emulate::paged_memory {

inline constexpr std::uint32_t kPageSize = 0x1000;

// Bytes loaded at an address.
struct Region {
    std::uint32_t address = 0;
    Bytes bytes;
};
using Regions = std::shared_ptr<const std::vector<Region>>;

class Memory {
public:
    Memory() = default;
    explicit Memory(Regions regions);

    // `size` (1, 2, 4 or 8) bytes at `address`, little-endian.
    std::uint64_t read(std::uint32_t address, unsigned size) const;
    void write(std::uint32_t address, unsigned size, std::uint64_t value);

    // Does a loaded region or a written page cover this address?
    bool covers(std::uint32_t address) const;

    // Every byte that now differs from what was loaded (zero where nothing was), in address order, at most `limit` of them.
    std::vector<std::pair<std::uint32_t, std::uint8_t>> changes(std::size_t limit = 4000) const;

private:
    using Page = std::array<std::uint8_t, kPageSize>;
    std::uint8_t base_byte(std::uint32_t address) const;
    std::uint8_t byte(std::uint32_t address) const;
    void put(std::uint32_t address, std::uint8_t value);

    Regions regions_;
    std::unordered_map<std::uint32_t, std::unique_ptr<Page>> pages_;
};

}  // namespace cb::emulate::paged_memory
