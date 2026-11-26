// dotnet/decompiler/util/long_set.hpp -- sets of 64-bit integers as sorted, disjoint, non-adjacent intervals (a switch section's
// labels): union, intersection, inversion, offset, subset tests.
//
//     LongSet labels(LongInterval(0, 5));      // 0..4
//     LongSet rest = labels.invert();         // everything else
//     labels.contains(3); rest.to_string();   // "[long.MinValue..0),[5..long.MaxValue]"
//
// Ported from ILSpy (ICSharpCode.Decompiler.Util: LongInterval, from Interval.cs; LongSet), Copyright AlphaSierraPapa for the ILSpy
// team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <cstdint>
#include <limits>
#include <string>
#include <vector>

namespace cb::dotnet::decompiler::util {

// [start, end): end == start is empty; end may wrap to long.MinValue to include long.MaxValue.
struct LongInterval {
    std::int64_t start = 0;
    std::int64_t end = 0;
    LongInterval() = default;
    LongInterval(std::int64_t start_, std::int64_t end_);
    static LongInterval inclusive(std::int64_t start, std::int64_t inclusive_end);
    std::int64_t inclusive_end() const { return static_cast<std::int64_t>(static_cast<std::uint64_t>(end) - 1); }
    bool is_empty() const { return start > inclusive_end(); }
    bool contains(std::int64_t v) const { return start <= v && v <= inclusive_end(); }
    LongInterval intersect(LongInterval other) const;
    std::vector<std::int64_t> range() const;
    std::string to_string() const;
    friend bool operator==(LongInterval, LongInterval) = default;
};

class LongSet {
public:
    LongSet() = default;  // empty
    explicit LongSet(std::int64_t value);
    explicit LongSet(LongInterval interval);
    // Any intervals: the empty ones dropped, the rest sorted and merged.
    explicit LongSet(const std::vector<LongInterval>& intervals);
    static LongSet empty_set() { return LongSet(); }
    static LongSet universe();

    const std::vector<LongInterval>& intervals() const { return intervals_; }
    bool is_empty() const { return intervals_.empty(); }
    // The number of elements (ulong.MaxValue for the universe, which does not fit).
    std::uint64_t count() const;
    bool overlaps(const LongSet& other) const;
    LongSet intersect_with(const LongSet& other) const;
    LongSet union_with(const LongSet& other) const;
    LongSet add_offset(std::int64_t val) const;
    LongSet except_with(const LongSet& other) const;
    LongSet invert() const;
    bool is_subset_of(const LongSet& other) const;
    bool is_superset_of(const LongSet& other) const;
    bool is_proper_subset_of(const LongSet& other) const;
    bool is_proper_superset_of(const LongSet& other) const;
    bool contains(std::int64_t val) const;
    std::vector<std::int64_t> values() const;
    LongInterval containing_interval() const;
    bool set_equals(const LongSet& other) const { return intervals_ == other.intervals_; }
    std::string to_string() const;

private:
    struct Sorted {};
    LongSet(Sorted, std::vector<LongInterval> intervals) : intervals_(std::move(intervals)) {}
    std::vector<LongInterval> do_intersect_with(const LongSet& other) const;
    int upper_bound(std::int64_t val) const;
    static std::vector<LongInterval> merge_overlapping(const std::vector<LongInterval>& input);

    std::vector<LongInterval> intervals_;
};

}  // namespace cb::dotnet::decompiler::util
