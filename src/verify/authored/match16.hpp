// verify/authored/match16.hpp -- does Microsoft C turn a 16-bit module's reconstruction back into its bytes?
//
//     codebreaker match <16-bit module> [--flags "/AM /Gcsw /Os"] [--first N]
//
// The 16-bit counterpart of match (see there for what the comparison is and is not). Each reconstructed function is compiled by
// Microsoft C (build/msc6: the real compiler, in an emulator, a whole module's functions in one session) and its bytes are compared
// with the original function's, outside the places a loader or linker fills in: in the original, every word a relocation record of its
// segment names and the distance of every near call and jump to another function; in the compiled function, every place the object
// file has a fix-up for.
//
// Without switches given the compiler and its switches are found first, segment by segment (a segment is one source file's code, or
// several the linker packed together): those under which most of a sample of its functions come out the original's bytes say how it
// was built. Every function is then judged under its segment's three best settings and keeps the one it comes nearest under; the
// functions of one source file, which was built with one command line, take together the setting they come nearest under. Each
// function is also said several ways (as the decompiler wrote it, with its temporaries folded back, its far parameters joined, its
// frame split, through the SDK's prototypes ...: the `Variant`s) and judged by the one nearest the original, since which a
// function's source was is the compiler's to say. What the choices were is remembered in a file, and found again by the next run.
//
// This module is the driver: what to ask the compiler, in what order, and what to keep. The toolchain it drives comes in as a
// `Toolchain` (build/msc6 and the comparison of match are its implementation), the text a function is said as in each variant as the
// `Variant`s the caller made, and the run-time library and the search of the last bytes (verify/authored/search16) as hooks, so the
// whole of the selection is testable with a toolchain that is not a DOS emulator.
//
//     match16::Inputs in{.functions = ..., .variants = ..., .compilers = {"msvc152"}, .toolchain = &chain, .header = header};
//     match16::Result r = match16::run(in);
//     for (const std::string& line : match16::report_lines("setup.dll", r)) ...
#pragma once

#include <compare>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "core/bytes.hpp"
#include "platforms/decoder.hpp"
#include "verify/authored/search16.hpp"

namespace cb::verify::authored::match16 {

// How near a function's bytes came: the verdict word, the bytes that differ and the bytes compared.
using Verdict = search16::ByteVerdict;

// How near a verdict is: nothing for a function that did not compile, 2 for an identical one, else the share of its bytes that
// are the original's.
double score(const Verdict& v);
// The sum over a set of functions.
double score(std::span<const Verdict> verdicts);

// A function's bytes, and which of them a loader or linker fills in (a mask byte is non-zero there) and a comparison leaves out.
struct Masked {
    Bytes code;
    Bytes mask;
};

// One function of the module: where it is (its segment and its offset in it) and the original's bytes.
struct Function {
    std::string name;
    std::uint32_t segment = 0;
    std::uint32_t offset = 0;
    Masked original;
};

// The original's source files, as far as its calls tell: a call to a function in the same segment (`push cs; call near`, or a near
// call) puts caller and callee in one file, and a file is contiguous, so everything between them too. A function no such call
// reaches is a file of its own. Files are listed by segment (in the order a segment is first met), each segment's by offset.
std::vector<std::vector<std::string>> source_files(std::span<const Function> functions, const platforms::Decoder& x86_16);

// The compiler and its switches.
struct Setting {
    std::string compiler;
    std::string switches;
    auto operator<=>(const Setting&) const = default;
};

// One way of saying every function: which of the passes (`fold`, `far`, `frame`, `sdk`, `wine`) were applied, the header the
// functions are compiled with and each one's text. `name` is the passes joined by `+` in their fixed order, `plain` for none.
struct Variant {
    std::string name;
    std::set<std::string> stages;
    std::string header;
    std::map<std::string, std::string> texts;   // function -> its text said this way
};
// The variants of a set of passes: one for each subset, in binary order of the subset (`plain` first). Names and stages only; the
// caller fills in each one's header and texts.
std::vector<Variant> make_variants(std::span<const std::string> stages);

// What the toolchain is asked to compile: these functions' texts under this header, compiler and switches. The functions of a source
// file (`files`) are compiled together, as they were in the original.
struct Compilation {
    std::string compiler;
    std::string switches;
    std::string header;
    std::vector<std::pair<std::string, std::string>> sources;   // function -> text
    const std::vector<std::vector<std::string>>* files = nullptr;
};

// The compilers and the comparison.
class Toolchain {
public:
    virtual ~Toolchain() = default;
    // The switch sets a compiler is tried with when none are given.
    virtual std::vector<std::string> flag_sets(const std::string& compiler) const = 0;
    // Each function's compiled bytes, or nothing where it did not compile.
    virtual std::map<std::string, std::optional<Masked>> compile(const Compilation& request) = 0;
    // How near compiled bytes are to the original's.
    virtual Verdict distance(const Masked& built, const Masked& original) const = 0;
};

using Log = std::function<void(std::string_view line)>;   // empty: nothing is logged

struct Inputs {
    std::vector<Function> functions;               // the functions to judge, in the module's order
    std::vector<Variant> variants;                 // each said, from make_variants; `plain` is among them
    std::string header;                            // the module's header, for a text that no variant's header names (a searched one)
    std::vector<std::string> compilers;            // the compilers to try, in order (at least one)
    std::optional<std::string> switches;           // given: every segment is built with them, by the first compiler
    bool choose = false;                           // find the settings again though an earlier run remembered them
    bool compiler_given = false;                   // the caller named a compiler: nothing is remembered, nothing read back
    std::filesystem::path choices_file;            // where the choices are remembered (empty: nowhere)
    Toolchain* toolchain = nullptr;
    // The functions that are the compiler's run-time library, linked and not written (build/gate16 runtime16): function -> its name.
    std::function<std::map<std::string, std::string>(std::span<const Function>)> identify;
    int search_rounds = 0;                         // the rewrites of the last bytes of the near misses; 0: none
    Log log;
};

struct Result {
    std::map<std::uint32_t, Setting> chosen;       // each segment's setting
    std::map<std::string, Verdict> verdicts;       // function -> how near it came (Identified for a library function)
    std::map<std::string, Setting> setting;        // the setting each function was judged under in the end (a file's: one)
    std::set<Setting> settings;
    std::vector<std::vector<std::string>> files;   // the source files, as source_files made them
    std::map<std::string, std::uint32_t> segment;
    std::map<std::string, std::string> winner;     // the way of saying each function came nearest under (`search` for a rewrite)
    std::map<std::string, std::string> texts;      // each function's text in its winning way
    std::map<std::string, std::optional<Masked>> code;   // its compiled bytes in that way
    std::map<std::string, std::string> library;
};

Result run(const Inputs& inputs);

// What a run prints: the counts by verdict under the settings used, which way of saying won the identical ones, and the eight nearest
// misses. `module_name` is the file's name.
std::vector<std::string> report_lines(std::string_view module_name, const Result& result);

}  // namespace cb::verify::authored::match16
