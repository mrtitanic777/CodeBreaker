// emulate/m6502.cpp -- see m6502.hpp.
#include "emulate/m6502.hpp"

#include <algorithm>
#include <format>

#include "core/error.hpp"

namespace cb::emulate::m6502 {

// ---- Bus ---------------------------------------------------------------------------------------------------------

Bus::Bus() : owned_(kMemorySize, 0), ram_(owned_) {}

Bus::Bus(ByteView image) : owned_(kMemorySize, 0), ram_(owned_) {
    if (image.size() > kMemorySize) throw UsageError(std::format("a 6502 bus holds 64 KB, not {} bytes", image.size()));
    std::copy(image.begin(), image.end(), owned_.begin());
}

Bus Bus::over(std::span<std::uint8_t> ram) {
    if (ram.size() != kMemorySize) throw UsageError(std::format("a 6502 bus is 64 KB, not {} bytes", ram.size()));
    Bus bus;
    bus.owned_.clear();
    bus.owned_.shrink_to_fit();
    bus.ram_ = ram;
    return bus;
}

void Bus::map(std::uint16_t lo, std::uint16_t hi, ReadFn read, WriteFn write) {
    ranges_.push_back({lo, hi, std::move(read), std::move(write), true});
}

void Bus::map_quiet(std::uint16_t lo, std::uint16_t hi, ReadFn read, WriteFn write) {
    ranges_.push_back({lo, hi, std::move(read), std::move(write), false});
}

void Bus::map_writes(std::uint16_t lo, std::uint16_t hi, WriteFn write) {
    ranges_.push_back({lo, hi, {}, std::move(write), true, false});
}

std::uint8_t Bus::read(std::uint16_t address) {
    address &= mask_;
    for (const Range& r : ranges_) {
        if (r.reads && r.lo <= address && address <= r.hi) {
            const std::uint8_t v = r.read ? r.read(address) : 0;
            if (r.logged) log_.push_back({DeviceAccess::Kind::Read, address, v});
            return v;
        }
    }
    return ram_[address];
}

std::uint8_t Bus::peek(std::uint16_t address) const {
    address &= mask_;
    for (const Range& r : ranges_) {
        if (r.reads && r.lo <= address && address <= r.hi) return r.logged ? ram_[address] : (r.read ? r.read(address) : std::uint8_t{0});
    }
    return ram_[address];
}

std::vector<std::pair<std::uint16_t, std::uint16_t>> Bus::logged_reads() const {
    std::vector<std::pair<std::uint16_t, std::uint16_t>> out;
    for (const Range& r : ranges_)
        if (r.logged && r.reads) out.emplace_back(r.lo, r.hi);
    return out;
}

void Bus::write(std::uint16_t address, std::uint8_t value) {
    address &= mask_;
    for (const Range& r : ranges_) {
        if (r.lo <= address && address <= r.hi) {
            if (r.logged) log_.push_back({DeviceAccess::Kind::Write, address, value});
            if (r.write) r.write(address, value);
            return;
        }
    }
    ram_[address] = value;
}

// ---- the run record ----------------------------------------------------------------------------------------------

std::string Fault::text() const { return std::format("undocumented opcode {:02X} at {:04X}", opcode, pc); }

std::string RunResult::exit() const {
    switch (end) {
        case End::Return: return "return";
        case End::Stopped: return "stopped: " + (fault ? fault->text() : std::string());
        case End::Limit: break;
    }
    return "limit";
}

cb::emulate::Run record(const RunResult& result) {
    cb::emulate::Run run;
    run.exit = result.exit();
    run.results["A"] = static_cast<std::uint64_t>(result.regs.a);
    run.results["X"] = static_cast<std::uint64_t>(result.regs.x);
    run.results["Y"] = static_cast<std::uint64_t>(result.regs.y);
    run.results["P"] = static_cast<std::uint64_t>(result.regs.p);
    run.results["S"] = static_cast<std::uint64_t>(result.regs.s);
    for (const DeviceAccess& a : result.io) {
        cb::emulate::IoAccess io;
        io.kind = a.kind == DeviceAccess::Kind::Read ? "r" : "w";
        io.port = a.address;
        io.value = a.value;
        run.io.push_back(std::move(io));
    }
    run.steps = result.steps;
    run.truncated = result.end == End::Limit;
    run.endian = cb::emulate::Endian::Little;
    run.raw = result;
    return run;
}

}  // namespace cb::emulate::m6502
