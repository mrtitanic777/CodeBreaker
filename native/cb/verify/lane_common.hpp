// cb/verify/lane_common.hpp -- what every kind's judge does the same way.
//
// A kind of module (the win32, win16, powerpc and driver lanes) knows how to run its code -- its emulator, its
// calling convention, which addresses mean what -- and turns a run (the one record every emulator returns,
// emulate/run.hpp) into a fingerprint for verify/compare. What is left over is the same for all of them, and lives
// here so that it stays the same:
//
//     the switches     CB_NO_FP_CACHE, CB_NO_GUIDED, CB_NO_CONCOLIC and CB_PLAIN, read in one place
//     remembered()     a fingerprint kept across runs, under a key naming everything the run depends on
//     call(), calls()  a recorded call as the text two builds of the same code agree on
//     argument()       one argument of such a call
//     writes()         the memory a run left changed, as the digest a fingerprint carries
//     fingerprint()    what compare compares, made from an emulator's run: how it ended, the result, the calls, the
//                      memory, the stack, the saved registers
//     changed(), left() the memory a run says it left changed, as a lane names its places
//     the inputs       the argument patterns the kinds' tables of inputs are made of
//     guided()         branch-guided inputs (verify/guided), with the limits every kind uses
//     explored()       the same found path by path (verify/concolic), where the kind can follow a run's values
//                      symbolically; and the limits a verdict then carries
//     judged()         one function's verdict, whatever goes wrong while reaching it
//
// Nothing here is global state: the fingerprint store is handed to remembered(), the switches are read from the
// environment when asked.
#pragma once

#include <compare>
#include <cstdint>
#include <functional>
#include <map>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "cb/base/cache.hpp"
#include "cb/emulate/run.hpp"
#include "cb/verify/compare.hpp"
#include "cb/verify/concolic.hpp"
#include "cb/verify/guided.hpp"
#include "cb/verify/verdicts.hpp"

namespace cb::verify::lane_common {

// ---- the switches ---------------------------------------------------------------------------------------------------

// False when CB_NO_FP_CACHE is set (and not empty): every run is made afresh, none is kept (the test suites run so,
// since a verdict must then reflect the code as it is).
bool fp_cache_on();
// False when CB_NO_GUIDED is set: no branch-guided inputs (to measure what they add).
bool guided_on();
// False when CB_NO_CONCOLIC is set: the branch-guided inputs are guessed (verify/guided) rather than solved for along
// each path (verify/concolic) -- to compare the two.
bool concolic_on();
// CB_PLAIN=1: one instruction at a time on a fresh machine each run -- the slow way the fast one must agree with (the
// 32-bit emulator reads it when a judge is made).
bool plain();

// ---- fingerprints kept across runs ----------------------------------------------------------------------------------

// Where fingerprints are kept: a lookup and a store, by key. store_of() is the process's cache.
struct FingerprintStore {
    std::function<std::optional<compare::Fingerprint>(std::string_view key)> get;
    std::function<void(std::string_view key, const compare::Fingerprint&)> put;
};
FingerprintStore store_of(base::cache::Cache& cache);

// run()'s fingerprint, or the one it gave before under `key`.
//
// A run is a pure function of what its key names -- the module's content, the function, the input, whatever else the
// kind varies -- so a fingerprint made once can be used again: by the next judgement of the same original, by the next
// sweep. No key (the module has no content hash) never keeps anything. A run that was not an observation (`err`) is
// never kept either, nor one `keep(fp)` turns down.
compare::Fingerprint remembered(const FingerprintStore& store, const std::optional<std::string>& key,
                                const std::function<compare::Fingerprint()>& run,
                                const std::function<bool(const compare::Fingerprint&)>& keep = nullptr);

// ---- calls, as two builds agree on them -----------------------------------------------------------------------------

// One call as a fingerprint records it: `name(a,b,...)`, or just `name` when how many arguments it takes is not known
// -- compare counts those (argc-unknown), since two calls alike by name alone may still differ in what they were given.
std::string call(std::string_view name, const std::optional<std::vector<std::string>>& args = std::nullopt);

// A recorded call: its name (none for one that has none) and its arguments (none when their number is not known).
struct Called {
    std::optional<std::string> name;
    std::optional<std::vector<std::int64_t>> args;
};
// The calls of a run.
std::vector<Called> called(const emulate::Run& run);

// The calls a run made, in order, as a fingerprint records them. Each name goes through `name` (default: the name,
// "?" for a call that has none) and each argument through `argument(value)`. Calls `skip(name)` says are not the
// function's own are left out. Past `cap` calls the rest are one "..." (compare: calls-capped).
std::vector<std::string> calls(std::span<const Called> seq, const std::function<std::string(std::int64_t)>& argument,
                               const std::function<std::string(const std::optional<std::string>&)>& name = nullptr,
                               const std::function<bool(const std::optional<std::string>&)>& skip = nullptr,
                               std::optional<std::size_t> cap = std::nullopt);

// A range of addresses, low inclusive and high exclusive.
struct AddressRange {
    std::int64_t low = 0;
    std::int64_t high = 0;
    bool contains(std::int64_t v) const { return low <= v && v < high; }
};
// Which addresses are this build's own image: a range, a test, or (monostate) nothing.
using ImageTest = std::variant<std::monostate, AddressRange, std::function<bool(std::int64_t)>>;

// What argument() knows about the call it shows.
struct ArgumentContext {
    std::vector<std::int64_t> inputs;                  // the function's own inputs
    std::optional<std::int64_t> above;                 // an input counts as one only when it is above this
    bool small = false;                                // show a small constant as a signed number
    std::vector<std::optional<AddressRange>> frame;    // ranges of "somewhere in this call's frame" (an unset one is skipped)
    ImageTest image;
};

// One argument value in a form two builds of the same code agree on. In this order:
//
//     "a2"     it is one of the function's own inputs (the third), when it is above `above`
//              (a small input is as likely to be a constant that happens to equal it)
//     "-1"     with `small`, a small constant, as a signed number
//     "S"      an address in one of the `frame` ranges: the stack, or anything else that is
//              "somewhere in this call's frame" and lies elsewhere in each build
//     "P"      an address in this build's own image, where the two builds do not have the same layout
//     "1f3a"   otherwise the value itself
//
// A kind with forms of its own (a selector, the contents of what a pointer points at) tests for those first and hands
// the rest to this.
std::string argument(std::int64_t v, const ArgumentContext& context = {});

// ---- memory a run left changed --------------------------------------------------------------------------------------

// A byte of memory a lane names by its address, or by a tag and an offset in that tag's region.
struct TaggedPlace {
    std::string tag;
    std::int64_t offset = 0;
    friend auto operator<=>(const TaggedPlace&, const TaggedPlace&) = default;
};
using Place = std::variant<std::uint64_t, TaggedPlace>;
// What a place held at the end: a byte, or the token standing for the whole word it was part of ("S", a stack address).
using Held = std::variant<std::uint8_t, std::string>;
// Sorted by place (addresses before tagged places): the digest depends on it.
using State = std::map<Place, Held>;

// The memory a run left changed -- the places that ended up with a value other than the one they had -- as a
// fingerprint carries it: how many, and a digest of which and what. Empty when nothing changed. The digest's layout is
// the one the fingerprints already in the cache were made with.
std::string writes(const State& state);

// The ways a lane names a write's bytes: leave the write out, name its first byte (the next ones follow it), or name
// each byte (i -> its place, or none for a byte to leave out).
struct Skip {};
using ByteNamer = std::function<std::optional<Place>(std::size_t i)>;
using Where = std::variant<Skip, Place, ByteNamer>;

struct ChangedOptions {
    // Asked of each write's first byte; unset: the address itself.
    std::function<Where(std::uint64_t address)> place;
    // What a 4-byte value stands for in every one of its bytes when it is not a number to compare (a stack address:
    // "S"), else none.
    std::function<std::optional<std::string>(std::uint64_t value)> word;
    std::size_t most = 8;                              // bytes of a write taken (at most 8 hold a value)
    // What a byte held before is what the first write to it found there: the first of every write (the harness's and
    // those left out too), or, when false, the first of those kept.
    bool everything = true;
};

// The bytes a run left with a value other than they held before it, as a lane names its places.
State changed(const emulate::Run& run, const ChangedOptions& options = {});

// The memory a run left changed (changed()) as a fingerprint carries it (writes()); none when the emulator recorded
// fewer writes than were made (compare: writes-capped).
std::optional<std::string> left(const emulate::Run& run, const ChangedOptions& options = {});

// ---- from a run to a fingerprint ------------------------------------------------------------------------------------

// What a lane adds to what the run itself says.
struct FingerprintParts {
    std::optional<std::int64_t> ret;                   // the result; none for a run that did not return
    std::optional<std::string> writes = std::string(); // left(); none: more than is recorded
    std::optional<bool> saved;                         // the preserved registers handed back; none: the lane does not compare them
    std::vector<std::int64_t> cov;                     // the function's own instructions run
    std::vector<std::int64_t> reach;                   // every instruction run
    std::optional<std::int64_t> steps;                 // instructions the run took, where the lane records it
};

// What compare compares, made from a run: how it ended, `deps` (its calls and effects out, as text in order: calls()),
// and the parts. The stack's move and `saved` are only for a run that returned.
compare::Fingerprint fingerprint(const emulate::Run& run, std::vector<std::string> deps, FingerprintParts parts = {});

// ---- inputs ---------------------------------------------------------------------------------------------------------
// The argument patterns the kinds' inputs are made of, as machine words. Every argument gets a DIFFERENT value
// wherever the pattern allows: with one value in every slot, a body that uses its parameters in the wrong order
// behaves the same as the right one.

// first, first + step, ...: distinct, small, increasing.
std::vector<std::uint64_t> counting(std::size_t n, std::uint64_t first = 1, std::uint64_t step = 1);
// top, top - 1, ...: large, and negative when read as signed.
std::vector<std::uint64_t> descending(std::size_t n, std::uint64_t top);

// A value, or a function of the argument's position.
using Pattern = std::variant<std::uint64_t, std::function<std::uint64_t(std::size_t)>>;
// Arguments 0, 2, 4 ... from `even` and 1, 3, 5 ... from `odd` (a pointer in one place, an integer in the next).
std::vector<std::uint64_t> alternating(std::size_t n, const Pattern& even, const Pattern& odd);

inline constexpr int kMaxGuided = 16;       // branch-guided inputs kept per function (verify/guided)
inline constexpr int kGuidedBudget = 96;    // runs of the original they may spend finding them

// Inputs that take branches of the original no seed took (guided_inputs, with these limits; `machine` says how wide a
// word is and which way round its bytes go), or none when CB_NO_GUIDED is set. `run(input)` traces the original with
// its branches logged.
std::vector<guided::Input> guided(const guided::Run& run, std::span<const guided::Input> seeds, std::uint64_t seed,
                                  guided::Machine machine = {});

inline constexpr int kPathBudget = 32;      // runs of the original the path explorer may spend (verify/concolic)
inline constexpr int kMaxPaths = 24;        // inputs it keeps, one per new path
inline constexpr int kGuidedRest = 32;      // runs then left to guessing, where the paths found are not all

// Inputs for the paths of the original no seed took (concolic::explore, with these limits): the Exploration's `kept`
// are the inputs. `steps`: the solver's limit for one question, in its own steps (0: concolic::kSolverSteps); `more`:
// runs beyond kPathBudget (for seeds whose paths are not known beforehand, each run again with its shadow); `known`:
// the trails of seeds that were run already, by seed index.
template <class V>
concolic::Exploration<V> explored(const std::function<std::optional<concolic::Observation>(V&)>& run, std::span<V> seeds,
                                  const std::function<std::optional<V>(const V&, const concolic::Values&, const concolic::Vars&)>& derive,
                                  std::function<bool(std::int64_t)> own, const std::map<std::size_t, concolic::Trail>& known = {},
                                  std::uint32_t steps = 0, int more = 0) {
    concolic::Options options;
    options.own = std::move(own);
    options.budget = kPathBudget + more;
    options.keep = kMaxPaths;
    options.steps = steps != 0 ? steps : concolic::kSolverSteps;
    return concolic::explore<V>(run, seeds, derive, options, known);
}

// What an exploration adds to a verdict's limits: `paths=N`, the distinct paths the original took on the inputs, and
// `paths-all` when those are all it has. Nothing for no exploration.
std::vector<std::string> path_limits(const concolic::Stats* got);

// ---- the verdict ----------------------------------------------------------------------------------------------------

// A function's verdict as the store holds it: the verdict, why, its limits (weak limit codes, and a lane's own such as
// `paths=N`) and the one line saying what differed.
struct Judgement {
    Verdict verdict = Verdict::Inconclusive;
    std::string reason;
    std::vector<std::string> limits;
    std::string detail;
};

// compare_one() makes the comparison and gives its Judgement. Anything that goes wrong on the way makes this function
// inconclusive -- one function must not sink the rest -- except a missing module or tool (NotFoundError): a broken
// setup is not an observation, and propagates.
Judgement judged(const std::function<Judgement()>& compare_one);

}  // namespace cb::verify::lane_common
