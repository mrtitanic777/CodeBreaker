// engine/pcode_cpu_a64.hpp -- the Nintendo Switch lane's AArch64 on CodeBreaker's one emulator (decompile/engine/pcode_emulator), on AARCH64.sla, beside the lane's own core
// (emulate/a64).
//
//     RunResult r = engine::pcode_cpu::call(memory, entry, options);   // as emulate::a64::run, on the core chosen by CB_PCODE_EMU
//
// The registers live in one place at a time: the emulator while the description runs instructions, the own core's Cpu while it does (a switch copies the 31 registers, SP and
// the flags, and the 32 vector registers only when a floating-point instruction needs them). An instruction goes to the description when it is one the description carries the
// way the own core runs it; every data access goes through emulate::a64::Memory (the regions the program was loaded into, the stack, the heap), and an access Memory does not
// answer is found out by the hook after the fact: the registers the instruction could have written are put back and the own core runs it again, which also gives the data abort.
//
// The description runs the integer machine: the data processing, the branches, the loads and stores of every addressing form (pairs and the exclusives included: the
// exclusive monitor is the own core's Cpu, which the userops read), the hints and barriers, and the scalar floating point. The own core runs what the description does not
// carry: every vector instruction and every 16-byte access (the emulator's values are 64 bits wide, a 128-bit register is not one), the 64-bit exclusive pair store (the
// description stores the second register 4 bytes on), the system instructions (svc, mrs, msr, dc, the kernel calls and the registers the own core models), brk, hlt and udf, the
// pointer-authentication hints, compare-and-swap, and everything the own core itself stops at ("not run"), so the stop is its stop. share() counts both.
#pragma once

#include <cstdint>
#include <map>
#include <string>

#include "emulate/a64.hpp"
#include "engine/pcode_cpu.hpp"

namespace cb::engine::pcode_cpu {

// The run on the emulator, always.
emulate::a64::RunResult run(emulate::a64::Memory& memory, std::uint64_t entry, const emulate::a64::RunOptions& options = {});

// emulate::a64::run or run(), by enabled().
emulate::a64::RunResult call(emulate::a64::Memory& memory, std::uint64_t entry, const emulate::a64::RunOptions& options = {});

// How the run ended and why, as the own core's loop gives it.
struct A64End {
    emulate::a64::End end = emulate::a64::End::Limit;
    std::string reason;
};

// The loop of run() on a Cpu the caller has set up (registers, pc, the svc handler): until the pc is emulate::a64::kReturnAddress, an instruction stops it, or `max_steps`
// instructions have run (Cpu::steps counts them). On return `cpu` holds every register as the run left it.
A64End run_a64(emulate::a64::Cpu& cpu, std::uint64_t max_steps);

// What the instructions of the runs so far went to (all threads), by the mnemonic the own core's decoder gives the ones that went to the own core.
struct A64Share {
    std::uint64_t emulator = 0, own_core = 0;
    std::map<std::string, std::uint64_t> own_core_by_mnemonic;
};
A64Share a64_share();
void reset_a64_share();

}  // namespace cb::engine::pcode_cpu
