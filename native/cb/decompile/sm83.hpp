// cb/decompile/sm83.hpp -- the Game Boy's CPU (Sharp SM83, "GBZ80"): the table, a disassembler, an assembler.
//
//     decode(code, at, pc)         the instruction at code[at:] running at address pc, or nullopt
//     text(insn, labels)           "LD A,(HL+)" / "JR NZ,loop" / "LDH ($FF44),A"
//     assemble(line, pc, labels)   the bytes of one instruction line
//
// The SM83 runs most of the Z80's 8080 subset with a different map: no IX/IY, no ED page, no alternate registers, no
// parity flag, only the conditions NZ Z NC C; and its own instructions in the slots that frees: LD (nn),SP, STOP,
// LD (HL+)/(HL-), LDH (n) and (C) for the $FF00 page, ADD SP,e, LD HL,SP+e, RETI at $D9, and SWAP in place of SLL on
// the CB page. Eleven opcodes do not exist ($D3 $DB $DD $E3 $E4 $EB $EC $ED $F4 $FC $FD). The table is generated from
// the opcode fields as decompile/z80's is, and serves the disassembler, the assembler and the interpreter
// (emulate/sm83).
//
// Syntax is RGBDS's (the Game Boy's usual assembler): `LD A,(HL+)`, `LDH ($FF44),A` -- an LDH operand is written as the
// full address, $FF00 + n --, `LD HL,SP+$05`, `STOP`. Operands taken as values:
//
//     n  a byte         nn  a word        e  a relative jump, written as its target
//     s  the signed byte of ADD SP,s and LD HL,SP+s        h  LDH's byte, written as the address $FFxx
//
// The expression evaluator and the operand splitter are the Z80 module's (z80::value, z80::split_operands).
#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>

#include "cb/core/bytes.hpp"
#include "cb/core/error.hpp"
#include "cb/decompile/labels.hpp"
#include "cb/decompile/z80.hpp"

namespace cb::decompile::sm83 {

// The names the opcode fields select (r, rp, rp2, cc, alu, rot). The ALU names are the Z80's.
inline constexpr std::array<std::string_view, 8> kRegisters = {"B", "C", "D", "E", "H", "L", "(HL)", "A"};
inline constexpr std::array<std::string_view, 4> kRegisterPairs = {"BC", "DE", "HL", "SP"};
inline constexpr std::array<std::string_view, 4> kRegisterPairsAf = {"BC", "DE", "HL", "AF"};
inline constexpr std::array<std::string_view, 4> kConditions = {"NZ", "Z", "NC", "C"};
inline constexpr std::array<std::string_view, 8> kAlu = {"ADD A,", "ADC A,", "SUB ", "SBC A,", "AND ", "XOR ", "OR ", "CP "};
inline constexpr std::array<std::string_view, 8> kRotates = {"RLC", "RRC", "RL", "RR", "SLA", "SRA", "SWAP", "SRL"};

// The opcodes that are not instructions on the SM83.
inline constexpr std::array<std::uint8_t, 11> kIllegalOpcodes = {0xD3, 0xDB, 0xDD, 0xE3, 0xE4, 0xEB, 0xEC, 0xED, 0xF4, 0xFC, 0xFD};

// The two opcode pages: the unprefixed one and the one behind the CB prefix ($CB itself is the prefix, not an
// instruction).
enum class Page : std::uint8_t { Main, CB };

// The pattern of an opcode on a page ("LD B,n", "JR NZ,e", "LDH (h),A", "SWAP B"), or an empty view when the opcode is
// not an instruction there. In a pattern `n` is a byte, `nn` a word, `e` a relative jump target, `s` a signed byte, `h`
// LDH's byte.
std::string_view pattern_of(Page page, std::uint8_t opcode);

// How control leaves an instruction (the Z80's categories; the SM83 has no DJNZ and no indirect jump but JP HL).
using Flow = z80::Flow;
// The flow of an instruction pattern.
Flow flow_of(std::string_view pattern);

// One decoded instruction. Its pattern has at most one operand field.
struct Insn {
    std::string pattern;
    std::uint8_t size = 0;
    std::optional<std::int32_t> value;    // the n / nn / e / s / h of the pattern (e is the target; h is $FFxx)
    std::optional<std::uint16_t> target;  // where a jump, call, RST or relative branch goes
    Flow flow = Flow::None;
    std::uint32_t pc = 0;                 // the address it runs at
    std::array<std::uint8_t, 3> raw{};    // its `size` bytes

    ByteView bytes() const { return {raw.data(), size}; }
};

// The instruction at code[at:], which runs at address `pc`; nullopt when the code ends inside it or the opcode does not
// exist.
std::optional<Insn> decode(ByteView code, std::size_t at, std::uint32_t pc);

// The instruction as source; `labels` names jump targets, absolute words and LDH addresses. A STOP with a second byte
// other than 0 is written as its bytes (`DB $10,$xx`), which is what an assembler cannot write as STOP.
std::string text(const Insn& insn, const AddressLabels& labels = {});

// The bytes of one instruction (`labels` resolves names). z80::UnknownLabel for a label that is not there, ParseError for
// something that is not an instruction, a relative jump out of reach, or an LDH address outside $FF00-$FFFF.
Bytes assemble(std::string_view line, std::uint32_t pc = 0, const NameLabels& labels = {});

}  // namespace cb::decompile::sm83
