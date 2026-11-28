// emulate/cpu/pcode_cpu_tlcs900.hpp -- the Neo Geo Pocket lane's TLCS-900/H on CodeBreaker's one emulator (decompile/engine/pcode_emulator), the p-code interpreter of
// languages/tlcs900exact.sla.
//
//     emulate::tlcs900::RunResult r = emulate::pcode_cpu::run(bus, entry, options);   // call the function as `call` would, run to its `ret`
//
// The machine stays the lane's: the Bus answers every data access and every byte of an instruction is read through it as the processor reads it. The description runs
// every instruction; the BIOS (`swi`) and HALT are the machine's (see pcode_cpu_tlcs900.cpp). The retired interpreter's runs are kept as digests (pcode_cpu_tlcs900_test).
#pragma once

#include "emulate/tlcs900.hpp"

namespace cb::emulate::pcode_cpu {

// Call the function at `entry` as `call` would and run it until it returns (`ret` to kReturnAddress), faults or takes `max_steps` instructions.
emulate::tlcs900::RunResult run(emulate::tlcs900::Bus& bus, std::uint32_t entry, const emulate::tlcs900::RunOptions& options = {});

}  // namespace cb::emulate::pcode_cpu
