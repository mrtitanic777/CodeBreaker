// engine/pcode_cpu_sm83.hpp -- the Game Boy lane's SM83 on CodeBreaker's one emulator (decompile/engine/pcode_emulator), on sm83.sla, in place of the lane's own core (emulate/sm83).
//
//     RunResult r = engine::pcode_cpu::call(bus, entry, options);   // as emulate::sm83::run, on the core chosen by CB_PCODE_EMU
//
// The Bus (the Z80 module's: memory, the device ranges and their log) is the emulator's read and write hook, so every data access goes through Bus::read and Bus::write as
// the own core makes it; the emulator decodes from a copy of the bus's 64 KB, kept current. The call is made as CALL makes it (return address pushed, kReturnAddress ends the
// run). What the description leaves to the machine is the adapter's: HALT (the Halt fault from the next instruction on), STOP (the Stop fault, pc past it, the step not counted),
// the interrupt-master flag (EI, DI, RETI), and an opcode the CPU does not define (the Undecodable fault, pc at it, the step not counted). `10 nn` with nn not zero is STOP
// for the own core (a STOP is two bytes whatever the second is); the description makes it an undefined opcode, and the adapter gives it the own core's meaning.
#pragma once

#include "emulate/sm83.hpp"
#include "engine/pcode_cpu.hpp"

namespace cb::engine::pcode_cpu {

// The run on the emulator, always. (The options are always given: the Z80 overloads share the Bus type, so a default would make a call with two arguments ambiguous.)
emulate::sm83::RunResult run(emulate::sm83::Bus& bus, std::uint16_t entry, const emulate::sm83::RunOptions& options);

// emulate::sm83::run or run(), by enabled().
emulate::sm83::RunResult call(emulate::sm83::Bus& bus, std::uint16_t entry, const emulate::sm83::RunOptions& options);

}  // namespace cb::engine::pcode_cpu
