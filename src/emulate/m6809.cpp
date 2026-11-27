// cb-unity: alone
#include "emulate/m6809.hpp"

#include "core/error.hpp"

namespace cb::emulate::m6809 {

// ---- the bus --------------------------------------------------------------------------------------------------------

Bus::Bus() : ram_(kMemorySize, 0) {}

Bus::Bus(ByteView image) : ram_(kMemorySize, 0) {
    if (image.size() > kMemorySize) throw UsageError("a 6809 image is at most 64 KB");
    std::copy(image.begin(), image.end(), ram_.begin());
}

void Bus::map(std::uint16_t lo, std::uint16_t hi, ReadFn read, WriteFn write) { ranges_.push_back({lo, hi, std::move(read), std::move(write), true}); }
void Bus::map_quiet(std::uint16_t lo, std::uint16_t hi, ReadFn read, WriteFn write) { ranges_.push_back({lo, hi, std::move(read), std::move(write), false}); }

std::uint8_t Bus::read(std::uint16_t address) {
    for (const Range& r : ranges_) {
        if (address < r.lo || address > r.hi) continue;
        const std::uint8_t v = r.read ? r.read(address) : 0;
        if (r.logged) log_.push_back({DeviceAccess::Kind::Read, address, v});
        return v;
    }
    return ram_[address];
}

void Bus::write(std::uint16_t address, std::uint8_t value) {
    for (const Range& r : ranges_) {
        if (address < r.lo || address > r.hi) continue;
        if (r.logged) log_.push_back({DeviceAccess::Kind::Write, address, value});
        if (r.write) r.write(address, value);
        return;
    }
    ram_[address] = value;
}

// ---- the run record -------------------------------------------------------------------------------------------------

std::string RunResult::exit() const {
    switch (end) {
        case End::Return: return "return";
        case End::Stopped: return "stopped: " + (fault ? fault->text() : std::string("?"));
        default: return "limit";
    }
}

}  // namespace cb::emulate::m6809
