// symbols/debug_build.hpp -- real internal function names for a shipped NE module, from a debug build of the same version.
//
// The retail export table names only about a quarter of a system module's functions; the rest carve as anonymous `sub_XXXX`. A debug build
// of the identical Windows version carries the true names (debug builds declare internals PUBLIC and the linker's .MAP lists every one), but at
// DIFFERENT offsets: debug adds assertion and RIP code and shifts everything. So the two cannot be aligned by address; they are aligned by an
// offset-invariant byte signature.
//
// The signature is the objdump text of each instruction with only the OFFSET-dependent operands masked: branch and call targets, absolute
// `[0xNNNN]` memory and far `seg:off` pointers. IMMEDIATES stay, because they are part of a function's identity (masking them let functions with
// the same instruction skeleton and different constants collide). A signature match is therefore byte identity modulo relocation: the two
// functions are provably the same code, and the debug build's name for it is exact.
//
// Three guards keep the result at no false positives:
//   1. ambiguity: a signature that two DIFFERENT debug names share is discarded (which name is right cannot be told);
//   2. export validation: the retail export table is a labelled test set, so any signature a retail EXPORT already owns under a different name is
//      discarded as collision-prone (this catches a two-instruction predicate that matches the wrong short function). After the filter every known
//      export agrees with its own signature, the proof the method is sound;
//   3. minimum length and uniqueness: functions under kMinLength instructions are too skeleton-like to trust, and a real name claimed by more than
//      one retail offset is dropped entirely (a symbol lives at one address, so a double match is an internal collision, and duplicate labels would
//      break the emission).
// Only a module with BOTH a debug binary and its .MAP can be recovered this way. The result, `names.json` ("seg:off" -> NAME), renames the anonymous
// functions in the emitted idiomatic source (`segment::Seg`'s overrides): a pure label rename, so byte exactness is untouched.
//
//     codebreaker ne-asm debug-names
#pragma once

#include <compare>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <map>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "base/corpus.hpp"
#include "formats/ne.hpp"
#include "source/ne_asm/fixups.hpp"
#include "source/ne_asm/segment.hpp"

namespace cb::symbols::debug_build {

// Functions shorter than this share skeletons and are collision-prone.
inline constexpr std::size_t kMinLength = 5;

// One function of a module: its code segment (from 1) and its offset in it.
struct FunctionKey {
    std::uint32_t segment = 0, offset = 0;
    friend auto operator<=>(const FunctionKey&, const FunctionKey&) = default;
    // "seg:off" as names.json writes it: the segment in decimal, the offset in four lowercase hex digits.
    std::string text() const;
};
using Names = std::map<FunctionKey, std::string>;

// A function's signature: one entry per instruction, the offset-dependent operands masked.
using Signature = std::vector<std::string>;
using SignatureIndex = std::map<Signature, std::string>;

// The signature of the function at `start`: its instructions with whitespace collapsed, a branch or call target (`jmp 0x1234`) as `jmp X`, an
// absolute memory operand `[0x1234]` as `[X]` and a far pointer `0x1:0x2` as `X:X`.
Signature signature(const source::ne_asm::segment::Seg& seg, std::uint32_t start);

// The names a linker .MAP lists: segment -> offset -> name, from the lines ` SSSS:OOOO  name` (hex, then a word). A name met twice at one place keeps
// the later one.
std::map<std::uint32_t, std::map<std::uint32_t, std::string>> parse_map(std::string_view map_text);

// {signature -> name} for every uniquely-signatured named function of the debug build: each code segment the map names functions in is disassembled,
// the signature of each named function that starts an instruction taken, and signatures that carry two different names dropped. FormatError for a
// module that is not an NE.
SignatureIndex build_index(const formats::ne::Module& debug_build, std::string_view map_text, const source::ne_asm::segment::SharedOrdinals& ordinals,
                           const source::ne_asm::fixups::Objdump& objdump = source::ne_asm::fixups::run_objdump);

struct Match {
    Names names;                           // the exactly matching real names for the retail module's anonymous functions
    std::size_t agree = 0, disagree = 0;   // over the retail module's known exports: disagree must be 0
};
// The names `index` gives the retail module's functions, with the three guards. A function that already has a real (export) name is not renamed, but
// counts in `agree` or `disagree` when the match names it too.
Match match_module(const formats::ne::Module& retail, const SignatureIndex& index, const source::ne_asm::segment::SharedOrdinals& ordinals,
                   std::size_t min_length = kMinLength, const source::ne_asm::fixups::Objdump& objdump = source::ne_asm::fixups::run_objdump);

// The whole run: the debug index, then the match.
Match recover(const formats::ne::Module& retail, const formats::ne::Module& debug_build, std::string_view map_text,
              const source::ne_asm::segment::SharedOrdinals& ordinals, const source::ne_asm::fixups::Objdump& objdump = source::ne_asm::fixups::run_objdump);

// names.json: the names as a JSON object, keys sorted as text, one entry a line. ParseError reads it back; the names are a segment -> offset -> name table
// ready for `segment::Seg`'s overrides.
std::string to_json(const Names& names);
std::map<std::size_t, source::ne_asm::segment::NameOverrides> parse_overrides(std::string_view names_json);

// ---- the command ------------------------------------------------------------------------------------------------------------

struct Environment {
    const base::Corpus& corpus;
    std::filesystem::path state_folder;   // names.json goes to <state>/idiomatic/krnl386/names.json
    source::ne_asm::fixups::Objdump objdump = source::ne_asm::fixups::run_objdump;
};
// `[--ords FILE]`: the names of KRNL386 from its debug build (the reference SDK's WIN32SDK/SCT/DEBUG16/I386 KRNL386.EXE and .MAP), saved, with the
// export validation. Returns 0 when every known export agrees with its own signature, else 1.
int main(const Environment& env, std::span<const std::string> args);

}  // namespace cb::symbols::debug_build
