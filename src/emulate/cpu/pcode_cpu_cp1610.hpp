// emulate/cpu/pcode_cpu_cp1610.hpp -- the Intellivision lane's CP1610 on the one emulator (decompile/engine/pcode_emulator), on languages/CP1600.sla, in place of the own core (emulate/cp1610's interpreter, retired when this matched it).
//
//     RunResult r = emulate::pcode_cpu::run(bus, entry, options);
//
// The bus stays the lane's: every word the program reads or writes is Bus::read and Bus::write, and the three words an instruction is decoded from are read through the bus
// as the own core reads them (what the log shows of a device range is the same). The words are handed to the emulator a step at a time. SDBD is the adapter's: the context the
// description decodes the next instruction under is the one the previous instruction left.
#pragma once

#include "emulate/cp1610.hpp"

namespace cb::emulate::pcode_cpu {

// The run on the emulator.
emulate::cp1610::RunResult run(emulate::cp1610::Bus& bus, std::uint16_t entry, const emulate::cp1610::RunOptions& options = {});

}  // namespace cb::emulate::pcode_cpu
