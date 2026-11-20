// exact/draft/prove.hpp -- a candidate held to the program: compiled (mended where the compiler stops), compared, and the
// nearest refined by a beam search over the rewrites, the kind of difference choosing which first.
//
//     draft::prove::Collaborators c;                      // what proving asks of the compiler, the comparison, the rewrites
//     c.compile = ...; c.judge = ...; c.refiners = ...;   // bound to ONE function of the program
//     draft::prove::Proof proof(settings, c, &priors, length);
//     auto [source, verdict] = proof.prove_texts(texts);  // the first reading the compiler proves identical, else the nearest
//
// What is here is the search; what it searches with comes in through `Collaborators`, bound to the one function being proved:
//   compile      a source through the project's toolchain (its headers, include files and switches): the object and the console
//   judge        the k-th function of an object held to the program's function: identical, or how it differs
//   compile_many several sources through one run of the compiler (exact/batch), when the toolchain allows
//   diagnose     what differs between the function and a candidate, by kind and source line (exact/aim): it orders the rewrites
//   refiners     the rewrites (REFINERS: each reading of a statement the model may have misspelled), named
//   candidates   the model's text made the project's candidates (draft/candidates)
// so the search is tested without a compiler, and the stage (draft/package) binds it to the project.
//
// A source is bytes (Latin-1 or CP437 text of the period's C): nothing here reads it as characters.
//
// The beam's history -- which rewrite brought a reading nearer, for which kind of difference -- is `Priors`, kept in
// state/exact/priors.json and merged with what other processes wrote meanwhile; nothing else is shared.
#pragma once

#include <compare>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <mutex>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "core/bytes.hpp"
#include "exact/aim.hpp"
#include "exact/batch.hpp"
#include "exact/draft/candidates.hpp"

namespace cb::exact::draft::prove {

// ---- verdicts ------------------------------------------------------------------------------------------------------
struct Verdict {
    enum class Kind { Identical, Differs, DoesNotCompile, RegistersLeft, NoFunction };
    Kind kind = Kind::NoFunction;
    std::uint32_t bytes = 0;                                       // Differs: how many bytes differ
    std::optional<std::pair<std::uint32_t, std::uint32_t>> length; // Differs: (length it has, length the program's has) when they are not the same
    std::string detail;                                            // DoesNotCompile: the compiler's error; RegistersLeft: the registers

    bool identical() const { return kind == Kind::Identical; }
    bool differs() const { return kind == Kind::Differs; }
    // "identical", "differs: 12 bytes, length 30 not 32", "does not compile: <error>", "registers left: ax bx", "no function compiled"
    std::string text() const;

    static Verdict same() { return {Kind::Identical, 0, std::nullopt, {}}; }
    static Verdict differing(std::uint32_t bytes, std::optional<std::pair<std::uint32_t, std::uint32_t>> length = std::nullopt) {
        return {Kind::Differs, bytes, length, {}};
    }
    static Verdict not_compiled(std::string error) { return {Kind::DoesNotCompile, 0, std::nullopt, std::move(error)}; }
    static Verdict registers_left(const std::vector<std::string>& regs);
    static Verdict no_function() { return {Kind::NoFunction, 0, std::nullopt, {}}; }
    friend bool operator==(const Verdict&, const Verdict&) = default;
};

// How near a verdict is, smaller is nearer: one that differs (the right length first, then by fewer bytes -- what is missing
// counts as differing), before one that does not compile, before one with registers left.
struct Rank {
    int cls = 2;
    int gap = 0;            // 1 when the length is off
    std::int64_t bytes = 0;
    friend auto operator<=>(const Rank&, const Rank&) = default;
};
Rank rank(const Verdict& v);

// A verdict too far from the program's code for the beam's rewrites to close (a statement missing, a loop misread): the model's
// reading is wrong, not its spelling. By the length alone: a byte too many early shifts every byte after it.
bool hopeless(const Verdict& v, std::uint32_t function_length);

// ---- mending what the compiler stops at ----------------------------------------------------------------------------
// The names a draft reads a byte at a time (a pointer to chars): the types tried first for them. (What byte_fields found.)
using BytePointers = std::set<std::string>;

// The sources mended at the line the compiler stopped at, as its error says; none when it names no mending:
//     left of `->fN' must have a struct/union type   E->fN  -> *(int *)((char *)(E) + N)
//     illegal indirection                             *X     -> X an int * (or a char *); else *(char *)X
//     subscript on non-array                          X[I]   -> X an int *; else ((int *)X)[I]
//     bad operand / 2 pointers                        a char * local an int
// (the model's expressions carry no types: what the code shows is given them -- each way, the bytes decide)
std::vector<std::string> compile_fixes(std::string_view source, std::string_view console, const BytePointers& byte_pointers = {});
// The one mending a cast makes of the line the compiler stopped at, or none.
std::optional<std::string> compile_fix(std::string_view source, std::string_view console);

// A candidate as the compiler sees it, to tell two apart: the spacing one, a doubled pair of parentheses one (neither changes a byte).
std::string canon(std::string_view source);
// A candidate's declarations -- its parameters' and locals' types, names aside -- and whether it returns a value: the kind of reading.
using DeclSig = std::pair<std::vector<std::string>, bool>;
DeclSig decl_sig(std::string_view source);
// True: the prologue calls the stack probe; false: a frame without one; none: not a frame the rule knows.
std::optional<bool> probes(ByteView code);

// ---- the beam's history ---------------------------------------------------------------------------------------------
class Priors {
public:
    // `file` is read now and merged with at every learn(); an empty path keeps the history in memory only.
    explicit Priors(std::filesystem::path file = {});
    // How often `refiner` brought a reading nearer for a difference of this kind.
    std::int64_t wins(std::string_view kind, std::string_view refiner) const;
    // A refiner brought a reading nearer, for a difference of these kinds (none: "shape"): counted and kept.
    void learn(const std::vector<std::string>& kinds, std::string_view refiner);

private:
    std::filesystem::path file_;
    mutable std::mutex mutex_;
    std::map<std::string, std::map<std::string, std::int64_t>, std::less<>> wins_;
};

// ---- what proving asks of its surroundings -------------------------------------------------------------------------
// A rewrite of a statement: its name (which kind of difference asks for it) and the sources it makes of one.
struct Refiner {
    std::string name;
    std::function<std::vector<std::string>(const std::string& source)> rewrite;
};

struct Collaborators {
    std::function<batch::CompileOutcome(std::string_view source)> compile;
    // The k-th function of the object, held to the function being proved.
    std::function<Verdict(ByteView object, std::size_t k)> judge;
    // {index in sources: (the object, the function's index in it)} of those a run of the compiler made together; empty/absent:
    // the toolchain cannot. A run counts as two compiles.
    std::function<std::map<std::size_t, std::pair<Bytes, std::size_t>>(const std::vector<std::string>& sources)> compile_many;
    // What differs between the program's function and a source's compile (one compile more, with the listing).
    std::function<std::optional<aim::Diagnosis>(std::string_view source)> diagnose;
    std::vector<Refiner> refiners;
    // A source where a number that is a string's place is the string.
    std::function<std::vector<std::string>(std::string_view source)> string_variants;
    // The model's text made the project's candidates (draft/candidates), for these register parameters.
    std::function<std::vector<candidates::Candidate>(const std::string& text, const candidates::RegisterParams& regparam)> candidates;
    // The names a draft reads a byte at a time, for these register parameters' text.
    std::function<BytePointers()> byte_pointers;
};

// The knobs of a search, and what the profile says of switches.
struct Settings {
    std::size_t max_candidates = 64;     // (a bench must end)
    std::int64_t function_budget = 150;  // compiles a function's candidates may take in all
    std::size_t batch = 16;              // candidates compiled in one run (0: none)
    std::size_t beam = 4;                // readings the search carries on with each round
    std::int64_t refine_budget = 240;    // compiles one function's refining may spend
    bool aim = true;                     // the beam aimed by the differences
    std::pair<double, double> hopeless{0.25, 0.6};
    // switches: a source's first line gives its own (profile "file_switches", NAME in /* NAME: ... */)
    std::optional<std::string> file_switches;
    std::string switches;                // the project's, else the profile's
    std::optional<std::string> fp_switch, probe_switch;   // profile "fp_switch", "probe_switch"
    bool fp = false;                     // the function's code is the 8087's
    std::optional<bool> probes;          // the function's prologue calls the stack probe (see probes())
};

// A reading and how near it is.
struct Reading {
    Rank rank;
    std::string source;
    Verdict verdict;
};
struct Outcome {
    std::string source;     // the source the verdict is of
    Verdict verdict;
};

// ---- the search ----------------------------------------------------------------------------------------------------
// One function's proving. Not shared between threads (it counts the compiles it has spent).
class Proof {
public:
    Proof(Settings settings, Collaborators collaborators, Priors* priors, std::uint32_t function_length);

    // The verdict on `source` -- mended where the compiler says it cannot be compiled, each way the error offers, and again (a few
    // rounds, a few compiles): the best of what compiles. Outcome::source is the source the verdict is of.
    Outcome prove(const std::string& source, int tries = 4, int budget = 10);

    // Of the model's texts [(text, register parameters)] and their variants, the first the compiler proves identical; else the
    // best of them (one that differs before one that does not compile, before one with registers left). A register loaded from a
    // parameter and then given a new value is maybe a local the parameter started: the text is tried both ways.
    Outcome prove_texts(const std::vector<std::pair<std::string, candidates::RegisterParams>>& texts);

    // Of the model's text and its variants, the first the compiler proves identical; else the nearest, refined.
    Outcome prove_best(const std::string& text, const candidates::RegisterParams& regparam);

    // A beam search over the refiners' rewritings: the beam's nearest readings (the best and the other seeds) each rewritten every
    // way, the nearest of all kept, again, until one is identical, no round brings any nearer, or the budget is spent.
    Outcome refine(const Reading& best, int rounds = 6, std::optional<std::int64_t> budget = std::nullopt, const std::vector<Reading>& seeds = {});

    // Compiles spent on the function so far.
    std::int64_t spent() const { return spent_; }

private:
    std::map<std::string, Verdict> batched(const std::vector<std::string>& sources);
    std::optional<aim::Diagnosis> aimed(const std::string& source);
    // the refiners in the order a reading's difference asks for (each with the source lines to keep to, when only statements
    // differ), and the kinds diagnosed
    struct Ordered {
        std::vector<std::pair<const Refiner*, std::optional<std::vector<std::uint32_t>>>> list;
        std::vector<std::string> kinds;
    };
    Ordered ordered(const std::string& source);

    Settings settings_;
    Collaborators c_;
    Priors* priors_;
    std::uint32_t length_;
    std::int64_t spent_ = 0;
    std::map<std::string, std::optional<aim::Diagnosis>> aims_;   // by source: one compile each
};

// The source with the first line that gives its own switches (`/* NAME: -Ox -Gs */`), or `source` when the function needs none
// beyond the project's: the 8087's switch when the code is the 8087's, and the one that leaves the stack probe out of a prologue
// when the prologue shows none (and leaves it in when it shows one).
std::string with_switches(std::string_view source, const Settings& settings);

}  // namespace cb::exact::draft::prove
