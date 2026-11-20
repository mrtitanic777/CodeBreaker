// decompile/f8.hpp -- the Fairchild F8's instructions (the Channel F's CPU, the 3850): the table, a disassembler, an assembler.
//
//     decode(code, at, pc)         the instruction at code[at:] running at address pc, or nullopt
//     text(insn, labels)           "LI $12"  "LR A,R3"  "LR S,A"  "LIS 5"  "BT 4,L_0820"  "PI sub_0900"  "DCI $2800"  "OUT $01"
//     assemble(line, pc, labels)   the bytes of one instruction line
//
// One to three bytes. The accumulator is A; the 64-byte scratchpad is reached through the registers R0..R11 (directly), S, I and D (the register the indirect scratchpad register
// ISAR points at, then counted up or down in its low three bits after the access: codes 12, 13 and 14). The scratchpad's named pairs are written as the byte registers `KU KL QU QL`
// and the pairs `K Q H` (HU / HL, J, W (the status register), IS (the ISAR), DC (the data counter), P0 / P (the program counters)). Branches take a signed byte counted from the
// displacement byte's own address: `BR target`, `BT n,target` (branch when one of the flags in n is set: 1 sign (positive), 2 carry, 4 zero), `BF n,target` (branch when none of the flags in
// n is set; 8 is overflow), `BR7 target` (branch when ISAR's low three bits are not 7). `PI` and `JMP` take a 16-bit address, high byte first. Numbers are `$1F`, `0x1F`, `%101` or decimal.
// R15 does not exist: the opcodes that would name it are undefined (246 opcodes of 256 are defined).
#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>

#include "core/bytes.hpp"
#include "core/error.hpp"
#include "decompile/labels.hpp"

namespace cb::decompile::f8 {

// What follows the opcode: nothing; a byte (immediate, port or branch displacement); an address or constant of 16 bits; a value in the opcode's low three or four bits.
enum class Kind : std::uint8_t { None, Imm8, Port8, Rel8, Addr16, Imm16, Low3, Low4 };

enum class Flow : std::uint8_t { None, Branch, Jump, Call, Return, Indirect };

struct OpcodeEntry {
    std::string text;   // the mnemonic and operands up to the operand ("LI ", "BT 4,", "LR A,R3", "LIS ")
    Kind kind = Kind::None;
};

const std::array<std::optional<OpcodeEntry>, 256>& opcode_table();

struct Insn {
    std::uint32_t at = 0;
    std::uint8_t size = 0;
    std::uint8_t op = 0;
    std::uint32_t value = 0;                    // the byte, the 16-bit operand, or the low-bits value
    const OpcodeEntry* entry = nullptr;
    std::optional<std::uint32_t> target;        // a branch, jump or call: where it goes
    Flow flow = Flow::None;
    std::array<std::uint8_t, 3> raw{};
    ByteView bytes() const { return {raw.data(), size}; }
};

// The instruction at code[offset], which sits at address `pc`; nullopt for an undefined opcode or one cut off by the end of `code`.
std::optional<Insn> decode(ByteView code, std::size_t offset, std::uint32_t pc);

using decompile::AddressLabels;
using decompile::NameLabels;

// The instruction as source; a branch, jump or call target that has a label is written as the label (and a DCI's constant, with `label_constant`).
std::string text(const Insn& insn, const AddressLabels& labels = {}, bool label_constant = false);

// The bytes of one instruction line. With `lenient`, a name that is not known counts as 0 and out-of-range values are not refused. Throws ParseError for a line that is not an
// instruction, an unknown name or a value out of range.
Bytes assemble(std::string_view line, std::uint32_t pc, const NameLabels& labels = {}, bool lenient = false);

}  // namespace cb::decompile::f8
