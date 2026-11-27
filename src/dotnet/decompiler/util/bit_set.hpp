// dotnet/decompiler/util/bit_set.hpp -- a fixed-size set of bits, and an interval of integers.
//
// Ported from ILSpy (ICSharpCode.Decompiler.Util: BitSet, Interval), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in
// dotnet/decompiler). BitSet: every member of upstream except the enumerator form of SetBits (set_bits returns a vector). An index outside the set
// throws std::out_of_range (upstream's IndexOutOfRangeException).
#pragma once

#include <algorithm>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

namespace cb::dotnet::decompiler::util {

class BitSet {
public:
    explicit BitSet(int capacity) : words_(static_cast<std::size_t>(std::max(1, word_index(capacity + 63)))) {}

    bool operator[](int index) const { return (words_.at(word(index)) & bit(index)) != 0; }
    void set(int index) { words_.at(word(index)) |= bit(index); }
    void clear(int index) { words_.at(word(index)) &= ~bit(index); }
    void assign(int index, bool value) { value ? set(index) : clear(index); }
    bool any() const {
        for (const std::uint64_t w : words_)
            if (w != 0) return true;
        return false;
    }
    bool set_equals(const BitSet& other) const { return words_ == other.words_; }
    void union_with(const BitSet& other) {
        for (std::size_t i = 0; i < words_.size(); ++i) words_[i] |= other.words_[i];
    }
    void intersect_with(const BitSet& other) {
        for (std::size_t i = 0; i < words_.size(); ++i) words_[i] &= other.words_[i];
    }

    // --- the rest of upstream's BitSet (added for the data flow analyses)
    BitSet clone() const { return *this; }
    // Whether all bits i with start_index <= i < end_index are set.
    bool all(int start_index, int end_index) const;
    bool is_subset_of(const BitSet& other) const;
    bool is_superset_of(const BitSet& other) const { return other.is_subset_of(*this); }
    bool is_proper_subset_of(const BitSet& other) const { return is_subset_of(other) && !set_equals(other); }
    bool is_proper_superset_of(const BitSet& other) const { return is_superset_of(other) && !set_equals(other); }
    // Whether at least one bit is set in both bitsets.
    bool overlaps(const BitSet& other) const;
    // Sets all bits i with start_index <= i < end_index.
    void set(int start_index, int end_index);
    // Clears all bits i with start_index <= i < end_index.
    void clear(int start_index, int end_index);
    void clear_all();
    // The first set bit i with start_index <= i < end_index, or -1.
    int next_set_bit(int start_index, int end_index) const;
    // Upstream's SetBits: the set bits in the range, in ascending order.
    std::vector<int> set_bits(int start_index, int end_index) const;
    void replace_with(const BitSet& incoming);
    std::string to_string() const;

private:
    static int word_index(int bit_index) { return bit_index >> 6; }
    static std::size_t word(int index) {
        if (index < 0) throw std::out_of_range("BitSet index");
        return static_cast<std::size_t>(word_index(index));
    }
    static std::uint64_t bit(int index) { return std::uint64_t{1} << (index & 63); }

    std::vector<std::uint64_t> words_;
};

// [start, end): an interval of int (end == start is empty; end may wrap to int.MinValue to include int.MaxValue).
struct Interval {
    int start = 0;
    int end = 0;
    Interval() = default;
    Interval(int start_, int end_);
    int inclusive_end() const { return static_cast<int>(static_cast<std::uint32_t>(end) - 1); }
    bool is_empty() const { return start > inclusive_end(); }
    bool contains(int v) const { return start <= v && v <= inclusive_end(); }
    Interval intersect(Interval other) const;
    std::string to_string() const;
    friend bool operator==(Interval, Interval) = default;
};

}  // namespace cb::dotnet::decompiler::util
