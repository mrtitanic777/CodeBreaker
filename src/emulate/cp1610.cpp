// cb-unity: alone
#include "emulate/cp1610.hpp"

namespace cb::emulate::cp1610 {

// ---- the bus --------------------------------------------------------------------------------------------------------

void Bus::memory(std::uint16_t lo, std::uint16_t hi, std::span<const std::uint16_t> initial, bool writable, bool logged, std::uint16_t mask) {
    Range r{lo, hi, std::vector<std::uint16_t>(static_cast<std::size_t>(hi) - lo + 1, 0), true, writable, logged, mask, {}, {}};
    std::copy(initial.begin(), initial.begin() + static_cast<std::ptrdiff_t>(std::min(initial.size(), r.data.size())), r.data.begin());
    ranges_.push_back(std::move(r));
}

void Bus::device(std::uint16_t lo, std::uint16_t hi, ReadFn read, WriteFn write, bool logged, std::uint16_t mask) {
    ranges_.push_back({lo, hi, {}, false, true, logged, mask, std::move(read), std::move(write)});
}

std::uint16_t Bus::read(std::uint16_t address) {
    for (Range& r : ranges_) {
        if (address < r.lo || address > r.hi) continue;
        const std::uint16_t v = r.is_memory ? r.data[address - r.lo] : (r.read ? r.read(address) : 0);
        if (r.logged) log_.push_back({DeviceAccess::Kind::Read, address, v});
        return v;
    }
    return 0;
}

void Bus::write(std::uint16_t address, std::uint16_t value) {
    for (Range& r : ranges_) {
        if (address < r.lo || address > r.hi) continue;
        value &= r.mask;
        if (r.logged) log_.push_back({DeviceAccess::Kind::Write, address, value});
        if (r.is_memory) {
            if (r.writable) r.data[address - r.lo] = value;
        } else if (r.write) {
            r.write(address, value);
        }
        return;
    }
}

std::span<std::uint16_t> Bus::words(std::uint16_t lo) {
    for (Range& r : ranges_) {
        if (r.lo == lo && r.is_memory) return r.data;
    }
    return {};
}

// ---- the run record -------------------------------------------------------------------------------------------------

std::string RunResult::exit() const {
    switch (end) {
        case End::Return: return "return";
        case End::Stopped: return "stopped: " + (fault ? fault->text() : std::string("?"));
        default: return "limit";
    }
}

}  // namespace cb::emulate::cp1610
