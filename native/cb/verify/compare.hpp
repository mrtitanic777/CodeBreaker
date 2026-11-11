// cb/verify/compare.hpp -- judge a reconstruction: do two builds of one function behave alike?
//
//     compare::Outcome o = compare::compare<Input>(run_original, run_rebuilt, inputs, {.insns = n});
//
// `run_original(v)` and `run_rebuilt(v)` run their side on input `v` and return a FINGERPRINT of the run (made from
// the emulator's run record by verify/lane_common): how it ended, what it returned, which calls it made with which
// arguments, what it left in memory, how far the stack moved, whether saved registers were preserved, and which of
// the original's instructions ran. This module knows nothing about processors or file formats: a kind of module (the
// win32 and win16 lanes) knows how to run its code and produce the fingerprint; the comparison, the verdicts and the
// list of reasons a match is weaker than it looks are the same for all of them.
//
//     Equivalent     every input compared equal, and at least one run returned normally
//     Consistent     compared equal, but no run returned (both sides only ever fail alike)
//     Divergent      some input showed a difference; `reason` says which kind
//     Inconclusive   the original could not be observed on any input
//
// It stops at the first input on which the two sides differ: one difference settles it, and most bodies that fail do
// so on the first input. A body that matches every basic input with at least one clean return is then run on the
// `confirm` inputs too. `directed` supplies inputs aimed at code no input has reached yet -- also for a body no input
// has yet returned from: an input that gets it past what makes it fault is what turns "only ever fails alike" into a
// comparison of what it does.
//
// Fingerprints are kept in the one cache (base/cache), kind "fingerprints", whose generation is the comparer's
// revision: a fingerprint made under another revision is dropped, not kept for ever. A write that loses the lock is
// swallowed (the run is made again next time), so the cache never affects a verdict.
#pragma once

#include <concepts>
#include <cstdint>
#include <functional>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/base/cache.hpp"
#include "cb/core/bytes.hpp"
#include "cb/verify/verdicts.hpp"

namespace cb::verify::compare {

// Bump when the oracle changes what a trace of the SAME code observes, so fingerprints cached under the old
// behaviour are not reused. 2: every PE code section is mapped (calls into another code section now run instead of
// being stub-returned). 3: the import stub removes the arguments a stdcall import takes, and the fingerprint is the
// full call sequence WITH arguments, not the set of import names reached. 4: the fingerprint also carries the memory
// the function wrote and which of its instructions ran; inputs can set what imported calls return. 25: `steps`, the
// instructions a run took (a rebuild that ends where the original was cut off is judged by it).
inline constexpr int kRevision = 25;

// How a run ended, in the words the emulator uses; these are the ones the comparer reads.
inline constexpr std::string_view kExitReturned = "ret";
inline constexpr std::string_view kExitTruncated = "truncated";
inline constexpr std::string_view kExitWildCall = "wild-call";

// What one run left behind, as the comparer sees it.
struct Fingerprint {
    std::optional<std::int64_t> ret;     // the result; absent for a run that did not return
    std::string exit;                    // how it ended ("ret", "truncated", "wild-call", "exception: ...")
    bool truncated = false;
    std::vector<std::string> deps;       // its calls and effects out, as text, in order: "API(arg,arg)", or "..." past the cap
    std::optional<std::string> writes;   // the memory it left changed; absent: more than is recorded
    std::optional<std::int64_t> pop;     // the stack's move, for a run that returned
    std::optional<bool> saved;           // whether saved registers were preserved, for a run that returned
    std::vector<std::int64_t> cov;       // the function's own instructions run
    std::vector<std::int64_t> reach;     // every instruction run
    std::optional<std::int64_t> steps;   // instructions the run took, where the lane records it
    std::optional<std::string> err;      // set for a run that was not an observation (it could not be traced)

    friend bool operator==(const Fingerprint&, const Fingerprint&) = default;
};

// What the comparer reads of a lane's input; a lane's own input type derives from it and keeps whatever else it needs.
struct Input {
    std::vector<std::int64_t> args;
    std::optional<std::int64_t> stub_ret;   // what imported calls return; absent or 0: nothing set
};

// ---- two fingerprints of the same input ------------------------------------------------------------------------

enum class Difference { None, Exit, Deps, Ret, Writes, Stack, Registers };

// The stored reason: "exit-differs", "deps-differ", "ret-differs", "writes-differ", "stack-differs",
// "registers-differ"; "" for None.
std::string_view reason(Difference d);

// The first observable difference between two fingerprints of the same input. `ret_mask` marks bits of the return
// value that are leftovers, not results.
Difference first_difference(const Fingerprint& a, const Fingerprint& b, std::uint32_t ret_mask = 0);

// One line saying exactly what differed, for triage by cause: which call first differs and how, or the two return
// values or exits. At most 300 bytes. Empty when there is no difference.
std::string difference_detail(const Fingerprint& a, const Fingerprint& b, const Input* input = nullptr,
                              std::uint32_t ret_mask = 0);

// ---- judging a body ---------------------------------------------------------------------------------------------

// What compare() tells its caller besides the verdict.
struct Outcome {
    Verdict verdict = Verdict::Inconclusive;
    std::string reason;                  // Divergent: the difference; Inconclusive: why nothing was observed
    WeakLimits limits;
    std::optional<int> coverage;         // percent of the original's instructions some input ran (with Options::insns)
    std::optional<int> ret_bits;         // 16 or 8: the part of the return value that is a leftover (0: not a known part)
    std::string detail;                  // Divergent: one line saying what differed

    // The limits as the store keeps them: the weak limits' codes and "coverage=NN", sorted as text.
    std::vector<std::string> limit_codes() const;
};

struct Options {
    std::int64_t insns = 0;              // the original function's instruction count (for the coverage limit)
};

// The evidence gathered across the inputs of one judgement. compare() is its only user; it is a class so that the
// template below stays small.
class Tally {
public:
    // Fold in the two runs of one input. Returns the difference (None: they agree or the input was not an
    // observation); `leftover` is asked, for a return that differs, which bits are leftovers.
    Difference observe(const Fingerprint& a, const Fingerprint& b, const Input& input, const std::function<std::uint32_t()>& leftover);
    void original_failed(const Fingerprint& a);
    bool clean() const { return clean_ > 0; }
    const std::set<std::int64_t>& covered() const { return covered_; }
    const std::set<std::int64_t>& reached() const { return reached_; }
    Outcome finish(Options options) &&;
    Outcome diverged(Difference d) &&;

private:
    WeakLimits limits_;
    std::set<std::int64_t> covered_, reached_;
    std::set<std::string> errors_;
    int seen_ = 0, clean_ = 0;
    std::optional<int> ret_bits_;
    std::string detail_;
};

// Judge one body. `V` is a lane's input type (an Input and whatever else the lane keeps). `leftover(run_original, v,
// a)` returns the mask of the bits of the return value that are leftovers (0: none); `directed(covered, reached)`
// gives the extra inputs (see the top of this file).
template <std::derived_from<Input> V>
Outcome compare(const std::function<Fingerprint(const V&)>& run_original, const std::function<Fingerprint(const V&)>& run_rebuilt,
                std::span<const V> vectors, std::span<const V> confirm = {}, Options options = {},
                const std::function<std::vector<V>(const std::set<std::int64_t>&, const std::set<std::int64_t>&)>& directed = nullptr,
                const std::function<std::uint32_t(const std::function<Fingerprint(const V&)>&, const V&, const Fingerprint&)>& leftover = nullptr) {
    Tally tally;
    auto run = [&](std::span<const V> inputs) -> Difference {
        for (const V& v : inputs) {
            const Fingerprint a = run_original(v);
            if (a.err) {                       // nothing to compare with: the rebuilt side is not run
                tally.original_failed(a);
                continue;
            }
            const Fingerprint b = run_rebuilt(v);
            const Difference d = tally.observe(a, b, v, [&]() -> std::uint32_t { return leftover ? leftover(run_original, v, a) : 0; });
            if (d != Difference::None) return d;
        }
        return Difference::None;
    };
    if (Difference d = run(vectors); d != Difference::None) return std::move(tally).diverged(d);
    // (nothing observed: `finish` says Inconclusive)
    if (tally.clean() && !confirm.empty()) {
        if (Difference d = run(confirm); d != Difference::None) return std::move(tally).diverged(d);
    }
    if (directed) {
        const bool clean_before = tally.clean();
        const std::vector<V> extra = directed(tally.covered(), tally.reached());
        if (Difference d = run(extra); d != Difference::None) return std::move(tally).diverged(d);
        if (tally.clean() && !clean_before && !confirm.empty()) {
            if (Difference d = run(confirm); d != Difference::None) return std::move(tally).diverged(d);
        }
    }
    return std::move(tally).finish(options);
}

// ---- fingerprints in the cache ----------------------------------------------------------------------------------

inline constexpr std::string_view kCacheKind = "fingerprints";

// Say who makes the kind (its generation is the revision above). Before the kind's first use in the process.
void declare_cache(base::cache::Cache& cache);

std::optional<Fingerprint> cache_get(base::cache::Cache& cache, std::string_view key);
void cache_put(base::cache::Cache& cache, std::string_view key, const Fingerprint& fp);

// A key for one run: everything that decides what the run does (the module's content hash, the function, the input,
// anything else the kind varies), plus the comparer's revision.
std::string cache_key(std::initializer_list<base::cache::KeyPart> parts);
// ... where the caller has the text that says it already (a lane whose ingredients are one string).
std::string cache_key_text(std::string_view text);

// The stored form of a fingerprint (the cache's codec), for a caller that holds the bytes itself.
Bytes encode(const Fingerprint& fp);
std::optional<Fingerprint> decode(ByteView data);

}  // namespace cb::verify::compare

namespace cb::base::cache {
template <>
struct Codec<verify::compare::Fingerprint> {
    static constexpr std::uint32_t kSchema = 0x6670'0001;   // "fp", layout 1
    static void write(io::ByteWriter& w, const verify::compare::Fingerprint& v);
    static verify::compare::Fingerprint read(io::ByteReader& r);
};
}  // namespace cb::base::cache
