// decompile/tlcs900.hpp -- the Toshiba TLCS-900/H's instructions (the Neo Geo Pocket's CPU, the TMP95C061): one table that the disassembler, the assembler and the
// interpreter (emulate/tlcs900) all read.
//
//     decode(code, at, pc)         the instruction at code[at:] running at address pc, or nullopt
//     text(insn, labels)           "ld XWA,0x00200100"  "add A,(XBC+4)"  "ldir (XDE+),(XHL+)"  "jr NZ,L_0420"  "call sub_0450"  "ldw (0x6C00),0x1234"
//     assemble(line, pc, labels)   the bytes of one instruction line
//
// The encoding is prefix based. The first byte says what the second byte (the operation) works on:
//
//     $00-$1F  one operation by itself (nop, ret, push A, jp #24, call #16, swi is $F8-$FF ...)     $20-$7F  register forms (ld r,#; push rr; jr cc; jrl cc ...)
//     $80-$BF  memory through an extended register (XRR) or XRR + d8, of a size (byte $80, word $90, long $A0) or as a destination ($B0)
//     $C0-$C5  memory by an absolute 8 / 16 / 24 bit address, an extended mode ($C3: XRR, XRR + d16, XRR + r8, XRR + r16, PC + d16) or -XRR / XRR+ ($C4 / $C5); the word ($D0)
//              and long ($E0) forms, and $F0 for a destination
//     $C7-$EF  a register: $C8+r byte W A B C D E H L, $D8+r word WA BC DE HL IX IY IZ SP, $E8+r long XWA ... XSP, or $C7 / $D7 / $E7 and a register code byte naming any part
//              of any bank (QA, IXL, RA3, XWA3, XBC' ...); $F7 is ldx and $F8-$FF swi
//
// and the byte after the operand (there are four tables, "register", "source memory", "destination memory" and the single bytes) holds the operation. Each row of the table says
// which first-byte class and which operand sizes it takes, the operation byte's pattern, the operation and the operand kinds in the order of the text: so a row is read forward
// to decode and backward to assemble. An instruction that has no register to carry its size gets a `w` on a word operation (`ldw`, `incw`, `pushw`, `ldiw`).
//
// The text is the Toshiba assembler's, with these choices: registers are `W A B C D E H L`, `WA BC DE HL IX IY IZ SP`, `XWA ... XSP`, the parts `QA ... QIZ`, `IXL IXH ...`, another
// bank's `RA3 RWA3 XWA3` and the previous bank's `A' WA' XWA'`; a memory operand is `(XWA)` `(XWA+8)` `(XWA-8)` `(XIX+A)` `(XIX+WA)` `(-XSP)` `(XIX+)` `(0x20)` `(0x6C00)` `(0x200100)`
// `(pc:0x200123)` (the absolute address a PC-relative operand reaches); an absolute address is written with 2, 4 or 6 hex digits as it is encoded; a decrement or increment by
// other than the operand's size is `(-XSP/2)`; numbers are `0x..` (the width of their size) except the small ones packed into the operation byte (`inc 3,A`, `ld WA,5`, `swi 1`),
// which are decimal; branch targets are absolute. `di` is `ei 7`.
//
// An instruction can have more than one encoding (`ld A,(XWA)` as $80 $21 or as $C3 $E0 $21; an absolute address in a longer form); the text is the same, assemble gives the
// shortest, and a caller that must keep the bytes writes them as data when the text does not assemble back to them.
#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "core/bytes.hpp"
#include "core/error.hpp"
#include "decompile/labels.hpp"

namespace cb::decompile::tlcs900 {

enum class Op : std::uint8_t {
    Nop, Push, Pop, Ei, Halt, Reti, Ld, Incf, Decf, Ret, Retd, Rcf, Scf, Ccf, Zcf, Ex, Ldf, Jp, Call, Calr, Jr, Jrl, Swi, Lda, Ldx, Link, Unlk, Cpl, Neg, Mul, Muls, Div, Divs, Daa,
    Extz, Exts, Paa, Mirr, Mula, Djnz, Andcf, Orcf, Xorcf, Ldcf, Stcf, Ldc, Res, Set, Chg, Bit, Tset, Minc1, Minc2, Minc4, Mdec1, Mdec2, Mdec4, Inc, Dec, Scc, Add, Adc, Sub, Sbc,
    And, Xor, Or, Cp, Rlc, Rrc, Rl, Rr, Sla, Sra, Sll, Srl, Rld, Rrd, Ldi, Ldir, Ldd, Lddr, Cpi, Cpir, Cpd, Cpdr, Bs1f, Bs1b
};

// What the first byte makes of the second.
enum class Ctx : std::uint8_t { Fixed, Reg, Src, Dst };

// The kinds of operand, in the order the text writes them.
enum class Tok : std::uint8_t {
    None,
    M,      // the operand the first byte chose: a register (Reg), a memory location (Src, Dst)
    Mw,     // the first byte's register, written as the register of twice the size (the destination of mul / div by an immediate)
    Rz,     // the register the operation byte's low 3 bits name, of the operand's size
    R8, R16, R32,   // likewise, of a size of its own
    Rmul,   // the low 3 bits as a mul / div destination: a word register for a byte operation (odd codes only), a long register for a word operation
    A,      // the byte accumulator A
    Acc,    // A for a byte operation, WA for a word operation
    Ib, Iw, Il, Iz,   // an immediate after the operation byte: byte, word, long, the operand's size
    L3,     // the low 3 bits as a number 0..7
    N3,     // the low 3 bits as a count 1..8 (0 is 8)
    Bit4,   // a byte after the operation: a bit number (low 4 bits)
    Cnt4,   // a byte after the operation: a shift count 1..16 (low 4 bits, 0 is 16)
    Cc,     // the low 4 bits as a condition
    Cr,     // a control register byte (DMAS0 ...)
    Rel8, Rel16,    // a branch displacement from the end of the instruction (written as the target)
    D16,    // a signed 16-bit number (retd, link)
    T16, T24,       // an absolute jump or call target
    A8, A16,        // an absolute address in parentheses: (n) (nn)
    BlkD, BlkS,     // the pointer operands of a block transfer / compare, written (XDE+) (XHL+)
    Sr, F, Fp       // SR, F, F'
};

inline constexpr std::uint8_t kSizeB = 1, kSizeW = 2, kSizeL = 4;
inline constexpr std::uint8_t kBlockInc = 1, kBlockDec = 2, kOptCc = 4, kBare = 8, kPad = 16;   // Row::flags

struct Row {
    Ctx ctx;
    std::uint8_t sizes;   // the operand sizes (kSizeB | ...) the row takes: the first byte's size for Reg and Src, the data's width for Dst, 0 for Fixed
    std::uint8_t op;      // the operation byte's value ...
    std::uint8_t mask;    // ... where the bits in `mask` are compared (the first byte for Fixed)
    Op id;
    Tok a, b;
    std::uint8_t flags = 0;   // kBlockInc / kBlockDec (a block operation's direction), kOptCc (cc omitted when T), kBare (no memory operand written: RET cc), kPad (ldx's zero bytes)
};

const std::vector<Row>& rows();
std::string_view op_name(Op id);

// How control leaves an instruction.
enum class Flow : std::uint8_t { None, Branch, Jump, Call, Return, Indirect, Halt };

// What names a memory operand.
enum class MemKind : std::uint8_t { Reg, RegD8, Abs8, Abs16, Abs24, RegD16, RegIdx8, RegIdx16, PcRel, PreDec, PostInc };

struct Mem {
    MemKind kind = MemKind::Reg;
    std::uint8_t reg = 0;     // the base register's code (see rcode_name)
    std::uint8_t idx = 0;     // the index register's code (RegIdx8: a byte register, RegIdx16: a word register)
    std::uint8_t step = 0;    // PreDec / PostInc: 1, 2 or 4
    std::int32_t disp = 0;    // RegD8, RegD16
    std::uint32_t addr = 0;   // Abs*: the address; PcRel: the address it reaches
    bool is_short = false;    // written with the one-byte-prefix form ($80+r, $88+r)
};

struct Insn {
    std::uint32_t at = 0;
    std::uint8_t size = 0;
    const Row* row = nullptr;
    std::uint8_t first = 0;               // the first byte
    std::uint8_t op = 0;                  // the operation byte (the first byte for Fixed)
    std::uint8_t z = 0;                   // the operand size in bytes (1, 2, 4), 0 for none
    std::uint8_t reg = 0;                 // Ctx::Reg: the register's code
    Mem m;                                // Ctx::Src, Ctx::Dst: the memory operand
    std::array<std::uint32_t, 2> v{};     // the values of the two tokens that follow the operation (an immediate, an address, a target, a count)
    std::optional<std::uint32_t> target;  // a direct jump, call or branch: where it goes
    Flow flow = Flow::None;
    std::array<std::uint8_t, 12> raw{};
    ByteView bytes() const { return {raw.data(), size}; }
    Op id() const { return row->id; }
};

// The instruction at code[offset], which sits at address `pc`; nullopt for a pattern the table does not define or one cut off by the end of `code`.
std::optional<Insn> decode(ByteView code, std::size_t offset, std::uint32_t pc);

// A register code's name at a size (1, 2 or 4 bytes), nullopt when the code names no register of that size; and back.
std::optional<std::string> rcode_name(std::uint8_t code, unsigned size);
struct RegName {
    std::uint8_t code;
    unsigned size;
};
std::optional<RegName> register_by_name(std::string_view name);
// The condition names F LT LE ULE OV MI Z C T GE GT UGT NOV PL NZ NC (index = the condition code).
extern const std::array<std::string_view, 16> kConditions;
// A control register's name (DMAS0 ...), or "CR_0x34".
std::string control_register_name(unsigned code);

}  // namespace cb::decompile::tlcs900
