// codebreaker/verify/guided.hpp -- COMPAT SHIM over cb/verify/guided.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module is native/cb/verify/guided.{hpp,cpp} now (typed words, enums for the comparison instruction, byte order
// and place kinds, an explicit generator). This header keeps the old py:: record types the lanes (win32, win16,
// powerpc, lane_common) build their inputs and observations from, converts them to the new ones, calls the new search,
// and converts the answer back. No logic.
//
// Differences the old callers can see, on purpose: the "fixed changes" stage draws from the new generator (SplitMix64),
// not CPython's Mersenne Twister, so its random picks differ from the Python's; and words are carried as uint64 inside
// (the same bits as the old int64).
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>
#include <variant>

#include "cb/compat/py_bridge.hpp"
#include "cb/verify/guided.hpp"

namespace codebreaker::verify::guided {

// A value of an Input's other keys: an int or a list of ints.
using Extra = std::variant<int64_t, py::list<int64_t>>;

// An input: {"args": [...], "mem_init": [(address, bytes)], "stub_ret": int, ...other keys}.
struct Input {
    py::list<int64_t> args;
    std::optional<py::list<std::tuple<int64_t, py::bytes>>> mem_init;
    std::optional<int64_t> stub_ret;
    py::dict<py::str, Extra> extra;
};

// What a comparison before a jump compared: (mnemonic, size, a, b[, a's address, b's address]).
struct Compare {
    py::str mn;
    int64_t size = 0;
    int64_t a = 0;
    int64_t b = 0;
    std::optional<int64_t> at_a;
    std::optional<int64_t> at_b;
    bool has_at = false;          // the six-element form (len(cmp) > 4)
};

// One trace of the original: its conditional jumps, the addresses it ran, and where the stack is.
struct Observation {
    py::list<std::tuple<int64_t, bool, std::optional<Compare>>> branches;   // (jump, taken, compare)
    py::set<int64_t> reach;
    std::optional<int64_t> args_at;
    std::optional<std::tuple<int64_t, int64_t>> stack;    // (low, high)
};

// What to change in an input: where a value lives.
enum class PlaceKind { arg, addr, stub_ret, mem };
struct Place {
    PlaceKind kind = PlaceKind::arg;
    int64_t a = 0;     // arg: word index; addr: the address; mem: index into mem_init
    int64_t b = 0;     // arg: byte within the word; mem: byte offset in that block
    friend bool operator==(const Place& x, const Place& y) { return x.kind == y.kind && x.a == y.a && x.b == y.b; }
};

using Run = std::function<std::optional<Observation>(const Input&)>;

namespace compat_detail {
namespace nv = cb::verify::guided;

inline nv::Input to_new(const Input& v) {
    nv::Input out;
    for (int64_t a : v.args) out.args.push_back(static_cast<std::uint64_t>(a));
    if (v.mem_init) {
        auto& blocks = out.mem_init.emplace();
        for (const auto& m : *v.mem_init) blocks.push_back({static_cast<std::uint64_t>(std::get<0>(m)), cb::compat::from_py(std::get<1>(m))});
    }
    if (v.stub_ret) out.stub_ret = static_cast<std::uint64_t>(*v.stub_ret);
    for (const auto& kv : v.extra.items()) {
        const Extra& e = std::get<1>(kv);
        out.extra[cb::compat::from_py(std::get<0>(kv))] =
            std::holds_alternative<int64_t>(e) ? nv::Extra(std::get<int64_t>(e)) : nv::Extra(cb::compat::from_py(std::get<py::list<int64_t>>(e)));
    }
    return out;
}

inline Input to_old(const nv::Input& v) {
    Input out;
    for (std::uint64_t a : v.args) out.args.append(static_cast<int64_t>(a));
    if (v.mem_init) {
        py::list<std::tuple<int64_t, py::bytes>> blocks;
        for (const nv::MemInit& m : *v.mem_init) blocks.append(std::make_tuple(static_cast<int64_t>(m.address), cb::compat::to_py(cb::ByteView(m.data))));
        out.mem_init = blocks;
    }
    if (v.stub_ret) out.stub_ret = static_cast<int64_t>(*v.stub_ret);
    for (const auto& [k, e] : v.extra)
        out.extra.set(cb::compat::to_py(k),
                      std::holds_alternative<std::int64_t>(e) ? Extra(std::get<std::int64_t>(e)) : Extra(cb::compat::to_py(std::get<std::vector<std::int64_t>>(e))));
    return out;
}

inline nv::Compare to_new(const Compare& c) {
    nv::Compare out;
    out.op = nv::parse_compare_op(c.mn.utf8());
    out.size = static_cast<unsigned>(c.size);
    out.a = static_cast<std::uint64_t>(c.a);
    out.b = static_cast<std::uint64_t>(c.b);
    if (c.has_at) {   // the four-element form has no addresses even if a lane set the fields
        if (c.at_a) out.a_at = static_cast<std::uint64_t>(*c.at_a);
        if (c.at_b) out.b_at = static_cast<std::uint64_t>(*c.at_b);
    }
    return out;
}

inline nv::Observation to_new(const Observation& o) {
    nv::Observation out;
    for (const auto& b : o.branches) {
        nv::Branch br;
        br.address = static_cast<std::uint64_t>(std::get<0>(b));
        br.taken = std::get<1>(b);
        if (std::get<2>(b)) br.compare = to_new(*std::get<2>(b));
        out.branches.push_back(br);
    }
    for (int64_t a : o.reach) out.reach.insert(static_cast<std::uint64_t>(a));
    if (o.args_at) out.args_at = static_cast<std::uint64_t>(*o.args_at);
    if (o.stack) {
        out.stack_low = static_cast<std::uint64_t>(std::get<0>(*o.stack));
        out.stack_high = static_cast<std::uint64_t>(std::get<1>(*o.stack));
    }
    return out;
}

inline nv::Machine to_new_machine(const py::str& order, int64_t word) {
    return nv::Machine{order == py::str("big") ? nv::ByteOrder::Big : nv::ByteOrder::Little, static_cast<unsigned>(word)};
}
}  // namespace compat_detail

inline const py::list<int64_t> INTERESTING = [] {
    py::list<int64_t> out;
    for (std::uint64_t v : cb::verify::guided::kInteresting) out.append(static_cast<int64_t>(v));
    return out;
}();

// an independent copy of an input (its lists are not shared)
inline Input _copy(const Input& v) { return compat_detail::to_old(compat_detail::to_new(v)); }

// (operand, value seen, value wanted): changes to one operand that would send a comparison the other way.
inline py::list<std::tuple<int64_t, int64_t, int64_t>> _flips(const Compare& cmp) {
    py::list<std::tuple<int64_t, int64_t, int64_t>> out;
    for (const cb::verify::guided::Flip& f : cb::verify::guided::flips(compat_detail::to_new(cmp)))
        out.append(std::make_tuple(static_cast<int64_t>(f.operand), static_cast<int64_t>(f.seen), static_cast<int64_t>(f.wanted)));
    return out;
}

struct _Machine {
    py::str order;       // "little" or "big"
    int64_t word;        // bytes in an argument
    uint64_t wmask;
    _Machine(const py::str& byteorder, int64_t word_)
        : order(byteorder), word(word_), wmask(word_ >= 8 ? ~uint64_t{0} : (uint64_t{1} << (8 * word_)) - 1) {}
};

// Places in input `v` to put a new value for operand `k` of comparison `cmp`.
inline py::list<Place> _places(const Input& v, const Observation& obs, const Compare& cmp, int64_t k, int64_t value, int64_t size,
                               const _Machine& mach) {
    py::list<Place> out;
    for (const cb::verify::guided::Place& p :
         cb::verify::guided::places(compat_detail::to_new(v), compat_detail::to_new(obs), compat_detail::to_new(cmp), static_cast<int>(k),
                                    static_cast<std::uint64_t>(value), static_cast<unsigned>(size),
                                    compat_detail::to_new_machine(mach.order, mach.word))) {
        Place o;
        o.kind = static_cast<PlaceKind>(static_cast<int>(p.kind));   // the enumerators are in the same order
        o.a = static_cast<int64_t>(p.index);
        o.b = static_cast<int64_t>(p.offset);
        out.append(o);
    }
    return out;
}

inline Input _put(const Input& v, const Place& place, int64_t value, int64_t size, const _Machine& mach) {
    cb::verify::guided::Place p;
    p.kind = static_cast<cb::verify::guided::PlaceKind>(static_cast<int>(place.kind));
    p.index = static_cast<std::uint64_t>(place.a);
    p.offset = static_cast<std::uint64_t>(place.b);
    return compat_detail::to_old(cb::verify::guided::put(compat_detail::to_new(v), p, static_cast<std::uint64_t>(value),
                                                         static_cast<unsigned>(size), compat_detail::to_new_machine(mach.order, mach.word)));
}

// Inputs (beyond `seeds`) that take branch directions the seeds did not.
inline py::list<Input> guided_inputs(const Run& run, const py::list<Input>& seeds, int64_t budget = 96, int64_t keep = 16,
                                     int64_t seed = 0, const py::str& byteorder = py::str("little"), int64_t word = 4,
                                     const py::list<std::tuple<Input, Observation>>& observed = py::list<std::tuple<Input, Observation>>()) {
    namespace nv = cb::verify::guided;
    std::vector<nv::Input> new_seeds;
    for (const Input& v : seeds) new_seeds.push_back(compat_detail::to_new(v));
    std::vector<nv::Seen> seen;
    for (const auto& s : observed) seen.push_back({compat_detail::to_new(std::get<0>(s)), compat_detail::to_new(std::get<1>(s))});
    nv::Options options;
    options.budget = static_cast<int>(budget);
    options.keep = static_cast<int>(keep);
    options.seed = static_cast<std::uint64_t>(seed);
    options.machine = compat_detail::to_new_machine(byteorder, word);
    options.observed = seen;
    // The new search runs the original through the OLD record types the lane's `run` takes, so each input it asks about is
    // converted back for the call, and the observation it returns converted forward.
    const nv::Run new_run = [&](const nv::Input& in) -> std::optional<nv::Observation> {
        std::optional<Observation> o = run(compat_detail::to_old(in));
        if (!o) return std::nullopt;
        return compat_detail::to_new(*o);
    };
    py::list<Input> out;
    for (const nv::Input& v : nv::guided_inputs(new_run, new_seeds, options)) out.append(compat_detail::to_old(v));
    return out;
}

}  // namespace codebreaker::verify::guided
