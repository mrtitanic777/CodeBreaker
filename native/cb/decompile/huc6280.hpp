// cb/decompile/huc6280.hpp -- the HuC6280's instructions (the PC Engine / TurboGrafx-16's CPU): the table, a disassembler, an assembler.
//
// The HuC6280 is a 65C02 (every CMOS instruction of decompile/m6502's Cmos variant, with the Rockwell bit instructions RMBn, SMBn, BBRn and BBSn)
// and what Hudson added: the register swaps SXY SAX SAY, the clears CLA CLX CLY, the speed switches CSL CSH, SET (the next ALU instruction works
// on memory at zero page,X), the VDC stores ST0 ST1 ST2, the mapping-register moves TAM TMA, the test TST, the subroutine branch BSR and the five
// block transfers TII TDD TIN TIA TAI. The base set is decompile/m6502's, not repeated here; this file adds the forms that set cannot hold
// (an instruction with two or three operands).
//
//     decode(code, offset, pc)            the instruction at code[offset] sitting at address pc, or nullopt
//     text(insn, labels, zero_page)       "LDA $2000,X" / "TII $4000, $2000, $0100" (ca65 syntax, as `.setcpu "HuC6280"` reads it)
//     assemble(line, pc, labels)          the bytes of one instruction line
//
// Addresses are the CPU's 16-bit logical addresses: the eight mapping registers (MPR0-7, one per 8 KB page) turn them into the 21-bit physical space.
// Page zero is at $2000 (the RAM bank $F8 sits in MPR1 by the convention every program follows) -- a zero-page operand is the low byte of that.
//
// Modes beyond decompile/m6502's: ZpRel `BBR0 $20, label` (a zero-page byte and a branch), ImmZp `TST #$01, $20`, ImmZpx, ImmAbs, ImmAbx
// (an immediate mask and the byte tested) and Block `TII src, dst, length` (three 16-bit operands).
#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>

#include "cb/core/bytes.hpp"
#include "cb/decompile/labels.hpp"

namespace cb::decompile::huc6280 {

// Label tables (decompile/labels.hpp): address -> name for text(), name -> address for assemble().
using decompile::AddressLabels;
using decompile::NameLabels;

// The first fifteen are decompile/m6502's Mode, in the same order.
enum class Mode : std::uint8_t { Imp, Acc, Imm, Zp, Zpx, Zpy, Abs, Abx, Aby, Ind, Izx, Izy, Rel, Izp, Iax, ZpRel, ImmZp, ImmZpx, ImmAbs, ImmAbx, Block };

// The instruction's length in bytes (opcode and operands) in this mode.
constexpr std::uint8_t instruction_size(Mode mode) {
    switch (mode) {
        case Mode::Imp:
        case Mode::Acc: return 1;
        case Mode::Abs:
        case Mode::Abx:
        case Mode::Aby:
        case Mode::Ind:
        case Mode::Iax:
        case Mode::ZpRel:
        case Mode::ImmZp:
        case Mode::ImmZpx: return 3;
        case Mode::ImmAbs:
        case Mode::ImmAbx: return 4;
        case Mode::Block: return 7;
        default: return 2;
    }
}

struct OpcodeEntry {
    std::string_view mnemonic;   // upper case, static storage
    Mode mode;
};

// The 256-entry table: nullopt for an opcode the HuC6280 has no instruction for.
const std::array<std::optional<OpcodeEntry>, 256>& opcode_table();
std::optional<std::uint8_t> opcode_of(std::string_view mnemonic, Mode mode);
bool is_mnemonic(std::string_view mnemonic);
bool is_branch(std::string_view mnemonic);   // BCC BCS BEQ BMI BNE BPL BVC BVS BRA BSR (a two-byte relative branch)
bool ends_flow(std::string_view mnemonic);   // JMP RTS RTI BRK BRA: flow does not fall through

// One decoded instruction. `operand` holds what the mode has: one value (an immediate, an address), or two (ZpRel: the zero-page byte and the
// branch displacement; ImmZp ...: the mask and the address), or three (Block: source, destination, length).
struct Insn {
    std::uint32_t at = 0;
    std::uint8_t op = 0;
    std::string_view mnemonic;
    Mode mode = Mode::Imp;
    std::array<std::uint16_t, 3> operand{};
    std::uint8_t operands = 0;                  // how many of `operand` are set
    std::uint8_t size = 0;
    std::optional<std::uint16_t> target;        // a branch, BSR, JMP abs or JSR: where it goes
};

// The instruction at code[offset], which sits at address `pc`; nullopt for an opcode with no instruction or one cut off by the end of `code`.
std::optional<Insn> decode(ByteView code, std::size_t offset, std::uint32_t pc);

// The instruction in ca65 syntax. An address that has a label is written as the label (`labels`: by logical address); a zero-page operand takes
// its name from `zero_page` (by the byte) when it has one. A zero-page-sized absolute address is written with ca65's `a:`.
std::string text(const Insn& insn, const AddressLabels& labels = {}, const AddressLabels& zero_page = {});

// The bytes of one instruction written as text() writes it. Throws ParseError when the line is not an instruction, names an unknown mnemonic,
// label or number, uses a form the mnemonic lacks, or branches out of reach.
Bytes assemble(std::string_view line, std::uint32_t pc, const NameLabels& labels = {});

}  // namespace cb::decompile::huc6280
