// engine/pcode_cpu_allegrex.hpp -- the PSP lane's Allegrex on CodeBreaker's one emulator (decompile/engine/pcode_emulator), in place of the lane's own core (emulate/allegrex,
// emulate/mips_le).
//
//     RunResult r = engine::pcode_cpu::call(bus, entry, options);   // as emulate::allegrex::run, on the core chosen by CB_PCODE_EMU
//
// There is no description of the Allegrex: languages/mips32le.sla, the closest MIPS, runs the integer instructions (see pcode_cpu_mipsle.hpp for which) with the Bus as its memory,
// and the adapter's own semantics (pcode_ops_mipsle.cpp) run the rest: the FPU and the VFPU's not-run end, CLZ CLO MADD MSUB MAX MIN, EXT INS SEB SEH WSBH BITREV ROTR, the unaligned
// loads and stores, LL and SC, SYSCALL and the faults, and the imported functions' stubs the Bus answers.
#pragma once

#include "emulate/allegrex.hpp"
#include "engine/pcode_cpu.hpp"

namespace cb::engine::pcode_cpu {

// The run on the emulator, always.
emulate::allegrex::RunResult run(emulate::allegrex::Bus& bus, std::uint32_t entry, const emulate::allegrex::RunOptions& options = {});

// emulate::allegrex::run or run(), by enabled().
emulate::allegrex::RunResult call(emulate::allegrex::Bus& bus, std::uint32_t entry, const emulate::allegrex::RunOptions& options = {});

}  // namespace cb::engine::pcode_cpu
