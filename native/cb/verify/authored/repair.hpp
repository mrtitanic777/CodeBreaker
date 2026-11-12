// cb/verify/authored/repair.hpp -- recover the authored source by shrinking the byte distance to the original.
//
//     codebreaker repair <module> [--flags "/O2 /Gz"] [--first N]
//     codebreaker repair --truth <compiler> [target]
//
// The behavioural pipeline (verify/) proves a reconstruction MEANS the same as the original, but many different sources
// mean the same thing, and the decompiler's C is only one of them -- and not usually the one that was written. The byte
// distance to the original, when the source is compiled with a compiler of its time (match), is the gradient that tells
// which of the many behaviourally-equal sources is closest to what was written: a register variable rather than a stack
// one, declarations in the order that lays the frame out the same way, an unused local the author left in, the operands
// of a sum in the order they were typed.
//
// This is a repair, not a fresh decompile. It starts from the behaviourally-verified candidate and changes one thing at
// a time -- a KNOB -- keeping a change when it brings the bytes closer. Every knob is a rewrite that cannot change what
// the function does (an unused local, a reordering of declarations, a commutative swap, an if-chain written as a switch
// ...), so a variant always still behaves like the original: the search moves toward the authored bytes without ever
// risking a plausible-but-wrong result. The variants, their distances and the knob each changed are kept in the store;
// the closest is the reconstruction offered as the authored source. Byte-exactness is not the goal and not the measure --
// behaviour is. This is the signal that chooses, among sources that all behave right, the one nearest to what a person
// wrote.
//
// The module is the search and its driver. The period compiler, the comparison of bytes, the module's prepared
// functions and the store the variants are kept in are the caller's: they come in as `Compile` / `Distance` /
// `Compiler` / `Module` and a `Record` callback, so the search is testable with a compiler that is not a compiler.
//
//     repair::Searcher searcher(repair::Isa::X86_32, distance);
//     std::optional<repair::Repaired> r = searcher.repair(compile, source, original);
#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/platforms/decoder.hpp"
#include "cb/verify/authored/search16.hpp"

namespace cb::verify::authored::repair {

// The processor of the code, as far as the search needs to know: with one it lines the two versions up by instruction and
// reads the size of the locals; without (Unknown) it has the bytes alone to go by.
enum class Isa { Unknown, X86_16, X86_32, PowerPC };

// A function's compiled code: its bytes, and what a loader or linker fills in (and a comparison leaves out). x86: `mask`
// has a non-zero byte for each such byte of `bytes`. PowerPC: `word_mask` has, for each 4-byte word, the bits left out.
struct Code {
    Bytes bytes;
    Bytes mask;
    std::vector<std::uint32_t> word_mask;
};

using ByteVerdict = search16::ByteVerdict;   // identical / same size / different, the bytes that differ and the bytes compared

// How far a compiled function is from the original, outside the places the linker fills in (match's comparison).
using Distance = std::function<ByteVerdict(const Code& built, const Code& original)>;
// A period compiler: source text -> the function's code, or nullopt when it does not compile.
using Compile = std::function<std::optional<Code>(std::string_view text)>;
using Log = std::function<void(std::string_view line)>;   // an empty function: nothing is logged

inline constexpr std::size_t kMaxLocals = 6;   // the leading locals whose order and storage are tried
inline constexpr std::size_t kMaxSites = 6;    // places in the body each kind of rewrite is tried at
inline constexpr std::size_t kMaxWork = 300;   // compiles per function, a ceiling on the search
inline constexpr std::size_t kBeam = 3;        // the sources the search goes on from, each round
inline constexpr std::size_t kPatience = 2;    // rounds without a better source before the search stops

// ---- the rewrites ------------------------------------------------------------------------------------------------

// A function's source cut into the heading (up to and including the opening brace), its declaration lines and the rest.
struct Split {
    std::string head;
    std::vector<std::string> decls;
    std::string body;
};
Split split(std::string_view source);

// One rewrite of a source and the name of the knob that made it (`swap0`, `order3`, `pad16` ...).
struct Knob {
    std::string name;
    std::string source;
};

// `if (A op B)` / `while (A op B)` whose whole condition is one comparison of two things that do nothing but have a value:
// the condition with the sides exchanged, as the part of the body to replace. `shadow` is the body with its strings and
// comments blanked.
struct TestSwap {
    std::size_t begin = 0, end = 0;   // the condition inside the parentheses
    std::string text;
};
std::vector<TestSwap> test_swaps(std::string_view shadow, std::string_view body);

// The line of a declaration written with `register`, or without it where it had it; nullopt for what is no plain
// declaration (an array, a static, an extern).
std::optional<std::string> toggle_register(std::string_view line);

// Each rewrite of `source` that preserves what the function does. `target_frame` is the bytes the original reserves for
// locals (0: unknown), `draft_frame` the draft's (nullopt: not known).
std::vector<Knob> knobs(std::string_view source, std::size_t target_frame, std::optional<std::size_t> draft_frame);

// ---- the search --------------------------------------------------------------------------------------------------

// What the search found for one function.
struct Repaired {
    std::string source;
    ByteVerdict verdict;   // how near the best source's bytes came
    std::string knobs;     // the knobs applied, comma-separated, empty when the starting source was the nearest
    std::size_t tried = 0;
};

class Searcher {
public:
    // `distance` compares two compiled functions; `isa` says how their instructions are read.
    Searcher(Isa isa, Distance distance);

    // The bytes a function reserves for locals: the N of `sub esp, N` (`sub sp, N`) in its prologue (PowerPC: of
    // `stwu r1,-N(r1)`), 0 when there is none.
    std::size_t frame(ByteView code);

    // A function's code as its instructions: each the bytes of one with the linker's bytes blanked (PowerPC: each word with
    // the bits the linker fills in cleared, as four bytes) -- what two versions of a function are lined up by.
    std::vector<std::string> instructions(const Code& built);

    // How far a compiled function is from the original, smaller the nearer: (the instructions that have no counterpart,
    // the bytes that differ). Bytes alone mislead: one instruction more near the top moves every later byte, and the
    // function looks wholly wrong when it is nearly right. Lined up by instruction, it is one instruction away.
    struct Score {
        std::pair<std::size_t, std::size_t> rank;
        ByteVerdict verdict;
    };
    Score score(const Code& built, const Code& original, const std::vector<std::string>* target = nullptr);

    // Search behaviour-preserving rewrites of one function's source for the bytes closest to the original's: the kBeam
    // nearest sources are kept and the search goes on from all of them, since the first step toward the original is not
    // always one that brings the bytes nearer by itself. nullopt when the starting source does not compile.
    std::optional<Repaired> repair(const Compile& compile, std::string_view source, const Code& original);

private:
    Isa isa_;
    Distance distance_;
    platforms::DecoderSet decoders_;
};

// ---- a module ----------------------------------------------------------------------------------------------------

// The period compiler of a module (it holds the paths and the environment the compiler needs).
class Compiler {
public:
    virtual ~Compiler() = default;
    // Compile one function's `source` under `header` with the switches `flags`, working in `work`.
    virtual std::optional<Code> compile(std::string_view header, std::string_view source, std::string_view name,
                                        std::string_view flags, const std::filesystem::path& work) const = 0;
    virtual ByteVerdict distance(const Code& built, const Code& original) const = 0;
    // The switch sets the module's switches are chosen from.
    virtual std::vector<std::string> flag_sets() const = 0;
    // The header with only the declarations `source` uses (types and macros are all kept); by default the whole header.
    virtual std::string needed_header(std::string_view header, std::string_view source) const {
        (void)source;
        return std::string(header);
    }
};

// One function of a module: its name, the key it has in the store, its reconstructed text and the original's code.
struct Target {
    std::string name;
    std::string key;
    std::string text;
    Code original;
};

struct Module {
    Isa isa = Isa::X86_32;
    std::string header;
    std::vector<Target> targets;   // in the order they are judged; only functions the original has bytes for
    std::string compiler_name;     // recorded with each variant, followed by the switches ("cl.exe /O2")
};

// repair() of a 32-bit function with Microsoft's compiler of 1994: before the search, the setting the function is
// compiled in is chosen, since the files of one module were not all compiled alike: with and without a frame pointer
// kept (/Oy-), and with the whole module's declarations before it or only those it uses. The search then runs in the
// setting whose plain compile is nearest; what was chosen is the first thing in `knobs` when it is not the default.
std::optional<Repaired> repair_function(const Compiler& cc, std::string_view header, const Target& target,
                                        std::string_view flags, const std::filesystem::path& work);

// The compiler switches the module was most likely built with: the set under which most of a sample of its functions come
// out as the original's bytes (3 for identical, 1 for the same size), the first of them on a tie.
std::string best_flags(const Compiler& cc, const Module& module, const std::filesystem::path& work, std::size_t sample = 60);

struct Counts {
    std::size_t functions = 0, identical = 0, same_size = 0, different = 0;
    bool operator==(const Counts&) const = default;
};

// What a repaired function is kept as (the store's variant: key, the compiler's setting, the text and how near it came).
using Record = std::function<void(const Target& target, const Repaired& repaired, const std::string& setting)>;

struct RunOptions {
    std::optional<std::string> flags;   // nullopt or empty: the switches are found first (best_flags)
    std::filesystem::path work;
    std::size_t limit = 0;              // repair only the first N functions (0: all)
    Log log;
    Record record;
};

// Repair every function of a module, on the threads of this process (one work folder each), and record the variants.
Counts run(const Module& module, const Compiler& cc, const RunOptions& options);

// ---- testing the search on known sources -------------------------------------------------------------------------

// A period compiler's unit: a source known to be the authored one, and the bytes it compiled to.
struct Unit {
    std::string name;
    std::string source;
    Code original;
};
// What build/period's compiler profile gives: the units, a compile of a text (in a work folder) and the code's processor.
struct Profile {
    Isa isa = Isa::Unknown;
    std::function<std::vector<Unit>()> units;
    std::function<std::optional<Code>(std::string_view text, const std::filesystem::path& work)> compile;
    Distance distance;
};

struct TruthCounts {
    std::size_t units = 0, identical = 0, knocked = 0, brought_back = 0;
    bool operator==(const TruthCounts&) const = default;
};

// Test the search on sources KNOWN to be the authored ones: each of a period compiler's units compiles to the original's
// bytes. Each is knocked away from the original by one knob and by two (a rewrite that keeps its meaning but not its
// bytes) and the search must bring it back.
TruthCounts truth(const Profile& profile, const std::filesystem::path& work, std::size_t limit = 0, const Log& log = {});

// ---- the command line --------------------------------------------------------------------------------------------

struct Arguments {
    bool truth = false;
    std::vector<std::string> truth_args;   // the compiler and the target
    std::string module;                    // a corpus name or a path
    std::optional<std::string> flags;
    std::size_t first = 0;
};
inline constexpr std::string_view kUsage = "codebreaker repair <module> [--flags \"/O2 /Gz\"] [--first N]\n"
                                           "codebreaker repair --truth <compiler> [target]\n"
                                           "\n"
                                           "Move each function's reconstruction toward the authored bytes with behaviour-preserving\n"
                                           "rewrites, compiled with a period compiler. Records the variants in the store.";
// nullopt: not a usage the command knows (print kUsage, exit 2).
std::optional<Arguments> parse_arguments(const std::vector<std::string>& args);

// "<name>: N functions repaired toward the original's bytes: A identical, B same size, C different"
std::string summary_line(std::string_view module_name, const Counts& counts);

}  // namespace cb::verify::authored::repair
