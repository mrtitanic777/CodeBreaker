// cb/decompile/vmx128.hpp -- VMX128, the Xbox 360 (Xenon) 128-register extension of AltiVec.
//
//     decode(0x102320C3)        {"lvx128", "v1,r3,r4"}
//     coverage(code_words)      how many vector words a code section has and how many are VMX128
//     sleigh(declared)          the text of languages/src/ppc/vmx128.sinc, generated from the table
//
// Stock PowerPC decoders get VMX128 silently wrong: its extra register bits sit where a base decoder reads
// opcode-extension bits, so a VMX128 word is usually a legal but different stock instruction (`lvx128 v1,r3,r4` =
// 0x102320C3 reads as `vucmprlb 1,3,4`). The table here is the one place VMX128 is described: the native decompiler's
// SLEIGH (languages/src/ppc/vmx128.sinc, the PowerPC:BE:64:Xenon language) is generated from it.
//
// Fields (IBM numbering, bit 0 = most significant):
//   VD128 = bits 28..29 ++ 6..10   (destination; also a source in the multiply-add forms)
//   VA128 = bits 21, 26 ++ 11..15  (its high bits sit inside the extended opcode)
//   VB128 = bits 30..31 ++ 16..20
//   VC128 = bits 23..25            (3 bits: vperm128's control is v0..v7)
//
// Clean-room from the decode specification (the field layout and the 77-row opcode table), checked against the Xbox 360
// compiler's own /FAcs listings: every row decodes to its mnemonic.
#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>

namespace cb::decompile::vmx128 {

// The register fields of an instruction word.
constexpr std::uint32_t vd128(std::uint32_t w) { return (((w >> 2) & 3) << 5) | ((w >> 21) & 31); }
constexpr std::uint32_t va128(std::uint32_t w) { return (((w >> 10) & 1) << 6) | (((w >> 5) & 1) << 5) | ((w >> 16) & 31); }
constexpr std::uint32_t vb128(std::uint32_t w) { return ((w & 3) << 5) | ((w >> 11) & 31); }
constexpr std::uint32_t vc128(std::uint32_t w) { return (w >> 6) & 7; }

// The operand shape of a row.
enum class Form : std::uint8_t {
    Load,     // vD,rA|0,rB          (an indexed vector load)
    Store,    // vS,rA|0,rB
    Sldoi,    // vD,vA,vB,shift
    Abc,      // vD,vA,vB
    Madd,     // vD,vA,vD,vB         (the multiply-adds show vD again, as a source)
    Perm,     // vD,vA,vB,vC
    Simm,     // vD,signed 5-bit
    Bimm,     // vD,vB,5-bit
    Permwi,   // vD,vB,8-bit control in two pieces
    D3d,      // vD,vB,three D3D pack fields
    Rlimi,    // vD,vB,mask,rotate
    B,        // vD,vB
    Cmp,      // vD,vA,vB, with a record bit (the mnemonic gains a '.')
};

// One row of the opcode table: an instruction is the row when `word & mask == pattern`. `semantics` is what the SLEIGH
// does: `=` and an expression over vD vA vB (and the loads' and stores' own forms) for the operations C can say plainly,
// else the name of the user operation the decompiler shows.
struct Row {
    std::string_view mnemonic;
    std::uint32_t pattern;
    std::uint32_t mask;
    Form form;
    std::string_view semantics;
};

// The 77 VMX128 rows, in the table's order.
std::span<const Row> rows();

// The Xenon's (and Cell's) unaligned vector loads and stores on the 32 AltiVec registers, in primary opcode 31 -- not
// VMX128, but missing from stock decoders alike: mnemonic, extended opcode, Form::Load or Form::Store, the user operation
// the SLEIGH calls.
struct X31Row {
    std::string_view mnemonic;
    std::uint32_t extended_opcode;
    Form form;
    std::string_view operation;
};
std::span<const X31Row> x31_rows();

struct Decoded {
    std::string mnemonic;   // "lvx128", "vcmpeqfp128."
    std::string operands;   // "v1,r3,r4"
};

// The instruction a VMX128 word (or one of the X31 rows) is, or nullopt for any other word.
std::optional<Decoded> decode(std::uint32_t word);
// "lvx128 v1,r3,r4", or nullopt.
std::optional<std::string> text(std::uint32_t word);

// How the vector words of a code section divide: those with primary opcode 4, 5 or 6, and how many of them are VMX128
// (the rest are AltiVec, or data).
struct Coverage {
    std::size_t vector_words = 0;
    std::size_t vmx128 = 0;
    std::size_t other = 0;
};
Coverage coverage(std::span<const std::uint32_t> code_words);

// The text of languages/src/ppc/vmx128.sinc (no final newline). `declared`: the user operations the other PowerPC .sinc
// files already define (a second definition is an error).
std::string sleigh(const std::set<std::string>& declared = {});

// The `define pcodeop NAME;` lines of a .sinc file.
std::set<std::string> declared_operations(std::string_view sinc_text);

// Write vmx128.sinc to `out`: the table's SLEIGH, with the operations the other .sinc files in the same folder define
// left out. LF line ends. Returns the number of rows. IoError if the file cannot be written.
std::size_t write_sleigh_file(const std::filesystem::path& out);

}  // namespace cb::decompile::vmx128
