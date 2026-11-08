// codebreaker/recompile/builders/system.hpp -- C++17 port of codebreaker.recompile.builders.system.
//
// system.py -- special registers, traps, barriers, the CR-bit logical operations and the other system instructions, as C++ for the recompiler (recompile/powerpc.py).
//
// Each `emit(mnemonic, operands, ...)` returns the C++ statements for one instruction, or None
// when the mnemonic is not this file's. Registers are always written as the context's fields
// (ctx.rN, ctx.fN, ctx.vN, ctx.crN, ctx.xer, ctx.ctr, ctx.lr); `ea` and `temp` are locals every
// recompiled function declares.
// Traps (tdi/twi/td/tw and every simplified form, tweqi ...) decode TO, RA, RB and SIMM from the
// raw instruction word (WORD), so all their spellings reach one builder. Nops, barriers (the host's
// ordering is strong) and cache hints emit nothing. The runtime provides what the statements call:
// ppc_trap, PPC_QUERY_TIMEBASE, PPC_{CHECK,ENTER,LEAVE}_GLOBAL_LOCK, PPC_RAW_ADDR.
//
// Derived from the rexglue SDK's code generator (rexcodegen/builders/system.cpp, logical.cpp's CR-bit operations; BSD-3-Clause,
// Copyright (c) 2026 Tom Clay, with parts from Xenia -- see NOTICE).
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>

#include "py.hpp"
#include "codebreaker/recompile/builders/floating.hpp"

namespace codebreaker::recompile::builders::system {

// The operand helpers recompile/powerpc.py passes in (the same types as the floating builders').
using OperandNumber = floating::OperandNumber;
using MemOperand = floating::MemOperand;
using BaseExpression = floating::BaseExpression;
using AddressExpression = floating::AddressExpression;
// _rv(n, field): as in logical.hpp (n is None when the operand token was not a register).
using RegisterValue = std::function<py::str(const std::optional<int64_t>&, const py::str&)>;

extern const uint64_t U64;
extern const uint64_t U32;

// CR bit-field member names, indexed by (bit index % 4). Matches crBitName().
extern const py::list<py::str> _CRBIT;

// Trap mnemonic groups -> shared builder (build_twi/tdi/tw/td). All decode from
// the raw word, so the exact simplified form does not matter.
extern const py::set<py::str> _TWI;
extern const py::set<py::str> _TDI;
extern const py::set<py::str> _TW;
extern const py::set<py::str> _TD;

// No-effect instructions (builder returns true, emits nothing): canonical nops,
// memory barriers (x86 strong ordering), Xenon-specific hints, and cache hints.
extern const py::set<py::str> _NOP_MNEMONICS;

// CR-logical: mnemonic -> (op, invertA, invertB, invertResult) for emitCRBitOperation.
using CrLogical = std::tuple<py::str, bool, bool, bool>;
extern const py::dict<py::str, CrLogical> _CRLOGICAL;

// Port of emitTrap() (helpers.h).
py::list<py::str> _emit_trap(int64_t to, const py::str& a_signed, const py::str& a_unsigned,
                             const py::str& b_signed, const py::str& b_unsigned);

// Port of emitCRBitOperation() (helpers.h). operands are CR *bit* indices.
// TypeError when an operand token was not a CR bit (the Python divides None).
py::list<py::str> _emit_crbit(const py::str& op, bool inv_a, bool inv_b, bool inv_res, const OperandNumber& _cr,
                              const py::list<py::str>& ops);

// The C++ statements for one instruction, or None when `mn` is not a system mnemonic handled here.
// A token the helper could not read (None) is written as "None" where the Python formats it, and
// is a TypeError where the Python does arithmetic on it. Raises IndexError when an operand the
// mnemonic needs is missing, ValueError for clrldi with a count above 64 (negative shift count).
std::optional<py::list<py::str>> emit(const py::str& mn, const py::list<py::str>& ops,
                                      const OperandNumber& _reg, const OperandNumber& _vreg, const OperandNumber& _freg,
                                      const OperandNumber& _imm, const OperandNumber& _cr, const MemOperand& _mem,
                                      const RegisterValue& _rv, const BaseExpression& _base_expr, const AddressExpression& _addr,
                                      int64_t va, int64_t WORD);

extern const py::str RUNTIME_HELPERS;

extern const py::list<py::str> MNEMONICS;

// Not translated: isel; mfspr/mtspr other than their simplified forms (mfxer mfctr mflr mftb ...
// and mtxer mtctr mtlr); isync, sc, ptesync, tlbsync. The code generator has no builders for them.

}  // namespace codebreaker::recompile::builders::system
