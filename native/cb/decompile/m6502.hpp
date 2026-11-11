// cb/decompile/m6502.hpp -- the 6502's instructions: the table, a disassembler, an assembler.
//
// The 151 documented opcodes of the NMOS 6502 -- the NES's 2A03 runs the same set (its decimal mode is wired
// off). One table serves the disassembler, the assembler that rebuilds the source the NES lane writes
// (source/nes_writer), and the interpreter (emulate/m6502).
//
//     decode(code, offset, pc)            the instruction at code[offset] sitting at address pc, or nullopt
//     text(insn, labels)                  "LDA $0200,X" / "BNE loop" (ca65 syntax)
//     assemble(line, pc, labels)          the bytes of one ca65 instruction line
//
// Addressing modes, and their operand size:
//
//     Imp  (none)        Acc  A            Imm  #$nn        Zp   $nn         Zpx  $nn,X
//     Zpy  $nn,Y         Abs  $nnnn        Abx  $nnnn,X     Aby  $nnnn,Y     Ind  ($nnnn)
//     Izx  ($nn,X)       Izy  ($nn),Y      Rel  a branch: one signed byte, from the next instruction
#pragma once

#include <array>
#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>

#include "cb/core/bytes.hpp"

namespace cb::decompile::m6502 {

enum class Mode : std::uint8_t { Imp, Acc, Imm, Zp, Zpx, Zpy, Abs, Abx, Aby, Ind, Izx, Izy, Rel };

// The instruction's length in bytes (opcode + operand) in this mode.
constexpr std::uint8_t instruction_size(Mode mode) {
    switch (mode) {
        case Mode::Imp:
        case Mode::Acc: return 1;
        case Mode::Abs:
        case Mode::Abx:
        case Mode::Aby:
        case Mode::Ind: return 3;
        default: return 2;
    }
}

// The mode's three-letter name as the documentation spells it ("zpx").
std::string_view mode_name(Mode mode);

struct OpcodeEntry {
    std::string_view mnemonic;   // upper case, static storage
    Mode mode;
};

// The 256-entry table: nullopt for an undocumented opcode.
const std::array<std::optional<OpcodeEntry>, 256>& opcode_table();

// The opcode of `mnemonic` (upper case) in `mode`; nullopt when that form does not exist.
std::optional<std::uint8_t> opcode_of(std::string_view mnemonic, Mode mode);
bool is_mnemonic(std::string_view mnemonic);
bool is_branch(std::string_view mnemonic);   // BCC BCS BEQ BMI BNE BPL BVC BVS
bool ends_flow(std::string_view mnemonic);   // JMP RTS RTI BRK: flow does not fall through

// One decoded instruction.
struct Insn {
    std::uint32_t at = 0;                       // the address it sits at
    std::uint8_t op = 0;
    std::string_view mnemonic;
    Mode mode = Mode::Imp;
    std::optional<std::uint16_t> operand;       // none for Imp / Acc
    std::uint8_t size = 0;
    std::optional<std::uint16_t> target;        // a branch, JMP abs or JSR: where it goes
};

// The instruction at code[offset], which sits at address `pc`; nullopt for an undocumented opcode or one cut
// off by the end of `code`.
std::optional<Insn> decode(ByteView code, std::size_t offset, std::uint32_t pc);

// Label name by address; an empty name counts as no label. Lookup only (never iterated).
using AddressLabels = std::unordered_map<std::uint16_t, std::string>;

struct NameHash {
    using is_transparent = void;
    std::size_t operator()(std::string_view s) const { return std::hash<std::string_view>()(s); }
};
// Address by label name, for the assembler. A value is whatever the caller's pass gives (the first pass of a
// two-pass assembler uses 0xFFFF for labels not yet seen), so it is signed and wide.
using NameLabels = std::unordered_map<std::string, std::int64_t, NameHash, std::equal_to<>>;

// The instruction in ca65 syntax; an address that has a label is written as the label. (A zero-page address is
// written with two hex digits, an absolute one with four: ca65 picks the instruction's size from how the
// address is written, so the bytes come back the same.)
std::string text(const Insn& insn, const AddressLabels& labels = {});

// The bytes of one instruction written as text() writes it. Labels are resolved from `labels`; `a:` forces an
// absolute address. Throws ParseError when the line is not an instruction, names an unknown mnemonic, label or
// number, uses a form the mnemonic lacks, or branches out of reach.
Bytes assemble(std::string_view line, std::uint32_t pc, const NameLabels& labels = {});

}  // namespace cb::decompile::m6502
