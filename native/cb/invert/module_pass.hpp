// cb/invert/module_pass.hpp -- `codebreaker invert <16-bit module>`: every function of a module read by the compiler models, and each reading put to the
// real compiler.
//
// A compiler model gives the source that was WRITTEN, for the functions it covers: it knows which instructions the compiler emits for which statement,
// so it reads the statements off the instructions instead of searching for them. What it says is then put to the compiler itself: the text is compiled
// and its bytes compared with the original's outside the places a linker fills in.
//
//     recognised    the model read the whole function
//     identical     ... and the real compiler turns that text back into the function's bytes
//     declined      the function has something the model does not cover; it says nothing, with the reason
//     assembly      a segment-switch thunk: generated code, written as the assembly that makes it (msc6_segment rebuilds its bytes exactly)
//
// A model never guesses: a function it does not cover is declined, with the reason. Each function is compiled by the compiler whose frame it has
// (`compiler_of`): Microsoft C 6, or Microsoft C/C++ 8 (Visual C++ 1.52c) for the modules it built.
#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/invert/msc6.hpp"

namespace cb::invert::module_pass {

// The switch sets a function is compiled under, in order, until one gives its bytes back.
inline constexpr std::string_view kFlagsMsc6[] = {"/AM /G2 /Gsw /Ox", "/AM /G2 /Gsw /Os", "/AM /G2 /Gsw", "/AM /G2 /Gsw /Ot"};
// Microsoft C/C++ 8 (Visual C++ 1.52c) built some of the 16-bit modules; it ships much unoptimised code.
inline constexpr std::string_view kFlagsMsvc152[] = {"/AM /G2 /Gsw", "/AM /G2 /Gsw /Od", "/AM /G2 /Gsw /Os", "/AM /G2 /Gsw /Ox"};

enum class Compiler { Msc6, Msvc152 };
// Which compiler made a function, by its frame: Microsoft C/C++ 8 enters with `mov ax,ds` (8c d8) or the near-loadds frame and leaves through
// `lea sp,[bp-2]` (8d 66 fe 1f 5d); Microsoft C 6 does neither.
Compiler compiler_of(ByteView code);

// A signature from the SDK headers: what the function returns ("void", "long" or "int") and its parameters.
struct Signature {
    std::string returns = "int";
    std::vector<msc6::Param> params;
};

// What the pass is given of one function.
struct Function {
    std::string name;
    std::uint32_t segment = 0, offset = 0;
    Bytes code;
    std::set<std::size_t> fixups;                          // the bytes of `code` the linker fills in (the module's relocations)
    std::optional<Signature> sdk;                          // from the SDK headers, for an exported function
    std::optional<std::vector<msc6::Param>> called;        // from how the segment's functions call it (msc6_segment::infer_signatures)
};

enum class Verdict { Identical, Recognised, Declined, Assembly };
std::string_view to_text(Verdict v);   // "identical" "recognised" "declined" "assembly"

struct Reading {
    Verdict verdict = Verdict::Declined;
    std::string text;                  // the C (or assembly), or the reason it was declined
    std::string flags;                 // identical: the switches that gave the bytes
    std::vector<std::string> tries;    // every text the models gave, in the order the compiler is asked
};

// What the compilers give: the code bytes of one text under a switch set, none when the compiler refuses it or is not set up.
using Compile = std::function<std::optional<Bytes>(std::string_view text, std::string_view flags)>;

// One function through the models, before the compilers: a thunk is assembly, else the one-shape candidates, then the Microsoft C 6.0a model with the
// SDK's signature and then without it (the signature the segment's calls give stands in where there is no SDK's).
Reading read_function(const Function& f);

// The variables a function names by address: a long shows as two words two apart used as one, anything else it stores to or pushes is a word.
// {offset: (name, width)}; the names say what they are -- an address -- and nothing more.
msc6_small::Globals globals_of(ByteView code);

// The framed functions of a module's code segments (msc6_segment::framed_functions), named `sub_<segment>_<offset>`, with the parameters the segment's
// calls give them and the relocations that fall inside them: what the pass has to go on where the caller has no describer's list.
std::vector<Function> framed_functions(const std::filesystem::path& module);

// Every function through the models, then each reading through the compilers (`msc6`, `msvc152`: absent where the compiler is not set up, and
// those functions stay recognised), under `flags` alone or the compiler's switch sets in order. `log` hears one line per compiler and switch set.
std::map<std::string, Reading> run(const std::vector<Function>& functions, const Compile& msc6, const Compile& msvc152, std::string_view flags = {},
                                   const std::function<void(const std::string&)>& log = {});

}  // namespace cb::invert::module_pass
