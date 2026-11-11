// cb/recompile/builders/floating.hpp -- scalar floating point as C++ for the recompiler: arithmetic, multiply-add, sign,
// conversion, compare and select.
//
// `emit` turns one PowerPC instruction into the C++ statements the recompiled function holds, or says it is not this
// builder's. Registers are written as the context's fields (ctx.rN, ctx.fN, ctx.vN, ctx.crN, ctx.xer, ctx.ctr,
// ctx.lr); `ea` and `temp` are locals every recompiled function declares. Every statement list starts with
// `ctx.fpscr.disableFlushMode();`. Values are ctx.fN.f64, raw bits ctx.fN.u64; single-precision results are rounded
// through double(float(...)). The floating-point loads and stores belong to the memory builder, mffs/mtfsf to the
// system builder; fctiw has no builder (only fctiwz does).
//
// Derived from the rexglue SDK's code generator (rexcodegen/builders/floating_point.cpp; BSD-3-Clause,
// Copyright (c) 2026 Tom Clay, with parts from Xenia -- see NOTICE).
#pragma once

#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/recompile/builders/operands.hpp"

namespace cb::recompile::builders::floating {

// The statements for `insn`; nullopt when its mnemonic is not a scalar floating-point one handled here. Throws
// ParseError when the mnemonic is ours but an operand is missing or is not the register kind it must be.
std::optional<std::vector<std::string>> emit(const Instruction& insn, const Operands& operands);

// Standard headers the emitted C++ relies on (isnan/sqrt from <cmath>, LLONG_MAX/INT_MAX from <climits>). The
// simde_mm_* intrinsics are already in the recompiler's prelude; no helper function of ours is needed.
std::string_view runtime_helpers();

// The mnemonics `emit` handles, in the order the builder groups them.
std::span<const std::string_view> mnemonics();

}  // namespace cb::recompile::builders::floating
