// engine/pcode_cpu_m6809.hpp -- the Vectrex lane's 6809 on the one emulator (decompile/engine/pcode_emulator), on languages/6809.sla, in place of the own core (emulate/m6809).
//
//     RunResult r = engine::pcode_cpu::call(bus, entry, options);   // as emulate::m6809::run, on the core chosen by CB_PCODE_EMU
//
// Every data access of the program goes through Bus::read and Bus::write (the device ranges and their log are the bus's); what the emulator decodes instructions from is a copy of the
// bus's memory (the plain bytes behind the device ranges, as the own core decodes them), kept current. The call is made as JSR makes it, SYNC, CWAI and SWI, SWI2, SWI3 end the run with the
// reason the own core gives (the model raises no interrupt), an opcode the table has no 6809 instruction for is the same fault.
#pragma once

#include "emulate/m6809.hpp"

namespace cb::engine::pcode_cpu {

// The run on the emulator, always.
emulate::m6809::RunResult run(emulate::m6809::Bus& bus, std::uint16_t entry, const emulate::m6809::RunOptions& options = {});

// emulate::m6809::run or run(), by enabled().
emulate::m6809::RunResult call(emulate::m6809::Bus& bus, std::uint16_t entry, const emulate::m6809::RunOptions& options = {});

}  // namespace cb::engine::pcode_cpu
