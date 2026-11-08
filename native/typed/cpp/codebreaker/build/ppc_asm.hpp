// codebreaker/build/ppc_asm.hpp -- typed C++ port of codebreaker.build.ppc_asm.
//
// ppc_asm.py -- an Xbox 360 PowerPC assembler: mnemonic -> big-endian 4-byte word.
//
// For patching and hooking a title's code (formats/xex_edit.py): branches, the common D / X / XL / XFX
// forms and the simplified mnemonics (nop, li, lis, mr, mtlr, blr, ...). Not a whole assembler -- an
// instruction outside that set stops with AsmError. tests/test_xex_edit.py checks every encoding against
// Capstone's PowerPC decoder.
//
// PowerPC fields (bits numbered from the LSB): opcode<<26, RT/RS<<21, RA<<16, RB<<11, XO<<1, Rc<<0.
// Big-endian in memory.
//
// Notes on the port:
//   * ints are int64_t; a literal that does not fit one raises OverflowError where Python would go on
//     (only _imm with signed=False, int(tok, 0) of a branch target and _mem's displacement can reach that;
//     a register number or a signed immediate that large is the same AsmError as Python's);
//   * `int(text[, 0])` is Python's (prefixes, underscores, Unicode digits, no leading zeros in base 0),
//     written locally;
//   * `signed` is spelled signed_ (a C++ keyword); `labels` defaults to an empty dict (None and {} act alike);
//   * the Python evaluates call arguments left to right; the port sequences them in locals so the first
//     error raised is the same one.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::build::ppc_asm {

extern const int64_t MASK32;

struct AsmError : py::Exception {
    AsmError() : py::Exception() {}
    explicit AsmError(const std::string& m) : py::Exception(m) {}
    explicit AsmError(const char* m) : py::Exception(m) {}
    explicit AsmError(const py::str& m) : py::Exception(m.utf8()) {}
    const char* type() const override { return "AsmError"; }
};

int64_t _reg(const py::str& tok);
int64_t _imm(const py::str& tok, int64_t bits = 16, bool signed_ = true);
// Parse `D(rA)` -> (disp, rA).
std::tuple<int64_t, int64_t> _mem(const py::str& tok);
int64_t _dform(int64_t op, int64_t rt, int64_t ra, int64_t imm);
int64_t _xform(int64_t op, int64_t rt, int64_t ra, int64_t rb, int64_t xo, int64_t rc = 0);
int64_t _spr_enc(int64_t spr);

extern const py::dict<py::str, int64_t> _SPR;

// X-form 3-register ops: mnemonic -> (extended-opcode, has-Rc-dot-variant)
extern const py::dict<py::str, int64_t> _XFORM3;
// D-form ALU/logical: mnemonic -> (opcode, signed-imm)
extern const py::dict<py::str, std::tuple<int64_t, bool>> _DFORM;
// D-form load/store: mnemonic -> opcode
extern const py::dict<py::str, int64_t> _MEMOP;
// conditional-branch simplified mnemonics -> (BO, BI)
extern const py::dict<py::str, std::tuple<int64_t, int64_t>> _BCOND;

// Assemble one instruction at virtual address `addr` -> u32 (or None for a
// blank/comment line). `labels` maps name->address for branch targets.
std::optional<int64_t> assemble_one(const py::str& line, int64_t addr = 0,
                                    const py::dict<py::str, int64_t>& labels = py::dict<py::str, int64_t>());

// Assemble multi-line asm -> big-endian bytes. Two passes for labels.
py::bytes assemble(const py::str& text, int64_t base_addr = 0);

}  // namespace codebreaker::build::ppc_asm
