// decompile/mips_rsp.hpp -- the Nintendo 64 RSP's own instructions: its vector unit (COP2) and the vector loads and stores (LWC2, SWC2). The
// RSP is a MIPS R4000 core cut down to 32 bits -- no multiply or divide, no LWL/LWR, no FPU, no branch likely, no exceptions -- that runs
// microcode from its 4 KB of IMEM over its 4 KB of DMEM; mips.cpp's table gives it the scalar instructions it keeps (Variant::Rsp) and calls
// here for the rest. Its words are big-endian, as the R4300i's.
//
// Every instruction here decodes to `Form::Special`; its text is
//
//     vmulf $v1, $v2, $v3[0q]     vd, vs, vt and vt's element: none for the whole vector, [0q] [1q] (pairs), [0h]-[3h] (halves), [0]-[7] (one
//                                 lane), and [e1] for the element number 1, which the hardware reads as the whole vector too
//     vrcp $v1[2], $v3[5]         the one-lane instructions (vrcp vrcpl vrcph vrsq vrsql vrsqh vmov): the destination's lane, then vt's element
//     vnop                        (with its fields, when they are not zero: vnop $v1, $v2, $v3[e5])
//     mfc2 $t0, $v1[4]            the element is the byte the move starts at (0-15)
//     cfc2 $t0, $vcc              the control registers $vco $vcc $vce
//     lqv $v1[0], 16($a0)         a vector load or store: the element (the byte of the register it starts at), then the offset in bytes (the
//     sdv $v2[8], -8($sp)         word holds it divided by the access's size)
//
// What is written here is read back to the same word; a word whose unused fields are not zero is not an instruction (so data is not read as
// code).
#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "decompile/mips.hpp"

namespace cb::decompile::mips::rsp {

// Fill `insn` (which has the word, its pc and its raw bytes) when `word` is one of the RSP's COP2, LWC2 or SWC2 instructions: its mnemonic and
// form (Special). false for anything else.
bool decode(std::uint32_t word, Insn& insn);

// The text of such an instruction.
std::string text(const Insn& insn);

// The word of `name` with `operands` (already split at the commas), or nullopt when the name is none of these. ParseError for operands that
// do not fit, z80::UnknownLabel for a label that is not there.
std::optional<std::uint32_t> assemble(std::string_view name, const std::vector<std::string>& operands, std::uint32_t pc, const NameLabels& labels);

// The vector loads' and stores' sizes in bytes, by their opcode field (bits 15-11): lbv 1, lsv 2, llv 4, ldv 8, lqv lrv 16, lpv luv 8, lhv lfv
// lwv ltv 16. 0 for a field that names none.
unsigned access_size(unsigned opcode);

}  // namespace cb::decompile::mips::rsp
