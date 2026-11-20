// cb-unity: alone (file-local names that other files in a batch also use)
#include "emulate/paged_memory.hpp"

#include <algorithm>

namespace cb::emulate::paged_memory {

Memory::Memory(Regions regions) : regions_(std::move(regions)) {}

std::uint8_t Memory::base_byte(std::uint32_t address) const {
    if (!regions_) return 0;
    for (const Region& r : *regions_)
        if (address >= r.address && address - r.address < r.bytes.size()) return r.bytes[address - r.address];
    return 0;
}

std::uint8_t Memory::byte(std::uint32_t address) const {
    if (const auto it = pages_.find(address / kPageSize); it != pages_.end()) return (*it->second)[address % kPageSize];
    return base_byte(address);
}

void Memory::put(std::uint32_t address, std::uint8_t value) {
    auto& page = pages_[address / kPageSize];
    if (!page) {
        page = std::make_unique<Page>();
        const std::uint32_t first = address & ~(kPageSize - 1);
        for (std::uint32_t i = 0; i < kPageSize; ++i) (*page)[i] = base_byte(first + i);
    }
    (*page)[address % kPageSize] = value;
}

std::uint64_t Memory::read(std::uint32_t address, unsigned size) const {
    std::uint64_t v = 0;
    if (address % kPageSize + size <= kPageSize) {   // (inside one page: one lookup)
        if (const auto it = pages_.find(address / kPageSize); it != pages_.end()) {
            const Page& page = *it->second;
            for (unsigned i = 0; i < size; ++i) v |= static_cast<std::uint64_t>(page[address % kPageSize + i]) << (8 * i);
            return v;
        }
    }
    for (unsigned i = 0; i < size; ++i) v |= static_cast<std::uint64_t>(byte(address + i)) << (8 * i);
    return v;
}

void Memory::write(std::uint32_t address, unsigned size, std::uint64_t value) {
    for (unsigned i = 0; i < size; ++i) put(address + i, static_cast<std::uint8_t>(value >> (8 * i)));
}

bool Memory::covers(std::uint32_t address) const {
    if (pages_.contains(address / kPageSize)) return true;
    if (!regions_) return false;
    for (const Region& r : *regions_)
        if (address >= r.address && address - r.address < r.bytes.size()) return true;
    return false;
}

std::vector<std::pair<std::uint32_t, std::uint8_t>> Memory::changes(std::size_t limit) const {
    std::vector<std::uint32_t> keys;
    keys.reserve(pages_.size());
    for (const auto& entry : pages_) keys.push_back(entry.first);
    std::ranges::sort(keys);
    std::vector<std::pair<std::uint32_t, std::uint8_t>> out;
    for (const std::uint32_t key : keys) {
        const Page& page = *pages_.at(key);
        for (std::uint32_t i = 0; i < kPageSize && out.size() < limit; ++i) {
            const std::uint32_t address = key * kPageSize + i;
            if (page[i] != base_byte(address)) out.emplace_back(address, page[i]);
        }
        if (out.size() >= limit) break;
    }
    return out;
}

}  // namespace cb::emulate::paged_memory
