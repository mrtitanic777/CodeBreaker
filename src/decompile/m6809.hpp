// decompile/m6809.hpp -- the Motorola 6809's instructions: the table, a disassembler, an assembler.
//
// Every documented opcode of the 6809 (the Vectrex's CPU): the 256 of the first page and the two prefixed pages ($10 and $11). One table serves the
// disassembler, the assembler that rebuilds the source the Vectrex lane writes (source/console_m6809) and the interpreter (emulate/m6809).
//
//     decode(code, at, pc)         the instruction at code[at:] running at address pc, or nullopt
//     text(insn, labels)           "LDA #$12" / "LDX <$10" / "STA ,X+" / "BNE L_0040" / "PSHS A,B,X"
//     assemble(line, pc, labels)   the bytes of one instruction line
//
// Addressing modes and how they are written (the syntax is the common one of the 6809 assemblers, lwasm and as9 among them):
//
//     inherent   RTS  NEGA         immediate  #$12 / #$1234          direct  <$10 (the `<` is always written)     extended  $C800 (a bare address is extended)
//     relative   BRA label         long       LBRA label             indexed ,X  5,X  -3,Y  A,X  B,U  D,S  ,X+  ,X++  ,-X  ,--X  <$20,X (8-bit)  >$20,X (16-bit)
//                $20,PCR (PC-relative, the offset as written; `<` and `>` choose its size)   indirect [ ... ]: [,X] [$10,X] [,X++] [,--X] [$C800] [D,X]
//     registers  PSHS A,B,X,PC     (the stack pushes in CC A B DP X Y U/S PC order)       TFR A,B   EXG X,Y
//
// A constant offset is written with the shortest form the postbyte has (5, 8 or 16 bits) unless the source says `<` or `>`; the disassembler writes `<` or `>` where the
// instruction's own encoding is the longer one, so what is written assembles back to the same bytes.
#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>

#include "core/bytes.hpp"
#include "core/error.hpp"
#include "decompile/labels.hpp"

namespace cb::decompile::m6809 {

enum class Mode : std::uint8_t { Inh, Imm8, Imm16, Dir, Ext, Idx, Rel8, Rel16, Regs, Pair };

// How control leaves an instruction.
enum class Flow : std::uint8_t { None, Branch, Jump, Call, Return, Indirect, Halt };

struct OpcodeEntry {
    std::string_view mnemonic;   // upper case, static storage
    Mode mode;
};

// The 256-entry table of a page (0: no prefix, 1: $10, 2: $11): nullopt for an opcode the 6809 does not define.
const std::array<std::optional<OpcodeEntry>, 256>& opcode_table(unsigned page);

// The (page, opcode) of `mnemonic` in `mode`; nullopt when there is none.
struct Opcode {
    unsigned page;
    std::uint8_t op;
};
std::optional<Opcode> opcode_of(std::string_view mnemonic, Mode mode);
bool is_mnemonic(std::string_view mnemonic);
bool is_branch(std::string_view mnemonic);   // the short and long conditional branches, BRA, LBRA, BSR, LBSR

// An indexed operand, decoded.
struct Index {
    std::uint8_t postbyte = 0;
    std::uint8_t reg = 0;           // 0 X, 1 Y, 2 U, 3 S (the base register)
    enum class Kind : std::uint8_t { Offset5, Offset8, Offset16, A, B, D, Inc1, Inc2, Dec1, Dec2, Zero, Pc8, Pc16, Extended };
    Kind kind = Kind::Zero;
    std::int32_t offset = 0;        // the constant offset (Offset*, Pc*), or the address of an extended indirect
    bool indirect = false;
};

struct Insn {
    std::uint32_t at = 0;                       // the address it sits at
    unsigned page = 0;
    std::uint8_t op = 0;
    std::string_view mnemonic;
    Mode mode = Mode::Inh;
    std::uint8_t size = 0;
    std::uint32_t value = 0;                    // Imm8, Imm16, Dir (the address byte), Ext (the address), Regs (the mask), Pair (the postbyte)
    Index index;                                // Idx
    std::optional<std::uint32_t> target;        // a branch, a call, or a JMP / JSR to an absolute address: where it goes
    Flow flow = Flow::None;
    std::array<std::uint8_t, 6> raw{};
    ByteView bytes() const { return {raw.data(), size}; }
};

// The instruction at code[offset], which sits at address `pc`; nullopt for an undefined opcode or one cut off by the end of `code`.
std::optional<Insn> decode(ByteView code, std::size_t offset, std::uint32_t pc);

}  // namespace cb::decompile::m6809
