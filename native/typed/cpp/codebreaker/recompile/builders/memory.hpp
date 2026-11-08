// codebreaker/recompile/builders/memory.hpp -- C++17 port of codebreaker.recompile.builders.memory.
//
// memory.py -- loads and stores: indexed, update, algebraic and byte-reversed forms, the atomics (lwarx/ldarx/stwcx./stdcx.), floating-point and vector loads and stores, as C++ for the recompiler (recompile/powerpc.py).
//
// Each `emit(mnemonic, operands, ...)` returns the C++ statements for one instruction, or None
// when the mnemonic is not this file's. Registers are always written as the context's fields
// (ctx.rN, ctx.fN, ctx.vN, ctx.crN, ctx.xer, ctx.ctr, ctx.lr); `ea` and `temp` are locals every
// recompiled function declares.
// Every access goes through PPC_LOAD_* / PPC_STORE_* (big-endian, on guest memory). The code
// generator's memory-mapped I/O path -- chosen when an access follows `eieio` or uses a register
// that holds an I/O base -- is not taken: emit() sees one instruction at a time, so every access
// here is the ordinary one.
//
// Derived from the rexglue SDK's code generator (rexcodegen/builders/memory.cpp; BSD-3-Clause,
// Copyright (c) 2026 Tom Clay, with parts from Xenia -- see NOTICE).
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>

#include "py.hpp"
#include "codebreaker/recompile/builders/floating.hpp"

namespace codebreaker::recompile::builders::memory {

// The operand helpers recompile/powerpc.py passes in (the same types as the floating builders').
using OperandNumber = floating::OperandNumber;
using MemOperand = floating::MemOperand;
using BaseExpression = floating::BaseExpression;
using AddressExpression = floating::AddressExpression;
// _rv(n, field): as in the logical and system builders (n is None when the operand token was not a register).
using RegisterValue = std::function<py::str(const std::optional<int64_t>&, const py::str&)>;

// The C++ statements for one instruction, or None when `mn` is not a memory mnemonic handled here.
// A token the helper could not read (None) is written as "None" where the Python formats it, and
// is a TypeError where the Python does arithmetic on it or unpacks it (a memory operand that
// _mem could not read). Raises IndexError when an operand the mnemonic needs is missing.
// Only _reg, _vreg, _freg, _imm and _mem are used; the other helpers, `va` and `WORD` are part
// of the builders' common signature.
std::optional<py::list<py::str>> emit(const py::str& mn, const py::list<py::str>& ops,
                                      const OperandNumber& _reg, const OperandNumber& _vreg, const OperandNumber& _freg,
                                      const OperandNumber& _imm, const OperandNumber& _cr, const MemOperand& _mem,
                                      const RegisterValue& _rv, const BaseExpression& _base_expr, const AddressExpression& _addr,
                                      int64_t va, int64_t WORD);

// ---------------------------------------------------------------------------
// Runtime helpers required by the emitted code that are NOT standard simde.
//
// The statements use symbols the runtime provides; they are declared here, not defined. The
// shuffle tables' contents (256-byte masks) come from the rexglue kernel runtime
// (native/rexglue/kernel: ppc_context / ppc_recomp_shared), never re-derived.
//
//   Macros:  PPC_LOAD_U8/16/32/64, PPC_STORE_U8/16/32/64, PPC_RAW_ADDR,
//            (the I/O variants PPC_MM_LOAD_*/PPC_MM_STORE_* are not emitted: see the note below)
//   Builtins: __builtin_bswap16/32/64, __sync_bool_compare_and_swap
//   Tables (extern, 16-byte-aligned, indexed as [(off)*16] byte layouts):
//            VectorMaskL[], VectorMaskR[], VectorShiftTableL[], VectorShiftTableR[]
// ---------------------------------------------------------------------------
extern const py::str RUNTIME_HELPERS;

// Every mnemonic handled above (including VMX128 / element / record-form variants).
extern const py::list<py::str> MNEMONICS;

// Not translated here:
//  * the memory-mapped I/O path (PPC_MM_LOAD_* / PPC_MM_STORE_*), which the code generator takes for
//    an access followed by eieio (0xAC06007C) or through a register lis loaded with an I/O base
//    (0x7FC8-0x7FCF, 0x7FEA): emit() sees one instruction, so every access is the ordinary one;
//  * lmw/stmw, lswi/stswi and the cache operations (dcbt dcbtst dcbf dcbst dcbz icbi): no builders;
//  * the contents of the vector shuffle tables (VectorMaskL/R, VectorShiftTableL/R), which the
//    statements reference and the runtime header defines.

}  // namespace codebreaker::recompile::builders::memory
