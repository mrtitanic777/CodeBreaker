// dotnet/decompiler/util/insertion_ordered_map.hpp -- a map that enumerates in the order its keys were first added, as a C# Dictionary
// does while nothing has been removed (a removal closes the gap; upstream's slot reuse after a removal is not reproduced). The ILAst
// transforms use dictionaries whose iteration order shows in the output (the order variables are declared in, the order steps run in).
//
//     InsertionOrderedMap<const IField*, Info*> map;
//     map.set(field, info);                     // upstream's map[field] = info (an existing key keeps its place)
//     for (const auto& [key, value] : map.items()) ...
//
// Ported from: no upstream file (stands in for System.Collections.Generic.Dictionary where enumeration order matters), part of the ILSpy
// port to dotnet/decompiler (see NOTICE).
#pragma once

#include <cstddef>
#include <functional>
#include <unordered_map>
#include <utility>
#include <vector>

namespace cb::dotnet::decompiler::util {

template <class Key, class Value, class Hash = std::hash<Key>>
class InsertionOrderedMap {
public:
    // The value of `key`, or nullptr (upstream's TryGetValue).
    Value* find(const Key& key) {
        auto it = index_.find(key);
        return it == index_.end() ? nullptr : &items_[it->second].second;
    }
    const Value* find(const Key& key) const {
        auto it = index_.find(key);
        return it == index_.end() ? nullptr : &items_[it->second].second;
    }
    bool contains(const Key& key) const { return index_.contains(key); }
    int count() const { return static_cast<int>(items_.size()); }
    bool empty() const { return items_.empty(); }

    // map[key] = value
    void set(const Key& key, Value value) {
        auto it = index_.find(key);
        if (it != index_.end()) {
            items_[it->second].second = std::move(value);
            return;
        }
        index_.emplace(key, items_.size());
        items_.emplace_back(key, std::move(value));
    }
    // The value of `key`, added with a default value when it is not there yet.
    Value& get_or_add(const Key& key) {
        auto it = index_.find(key);
        if (it != index_.end()) return items_[it->second].second;
        index_.emplace(key, items_.size());
        items_.emplace_back(key, Value());
        return items_.back().second;
    }
    bool erase(const Key& key) {
        auto it = index_.find(key);
        if (it == index_.end()) return false;
        items_.erase(items_.begin() + static_cast<std::ptrdiff_t>(it->second));
        index_.clear();
        for (std::size_t i = 0; i < items_.size(); ++i) index_.emplace(items_[i].first, i);
        return true;
    }
    void clear() {
        items_.clear();
        index_.clear();
    }
    // The entries in order (references stay valid until the next add or removal; take a copy to change the map while walking).
    const std::vector<std::pair<Key, Value>>& items() const { return items_; }
    std::vector<std::pair<Key, Value>>& items() { return items_; }
    std::vector<Value> values() const {
        std::vector<Value> result;
        result.reserve(items_.size());
        for (const auto& item : items_) result.push_back(item.second);
        return result;
    }

private:
    std::vector<std::pair<Key, Value>> items_;
    std::unordered_map<Key, std::size_t, Hash> index_;
};

}  // namespace cb::dotnet::decompiler::util
