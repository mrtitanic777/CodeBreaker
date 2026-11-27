// emulate/z80.cpp -- see z80.hpp.
#include "emulate/z80.hpp"

#include <algorithm>
#include <format>
#include <string_view>

#include "core/error.hpp"

namespace cb::emulate::z80 {

// ---- Bus ---------------------------------------------------------------------------------------------------------

Bus::Bus(PortIn in, PortOut out) : owned_(kMemorySize, 0), ram_(owned_), port_in_(std::move(in)), port_out_(std::move(out)) {}

Bus::Bus(ByteView image, PortIn in, PortOut out) : Bus(std::move(in), std::move(out)) {
    if (image.size() > kMemorySize) throw UsageError(std::format("a Z80 bus holds 64 KB, not {} bytes", image.size()));
    std::copy(image.begin(), image.end(), owned_.begin());
}

Bus Bus::over(ByteSpan ram, PortIn in, PortOut out) {
    if (ram.size() != kMemorySize) throw UsageError(std::format("a Z80 bus is 64 KB, not {} bytes", ram.size()));
    Bus bus(std::move(in), std::move(out));
    bus.owned_.clear();
    bus.owned_.shrink_to_fit();
    bus.ram_ = ram;
    return bus;
}

Bus Bus::bare(ByteSpan ram) {
    Bus bus = over(ram);
    bus.ports_ = false;
    return bus;
}

void Bus::map(std::uint16_t lo, std::uint16_t hi, ReadFn read, WriteFn write) {
    ranges_.push_back({lo, hi, std::move(read), std::move(write)});
}

std::vector<std::pair<std::uint16_t, std::uint16_t>> Bus::mapped() const {
    std::vector<std::pair<std::uint16_t, std::uint16_t>> out;
    for (const Range& r : ranges_) out.emplace_back(r.lo, r.hi);
    return out;
}

std::uint8_t Bus::read(std::uint16_t address) {
    for (const Range& r : ranges_) {
        if (r.lo <= address && address <= r.hi) {
            const std::uint8_t v = r.read ? r.read(address) : 0;
            log_.push_back({BusAccess::Kind::Read, address, v});
            return v;
        }
    }
    return ram_[address];
}

void Bus::write(std::uint16_t address, std::uint8_t value) {
    for (const Range& r : ranges_) {
        if (r.lo <= address && address <= r.hi) {
            log_.push_back({BusAccess::Kind::Write, address, value});
            if (r.write) r.write(address, value);
            return;
        }
    }
    ram_[address] = value;
}

std::uint8_t Bus::in(std::uint16_t port) {
    if (!ports_) return 0xFF;
    const std::uint8_t v = port_in_ ? port_in_(port) : 0xFF;
    log_.push_back({BusAccess::Kind::In, static_cast<std::uint8_t>(port), v});
    return v;
}

void Bus::out(std::uint16_t port, std::uint8_t value) {
    if (!ports_) return;
    log_.push_back({BusAccess::Kind::Out, static_cast<std::uint8_t>(port), value});
    if (port_out_) port_out_(port, value);
}

std::string_view to_text(BusAccess::Kind kind) {
    switch (kind) {
        case BusAccess::Kind::Read: return "r";
        case BusAccess::Kind::Write: return "w";
        case BusAccess::Kind::In: return "in";
        case BusAccess::Kind::Out: return "out";
    }
    return "?";
}

std::string Fault::text() const {
    return kind == Kind::Halt ? std::string("HALT") : std::format("undecodable at {:04X}", pc);
}

// ---- the run ----------------------------------------------------------------------------------------------------

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
    const Registers& r = result.regs;
    run.results["A"] = static_cast<std::uint64_t>(r.a);
    run.results["F"] = static_cast<std::uint64_t>(r.f);
    run.results["BC"] = static_cast<std::uint64_t>(r.bc());
    run.results["DE"] = static_cast<std::uint64_t>(r.de());
    run.results["HL"] = static_cast<std::uint64_t>(r.hl());
    run.results["IX"] = static_cast<std::uint64_t>(r.ix);
    run.results["IY"] = static_cast<std::uint64_t>(r.iy);
    run.results["SP"] = static_cast<std::uint64_t>(r.sp);
    for (const BusAccess& a : result.io) {
        cb::emulate::IoAccess io;
        io.kind = std::string(to_text(a.kind));
        io.port = a.where;
        io.value = a.value;
        run.io.push_back(std::move(io));
    }
    run.steps = result.steps;
    run.truncated = result.end == End::Limit;
    run.endian = cb::emulate::Endian::Little;
    run.raw = result;
    return run;
}

}  // namespace cb::emulate::z80
