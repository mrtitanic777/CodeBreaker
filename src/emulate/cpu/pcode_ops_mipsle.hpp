// emulate/cpu/pcode_ops_mipsle.hpp -- the Allegrex and Emotion Engine adapters' own semantics (pcode_ops_mipsle.cpp): the instructions the description does not carry, and the faults.
#pragma once

#include <cstdint>
#include <optional>

#include "emulate/mips_le.hpp"

namespace cb::emulate::pcode_ops_mipsle {

// Execute the instruction at c.pc on the registers of `c` and its Port; c.next_pc follows it (a branch changes that, a branch likely whose branch is not taken skips its slot).
// nullopt: it ran, and c.steps counts it. A Fault: it did not finish (pc and next_pc are as they were, except for BREAK and SYSCALL). `fetched_word`: the instruction, when the
// caller has read it already (the Port is then not asked for it or for an intercept); else the Port is asked for the call of an imported function, then for the word.
std::optional<emulate::mipsle::Fault> step(emulate::mipsle::Cpu& c, const std::optional<std::uint32_t>& fetched_word = std::nullopt);

}  // namespace cb::emulate::pcode_ops_mipsle
