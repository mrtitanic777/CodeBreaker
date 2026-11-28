// cb-unity: alone (file-local names that other files in a batch also use)
// emulate/cpu/pcode_cpu_allegrex.cpp -- the PSP lane's Allegrex on the one p-code emulator: emulate::allegrex::run's call, on languages/mips32le.sla and the shared run of
// pcode_cpu_mipsle.cpp. What the run does is the lane's own start (the registers a function is called with) and end (the result's fields).
#include "emulate/cpu/pcode_cpu_allegrex.hpp"

#include "emulate/cpu/pcode_cpu_mipsle.hpp"

namespace cb::emulate::pcode_cpu {

namespace al = cb::emulate::allegrex;
namespace ml = cb::emulate::mipsle;

al::RunResult run(al::Bus& bus, std::uint32_t entry, const al::RunOptions& options) {
    ml::Cpu cpu(ml::Model::Allegrex, bus);
    const auto sx = [](std::uint32_t v) { return static_cast<std::uint64_t>(static_cast<std::int64_t>(static_cast<std::int32_t>(v))); };
    for (std::size_t i = 0; i < 4; ++i) cpu.r[4 + i] = sx(options.a[i]);
    for (std::size_t i = 0; i < 8; ++i) {
        cpu.r[16 + i] = sx(options.s[i]);
        cpu.r[8 + i] = sx(options.t[i]);
    }
    cpu.r[28] = sx(options.gp);
    cpu.r[29] = sx(options.sp);
    cpu.r[31] = sx(al::kReturnAddress);
    cpu.jump_to(entry);
    const std::size_t io_before = bus.log().size(), calls_before = bus.calls().size();
    MipsLeMachine machine;
    machine.peek = [&](std::uint32_t address, unsigned size, std::uint64_t& value) { return bus.peek(address, size, value); };
    machine.is_stub = [&](std::uint32_t address) { return bus.is_stub(address); };
    const MipsLeEnd end = run_mipsle(cpu, machine, al::kReturnAddress, options.max_steps);
    al::RunResult result;
    result.end = end.kind == MipsLeEnd::Kind::Return ? al::End::Return : end.kind == MipsLeEnd::Kind::Stopped ? al::End::Stopped : al::End::Limit;
    result.fault = end.fault;
    result.r = cpu.r;
    result.hi = cpu.hi;
    result.lo = cpu.lo;
    result.f0 = cpu.fpr[0];
    result.steps = cpu.steps;
    result.io.assign(bus.log().begin() + static_cast<std::ptrdiff_t>(io_before), bus.log().end());
    result.calls.assign(bus.calls().begin() + static_cast<std::ptrdiff_t>(calls_before), bus.calls().end());
    return result;
}

al::RunResult call(al::Bus& bus, std::uint32_t entry, const al::RunOptions& options) { return enabled() ? pcode_cpu::run(bus, entry, options) : al::run(bus, entry, options); }

}  // namespace cb::emulate::pcode_cpu
