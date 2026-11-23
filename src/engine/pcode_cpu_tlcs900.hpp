// engine/pcode_cpu_tlcs900.hpp -- the Neo Geo Pocket lane's TLCS-900/H on CodeBreaker's one emulator (decompile/engine/pcode_emulator), the p-code interpreter of
// languages/tlcs900.sla, with the lane's own core (emulate/tlcs900) for what the description does not carry.
//
//     emulate::tlcs900::RunResult r = engine::pcode_cpu::call(bus, entry, options);   // as emulate::tlcs900::run, on the core chosen by CB_PCODE_EMU
//
// The machine stays the lane's: the Bus answers every data access and every byte of an instruction is read through it as the own core reads it. The emulator runs
// the instructions whose description gives the own core's result (see pcode_cpu_tlcs900.cpp for which); the rest run on the own core, one instruction at a time, on the
// registers as they stand. The same run and the same result as emulate::tlcs900::run.
#pragma once

#include "emulate/tlcs900.hpp"

namespace cb::engine::pcode_cpu {

// The run on the emulator, always.
emulate::tlcs900::RunResult run(emulate::tlcs900::Bus& bus, std::uint32_t entry, const emulate::tlcs900::RunOptions& options = {});

// emulate::tlcs900::run or run(), by pcode_cpu::enabled().
emulate::tlcs900::RunResult call(emulate::tlcs900::Bus& bus, std::uint32_t entry, const emulate::tlcs900::RunOptions& options = {});

}  // namespace cb::engine::pcode_cpu
