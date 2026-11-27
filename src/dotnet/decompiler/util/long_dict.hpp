// dotnet/decompiler/util/long_dict.hpp -- an immutable mapping from 64-bit integers to values, built from sets of integers (the states of
// a state machine and the block each leads to): the first entry that holds a key wins, and a key is found by binary search.
//
//     LongDict<Block*> map = LongDict<Block*>::create({{LongSet(0), block0}, {LongSet(LongInterval(1, 5)), block1}});
//     Block* b = map.get_or_default(3);
//
// Ported from ILSpy (ICSharpCode.Decompiler.Util: LongDict and LongDict<T>, from LongDict.cs), Copyright AlphaSierraPapa for the ILSpy
// team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <algorithm>
#include <cstdint>
#include <utility>
#include <vector>

#include "dotnet/decompiler/util/long_set.hpp"

namespace cb::dotnet::decompiler::util {

template <class T>
class LongDict {
public:
    LongDict() = default;

    // If there are multiple entries for the same key, the dictionary stores the value from the first entry.
    static LongDict create(const std::vector<std::pair<LongSet, T>>& entries) {
        LongDict result;
        LongSet available = LongSet::universe();
        std::vector<std::pair<LongInterval, T>> items;
        for (const auto& [key, val] : entries) {
            // a named set: the range-for must not iterate over the intervals of a temporary that is already gone
            const LongSet claimed = key.intersect_with(available);
            for (const LongInterval& interval : claimed.intervals()) items.emplace_back(interval, val);
            available = available.except_with(key);
        }
        // Array.Sort with a key comparer: the intervals are disjoint, so no two keys start at the same place
        std::stable_sort(items.begin(), items.end(), [](const auto& a, const auto& b) { return a.first.start < b.first.start; });
        result.items_ = std::move(items);
        return result;
    }

    bool try_get_value(std::int64_t key, T* value) const {
        // The last interval that starts at or before the key.
        auto it = std::upper_bound(items_.begin(), items_.end(), key, [](std::int64_t k, const auto& item) { return k < item.first.start; });
        if (it != items_.begin()) {
            --it;
            if (it->first.contains(key)) {
                if (value != nullptr) *value = it->second;
                return true;
            }
        }
        if (value != nullptr) *value = T();
        return false;
    }

    T get_or_default(std::int64_t key) const {
        T value = T();
        try_get_value(key, &value);
        return value;
    }

    auto begin() const { return items_.begin(); }
    auto end() const { return items_.end(); }

private:
    std::vector<std::pair<LongInterval, T>> items_;
};

}  // namespace cb::dotnet::decompiler::util
