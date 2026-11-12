// cb/recompile/builders/system.hpp -- special registers, traps, barriers, the condition-register bit operations and the
// other system instructions, as C++ for the recompiler.
//
// `emit` turns one PowerPC instruction into the C++ statements the recompiled function holds, or says it is not this
// builder's. Registers are written as the context's fields (ctx.rN, ctx.crN.lt, ctx.xer, ctx.ctr, ctx.lr); `ea` is a local
// every recompiled function declares. Traps (tdi, twi, td, tw and every simplified form: tweqi ...) decode TO, RA, RB and
// SIMM from the instruction's raw word (`Instruction::word`), so all their spellings reach one builder. Nops, barriers (the
// host's ordering is strong), the Xenon's hints and the cache hints emit nothing (an empty statement list, not "not ours").
// Only `mr` and `clrldi` honour a trailing '.', by appending a CR0 compare. The runtime provides what the statements call:
// ppc_trap, PPC_QUERY_TIMEBASE, PPC_{CHECK,ENTER,LEAVE}_GLOBAL_LOCK, PPC_RAW_ADDR.
//
// Not built (the code generator has none): isel; mfspr / mtspr other than their simplified forms (mfxer mfctr mflr mftb ...
// and mtxer mtctr mtlr); isync, sc, ptesync, tlbsync.
//
// Derived from the rexglue SDK's code generator (rexcodegen/builders/system.cpp, logical.cpp's condition-register
// operations; BSD-3-Clause, Copyright (c) 2026 Tom Clay, with parts from Xenia -- see NOTICE).
#pragma once

#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/recompile/builders/operands.hpp"

namespace cb::recompile::builders::system {

// The statements for `insn`; nullopt when its mnemonic is not a system one handled here. Throws ParseError when the
// mnemonic is ours but an operand is missing or is not the kind it must be (a condition-register bit operand that is not a
// bit; a clrldi count above 64).
std::optional<std::vector<std::string>> emit(const Instruction& insn, const Operands& operands);

// None: the statements call only what the runtime provides.
std::string_view runtime_helpers();

// The mnemonics `emit` handles, in the order the builder groups them.
std::span<const std::string_view> mnemonics();

}  // namespace cb::recompile::builders::system
