// dotnet/decompiler/util/bit_set.cpp -- Interval and the range, subset and search members of BitSet (ICSharpCode.Decompiler.Util).
#include "dotnet/decompiler/util/bit_set.hpp"

#include <algorithm>
#include <cassert>
#include <bit>
#include <format>

#include "core/error.hpp"

namespace cb::dotnet::decompiler::util {

// ---- BitSet (the members added for the data flow analyses) ----

namespace {
constexpr std::uint64_t kMask = ~std::uint64_t{0};
// upstream's `Mask >> -endIndex`: C# masks the shift count to six bits, the same as `Mask >> (64 - (endIndex % 64))`
std::uint64_t end_mask(int end_index) { return kMask >> (static_cast<unsigned>(-end_index) & 63u); }
std::uint64_t start_mask(int start_index) { return kMask << (static_cast<unsigned>(start_index) & 63u); }
}  // namespace

bool BitSet::all(int start_index, int end_index) const {
    assert(start_index <= end_index);
    if (start_index >= end_index) return true;
    const std::size_t start_word = word(start_index);
    const std::size_t end_word = word(end_index - 1);
    const std::uint64_t smask = start_mask(start_index);
    const std::uint64_t emask = end_mask(end_index);
    if (start_word == end_word) return (words_.at(start_word) & (smask & emask)) == (smask & emask);
    if ((words_.at(start_word) & smask) != smask) return false;
    for (std::size_t i = start_word + 1; i < end_word; ++i)
        if (words_.at(i) != ~std::uint64_t{0}) return false;
    return (words_.at(end_word) & emask) == emask;
}

bool BitSet::is_subset_of(const BitSet& other) const {
    for (std::size_t i = 0; i < words_.size(); ++i)
        if ((words_[i] & ~other.words_.at(i)) != 0) return false;
    return true;
}

bool BitSet::overlaps(const BitSet& other) const {
    for (std::size_t i = 0; i < words_.size(); ++i)
        if ((words_[i] & other.words_.at(i)) != 0) return true;
    return false;
}

void BitSet::set(int start_index, int end_index) {
    assert(start_index <= end_index);
    if (start_index >= end_index) return;
    const std::size_t start_word = word(start_index);
    const std::size_t end_word = word(end_index - 1);
    const std::uint64_t smask = start_mask(start_index);
    const std::uint64_t emask = end_mask(end_index);
    if (start_word == end_word) {
        words_.at(start_word) |= (smask & emask);
    } else {
        words_.at(start_word) |= smask;
        for (std::size_t i = start_word + 1; i < end_word; ++i) words_.at(i) = ~std::uint64_t{0};
        words_.at(end_word) |= emask;
    }
}

void BitSet::clear(int start_index, int end_index) {
    assert(start_index <= end_index);
    if (start_index >= end_index) return;
    const std::size_t start_word = word(start_index);
    const std::size_t end_word = word(end_index - 1);
    const std::uint64_t smask = start_mask(start_index);
    const std::uint64_t emask = end_mask(end_index);
    if (start_word == end_word) {
        words_.at(start_word) &= ~(smask & emask);
    } else {
        words_.at(start_word) &= ~smask;
        for (std::size_t i = start_word + 1; i < end_word; ++i) words_.at(i) = 0;
        words_.at(end_word) &= ~emask;
    }
}

void BitSet::clear_all() { std::fill(words_.begin(), words_.end(), std::uint64_t{0}); }

int BitSet::next_set_bit(int start_index, int end_index) const {
    assert(start_index <= end_index);
    if (start_index >= end_index) return -1;
    const std::size_t start_word = word(start_index);
    const std::size_t end_word = word(end_index - 1);
    const std::uint64_t smask = start_mask(start_index);
    const std::uint64_t emask = end_mask(end_index);
    if (start_word == end_word) {
        const std::uint64_t masked = words_.at(start_word) & smask & emask;
        if (masked != 0) return static_cast<int>(start_word * 64) + std::countr_zero(masked);
    } else {
        std::uint64_t masked = words_.at(start_word) & smask;
        if (masked != 0) return static_cast<int>(start_word * 64) + std::countr_zero(masked);
        for (std::size_t i = start_word + 1; i < end_word; ++i) {
            masked = words_.at(i);
            if (masked != 0) return static_cast<int>(i * 64) + std::countr_zero(masked);
        }
        masked = words_.at(end_word) & emask;
        if (masked != 0) return static_cast<int>(end_word * 64) + std::countr_zero(masked);
    }
    return -1;
}

std::vector<int> BitSet::set_bits(int start_index, int end_index) const {
    std::vector<int> result;
    while (true) {
        const int next = next_set_bit(start_index, end_index);
        if (next == -1) break;
        result.push_back(next);
        start_index = next + 1;
    }
    return result;
}

void BitSet::replace_with(const BitSet& incoming) {
    assert(words_.size() == incoming.words_.size());
    words_ = incoming.words_;
}

std::string BitSet::to_string() const {
    std::string b = "{";
    for (int i = 0; i < static_cast<int>(words_.size()) * 64; ++i) {
        if ((*this)[i]) {
            if (b.size() > 1) b += ", ";
            if (b.size() > 500) {
                b += "...";
                break;
            }
            b += std::to_string(i);
        }
    }
    b += '}';
    return b;
}

Interval::Interval(int start_, int end_) : start(start_), end(end_) {
    if (!(start_ <= static_cast<int>(static_cast<std::uint32_t>(end_) - 1) || start_ == end_)) throw UsageError("Interval: the end must be after the start");
}

Interval Interval::intersect(Interval other) const {
    const int s = std::max(start, other.start);
    const int e = std::min(inclusive_end(), other.inclusive_end());
    if (s <= e) return Interval(s, static_cast<int>(static_cast<std::uint32_t>(e) + 1));
    return Interval();
}

std::string Interval::to_string() const {
    if (end == INT32_MIN) return std::format("[{}..int.MaxValue]", start);
    return std::format("[{}..{})", start, end);
}

}  // namespace cb::dotnet::decompiler::util
