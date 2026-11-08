// codebreaker/recompile/builders/logical.hpp -- C++17 port of codebreaker.recompile.builders.logical.
//
// logical.py -- logical, rotate, shift, extend and immediate boolean operations, as C++ for the recompiler (recompile/powerpc.py).
//
// Each `emit(mnemonic, operands, ...)` returns the C++ statements for one instruction, or None
// when the mnemonic is not this file's. Registers are always written as the context's fields
// (ctx.rN, ctx.fN, ctx.vN, ctx.crN, ctx.xer, ctx.ctr, ctx.lr); `ea` and `temp` are locals every
// recompiled function declares.
// Record forms emit only the base operation (powerpc.py appends the CR0 update), except andi. and
// andis., which always set CR0 and emit it themselves. The CR-bit operations (crand ...) are
// system.py's.
//
// Derived from the rexglue SDK's code generator (rexcodegen/builders/logical.cpp; BSD-3-Clause,
// Copyright (c) 2026 Tom Clay, with parts from Xenia -- see NOTICE).
#pragma once

#include <cstdint>
#include <functional>
#include <optional>

#include "py.hpp"
#include "codebreaker/recompile/builders/floating.hpp"

namespace codebreaker::recompile::builders::logical {

// The operand helpers recompile/powerpc.py passes in (the same types as the floating builders').
using OperandNumber = floating::OperandNumber;
using MemOperand = floating::MemOperand;
using BaseExpression = floating::BaseExpression;
using AddressExpression = floating::AddressExpression;
// _rv(n, field): the register-value expression "ctx.r<n>.<field>". Unlike the floating builders'
// type, n is None (std::nullopt) when the operand token was not a register: the Python's f-string
// writes it as "None".
using RegisterValue = std::function<py::str(const std::optional<int64_t>&, const py::str&)>;

const uint64_t U64 = 0xFFFFFFFFFFFFFFFFull;

// the rotate mask from bit mstart to bit mstop (the code generator's compute_mask)
uint64_t _compute_mask(int64_t mstart, int64_t mstop);

// The C++ statements for one instruction, or None when `mn` is not a logical, rotate, shift or
// extend mnemonic handled here. A token the helper could not read (None) is written as "None"
// where the Python formats it, and is a TypeError where the Python does arithmetic on it.
// Raises IndexError when an operand the mnemonic needs is missing, ValueError for clrlwi with a
// count above 32 (the Python's negative shift count).
std::optional<py::list<py::str>> emit(const py::str& mn, const py::list<py::str>& ops,
                                      const OperandNumber& _reg, const OperandNumber& _vreg, const OperandNumber& _freg,
                                      const OperandNumber& _imm, const OperandNumber& _cr, const MemOperand& _mem,
                                      const RegisterValue& _rv, const BaseExpression& _base_expr, const AddressExpression& _addr,
                                      int64_t va, int64_t WORD);

extern const py::str RUNTIME_HELPERS;

extern const py::list<py::str> MNEMONICS;

// Elsewhere: the CR-bit operations crand crandc creqv crnand crnor cror crorc crxor (system.py).

}  // namespace codebreaker::recompile::builders::logical
