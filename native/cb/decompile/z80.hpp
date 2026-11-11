// cb/decompile/z80.hpp -- the Z80's instructions: the table, a disassembler, an assembler.
//
//     decode(code, at, pc)         the instruction at code[at:] running at address pc, or nullopt
//     text(insn, labels)           "LD A,(IX+5)" / "JR NZ,loop" / "CALL sub_0150"
//     assemble(line, pc, labels)   the bytes of one instruction line
//
// Every opcode of the Z80, documented and not: the unprefixed page, CB (rotates and bits), ED (the rest), and the
// DD/FD pages that put IX/IY (and their halves IXH IXL IYH IYL) in place of HL (H L), with DDCB/FDCB for the indexed
// bit operations. The table is generated from the way the CPU decodes an opcode (its fields x y z p q, as in
// "Decoding Z80 opcodes", z80.info), so the disassembler, the assembler and the interpreter (emulate/z80) read one
// description.
//
// Syntax is Zilog's, as assemblers for the Master System, Game Gear, MSX and Spectrum take it (WLA-DX, sjasmplus,
// pasmo): `LD A,(IX+5)`, `JP (HL)`, `EX AF,AF'`, `RST $38`, `IM 1`. Numbers are written `$1F`. Operands an assembler
// may take as a value:
//
//     n   a byte          nn  a word         d   an index displacement, -128..127
//     e   a relative jump: a signed byte from the next instruction (written as its target)
//
// The SM83 (Game Boy) lane reuses the expression evaluator and the operand splitter: value(), split_operands().
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

namespace cb::decompile::z80 {

// The names the opcode fields select, as the Z80 decodes them (r, rp, rp2, cc, alu, rot, im).
inline constexpr std::array<std::string_view, 8> kRegisters = {"B", "C", "D", "E", "H", "L", "(HL)", "A"};
inline constexpr std::array<std::string_view, 4> kRegisterPairs = {"BC", "DE", "HL", "SP"};
inline constexpr std::array<std::string_view, 4> kRegisterPairsAf = {"BC", "DE", "HL", "AF"};
inline constexpr std::array<std::string_view, 8> kConditions = {"NZ", "Z", "NC", "C", "PO", "PE", "P", "M"};
inline constexpr std::array<std::string_view, 8> kAlu = {"ADD A,", "ADC A,", "SUB ", "SBC A,", "AND ", "XOR ", "OR ", "CP "};
inline constexpr std::array<std::string_view, 8> kRotates = {"RLC", "RRC", "RL", "RR", "SLA", "SRA", "SLL", "SRL"};
inline constexpr std::array<std::string_view, 8> kInterruptModes = {"0", "0", "1", "2", "0", "0", "1", "2"};

// The opcode pages: the unprefixed one, the CB, ED, DD and FD prefixes, and DDCB / FDCB (DD CB d op).
enum class Page : std::uint8_t { Main, CB, ED, DD, FD, DDCB, FDCB };

// The pattern of an opcode on a page ("LD B,n", "JR NZ,e", "LD (IX+d),n"), or an empty view when the opcode is not an
// instruction there (a prefix byte on the main page; the ED opcodes that do nothing; a DD/FD opcode the prefix does
// not change). In a pattern `n` is a byte, `nn` a word, `d` an index displacement, `e` a relative jump target.
std::string_view pattern_of(Page page, std::uint8_t opcode);

// How control leaves an instruction.
enum class Flow : std::uint8_t {
    None,                // falls through
    Jump,                // JP nn, JR e
    Branch,              // JP cc,nn, JR cc,e, DJNZ e
    Call,                // CALL nn, RST n
    ConditionalCall,     // CALL cc,nn
    Return,              // RET, RETI, RETN
    ConditionalReturn,   // RET cc
    Indirect,            // JP (HL), JP (IX), JP (IY)
    Halt,
};
// The flow's name as the documentation has it ("jump", "branch", "call", "ccall", "ret", "cret", "indirect", "halt";
// "" for None).
std::string_view flow_name(Flow flow);
// The flow of an instruction pattern.
Flow flow_of(std::string_view pattern);

// One decoded instruction.
struct Insn {
    std::string pattern;                       // "LD A,(IX+d)"; ".db $DD" for a prefix that does nothing
    std::uint8_t size = 0;
    std::array<std::int32_t, 2> values{};      // the n / nn / d / e of the pattern, in order (d signed; e is the target)
    std::uint8_t value_count = 0;
    std::optional<std::uint16_t> target;       // where a jump, call, RST or relative branch goes
    Flow flow = Flow::None;
    std::uint32_t pc = 0;                      // the address it runs at
    std::array<std::uint8_t, 4> raw{};         // its `size` bytes

    ByteView bytes() const { return {raw.data(), size}; }
    std::span<const std::int32_t> operands() const { return {values.data(), value_count}; }
};

// The instruction at code[at:], which runs at address `pc`; nullopt when the code ends inside it.
std::optional<Insn> decode(ByteView code, std::size_t at, std::uint32_t pc);

// The instruction as source; `labels` names jump targets and absolute words. An encoding that is not the one an
// assembler writes for its instruction (the undocumented mirrors: NEG at ED 4C ... 7C, RETN, IM, LD HL,(nn) at ED 6B) is
// written as its bytes, the instruction in a comment, so that the source rebuilds the same.
std::string text(const Insn& insn, const AddressLabels& labels = {});
// The instruction as an assembler writes it, whatever its encoding.
std::string plain_text(const Insn& insn, const AddressLabels& labels = {});
// Whether the instruction's prefix and opcode are those its pattern assembles to.
bool is_canonical(const Insn& insn);

// An unknown label in an expression or operand: routine in a two-pass assembler, so it has its own type and the
// name; `try_value` reports it as a Result.
class UnknownLabel : public ParseError {
public:
    explicit UnknownLabel(std::string name) : ParseError("", 0, 0, "unknown label " + name), name_(std::move(name)) {}
    const std::string& name() const { return name_; }
    CB_ERROR_BOILERPLATE(UnknownLabel)

private:
    std::string name_;
};

// A numeric expression: numbers ($1F 0x1F 1Fh %0101 31 'c'), labels, `$` (here), + - * / (floor division) & | ^ ~ << >>
// and parentheses, with the usual precedence; a leading `<` or `>` takes the low or high byte of what follows
// (WLA-DX). ParseError for anything else (and for a division by zero or an overflow of 64 bits), UnknownLabel for a label
// not in `labels`.
Result<std::int64_t> try_value(std::string_view expr, const NameLabels& labels, std::uint32_t pc = 0);
std::int64_t value(std::string_view expr, const NameLabels& labels, std::uint32_t pc = 0);

// Split an operand list at the commas that are not inside parentheses; each piece is trimmed, an empty last piece is
// dropped.
std::vector<std::string> split_operands(std::string_view operands);
// Are the parentheses of the text balanced (never closing more than opened)?
bool balanced(std::string_view s);

// The bytes of one instruction (`labels` resolves names). UnknownLabel for a label that is not there, ParseError for
// something that is not an instruction or a relative jump out of reach.
Bytes assemble(std::string_view line, std::uint32_t pc = 0, const NameLabels& labels = {});

}  // namespace cb::decompile::z80
