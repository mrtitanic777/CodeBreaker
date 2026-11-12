// cb/recompile/builders/reading.hpp -- what every instruction builder does first: read its operand tokens by kind, and
// write the statements of a table row.
//
// A builder asks for operand n as a general register, a floating-point register, a vector register, a condition field, an
// immediate or a memory operand; a missing operand, or one that is not that kind, is a ParseError naming the mnemonic and
// the operand's number (the recompiler then falls back to its hand-written handlers). The tables of arith, logical,
// memory and system hold their statements as text with {d} {a} {b} {c} (register numbers), {i} {j} {k} (immediates) and {z}
// (the "ctx.rA.s64 + " prefix an addi leaves out when rA is r0) and {p} {q} {r} (a row for several mnemonics gives each its own text) in them; `fill_lines` writes one row's statements.
#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

#include "cb/recompile/builders/operands.hpp"

namespace cb::recompile::builders {

// General register rN, floating-point register fN, vector register vN, condition field crN: the number.
int gpr_at(const Instruction& insn, const Operands& ops, std::size_t index);
int fpr_at(const Instruction& insn, const Operands& ops, std::size_t index);
int vpr_at(const Instruction& insn, const Operands& ops, std::size_t index);
int crf_at(const Instruction& insn, const Operands& ops, std::size_t index);
// An immediate, and a "disp(rN)" operand.
std::int64_t imm_at(const Instruction& insn, const Operands& ops, std::size_t index);
MemoryOperand mem_at(const Instruction& insn, const Operands& ops, std::size_t index);

// The low 32 bits of x as a signed / unsigned number.
inline std::int64_t as_i32(std::int64_t x) { return static_cast<std::int32_t>(static_cast<std::uint32_t>(static_cast<std::uint64_t>(x))); }
inline std::int64_t as_u32(std::int64_t x) { return static_cast<std::int64_t>(static_cast<std::uint64_t>(x) & 0xFFFFFFFFull); }

// The values a row's text names.
struct Slots {
    int d = 0, a = 0, b = 0, c = 0;
    std::string i, j, k, z;
    std::string p, q, r;   // text parameters of a row that serves several mnemonics (a width, a type, a field)
};

// `lines` is statements separated by '\n'; each has its {x} slots replaced.
std::vector<std::string> fill_lines(std::string_view lines, const Slots& slots);

// "0x" and the upper-case hex digits.
std::string hex(std::uint64_t value);

}  // namespace cb::recompile::builders
