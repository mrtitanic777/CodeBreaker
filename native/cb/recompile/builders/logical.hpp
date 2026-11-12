// cb/recompile/builders/logical.hpp -- logical, rotate, shift, extend and immediate boolean operations as C++ for the
// recompiler.
//
// `emit` turns one PowerPC instruction into the C++ statements the recompiled function holds, or says it is not this
// builder's. Registers are written as the context's fields (ctx.rN.u64 ...); `temp` is a local every recompiled function
// declares. Rotates and shifts compute their mask here, as a constant (the code generator's compute_mask). A record form
// emits only the base operation (the recompiler appends the CR0 update), except andi. and andis., which always set CR0 and
// emit it themselves. The condition-register bit operations (crand ...) are the system builder's.
//
// Derived from the rexglue SDK's code generator (rexcodegen/builders/logical.cpp; BSD-3-Clause, Copyright (c) 2026 Tom
// Clay, with parts from Xenia -- see NOTICE).
#pragma once

#include <cstdint>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/recompile/builders/operands.hpp"

namespace cb::recompile::builders::logical {

// The rotate mask from bit `start` to bit `stop`, both taken modulo 64, wrapping round when start > stop (bit 0 is the
// most significant).
std::uint64_t rotate_mask(std::int64_t start, std::int64_t stop);

// The statements for `insn`; nullopt when its mnemonic is not a logical, rotate, shift or extend one handled here. Throws
// ParseError when the mnemonic is ours but an operand is missing or is not the kind it must be, or an immediate is out of
// the range the statement can carry (a clrlwi count above 32; an immediate shifted left by 16 that does not fit 64 bits).
std::optional<std::vector<std::string>> emit(const Instruction& insn, const Operands& operands);

// None: the statements use only the recompiler's own context and the compiler's builtins.
std::string_view runtime_helpers();

// The mnemonics `emit` handles, in the order the builder groups them.
std::span<const std::string_view> mnemonics();

}  // namespace cb::recompile::builders::logical
