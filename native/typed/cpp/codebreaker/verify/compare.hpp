// codebreaker/verify/compare.hpp -- COMPAT SHIM over cb/verify/compare.hpp (docs/CPP_DESIGN.md 6.3), for the old
// verify/lane_common and verify/lanes/* ports. Deleted with its last py:: consumer.
//
// The comparer (first_difference, difference_detail, the judgement over a lane's inputs, the fingerprint and its cache
// entry) is native/cb/verify/compare.hpp. Here: the old record shapes (py:: containers, a verdict as its word, the
// limits as a sorted list of codes), converted in and out; nothing else.
//
// A fingerprint kept in the cache is the new module's stored form (cb's own cache file); one a Python-era cache held
// is not read. `cache_key` files a run under cb's key of the parts, not the Python's repr of them.
#pragma once

#include <algorithm>
#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>
#include <type_traits>
#include <vector>

#include "cb/compat/py_bridge.hpp"
#include "cb/verify/compare.hpp"
#include "codebreaker/base/cache.hpp"
#include "codebreaker/verify/verdicts.hpp"
#include "py.hpp"

namespace codebreaker::verify::compare {

namespace ns = ::cb::verify::compare;

inline constexpr int64_t REV = ns::kRevision;

struct Fingerprint {
    std::optional<int64_t> ret;          // the result, None for a run that did not return
    py::str exit;                        // how it ended ("ret", "fault", "truncated", "wild-call" ...)
    bool truncated = false;
    py::list<py::str> deps;              // its calls and effects out, as text in order
    std::optional<py::str> writes;       // the memory it left changed, None: more than is recorded
    std::optional<int64_t> pop;          // the stack's move, for a run that returned
    std::optional<bool> saved;           // whether saved registers were preserved, for a run that returned
    py::list<int64_t> cov;               // the function's own instructions run
    py::list<int64_t> reach;             // every instruction run
    std::optional<int64_t> steps;        // instructions the run took, where the lane records it
    std::optional<py::str> err;          // set for a run that was not an observation

    friend bool operator==(const Fingerprint& a, const Fingerprint& b) {
        return a.ret == b.ret && a.exit == b.exit && a.truncated == b.truncated && a.deps == b.deps && a.writes == b.writes &&
               a.pop == b.pop && a.saved == b.saved && a.cov == b.cov && a.reach == b.reach && a.steps == b.steps && a.err == b.err;
    }
};

// What a lane's input has that the comparer reads.
struct Input {
    py::list<int64_t> args;
    std::optional<int64_t> stub_ret;
};

// What compare() tells its caller besides the verdict.
struct Info {
    std::optional<int64_t> ret_bits;
    std::optional<py::str> detail;
};

namespace shim {
inline ns::Fingerprint to_new(const Fingerprint& f) {
    ns::Fingerprint n;
    n.ret = f.ret;
    n.exit = ::cb::compat::from_py(f.exit);
    n.truncated = f.truncated;
    for (const py::str& d : f.deps) n.deps.push_back(::cb::compat::from_py(d));
    if (f.writes) n.writes = ::cb::compat::from_py(*f.writes);
    n.pop = f.pop;
    n.saved = f.saved;
    n.cov.assign(f.cov.vec().begin(), f.cov.vec().end());
    n.reach.assign(f.reach.vec().begin(), f.reach.vec().end());
    n.steps = f.steps;
    if (f.err) n.err = ::cb::compat::from_py(*f.err);
    return n;
}
inline Fingerprint to_old(const ns::Fingerprint& n) {
    Fingerprint f;
    f.ret = n.ret;
    f.exit = ::cb::compat::to_py(n.exit);
    f.truncated = n.truncated;
    for (const std::string& d : n.deps) f.deps.append(::cb::compat::to_py(d));
    if (n.writes) f.writes = ::cb::compat::to_py(*n.writes);
    f.pop = n.pop;
    f.saved = n.saved;
    for (int64_t x : n.cov) f.cov.append(x);
    for (int64_t x : n.reach) f.reach.append(x);
    f.steps = n.steps;
    if (n.err) f.err = ::cb::compat::to_py(*n.err);
    return f;
}
inline ns::Input to_new(const Input& v) {
    ns::Input n;
    n.args.assign(v.args.vec().begin(), v.args.vec().end());
    n.stub_ret = v.stub_ret;
    return n;
}
}  // namespace shim

inline py::str first_difference(const Fingerprint& a, const Fingerprint& b, int64_t ret_mask = 0) {
    return ::cb::compat::to_py(ns::reason(ns::first_difference(shim::to_new(a), shim::to_new(b), static_cast<uint32_t>(ret_mask))));
}

inline py::str difference_detail(const Fingerprint& a, const Fingerprint& b, const std::optional<Input>& v = std::nullopt,
                                 int64_t ret_mask = 0) {
    const ns::Input input = v ? shim::to_new(*v) : ns::Input{};
    return ::cb::compat::to_py(ns::difference_detail(shim::to_new(a), shim::to_new(b), v ? &input : nullptr, static_cast<uint32_t>(ret_mask)));
}

template <class V> using FingerprintOf = std::function<Fingerprint(const V&)>;

// Judge one body: (verdict word, reason, the limits' codes sorted).
template <class V>
std::tuple<py::str, py::str, py::list<py::str>> compare(
    const FingerprintOf<V>& fp_retail, const FingerprintOf<V>& fp_rebuilt, const py::list<V>& vectors,
    const py::list<V>& confirm = py::list<V>(), int64_t insns = 0,
    const std::function<py::list<V>(const py::set<int64_t>&, const py::set<int64_t>&)>& directed = nullptr, Info* info = nullptr,
    const std::function<int64_t(const FingerprintOf<V>&, const V&, const Fingerprint&)>& leftover = nullptr) {
    static_assert(std::is_base_of<Input, V>::value, "an input of compare() is an Input (args, stub_ret) and what the lane adds");
    struct Wrapped : ns::Input {   // the new comparer's input, carrying the lane's
        const V* lane = nullptr;
    };
    std::vector<std::unique_ptr<V>> kept;   // (the directed inputs live as long as the call)
    auto wrap = [](const V& v) {
        Wrapped w;
        static_cast<ns::Input&>(w) = shim::to_new(v);
        w.lane = &v;
        return w;
    };
    auto wrap_all = [&](const py::list<V>& vs) {
        std::vector<Wrapped> out;
        for (const V& v : vs) out.push_back(wrap(v));
        return out;
    };
    const std::vector<Wrapped> basic = wrap_all(vectors), more = wrap_all(confirm);
    std::vector<py::list<V>> directed_lists;   // (their elements are what `Wrapped::lane` points at)
    directed_lists.reserve(8);
    const std::function<ns::Fingerprint(const Wrapped&)> original = [&](const Wrapped& w) { return shim::to_new(fp_retail(*w.lane)); };
    const std::function<ns::Fingerprint(const Wrapped&)> rebuilt = [&](const Wrapped& w) { return shim::to_new(fp_rebuilt(*w.lane)); };
    std::function<std::vector<Wrapped>(const std::set<int64_t>&, const std::set<int64_t>&)> aimed;
    if (directed) {
        aimed = [&](const std::set<int64_t>& covered, const std::set<int64_t>& reached) {
            py::set<int64_t> c, r;
            for (int64_t x : covered) c.add(x);
            for (int64_t x : reached) r.add(x);
            directed_lists.push_back(directed(c, r));
            return wrap_all(directed_lists.back());
        };
    }
    std::function<uint32_t(const std::function<ns::Fingerprint(const Wrapped&)>&, const Wrapped&, const ns::Fingerprint&)> mask;
    if (leftover) {
        mask = [&](const std::function<ns::Fingerprint(const Wrapped&)>&, const Wrapped& w, const ns::Fingerprint& a) {
            return static_cast<uint32_t>(leftover(fp_retail, *w.lane, shim::to_old(a)));
        };
    }
    const ns::Outcome o = ns::compare<Wrapped>(original, rebuilt, basic, more, {.insns = insns}, aimed, mask);
    if (info != nullptr) {
        if (o.ret_bits) info->ret_bits = *o.ret_bits;
        if (!o.detail.empty()) info->detail = ::cb::compat::to_py(o.detail);
    }
    py::list<py::str> limits;
    for (const std::string& code : o.limit_codes()) limits.append(::cb::compat::to_py(code));
    return {::cb::compat::to_py(std::string(::cb::verify::word(o.verdict))), ::cb::compat::to_py(o.reason), limits};
}

// Fingerprints are kept in the one cache, kind "fingerprints", its generation REV.
inline void register_generation_() {
    namespace oc = codebreaker::base::cache;
    if (!oc::GENERATION_REPRS.contains(py::str("fingerprints"))) {
        oc::GENERATION_REPRS.set(py::str("fingerprints"), []() -> std::optional<py::str> { return py::str(std::to_string(REV)); });
    }
}

inline std::optional<Fingerprint> cache_get(const py::str& key) {
    register_generation_();
    const std::optional<py::bytes> raw = codebreaker::base::cache::get(py::str("fingerprints"), key);
    if (!raw) return std::nullopt;
    const std::optional<ns::Fingerprint> f = ns::decode(::cb::compat::from_py(*raw));
    return f ? std::optional<Fingerprint>(shim::to_old(*f)) : std::nullopt;
}

inline void cache_put(const py::str& key, const Fingerprint& fp) {
    register_generation_();
    codebreaker::base::cache::put(py::str("fingerprints"), key, ::cb::compat::to_py(::cb::ByteView(ns::encode(shim::to_new(fp)))));
}

inline py::str cache_key(const std::vector<codebreaker::base::cache::Part>& parts) {
    return py::str("r" + std::to_string(REV) + ":") + codebreaker::base::cache::key(parts);
}

inline py::str cache_key_repr(const py::str& parts_repr) { return ::cb::compat::to_py(ns::cache_key_text(parts_repr.utf8())); }

}  // namespace codebreaker::verify::compare
