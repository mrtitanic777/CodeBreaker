// engine/pcode_cpu_x86.hpp -- an x86 engine on CodeBreaker's one emulator (decompile/engine/pcode_emulator), as the core16::Uc the lanes that run x86 code on Unicorn use: the
// WonderSwan's V30MZ (16-bit, x86.sla) and the PlayStation 4 and 5's and the Xbox One's x86-64 (x86-64.sla).
//
//     std::unique_ptr<core16::Uc> uc = engine::pcode_cpu::open_x86(64);   // the emulator, unless CB_PCODE_EMU=0, then Unicorn
//
// The engine is what a lane would have made with emulate::unicorn_uc::open_x86(bits): the same calls, the same registers (Unicorn's ids), the same faults (emu_start throws
// UcFault with Unicorn's number and text), the same hooks. What Unicorn is, the engine is made of: mapped regions, the registers as the description names them (EFLAGS put
// together from its flag bits, IP and CS as the 16-bit machine has them), the instruction count of emu_start, `until`, the in and out hooks, a hook on the syscall instruction.
//
// What the description does not carry, and what it leaves to the machine, is Unicorn's (see pcode_cpu_x86.cpp): when an instruction of that kind is next, the machine goes to a
// Unicorn engine as it stands and the run ends there. X86Stats counts what went and why.
#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <string>

#include "core16_types.hpp"
#include "core/bytes.hpp"

namespace cb::engine::pcode_cpu {

// An engine on the emulator, always. `bits` is 16 (real mode, x86.sla) or 64 (x86-64.sla); UsageError for another.
std::unique_ptr<core16::Uc> open_x86_emulator(int bits);

// open_x86_emulator() or emulate::unicorn_uc::open_x86(), by enabled() (CB_PCODE_EMU=0 gives Unicorn): the emulator is on by default, about as fast as Unicorn on the lanes' runs.
std::unique_ptr<core16::Uc> open_x86(int bits);

// Whether `uc` is the emulator's engine, and whether its run has gone to Unicorn.
bool is_emulator_engine(const core16::Uc& uc);
bool ran_on_unicorn(const core16::Uc& uc);

// A hook on the SYSCALL instruction: the instruction is the hook's (as on Unicorn, RCX, R11 and RIP are left as they are, RIP past the instruction). The hook is removed when the
// returned object goes, which must be before `uc` does.
std::shared_ptr<void> hook_syscall(core16::Uc& uc, std::function<void(core16::Uc&)> hook);

// A whole vector register (YMM0..15, 32 bytes) of the emulator's engine (a Unicorn engine has the low 16 bytes: the rest read as zero and are not written).
Bytes read_ymm(core16::Uc& uc, unsigned index);
void write_ymm(core16::Uc& uc, unsigned index, ByteView bytes);

// A vector register (the low 128 bits of XMM0..15) of either engine: for the tests and the lanes that compare them.
Bytes read_xmm(core16::Uc& uc, unsigned index);
void write_xmm(core16::Uc& uc, unsigned index, ByteView bytes);

// How the engine of this thread treats what Unicorn does not do right. VEX-encoded instructions (AVX, AVX2, F16C, FMA) are the architecture's by default: three operands, 256-bit
// registers. Unicorn's decoder reads the VEX prefix and runs the instruction as the legacy one of the same opcode (two operands, 128 bits: the VEX register operand and length
// bits are ignored); `vex_as_unicorn` does the same on the emulator, which is what the tests that compare the two engines run with.
struct X86Options {
    bool vex_as_unicorn = false;
    bool x87_on_description = false;   // run the x87 float instructions on the description (it keeps no top of stack, tag word or exception flags: not Unicorn's results)
};
X86Options& x86_options();

// What the engines of this thread did: runs started, instructions the description ran, runs that went to Unicorn by reason, the opcodes the description could not decode (their
// first three bytes), user operations nothing served.
struct X86Stats {
    std::uint64_t runs = 0;
    std::uint64_t steps = 0;
    std::uint64_t vex = 0;                // VEX-encoded instructions the description ran
    bool count_tail = false;              // count the instructions Unicorn runs after a hand-off (a hook on each: slow; for the tests that report the share)
    std::uint64_t tail_steps = 0;
    std::map<std::string, std::uint64_t> handoffs;
    std::map<std::string, std::uint64_t> opcodes;
    std::map<std::string, std::uint64_t> unserved;
    void clear() {
        const bool counting = count_tail;
        *this = X86Stats{};
        count_tail = counting;
    }
};
X86Stats& x86_stats();

}  // namespace cb::engine::pcode_cpu
