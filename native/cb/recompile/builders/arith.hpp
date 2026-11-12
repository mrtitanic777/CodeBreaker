// cb/recompile/builders/arith.hpp -- integer arithmetic and comparison as C++ for the recompiler: add, subtract, multiply,
// divide, negate, and the compares that set a condition field.
//
// `emit` turns one PowerPC instruction into the C++ statements the recompiled function holds, or says it is not this
// builder's. Registers are written as the context's fields (ctx.rN.u64, .s32 ...); `ea` and `temp` are locals every
// recompiled function declares. A record form (`add.`, `subfc.` ...) reaches its base builder with the trailing '.'
// stripped: only the base operation is emitted here, and the recompiler appends the CR0 update. addi, addis, mulli, subfic
// and the compares have no record form. A compare is `ctx.crD.compare<T>(rA.field, rB.field | imm, ctx.xer)`; operand 0 is
// the condition field. Division gives 0 for a zero divisor and for INT_MIN / -1.
//
// The overflow-enabled forms (addo, subfo, divwo, mullwo ...) have no builder: the code generator never sets XER[OV], and a
// trailing 'o' is not stripped, so addo does not fall through to add.
//
// Derived from the rexglue SDK's code generator (rexcodegen/builders/arithmetic.cpp, comparison.cpp; BSD-3-Clause,
// Copyright (c) 2026 Tom Clay, with parts from Xenia -- see NOTICE).
#pragma once

#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/recompile/builders/operands.hpp"

namespace cb::recompile::builders::arith {

// The statements for `insn`; nullopt when its mnemonic is not an integer arithmetic or compare one handled here. Throws
// ParseError when the mnemonic is ours but an operand is missing or is not the kind it must be.
std::optional<std::vector<std::string>> emit(const Instruction& insn, const Operands& operands);

// Standard headers the emitted C++ relies on: none beyond the recompiler's own (CRRegister::compare<T>, INT64_MIN,
// INT32_MIN, the compiler's 128-bit integers).
std::string_view runtime_helpers();

// The mnemonics `emit` handles, base forms (the '.' forms reach them through the strip), in the order the builder groups them.
std::span<const std::string_view> mnemonics();

}  // namespace cb::recompile::builders::arith
