// decompile/i8048.hpp -- the Intel 8048's instructions (the Magnavox Odyssey 2's CPU, also the 8035/8039/8748): the table, a disassembler, an assembler.
//
//     decode(code, at, pc)         the instruction at code[at:] running at address pc, or nullopt
//     text(insn, labels)           "MOV A,#$12"  "ADD A,R3"  "MOV @R0,A"  "DJNZ R2,L_0420"  "JMP sub_0450"  "MOVX A,@R1"
//     assemble(line, pc, labels)   the bytes of one instruction line
//
// One- and two-byte instructions; 244 of the 256 opcodes are defined. The 8048 has 12 address lines: `JMP` and `CALL` carry 11 bits (three in the opcode, eight in the second
// byte) and the memory-bank flip-flop (SEL MB0 / SEL MB1) supplies the twelfth. This table has no way to know the flip-flop, so a `JMP` / `CALL` is taken to stay in the bank of its
// own address. A conditional jump (`JNZ`, `JC`, `JB0` .. `JB7`, `JT0`, `DJNZ` ...) goes to a place in the page of its second byte. Syntax: `A`, `R0`..`R7`, `@R0` / `@R1`, `#n`,
// `P1` `P2` `P4`..`P7` `BUS` `DBB` `T` `PSW` `C` `F0` `F1`, `I` `TCNTI` `CNT` `CLK`; numbers are `$1F`, `0x1F`, `%101` or decimal; an address is written as a label or `$0420`.
#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>

#include "core/bytes.hpp"
#include "core/error.hpp"
#include "decompile/labels.hpp"

namespace cb::decompile::i8048 {

// What follows the opcode.
enum class Kind : std::uint8_t { None, Imm, Addr11, Addr8, Djnz };

// How control leaves an instruction.
enum class Flow : std::uint8_t { None, Branch, Jump, Call, Return, Indirect };

struct OpcodeEntry {
    std::string text;   // the mnemonic and operands up to the operand byte ("MOV A,#", "JMP", "DJNZ R3,")
    Kind kind = Kind::None;
};

// The 256-entry table: nullopt for an opcode the 8048 does not define.
const std::array<std::optional<OpcodeEntry>, 256>& opcode_table();

struct Insn {
    std::uint32_t at = 0;
    std::uint8_t size = 0;
    std::uint8_t op = 0;
    std::uint8_t operand = 0;                   // the second byte
    const OpcodeEntry* entry = nullptr;
    std::optional<std::uint32_t> target;        // a jump or call: where it goes
    Flow flow = Flow::None;
    std::array<std::uint8_t, 2> raw{};
    ByteView bytes() const { return {raw.data(), size}; }
};

// The instruction at code[offset], which sits at address `pc`; nullopt for an undefined opcode or one cut off by the end of `code`.
std::optional<Insn> decode(ByteView code, std::size_t offset, std::uint32_t pc);

}  // namespace cb::decompile::i8048
