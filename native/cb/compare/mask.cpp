// cb/compare/mask.cpp -- see mask.hpp.
#include "cb/compare/mask.hpp"

#include <algorithm>
#include <format>

#include "cb/core/error.hpp"

namespace cb::compare::mask {

namespace {

// Whether some mask leaves byte i out (a mask that ends before i leaves nothing out there).
inline bool left_out(std::span<const ByteMask> masks, std::size_t i) {
    for (const ByteMask& m : masks) {
        if (i < m.size() && m[i] != 0) return true;
    }
    return false;
}

Match verdict_of(std::size_t diff, std::size_t la, std::size_t lb) {
    return diff == 0 ? Match::Identical : la == lb ? Match::SameSize : Match::Different;
}

}  // namespace

std::string_view to_text(Match m) {
    switch (m) {
        case Match::Identical: return "identical";
        case Match::SameSize: return "same size";
        case Match::Different: return "different";
    }
    return "different";
}

bool same(ByteView a, ByteView b, std::span<const ByteMask> masks) {
    if (a.size() != b.size()) return false;
    for (std::size_t i = 0; i < a.size(); ++i) {
        if (a[i] != b[i] && !left_out(masks, i)) return false;
    }
    return true;
}

std::vector<std::size_t> differing(ByteView a, ByteView b, std::span<const ByteMask> masks) {
    std::vector<std::size_t> out;
    const std::size_t n = std::min(a.size(), b.size());
    for (std::size_t i = 0; i < n; ++i) {
        if (a[i] != b[i] && !left_out(masks, i)) out.push_back(i);
    }
    return out;
}

bool same_words(std::span<const std::uint64_t> a, std::span<const std::uint64_t> b, WordMask ignore) {
    const std::size_t n = std::min({a.size(), b.size(), ignore.size()});
    for (std::size_t i = 0; i < n; ++i) {
        if ((a[i] ^ b[i]) & ~ignore[i]) return false;
    }
    return true;
}

DistanceResult distance(ByteView a, ByteView b, ByteMask ma, ByteMask mb) {
    const ByteMask both[2] = {ma, mb};
    const std::size_t n = std::min(a.size(), b.size());
    DistanceResult r;
    for (std::size_t i = 0; i < n; ++i) {
        if (a[i] != b[i] && !left_out(both, i)) {
            ++r.differing;
            if (!r.first) r.first = i;
        }
    }
    const std::size_t extra = std::max(a.size(), b.size()) - n;
    r.differing += extra;
    if (!r.first && extra) r.first = n;
    r.verdict = verdict_of(r.differing, a.size(), b.size());
    r.compared = std::max(a.size(), b.size());
    return r;
}

DistanceResult distance_words(ByteView a, ByteView b, std::size_t unit, ByteOrder order, WordMask ma, WordMask mb) {
    if (unit < 1 || unit > 8) throw UsageError(std::format("distance_words: a unit of {} bytes is not 1..8", unit));
    const std::size_t n = std::min(a.size(), b.size()) / unit;
    const std::uint64_t full = unit == 8 ? ~std::uint64_t{0} : (std::uint64_t{1} << (8 * unit)) - 1;
    auto word = [&](ByteView v, std::size_t i) {
        std::uint64_t w = 0;
        for (std::size_t k = 0; k < unit; ++k) {
            const std::size_t byte = order == ByteOrder::Little ? unit - 1 - k : k;   // most significant first
            w = (w << 8) | v[unit * i + byte];
        }
        return w;
    };
    DistanceResult r;
    for (std::size_t i = 0; i < n; ++i) {
        const std::uint64_t ignore = (i < ma.size() ? ma[i] : 0) | (i < mb.size() ? mb[i] : 0);
        if ((word(a, i) ^ word(b, i)) & ~ignore & full) {
            ++r.differing;
            if (!r.first) r.first = i;
        }
    }
    const std::size_t extra = (std::max(a.size(), b.size()) - std::min(a.size(), b.size())) / unit;
    r.differing += extra;
    if (!r.first && extra) r.first = n;
    r.verdict = verdict_of(r.differing, a.size(), b.size());
    r.compared = std::max(a.size(), b.size()) / unit;
    return r;
}

}  // namespace cb::compare::mask
