// codebreaker/verify/concolic.hpp -- typed C++ port of codebreaker.verify.concolic.
//
// concolic.py -- inputs for every path of the original that its conditions let an input reach.
//
//     Exploration<V> got = explore<V>(run, seeds, derive, own, budget, ...);
//     got.kept       inputs, each taking a path of the original no input before it took
//     got.paths      how many distinct paths (of the function's own jumps) the runs took
//     got.complete   True when those are all the paths there are (see below)
//
// verify/guided.py guesses: it watches each comparison before a jump and tries the value compared
// with in the places the input could hold it. This one knows: each run of the original is followed
// by its symbolic shadow (emulate/shadow.py: the decompiler's own p-code of each instruction,
// evaluated over the inputs), so the run hands back the conditions its path went by, as
// expressions over the inputs (arguments, bytes of memory, what an unmodelled Windows call
// returns). One condition turned the other way, with all those before it kept, is a question for
// an SMT solver (z3): an input that answers it takes the same way to that jump and then the other
// way -- a new path, and often a jump direction no input took yet. Asked again of each new run,
// this walks the tree of the function's paths (generational search):
//
//   * the jumps whose other side no run has reached are asked first, those of the function's own
//     code before those of what it calls, then the rest (another path through the same code), and
//     last a string instruction's count (`rep movsb` as many times again);
//   * a condition the solver proves cannot be turned is a path that does not exist;
//   * of the inputs that turn a comparison, one at its bound is taken where there is one (what
//     was compared equal to what it was compared with, or one off it), so that a bound written one
//     off in a rebuild shows;
//   * one input is kept for each new path, to be run on both sides by the judge.
//
// `complete` is the strong claim: every condition of every path found was turned or proved
// impossible, the solver never gave up, and no run left anything unfollowed (shadow `lost`: a
// value of the inputs into something the shadow does not follow, a run cut off at the step limit,
// more conditions than are kept). Then the paths found are every path of the original for the
// inputs the explorer may set -- addresses and selectors held where the inputs put them (pinned):
// a pointer argument points where it was given. A function whose loop runs as many times as an
// input says has more paths than any budget; there `complete` is False and the paths found are a
// sample, as before.
//
// `run(v)` runs the original on input v with its shadow and returns an Observation: its "trail"
// [(site, taken)], its "path" (shadow.path), its "vars" (shadow.vars), "lost" and "cut" (whether the
// run was cut off at the step limit) -- or nullopt (not an observation). An input whose run was cut
// off is not kept: its path is counted (its conditions are asked about like any other), but it
// compares nothing to its end, and a verdict with it in would be weak for it.
// `derive(v, values, vars)` makes the input that is v with the named inputs (shadow.vars names) set
// to `values`. `own(site)`: whether a jump is the function's own (its paths are counted by those).
// `known`: {i: trail} of seeds already run without a shadow (by the judge, for their verdicts): of
// seeds that took the same path only the first is run again with one.
//
// Typing notes:
//   The input `v` is the caller's own type V (the Python's is a dict of the lane's making; the
//   judge's lanes each have their own). explore() is a template over V: it copies a V, hands it to
//   run() by reference (a run may note things in it, as the Python's do -- the kept inputs show
//   them) and never looks inside.
//   A shadow's "vars" value is (kind, where, bits, value): "arg" or "mem" with its index or address,
//   the input's width in bits and the value it had in the run.
//   A model's value is read as the bits z3 gives, up to 64: a 64-bit value of 2**63 or more is
//   held as its two's-complement pattern in the int64_t (the Python's int would be positive), and
//   an input wider than 64 bits is outside int64 (z3's numeral throws).
//   z3's `rlimit` is an unsigned 32-bit number in the C++ binding: `steps` above 2**32-1 is clamped.
#pragma once

#include <algorithm>
#include <cstdint>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

#include <z3++.h>

#include "py.hpp"
#include "codebreaker/emulate/symbolic.hpp"

namespace codebreaker::verify::concolic {

// The solver's limits are counted in its own steps (z3's rlimit), not in time: the same function
// is then explored the same way on any machine, however busy -- a verdict must not depend on the
// load (with a time limit, one run of KEYBOARD's judge found an input another did not).
constexpr int64_t SOLVER_STEPS = 2000000;     // one question's
constexpr int64_t QUESTIONS = 256;            // questions asked of one function at most

using Step = std::tuple<int64_t, bool>;       // (site, taken)
using Trail = py::list<Step>;
// One condition a run's path went by (shadow.path's (site, taken, cond, kind) tuple).
// kind: "jump", "inner" (a string instruction's count), "pin" (an address or selector held where
// the inputs put it).
struct PathEntry {
    int64_t site = 0;
    bool taken = false;
    emulate::symbolic::ExprP cond;
    py::str kind;
};
// shadow.vars: name -> (kind, where, bits, value)
using Vars = py::dict<py::str, std::tuple<py::str, int64_t, int64_t, int64_t>>;
// the values the solver gives the named inputs
using Values = py::dict<py::str, int64_t>;

// What run(v) hands back: {"trail", "path", "vars", "lost", "cut"}.
struct Observation {
    Trail trail;
    py::list<PathEntry> path;
    Vars vars;
    py::set<py::str> lost;                   // (absent in the Python's dict: empty)
    bool cut = false;                        // (o.get("cut"))
};

// The counts and sets of an exploration (everything of the Python's Exploration but the inputs).
struct ExplorationStats {
    int64_t runs = 0, queries = 0, unsat = 0, unknown = 0;
    int64_t paths = 0;
    py::set<Step> edges;
    py::set<py::str> lost;
    int64_t missed = 0;
    bool complete = false;
    int64_t pins = 0;
    bool budget_spent = false;
    int64_t guessed = 0;                     // inputs a lane's guessing added after (lanes/win16.py)

    // One line for a verdict's notes.
    py::str summary() const;
};

template <class V>
struct Exploration : ExplorationStats {
    py::list<V> kept;                        // inputs, each taking a path no input before it took
};

// the conditions a question may turn: a jump's, a string instruction's
extern const std::tuple<py::str, py::str> TURNED;

// The solver's term of entry's condition as the run went (taken), made once (`memo`).
z3::expr _as_went(const PathEntry& entry, emulate::symbolic::Z3Memo& memo, z3::context& ctx);
// term asserted in solver s (the binding's own call: its checks cost more than the solving).
void _assert(z3::solver& s, const z3::expr& term);
// For a comparison, the inputs at its bound: what was compared equal to what it was compared
// with, one above, one below (a zero test: the value 1) -- as z3 conditions to try in turn.
py::list<z3::expr> _edges(const emulate::symbolic::ExprP& cond, emulate::symbolic::Z3Memo& memo, z3::context& ctx);
// {name: value} of a model's constants (the Python's `{d.name(): m[d].as_long() for d in m.decls()}`)
Values _model_values(const z3::model& m);
// (a function of the module's, written once for the template: the pop order of the queue)
bool _turned(const py::str& kind);

namespace detail {
struct QItem {
    int64_t pri, order, idx, j;
    Trail node;
};
}  // namespace detail

template <class V>
Exploration<V> explore(
    const std::function<std::optional<Observation>(V&)>& run, const py::list<V>& seeds,
    const std::function<std::optional<V>(const V&, const Values&, const Vars&)>& derive,
    const std::function<bool(int64_t)>& own = [](int64_t) { return true; }, int64_t budget = 24, int64_t keep = 24,
    const py::dict<int64_t, Trail>& known = py::dict<int64_t, Trail>(), int64_t steps = SOLVER_STEPS,
    int64_t questions = QUESTIONS) {
    Exploration<V> got;
    py::set<Step> covered;                   // (site, taken) of the function's own jumps, any run
    py::set<Step> other;                     // ... of the code it calls
    std::set<Trail> trails;                  // the paths: own jumps in order
    std::set<Trail> explored;                // every prefix of decisions some run took
    std::vector<std::pair<V, Observation>> pool;   // (input, observation)
    std::vector<detail::QItem> queue;        // a heap: the smallest (priority, order) first
    int64_t order = 0;
    auto later = [](const detail::QItem& a, const detail::QItem& b) {
        return std::tie(a.pri, a.order) > std::tie(b.pri, b.order);
    };
    // (in a z3 context of this exploration's own: z3's main context keeps every term made in it
    // and what it learnt, and its answers -- which input a question gets, whether the step limit
    // runs out -- then depended on what was explored before in the same process. A function's
    // verdict changed with the functions a worker had judged before it: LZEXPAND's 4:015c)
    z3::context ctx;
    emulate::symbolic::Z3Memo memo;          // the solver's terms of the conditions, made once
    std::optional<z3::model> m;              // the last question's model, and edge, the last bound tried
    std::optional<z3::expr> edge;
    std::unordered_map<int64_t, std::pair<z3::solver, int64_t>> solvers;   // run -> (its solver, how many of its conditions it holds)

    // A run's path recorded; -> whether it is a new one.
    auto took = [&](const V& v, const Observation& o) -> bool {
        Trail trail;
        for (const Step& t : o.trail) {
            if (own(std::get<0>(t))) trail.append(t);
        }
        bool is_new = trails.count(trail) == 0;
        trails.insert(trail);
        for (const Step& t : o.trail) {
            (own(std::get<0>(t)) ? covered : other).add(t);
        }
        Trail dec;
        for (const PathEntry& e : o.path) {
            if (_turned(e.kind)) dec.append(Step{e.site, e.taken});
        }
        for (int64_t k = 0; k < py::len(dec); ++k) {
            explored.insert(dec.slice(0, k + 1));
        }
        for (const py::str& s : o.lost) got.lost.add(s);
        for (const PathEntry& e : o.path) {
            if (e.kind == py::str("pin")) got.pins += 1;
        }
        pool.emplace_back(v, o);
        int64_t idx = static_cast<int64_t>(pool.size()) - 1;
        int64_t k = 0;
        int64_t j = 0;
        for (const PathEntry& e : o.path) {
            int64_t jj = j++;
            if (!_turned(e.kind)) continue;
            Step flip{e.site, !e.taken};
            Trail node = dec.slice(0, k) + Trail{flip};
            k += 1;
            if (explored.count(node)) continue;
            // (a string instruction's count last: another number of passes of the same code)
            int64_t pri = e.kind == py::str("inner")                       ? 3
                          : own(e.site) && !covered.contains(flip)          ? 0
                          : !(covered.contains(flip) || other.contains(flip)) ? 1
                                                                            : 2;
            order += 1;
            queue.push_back(detail::QItem{pri, order, idx, jj, node});
            std::push_heap(queue.begin(), queue.end(), later);
        }
        return is_new;
    };

    auto attempt = [&](V& v) -> std::optional<Observation> {
        if (got.runs >= budget) {
            got.budget_spent = true;
            return std::nullopt;
        }
        got.runs += 1;
        return run(v);
    };

    // the seeds: of those already run, one per path they took; the others each run
    std::map<Trail, int64_t> by_trail;
    for (int64_t i = 0; i < py::len(seeds); ++i) {
        V& v = seeds[i];
        std::optional<Trail> t = known.get(i);
        if (t) {
            Trail kept_steps;
            for (const Step& x : *t) {
                if (own(std::get<0>(x))) kept_steps.append(x);
            }
            if (by_trail.count(kept_steps)) continue;
            by_trail[kept_steps] = i;
        }
        std::optional<Observation> o = attempt(v);
        if (o) took(v, *o);
    }
    while (!queue.empty() && py::len(got.kept) < keep) {
        std::pop_heap(queue.begin(), queue.end(), later);
        detail::QItem item = queue.back();
        queue.pop_back();
        int64_t pri = item.pri, idx = item.idx, j = item.j;
        if (explored.count(item.node)) continue;
        // (a reference into the pool would not survive its growing: a copy of the run's input and path)
        const Observation o = pool[static_cast<size_t>(idx)].second;
        const V v = pool[static_cast<size_t>(idx)].first;
        const PathEntry e = o.path[j];
        if (pri == 0 && covered.contains(Step{e.site, !e.taken})) {     // (reached meanwhile: now only another path)
            order += 1;
            queue.push_back(detail::QItem{2, order, idx, j, item.node});
            std::push_heap(queue.begin(), queue.end(), later);
            continue;
        }
        if (got.runs >= budget || got.queries >= questions) {
            got.budget_spent = true;
            break;
        }
        // (one solver per run, its path's conditions asserted as far as asked: the questions of
        // one run come mostly in the order of its path, each one condition further on)
        std::optional<z3::solver> s;
        int64_t k = 0;
        auto found = solvers.find(idx);
        if (found != solvers.end()) {
            s = found->second.first;
            k = found->second.second;
        }
        if (!s || k > j) {
            s = z3::solver(ctx);
            s->set("rlimit", static_cast<unsigned>(std::min<int64_t>(steps, 0xFFFFFFFFLL)));
            k = 0;
        }
        for (int64_t p = k; p < j; ++p) {
            _assert(*s, _as_went(o.path[p], memo, ctx));
        }
        solvers.erase(idx);                                          // (the replaced solver goes only now)
        solvers.emplace(idx, std::make_pair(*s, j));
        s->push();
        _assert(*s, !_as_went(e, memo, ctx));
        got.queries += 1;
        z3::check_result r = s->check();
        // (m and edge are kept until the next question replaces them, as the Python's locals are:
        // z3 hands a freed term's number to the next term made, and which model it finds can
        // depend on the numbers, so the terms must live as long as the Python's)
        if (r == z3::sat) {
            m = s->get_model();
        } else {
            m.reset();
        }
        if (m) {
            // (and on the edge: of the inputs that turn it, one where what was compared is what
            // it was compared with, or one past -- a bound written one off is seen there)
            for (const z3::expr& e2 : _edges(e.cond, memo, ctx)) {
                edge = e2;
                s->push();
                _assert(*s, e2);
                if (s->check() == z3::sat) {
                    m = s->get_model();
                    s->pop();
                    break;
                }
                s->pop();
            }
        }
        s->pop();
        explored.insert(item.node);                                  // (asked: whatever the answer)
        if (r == z3::unsat) {
            got.unsat += 1;
            continue;
        }
        if (r != z3::sat) {
            got.unknown += 1;
            continue;
        }
        Values values = _model_values(*m);
        std::optional<V> w = derive(v, values, o.vars);
        if (!w) {
            got.missed += 1;
            continue;
        }
        std::optional<Observation> o2 = attempt(*w);
        if (!o2) {
            got.missed += 1;
            continue;
        }
        Trail dec2;
        for (const PathEntry& p : o2->path) {
            if (_turned(p.kind)) dec2.append(Step{p.site, p.taken});
        }
        if (dec2.slice(0, py::len(item.node)) != item.node) {        // (the run did not go where asked)
            got.missed += 1;
        }
        if (took(*w, *o2) && !o2->cut) {
            got.kept.append(*w);
        }
    }
    got.paths = static_cast<int64_t>(trails.size());
    got.edges = covered;
    bool queued_all_explored = true;
    for (const detail::QItem& q : queue) {
        if (!explored.count(q.node)) queued_all_explored = false;
    }
    got.complete = (queue.empty() || queued_all_explored) && !got.budget_spent && !got.unknown && !got.missed &&
                   py::len(got.lost) == 0 && py::len(got.kept) < keep && !pool.empty();
    return got;
}

}  // namespace codebreaker::verify::concolic
