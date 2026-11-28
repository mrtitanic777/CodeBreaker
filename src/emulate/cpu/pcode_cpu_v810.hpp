// emulate/cpu/pcode_cpu_v810.hpp -- the Virtual Boy lane's NEC V810 on CodeBreaker's one emulator (decompile/engine/pcode_emulator), on v810exact.sla.
//
//     RunResult r = emulate::pcode_cpu::run(bus, entry, options);   // call the function as `jal` would, run to its `jmp [lp]`
//
// The Bus (emulate/v810: memory ranges, devices and their log) is the emulator's read and write hook, so every data access goes through Bus::read and Bus::write as the own core
// makes it; instructions are fetched through the bus a halfword at a time. The description runs every instruction; trap, reti, halt, a divide by zero, a bit string over a
// megabit and an undefined pattern end the run with the fault the own core gives.
#pragma once

#include "emulate/v810.hpp"

namespace cb::emulate::pcode_cpu {

// Call the function at `entry` as jal would and run it until it returns (jmp [lp] to kReturnAddress), faults or takes `max_steps` instructions.
emulate::v810::RunResult run(emulate::v810::Bus& bus, std::uint32_t entry, const emulate::v810::RunOptions& options = {});

}  // namespace cb::emulate::pcode_cpu
