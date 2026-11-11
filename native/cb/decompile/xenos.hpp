// cb/decompile/xenos.hpp -- the Xbox 360 GPU's (Xenos) shader microcode, as instructions.
//
//     disassemble(words)      {"CF: alloc / exece", "muls r0.z, r0.yyxx", ...}
//
// The console's second instruction set: what the SDK's shader compiler (fxc) makes of HLSL. An ALU instruction
// is three words (96 bits) that issue a vector and a scalar operation together; control flow (CF) instructions
// are 48 bits, two to three words; fetch instructions read vertices and textures. The encodings are the ones
// docs/XBOX360_TOOLCHAIN.md's study recovered from the SDK's microcode assembler.
//
// What is checked (tests/test_xenos.py and xenos_test.cpp), against the shader compiler's own listing of the same
// microcode (`fxc /T ps_3_0 /XuFormatRaw /Fh`): the ALU word's fields -- vector and scalar opcode, destination,
// write masks, the export bit, saturate, each source's register, select, negate and swizzle -- and the CF opcodes.
// What is not yet: the layout of a whole program (here the leading CF words are one pair and every later group
// of three words is an ALU instruction -- true of the simple shaders it is checked on; a real program interleaves
// them through CF exec ranges), the fetch words (recognised, not decoded), and the swizzle's spelling (relative,
// so `r0.xy` reads `r0.yyxx`: the same channels, written out in full).
#pragma once

#include <array>
#include <cstdint>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace cb::decompile::xenos {

// The vector ALU opcode's name; empty for an opcode with no name (0x1e, 0x1f).
std::string_view vector_name(std::uint32_t opcode);
// How many sources a vector opcode reads (0..3).
std::uint32_t vector_sources(std::uint32_t opcode);
// The scalar ALU opcode's name; empty when unnamed.
std::string_view scalar_name(std::uint32_t opcode);
// The control-flow opcode's name; empty when unnamed.
std::string_view control_name(std::uint32_t opcode);

// A write mask (x=1, y=2, z=4, w=8) as its channels; bits past the fourth are ignored.
std::string mask_text(std::uint32_t mask);
// Two bits per channel, relative to the channel itself (0 = the same channel).
std::string swizzle_text(std::uint32_t swizzle);
// A source: a temporary register (rN) when `selected`, else a constant (cN); bit 7 of the number takes its
// absolute value, bit 6 indexes it by the loop counter.
std::string register_text(bool selected, std::uint32_t number);

// The instructions one 96-bit ALU word issues: [vector, scalar], each when its slot is used.
std::vector<std::string> alu(std::uint32_t d0, std::uint32_t d1, std::uint32_t d2);
// The two 48-bit CF instructions in three words: their opcodes.
std::vector<std::string> control(std::uint32_t d0, std::uint32_t d1, std::uint32_t d2);
// Lines for a simple shader: one CF pair, then ALU words up to three zero words.
std::vector<std::string> disassemble(std::span<const std::uint32_t> dwords);

// A pixel shader as fxc built it (/XuFormatRaw /Fh), and fxc's own listing of it.
inline constexpr std::array<std::uint32_t, 12> kSample = {0x00000000, 0x2001c400, 0x22000000, 0x08400000,
                                                          0x00000061, 0xe2000000, 0xc80f8000, 0x00404000,
                                                          0xe2000000, 0,          0,          0};
inline constexpr std::array<std::string_view, 4> kSampleListing = {"alloc colors", "exece", "muls r0.z, r0.xy",
                                                                    "mov oC0, r0.xyzx"};

}  // namespace cb::decompile::xenos
