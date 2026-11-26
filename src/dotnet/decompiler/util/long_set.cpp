// dotnet/decompiler/util/long_set.cpp -- see long_set.hpp.
#include "dotnet/decompiler/util/long_set.hpp"

#include <algorithm>
#include <stdexcept>

namespace cb::dotnet::decompiler::util {

namespace {
constexpr std::int64_t long_min = std::numeric_limits<std::int64_t>::min();
constexpr std::int64_t long_max = std::numeric_limits<std::int64_t>::max();
std::int64_t wrapping_add(std::int64_t a, std::int64_t b) {
    return static_cast<std::int64_t>(static_cast<std::uint64_t>(a) + static_cast<std::uint64_t>(b));
}
}  // namespace

LongInterval::LongInterval(std::int64_t start_, std::int64_t end_) : start(start_), end(end_) {
    if (!(start_ <= wrapping_add(end_, -1) || start_ == end_)) throw std::invalid_argument("The end must be after the start");
}

LongInterval LongInterval::inclusive(std::int64_t start, std::int64_t inclusive_end) {
    if (!(start <= inclusive_end)) throw std::invalid_argument("inclusive_end");
    return LongInterval(start, wrapping_add(inclusive_end, 1));
}

LongInterval LongInterval::intersect(LongInterval other) const {
    std::int64_t s = std::max(start, other.start);
    std::int64_t e = std::min(inclusive_end(), other.inclusive_end());
    if (s <= e) return LongInterval(s, wrapping_add(e, 1));
    return LongInterval();
}

std::vector<std::int64_t> LongInterval::range() const {
    std::vector<std::int64_t> result;
    if (end == long_min) {
        for (std::int64_t i = start;; ++i) {
            result.push_back(i);
            if (i == long_max) break;
        }
    } else {
        for (std::int64_t i = start; i < end; ++i) result.push_back(i);
    }
    return result;
}

std::string LongInterval::to_string() const {
    if (end == long_min) {
        if (start == long_min) return "[long.MinValue..long.MaxValue]";
        return "[" + std::to_string(start) + "..long.MaxValue]";
    }
    if (start == long_min) return "[long.MinValue.." + std::to_string(end) + ")";
    return "[" + std::to_string(start) + ".." + std::to_string(end) + ")";
}

LongSet::LongSet(std::int64_t value) : intervals_{LongInterval::inclusive(value, value)} {}

LongSet::LongSet(LongInterval interval) {
    if (!interval.is_empty()) intervals_.push_back(interval);
}

LongSet::LongSet(const std::vector<LongInterval>& intervals) {
    std::vector<LongInterval> nonempty;
    for (const auto& i : intervals)
        if (!i.is_empty()) nonempty.push_back(i);
    std::stable_sort(nonempty.begin(), nonempty.end(), [](const LongInterval& a, const LongInterval& b) { return a.start < b.start; });
    intervals_ = merge_overlapping(nonempty);
}

LongSet LongSet::universe() { return LongSet(LongInterval::inclusive(long_min, long_max)); }

std::uint64_t LongSet::count() const {
    std::uint64_t count = 0;
    for (const auto& interval : intervals_) count += static_cast<std::uint64_t>(interval.end) - static_cast<std::uint64_t>(interval.start);
    if (count == 0 && !intervals_.empty()) return std::numeric_limits<std::uint64_t>::max();
    return count;
}

std::vector<LongInterval> LongSet::do_intersect_with(const LongSet& other) const {
    std::vector<LongInterval> result;
    std::size_t a = 0, b = 0;
    while (a < intervals_.size() && b < other.intervals_.size()) {
        LongInterval intersection = intervals_[a].intersect(other.intervals_[b]);
        if (!intersection.is_empty()) result.push_back(intersection);
        if (intervals_[a].inclusive_end() < other.intervals_[b].inclusive_end())
            ++a;
        else
            ++b;
    }
    return result;
}

bool LongSet::overlaps(const LongSet& other) const { return !do_intersect_with(other).empty(); }

LongSet LongSet::intersect_with(const LongSet& other) const { return LongSet(Sorted{}, do_intersect_with(other)); }

std::vector<LongInterval> LongSet::merge_overlapping(const std::vector<LongInterval>& input) {
    std::vector<LongInterval> result;
    std::int64_t start = long_min;
    std::int64_t end = long_min;
    bool empty = true;
    for (const auto& element : input) {
        if (!empty && element.start <= end) {
            end = element.end == long_min ? long_min : std::max(end, element.end);
        } else {
            if (!empty)
                result.emplace_back(start, end);
            else
                empty = false;
            start = element.start;
            end = element.end;
        }
        if (end == long_min) break;
    }
    if (!empty) result.emplace_back(start, end);
    return result;
}

LongSet LongSet::union_with(const LongSet& other) const {
    // the two sorted lists merged by start (on a tie, this set's interval first)
    std::vector<LongInterval> merged;
    std::size_t a = 0, b = 0;
    while (a < intervals_.size() && b < other.intervals_.size()) {
        if (intervals_[a].start <= other.intervals_[b].start)
            merged.push_back(intervals_[a++]);
        else
            merged.push_back(other.intervals_[b++]);
    }
    while (a < intervals_.size()) merged.push_back(intervals_[a++]);
    while (b < other.intervals_.size()) merged.push_back(other.intervals_[b++]);
    return LongSet(Sorted{}, merge_overlapping(merged));
}

LongSet LongSet::add_offset(std::int64_t val) const {
    if (val == 0) return *this;
    std::vector<LongInterval> new_intervals;
    for (const auto& element : intervals_) {
        std::int64_t new_start = wrapping_add(element.start, val);
        std::int64_t new_inclusive_end = wrapping_add(element.inclusive_end(), val);
        if (new_start <= new_inclusive_end) {
            new_intervals.push_back(LongInterval::inclusive(new_start, new_inclusive_end));
        } else {
            new_intervals.push_back(LongInterval::inclusive(new_start, long_max));
            new_intervals.push_back(LongInterval::inclusive(long_min, new_inclusive_end));
        }
    }
    std::sort(new_intervals.begin(), new_intervals.end(), [](const LongInterval& a, const LongInterval& b) { return a.start < b.start; });
    return LongSet(Sorted{}, merge_overlapping(new_intervals));
}

LongSet LongSet::except_with(const LongSet& other) const { return intersect_with(other.invert()); }

LongSet LongSet::invert() const {
    if (is_empty()) return universe();
    std::vector<LongInterval> new_intervals;
    std::int64_t prev_end = long_min;  // the previous exclusive end
    for (const auto& interval : intervals_) {
        if (interval.start > prev_end) new_intervals.emplace_back(prev_end, interval.start);
        prev_end = interval.end;
    }
    if (prev_end != long_min) new_intervals.emplace_back(prev_end, long_min);
    return LongSet(Sorted{}, std::move(new_intervals));
}

bool LongSet::is_subset_of(const LongSet& other) const { return union_with(other).set_equals(other); }
bool LongSet::is_superset_of(const LongSet& other) const { return other.is_subset_of(*this); }
bool LongSet::is_proper_subset_of(const LongSet& other) const { return is_subset_of(other) && !set_equals(other); }
bool LongSet::is_proper_superset_of(const LongSet& other) const { return is_superset_of(other) && !set_equals(other); }

bool LongSet::contains(std::int64_t val) const {
    int index = upper_bound(val);
    return index > 0 && intervals_[static_cast<std::size_t>(index - 1)].contains(val);
}

int LongSet::upper_bound(std::int64_t val) const {
    int min = 0, max = static_cast<int>(intervals_.size()) - 1;
    while (max >= min) {
        int m = min + (max - min) / 2;
        const LongInterval& i = intervals_[static_cast<std::size_t>(m)];
        if (val < i.start) {
            max = m - 1;
            continue;
        }
        if (val > i.end) {
            min = m + 1;
            continue;
        }
        return m + 1;
    }
    return min;
}

std::vector<std::int64_t> LongSet::values() const {
    std::vector<std::int64_t> result;
    for (const auto& i : intervals_)
        for (std::int64_t v : i.range()) result.push_back(v);
    return result;
}

LongInterval LongSet::containing_interval() const {
    if (is_empty()) return LongInterval();
    return LongInterval(intervals_.front().start, intervals_.back().end);
}

std::string LongSet::to_string() const {
    std::string result;
    for (std::size_t i = 0; i < intervals_.size(); ++i) {
        if (i > 0) result += ',';
        result += intervals_[i].to_string();
    }
    return result;
}

}  // namespace cb::dotnet::decompiler::util
