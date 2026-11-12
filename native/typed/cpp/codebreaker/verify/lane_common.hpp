// codebreaker/verify/lane_common.hpp -- COMPAT SHIM over cb/verify/lane_common.hpp (docs/CPP_DESIGN.md 6.3), for the lanes
// that are still the old ports (verify/lanes/{win16,win32,powerpc,driver}, verify/gate16/isolated16, build/module).
// Deleted with its last py:: consumer. Conversion and forwarding only; the engine is native/cb/verify/lane_common.hpp
// (see there for what each function is for).
//
// Differences a caller of the old port may notice: a call with no name is shown "None" as before (the new code says
// "?"); writes() of places of both kinds sorts them instead of raising TypeError.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>
#include <variant>

#include "cb/compat/py_bridge.hpp"
#include "cb/verify/lane_common.hpp"
#include "codebreaker/emulate/run.hpp"
#include "codebreaker/verify/compare.hpp"
#include "codebreaker/verify/concolic.hpp"
#include "codebreaker/verify/guided.hpp"
#include "codebreaker/verify/verdicts.hpp"
#include "py.hpp"

namespace codebreaker::verify::lane_common {

namespace ns = ::cb::verify::lane_common;

// --- the switches -----------------------------------------------------------------------------
inline bool fp_cache_on() { return ns::fp_cache_on(); }
inline bool guided_on() { return ns::guided_on(); }
inline bool concolic_on() { return ns::concolic_on(); }
// read once, when the judge is loaded
inline const bool PLAIN = ns::plain();

// --- fingerprints kept across runs --------------------------------------------------------------
inline compare::Fingerprint remembered(const std::optional<py::str>& key, const std::function<compare::Fingerprint()>& run,
                                       const std::function<bool(const compare::Fingerprint&)>& keep = nullptr) {
    ns::FingerprintStore store;
    store.get = [](std::string_view k) -> std::optional<::cb::verify::compare::Fingerprint> {
        std::optional<compare::Fingerprint> f = compare::cache_get(py::str(std::string(k)));
        return f ? std::optional<::cb::verify::compare::Fingerprint>(compare::shim::to_new(*f)) : std::nullopt;
    };
    store.put = [](std::string_view k, const ::cb::verify::compare::Fingerprint& f) {
        compare::cache_put(py::str(std::string(k)), compare::shim::to_old(f));
    };
    std::function<bool(const ::cb::verify::compare::Fingerprint&)> new_keep;
    if (keep) new_keep = [&](const ::cb::verify::compare::Fingerprint& f) { return keep(compare::shim::to_old(f)); };
    return compare::shim::to_old(ns::remembered(
        store, key ? std::optional<std::string>(::cb::compat::from_py(*key)) : std::nullopt,
        [&] { return compare::shim::to_new(run()); }, new_keep));
}

// --- calls, as two builds agree on them ---------------------------------------------------------
inline py::str call(const py::str& name, const std::optional<py::list<py::str>>& args = std::nullopt) {
    std::optional<std::vector<std::string>> a;
    if (args) {
        a.emplace();
        for (const py::str& s : *args) a->push_back(s.utf8());
    }
    return ::cb::compat::to_py(ns::call(name.utf8(), a));
}

// A recorded call: its name (None for one that has none) and its arguments (None when their number is not known).
struct Called {
    std::optional<py::str> name;
    std::optional<py::list<int64_t>> args;
};
inline py::list<Called> called(const py::list<emulate::run::Call>& seq) {
    py::list<Called> out;
    for (const emulate::run::Call& c : seq) out.append(Called{c.name, c.args});
    return out;
}

inline py::list<py::str> calls(const py::list<Called>& seq, const std::function<py::str(int64_t)>& argument,
                               const std::function<py::str(const std::optional<py::str>&)>& name = nullptr,
                               const std::function<bool(const std::optional<py::str>&)>& skip = nullptr,
                               const std::optional<int64_t>& cap = std::nullopt) {
    std::vector<ns::Called> in;
    for (const Called& c : seq) {
        ns::Called n;
        if (c.name) n.name = c.name->utf8();
        if (c.args) n.args = std::vector<std::int64_t>(c.args->vec().begin(), c.args->vec().end());
        in.push_back(std::move(n));
    }
    auto old_name = [](const std::optional<std::string>& n) { return n ? std::optional<py::str>(py::str(*n)) : std::nullopt; };
    const auto show = [&](std::int64_t v) { return argument(v).utf8(); };
    const auto named = [&](const std::optional<std::string>& n) {   // (as the port did: a call with no name is "None")
        const std::optional<py::str> shown = old_name(n);
        return name ? name(shown).utf8() : (shown ? shown->utf8() : std::string("None"));
    };
    std::function<bool(const std::optional<std::string>&)> new_skip;
    if (skip) new_skip = [&](const std::optional<std::string>& n) { return skip(old_name(n)); };
    py::list<py::str> out;
    for (const std::string& s : ns::calls(in, show, named, new_skip,
                                          cap ? std::optional<std::size_t>(static_cast<std::size_t>(*cap)) : std::nullopt)) {
        out.append(::cb::compat::to_py(s));
    }
    return out;
}

// A range of the addresses of a frame (None: the kind has no such place)
using Span = std::optional<std::tuple<int64_t, int64_t>>;
// The addresses of this build's own image: a (low, high) range, or a test
using Image = std::variant<std::monostate, std::tuple<int64_t, int64_t>, std::function<bool(int64_t)>>;

inline py::str argument(int64_t v, const py::list<int64_t>& inputs = py::list<int64_t>(), const std::optional<int64_t>& above = std::nullopt,
                        bool small = false, const py::list<Span>& frame = py::list<Span>(), const Image& image = std::monostate{}) {
    ns::ArgumentContext c;
    c.inputs.assign(inputs.vec().begin(), inputs.vec().end());
    c.above = above;
    c.small = small;
    for (const Span& s : frame) {
        c.frame.push_back(s ? std::optional<ns::AddressRange>(ns::AddressRange{std::get<0>(*s), std::get<1>(*s)}) : std::nullopt);
    }
    if (const auto* range = std::get_if<std::tuple<int64_t, int64_t>>(&image)) {
        c.image = ns::AddressRange{std::get<0>(*range), std::get<1>(*range)};
    } else if (const auto* test = std::get_if<std::function<bool(int64_t)>>(&image)) {
        c.image = *test;
    }
    return ::cb::compat::to_py(ns::argument(v, c));
}

// A byte of memory a lane names: an address, or (tag, offset)
using Place = std::variant<int64_t, std::tuple<py::str, int64_t>>;
// What a place held at the end: a byte, or the token that stands for the whole word it was part of
using Held = std::variant<int64_t, py::str>;
using State = py::dict<Place, Held>;

namespace shim_detail {
inline ns::Place place_to_new(const Place& p) {
    if (const auto* a = std::get_if<int64_t>(&p)) return ns::Place(static_cast<std::uint64_t>(*a));
    const auto& t = std::get<std::tuple<py::str, int64_t>>(p);
    return ns::Place(ns::TaggedPlace{std::get<0>(t).utf8(), std::get<1>(t)});
}
inline Place place_to_old(const ns::Place& p) {
    if (const auto* a = std::get_if<std::uint64_t>(&p)) return Place(static_cast<int64_t>(*a));
    const auto& t = std::get<ns::TaggedPlace>(p);
    return Place(std::make_tuple(py::str(t.tag), t.offset));
}
inline State state_to_old(const ns::State& s) {
    State out;
    for (const auto& [place, held] : s) {
        out.set(place_to_old(place), std::holds_alternative<std::uint8_t>(held) ? Held(static_cast<int64_t>(std::get<std::uint8_t>(held)))
                                                                                 : Held(py::str(std::get<std::string>(held))));
    }
    return out;
}
inline ::cb::emulate::Run run_to_new(const emulate::run::Run& r) {
    ::cb::emulate::Run n;
    if (r.exit) n.exit = r.exit->utf8();
    n.capped = r.capped;
    n.truncated = r.truncated;
    if (r.stack) n.stack = *r.stack;
    n.endian = r.endian == py::str("big") ? ::cb::emulate::Endian::Big : ::cb::emulate::Endian::Little;
    for (const emulate::run::Write& w : r.writes) {
        ::cb::emulate::MemoryWrite m;
        m.address = static_cast<std::uint64_t>(w.address);
        m.size = static_cast<std::uint32_t>(w.size);
        m.value = static_cast<std::uint64_t>(w.value);
        if (w.old) m.old = static_cast<std::uint64_t>(*w.old);
        m.harness = w.harness;
        n.writes.push_back(m);
    }
    for (const emulate::run::Call& c : r.calls) {
        ::cb::emulate::Call k;
        if (c.name) k.name = c.name->utf8();
        if (c.args) k.args = std::vector<std::int64_t>(c.args->vec().begin(), c.args->vec().end());
        n.calls.push_back(std::move(k));
    }
    return n;
}
}  // namespace shim_detail

inline py::str writes(const State& state) {
    ns::State s;
    for (const auto& kv : state.items()) {
        const Held& h = std::get<1>(kv);
        s.insert_or_assign(shim_detail::place_to_new(std::get<0>(kv)),
                           std::holds_alternative<int64_t>(h) ? ns::Held(static_cast<std::uint8_t>(std::get<int64_t>(h)))
                                                              : ns::Held(std::get<py::str>(h).utf8()));
    }
    return ::cb::compat::to_py(ns::writes(s));
}

// --- from a run record (emulate/run.py) to a fingerprint ----------------------------------------
using ByteNamer = std::function<std::optional<Place>(int64_t)>;
using Where = std::variant<int64_t, std::tuple<py::str, int64_t>, ByteNamer>;

namespace shim_detail {
inline ns::ChangedOptions options_to_new(const std::function<std::optional<Where>(int64_t)>& place,
                                         const std::function<std::optional<py::str>(int64_t)>& word, int64_t most, bool everything) {
    ns::ChangedOptions o;
    if (place) {
        o.place = [place](std::uint64_t a) -> ns::Where {
            const std::optional<Where> w = place(static_cast<int64_t>(a));
            if (!w) return ns::Skip{};
            if (const auto* x = std::get_if<int64_t>(&*w)) return ns::Place(static_cast<std::uint64_t>(*x));
            if (const auto* t = std::get_if<std::tuple<py::str, int64_t>>(&*w)) return place_to_new(Place(*t));
            const ByteNamer namer = std::get<ByteNamer>(*w);
            return ns::ByteNamer([namer](std::size_t i) -> std::optional<ns::Place> {
                const std::optional<Place> p = namer(static_cast<int64_t>(i));
                return p ? std::optional<ns::Place>(place_to_new(*p)) : std::nullopt;
            });
        };
    }
    if (word) {
        o.word = [word](std::uint64_t v) -> std::optional<std::string> {
            const std::optional<py::str> t = word(static_cast<int64_t>(v));
            return t ? std::optional<std::string>(t->utf8()) : std::nullopt;
        };
    }
    o.most = static_cast<std::size_t>(most);
    o.everything = everything;
    return o;
}
}  // namespace shim_detail

inline State changed(const emulate::run::Run& run, const std::function<std::optional<Where>(int64_t)>& place = nullptr,
                     const std::function<std::optional<py::str>(int64_t)>& word = nullptr, int64_t most = 8, bool everything = true) {
    return shim_detail::state_to_old(ns::changed(shim_detail::run_to_new(run), shim_detail::options_to_new(place, word, most, everything)));
}

inline std::optional<py::str> left(const emulate::run::Run& run, const std::function<std::optional<Where>(int64_t)>& place = nullptr,
                                   const std::function<std::optional<py::str>(int64_t)>& word = nullptr, int64_t most = 8,
                                   bool everything = true) {
    const std::optional<std::string> s = ns::left(shim_detail::run_to_new(run), shim_detail::options_to_new(place, word, most, everything));
    return s ? std::optional<py::str>(::cb::compat::to_py(*s)) : std::nullopt;
}

inline compare::Fingerprint fingerprint(const emulate::run::Run& run, const py::list<py::str>& deps,
                                        const std::optional<int64_t>& ret = std::nullopt,
                                        const std::optional<py::str>& writes_ = py::str(""), const std::optional<bool>& saved = std::nullopt,
                                        const py::list<int64_t>& cov = py::list<int64_t>(), const py::list<int64_t>& reach = py::list<int64_t>(),
                                        const std::optional<int64_t>& steps = std::nullopt) {
    ns::FingerprintParts parts;
    parts.ret = ret;
    if (writes_) {
        parts.writes = writes_->utf8();
    } else {
        parts.writes = std::nullopt;
    }
    parts.saved = saved;
    parts.cov.assign(cov.vec().begin(), cov.vec().end());
    parts.reach.assign(reach.vec().begin(), reach.vec().end());
    parts.steps = steps;
    std::vector<std::string> d;
    for (const py::str& s : deps) d.push_back(s.utf8());
    return compare::shim::to_old(ns::fingerprint(shim_detail::run_to_new(run), std::move(d), std::move(parts)));
}

// --- inputs -------------------------------------------------------------------------------------
namespace shim_detail {
inline py::list<int64_t> words_to_old(const std::vector<std::uint64_t>& v) {
    py::list<int64_t> out;
    for (std::uint64_t x : v) out.append(static_cast<int64_t>(x));
    return out;
}
}  // namespace shim_detail

inline py::list<int64_t> counting(int64_t n, int64_t first = 1, int64_t step = 1) {
    return shim_detail::words_to_old(ns::counting(static_cast<std::size_t>(n), static_cast<std::uint64_t>(first), static_cast<std::uint64_t>(step)));
}
inline py::list<int64_t> descending(int64_t n, int64_t top) {
    return shim_detail::words_to_old(ns::descending(static_cast<std::size_t>(n), static_cast<std::uint64_t>(top)));
}
// A value, or a function of the argument's position
using Pattern = std::variant<int64_t, std::function<int64_t(int64_t)>>;
inline py::list<int64_t> alternating(int64_t n, const Pattern& even, const Pattern& odd) {
    auto to_new = [](const Pattern& p) -> ns::Pattern {
        if (const auto* v = std::get_if<int64_t>(&p)) return static_cast<std::uint64_t>(*v);
        const auto f = std::get<1>(p);
        return std::function<std::uint64_t(std::size_t)>([f](std::size_t i) { return static_cast<std::uint64_t>(f(static_cast<int64_t>(i))); });
    };
    return shim_detail::words_to_old(ns::alternating(static_cast<std::size_t>(n), to_new(even), to_new(odd)));
}

constexpr int64_t MAX_GUIDED = ns::kMaxGuided;
constexpr int64_t GUIDED_BUDGET = ns::kGuidedBudget;

inline py::list<guided::Input> guided(const guided::Run& run, const py::list<guided::Input>& seeds, int64_t seed,
                                      const py::str& byteorder = py::str("little"), int64_t word = 4) {
    if (!guided_on()) return py::list<guided::Input>();
    return codebreaker::verify::guided::guided_inputs(run, seeds, GUIDED_BUDGET, MAX_GUIDED, seed, byteorder, word);
}

constexpr int64_t PATH_BUDGET = ns::kPathBudget;
constexpr int64_t MAX_PATHS = ns::kMaxPaths;
constexpr int64_t GUIDED_REST = ns::kGuidedRest;

template <class V>
concolic::Exploration<V> explored(const std::function<std::optional<concolic::Observation>(V&)>& run, const py::list<V>& seeds,
                                  const std::function<std::optional<V>(const V&, const concolic::Values&, const concolic::Vars&)>& derive,
                                  const std::function<bool(int64_t)>& own,
                                  const py::dict<int64_t, concolic::Trail>& known = py::dict<int64_t, concolic::Trail>(),
                                  const std::optional<int64_t>& steps = std::nullopt, int64_t more = 0) {
    return concolic::explore<V>(run, seeds, derive, own, PATH_BUDGET + more, MAX_PATHS, known,
                                (steps && *steps != 0) ? *steps : concolic::SOLVER_STEPS);
}

inline py::list<py::str> path_limits(const concolic::ExplorationStats* got) {
    py::list<py::str> out;
    if (got == nullptr) return out;
    ::cb::verify::concolic::Stats s;
    s.paths = static_cast<int>(got->paths);
    s.complete = got->complete;
    for (const std::string& l : ns::path_limits(&s)) out.append(::cb::compat::to_py(l));
    return out;
}

// --- the verdict --------------------------------------------------------------------------------
// The old comparison throws the old layer's exceptions, which the new judged() does not know: they are told apart here.
inline std::tuple<py::str, py::str, py::list<py::str>, py::str> judged(
    const std::function<std::tuple<py::str, py::str, py::list<py::str>>(compare::Info&)>& compare_one) {
    compare::Info info;
    py::str verdict, reason;
    py::list<py::str> limits;
    try {
        std::tie(verdict, reason, limits) = compare_one(info);
    } catch (const py::ImportError&) {
        throw;
    } catch (const py::Exception& e) {
        verdict = py::str(verdicts::INCONCLUSIVE);
        reason = py::percent("judge: %s", py::str(e.type()));
        limits = py::list<py::str>();
    } catch (const std::exception&) {
        verdict = py::str(verdicts::INCONCLUSIVE);
        reason = py::str("judge: Exception");
        limits = py::list<py::str>();
    }
    return {verdict, reason, limits.copy(), info.detail ? *info.detail : py::str("")};
}

}  // namespace codebreaker::verify::lane_common
