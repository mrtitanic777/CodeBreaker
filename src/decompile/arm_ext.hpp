// decompile/arm_ext.hpp -- the ARM instructions of the later cores that arm.cpp's tables do not say: ARMv6 (the 3DS's ARM11, "V6K") and ARMv7-A (the
// PlayStation Vita's Cortex-A9, "V7A"). They are one table of rows read both ways: the bits a row fixes (a mask and a value), the mnemonic, and an operand
// list; the decoder takes the first row a word matches, the text is the mnemonic (the condition after the name, before a `.f32` type suffix) and the operands
// as `.syntax unified` writes them, and the assembler finds the row from the mnemonic and builds the word from the operands.
//
//     rev      r0, r1                           ARMv6 byte reversal, sign and zero extension (`uxtab r0, r1, r2, ror #8`), LDREX / STREX, the saturating
//     uxtb     r0, r1, ror #16                  and packing instructions, the DSP multiplies (SMMUL, SMLAD, UMAAL ...), and the parallel add and subtract
//     ldrex    r0, [r1]                         families (`uadd8`, `qsub16` ...)
//     vadd.f32 s0, s1, s2                       the VFP: single and double arithmetic, VLDR / VSTR, VPUSH / VPOP, VLDM / VSTM, the moves between the VFP
//     vldr     d8, [r4, #16]                    and the core registers, VMRS / VMSR, comparisons and conversions
//
// A word whose fixed bits do not match a row is not an instruction here, and what a row writes assembles back to the same word.
#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>

#include "decompile/arm.hpp"

namespace cb::decompile::arm::ext {

// What the interpreter does with a row (None: decoded and written, not run).
enum class Sem : std::uint8_t {
    None, Rev, Rev16, Revsh, Sxtb, Sxth, Uxtb, Uxth, Sxtab, Sxtah, Uxtab, Uxtah, Ldrex, Strex, Ldrexb, Strexb, Ldrexh, Strexh, Umaal, Smmul, Smmla, Smmls,
    Ssat, Usat, Pkhbt, Pkhtb, Sel, Usad8, Usada8, Smuad, Smusd, Smlad, Smlsd, Clrex, Hint,
};

// The row an ARM word matches under `arch` (V6K or later), or -1. The condition field is ignored except by the rows of the unconditional space.
int find_arm(std::uint32_t word, Arch arch);
// The word's text (`rev r0, r1`, `vadd.f32 s0, s1, s2`), with the condition of an ARM word.
std::string text(int row, std::uint32_t word, std::uint32_t pc, const AddressLabels& labels);
// The word of a line whose mnemonic is a row's; nullopt for another mnemonic; ParseError for a row's mnemonic with operands that do not fit it.
std::optional<std::uint32_t> assemble(std::string_view line, std::uint32_t pc, const NameLabels& labels, Arch arch);
// What the interpreter does with the row.
Sem semantics(int row);
// The number of rows (the rows are numbered from 0).
int row_count();
// The row's mnemonic ("vadd.f32").
std::string_view mnemonic(int row);
// Does the row use the condition field (false for the unconditional space)?
bool conditional(int row);

}  // namespace cb::decompile::arm::ext

// ARMv7-A Thumb: the 32-bit instructions (a first halfword of $E800 or more) and the 16-bit ones Thumb-1 does not have. The branches decode into the ordinary
// `Op::B` / `Op::Bl` / `Op::Blx` (so flow, the interpreter and the writers need nothing new); IT, CBZ / CBNZ, the hints, the byte-reversal and extension
// instructions and the VFP's Thumb encodings (the ARM rows, their condition 1110) are `Op::Ext`; every other 32-bit instruction is decoded for its length and
// its text only (Capstone's disassembly of it), and an interpreter stops at it as not run.
namespace cb::decompile::arm::t2 {

inline constexpr int kIt = 1000, kCbz = 1001, kCbnz = 1002, kHint = 1010, kExtend = 1020, kMovt = 1030, kUbfx = 1031, kSbfx = 1032, kBfi = 1033, kExtend32 = 1040, kOpaque = 1100;
// The Advanced SIMD structure loads and stores, one register and a modified immediate, and VMOV (immediate) of the VFP (decompile/arm32_neon.cpp).
inline constexpr int kNeon = 1300;
// The 32-bit integer instructions decode_more reads (decompile/arm32_thumb.cpp; the fields are in the comments there and in emulate/arm32_exec.cpp).
inline constexpr int kOrn = 1200, kPkh = 1201, kAdr = 1202, kSsatT = 1203, kUsatT = 1204, kSsat16T = 1205, kUsat16T = 1206, kMls = 1207, kNop32 = 1208, kLdrexT = 1209,
                     kStrexT = 1210, kLdrexdT = 1211, kStrexdT = 1212, kTb = 1213;
// Whether an ARM-state instruction names its registers as the manual defines it (the pc as an operand of a data-processing instruction that shifts by a register, as the
// base of a block transfer or of an access that writes the base back, a base that is also the register loaded or stored with write-back ...: UNPREDICTABLE). The ARMv7-A
// decoder reads only the defined ones.
bool defined_arm_operands(const Insn& insn);
// A structure load or store as its fields: the register list, the element, the bytes it moves.
struct NeonTransfer {
    enum Kind { Multiple, Lane, AllLanes } kind = Multiple;
    bool load = false;
    unsigned rn = 0, rm = 0;      // the base register; the offset register (15: no write-back, 13: by the bytes moved)
    unsigned d = 0;               // the first D register
    unsigned regs = 1;            // registers a multiple-structure transfer names in each step; AllLanes: the registers of VLD1
    unsigned selem = 1;           // elements in a structure (1 to 4)
    unsigned inc = 1;             // the step between the registers of a structure
    unsigned ebytes = 1;          // bytes of an element
    unsigned lane = 0;            // Lane: the element number
    unsigned bytes = 0;           // the bytes it moves
};
// The ARM word a Thumb Advanced SIMD instruction means (the data-processing and the load and store forms are ARM's with the first byte changed; the coprocessor space is the same).
std::uint32_t neon_arm_word(std::uint16_t a, std::uint16_t b);
std::optional<NeonTransfer> neon_transfer(std::uint32_t arm_word);
bool neon_modified_immediate(std::uint32_t arm_word);
bool vfp_move_immediate(std::uint32_t arm_word);
// The instruction when its ARM word is one of those (`word_as_held`: the instruction as the code holds it, for `Insn::word`), or nullopt.
std::optional<Insn> decode_neon(std::uint32_t arm_word, bool thumb, std::uint32_t pc, std::uint32_t word_as_held, Arch arch);
// The 32-bit Thumb instruction `a`, `b` that decode_thumb32_int does not read, as one the interpreter runs, or nullopt.
std::optional<Insn> decode_more(std::uint16_t a, std::uint16_t b, std::uint32_t pc_tagged);

bool is_32bit(std::uint16_t first);
// The instruction at `a` (and `b` for a 32-bit one) running at `pc`, or nullopt where Thumb-1 decoding is what is wanted.
std::optional<Insn> decode(std::uint16_t a, std::uint16_t b, std::uint32_t pc, Arch arch);
// The text of an `Op::Ext` Thumb instruction.
std::string text(const Insn& insn, const AddressLabels& labels);
// The bytes of a line that is one of the 16-bit rows (IT, CBZ, `nop.n`, `uxtb r0, r1` ...); nullopt for another mnemonic.
std::optional<Bytes> assemble(std::string_view line, std::uint32_t pc, const NameLabels& labels);
// The condition (0-14) that applies to the instruction the `it` at its position leaves `index` instructions before (0 first), or 14 outside a block.
unsigned it_condition(std::uint8_t first_condition, std::uint8_t mask, unsigned index);

}  // namespace cb::decompile::arm::t2
