// codebreaker/decompile/z80.hpp -- C++ port of codebreaker.decompile.z80.
//
// z80.py -- the Z80's instructions: the table, a disassembler, an assembler.
//
//     z80::decode(code, at, pc)        Insn(text, size, target, flow, operands) or None
//     z80::text(insn, labels)          "LD A,(IX+5)" / "JR NZ,loop" / "CALL sub_0150"
//     z80::assemble(line, pc, labels)  the bytes of one instruction line
//
// Every opcode of the Z80, documented and not: the unprefixed page, CB (rotates and bits), ED
// (the rest), and the DD/FD pages that put IX/IY (and their halves IXH IXL IYH IYL) in place of HL
// (H L), with DDCB/FDCB for the indexed bit operations. The table is generated from the way the
// CPU decodes an opcode (its fields x y z p q, as in "Decoding Z80 opcodes", z80.info), so the
// disassembler, the assembler and the interpreter (emulate/z80.py) read one description.
//
// Syntax is Zilog's, as assemblers for the Master System, Game Gear, MSX and Spectrum take it
// (WLA-DX, sjasmplus, pasmo): `LD A,(IX+5)`, `JP (HL)`, `EX AF,AF'`, `RST $38`, `IM 1`. Numbers
// are written `$1F`. Operands an assembler may take as a value:
//
//     n   a byte          nn  a word         d   an index displacement, -128..127
//     e   a relative jump: a signed byte from the next instruction (written as its target)
#pragma once

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::decompile::z80 {

extern const py::list<py::str> R;
extern const py::list<py::str> RP;
extern const py::list<py::str> RP2;
extern const py::list<py::str> CC;
extern const py::list<py::str> ALU;
extern const py::list<py::str> ROT;
extern const py::list<py::str> IM;
// {(row, column): mnemonic}
extern const py::dict<std::tuple<int64_t, int64_t>, py::str> BLOCK;

// The table: (prefix bytes, opcode) -> pattern (None for an ED opcode that does nothing).
using TableKey = std::tuple<py::bytes, int64_t>;
using Table = py::dict<TableKey, std::optional<py::str>>;
extern const Table TABLE;
// placeholder sizes
extern const py::dict<py::str, int64_t> _PLACE;

// SyntaxError (eval of a malformed expression in value()); the runtime has none of its own.
struct SyntaxError : py::Exception {
    explicit SyntaxError(const std::string& m) : py::Exception(m) {}
    const char* type() const override { return "SyntaxError"; }
};

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

// {normalised pattern: (prefix + opcode bytes, displacement-first?)} for the assembler; for a
// displacement-first (DDCB/FDCB) entry `code` is the two prefix bytes and `cbop` the opcode.
struct AsmEntry {
    py::bytes code;
    std::optional<int64_t> cbop;
    bool displacement_first = false;
};
using AsmTable = py::dict<py::str, AsmEntry>;

extern const py::set<py::str> _WORDS;
extern const py::re::Pattern _NUMBER;

std::optional<py::str> _main(int64_t op);
py::str _cb(int64_t op, const py::str& target = py::str("(HL)"));
std::optional<py::str> _ed(int64_t op);
std::optional<py::str> _index(const std::optional<py::str>& pattern, const py::str& ix);
Table _build();
py::str _flow(const py::str& pattern);
py::list<py::str> _fields(const py::str& pattern);
std::optional<Insn> _lone_prefix(const py::bytes& code, int64_t at, int64_t pc);
std::optional<Insn> decode(const py::bytes& code, int64_t at, int64_t pc);
py::str text(const Insn& insn, const py::dict<int64_t, py::str>& labels = py::dict<int64_t, py::str>());
bool _canonical(const Insn& insn);
py::str _plain_text(const Insn& insn, const py::dict<int64_t, py::str>& labels);
AsmTable _patterns();
int64_t value(const py::str& expr, const py::dict<py::str, int64_t>& labels, int64_t pc = 0);
py::list<py::str> _split_operands(const py::str& s);
py::bytes assemble(const py::str& line, int64_t pc = 0, const py::dict<py::str, int64_t>& labels = py::dict<py::str, int64_t>());
bool _balanced(const py::str& s);
py::list<py::list<py::str>> _choices(int64_t count);

}  // namespace codebreaker::decompile::z80
