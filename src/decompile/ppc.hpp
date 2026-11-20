// decompile/ppc.hpp -- the PowerPC 750 family's instructions: the Nintendo GameCube's Gekko, the Wii's Broadway and the Wii U's
// Espresso. A disassembler and an assembler, big-endian, one table read both ways.
//
//     decode(code, at, pc)         the instruction at code[at:] running at address pc, or nullopt
//     text(insn, labels)           "stwu r1,-32(r1)" / "bne cr7,L_80003120" / "ps_madd f1,f2,f3,f4"
//     assemble(line, pc, labels)   the four big-endian bytes of one instruction line
//
// What is decoded is the 32-bit PowerPC user and supervisor instruction set of the 750 (integer, branch, condition register,
// load/store, floating point, the cache and system instructions) and what Gekko added to it: the paired-single unit
// (`ps_add`, `ps_madd`, `ps_merge00` ... with `psq_l`, `psq_st` and their indexed and update forms, `dcbz_l`). Broadway and
// Espresso run the same instructions, so one table serves the three consoles; what differs between them is the machine around
// the CPU (the writer and the lane keep that). A word with a field the instruction does not use set (a nonzero reserved bit) is
// not an instruction, so data is not read as code and whatever is written as an instruction assembles back to the same word.
//
// Syntax is GNU as's (`powerpc-eabi-as -mregnames`, the assembler devkitPPC ships), with the simplified mnemonics
// the disassembler writes: `nop li lis mr not mflr mtlr mfctr mtctr mfxer mtxer mftb mftbu blr bctr`, `cmpwi cmplwi cmpw cmplw` (with
// the condition register field written only when it is not cr0), and the conditional branches `beq bne blt bge bgt ble bso bns bdnz bdz`
// with their `lr` and `ctr` forms (`beqlr`, `bnectr`). The assembler reads those and the general forms (`bc BO,BI,target`).
//
//     lis   r3,msg@ha               sym@ha is (x + 0x8000) >> 16, so it pairs with an addi or a load or store's offset
//     addi  r3,r3,msg@l             sym@l is x & 0xFFFF
//     bl    sub_80003200            a branch's displacement is made from its target
//     lwz   r0,20(r1)               registers r0-r31, floating registers f0-f31, condition register fields cr0-cr7
//     psq_l f1,8(r3),0,0            paired-single loads and stores: offset(base), then W and I
//
// Expressions are numbers (decimal, 0x hex), labels, `@ha` `@l` `@h`, `+ - * / & | ^ ~ << >>` and parentheses.
#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>

#include "core/bytes.hpp"
#include "core/error.hpp"
#include "decompile/labels.hpp"
#include "decompile/z80.hpp"

namespace cb::decompile::ppc {

// How control leaves an instruction (the Z80 module's: None, Jump, Branch, Call, Return, Indirect, Halt).
using Flow = z80::Flow;

// One decoded instruction.
struct Insn {
    std::uint32_t word = 0;
    std::uint32_t pc = 0;                  // the address it runs at
    std::string mnemonic;                  // as written: "addi", "li", "add.", "bl", "beqlr", "ps_madd" ...
    std::optional<std::uint32_t> target;   // where a branch or call goes (not for the register branches)
    Flow flow = Flow::None;
    std::uint16_t row = 0;                 // which row of the table it is (for the assembler's tests)

    // The word's primary opcode (bits 0-5 in IBM numbering) and the fields the writers look at.
    unsigned opcode() const { return word >> 26; }
    unsigned rt() const { return (word >> 21) & 31; }   // rD / rS / frD / frS / BO
    unsigned ra() const { return (word >> 16) & 31; }
    unsigned rb() const { return (word >> 11) & 31; }
    std::int32_t simm() const { return static_cast<std::int16_t>(word & 0xFFFF); }
    std::uint32_t uimm() const { return word & 0xFFFF; }
};

// The instruction at code[at:], which runs at address `pc`, or nullopt when the bytes are not one (or are fewer than four).
// `wide`: also the 64-bit instructions of the Cell's PPU (the 750 family has none: those words are not instructions for it).
std::optional<Insn> decode(ByteView code, std::size_t at, std::uint32_t pc, bool wide = false);
// The same from the word.
std::optional<Insn> decode_word(std::uint32_t word, std::uint32_t pc, bool wide = false);

// The instruction as source. `labels` names branch targets; an empty name counts as no label.
std::string text(const Insn& insn, const AddressLabels& labels = {});
// The instruction with its 16-bit immediate or displacement written as `immediate` ("msg@ha", "msg@l", "msg + 4"), for an
// instruction that has one (addi addis li lis ori the loads and stores ...); any other is written as text() does.
std::string text(const Insn& insn, const AddressLabels& labels, std::string_view immediate);

// Does the instruction have a 16-bit immediate or displacement that text(insn, labels, immediate) can replace?
bool has_immediate(const Insn& insn);

// A numeric expression as the assembler reads it. ParseError for a malformed one, z80::UnknownLabel for a label not in `labels`.
std::int64_t value(std::string_view expr, const NameLabels& labels, std::uint32_t pc = 0);

// The big-endian bytes of one instruction line. `labels` resolves names; `pc` is where the instruction will be. ParseError for text
// that is not an instruction or one whose operand is out of reach (a branch too far, an immediate that does not fit its field);
// z80::UnknownLabel for a label that is not there.
Bytes assemble(std::string_view line, std::uint32_t pc = 0, const NameLabels& labels = {}, bool wide = false);

// The number of instruction rows in the table (tests walk them).
std::size_t row_count();
// The row's name, pattern and the bits that are not fixed (tests build a word from them).
std::string_view row_name(std::size_t row);
std::uint32_t row_pattern(std::size_t row);
std::uint32_t row_variable_bits(std::size_t row);

}  // namespace cb::decompile::ppc
