// cb/decompile/v810.hpp -- the NEC V810's instructions: the table, a disassembler, an assembler.
//
// The V810 is the Virtual Boy's (and the PC-FX's) 32-bit RISC: 32 registers (r0 is zero), 16- and 32-bit instructions in little-endian halfwords. One table serves the
// disassembler, the assembler that rebuilds the source the Virtual Boy lane writes (source/vb_writer) and the interpreter (emulate/v810).
//
//     decode(code, at, pc)         the instruction at code[at:] running at address pc, or nullopt
//     text(insn, labels)           "mov r1,r2"  "movea -4,r3,r3"  "ld.w 8[r5],r6"  "bne L_0040"  "jmp [r31]"
//     assemble(line, pc, labels)   the bytes of one instruction line
//
// The formats (the first halfword is  opcode[15:10] reg2[9:5] reg1[4:0]):
//
//     I    reg1,reg2                 mov add sub cmp shl shr sar mul div mulu divu or and xor not      jmp [reg1]
//     II   imm5,reg2                 mov add cmp (imm5 signed -16..15)  shl shr sar (0..31)  setf (a condition 0..15)  ldsr reg2,sysreg  stsr sysreg,reg2   trap imm5
//          (no operands)             cli sei reti halt, and the ten bit-string instructions (sch0bsu ... notbsu) which work on r26-r30
//     III  b<cond> target            a 9-bit signed even displacement in the first halfword's low bits (bits 15-13 are 100, the condition is bits 12-9)
//     IV   jr / jal target           a 26-bit signed even displacement split over the two halfwords
//     V    movea addi imm16,reg1,reg2   (imm16 signed)   ori andi xori movhi imm16,reg1,reg2   (imm16 unsigned)
//     VI   ld.b ld.h ld.w in.b in.h in.w caxi  disp[reg1],reg2      st.b st.h st.w out.b out.h out.w  reg2,disp[reg1]
//     VII  opcode $3E and a sub-opcode in the second halfword's top six bits: the floating-point instructions addf.s subf.s mulf.s divf.s cmpf.s cvt.ws cvt.sw trnc.sw, and
//          xb xh rev mpyhw
//
// Registers are r0..r31 (the aliases hp sp gp tp lp for r2 r3 r4 r5 r31 are read, never written). An immediate is a number, a label, `hi(x)` ((x + $8000) >> 16, what movhi wants
// before a signed low half) or `lo(x)` (x & $FFFF). A bit pattern the table does not define, one with a field the instruction ignores set (the second register of jmp, the
// reserved bits of the extended instructions) or an odd branch displacement is not an instruction here: it has no text that assembles back to it.
#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>

#include "cb/core/bytes.hpp"
#include "cb/core/error.hpp"
#include "cb/decompile/labels.hpp"

namespace cb::decompile::v810 {

enum class Mode : std::uint8_t { None, R1R2, R2, Jmp, S5R2, U5R2, Trap, LdSr, StSr, Branch, Jump26, Imm16S, Imm16U, Load, Store };

// How control leaves an instruction.
enum class Flow : std::uint8_t { None, Branch, Jump, Call, Return, Indirect, Halt };

struct OpcodeEntry {
    std::string_view mnemonic;   // lower case, static storage
    Mode mode;
};

// What an instruction is, for the interpreter: its 6-bit opcode, 0x100 + the sub-opcode for a bit-string instruction, 0x200 + the sub-opcode for an extended one.
namespace id {
inline constexpr unsigned kMov = 0x00, kAdd = 0x01, kSub = 0x02, kCmp = 0x03, kShl = 0x04, kShr = 0x05, kJmp = 0x06, kSar = 0x07, kMul = 0x08, kDiv = 0x09, kMulu = 0x0A, kDivu = 0x0B,
                          kOr = 0x0C, kAnd = 0x0D, kXor = 0x0E, kNot = 0x0F;
inline constexpr unsigned kMovI = 0x10, kAddI5 = 0x11, kSetf = 0x12, kCmpI5 = 0x13, kShlI5 = 0x14, kShrI5 = 0x15, kCli = 0x16, kSarI5 = 0x17, kTrap = 0x18, kReti = 0x19, kHalt = 0x1A,
                          kLdsr = 0x1C, kStsr = 0x1D, kSei = 0x1E, kBitString = 0x1F;
inline constexpr unsigned kBranch = 0x20;   // 0x20..0x27 are all the branches; the condition is Insn::cond
inline constexpr unsigned kMovea = 0x28, kAddi = 0x29, kJr = 0x2A, kJal = 0x2B, kOri = 0x2C, kAndi = 0x2D, kXori = 0x2E, kMovhi = 0x2F;
inline constexpr unsigned kLdB = 0x30, kLdH = 0x31, kLdW = 0x33, kStB = 0x34, kStH = 0x35, kStW = 0x37, kInB = 0x38, kInH = 0x39, kCaxi = 0x3A, kInW = 0x3B, kOutB = 0x3C, kOutH = 0x3D,
                          kExtended = 0x3E, kOutW = 0x3F;
inline constexpr unsigned kBitStringBase = 0x100, kExtendedBase = 0x200;
inline constexpr unsigned kCmpfS = 0x200, kCvtWs = 0x202, kCvtSw = 0x203, kAddfS = 0x204, kSubfS = 0x205, kMulfS = 0x206, kDivfS = 0x207, kXb = 0x208, kXh = 0x209, kRev = 0x20A,
                          kTrncSw = 0x20B, kMpyhw = 0x20C;
}  // namespace id

// The 64-entry table of the major opcodes (nullopt: undefined; the branches 0x20-0x27 are not in it), the extended instructions by sub-opcode, and the bit-string instructions
// by their 5-bit code.
const std::array<std::optional<OpcodeEntry>, 64>& opcode_table();
const std::array<std::optional<OpcodeEntry>, 64>& extended_table();
const std::array<std::optional<OpcodeEntry>, 32>& bitstring_table();

// The 16 conditions of a branch, in their order: v l e nh n r lt le nv nl ne h p (never) ge gt. A branch is written b + the name.
inline constexpr std::array<std::string_view, 16> kConditions = {"v", "l", "e", "nh", "n", "r", "lt", "le", "nv", "nl", "ne", "h", "p", "nop", "ge", "gt"};

// The system registers that have names (eipc eipsw fepc fepsw ecr psw pir tkcw chcw adtre); others are written as numbers.
std::string_view system_register_name(unsigned n);

bool is_mnemonic(std::string_view mnemonic);

struct Insn {
    std::uint32_t at = 0;                       // the address it sits at
    std::uint8_t size = 0;                      // 2 or 4
    unsigned id = 0;                            // see namespace id
    std::string_view mnemonic;
    Mode mode = Mode::None;
    std::uint8_t r1 = 0, r2 = 0;                // the register fields
    std::uint8_t cond = 0;                      // a branch's condition
    std::int32_t imm = 0;                       // the immediate / displacement / system register / condition code, signed or not as its mode says
    std::optional<std::uint32_t> target;        // a branch, jr or jal: where it goes (also the never-taken branch)
    Flow flow = Flow::None;
    std::array<std::uint8_t, 4> raw{};
    ByteView bytes() const { return {raw.data(), size}; }
};

// The instruction at code[offset], which sits at address `pc`; nullopt for an undefined pattern or one cut off by the end of `code`.
std::optional<Insn> decode(ByteView code, std::size_t offset, std::uint32_t pc);

using decompile::AddressLabels;
using decompile::NameLabels;

// The instruction as source; a branch target that has a label is written as the label. `immediate`, when not empty, is written for the instruction's immediate or
// displacement (the writer's `hi(label)` / `lo(label)`).
std::string text(const Insn& insn, const AddressLabels& labels = {}, std::string_view immediate = {});

// The bytes of one instruction line. Labels are resolved from `labels`; with `lenient`, a name that is not there counts as 0 and out-of-range values are not refused (the first
// pass of an assembler needs only the size). Throws ParseError for a line that is not an instruction, an unknown name or a value out of range.
Bytes assemble(std::string_view line, std::uint32_t pc, const NameLabels& labels = {}, bool lenient = false);

// A numeric expression as the assembler reads it: decimal, 0xhex, labels, `hi(x)`, `lo(x)`, `+` and `-`.
std::int64_t value(std::string_view expr, const NameLabels& labels, bool lenient = false);

}  // namespace cb::decompile::v810
