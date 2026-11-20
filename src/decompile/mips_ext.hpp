// decompile/mips_ext.hpp -- the instructions of the two later MIPS consoles that mips.cpp's table does not say in one line: the
// Emotion Engine's (R5900) vector unit 0 in macro mode (COP2) and its LQC2 / SQC2, and the Allegrex's EXT and INS and its VFPU. They are
// read and written here, and mips.cpp calls them (decode, text, assemble) for what its table did not know.
//
// Every instruction here decodes to `Form::Special`; its text is
//
//     ext     $t0, $a0, 4, 8                      rt, rs, position, size
//     ins     $t0, $a0, 4, 8
//     lqc2    $vf1, 16($a0)                       the VU0 registers: $vf0-$vf31 (floating point), $vi0-$vi15 (integer), $Q and $I
//     vaddx.xyzw $vf1, $vf2, $vf3                 a VU0 macro: its destination fields as letters after the dot (`.xyzw`, `.xy`, `.0` for none),
//     vmulq.xyz  $vf1, $vf2, $Q                   a broadcast of x, y, z or w in the name, the Q and I registers as written
//     viadd   $vi1, $vi2, $vi3
//     vadd.q  C000, C010, C020                    a VFPU instruction: the size after the dot (.s single, .p pair, .t triple, .q quad)
//     vdot.q  S000, C010, C020                    and its registers written as the 7-bit field reads
//     lv.q    C100, 16($a0)                       (S{matrix}{column}{row} for a single, C or R for a column or row vector, M or E for a matrix
//     sv.q    C100, 16($a0), wb                    and its transpose, each with its matrix, column and row digits: the same digits the hardware
//     bvt     3, L_08804020                       numbers them with)
//
// What is written here is read back to the same word; a word whose unused fields are not zero is not an instruction (so data is not read
// as code).
#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "decompile/mips.hpp"

namespace cb::decompile::mips::ext {

// Fill `insn` (which has the word, its pc and its raw bytes) when `word` is one of the extension instructions of `variant`: its mnemonic,
// form (Special), flow, delay slot, branch target and likelihood. false for anything else (and for every variant but the R5900 and the Allegrex).
bool decode(std::uint32_t word, std::uint32_t pc, Variant variant, Insn& insn);

// The text of such an instruction (its branch target named from `labels` when it has a label).
std::string text(const Insn& insn, const AddressLabels& labels);

// The word of `name` with `operands` (already split at the commas), or nullopt when the name is none of the extension's. ParseError for
// operands that do not fit, z80::UnknownLabel for a label that is not there.
std::optional<std::uint32_t> assemble(std::string_view name, const std::vector<std::string>& operands, std::uint32_t pc, const NameLabels& labels, Variant variant);

}  // namespace cb::decompile::mips::ext
