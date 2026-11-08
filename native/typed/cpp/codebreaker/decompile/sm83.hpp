// codebreaker/decompile/sm83.hpp -- C++ port of codebreaker.decompile.sm83.
//
// sm83.py -- the Game Boy's CPU (Sharp SM83, "GBZ80"): the table, a disassembler, an assembler.
//
//     sm83::decode(code, at, pc)        Insn(pattern, size, values, target, flow) or None
//     sm83::text(insn, labels)          "LD A,(HL+)" / "JR NZ,loop" / "LDH ($FF44),A"
//     sm83::assemble(line, pc, labels)  the bytes of one instruction line
//
// The SM83 runs most of the Z80's 8080 subset with a different map: no IX/IY, no ED page, no
// alternate registers, no parity flag, only the conditions NZ Z NC C; and its own instructions in
// the slots that frees: LD (nn),SP, STOP, LD (HL+)/(HL-), LDH (n) and (C) for the $FF00 page, ADD
// SP,e, LD HL,SP+e, RETI at $D9, and SWAP in place of SLL on the CB page. Eleven opcodes do not
// exist ($D3 $DB $DD $E3 $E4 $EB $EC $ED $F4 $FC $FD). The table is generated from the opcode
// fields as decompile/z80.py's is, and serves the disassembler, the assembler and the
// interpreter (emulate/sm83.py).
//
// Syntax is RGBDS's (the Game Boy's usual assembler): `LD A,(HL+)`, `LDH ($FF44),A` -- an LDH
// operand is written as the full address, $FF00 + n --, `LD HL,SP+$05`, `STOP`. Operands taken as
// values: n (byte), nn (word), e (relative jump, written as its target), s (signed byte of ADD SP
// and LD HL,SP+), h (LDH's byte, written $FFxx).
#pragma once

#include "py.hpp"
#include "py_os.hpp"
#include "codebreaker/decompile/z80.hpp"

namespace codebreaker::decompile::sm83 {

extern const py::list<py::str> R;
extern const py::list<py::str> RP;
extern const py::list<py::str> RP2;
extern const py::list<py::str> CC;
extern const py::list<py::str> ALU;
extern const py::list<py::str> ROT;
extern const py::set<int64_t> ILLEGAL;

// The table: (prefix bytes, opcode) -> pattern (None for an opcode that does not exist).
using TableKey = std::tuple<py::bytes, int64_t>;
using Table = py::dict<TableKey, std::optional<py::str>>;
extern const Table TABLE;
// placeholder sizes
extern const py::dict<py::str, int64_t> _PLACE;
extern const py::set<py::str> _WORDS;

struct Insn {
    py::str pattern;
    int64_t size = 0;
    py::list<int64_t> values;
    std::optional<int64_t> target;
    py::str flow;
    int64_t pc = 0;
    py::bytes bytes;

    Insn() = default;
    Insn(const py::str& pattern, int64_t size, const py::list<int64_t>& values, std::optional<int64_t> target,
         const py::str& flow, int64_t pc, const py::bytes& raw)
        : pattern(pattern), size(size), values(values), target(target), flow(flow), pc(pc), bytes(raw) {}
    py::str text() const;   // (the property: text(self))
    py::str repr() const;   // Insn(LD A,B)
};

std::optional<py::str> _main(int64_t op);
py::str _cb(int64_t op);
py::str _flow(const py::str& pattern);
py::list<py::str> _fields(const py::str& pattern);
std::optional<Insn> decode(const py::bytes& code, int64_t at, int64_t pc);
py::str text(const Insn& insn, const py::dict<int64_t, py::str>& labels = py::dict<int64_t, py::str>());
py::bytes assemble(const py::str& line, int64_t pc = 0, const py::dict<py::str, int64_t>& labels = py::dict<py::str, int64_t>());
py::list<py::list<py::str>> _choices(int64_t count);

}  // namespace codebreaker::decompile::sm83
