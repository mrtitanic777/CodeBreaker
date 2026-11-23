// cb-unity: alone (file-local names that other files in a batch also use)
// engine/pcode_cpu_ee.cpp -- the PlayStation 2 lane's Emotion Engine on the one p-code emulator: emulate::ee::run's call, on languages/mips64le.sla and the shared run of
// pcode_cpu_mipsle.cpp. What the run does is the own core's start (the registers a function is called with, Status as the kernel leaves it) and end (the result's fields).
#include "engine/pcode_cpu_ee.hpp"

#include "engine/pcode_cpu_mipsle.hpp"

namespace cb::engine::pcode_cpu {

namespace ee = cb::emulate::ee;
namespace ml = cb::emulate::mipsle;

ee::RunResult run(ee::Bus& bus, std::uint32_t entry, const ee::RunOptions& options) {
    ml::Cpu cpu(ml::Model::R5900, bus);
    const auto sx = [](std::uint32_t v) { return static_cast<std::uint64_t>(static_cast<std::int64_t>(static_cast<std::int32_t>(v))); };
    for (std::size_t i = 0; i < 4; ++i) cpu.r[4 + i] = sx(options.a[i]);
    for (std::size_t i = 0; i < 8; ++i) {
        cpu.r[16 + i] = sx(options.s[i]);
        cpu.r[8 + i] = sx(options.t[i]);
    }
    cpu.r[28] = sx(options.gp);
    cpu.r[29] = sx(options.sp);
    cpu.r[31] = sx(ee::kReturnAddress);
    cpu.cop0[12] = 0x70000000;   // Status as the kernel leaves it for a program: the coprocessors usable
    cpu.jump_to(entry);
    const std::size_t io_before = bus.log().size(), calls_before = bus.syscalls().size();
    MipsLeMachine machine;
    machine.peek = [&](std::uint32_t address, unsigned size, std::uint64_t& value) { return bus.peek(address, size, value); };
    machine.is_stub = [](std::uint32_t) { return false; };
    const MipsLeEnd end = run_mipsle(cpu, machine, ee::kReturnAddress, options.max_steps);
    ee::RunResult result;
    result.end = end.kind == MipsLeEnd::Kind::Return ? ee::End::Return : end.kind == MipsLeEnd::Kind::Stopped ? ee::End::Stopped : ee::End::Limit;
    result.fault = end.fault;
    result.r = cpu.r;
    result.rh = cpu.rh;
    result.hi = cpu.hi;
    result.lo = cpu.lo;
    result.f0 = cpu.fpr[0];
    result.steps = cpu.steps;
    result.io.assign(bus.log().begin() + static_cast<std::ptrdiff_t>(io_before), bus.log().end());
    result.syscalls.assign(bus.syscalls().begin() + static_cast<std::ptrdiff_t>(calls_before), bus.syscalls().end());
    return result;
}

ee::RunResult call(ee::Bus& bus, std::uint32_t entry, const ee::RunOptions& options) { return enabled() ? pcode_cpu::run(bus, entry, options) : ee::run(bus, entry, options); }

}  // namespace cb::engine::pcode_cpu
