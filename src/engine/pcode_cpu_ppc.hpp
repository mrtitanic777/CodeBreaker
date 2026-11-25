// engine/pcode_cpu_ppc.hpp -- the PowerPC adapter's entry for a chosen description (the PowerPC lanes call pcode_cpu::run / pcode_cpu::call of pcode_cpu.hpp, which are the AltiVec one).
#pragma once

#include <cstdint>
#include <span>

#include "emulate/powerpc.hpp"
#include "engine/pcode_ops_ppc.hpp"

namespace cb::engine::pcode_cpu {

// The run on the emulator, on ppc_64_isa_altivec_be.sla (the Cell, and Gekko with `options.paired_singles`) or ppc_64_xenon_be.sla (VMX128, the Xbox 360's Xenon).
emulate::powerpc::RunResult run(pcode_ops_ppc::Flavor flavor, std::span<const emulate::powerpc::Region> regions, std::uint32_t entry, std::span<const std::uint64_t> args,
                                const emulate::powerpc::RunOptions& options);

// Whether the interpreter has an instruction for the word (what the adapter stops on, with `unsupported`, where it has none): the tests compare this with the interpreter.
bool ppc_interpreter_handles(std::uint32_t word, bool paired);

// The emulator of a flavor with its registers and the context its bound operations use, for the tests that run an instruction apart from a run.
struct PpcProbe {
    decompile::sleigh::Emulator& emu;
    const pcode_ops_ppc::Regs& regs;
    pcode_ops_ppc::VectorContext& vector;
    const pcode_ops::Binding& binding;
};
PpcProbe ppc_probe(pcode_ops_ppc::Flavor flavor);

// run(), or the interpreter's run where the emulator is off (enabled() of pcode_cpu.hpp): the lanes' call, for a chosen description.
emulate::powerpc::RunResult call(pcode_ops_ppc::Flavor flavor, std::span<const emulate::powerpc::Region> regions, std::uint32_t entry, std::span<const std::uint64_t> args,
                                 const emulate::powerpc::RunOptions& options);

}  // namespace cb::engine::pcode_cpu
