// cb/decompile/cp1610.hpp -- the General Instrument CP1610's instructions (the Intellivision's CPU): a disassembler and an assembler over 10-bit "decles".
//
// The CP1610 has eight 16-bit registers (R6 is the stack pointer, R7 the program counter) and a word-addressed memory; an instruction is one to three decles of 10 bits
// (held in 16-bit words here; only the low ten bits of a word are read, as a ROM does). One table serves the disassembler, the assembler that rebuilds the source the Intellivision lane writes
// (source/intv_writer) and the interpreter (emulate/cp1610).
//
//     decode(code, at, pc, sdbd)   the instruction at code[at:] running at word address pc, or nullopt;   `sdbd`: the previous instruction was SDBD
//     text(insn, labels)           "MVII #$1234,R0"  "MVI@ R4,R1"  "ADDR R1,R2"  "BNEQ L_0040"  "JSR R5,sub_5100"
//     assemble(line, pc, ...)      the decles of one instruction line
//
// Syntax (as1600's): a register is R0..R7. Memory operands: `MVI $0100,R1` (direct, an address below $400: the decle holds 10 bits), `MVI@ R2,R1` (indirect through R1..R6;
// R4 and R5 count up after the access, R6 counts down before a read and up after a write: the stack), `MVII #5,R1` (immediate: the same for ADDI SUBI CMPI ANDI XORI), `MVO R1,$0100`,
// `MVO@ R1,R4`. An immediate or indirect access after `SDBD` is double-byte: the immediate is two decles of eight bits (low byte first), an @R4 or @R5 read takes two words. Jumps:
// `J addr` `JE addr` `JD addr` (and with a link register `JSR R5,addr` `JSRE` `JSRD`: the return address goes in R4, R5 or R6), branches `B BC BOV BPL BEQ BLT BLE BUSC NOPP
// BNC BNOV BMI BNEQ BGE BGT BESC target` and `BEXT n,target`. `NOP` and `SIN` may take a 1 (the second encoding). Shifts name R0..R3 and may give a count of 2: `SLL R1,2`.
#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/error.hpp"
#include "cb/decompile/labels.hpp"

namespace cb::decompile::cp1610 {

enum class Op : std::uint8_t {
    Hlt, Sdbd, Eis, Dis, Tci, Clrc, Setc, Nop, Sin,
    Incr, Decr, Comr, Negr, Adcr, Gswd, Rswd,
    Swap, Sll, Rlc, Sllc, Slr, Sar, Rrc, Sarc,
    Movr, Addr, Subr, Cmpr, Andr, Xorr,
    Branch, Bext, Jump, Mvo, Mvi, Add, Sub, Cmp, And, Xor,
};

// How control leaves an instruction.
enum class Flow : std::uint8_t { None, Branch, Jump, Call, Return, Indirect, Halt };

// How the memory instructions (Mvo ... Xor) name their operand.
enum class Access : std::uint8_t { None, Direct, Indirect, Immediate };

struct Insn {
    std::uint32_t at = 0;                       // the word address it sits at
    std::uint8_t size = 0;                      // decles: 1 to 3
    Op op = Op::Hlt;
    std::array<std::uint16_t, 3> words{};       // the decles
    std::uint8_t r1 = 0;                        // the first register (a source; the pointer of an indirect access is `r1`)
    std::uint8_t r2 = 0;                        // the second register (a destination; of Mvo: the source)
    std::uint8_t cond = 0;                      // a branch's condition, a Jump's link register (4, 5, 6; 7: none), a NOP / SIN's 1, a shift's count
    std::uint8_t flag = 0;                      // a Jump's interrupt flag: 0 none, 1 enable (E), 2 disable (D)
    Access access = Access::None;
    bool backward = false;                      // a branch's direction bit
    bool sdbd = false;                          // double-byte: the immediate is two decles
    std::uint32_t value = 0;                    // an immediate, a direct address, a jump's address, a branch's displacement
    std::optional<std::uint32_t> target;        // a branch or jump: where it goes
    Flow flow = Flow::None;
};

// The instruction at code[offset], which sits at word address `pc`; nullopt for a pattern that is not an instruction, an SDBD immediate decle above eight bits or a pattern cut off by
// the end of `code`. A word with bits above the tenth set decodes as its low ten bits (its `words` keep them, so the text does not assemble back to it).
std::optional<Insn> decode(std::span<const std::uint16_t> code, std::size_t offset, std::uint32_t pc, bool sdbd = false);

using decompile::AddressLabels;
using decompile::NameLabels;

// The mnemonic of an instruction as written ("MVI@", "MVII", "BNEQ", "JSR" ...).
std::string mnemonic(const Insn& insn);

// The instruction as source; an address that has a label is written as the label (jump and branch targets, direct addresses, and with `label_immediates` an immediate).
std::string text(const Insn& insn, const AddressLabels& labels = {}, bool label_immediates = false);

// The decles of one instruction line. `sdbd`: the previous line was SDBD. With `lenient`, a name that is not known counts as 0 and out-of-range values are not refused (the
// first pass needs only the size). Throws ParseError for a line that is not an instruction, an unknown name or a value out of range.
std::vector<std::uint16_t> assemble(std::string_view line, std::uint32_t pc, const NameLabels& labels = {}, bool lenient = false, bool sdbd = false);

// A numeric expression as the assembler reads it: `$hex`, `0xhex`, decimal, labels, `+` and `-`.
std::int64_t value(std::string_view expr, const NameLabels& labels, bool lenient = false);

}  // namespace cb::decompile::cp1610
