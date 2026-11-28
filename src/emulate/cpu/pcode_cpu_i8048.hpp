// emulate/cpu/pcode_cpu_i8048.hpp -- the Odyssey 2 lane's Intel 8048 on CodeBreaker's one emulator (decompile/engine/pcode_emulator), on 8048.sla, in place of the lane's own core
// (emulate/i8048's interpreter, retired when this matched it).
//
//     RunResult r = emulate::pcode_cpu::run(bus, entry, options);
//
// The Bus (4 KB of program memory, and the ports, test pins and data space through its callbacks, every access logged) is what the emulator's PORT and EXTMEM spaces and the test
// operations (getT0, getT1, getExtInt) are served from, access for access as the own core makes them; the program memory is the emulator's page map (mirrored through its 64 KB).
// A run is made as CALL makes it, one instruction at a time (the options' on_instruction is called before each). The description carries the accumulator, the carry flags, the
// registers of the active bank and the stack; the adapter carries the rest of what the own core models, and what the own core does that the description does not:
//   - the 64 bytes of internal RAM are the INTMEM space served by the adapter (an address wraps at 64, as the chip's RAM does), the stack wraps within its eight levels, and the
//     registers of the bank not selected are kept there; a change of bank (SEL RB, MOV PSW,A, RETR) swaps the registers;
//   - the ports' output latches, the timer and its flags, and the interrupt enables (the description gives them to the host as user operations); ORL, ANL, ORLD and ANLD write the
//     operand as the own core does, and read no port;
//   - DA adjusts the high digit when it is above 9 (the description compares with 0x99), MOV A,PSW gives the stack pointer as a level count (the description's is a byte count),
//     a conditional jump, DJNZ or JBn in the last two bytes of a page goes to the page of its second byte, and MOVP or JMPP in the last byte of a page reads its own page, as the
//     own core has them (the description takes the page after the whole instruction, and after the opcode).
#pragma once

#include "emulate/i8048.hpp"
#include "emulate/cpu/pcode_cpu.hpp"

namespace cb::emulate::pcode_cpu {

// The run on the emulator.
emulate::i8048::RunResult run(emulate::i8048::Bus& bus, std::uint16_t entry, const emulate::i8048::RunOptions& options);

}  // namespace cb::emulate::pcode_cpu
