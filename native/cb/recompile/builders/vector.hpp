// cb/recompile/builders/vector.hpp -- the Xenon's vector instructions (VMX and VMX128) as C++ for the recompiler: float and
// integer arithmetic, saturating arithmetic, logical, compare, convert, merge, permute, shift and rotate, splat, pack and
// unpack including the Direct3D packed formats (vpkd3d128, vupkd3d128).
//
// `emit` turns one PowerPC instruction into the C++ statements the recompiled function holds, or says it is not this
// builder's. Vector registers are `ctx.vN` (members .u8 .s8 .u16 .s16 .u32 .s32 .u64 .f32); every statement goes through
// SIMDe (`simde_mm_*`), so the generated code runs on any host. Operands are in the order the VMX128 decoder gives them
// (vD, vA, vB, then the immediate or vC). The float statements start with `ctx.fpscr.enableFlushMode();`. A record form
// (`vcmpequw.`) is a mnemonic ending in '.', and updates CR6 from the compare's mask; the 128 spellings (`vaddfp128`) are the
// same statement. `vTemp` (a vector) and `temp` are locals every recompiled function declares; the lane order is the
// reversed one the Xenon's big-endian layout needs, which the statements spell out (`u32[3 - i]`).
//
// Derived from the rexglue SDK's code generator (rexcodegen/builders/vector*.cpp; BSD-3-Clause, Copyright (c) 2026 Tom Clay,
// with parts from Xenia -- see NOTICE).
#pragma once

#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/recompile/builders/operands.hpp"

namespace cb::recompile::builders::vector {

// The statements for `insn`; nullopt when its mnemonic is not a vector one handled here. Throws ParseError when the
// mnemonic is ours but an operand is missing or is not the kind it must be, or an immediate is outside what the statement
// can carry (a convert scale outside 0..62 / 0..126, a vrlimi128 rotate outside 0..3).
std::optional<std::vector<std::string>> emit(const Instruction& insn, const Operands& operands);

// What the emitted statements expect of the runtime's `rex` namespace and context (documentation: a block of comments for
// the generated source).
std::string_view runtime_helpers();

// The mnemonics `emit` handles (record and non-record spellings alike), in the order the builder groups them.
std::span<const std::string_view> mnemonics();

}  // namespace cb::recompile::builders::vector
