// engine/pcode_cpu_huc6280.hpp -- the PC Engine lane's HuC6280 on the one emulator (decompile/engine/pcode_emulator), on languages/huc6280.sla, in place of the own core (emulate/huc6280's interpreter, retired when this matched it).
//
//     Run r = engine::pcode_cpu::run(machine, entry, options);
//
// The machine stays the Machine's: the eight mapping registers, the ROM banks, RAM and the hardware page with the log of its accesses (every data access of the program is Machine::read and
// Machine::write). What the emulator decodes instructions from is the logical address space as the mapping registers show it (the hardware page reads $FF), made again when TAM
// changes a register and kept current by the stores into RAM. What the description does not do the adapter does as the own core does: ST0 ST1 ST2 go to the VDC whatever the mapping,
// SET makes the next ORA, AND, EOR or ADC work on the byte at zero page,X, CSL and CSH do nothing.
#pragma once

#include "emulate/huc6280.hpp"

namespace cb::engine::pcode_cpu {

// The run on the emulator.
emulate::huc6280::Run run(emulate::huc6280::Machine& machine, std::uint16_t entry, const emulate::huc6280::Options& options = {});

}  // namespace cb::engine::pcode_cpu
