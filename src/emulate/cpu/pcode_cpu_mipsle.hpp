// emulate/cpu/pcode_cpu_mipsle.hpp -- the run the PSP's Allegrex (pcode_cpu_allegrex) and the PlayStation 2's Emotion Engine (pcode_cpu_ee) share: emulate/mips_le's register record, with the
// description of the closest MIPS (languages/mips32le.sla for the Allegrex, mips64le.sla for the R5900) doing the instructions it carries and the adapter's own semantics
// (pcode_ops_mipsle.cpp) doing the rest, on the registers as they stand.
//
// The description runs the integer machine: the shifts, the ALU, the multiplier and HI/LO, the aligned loads and stores (every data access is the machine's Port: its memory,
// its hardware and their log), and a branch with its delay slot as one step. The adapter's own semantics run everything the description does not carry or carries another way: the
// FPU (the R5900's has no infinities, NaNs or denormals), coprocessors 0 and 2, the multimedia and 128-bit instructions, the Allegrex's extras, the unaligned loads and
// stores, SYSCALL and BREAK and the traps, the machine's import stubs, and every instruction that would fault (an unaligned or unanswered access, an overflowing add,
// a misaligned jump) so that the fault is theirs; also a branch whose delay slot is another branch or touches its registers, and one that links through the register it reads.
// Nothing goes to the lane's old core.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>

#include "emulate/mips_le.hpp"

namespace cb::emulate::pcode_cpu {

// What the engine asks of the machine besides the Port.
struct MipsLeMachine {
    // A read that logs nothing, for fetching instructions (false: the machine would log it, or nothing answers: the own core fetches).
    std::function<bool(std::uint32_t address, unsigned size, std::uint64_t& value)> peek;
    // Whether the address is the stub of an imported function (the Port answers a call to it in intercept: the own core runs it).
    std::function<bool(std::uint32_t address)> is_stub;
};

struct MipsLeEnd {
    enum class Kind : std::uint8_t { Return, Stopped, Limit };
    Kind kind = Kind::Limit;
    std::optional<emulate::mipsle::Fault> fault;
};

// Run `cpu` (its registers, pc and cop0 set as the machine's run starts them) until its pc is `return_address`, it faults, or it has taken `max_steps` instructions.
// On return the cpu holds every register as the run left it.
MipsLeEnd run_mipsle(emulate::mipsle::Cpu& cpu, const MipsLeMachine& machine, std::uint32_t return_address, std::uint64_t max_steps);

}  // namespace cb::emulate::pcode_cpu
