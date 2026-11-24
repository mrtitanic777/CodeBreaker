// engine/pcode_cpu_a64.hpp -- the Nintendo Switch lane's AArch64 on CodeBreaker's one emulator (decompile/engine/pcode_emulator), on AARCH64.sla, beside the lane's own core
// (emulate/a64).
//
//     RunResult r = engine::pcode_cpu::call(memory, entry, options);   // as emulate::a64::run, on the core chosen by CB_PCODE_EMU
//
// The registers live in one place at a time: the emulator while the description runs instructions, the own core's Cpu while a kernel call or a replay needs it (a switch copies
// the 31 registers, SP and the flags, and the 32 vector registers only when an instruction needs them). Every data access goes through emulate::a64::Memory (the regions the
// program was loaded into, the stack, the heap).
//
// What runs where:
//   - the emulator runs the description: the integer machine, the branches, the loads and stores of every addressing form (the 16 byte ones included: a wide value is a vector
//     register), the exclusives, and the Advanced SIMD and floating-point instructions, whose named operations (NEON_*, SIMD_PIECE, a64_TBL, MP_INT_ABS) are host code over the
//     shared lane library (engine/pcode_ops_a64.cpp, engine/pcode_vector.cpp);
//   - this file runs, as the lane's own semantics, what the description cannot be asked: svc (the kernel call handler), mrs and msr, the stops (what the own core stops at as "not
//     run" or as an undefined instruction, brk, hlt, udf), the barriers, hints and cache operations (which change nothing), and the exclusive monitor;
//   - the description's errors are carried here, around the emulator's step: a result it makes differently from the manual or from the own core (the high half of a 64 bit or scalar
//     result, SRHADD's destination, the scalar SQSHRN's source, the W register after a sign-extending load, a write-back base that is also the destination, STXP, SBCS and NGCS) and
//     the instructions it cannot do (the float-to-integer conversions, the rounding, the fused multiply-adds, FMAX and FMIN, a NaN result: engine/pcode_cpu_a64_fp.cpp, whose
//     results are the host's, as the own core's are; a few vector instructions: engine/pcode_cpu_a64_vec.cpp);
//   - the own core runs only an instruction whose data access nothing answers: it replays that instruction from the registers as they were, which makes the data abort and its
//     partial effects. share() counts all four.
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
// `description_only` (what the tests of the description use): the instructions the own core stops at run on the description too, and nothing is handed to the own core (an
// instruction the description does not run stops the run with "the description does not run it").
A64End run_a64(emulate::a64::Cpu& cpu, std::uint64_t max_steps, bool description_only = false);

// Whether the lane stops at the word (what the own core stops at as "not run", an undefined instruction, brk, hlt, udf, hvc, smc, eret): the adapter ends the run there itself.
bool lane_stops_at(std::uint32_t word);

// What the instructions of the runs so far went to (all threads), by the mnemonic the own core's decoder gives the ones that went to the own core.
struct A64Share {
    std::uint64_t emulator = 0, own_core = 0, adapter = 0;   // (adapter: the instructions this file runs itself: the kernel call, the system registers, the stops)
    std::map<std::string, std::uint64_t> own_core_by_mnemonic;
    std::map<std::string, std::uint64_t> unimplemented;     // the description's named operations reached that nothing served, by name, and how often
};
A64Share a64_share();
void reset_a64_share();

}  // namespace cb::engine::pcode_cpu
