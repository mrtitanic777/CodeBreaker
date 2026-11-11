// cb/build/ppc_asm.hpp -- an Xbox 360 PowerPC assembler: mnemonic -> big-endian 4-byte word.
//
//     std::optional<std::uint32_t> word = ppc_asm::assemble_one("li r3, 1", address, labels);
//     Bytes code = ppc_asm::assemble("top:\n  nop\n  b top\n", base_address);
//
// For patching and hooking a title's code (formats/xex_edit): branches, the common D / X / XL / XFX forms and
// the simplified mnemonics (nop, li, lis, mr, mtlr, blr, ...). Not a whole assembler -- an instruction outside
// that set stops with AsmError. A comment starts with `#` or `;`; `label:` on a line of its own names the next
// instruction's address; operands are separated by commas and/or white space.
//
// PowerPC fields (bits numbered from the LSB): opcode<<26, RT/RS<<21, RA<<16, RB<<11, XO<<1, Rc<<0.
// Big-endian in memory. Assembler operand order is destination first (`or rA,rS,rB`, `ori rA,rS,UI`); the
// encoding puts RS in the high register field, so the logical forms swap the first two operands.
#pragma once

#include <cstdint>
#include <optional>
#include <string_view>

#include "cb/build/asm_text.hpp"

namespace cb::build::ppc_asm {

// One instruction at virtual address `address` as its 32-bit word; nullopt for a blank, comment or label-only
// line. `labels` maps names to the addresses branch operands may use. AsmError for an unsupported mnemonic, a
// missing or malformed operand, an immediate or branch distance that does not fit, or an unaligned branch.
std::optional<std::uint32_t> assemble_one(std::string_view line, std::uint32_t address, const Labels& labels = {});

// Several lines to big-endian bytes, in two passes so a label may be used before it is defined. The AsmError of
// a line names its number.
Bytes assemble(std::string_view text, std::uint32_t base_address = 0);

}  // namespace cb::build::ppc_asm
