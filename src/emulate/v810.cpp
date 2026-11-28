// cb-unity: alone
#include "emulate/v810.hpp"

namespace cb::emulate::v810 {

// ---- the bus --------------------------------------------------------------------------------------------------------

void Bus::memory(std::uint32_t lo, std::uint32_t hi, ByteView initial, bool writable, bool logged, std::size_t size) {
    if (size == 0) size = static_cast<std::size_t>(hi) - lo + 1;
    Range r{lo, hi, Bytes(size, 0), writable, logged, {}, {}};
    std::copy(initial.begin(), initial.begin() + static_cast<std::ptrdiff_t>(std::min(initial.size(), size)), r.data.begin());
    ranges_.push_back(std::move(r));
}

void Bus::device(std::uint32_t lo, std::uint32_t hi, ReadFn read, WriteFn write, bool logged) {
    ranges_.push_back({lo, hi, {}, true, logged, std::move(read), std::move(write)});
}

Bus::Range* Bus::find(std::uint32_t address) {
    for (Range& r : ranges_) {
        if (address >= r.lo && address <= r.hi) return &r;
    }
    return nullptr;
}

std::uint32_t Bus::read(std::uint32_t address, unsigned size) {
    const std::uint32_t a = (address & mask_) & ~(size - 1);
    Range* r = find(a);
    if (r == nullptr) return 0;
    std::uint32_t v = 0;
    if (r->data.empty()) {
        v = r->read ? r->read(a, size) : 0;
        if (size < 4) v &= (1u << (size * 8)) - 1;
    } else {
        const std::size_t base = (a - r->lo) % r->data.size();
        for (unsigned i = 0; i < size; ++i) v |= static_cast<std::uint32_t>(r->data[(base + i) % r->data.size()]) << (8 * i);
    }
    if (r->logged) log_.push_back({DeviceAccess::Kind::Read, a, static_cast<std::uint8_t>(size), v});
    return v;
}

void Bus::write(std::uint32_t address, unsigned size, std::uint32_t value) {
    const std::uint32_t a = (address & mask_) & ~(size - 1);
    Range* r = find(a);
    if (r == nullptr) return;
    if (size < 4) value &= (1u << (size * 8)) - 1;
    if (r->logged) log_.push_back({DeviceAccess::Kind::Write, a, static_cast<std::uint8_t>(size), value});
    if (r->data.empty()) {
        if (r->write) r->write(a, size, value);
    } else if (r->writable) {
        const std::size_t base = (a - r->lo) % r->data.size();
        for (unsigned i = 0; i < size; ++i) r->data[(base + i) % r->data.size()] = static_cast<std::uint8_t>(value >> (8 * i));
    }
}

std::optional<std::uint32_t> Bus::peek(std::uint32_t address, unsigned size) const {
    const std::uint32_t a = (address & mask_) & ~(size - 1);
    for (const Range& r : ranges_) {
        if (a < r.lo || a > r.hi) continue;
        if (r.data.empty() || r.logged) return std::nullopt;
        std::uint32_t v = 0;
        const std::size_t base = (a - r.lo) % r.data.size();
        for (unsigned i = 0; i < size; ++i) v |= static_cast<std::uint32_t>(r.data[(base + i) % r.data.size()]) << (8 * i);
        return v;
    }
    return 0u;   // (an address no range holds reads 0 and logs nothing)
}

std::span<std::uint8_t> Bus::bytes(std::uint32_t lo) {
    for (Range& r : ranges_) {
        if (r.lo == lo && !r.data.empty()) return r.data;
    }
    return {};
}

// ---- a run's result ------------------------------------------------------------------------------------------------

std::string RunResult::exit() const {
    switch (end) {
        case End::Return: return "return";
        case End::Stopped: return "stopped: " + (fault ? fault->text() : std::string("?"));
        default: return "limit";
    }
}

}  // namespace cb::emulate::v810
