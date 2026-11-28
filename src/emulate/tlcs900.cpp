// cb-unity: alone
#include "emulate/tlcs900.hpp"

namespace cb::emulate::tlcs900 {

// ---- the bus --------------------------------------------------------------------------------------------------------------------------------------------------------------

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
    const std::uint32_t a = address & 0xFFFFFF;
    Range* r = find(a);
    if (r != nullptr && r->data.empty()) {
        std::uint32_t v = r->read ? r->read(a, size) : 0;
        if (size < 4) v &= (1u << (size * 8)) - 1;
        if (r->logged) log_.push_back({DeviceAccess::Kind::Read, a, static_cast<std::uint8_t>(size), v});
        return v;
    }
    std::uint32_t v = 0;
    for (unsigned k = 0; k < size; ++k) {   // (byte by byte: an access may straddle two ranges)
        Range* byte_range = find((a + k) & 0xFFFFFF);
        if (byte_range == nullptr) continue;
        if (byte_range->data.empty()) {
            const std::uint32_t b = byte_range->read ? byte_range->read((a + k) & 0xFFFFFF, 1) : 0;
            v |= (b & 0xFF) << (8 * k);
            if (byte_range->logged) log_.push_back({DeviceAccess::Kind::Read, (a + k) & 0xFFFFFF, 1, b & 0xFF});
        } else {
            v |= static_cast<std::uint32_t>(byte_range->data[((a + k) - byte_range->lo) % byte_range->data.size()]) << (8 * k);
        }
    }
    if (r != nullptr && r->logged && !r->data.empty()) log_.push_back({DeviceAccess::Kind::Read, a, static_cast<std::uint8_t>(size), v});
    return v;
}

void Bus::write(std::uint32_t address, unsigned size, std::uint32_t value) {
    const std::uint32_t a = address & 0xFFFFFF;
    if (size < 4) value &= (1u << (size * 8)) - 1;
    Range* r = find(a);
    if (r != nullptr && r->data.empty()) {
        if (r->logged) log_.push_back({DeviceAccess::Kind::Write, a, static_cast<std::uint8_t>(size), value});
        if (r->write) r->write(a, size, value);
        return;
    }
    if (r != nullptr && r->logged) log_.push_back({DeviceAccess::Kind::Write, a, static_cast<std::uint8_t>(size), value});
    for (unsigned k = 0; k < size; ++k) {
        Range* byte_range = find((a + k) & 0xFFFFFF);
        if (byte_range == nullptr) continue;
        const std::uint8_t b = static_cast<std::uint8_t>(value >> (8 * k));
        if (byte_range->data.empty()) {
            if (byte_range != r && byte_range->logged) log_.push_back({DeviceAccess::Kind::Write, (a + k) & 0xFFFFFF, 1, b});
            if (byte_range->write) byte_range->write((a + k) & 0xFFFFFF, 1, b);
        } else if (byte_range->writable) {
            byte_range->data[((a + k) - byte_range->lo) % byte_range->data.size()] = b;
        }
    }
}

std::span<std::uint8_t> Bus::bytes(std::uint32_t lo) {
    for (Range& r : ranges_) {
        if (r.lo == lo && !r.data.empty()) return r.data;
    }
    return {};
}

// ---- the registers ------------------------------------------------------------------------------------------------------------------------------------------------------

namespace {

constexpr std::uint32_t mask_of(unsigned size) { return size >= 4 ? 0xFFFFFFFFu : (1u << (8 * size)) - 1; }

struct Slot {
    std::uint32_t* word;
    unsigned pos;
};

Slot slot_of(Registers& g, std::uint8_t code) {
    unsigned bank, group;
    if (code >= 0xE0) {
        bank = g.rfp();
        group = (code >> 2) & 7u;
    } else if (code >= 0xD0) {
        bank = (g.rfp() + 3) & 3u;
        group = (code >> 2) & 3u;
    } else {
        bank = (code >> 4) & 3u;
        group = (code & 0x0Fu) >> 2;
    }
    std::uint32_t* word = group < 4 ? &g.bank[bank][group] : group == 4 ? &g.xix : group == 5 ? &g.xiy : group == 6 ? &g.xiz : &g.xsp;
    return {word, code & 3u};
}

}  // namespace

std::uint32_t Registers::read(std::uint8_t code, unsigned size) const {
    const Slot s = slot_of(const_cast<Registers&>(*this), code);
    return (*s.word >> (8 * s.pos)) & mask_of(size);
}

void Registers::write(std::uint8_t code, unsigned size, std::uint32_t value) {
    const Slot s = slot_of(*this, code);
    const std::uint32_t mask = mask_of(size) << (8 * s.pos);
    *s.word = (*s.word & ~mask) | ((value << (8 * s.pos)) & mask);
}

// ---- a run's result ----------------------------------------------------------------------------------------------------------------------------------------------------

std::string RunResult::exit() const {
    switch (end) {
        case End::Return: return "return";
        case End::Stopped: return "stopped: " + (fault ? fault->text() : std::string("?"));
        default: return "limit";
    }
}

}  // namespace cb::emulate::tlcs900
