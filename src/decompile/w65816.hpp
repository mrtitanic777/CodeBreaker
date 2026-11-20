// decompile/w65816.hpp -- the 65C816's instructions (the Super Nintendo's CPU, the 5A22 is one): the table, a disassembler, an assembler.
//
// The 65C816 is a 65C02 with a 24-bit address space and 16-bit registers. The accumulator is 8 or 16 bits wide (the M flag) and X and Y are 8 or 16
// (the X flag): the instructions whose immediate operand is the register's size -- ADC AND BIT CMP EOR LDA ORA SBC for the accumulator, CPX CPY LDX
// LDY for the index registers -- are one byte longer when it is 16 bits, so a disassembler has to know the flags at each instruction (REP and SEP set
// them; in emulation mode they are forced to 8 bits). `decode` takes them as arguments.
//
//     decode(code, offset, pc, m16, x16)       the instruction at code[offset] sitting at the 24-bit address pc, or nullopt
//     text(insn, labels, direct_page)          "LDA #$1234" / "LDA [$12],y" / "JSL $808000" (ca65 syntax)
//     assemble(line, pc, labels, a16, i16)     the bytes of one instruction line, the widths being what `.a16` / `.i16` last said
//
// Addressing modes (the name, the operand's size, ca65's spelling):
//
//     Imp  -               Acc  A               Imm8 1  #$nn  (REP SEP COP BRK WDM)       ImmM 1 or 2  #$nn  (the accumulator's width)
//     ImmX 1 or 2  #$nn  (the index registers')  Dp 1  $nn     DpX 1  $nn,x     DpY 1  $nn,y
//     Abs  2  $nnnn  (the data bank)    AbsX 2  $nnnn,x    AbsY 2  $nnnn,y    Long 3  $nnnnnn    LongX 3  $nnnnnn,x
//     AbsInd 2  ($nnnn)  (JMP)    AbsIndX 2  ($nnnn,x)  (JMP, JSR)    AbsIndL 2  [$nnnn]  (JML)
//     DpInd 1  ($nn)  (and PEI)    DpIndX 1  ($nn,x)    DpIndY 1  ($nn),y    DpIndL 1  [$nn]    DpIndLY 1  [$nn],y
//     Rel 1  a branch from the next instruction, within the bank    RelLong 2  (BRL, PER)
//     Sr 1  $nn,s    SrIndY 1  ($nn,s),y    Block 2  `MVN src, dst`: the two bank bytes (the opcode is followed by the destination, then the source)
//
// A zero-page-sized value in an Abs mode is written `a:$0012`, a 16-bit value in a Long mode `f:$001234`: ca65 chooses the form from the value's size, and
// these say which was meant (as `a:` does for decompile/m6502).
#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>

#include "core/bytes.hpp"
#include "decompile/labels.hpp"

namespace cb::decompile::w65816 {

using decompile::AddressLabels;
using decompile::NameLabels;

enum class Mode : std::uint8_t {
    Imp, Acc, Imm8, ImmM, ImmX, Dp, DpX, DpY, Abs, AbsX, AbsY, Long, LongX, AbsInd, AbsIndX, AbsIndL, DpInd, DpIndX, DpIndY, DpIndL, DpIndLY, Rel, RelLong, Sr, SrIndY, Block
};

// The instruction's length in bytes in this mode with the accumulator and index registers 16 bits wide or not.
constexpr std::uint8_t instruction_size(Mode mode, bool m16, bool x16) {
    switch (mode) {
        case Mode::Imp:
        case Mode::Acc: return 1;
        case Mode::ImmM: return m16 ? 3 : 2;
        case Mode::ImmX: return x16 ? 3 : 2;
        case Mode::Abs:
        case Mode::AbsX:
        case Mode::AbsY:
        case Mode::AbsInd:
        case Mode::AbsIndX:
        case Mode::AbsIndL:
        case Mode::RelLong:
        case Mode::Block: return 3;
        case Mode::Long:
        case Mode::LongX: return 4;
        default: return 2;
    }
}

struct OpcodeEntry {
    std::string_view mnemonic;   // upper case, static storage
    Mode mode;
};

// The 256 opcodes: every one is an instruction (WDM, the reserved one, takes a signature byte).
const std::array<OpcodeEntry, 256>& opcode_table();
std::optional<std::uint8_t> opcode_of(std::string_view mnemonic, Mode mode);
bool is_mnemonic(std::string_view mnemonic);
bool is_branch(std::string_view mnemonic);   // BCC BCS BEQ BMI BNE BPL BVC BVS BRA (two bytes) and BRL, PER (three): a relative branch
bool ends_flow(std::string_view mnemonic);   // JMP JML RTS RTL RTI BRA BRL BRK COP STP: flow does not fall through

struct Insn {
    std::uint32_t at = 0;                  // the 24-bit address
    std::uint8_t op = 0;
    std::string_view mnemonic;
    Mode mode = Mode::Imp;
    std::uint32_t operand = 0;             // the immediate, address or displacement as written (a Block's destination bank)
    std::uint32_t operand2 = 0;            // a Block's source bank
    std::uint8_t size = 0;
    bool m16 = false;                      // the widths the instruction was decoded under
    bool x16 = false;
    std::optional<std::uint32_t> target;   // a branch, JMP, JSR, JML or JSL: where it goes (a 24-bit address; a branch or JMP/JSR stays in its bank)
};

// The instruction at code[offset], which sits at address `pc`; nullopt when it is cut off by the end of `code`.
std::optional<Insn> decode(ByteView code, std::size_t offset, std::uint32_t pc, bool m16, bool x16);

// The instruction in ca65 syntax. `labels` names the value an operand has (for an Abs mode the 16-bit address, for a Long mode the 24-bit one, for
// a branch the target's 24-bit address; JMP and JSR by their 16-bit operand); `direct_page` names a direct-page byte. A name is written as it is
// given, so a label may be an expression (`.loword(sub_018123)`).
std::string text(const Insn& insn, const AddressLabels& labels = {}, const AddressLabels& direct_page = {});

// The bytes of one instruction written as text() writes it. `a16` and `i16` are the widths `.a16` / `.i16` last gave. Throws ParseError when the line
// is not an instruction, names an unknown mnemonic, label or number, uses a form the mnemonic lacks, or a value that does not fit it.
Bytes assemble(std::string_view line, std::uint32_t pc, const NameLabels& labels = {}, bool a16 = false, bool i16 = false);

}  // namespace cb::decompile::w65816
