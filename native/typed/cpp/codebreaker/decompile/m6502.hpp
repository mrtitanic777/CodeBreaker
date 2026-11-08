// codebreaker/decompile/m6502.hpp -- C++ port of codebreaker.decompile.m6502.
//
// m6502.py -- the 6502's instructions: the table, a disassembler, an assembler.
//
//     m6502::decode(code, at, pc)         Insn(mnemonic, mode, operand, size, target) or None
//     m6502::text(insn, labels)           "LDA $0200,X" / "BNE loop" (ca65 syntax)
//     m6502::assemble(line, pc, labels)   the bytes of one ca65 instruction line
//
// The 151 documented opcodes of the NMOS 6502 -- the NES's 2A03 runs the same set (its decimal
// mode is wired off). One table serves the disassembler, the assembler that rebuilds the source
// the NES lane writes (source/nes_writer.py), and the interpreter (emulate/m6502.py).
//
// Addressing modes, and their operand size:
//
//     imp  (none)        acc  A            imm  #$nn        zp   $nn         zpx  $nn,X
//     zpy  $nn,Y         abs  $nnnn        abx  $nnnn,X     aby  $nnnn,Y     ind  ($nnnn)
//     izx  ($nn,X)       izy  ($nn),Y      rel  a branch: one signed byte, from the next instruction
#pragma once

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::decompile::m6502 {

// addressing mode -> instruction size in bytes
extern const py::dict<py::str, int64_t> SIZE;
// mnemonic -> {mode: opcode}
using ModeTable = py::dict<py::str, int64_t>;
extern const py::dict<py::str, ModeTable> _GROUPS;
// opcode -> (mnemonic, mode)
extern const py::dict<int64_t, std::tuple<py::str, py::str>> OPCODES;
extern const py::set<py::str> BRANCHES;
extern const py::set<py::str> ENDS;  // flow does not fall through
extern const py::re::Pattern _LINE;

// One decoded instruction (the Python's __slots__ class).
struct Insn {
    int64_t at = 0;
    int64_t op = 0;
    py::str mnemonic;
    py::str mode;
    std::optional<int64_t> operand;  // None for imp / acc
    int64_t size = 0;
    std::optional<int64_t> target;   // None unless a branch, JMP or JSR

    Insn() = default;
    Insn(int64_t at, int64_t op, const py::str& mnemonic, const py::str& mode, std::optional<int64_t> operand,
         int64_t size, std::optional<int64_t> target)
        : at(at), op(op), mnemonic(mnemonic), mode(mode), operand(operand), size(size), target(target) {}
    py::str repr() const;  // <8000 LDA #$01>
};

// The instruction at code[offset], which sits at address `pc`; None for an undocumented
// opcode or one cut off by the end of `code`.
std::optional<Insn> decode(const py::bytes& code, int64_t offset, int64_t pc);

// The instruction in ca65 syntax; an address that has a label is written as the label.
// (A zero-page address is written with two hex digits, an absolute one with four: ca65 picks
// the instruction's size from how the address is written, so the bytes come back the same.)
py::str text(const Insn& insn, const py::dict<int64_t, py::str>& labels = py::dict<int64_t, py::str>());

// The bytes of one instruction written as text() writes it. Labels are resolved from
// `labels` ({name: address}); `a:` forces an absolute address.
py::bytes assemble(const py::str& line, int64_t pc, const py::dict<py::str, int64_t>& labels = py::dict<py::str, int64_t>());

}  // namespace codebreaker::decompile::m6502
