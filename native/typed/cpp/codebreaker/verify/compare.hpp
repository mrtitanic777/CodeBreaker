// codebreaker/verify/compare.hpp -- C++17 port of codebreaker.verify.compare.
//
// compare.py -- judge a reconstruction: do two builds of one function behave alike?
//
//     auto [verdict, reason, limits] = compare::compare<V>(fp_original, fp_rebuilt, inputs, ...);
//
// `fp_original(v)` and `fp_rebuilt(v)` run their side on input `v` and return a FINGERPRINT of the
// run (made from the emulator's run record, emulate/run.py, by lane_common.fingerprint) -- a record
// saying how it ended, what it returned, which calls it made with which arguments,
// what it left in memory, how far the stack moved, whether saved registers were preserved, and
// which of the original's instructions ran. This module knows nothing about processors or file
// formats: a kind of module (verify/lanes/win32.py, verify/lanes/win16.py) knows how to run its code and
// produce the fingerprint; the comparison, the verdicts and the list of reasons a match is weaker
// than it looks are the same for all of them.
//
//     equivalent     every input compared equal, and at least one run returned normally
//     consistent     compared equal, but no run returned (both sides only ever fail alike)
//     divergent      some input showed a difference; `reason` says which kind
//     inconclusive   the original could not be observed on any input
//
// Typing notes:
//   * The fingerprint (Python: a dict with the fields below, which a lane may add to) is the struct `Fingerprint`:
//     ret, exit, truncated, deps, writes, pop, saved, cov, reach, and the two a lane adds that this module reads,
//     steps and err.  A dict key that is absent is a field that is nullopt (or empty).  `deps` is a tuple of
//     text in the Python; here a list, and it is pickled as a tuple.  A fingerprint in the cache holding any
//     other key than these (a lane's own field) is read as not there: the lane makes the run again.
//   * An input `v` (a dict of the lane's making) is the caller's own type V, which is an `Input` (its "args": the
//     arguments, and "stub_ret": what imported calls return, 0 or none for none) with whatever else the lane
//     keeps: compare() hands it to the lane's callbacks and reads only those two fields of it.
//   * `info` (a dict the caller reads "ret_bits" and "detail" from) is an `Info` handed by pointer, null for None.
//   * `leftover(fp_retail, v, a)` returns the mask of the bits of the return value that are leftovers (0: none).
//   * cache_key(*parts) takes cache::Part values (the Python repr()s the tuple of its arguments: cache_key_repr is
//     the same for a caller that has the text of that tuple already, as a lane whose parts are not Parts does).
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>
#include <type_traits>
#include <vector>

#include "codebreaker/base/cache.hpp"
#include "codebreaker/verify/verdicts.hpp"
#include "py.hpp"

namespace codebreaker::verify::compare {

// Bump when the oracle changes what a trace of the SAME code observes, so fingerprints cached under
// the old behaviour are not reused. 2: every PE code section is mapped (calls into another code
// section now run instead of being stub-returned). 3: the import stub removes the arguments a
// stdcall import takes, and the fingerprint is the full call sequence WITH arguments, not the set of
// import names reached. 4: the fingerprint also carries the memory the function wrote and which of
// its instructions ran; inputs can set what imported calls return. 25: `steps`, the instructions a
// run took (a rebuild that ends where the original was cut off is judged by it).
extern const int64_t REV;

struct Fingerprint {
    std::optional<int64_t> ret;          // the result, None for a run that did not return
    py::str exit;                        // how it ended ("ret", "fault", "truncated", "wild-call" ...)
    bool truncated = false;
    py::list<py::str> deps;              // its calls and effects out, as text in order
    std::optional<py::str> writes;       // the memory it left changed (lane_common.writes), None: more than is recorded
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

// What a lane's input has that this module reads.
struct Input {
    py::list<int64_t> args;
    std::optional<int64_t> stub_ret;
};

// What compare() tells its caller besides the verdict.
struct Info {
    std::optional<int64_t> ret_bits;     // 16 or 8: the part of the return value that is a leftover (0: not a known part)
    std::optional<py::str> detail;       // one line saying what differed
};

// The first observable difference between two fingerprints of the same input, or ''.
// `ret_mask` marks bits of the return value that are leftovers, not results.
py::str first_difference(const Fingerprint& a, const Fingerprint& b, int64_t ret_mask = 0);

// One line saying exactly what differed between two fingerprints, for triage by cause:
// which call first differs and how, or the two return values / exits.
py::str difference_detail(const Fingerprint& a, const Fingerprint& b, const std::optional<Input>& v = std::nullopt,
                          int64_t ret_mask = 0);

template <class V> using FingerprintOf = std::function<Fingerprint(const V&)>;

// Judge one body. `fp_retail(v)` / `fp_rebuilt(v)` fingerprint each side on input `v`.
// Returns (verdict, reason, limits). With `insns` (the original function's instruction count),
// the share of them that ran on some input is appended to limits as `coverage=NN`.
//
// Stops at the first input on which the two sides differ -- one difference settles it, and most
// bodies that fail do so on the first input. A body that matches every basic input with at least
// one clean return is then run on the `confirm` inputs too.
//
// `limits` are the reasons a match is weaker than it looks, as codes that can be counted:
//   fault-only      no input returned cleanly; the two sides only fault/stop alike
//   truncated       a trace hit the step or time cap
//   argc-unknown    an imported call was compared by name only (its argument count is unknown)
//   calls-capped    the call sequence was longer than is compared
//   unobserved      some inputs could not be traced on one side
//   writes-capped   a trace wrote more memory than is recorded, so its writes were not compared
//   low-coverage    under half of the original's instructions ran on any input
template <class V>
std::tuple<py::str, py::str, py::list<py::str>> compare(
    const FingerprintOf<V>& fp_retail, const FingerprintOf<V>& fp_rebuilt, const py::list<V>& vectors,
    const py::list<V>& confirm = py::list<V>(), int64_t insns = 0,
    const std::function<py::list<V>(const py::set<int64_t>&, const py::set<int64_t>&)>& directed = nullptr, Info* info = nullptr,
    const std::function<int64_t(const FingerprintOf<V>&, const V&, const Fingerprint&)>& leftover = nullptr) {
    static_assert(std::is_base_of<Input, V>::value, "an input of compare() is an Input (args, stub_ret) and what the lane adds");
    py::set<py::str> limits;
    py::set<int64_t> covered;
    py::set<int64_t> reached;
    int64_t seen = 0, clean = 0;
    py::set<py::str> errs;

    auto run = [&](const py::list<V>& vs) -> py::str {
        for (const V& v : vs) {
            Fingerprint a = fp_retail(v);
            if (a.err) {                  // nothing to compare with: the rebuilt side is not run
                limits.add(py::str("unobserved"));
                errs.add(py::str("retail: ") + *a.err);
                continue;
            }
            Fingerprint b = fp_rebuilt(v);
            if (a.err || b.err) {
                limits.add(py::str("unobserved"));
                if (a.err) {
                    errs.add(py::str("retail: ") + *a.err);
                }
                if (b.err) {
                    errs.add(py::str("rebuilt: ") + *b.err);
                }
                continue;
            }
            ++seen;
            py::str d = first_difference(a, b);
            int64_t mask = 0;
            if (d == py::str("ret-differs")) {
                mask = leftover ? leftover(fp_retail, v, a) : 0;
                if (mask) {
                    d = first_difference(a, b, mask);
                    if (info != nullptr) {
                        info->ret_bits = mask == 0xFFFF0000 ? 16 : mask == 0xFFFFFF00 ? 8 : 0;
                    }
                }
            }
            if (!d.empty()) {
                if (info != nullptr) {
                    info->detail = difference_detail(a, b, std::optional<Input>(static_cast<const Input&>(v)), mask);
                }
                return d;
            }
            for (int64_t x : a.cov) {
                covered.add(x);
            }
            for (int64_t x : a.reach) {
                reached.add(x);
            }
            if (!a.writes || !b.writes) {
                limits.add(py::str("writes-capped"));
            }
            if (a.truncated || b.truncated) {
                limits.add(py::str("truncated"));
            } else if (a.exit == py::str("ret")) {
                ++clean;
            }
            for (const py::str& dep : a.deps) {
                if (dep == py::str("...")) {
                    limits.add(py::str("calls-capped"));
                } else if (!dep.contains(py::str("("))) {
                    limits.add(py::str("argc-unknown"));
                }
            }
        }
        return py::str("");
    };
    auto sorted_limits = [&]() {
        py::list<py::str> out = py::list<py::str>::from(limits);
        std::sort(out.vec().begin(), out.vec().end(), [](const py::str& x, const py::str& y) { return x.u32() < y.u32(); });
        return out;
    };

    py::str d = run(vectors);
    if (!d.empty()) {
        return {py::str(verdicts::DIVERGENT), d, py::list<py::str>()};
    }
    if (!seen) {
        py::list<py::str> reasons = py::list<py::str>::from(errs);
        std::sort(reasons.vec().begin(), reasons.vec().end(), [](const py::str& x, const py::str& y) { return x.u32() < y.u32(); });
        return {py::str(verdicts::INCONCLUSIVE), py::join(py::str("; "), reasons), sorted_limits()};
    }
    if (clean && !confirm.empty()) {
        d = run(confirm);
        if (!d.empty()) {
            return {py::str(verdicts::DIVERGENT), d, py::list<py::str>()};
        }
    }
    // `directed(covered, reached)` supplies inputs aimed at code no input has reached yet -- also
    // for a body no input has yet returned from: an input that gets it past what makes it fault
    // is what turns "only ever fails alike" into a comparison of what it does.
    if (directed) {
        int64_t before = clean;
        d = run(directed(covered, reached));
        if (!d.empty()) {
            return {py::str(verdicts::DIVERGENT), d, py::list<py::str>()};
        }
        if (clean && !before && !confirm.empty()) {
            d = run(confirm);
            if (!d.empty()) {
                return {py::str(verdicts::DIVERGENT), d, py::list<py::str>()};
            }
        }
    }
    if (insns) {
        int64_t pct = std::min<int64_t>(100, py::floordiv(100 * covered.size(), insns));
        if (pct < 50) {
            limits.add(py::str("low-coverage"));
        }
        limits.add(py::percent("coverage=%d", pct));
    }
    if (!clean) {
        limits.add(py::str("fault-only"));
        return {py::str(verdicts::CONSISTENT), py::str(""), sorted_limits()};
    }
    return {py::str(verdicts::EQUIVALENT), py::str(""), sorted_limits()};
}

// Fingerprints are kept in the one cache (base/cache.py), kind "fingerprints", its generation
// REV: a fingerprint made under another revision of the comparer is dropped, not kept for ever,
// and all of them count toward the cache's cap. A write that loses the lock is swallowed
// (traced again next time), so the cache never affects a verdict.
std::optional<Fingerprint> cache_get(const py::str& key);
void cache_put(const py::str& key, const Fingerprint& fp);

// A key for one run: everything that decides what the run does (the module's content hash,
// the function, the input, anything else the kind varies), plus the comparer's revision.
py::str cache_key(const std::vector<base::cache::Part>& parts);
// ... the same where the caller has the text of repr(parts) (the tuple of the arguments) already
py::str cache_key_repr(const py::str& parts_repr);

}  // namespace codebreaker::verify::compare
