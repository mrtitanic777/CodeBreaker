// engine/pcode_cpu.hpp -- the lanes' 6502 run on CodeBreaker's one emulator (decompile/engine/pcode_emulator), the p-code
// interpreter of the processor's SLEIGH description, in place of the lane's own CPU core (emulate/m6502).
//
//     RunResult r = engine::pcode_cpu::call(bus, entry, options);   // as emulate::m6502::run, on the core chosen by CB_PCODE_EMU
//
// The machine stays the lane's: the Bus (memory map, address mask, devices, the log of device accesses) is the emulator's
// read and write hook, so every data access of the program goes through Bus::read and Bus::write exactly as the own core
// makes it. What the emulator reads instructions from is its page map, a copy of the bus's memory as the processor sees it
// (the address mask's mirrors included), kept current: after an instruction that touched the bus the copy is compared with
// the bus's memory and what changed (a bank switch, a store into RAM that code runs from) is written to every mirror.
//
// The same run and the same result as emulate::m6502::run: a function is called as JSR calls it (return address pushed,
// returning when the pc reaches kReturnAddress), the registers A X Y P S as the run left them, the device accesses of the
// run in order, an instruction the NMOS 6502 documents nothing for stops the run with the same fault. Differences from the
// own core, which the description decides: ADC and SBC honour the D flag (the own core's `decimal` is off by default);
// `Variant::Cmos` runs 65c02.sla.
//
// CB_PCODE_EMU=1 (or force(true)) makes call() use the emulator; unset or 0, the own core.
#pragma once

#include <optional>

#include "emulate/m6502.hpp"

namespace cb::engine::pcode_cpu {

// Whether call() runs on the emulator: force() if set, else the environment variable CB_PCODE_EMU ("1", "true", "yes").
bool enabled();
// Override the environment (tests flip it); std::nullopt goes back to the environment.
void force(std::optional<bool> on);

// The run on the emulator, always.
emulate::m6502::RunResult run(emulate::m6502::Bus& bus, std::uint16_t entry, const emulate::m6502::RunOptions& options = {});

// emulate::m6502::run or run(), by enabled().
emulate::m6502::RunResult call(emulate::m6502::Bus& bus, std::uint16_t entry, const emulate::m6502::RunOptions& options = {});

}  // namespace cb::engine::pcode_cpu
