// cb/verify/compare.cpp -- see compare.hpp.
#include "cb/verify/compare.hpp"

#include <algorithm>
#include <format>

#include "cb/core/error.hpp"
#include "cb/core/io.hpp"
#include "cb/core/text.hpp"

namespace cb::verify::compare {

std::string_view reason(Difference d) {
    switch (d) {
        case Difference::None: return "";
        case Difference::Exit: return "exit-differs";
        case Difference::Deps: return "deps-differ";
        case Difference::Ret: return "ret-differs";
        case Difference::Writes: return "writes-differ";
        case Difference::Stack: return "stack-differs";
        case Difference::Registers: return "registers-differ";
    }
    return "";
}

Difference first_difference(const Fingerprint& a, const Fingerprint& b, std::uint32_t ret_mask) {
    // The original (a) cut off at the step limit did only part of its work, and where it stopped is the harness's
    // doing: the rebuild must have made every call it made by then, in order, but how far either got is not
    // comparable (a C loop takes more steps than `rep stosd`). A rebuild that ENDED where the original was still
    // going ended early, unless it had run more steps than the original was given (`steps`, where the lane records it).
    if (a.exit == kExitTruncated && b.exit != kExitWildCall) {
        // (a list cut at its cap says so, "...", in both runs alike)
        const bool same_start = b.deps.size() >= a.deps.size() && std::equal(a.deps.begin(), a.deps.end(), b.deps.begin());
        if (!same_start) return Difference::Deps;
        const bool rebuilt_ran_past = a.steps.value_or(0) != 0 && b.steps.value_or(0) > *a.steps;
        if (b.exit != kExitTruncated && !rebuilt_ran_past) return Difference::Exit;
        return Difference::None;
    }
    if (a.exit != b.exit) return Difference::Exit;
    if (a.deps != b.deps) return Difference::Deps;
    // EAX is a return value only when the function returned: at a fault it holds whatever the compiler happened to
    // keep there, which says nothing about the function.
    if (a.exit == kExitReturned) {
        const std::uint64_t ra = static_cast<std::uint64_t>(a.ret.value_or(0));
        const std::uint64_t rb = static_cast<std::uint64_t>(b.ret.value_or(0));
        if (((ra ^ rb) & ~static_cast<std::uint64_t>(ret_mask) & 0xFFFFFFFFull) != 0) return Difference::Ret;
    }
    // A run cut off at the step limit stopped in the middle of something: the two builds spend different numbers of
    // instructions per loop pass, so memory at that instant is not comparable.
    if (a.truncated || b.truncated) return Difference::None;
    if (a.writes && b.writes && *a.writes != *b.writes) return Difference::Writes;
    if (a.pop && b.pop && *a.pop != *b.pop) return Difference::Stack;   // a different calling convention or argument count
    if (a.saved.value_or(false) && b.saved && !*b.saved) return Difference::Registers;   // the original preserves them; this does not
    return Difference::None;
}

namespace {

// The text before the first `sep`.
std::string_view before(std::string_view s, char sep) { return s.substr(0, s.find(sep)); }

// The name of a call "NAME(a,b)" and the text of its arguments.
std::string_view call_args(std::string_view call, std::size_t name_size) {
    return text::trim(call.substr(name_size), "()");
}

std::string writes_head(const std::optional<std::string>& w) {
    return std::string(before((w && !w->empty()) ? std::string_view(*w) : std::string_view("none"), ':'));
}

}  // namespace

std::string difference_detail(const Fingerprint& a, const Fingerprint& b, const Input* input, std::uint32_t ret_mask) {
    const Difference d = first_difference(a, b, ret_mask);
    std::string out;
    switch (d) {
        case Difference::None: return "";
        case Difference::Exit: out = std::format("exit {} vs {}", a.exit, b.exit); break;
        case Difference::Ret: out = std::format("ret {:x} vs {:x}", a.ret.value_or(0), b.ret.value_or(0)); break;
        case Difference::Writes: out = std::format("writes {} vs {}", writes_head(a.writes), writes_head(b.writes)); break;
        case Difference::Stack: out = std::format("removes {} bytes on return vs {}", *a.pop, *b.pop); break;
        case Difference::Registers: out = "saved registers preserved vs changed"; break;
        case Difference::Deps: {
            std::size_t i = 0;
            while (i < a.deps.size() && i < b.deps.size() && a.deps[i] == b.deps[i]) ++i;
            const std::string nothing = "(nothing)";
            const std::string& ra = i < a.deps.size() ? a.deps[i] : nothing;
            const std::string& rb = i < b.deps.size() ? b.deps[i] : nothing;
            const std::string_view na = before(ra, '('), nb = before(rb, '(');
            std::string kind;
            if (rb == nothing) {
                kind = "rebuilt stops calling";
            } else if (ra == nothing) {
                kind = "rebuilt calls extra";
            } else if (na != nb) {
                kind = "different call";
            } else {
                const auto aa = text::split(call_args(ra, na.size()), ',');
                const auto ab = text::split(call_args(rb, nb.size()), ',');
                std::vector<std::string> bad;
                for (std::size_t k = 0; k < std::min(aa.size(), ab.size()); ++k) {
                    if (aa[k] != ab[k]) bad.push_back(std::to_string(k));
                }
                kind = std::format("same call, argument {} differs", text::join(bad, "+"));
            }
            out = std::format("call #{} {}: {} vs {}", i, kind, ra, rb);
            break;
        }
    }
    if (input != nullptr) {
        std::vector<std::string> args;
        for (std::int64_t x : input->args) args.push_back(std::format("{:x}", x));
        out += std::format("  [input {}{}]", text::join(args, ","),
                           input->stub_ret.value_or(0) != 0 ? std::format(" ret={:x}", *input->stub_ret) : std::string());
    }
    if (out.size() > 300) out.resize(300);
    return out;
}

// ---- the evidence -----------------------------------------------------------------------------------------------

void Tally::original_failed(const Fingerprint& a) {
    limits_.insert(WeakLimit::Unobserved);
    errors_.insert("retail: " + *a.err);
}

Difference Tally::observe(const Fingerprint& a, const Fingerprint& b, const Input& input, const std::function<std::uint32_t()>& leftover) {
    if (a.err || b.err) {
        limits_.insert(WeakLimit::Unobserved);
        if (a.err) errors_.insert("retail: " + *a.err);
        if (b.err) errors_.insert("rebuilt: " + *b.err);
        return Difference::None;
    }
    ++seen_;
    Difference d = first_difference(a, b);
    std::uint32_t mask = 0;
    if (d == Difference::Ret) {
        mask = leftover ? leftover() : 0;
        if (mask != 0) {
            d = first_difference(a, b, mask);
            ret_bits_ = mask == 0xFFFF0000u ? 16 : mask == 0xFFFFFF00u ? 8 : 0;
        }
    }
    if (d != Difference::None) {
        detail_ = difference_detail(a, b, &input, mask);
        return d;
    }
    covered_.insert(a.cov.begin(), a.cov.end());
    reached_.insert(a.reach.begin(), a.reach.end());
    if (!a.writes || !b.writes) limits_.insert(WeakLimit::WritesCapped);
    if (a.truncated || b.truncated) {
        limits_.insert(WeakLimit::Truncated);
    } else if (a.exit == kExitReturned) {
        ++clean_;
    }
    for (const std::string& dep : a.deps) {
        if (dep == "...") limits_.insert(WeakLimit::CallsCapped);
        else if (dep.find('(') == std::string::npos) limits_.insert(WeakLimit::ArgcUnknown);
    }
    return Difference::None;
}

Outcome Tally::diverged(Difference d) && {
    Outcome o;
    o.verdict = Verdict::Divergent;
    o.reason = std::string(reason(d));
    o.ret_bits = ret_bits_;
    o.detail = std::move(detail_);
    return o;
}

Outcome Tally::finish(Options options) && {
    Outcome o;
    o.ret_bits = ret_bits_;
    if (seen_ == 0) {
        o.verdict = Verdict::Inconclusive;
        o.reason = text::join(errors_, "; ");   // (a std::set: sorted)
        o.limits = limits_;
        return o;
    }
    if (options.insns != 0) {
        const std::int64_t pct = std::min<std::int64_t>(100, 100 * static_cast<std::int64_t>(covered_.size()) / options.insns);
        if (pct < 50) limits_.insert(WeakLimit::LowCoverage);
        o.coverage = static_cast<int>(pct);
    }
    o.limits = limits_;
    if (clean_ == 0) {
        o.limits.insert(WeakLimit::FaultOnly);
        o.verdict = Verdict::Consistent;
    } else {
        o.verdict = Verdict::Equivalent;
    }
    return o;
}

std::vector<std::string> Outcome::limit_codes() const {
    std::vector<std::string> codes;
    for (WeakLimit l : limits) codes.emplace_back(word(l));
    if (coverage) codes.push_back(std::format("coverage={}", *coverage));
    std::sort(codes.begin(), codes.end());
    return codes;
}

// ---- the cache ---------------------------------------------------------------------------------------------------

void declare_cache(base::cache::Cache& cache) {
    base::cache::KindSpec spec;
    spec.generation = std::to_string(kRevision);
    cache.declare_kind(kCacheKind, spec);
}

std::optional<Fingerprint> cache_get(base::cache::Cache& cache, std::string_view key) {
    return cache.value<Fingerprint>(kCacheKind, key);
}

void cache_put(base::cache::Cache& cache, std::string_view key, const Fingerprint& fp) {
    cache.keep(kCacheKind, key, fp);
}

std::string cache_key(std::initializer_list<base::cache::KeyPart> parts) {
    return std::format("r{}:{}", kRevision, base::cache::key(parts));
}

std::string cache_key_text(std::string_view text) {
    return std::format("r{}:{}", kRevision, base::cache::key({base::cache::KeyPart(text)}));
}

Bytes encode(const Fingerprint& fp) {
    io::ByteWriter w;
    base::cache::Codec<Fingerprint>::write(w, fp);
    return w.take();
}

std::optional<Fingerprint> decode(ByteView data) {
    try {
        io::ByteReader r(data, "fingerprint");
        return base::cache::Codec<Fingerprint>::read(r);
    } catch (const FormatError&) {   // damaged or short: a miss
        return std::nullopt;
    }
}

}  // namespace cb::verify::compare

namespace cb::base::cache {

using verify::compare::Fingerprint;

void Codec<Fingerprint>::write(io::ByteWriter& w, const Fingerprint& v) {
    Codec<std::optional<std::int64_t>>::write(w, v.ret);
    Codec<std::string>::write(w, v.exit);
    Codec<bool>::write(w, v.truncated);
    Codec<std::vector<std::string>>::write(w, v.deps);
    Codec<std::optional<std::string>>::write(w, v.writes);
    Codec<std::optional<std::int64_t>>::write(w, v.pop);
    Codec<std::optional<bool>>::write(w, v.saved);
    Codec<std::vector<std::int64_t>>::write(w, v.cov);
    Codec<std::vector<std::int64_t>>::write(w, v.reach);
    Codec<std::optional<std::int64_t>>::write(w, v.steps);
    Codec<std::optional<std::string>>::write(w, v.err);
}

Fingerprint Codec<Fingerprint>::read(io::ByteReader& r) {
    Fingerprint v;
    v.ret = Codec<std::optional<std::int64_t>>::read(r);
    v.exit = Codec<std::string>::read(r);
    v.truncated = Codec<bool>::read(r);
    v.deps = Codec<std::vector<std::string>>::read(r);
    v.writes = Codec<std::optional<std::string>>::read(r);
    v.pop = Codec<std::optional<std::int64_t>>::read(r);
    v.saved = Codec<std::optional<bool>>::read(r);
    v.cov = Codec<std::vector<std::int64_t>>::read(r);
    v.reach = Codec<std::vector<std::int64_t>>::read(r);
    v.steps = Codec<std::optional<std::int64_t>>::read(r);
    v.err = Codec<std::optional<std::string>>::read(r);
    return v;
}

}  // namespace cb::base::cache
