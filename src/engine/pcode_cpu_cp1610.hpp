// engine/pcode_cpu_cp1610.hpp -- the Intellivision lane's CP1610 on the one emulator (decompile/engine/pcode_emulator), on languages/CP1600.sla, in place of the own core (emulate/cp1610).
//
//     RunResult r = engine::pcode_cpu::call(bus, entry, options);   // as emulate::cp1610::run, on the core chosen by CB_PCODE_EMU
//
// The bus stays the lane's: every word the program reads or writes is Bus::read and Bus::write, and the three words an instruction is decoded from are read through the bus
// as the own core reads them (what the log shows of a device range is the same). The words are handed to the emulator a step at a time. SDBD is the adapter's: the context the
// description decodes the next instruction under is the one the previous instruction left.
#pragma once

#include "emulate/cp1610.hpp"

namespace cb::engine::pcode_cpu {

// The run on the emulator, always.
emulate::cp1610::RunResult run(emulate::cp1610::Bus& bus, std::uint16_t entry, const emulate::cp1610::RunOptions& options = {});

// emulate::cp1610::run or run(), by enabled().
emulate::cp1610::RunResult call(emulate::cp1610::Bus& bus, std::uint16_t entry, const emulate::cp1610::RunOptions& options = {});

}  // namespace cb::engine::pcode_cpu
