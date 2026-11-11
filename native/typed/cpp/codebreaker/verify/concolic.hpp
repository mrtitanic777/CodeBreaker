// codebreaker/verify/concolic.hpp -- COMPAT SHIM over cb/verify/concolic.hpp (docs/CPP_DESIGN.md 6.3), for the old
// verify/lanes/driver, verify/lanes/win16 and verify/gate16/isolated16 ports. Deleted with its last py:: consumer.
//
// The explorer (the search, the solver questions, the stopping rules) is native/cb/verify/concolic.hpp. Here: the old
// record shapes (py:: containers, a condition's kind as its word, the expressions of the old symbolic shim) converted in
// and out around it; nothing else.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>
#include <vector>

#include "cb/compat/py_bridge.hpp"
#include "cb/verify/concolic.hpp"
#include "codebreaker/emulate/symbolic.hpp"
#include "py.hpp"

namespace codebreaker::verify::concolic {

namespace ns = ::cb::verify::concolic;

constexpr int64_t SOLVER_STEPS = ns::kSolverSteps;   // one question's
constexpr int64_t QUESTIONS = ns::kQuestions;        // questions asked of one function at most

using Step = std::tuple<int64_t, bool>;              // (site, taken)
using Trail = py::list<Step>;
// One condition a run's path went by. kind: "jump", "inner" (a string instruction's count), "pin" (an address or
// selector held where the inputs put it).
struct PathEntry {
    int64_t site = 0;
    bool taken = false;
    emulate::symbolic::ExprP cond;
    py::str kind;
};
// shadow.vars: name -> (kind, where, bits, value), kind "arg" or "mem"
using Vars = py::dict<py::str, std::tuple<py::str, int64_t, int64_t, int64_t>>;
// the values the solver gives the named inputs
using Values = py::dict<py::str, int64_t>;

struct Observation {
    Trail trail;
    py::list<PathEntry> path;
    Vars vars;
    py::set<py::str> lost;
    bool cut = false;
};

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
    py::str summary() const {
        ns::Stats s;
        s.runs = static_cast<int>(runs);
        s.queries = static_cast<int>(queries);
        s.unsat = static_cast<int>(unsat);
        s.paths = static_cast<int>(paths);
        s.complete = complete;
        return ::cb::compat::to_py(s.summary());
    }
};

template <class V>
struct Exploration : ExplorationStats {
    py::list<V> kept;                        // inputs, each taking a path no input before it took
};

namespace shim {

inline ns::Kind kind_of(const py::str& k) {
    const std::string s = k.utf8();
    return s == "jump" ? ns::Kind::Jump : s == "inner" ? ns::Kind::Inner : ns::Kind::Pin;
}

inline ns::Trail to_new(const Trail& t) {
    ns::Trail out;
    for (const Step& s : t) out.push_back({std::get<0>(s), std::get<1>(s)});
    return out;
}
inline Trail to_old(const ns::Trail& t) {
    Trail out;
    for (const ns::Step& s : t) out.append(Step{s.site, s.taken});
    return out;
}

inline ns::Observation to_new(const Observation& o) {
    ns::Observation n;
    n.trail = to_new(o.trail);
    {
        auto& st = emulate::symbolic::shim::state();
        std::lock_guard<std::mutex> g(st.lock);
        for (const PathEntry& e : o.path) n.path.push_back({e.site, e.taken, emulate::symbolic::shim::node_of(st, e.cond), kind_of(e.kind)});
    }
    for (const auto& [name, v] : o.vars) {
        n.vars[name.utf8()] = {std::get<0>(v) == py::str("arg") ? ns::InputVar::Place::Arg : ns::InputVar::Place::Mem, std::get<1>(v),
                               static_cast<uint32_t>(std::get<2>(v)), std::get<3>(v)};
    }
    for (const py::str& s : o.lost) n.lost.insert(s.utf8());
    n.cut = o.cut;
    return n;
}

inline Vars vars_to_old(const ns::Vars& vars) {
    Vars out;
    for (const auto& [name, v] : vars) {
        out.set(py::str(name), std::make_tuple(py::str(v.place == ns::InputVar::Place::Arg ? "arg" : "mem"), v.where,
                                               static_cast<int64_t>(v.bits), v.value));
    }
    return out;
}

}  // namespace shim

template <class V>
Exploration<V> explore(
    const std::function<std::optional<Observation>(V&)>& run, const py::list<V>& seeds,
    const std::function<std::optional<V>(const V&, const Values&, const Vars&)>& derive,
    const std::function<bool(int64_t)>& own = [](int64_t) { return true; }, int64_t budget = 24, int64_t keep = 24,
    const py::dict<int64_t, Trail>& known = py::dict<int64_t, Trail>(), int64_t steps = SOLVER_STEPS,
    int64_t questions = QUESTIONS) {
    const std::function<std::optional<ns::Observation>(V&)> new_run = [&](V& v) -> std::optional<ns::Observation> {
        std::optional<Observation> o = run(v);
        if (!o) return std::nullopt;
        return shim::to_new(*o);
    };
    const std::function<std::optional<V>(const V&, const ns::Values&, const ns::Vars&)> new_derive =
        [&](const V& v, const ns::Values& values, const ns::Vars& vars) -> std::optional<V> {
        Values old_values;
        for (const auto& [name, x] : values) old_values.set(py::str(name), static_cast<int64_t>(x));
        return derive(v, old_values, shim::vars_to_old(vars));
    };
    ns::Options options;
    options.own = own;
    options.budget = static_cast<int>(budget);
    options.keep = static_cast<int>(keep);
    options.steps = static_cast<uint32_t>(std::min<int64_t>(steps, 0xFFFFFFFFLL));
    options.questions = static_cast<int>(questions);
    std::map<std::size_t, ns::Trail> known_trails;
    for (const auto& [i, t] : known.items()) known_trails[static_cast<std::size_t>(i)] = shim::to_new(t);
    ns::Exploration<V> got = ns::explore<V>(new_run, std::span<V>(seeds.vec()), new_derive, options, known_trails);
    Exploration<V> out;
    out.runs = got.runs;
    out.queries = got.queries;
    out.unsat = got.unsat;
    out.unknown = got.unknown;
    out.paths = got.paths;
    for (const ns::Step& s : got.edges) out.edges.add(Step{s.site, s.taken});
    for (const std::string& s : got.lost) out.lost.add(py::str(s));
    out.missed = got.missed;
    out.complete = got.complete;
    out.pins = got.pins;
    out.budget_spent = got.budget_spent;
    out.guessed = got.guessed;
    for (V& v : got.kept) out.kept.append(std::move(v));
    return out;
}

}  // namespace codebreaker::verify::concolic
