// cb/source/ne_asm/segdiff.hpp -- the whole-segment byte-match harness: is this reconstructed C byte-exact?
//
// Turns "is this reconstructed C byte-exact?" into an automated verdict. The hard part (learned in the 3.1 project): you
// cannot match a function in isolation, because a near call or jump to a *neighbouring* function is a self-relative
// displacement that only becomes concrete when the neighbour is in the SAME compiland. So this harness compiles a GROUP of
// functions together (their mutual calls resolve to real displacements instead of masked relocations), then diffs against
// the shipped bytes.
//
// The comparison is mask-aware on both sides: ours masks the OMF fix-up sites (cb/formats/omf); the shipped side masks the
// NE relocation sites (following the fix-up chains). Absolute addresses legitimately differ between builds, so masking them
// is what makes a true opcode-stream match possible.
//
//     codebreaker ne-asm segdiff --selftest                  prove the mechanism
//     codebreaker ne-asm segdiff recon.c --module KRNL386 --seg 1 --off 0x1234 --func _Foo
//                                                            the verdict against the real bytes
//
// The compiler is Microsoft C 8 (Visual C++ 1.52c), given as a `Compiler16`: this module does not know how it is run.
#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>

#include "cb/base/corpus.hpp"
#include "cb/core/bytes.hpp"
#include "cb/core/ordered_map.hpp"
#include "cb/formats/ne.hpp"
#include "cb/formats/omf.hpp"

namespace cb::source::ne_asm::segdiff {

// Offsets of a segment whose bytes a loader or linker fills in: they are not compared.
using Mask = std::set<std::uint64_t>;

// The compiler: C source and its options in, the object file it made out. Throws ToolError when it made none (the
// error carries the compiler's messages).
using Compiler16 = std::function<Bytes(std::string_view c_source, std::string_view flags)>;

// Medium model, optimised, compile only.
inline constexpr std::string_view kDefaultFlags = "/AM /Ox /c";

// How many bytes the field an NE relocation record patches takes, by its source type (byte 1, selector 2, far pointer
// 4, offset 2, 48-bit pointer 6); any other type is taken as 2.
std::size_t relocation_width(std::uint8_t source);

// ---- our side: compile a group, get each function's bytes and fix-up mask -------------------------------------------------

// Each public function of the group with its bytes and the offsets its fix-ups cover (relative to the function).
OrderedMap<std::string, formats::omf::FunctionBytes> compile_group(const Compiler16& compile, std::string_view c_source,
                                                                   std::string_view flags = kDefaultFlags);

// ---- the shipped side: a segment's bytes and relocation mask from an NE module --------------------------------------------

struct Shipped {
    Bytes code;
    Mask mask;   // every byte of every relocated field, following the fix-up chains the way the module's loader does
};
// Segment `segment` (1-based) of `module`. UsageError when there is no such segment; FormatError when it lies outside
// the file.
Shipped shipped_segment(const formats::ne::Module& module, std::size_t segment);

// ---- the differ -----------------------------------------------------------------------------------------------------------

struct Verdict {
    bool exact = false;           // same length and no difference outside the masks
    bool length_match = false;
    std::size_t our_len = 0, shipped_len = 0;
    std::size_t n_diff = 0;       // differing bytes outside the masks, over the shorter length
    std::optional<std::size_t> first_diff;
    std::size_t masked = 0;       // how many offsets the two masks cover together
};

// Compare two byte strings ignoring the offsets either mask covers.
Verdict diff(ByteView ours, const Mask& our_mask, ByteView shipped, const Mask& shipped_mask);

// The verdict as text; given both byte strings, it shows them around the first difference ("--" where masked).
std::string render(const Verdict& verdict, std::optional<ByteView> ours = std::nullopt, std::optional<ByteView> shipped = std::nullopt,
                   const Mask& our_mask = {}, const Mask& shipped_mask = {});

// ---- the self-test ----------------------------------------------------------------------------------------------------------

// The function group the self-test compiles.
extern const std::string_view kSelftestGroup;

// Proves the mechanism without a retail anchor: a group matches itself, a mutation is found, and compiling the callee
// in the group makes the call concrete. Prints the report to stdout; true when all three hold.
bool selftest(const Compiler16& compile);

// ---- the command ------------------------------------------------------------------------------------------------------------

struct Environment {
    const base::Corpus& corpus;   // where a module is found
    Compiler16 compile;
};

// `segdiff <recon.c> --module M --seg N --off X --func F [--size N] [--flags F]` | `--selftest`. Returns the exit code:
// 0 byte-exact, 1 not (or the function is not in the group), 2 misuse.
int main(const Environment& env, std::span<const std::string> args);

}  // namespace cb::source::ne_asm::segdiff
