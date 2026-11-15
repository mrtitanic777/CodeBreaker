// cb/decompile/arm_ext.hpp -- the ARM instructions of the later cores that arm.cpp's tables do not say: ARMv6 (the 3DS's ARM11, "V6K") and ARMv7-A (the
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

#include "cb/decompile/arm.hpp"

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
// The row's mnemonic ("vadd.f32").
std::string_view mnemonic(int row);
// Does the row use the condition field (false for the unconditional space)?
bool conditional(int row);

}  // namespace cb::decompile::arm::ext
