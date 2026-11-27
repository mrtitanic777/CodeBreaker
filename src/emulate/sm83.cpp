// emulate/sm83.cpp -- see sm83.hpp.
#include "emulate/sm83.hpp"

#include <format>
#include <string>

namespace cb::emulate::sm83 {

std::string Fault::text() const {
    switch (kind) {
        case Kind::Halt: return "HALT";
        case Kind::Stop: return "STOP";
        case Kind::Undecodable: break;
    }
    return std::format("no instruction at {:04X}", pc);
}

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
    run.results["SP"] = static_cast<std::uint64_t>(r.sp);
    for (const BusAccess& a : result.io) {
        cb::emulate::IoAccess io;
        io.kind = std::string(z80::to_text(a.kind));
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

}  // namespace cb::emulate::sm83
