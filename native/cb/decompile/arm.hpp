// cb/decompile/arm.hpp -- the ARM7TDMI's instructions (ARMv4T: the ARM state and the Thumb state, as the Game Boy Advance's CPU has them):
// a disassembler and an assembler.
//
//     decode(code, at, pc)         the instruction at code[at:] running at address pc, or nullopt
//     text(insn, labels)           "ldr r0, [r1, #4]" / "bne L_08000040" / "push {r4, lr}"
//     assemble(line, pc, labels)   the bytes (little-endian) of one instruction line
//
// The ARMv5TE additions the Nintendo DS's ARM946E-S has (`Arch::V5TE`): BLX (immediate and register, in both states), CLZ, QADD QSUB QDADD QDSUB,
// the 16-bit multiplies SMLAxy SMLAWy SMULWy SMLALxy SMULxy, LDRD and STRD, PLD, BKPT, and MCR and MRC (the coprocessor 15 the DS has: caches, TCMs).
// They are instructions only when the decoder or the assembler is asked for `Arch::V5TE`: with the default `Arch::V4T` (the Game Boy Advance's
// ARM7TDMI, and the DS's second processor) their bits are not instructions, as before.
//
// An address carries the state it is executed in in its lowest bit, as the processor's own interworking does: `pc` and every
// `target` with bit 0 set are Thumb (2-byte instructions), bit 0 clear is ARM (4-byte instructions). `Insn::pc` is the real address.
//
// Every instruction of ARMv4T: the ARM state's data processing (with the shifter operand: immediate, shifted register, register-
// shifted register), MUL MLA UMULL UMLAL SMULL SMLAL, LDR STR LDRB STRB LDRT STRT, LDRH STRH LDRSB LDRSH, LDM STM (all four modes,
// writeback, ^), SWP SWPB, B BL BX, SWI, MRS MSR; and the Thumb state's nineteen formats. A word with a field the instruction does not
// use set (a nonzero Rn in MOV), an encoding that is undefined on the ARM7TDMI (LDRD, BLX, a Thumb `mov` between two low registers) and
// the coprocessor instructions (the GBA has none) are not instructions, so data is not read as code, and what is written as an
// instruction assembles back to the same bits.
//
// Syntax is GNU as's `.syntax unified` (the assembler of devkitARM and the other GBA toolchains: arm-none-eabi-as), written the way its
// output is read:
//
//     add   r0, r1, r2, lsl #2      a shifted register; `ands`, `addseq` ... the S and the condition after the mnemonic
//     mov   r0, #0x04000000         an immediate must be an 8-bit value rotated by an even amount; `#255, 30` names the rotate
//     ldr   r0, [r1, #-4]!          pre-indexed with writeback; `[r1], #4` post-indexed; `[r1, r2, lsl #2]` a scaled register
//     ldr   r0, L_08000100          a PC-relative load of the word at a label (the literal pool), as `ldr r0, [pc, #n]`
//     push  {r4, lr}                STMDB sp!; `pop` is LDMIA sp!; `ldmia r0!, {r1-r3}`, `stmdb`, `ldmfd` and the like are accepted
//     bl    sub_08000100            a branch's offset is made from its target; `bne`, `bleq`, `bx lr`
//     lsls  r0, r1, #2              (Thumb) every data-processing instruction that sets flags says so with the `s`
//
// Registers: r0-r12 sp lr pc (also ip fp sl sb). Expressions are numbers (decimal, 0x hex, 0b binary), labels, `.` (here), and
// `+ - * / & | ^ ~ << >>` with parentheses. Not written: literal pools (`ldr r0, =value`), `adr`, IT blocks (Thumb-2), coprocessor and
// Thumb-2 instructions.
#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>

#include "cb/core/bytes.hpp"
#include "cb/core/error.hpp"
#include "cb/decompile/labels.hpp"
#include "cb/decompile/z80.hpp"

namespace cb::decompile::arm {

// How control leaves an instruction (the Z80 module's: None, Jump, Branch, Call, Return, Indirect, Halt).
using Flow = z80::Flow;

enum class Isa : std::uint8_t { Arm, Thumb };

// The architecture revision: the ARM7TDMI's ARMv4T (default), or the ARM946E-S's ARMv5TE.
enum class Arch : std::uint8_t { V4T, V5TE, V6K, V7A };   // (V6K: the 3DS's ARM11, V7A: the Vita's Cortex-A9; each includes the ones before; decompile/arm_ext has their additions)

// What an instruction does. The first sixteen are the data-processing opcode field, in order.
enum class Op : std::uint8_t {
    And, Eor, Sub, Rsb, Add, Adc, Sbc, Rsc, Tst, Teq, Cmp, Cmn, Orr, Mov, Bic, Mvn,
    Mul, Mla, Umull, Umlal, Smull, Smlal,
    Ldr, Str, Ldm, Stm, Swp,
    B, Bl, Bx, Swi, Mrs, Msr,
    // ARMv5TE
    Blx, Clz, Qadd, Qsub, Qdadd, Qdsub, Smla, Smlaw, Smulw, Smlalxy, Smul, Pld, Mcr, Mrc, Bkpt,
    // ARMv6, ARMv7-A and the VFP: a row of decompile/arm_ext (`Insn::ext`)
    Ext,
};

enum class Shift : std::uint8_t { Lsl, Lsr, Asr, Ror };

// One decoded instruction. Thumb instructions are decoded into the same fields as the ARM ones they mean (a Thumb `lsls r0, r1, #2` is
// a MOV with a shifted register), so one interpreter and one writer serve both states.
struct Insn {
    std::uint32_t word = 0;       // ARM: the instruction; Thumb: the halfword (a BL: the first halfword, the second above it)
    std::uint32_t pc = 0;         // the address it runs at (bit 0 clear)
    Isa isa = Isa::Arm;
    std::uint8_t length = 4;      // 2 or 4
    Op op = Op::Mov;
    std::uint8_t cond = 14;       // 0 EQ ... 13 LE, 14 always
    bool s = false;               // sets the flags (always true for TST TEQ CMP CMN)
    std::uint8_t rd = 0, rn = 0, rm = 0, rs = 0;   // MUL MLA: rd, rm, rs, accumulate rn; UMULL ...: rd = low, rn = high; SWP: rd, rm, address rn
    // the second operand of data processing; the offset of a load or store
    bool imm_form = false;        // `imm` rather than the register rm
    std::uint32_t imm = 0;        // data processing: the value after the rotate; load or store: the offset's magnitude; SWI: the comment
    std::uint8_t rot = 0;         // data processing's immediate: the rotate field (the value is imm8 rotated right by twice it)
    bool rot_fixed = false;       // assembling: the rotate was written, not chosen
    Shift shift = Shift::Lsl;     // the register's shift ...
    std::uint8_t amount = 0;      // ... by this many (LSR and ASR of 32 are 32), or by the register rs
    bool shift_by_reg = false;
    bool rrx = false;             // rotate right with extend (a ROR by an immediate of 0)
    // load, store and block transfer
    std::uint8_t size = 4;        // 1, 2 or 4 bytes
    bool sign = false;            // a signed load (LDRSB, LDRSH)
    bool pre = true;              // pre-indexed (the offset is applied before the access)
    bool up = true;               // the offset is added
    bool wb = false;              // the base register is written back (a post-indexed access always does)
    bool user = false;            // LDRT, STRT
    std::uint16_t list = 0;       // LDM, STM: the registers
    bool hat = false;             // LDM, STM: the `^` (user registers, or the CPSR restored with the PC)
    // MRS MSR
    bool spsr = false;            // the SPSR rather than the CPSR
    std::uint8_t fields = 0;      // MSR: the field mask (bit 3 flags, 2 status, 1 extension, 0 control)
    // ARMv5TE: SMLAxy and the like pick the top (bit set) or bottom half of rm (bit 0) and rs (bit 1); MCR and MRC name the coprocessor
    std::uint8_t xy = 0;
    std::uint8_t cp = 0, opc1 = 0, opc2 = 0, crn = 0, crm = 0;   // MCR, MRC: rd is the ARM register
    bool v6 = false;                       // decoded or assembled for ARMv6 or later (a Thumb `mov` between two low registers is one there)
    std::int16_t ext = -1;                 // Op::Ext: the row of decompile/arm_ext
    std::optional<std::uint32_t> target;   // where a branch or call goes (bit 0 set: Thumb)
    Flow flow = Flow::None;

    std::array<std::uint8_t, 4> raw{};   // the bytes as the code holds them

    ByteView bytes() const { return {raw.data(), length}; }
    bool thumb() const { return isa == Isa::Thumb; }
};

// The instruction at code[at:], which runs at address `pc` (bit 0 set for Thumb), or nullopt when the bytes are not one (or too few).
std::optional<Insn> decode(ByteView code, std::size_t at, std::uint32_t pc, Arch arch = Arch::V4T);
// The same from the instruction's bits (`second` is used by a Thumb BL only).
std::optional<Insn> decode_arm(std::uint32_t word, std::uint32_t pc, Arch arch = Arch::V4T);
std::optional<Insn> decode_thumb(std::uint16_t first, std::uint16_t second, std::uint32_t pc, Arch arch = Arch::V4T);

// The instruction as source. `labels` names branch targets and the words PC-relative loads read (by real address); an empty name counts
// as no label.
std::string text(const Insn& insn, const AddressLabels& labels = {});

// The register's name ("sp"), and the number of a name ("sp", "r13", "ip": nullopt for neither).
std::string_view register_name(unsigned number);
std::optional<unsigned> register_number(std::string_view text);
// The condition's suffix ("eq"; "" for always), and the number of a suffix (cs hs cc lo ...).
std::string_view condition_suffix(unsigned cond);

// A numeric expression as the assembler reads it. ParseError for a malformed one, z80::UnknownLabel for a label not in `labels`.
std::int64_t value(std::string_view expr, const NameLabels& labels, std::uint32_t pc = 0);

// The bytes of one instruction line at `pc` (bit 0 set: Thumb state). ParseError for text that is not an instruction or one whose
// operand is out of reach (a branch too far, an immediate that does not fit); z80::UnknownLabel for a label that is not there.
Bytes assemble(std::string_view line, std::uint32_t pc = 0, const NameLabels& labels = {}, Arch arch = Arch::V4T);
// The bytes of a decoded instruction.
Bytes encode(const Insn& insn);

// The `ror` of the ARM immediate encoding.
constexpr std::uint32_t rotate_right(std::uint32_t v, unsigned n) {
    n &= 31;
    return n == 0 ? v : (v >> n) | (v << (32 - n));
}

}  // namespace cb::decompile::arm
