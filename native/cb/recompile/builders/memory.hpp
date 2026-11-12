// cb/recompile/builders/memory.hpp -- loads, stores and atomics as C++ for the recompiler: the integer and floating-point
// loads and stores in D form (rD, disp(rA)) and X form (rD, rA, rB), their update forms, the algebraic and byte-reversed
// ones, load-and-reserve / store-conditional, the vector loads and stores of VMX and VMX128, and li / lis.
//
// `emit` turns one PowerPC instruction into the C++ statements the recompiled function holds, or says it is not this
// builder's. Memory goes through the runtime's PPC_LOAD_U8..64 / PPC_STORE_U8..64 macros (big-endian, so no byte swap is
// written for them); an address is `ctx.rA.u32 + disp`, or just the displacement when rA is r0 (D form), and
// `ctx.rA.u32 + ctx.rB.u32`, or just rB when rA is r0 (X form). `ea` and `temp` are locals every recompiled function
// declares. The floating-point forms start with `ctx.fpscr.disableFlushMode();`. Vector loads reverse the whole vector
// with the runtime's VectorMaskL (the Xenon is big-endian, the host's vectors are not); the element index of a vector
// store is reversed in the statement itself.
//
// Derived from the rexglue SDK's code generator (rexcodegen/builders/memory.cpp; BSD-3-Clause, Copyright (c) 2026 Tom
// Clay, with parts from Xenia -- see NOTICE).
#pragma once

#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/recompile/builders/operands.hpp"

namespace cb::recompile::builders::memory {

// The statements for `insn`; nullopt when its mnemonic is not a load, store or atomic handled here. Throws ParseError when
// the mnemonic is ours but an operand is missing or is not the kind it must be.
std::optional<std::vector<std::string>> emit(const Instruction& insn, const Operands& operands);

// What the emitted statements expect of the runtime (documentation: a block of comments for the generated source).
std::string_view runtime_helpers();

// The mnemonics `emit` handles, in the order the builder groups them (the dotted and bare spellings of stwcx / stdcx both).
std::span<const std::string_view> mnemonics();

}  // namespace cb::recompile::builders::memory
