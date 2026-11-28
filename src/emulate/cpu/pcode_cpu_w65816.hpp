// emulate/cpu/pcode_cpu_w65816.hpp -- the Super Nintendo lane's 65C816 on CodeBreaker's one emulator (decompile/engine/pcode_emulator), the p-code interpreter of
// languages/65816.sla, in place of the lane's own core (emulate/w65816's interpreter, retired when this matched it).
//
//     emulate::w65816::Run r = emulate::pcode_cpu::run(machine, entry, options);
//
// The machine stays the lane's: the Machine (ROM map, WRAM, the chips' registers and their log) answers every data access of the program, and every byte of
// an instruction is read through it as the own core reads it, so the log of chip accesses is the same. Carried by the adapter, as the description leaves them
// out: the widths (the description's m16 and x16 context follow the M and X flags run by run), emulation mode (E: M and X set, the high bytes of X and Y clear,
// the stack in page 1), the program bank staying in place across the end of a bank, BRK, COP and RTI at both widths of the stack, the block moves, and WAI and STP
// ending the run. The same run and the same result as the own core gave.
#pragma once

#include "emulate/w65816.hpp"

namespace cb::emulate::pcode_cpu {

// The run on the emulator.
emulate::w65816::Run run(emulate::w65816::Machine& machine, std::uint32_t entry, const emulate::w65816::Options& options = {});

}  // namespace cb::emulate::pcode_cpu
