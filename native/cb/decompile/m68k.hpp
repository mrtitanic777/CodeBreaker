// cb/decompile/m68k.hpp -- the Motorola 68000's instructions: a disassembler and an assembler.
//
//     decode(code, at, pc)         the instruction at code[at:] running at address pc, or nullopt
//     text(insn, labels)           "MOVE.L D0,(A1)+" / "BNE.S loop" / "LEA $10(A0),A1"
//     assemble(line, pc, labels)   the bytes of one instruction line
//
// Every instruction of the 68000 (not the 68010's and later additions), with the addressing modes each one allows:
// an opcode word whose fields name a mode the instruction does not take is not an instruction, so data is not read as
// code. Where the CPU ignores bits (the brief extension word's 8-10, an immediate byte's high byte), only the encoding
// an assembler writes is taken, so that what is written as an instruction assembles back to the same bytes.
//
// Syntax is Motorola's, as vasm, asm68k and SNASM68K take it: `MOVE.W #$1234,(A0)+`, `LEA $10(A0),A1`,
// `MOVE.B 2(A0,D1.W),D2`, `JSR (sub).L`, `($FF8000).W`, `label(PC)`, `MOVEM.L D0-D7/A0-A6,-(SP)` (A7 is written SP),
// `BRA.S` / `BRA.W` for the two branch sizes.
//
// The assembler is the disassembler run backwards: every one of the 65,536 opcode words is decoded once into its shape
// (the text with the values that live in extension words left out), and a line is assembled by finding the opcode word
// with its shape, then writing its extension words. Expressions are the Z80 module's (decompile/z80.hpp), `*` being
// the address of the instruction as `$` is there.
#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/error.hpp"
#include "cb/decompile/labels.hpp"
#include "cb/decompile/z80.hpp"

namespace cb::decompile::m68k {

// How control leaves an instruction (the Z80 module's: None, Jump, Branch, Call, Return, Indirect, Halt).
using Flow = z80::Flow;

// The size suffix of an instruction: `.B .W .L`, and `.S` for a branch with its displacement in the opcode word.
enum class Size : std::uint8_t { None, Byte, Word, Long, Short };
// "", "B", "W", "L", "S".
std::string_view size_name(Size size);

// The addressing mode (or the kind of value) an operand is.
enum class OpKind : std::uint8_t {
    Dreg,       // Dn
    Areg,       // An (A7 is written SP)
    Indirect,   // (An)
    PostInc,    // (An)+
    PreDec,     // -(An)
    Disp,       // d16(An)
    Index,      // d8(An,Xn.s)
    AbsW,       // (xxx).W, a sign-extended 16-bit address
    AbsL,       // (xxx).L
    PcDisp,     // d16(PC), held as the address it reaches
    PcIndex,    // d8(PC,Xn.s), held as the address it reaches
    Imm,        // #value, of `size` B W L
    Quick,      // #n in the opcode word (ADDQ, SUBQ, shifts, MOVEQ, TRAP)
    Label,      // a branch's 16-bit displacement, held as the address it reaches
    Label8,     // a short branch's displacement in the opcode word; `target` is the address it reaches
    Regs,       // a MOVEM register list (a bit mask: bit 0 is D0, bit 15 is A7)
    Ccr,
    Sr,
    Usp,
    Expr,       // a bare value the assembler read, not yet made an address (never decoded)
};
// The name a kind has in a shape: "D", "A", "ind", "post", "pre", "disp", "index", "absw", "absl", "pcdisp", "pcindex",
// "imm", "quick", "label", "label8", "regs", "ccr", "sr", "usp", "expr".
std::string_view kind_name(OpKind kind);

// An index register: Dn or An, with the .W or .L it is used at.
struct IndexReg {
    bool address = false;
    std::uint8_t number = 0;
    Size size = Size::Word;   // Word or Long
};

// One operand. Which members mean something depends on the kind (named in each line below); the others are zero.
struct Op {
    OpKind kind = OpKind::Dreg;
    std::uint8_t reg = 0;         // Dreg Areg Indirect PostInc PreDec Disp Index: the register number
    // The number the kind carries, wide enough for all of them: a displacement (Disp, Index; signed), an address
    // (AbsW sign-extended, AbsL, PcDisp, PcIndex, Label, Expr: a 32-bit address, a result of the assembler's
    // expressions that is range-checked when it is written), the immediate, the quick value (signed for MOVEQ), the
    // register mask of a Regs, or the raw displacement byte of a Label8.
    std::int64_t value = 0;
    IndexReg index;               // Index PcIndex
    Size size = Size::None;       // Imm: Byte, Word or Long
    std::uint32_t target = 0;     // Label8: the address it reaches
    bool predecrement = false;    // Regs: the mask is in -(An)'s reversed order (it is bit 15 = D0)

    // The operand's part of a shape: registers and in-opcode values, not extension words.
    std::string key() const;
    friend bool operator==(const Op& a, const Op& b) { return a.key() == b.key(); }
};

// One decoded instruction.
struct Insn {
    std::string mnemonic;                  // "MOVE", "BNE", "DBF", "MOVEM" ...
    Size size = Size::None;
    std::array<Op, 2> ops{};
    std::uint8_t op_count = 0;
    std::uint8_t length = 0;               // in bytes (the 68000's longest instruction is 10)
    std::optional<std::uint32_t> target;   // where a branch, call or absolute jump goes
    Flow flow = Flow::None;
    std::uint32_t pc = 0;                  // the address it runs at
    std::uint16_t word = 0;                // its opcode word
    std::array<std::uint8_t, 10> raw{};    // its `length` bytes

    ByteView bytes() const { return {raw.data(), length}; }
    std::span<const Op> operands() const { return {ops.data(), op_count}; }
    // The text with the extension words' values left out ("MOVE.L D0,post1"): what the assembler finds an opcode by.
    std::string shape() const;
};

// The instruction at code[at:], which runs at address `pc`, or nullopt when it is not one (an opcode word the CPU does
// not define, an addressing mode the instruction does not take) or the code ends inside it. `pc` wraps at 32 bits.
std::optional<Insn> decode(ByteView code, std::size_t at, std::uint32_t pc);

// The instruction as Motorola syntax. `labels` names addresses (absolute and PC-relative operands, branch targets,
// long immediates); an empty name counts as no label.
std::string text(const Insn& insn, const AddressLabels& labels = {});
std::string operand_text(const Op& op, const AddressLabels& labels = {});

// A numeric expression as the assembler reads it: decompile/z80's `value`, with `*` for the instruction's address.
// ParseError for a malformed one, z80::UnknownLabel for a label not in `labels`.
std::int64_t value(std::string_view expr, const NameLabels& labels, std::uint32_t pc = 0);

// The bytes of one instruction line (`labels` resolves names; `pc` is where the instruction will be). ParseError for
// text that is not an instruction, or one whose branch or PC-relative operand is out of reach, an operand that
// does not fit its field; z80::UnknownLabel (a ParseError) for a label that is not there.
Bytes assemble(std::string_view line, std::uint32_t pc = 0, const NameLabels& labels = {});

}  // namespace cb::decompile::m68k
