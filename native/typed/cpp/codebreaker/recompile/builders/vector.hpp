// codebreaker/recompile/builders/vector.hpp -- C++17 port of codebreaker.recompile.builders.vector.
//
// vector.py -- AltiVec and VMX128 vector instructions, as C++ for the recompiler (recompile/powerpc.py).
//
// Each `emit(mnemonic, operands, ...)` returns the C++ statements for one instruction, or None
// when the mnemonic is not this file's. Registers are always written as the context's fields
// (ctx.rN, ctx.fN, ctx.vN, ctx.crN, ctx.xer, ctx.ctr, ctx.lr); `ea` and `temp` are locals every
// recompiled function declares.
// Vector registers are held element-reversed (for little-endian SIMD), and the index arithmetic
// that compensates is the code generator's own: vspltb perm = 15 - UIMM; vsplth perm = 7 - UIMM,
// then (perm*2) | ((perm*2+1) << 8); vspltw perm = 3 - UIMM, repeated in each 2-bit lane;
// vpermwi128 x=3-(imm&3), y=3-((imm>>2)&3), z=3-((imm>>4)&3), w=3-((imm>>6)&3); vsldoi's alignr
// count is 16 - SH; the pack and unpack forms use reversed element indices. Operations that run
// in VMX mode first emit `ctx.fpscr.enableFlushMode();`. The vector loads and stores, and
// mtvscr/mfvscr, are memory.py's. The SIMD helpers the statements call (simde_mm_* ...) are in the
// runtime header (powerpc.runtime_header).
//
// Derived from the rexglue SDK's code generator (rexcodegen/builders/vector.cpp; BSD-3-Clause,
// Copyright (c) 2026 Tom Clay, with parts from Xenia -- see NOTICE).
#pragma once

#include <cstdint>
#include <optional>

#include "py.hpp"
#include "codebreaker/recompile/builders/floating.hpp"

namespace codebreaker::recompile::builders::vector {

// The operand helpers recompile/powerpc.py passes in (the same types as the floating builders').
using OperandNumber = floating::OperandNumber;
using MemOperand = floating::MemOperand;
using RegisterValue = floating::RegisterValue;
using BaseExpression = floating::BaseExpression;
using AddressExpression = floating::AddressExpression;

// Runtime helpers referenced by the emitted code (rex::simde_mm_*) are NOT defined
// in vector.cpp; they are provided by the recompiler runtime (ppc.h / simde ext).
// We reproduce the emitted *calls* exactly and forward-declare the helpers here so
// the generated translation unit resolves them.  See notes at bottom.
extern const py::str RUNTIME_HELPERS;

// Mnemonics handled (base forms + Xbox360 VMX128 aliases). Record-form ('.')
// variants of the vcmp* instructions are handled dynamically (trailing '.').
extern const py::list<py::str> MNEMONICS;

// Elsewhere: the vector loads and stores (lvx lvxl lvlx lvrx lvebx lvehx lvewx lvsl lvsr, stvx
// stvxl stvlx stvrx stvebx stvehx stvewx) and mtvscr/mfvscr (memory.py).

// The C++ statements for one instruction, or None when `mn` is not a vector mnemonic handled here.
// A token the helper could not read (None) is a TypeError where the Python formats or does
// arithmetic on it; IndexError when an operand the mnemonic needs is missing.
// (A register or immediate the statements never use may be None; vxor vA,vA reads only vD.)
std::optional<py::list<py::str>> emit(const py::str& mn, const py::list<py::str>& ops,
                                      const OperandNumber& _reg, const OperandNumber& _vreg, const OperandNumber& _freg,
                                      const OperandNumber& _imm, const OperandNumber& _cr, const MemOperand& _mem,
                                      const RegisterValue& _rv, const BaseExpression& _base_expr, const AddressExpression& _addr,
                                      int64_t va, int64_t WORD);

// ---------------------------------------------------------------------------
// Unpack (simple, si128) helpers
// ---------------------------------------------------------------------------
py::list<py::str> _emit_vupkhsb(const std::optional<int64_t>& d, const std::optional<int64_t>& a);
py::list<py::str> _emit_vupkhsh(const std::optional<int64_t>& d, const std::optional<int64_t>& a);
py::list<py::str> _emit_vupklsb(const std::optional<int64_t>& d, const std::optional<int64_t>& a);
py::list<py::str> _emit_vupklsh(const std::optional<int64_t>& d, const std::optional<int64_t>& a);

// ---------------------------------------------------------------------------
// vpkd3d128 : D3D pack (scalar). operands[0]=vD, [1]=vB(src), [2]=type,
//             [3]=mask, [4]=shift.  Emits "ctx.fpscr.enableFlushMode();" first.
// ---------------------------------------------------------------------------
py::list<py::str> _emit_vpkd3d128(const py::list<py::str>& ops, const OperandNumber& _vreg, const OperandNumber& _imm);

// ---------------------------------------------------------------------------
// vupkd3d128 : D3D unpack (scalar). operands[0]=vD, [1]=vB(src), [2]=type.
//              switch is on (operands[2] >> 2).
// ---------------------------------------------------------------------------
py::list<py::str> _emit_vupkd3d128(const py::list<py::str>& ops, const OperandNumber& _vreg, const OperandNumber& _imm);

}  // namespace codebreaker::recompile::builders::vector
