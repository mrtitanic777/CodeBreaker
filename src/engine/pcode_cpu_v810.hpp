// engine/pcode_cpu_v810.hpp -- the Virtual Boy lane's NEC V810 on CodeBreaker's one emulator (decompile/engine/pcode_emulator), on v810.sla, in place of the lane's own core
// (emulate/v810).
//
//     RunResult r = engine::pcode_cpu::call(bus, entry, options);   // as emulate::v810::run, on the core chosen by CB_PCODE_EMU
//
// The Bus (emulate/v810: memory ranges, devices and their log) is the emulator's read and write hook, so every data access goes through Bus::read and Bus::write as the own core
// makes it; instructions are fetched from the bus's memory a word at a time. The integer instructions, the branches, jr / jal / jmp and the loads and stores run on the
// description. What it does not carry is the own core's: the floating-point instructions (the reserved-operand, zero-divide, overflow and underflow flags), the bit strings,
// ldsr / stsr, caxi, a divide by zero, trap / reti / halt, and an undefined pattern.
#pragma once

#include "emulate/v810.hpp"
#include "engine/pcode_cpu.hpp"

namespace cb::engine::pcode_cpu {

// The run on the emulator, always.
emulate::v810::RunResult run(emulate::v810::Bus& bus, std::uint32_t entry, const emulate::v810::RunOptions& options = {});

// emulate::v810::run or run(), by enabled().
emulate::v810::RunResult call(emulate::v810::Bus& bus, std::uint32_t entry, const emulate::v810::RunOptions& options = {});

}  // namespace cb::engine::pcode_cpu
