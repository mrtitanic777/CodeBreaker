// codebreaker/decompile/m68k.hpp -- C++ port of codebreaker.decompile.m68k.
//
// m68k.py -- the Motorola 68000's instructions: a disassembler, an assembler.
//
//     m68k::decode(code, at, pc)       Insn(mnemonic, size, operands, length, target, flow) or None
//     m68k::text(insn, labels)         "MOVE.L D0,(A1)+" / "BNE.S loop" / "LEA $10(A0),A1"
//     m68k::assemble(line, pc, labels) the bytes of one instruction line
//
// Every instruction of the 68000 (not the 68010's and later additions), with the addressing modes
// each one allows -- an opcode word whose fields name a mode the instruction does not take is not
// an instruction, so data is not read as code. Where the CPU ignores bits (the brief extension
// word's 8-10, an immediate byte's high byte), only the encoding an assembler writes is taken, so
// that what is written as an instruction assembles back to the same bytes.
//
// Syntax is Motorola's, as vasm, asm68k and SNASM68K take it: `MOVE.W #$1234,(A0)+`,
// `LEA $10(A0),A1`, `MOVE.B 2(A0,D1.W),D2`, `JSR (sub).L`, `($FF8000).W`, `label(PC)`,
// `MOVEM.L D0-D7/A0-A6,-(SP)` (A7 is written SP), `BRA.S` / `BRA.W` for the two branch sizes.
//
// The assembler is the disassembler run backwards: every one of the 65,536 opcode words is
// decoded once into its shape (the text with the values that live in extension words left out),
// and a line is assembled by finding the opcode word with its shape, then writing its extension
// words.
//
// Notes on the port:
//  * The Python's `_decode` has three closures (`ea`, `done`, `ext_pc`) that `_line4` and `_arith`
//    receive; here they are the methods of `_Ctx`, which those two take.
//  * An Op's `size` is the Python attribute that holds a str ("B" "W" "L") for an immediate and
//    the target address (an int) for a short branch's displacement: a std::variant.
//  * An Op's `value` is None only for a register list that did not parse (`_parse_regs` ->
//    None); `value_none` says so, and `num()` is the Python arithmetic on it (TypeError).
//  * Python ints are unbounded; here they are int64_t (overflow raises OverflowError).
#pragma once

#include <functional>
#include <tuple>
#include <variant>

#include "py.hpp"
#include "py_os.hpp"
#include "codebreaker/decompile/z80.hpp"

namespace codebreaker::decompile::m68k {

extern const py::dict<int64_t, py::str> SIZES;
extern const py::list<py::str> CC;
extern const py::list<py::str> BCC;     // line 6: 0 BRA, 1 BSR, then the conditions

// addressing-mode categories (68000 programmer's manual, table 2-4)
constexpr int64_t DATA = 1, MEMORY = 2, CONTROL = 4, ALTERABLE = 8;

// The categories (mode, reg) belongs to, or None if it is not a mode.
std::optional<int64_t> _category(int64_t mode, int64_t reg);
bool _ok(int64_t mode, int64_t reg, int64_t need, bool no_an = false, bool no_imm = false);

// An index register: ("D" or "A", number).
using XReg = std::tuple<py::str, int64_t>;

// One operand. kind: D A ind post pre disp index absw absl pcdisp pcindex imm quick label regs ccr sr usp
// (label8 and expr too: a short branch's displacement, a bare value the assembler reads).
struct Op {
    py::str kind;
    int64_t reg = 0;
    int64_t value = 0;
    bool value_none = false;                   // value is None (an unparsed register list)
    std::optional<XReg> xreg;
    py::str xsize = py::str("W");
    // None; the size of an immediate ("B" "W" "L"); the target address of a label8
    std::variant<std::monostate, py::str, int64_t> size;

    Op() = default;
    explicit Op(const py::str& kind, int64_t reg = 0, int64_t value = 0, const std::optional<XReg>& xreg = std::nullopt,
                const py::str& xsize = py::str("W"))
        : kind(kind), reg(reg), value(value), xreg(xreg), xsize(xsize) {}
    // `value` as a number: TypeError for None, as the Python's arithmetic on None.
    int64_t num() const;
    // `size == s` (False for None and for a label8's address)
    bool size_is(const char* s) const;
    // The operand's part of a shape: registers and in-opcode values, not extension words.
    py::str key() const;
    friend bool operator==(const Op& a, const Op& b) { return a.key() == b.key(); }
    friend bool operator!=(const Op& a, const Op& b) { return !(a == b); }
};

struct Insn {
    py::str mnemonic;
    py::str size;
    py::list<Op> operands;
    int64_t length = 0;
    std::optional<int64_t> target;
    py::str flow;
    int64_t pc = 0;
    py::bytes bytes;
    int64_t word = 0;

    Insn() = default;
    Insn(const py::str& mnemonic, const py::str& size, const py::list<Op>& operands, int64_t length, int64_t pc,
         const py::bytes& raw, int64_t word)
        : mnemonic(mnemonic), size(size), operands(operands), length(length), flow(py::str("")), pc(pc), bytes(raw), word(word) {}
    py::str shape() const;
    py::str repr() const;   // Insn(MOVE.L D0,D1)
};

class _Reader {
public:
    py::bytes code;
    int64_t at;
    _Reader(const py::bytes& code, int64_t at) : code(code), at(at) {}
    int64_t word();         // IndexError past the end
    int64_t long_();        // (`long` in the Python)
};

// The Python's `_Not`: the opcode word is not an instruction.
struct _Not {};

int64_t _s16(int64_t v);
int64_t _s8(int64_t v);

// What `_decode` hands `_line4` and `_arith` as closures.
struct _Ctx {
    _Reader r;
    int64_t at;
    int64_t pc;
    int64_t w;
    _Ctx(const py::bytes& code, int64_t at, int64_t pc) : r(code, at), at(at), pc(pc), w(0) {}
    // the PC a PC-relative extension word is relative to
    int64_t ext_pc(int64_t ext_at) const { return pc + (ext_at - at); }
    // the operand of effective address (mode, reg), checked against `need`; raises _Not
    Op ea(int64_t m, int64_t rg, const py::str& size, int64_t need, bool no_an = false, bool no_imm = false);
    Insn done(const py::str& mn, const py::str& size, const py::list<Op>& operands, const py::str& flow = py::str(""),
              const std::optional<int64_t>& target = std::nullopt);
};

// The operand of effective address (mode, reg), reading its extension words; None if invalid.
std::optional<Op> _ea(_Reader& r, int64_t mode, int64_t reg, const py::str& size, const std::function<int64_t(int64_t)>& pc_of_ext);
// The instruction at code[at:] (running at address `pc`), or None if it is not one (or code ends inside it).
std::optional<Insn> decode(const py::bytes& code, int64_t at, int64_t pc);
std::optional<Insn> _decode(const py::bytes& code, int64_t at, int64_t pc);
Insn _line4(_Ctx& c, int64_t mode, int64_t reg, int64_t hi);
std::optional<int64_t> _absolute(const Op& op);
Insn _arith(_Ctx& c, int64_t line, int64_t mode, int64_t reg, int64_t hi);

// ---- text
py::str _reg(const py::str& kind, int64_t n);
py::str _hex(int64_t v, int64_t digits = 0);
// A MOVEM register list: D0-D7/A0-A6 (the mask is reversed for -(An)).
py::str _regs(int64_t mask, bool predec);
py::str operand_text(const Op& o, const py::dict<int64_t, py::str>& labels);
py::str text(const Insn& insn, const py::dict<int64_t, py::str>& labels = py::dict<int64_t, py::str>());

// ---- the assembler
// {shape: opcode word} for every opcode word that decodes (the first with each shape).
const py::dict<py::str, int64_t>& _shapes();
int64_t _value(const py::str& expr, const py::dict<py::str, int64_t>& labels, int64_t pc);
std::optional<XReg> _parse_reg(const py::str& s);
std::optional<int64_t> _parse_regs(const py::str& s);
// -> Op for one operand's text.
Op _parse_operand(const py::str& s, const py::dict<py::str, int64_t>& labels, int64_t pc, const py::str& size);
py::list<py::str> _split(const py::str& s);
// The bytes of one instruction (Motorola syntax); KeyError for an unknown label.
py::bytes assemble(const py::str& line, int64_t pc = 0, const py::dict<py::str, int64_t>& labels = py::dict<py::str, int64_t>());
// (mnemonic, size, operands)
using Candidate = std::tuple<py::str, py::str, py::list<Op>>;
// The ways a parsed line can be read: branches by their size, values as absolute words or longs,
// quick values, and so on.
py::list<Candidate> _candidates(const py::str& mn, const py::str& size, const py::list<Op>& ops, int64_t pc);
py::bytes _encode(int64_t word, const py::str& mn, const py::str& size, const py::list<Op>& ops, int64_t pc);
// A difference of 32-bit addresses as a signed number (the address space wraps).
int64_t _wrap(int64_t v);
int64_t _brief(const Op& o);

}  // namespace codebreaker::decompile::m68k
