// cb/verify/concolic.hpp -- inputs for every path of the original that its conditions let an input reach.
//
//     Exploration<Input> got = explore<Input>(run, seeds, derive, {.own = is_own_jump});
//     got.kept       inputs, each taking a path of the original no input before it took
//     got.paths      how many distinct paths (of the function's own jumps) the runs took
//     got.complete   true when those are all the paths there are (see below)
//
// verify/guided guesses: it watches each comparison before a jump and tries the value compared with in the places the
// input could hold it. This one knows: each run of the original is followed by its symbolic shadow (the decompiler's
// own p-code of each instruction, evaluated over the inputs), so the run hands back the conditions its path went by,
// as expressions over the inputs (arguments, bytes of memory, what an unmodelled Windows call returns). One condition
// turned the other way, with all those before it kept, is a question for an SMT solver (Z3): an input that answers it
// takes the same way to that jump and then the other way -- a new path, and often a jump direction no input took yet.
// Asked again of each new run, this walks the tree of the function's paths (generational search):
//
//   * the jumps whose other side no run has reached are asked first, those of the function's own code before those of
//     what it calls, then the rest (another path through the same code), and last a string instruction's count
//     (`rep movsb` as many times again);
//   * a condition the solver proves cannot be turned is a path that does not exist;
//   * of the inputs that turn a comparison, one at its bound is taken where there is one (what was compared equal to
//     what it was compared with, or one off it), so that a bound written one off in a rebuild shows;
//   * one input is kept for each new path, to be run on both sides by the judge.
//
// `complete` is the strong claim: every condition of every path found was turned or proved impossible, the solver
// never gave up, and no run left anything unfollowed (the shadow's `lost`: a value of the inputs into something the
// shadow does not follow, a run cut off at the step limit, more conditions than are kept). Then the paths found are
// every path of the original for the inputs the explorer may set -- addresses and selectors held where the inputs put
// them (pinned): a pointer argument points where it was given. A function whose loop runs as many times as an input
// says has more paths than any budget; there `complete` is false and the paths found are a sample.
//
// The lane gives three things. `run(v)` runs the original on input `v` with its shadow and returns an Observation, or
// nothing when it was not an observation; a run may note things in `v` (the kept inputs show them). An input whose run
// was cut off is not kept: its path is counted (its conditions are asked about like any other), but it compares nothing
// to its end, and a verdict with it in would be weak for it. `derive(v, values, vars)` makes the input that is `v`
// with the named inputs set to `values`. `own(site)` says whether a jump is the function's own (its paths are counted
// by those). `known` holds the trails of seeds already run without a shadow (by the judge, for their verdicts): of
// seeds that took the same path only the first is run again with one.
//
// The solver's limits are counted in its own steps (Z3's rlimit), not in time: the same function is then explored the
// same way on any machine, however busy -- a verdict must not depend on the load. Each exploration has a Z3 context of
// its own, so what it finds does not depend on what was explored before in the same process.
#pragma once

#include <cstdint>
#include <deque>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <vector>

#include "cb/emulate/symbolic.hpp"

namespace cb::verify::concolic {

inline constexpr std::uint32_t kSolverSteps = 2'000'000;   // one question's
inline constexpr int kQuestions = 256;                     // questions asked of one function at most

// One decision of a run: the jump at `site`, taken or not.
struct Step {
    std::int64_t site = 0;
    bool taken = false;
    friend auto operator<=>(const Step&, const Step&) = default;
};
using Trail = std::vector<Step>;

// What a condition on a path is.
enum class Kind {
    Jump,    // a conditional jump
    Inner,   // a string instruction's count
    Pin,     // an address or selector held where the inputs put it
};

// One condition a run's path went by, in order. The expression belongs to the pool of the run's shadow, which must
// outlive the exploration.
struct PathEntry {
    std::int64_t site = 0;
    bool taken = false;
    const emulate::symbolic::Expr* cond = nullptr;
    Kind kind = Kind::Jump;
};

// An input the shadow follows: an argument (`where` is its index) or bytes of memory (`where` is the address), its
// width, and the value it had in the run.
struct InputVar {
    enum class Place { Arg, Mem };
    Place place = Place::Arg;
    std::int64_t where = 0;
    std::uint32_t bits = 0;
    std::int64_t value = 0;
};
using Vars = std::map<std::string, InputVar>;     // by name
// The values the solver gives the named inputs. An input wider than 64 bits is not given (the explorer sets at most 64).
using Values = std::map<std::string, std::uint64_t>;

struct Observation {
    Trail trail;                            // the path of the jumps (all of them, the function's own and its callees')
    std::vector<PathEntry> path;
    Vars vars;
    std::set<std::string> lost;             // what the shadow could not follow
    bool cut = false;                       // the run was cut off at the step limit
};

struct Options {
    std::function<bool(std::int64_t)> own;  // whether a jump is the function's own (empty: all are)
    int budget = 24;                        // runs at most
    int keep = 24;                          // inputs kept at most
    std::uint32_t steps = kSolverSteps;     // the solver's step limit per question
    int questions = kQuestions;
};

// The counts and sets of an exploration.
struct Stats {
    int runs = 0, queries = 0, unsat = 0, unknown = 0;
    int paths = 0;
    std::set<Step> edges;                   // the function's own jumps, each way any run took
    std::set<std::string> lost;
    int missed = 0;                         // questions answered whose input could not be made or did not go where asked
    int pins = 0;
    bool complete = false;
    bool budget_spent = false;
    int guessed = 0;                        // inputs a lane's guessing added after (lanes/win16)

    // One line for a verdict's notes: "all 4 paths, 5 runs, 6 questions (2 impossible)" or "4 paths (sampled), ...".
    std::string summary() const;
};

template <class V>
struct Exploration : Stats {
    std::vector<V> kept;                    // inputs, each taking a path no input before it took
};

// ---- the search, over inputs numbered in the order they come (seeds first) ---------------------------------------------

struct Hooks {
    std::function<std::optional<Observation>(std::size_t input)> run;
    // The new input made from `input` with the values set: its number, or nothing when it could not be made.
    std::function<std::optional<std::size_t>(std::size_t input, const Values&, const Vars&)> derive;
};

struct IdExploration {
    Stats stats;
    std::vector<std::size_t> kept;
};

IdExploration explore_ids(const Hooks& hooks, std::size_t seeds, const std::map<std::size_t, Trail>& known, const Options& options);

// ---- the search, over a lane's own input type ----------------------------------------------------------------------------

// `seeds` are run in place (a run may note things in them); derived inputs are copies the exploration owns until it
// returns them in `kept`.
template <class V>
Exploration<V> explore(const std::function<std::optional<Observation>(V&)>& run, std::span<V> seeds,
                       const std::function<std::optional<V>(const V&, const Values&, const Vars&)>& derive,
                       const Options& options = {}, const std::map<std::size_t, Trail>& known = {}) {
    std::deque<V> made;                      // the derived inputs (a deque: references stay put as it grows)
    auto input = [&](std::size_t i) -> V& { return i < seeds.size() ? seeds[i] : made[i - seeds.size()]; };
    Hooks hooks;
    hooks.run = [&](std::size_t i) { return run(input(i)); };
    hooks.derive = [&](std::size_t i, const Values& values, const Vars& vars) -> std::optional<std::size_t> {
        std::optional<V> w = derive(input(i), values, vars);
        if (!w) return std::nullopt;
        made.push_back(std::move(*w));
        return seeds.size() + made.size() - 1;
    };
    IdExploration got = explore_ids(hooks, seeds.size(), known, options);
    Exploration<V> out;
    static_cast<Stats&>(out) = std::move(got.stats);
    for (std::size_t i : got.kept) out.kept.push_back(input(i));
    return out;
}

}  // namespace cb::verify::concolic
