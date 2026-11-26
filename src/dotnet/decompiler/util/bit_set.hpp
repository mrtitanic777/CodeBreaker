// dotnet/decompiler/util/bit_set.hpp -- a fixed-size set of bits, and an interval of integers.
//
// Ported from ILSpy (ICSharpCode.Decompiler.Util: BitSet, Interval), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in
// dotnet/decompiler). BitSet: the members up to set ranges; the rest come with the IL AST, which uses them. An index outside the set
// throws std::out_of_range (upstream's IndexOutOfRangeException).
#pragma once

#include <cstdint>
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
