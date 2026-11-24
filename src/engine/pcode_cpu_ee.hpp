// engine/pcode_cpu_ee.hpp -- the PlayStation 2 lane's Emotion Engine on CodeBreaker's one emulator (decompile/engine/pcode_emulator), in place of the lane's own core
// (emulate/ee, emulate/mips_le).
//
//     RunResult r = engine::pcode_cpu::call(bus, entry, options);   // as emulate::ee::run, on the core chosen by CB_PCODE_EMU
//
// There is no description of the R5900: languages/mips64le.sla, the closest MIPS, runs the 64-bit integer instructions (see pcode_cpu_mipsle.hpp for which) with the Bus as its
// memory, and the own core runs the rest: the FPU (no infinities or NaNs), coprocessors 0 and 2, the 128-bit registers and multimedia instructions, the second multiplier
// and the three-operand multiplies, the shift amount register, the unaligned loads and stores, SYSCALL (the kernel calls the Bus stubs) and the faults.
#pragma once

#include "emulate/ee.hpp"
#include "engine/pcode_cpu.hpp"

namespace cb::engine::pcode_cpu {

// The run on the emulator, always.
emulate::ee::RunResult run(emulate::ee::Bus& bus, std::uint32_t entry, const emulate::ee::RunOptions& options = {});

// emulate::ee::run or run(), by enabled().
emulate::ee::RunResult call(emulate::ee::Bus& bus, std::uint32_t entry, const emulate::ee::RunOptions& options = {});

}  // namespace cb::engine::pcode_cpu
