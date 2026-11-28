// emulate/cpu/pcode_cpu_sm83.hpp -- the Game Boy lane's SM83 on CodeBreaker's one emulator (decompile/engine/pcode_emulator), on sm83.sla, the lane's only SM83.
//
//     RunResult r = emulate::pcode_cpu::run(bus, entry, options);   // the lane's only core (the old emulate::sm83 interpreter is retired)
//
// The Bus (the Z80 module's: memory, the device ranges and their log) is the emulator's read and write hook, so every data access goes through Bus::read and Bus::write as
// the own core makes it; the emulator decodes from a copy of the bus's 64 KB, kept current. The call is made as CALL makes it (return address pushed, kReturnAddress ends the
// run). What the description leaves to the machine is the adapter's: HALT (the Halt fault from the next instruction on), STOP (the Stop fault, pc past it, the step not counted),
// the interrupt-master flag (EI, DI, RETI), and an opcode the CPU does not define (the Undecodable fault, pc at it, the step not counted). `10 nn` with nn not zero is STOP
// for the own core (a STOP is two bytes whatever the second is); the description makes it an undefined opcode, and the adapter gives it the own core's meaning.
#pragma once

#include <memory>
#include <optional>

#include "emulate/sm83.hpp"
#include "emulate/cpu/pcode_cpu.hpp"

namespace cb::emulate::pcode_cpu {

// The run on the emulator, always. (The options are always given: the Z80 overloads share the Bus type, so a default would make a call with two arguments ambiguous.)
emulate::sm83::RunResult run(emulate::sm83::Bus& bus, std::uint16_t entry, const emulate::sm83::RunOptions& options);

// The SM83 one instruction at a time on the emulator (the retired emulate::sm83::Cpu's shape): `regs` (the interrupt-master flag and HALT among them) and `steps` are the processor's;
// step() runs the instruction at regs.pc. A Fault: it did not run (an earlier HALT; no instruction there), nothing changed, or it was STOP (pc past it, not a step).
// What is put in the bus's memory between steps is seen by the next one. The thread's emulator is shared: a run or another Sm83Cpu between steps is allowed.
class Sm83Session;
class Sm83Cpu {
public:
    explicit Sm83Cpu(emulate::sm83::Bus& bus);
    ~Sm83Cpu();
    Sm83Cpu(const Sm83Cpu&) = delete;
    Sm83Cpu& operator=(const Sm83Cpu&) = delete;

    emulate::sm83::Registers regs;
    std::uint64_t steps = 0;

    std::optional<emulate::sm83::Fault> step();

private:
    std::unique_ptr<Sm83Session> session_;
};


}  // namespace cb::emulate::pcode_cpu
