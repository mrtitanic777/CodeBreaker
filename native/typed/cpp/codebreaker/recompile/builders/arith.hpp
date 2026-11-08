// codebreaker/recompile/builders/arith.hpp -- C++17 port of codebreaker.recompile.builders.arith.
//
// arith.py -- integer arithmetic and comparison, as C++ for the recompiler (recompile/powerpc.py).
//
// Each `emit(mnemonic, operands, ...)` returns the C++ statements for one instruction, or None
// when the mnemonic is not this file's. Registers are always written as the context's fields
// (ctx.rN, ctx.fN, ctx.vN, ctx.crN, ctx.xer, ctx.ctr, ctx.lr); `ea` and `temp` are locals every
// recompiled function declares.
// Record forms (`add.`, `subfc.` ...): only the base operation is emitted here; powerpc.py's
// record-form wrapper appends the CR0 update, `ctx.cr0.compare<int32_t>(ctx.rD.s32, 0, ctx.xer);`
// (a trailing '.' is stripped here too, so a dotted mnemonic reaches its base builder). addi,
// addis, mulli, subfic and the comparisons have no record form. Comparisons are
// `crD.compare<T>(rA.field, rB.field | imm, ctx.xer)`; operands[0] is the CR field.
//
// Derived from the rexglue SDK's code generator (rexcodegen/builders/arithmetic.cpp, comparison.cpp; BSD-3-Clause,
// Copyright (c) 2026 Tom Clay, with parts from Xenia -- see NOTICE).
#pragma once

#include <cstdint>
#include <optional>

#include "py.hpp"
#include "codebreaker/recompile/builders/floating.hpp"

namespace codebreaker::recompile::builders::arith {

// The operand helpers recompile/powerpc.py passes in (the same types as the floating builders').
using OperandNumber = floating::OperandNumber;
using MemOperand = floating::MemOperand;
using RegisterValue = floating::RegisterValue;
using BaseExpression = floating::BaseExpression;
using AddressExpression = floating::AddressExpression;

// The C++ statements for one instruction, or None when `mn` is not an integer arithmetic or
// comparison mnemonic handled here. A token the helper could not read (None) is written as "None"
// where the Python formats it, and is a TypeError where the Python does arithmetic on it.
// Raises IndexError when an operand the mnemonic needs is missing.
std::optional<py::list<py::str>> emit(const py::str& mn, const py::list<py::str>& ops,
                                      const OperandNumber& _reg, const OperandNumber& _vreg, const OperandNumber& _freg,
                                      const OperandNumber& _imm, const OperandNumber& _cr, const MemOperand& _mem,
                                      const RegisterValue& _rv, const BaseExpression& _base_expr, const AddressExpression& _addr,
                                      int64_t va, int64_t WORD);

// No non-simde runtime helpers are needed: arithmetic.cpp / comparison.cpp
// emit only plain C++ arithmetic plus the CRRegister::compare<T> method and the
// INT64_MIN / INT32_MIN macros (all provided by the recompiler runtime /
// <cstdint>).  __int128 / __uint128_t are compiler builtins.
extern const py::str RUNTIME_HELPERS;

// Every mnemonic handled above (base forms: the '.' forms reach them through the strip).
extern const py::list<py::str> MNEMONICS;

// Not translated: the overflow-enabled forms (addo, subfo, divwo, mullwo ... -- a trailing 'o'):
// the code generator has no builder for them and never sets XER[OV]. ('o' is not stripped the way
// '.' is, so addo does not fall through to add.)

}  // namespace codebreaker::recompile::builders::arith
