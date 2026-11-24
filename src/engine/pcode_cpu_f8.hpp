// engine/pcode_cpu_f8.hpp -- the Channel F lane's Fairchild F8 on CodeBreaker's one emulator (decompile/engine/pcode_emulator), on f8.sla, in place of the lane's own core (emulate/f8).
//
//     RunResult r = engine::pcode_cpu::call(bus, entry, options);   // as emulate::f8::run, on the core chosen by CB_PCODE_EMU
//
// The Bus (64 KB of memory, some of it writable, and the ports with their log) is the emulator's read and write hook: every memory access is Bus::read / Bus::write, every IN and OUT
// is Bus::port_in / Bus::port_out. The 64-byte scratchpad is the description's SPAD space and the registers are the description's. A run is made as PI makes it (PC1 holds
// kReturnAddress, and a POP that lands on it ends the run), one instruction at a time (the own core's on_instruction is called before each). What the own core does and the
// description does not is the adapter's: PI and JMP load the accumulator with the high byte of the address; the decimal adds (the description leaves them to the host:
// bcdAdd and bcdCarry) are the own core's; and the overflow flag of CI and CM, which the description sets from either of two partial sums, is the flag of the whole
// subtraction.
#pragma once

#include "emulate/f8.hpp"
#include "engine/pcode_cpu.hpp"

namespace cb::engine::pcode_cpu {

// The run on the emulator, always.
emulate::f8::RunResult run(emulate::f8::Bus& bus, std::uint16_t entry, const emulate::f8::RunOptions& options);

// emulate::f8::run or run(), by enabled().
emulate::f8::RunResult call(emulate::f8::Bus& bus, std::uint16_t entry, const emulate::f8::RunOptions& options);

}  // namespace cb::engine::pcode_cpu
