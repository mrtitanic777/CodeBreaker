// codebreaker/recompile/builders/floating.hpp -- C++17 port of codebreaker.recompile.builders.floating.
//
// floating.py -- scalar floating point: arithmetic, multiply-add, sign, conversion, compare, select, as C++ for the recompiler (recompile/powerpc.py).
//
// Each `emit(mnemonic, operands, ...)` returns the C++ statements for one instruction, or None
// when the mnemonic is not this file's. Registers are always written as the context's fields
// (ctx.rN, ctx.fN, ctx.vN, ctx.crN, ctx.xer, ctx.ctr, ctx.lr); `ea` and `temp` are locals every
// recompiled function declares.
// Every builder first emits `ctx.fpscr.disableFlushMode();`. Values are ctx.fN.f64, raw bits
// ctx.fN.u64; single-precision results are rounded through double(float(...)). The floating-point
// loads and stores are memory.py's, mffs/mtfsf system.py's.
//
// Derived from the rexglue SDK's code generator (rexcodegen/builders/floating_point.cpp; BSD-3-Clause,
// Copyright (c) 2026 Tom Clay, with parts from Xenia -- see NOTICE).
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::recompile::builders::floating {

// The operand helpers recompile/powerpc.py passes in.
// _reg, _vreg, _freg, _imm, _cr: an operand token -> a number, or None when the token is not that kind.
using OperandNumber = std::function<std::optional<int64_t>(const py::str&)>;
// _mem: '-0x70(r1)' -> (disp, basereg), or None
using MemOperand = std::function<std::optional<std::tuple<int64_t, int64_t>>(const py::str&)>;
// _rv(n, field): the register-value expression "ctx.r<n>.<field>"
using RegisterValue = std::function<py::str(int64_t, const py::str&)>;
// _base_expr(base): the address base register expression
using BaseExpression = std::function<py::str(int64_t)>;
// _addr(disp, base): the address expression
using AddressExpression = std::function<py::str(int64_t, int64_t)>;

// The C++ statements for one instruction, or None when `mn` is not a scalar floating-point mnemonic
// handled here. Only _freg and _cr are used; the other helpers and `va`, `WORD` are part of the
// builders' common signature.
std::optional<py::list<py::str>> emit(const py::str& mn, const py::list<py::str>& ops,
                                      const OperandNumber& _reg, const OperandNumber& _vreg, const OperandNumber& _freg,
                                      const OperandNumber& _imm, const OperandNumber& _cr, const MemOperand& _mem,
                                      const RegisterValue& _rv, const BaseExpression& _base_expr, const AddressExpression& _addr,
                                      int64_t va, int64_t WORD);

// Standard headers relied on by the emitted C++ (isnan/sqrt from <cmath>,
// LLONG_MAX/INT_MAX from <climits>). simde_mm_* are standard simde intrinsics
// already available in the recomp prelude. No custom (ppc_vperm-style) helper
// is required for this category.
extern const py::str RUNTIME_HELPERS;

extern const py::list<py::str> MNEMONICS;

// Elsewhere: the floating-point loads and stores (memory.py), mffs/mtfsf (system.py).
// Not translated: fctiw (only fctiwz has a builder).

}  // namespace codebreaker::recompile::builders::floating
