// cb/core/ordered_map.hpp -- a map that iterates in insertion order.
//
// For the maps whose *insertion order is the meaning*: a function's locals in declaration order, JSON written in
// the order the producer chose, a build's steps. Lookup is O(1) through a hash index; iteration is over a plain
// vector, so it is stable and cache-friendly; erase is O(n) (it renumbers the index) and rare by design.
// Re-assigning an existing key keeps its original position (like an overwritten Python dict entry).
// See docs/CPP_DESIGN.md 3.5 for when to choose this over std::map / std::unordered_map.
#pragma once

#include <cstddef>
#include <functional>
#include <initializer_list>
#include <stdexcept>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

namespace cb {

template <class Key, class Value, class Hash = std::hash<Key>, class KeyEqual = std::equal_to<Key>>
class OrderedMap {
public:
    using value_type = std::pair<Key, Value>;
    using iterator = typename std::vector<value_type>::iterator;
    using const_iterator = typename std::vector<value_type>::const_iterator;

    OrderedMap() = default;
    OrderedMap(std::initializer_list<value_type> init) {
        for (const auto& kv : init) insert_or_assign(kv.first, kv.second);
    }

    std::size_t size() const { return items_.size(); }
    bool empty() const { return items_.empty(); }
    void clear() { items_.clear(); index_.clear(); }
    void reserve(std::size_t n) { items_.reserve(n); index_.reserve(n); }

    bool contains(const Key& key) const { return index_.find(key) != index_.end(); }

    // The value for the key, or nullptr. The pointer is invalidated by any insertion or erase.
    Value* find(const Key& key) {
        auto it = index_.find(key);
        return it == index_.end() ? nullptr : &items_[it->second].second;
    }
    const Value* find(const Key& key) const {
        auto it = index_.find(key);
        return it == index_.end() ? nullptr : &items_[it->second].second;
    }

    // The value for a key that must be present; absence is a bug (std::out_of_range).
    Value& at(const Key& key) {
        Value* v = find(key);
        if (v == nullptr) throw std::out_of_range("OrderedMap::at: key not present");
        return *v;
    }
    const Value& at(const Key& key) const {
        const Value* v = find(key);
        if (v == nullptr) throw std::out_of_range("OrderedMap::at: key not present");
        return *v;
    }

    // Insert a default-constructed value at the end if the key is new.
    Value& operator[](const Key& key) { return try_emplace(key).first->second; }

    // Insert at the end if the key is new; otherwise leave the existing entry. Returns (entry, inserted).
    template <class... Args>
    std::pair<iterator, bool> try_emplace(const Key& key, Args&&... args) {
        auto [it, fresh] = index_.try_emplace(key, items_.size());
        if (!fresh) return {items_.begin() + static_cast<std::ptrdiff_t>(it->second), false};
        items_.emplace_back(std::piecewise_construct, std::forward_as_tuple(key),
                            std::forward_as_tuple(std::forward<Args>(args)...));
        return {items_.end() - 1, true};
    }

    // Set the value; a key that is already present keeps its position.
    template <class V>
    void insert_or_assign(const Key& key, V&& value) {
        if (Value* existing = find(key)) {
            *existing = std::forward<V>(value);
        } else {
            try_emplace(key, std::forward<V>(value));
        }
    }

    // Remove the key; true if it was present. O(n): later entries move up one place.
    bool erase(const Key& key) {
        auto it = index_.find(key);
        if (it == index_.end()) return false;
        const std::size_t pos = it->second;
        items_.erase(items_.begin() + static_cast<std::ptrdiff_t>(pos));
        index_.erase(it);
        for (auto& [k, p] : index_) {
            if (p > pos) --p;
        }
        return true;
    }

    // Iteration is in insertion order. Do not modify `first` of a yielded entry (it is the index's key).
    iterator begin() { return items_.begin(); }
    iterator end() { return items_.end(); }
    const_iterator begin() const { return items_.begin(); }
    const_iterator end() const { return items_.end(); }
    const std::vector<value_type>& entries() const { return items_; }

    friend bool operator==(const OrderedMap& a, const OrderedMap& b) { return a.items_ == b.items_; }

private:
    std::vector<value_type> items_;
    std::unordered_map<Key, std::size_t, Hash, KeyEqual> index_;
};

// dict.get(k, default) for any map type: the value, or the fallback copy.
template <class Map, class Key, class Fallback>
auto get_or(const Map& map, const Key& key, Fallback&& fallback) {
    using V = typename Map::mapped_type;
    auto it = map.find(key);
    return it == map.end() ? static_cast<V>(std::forward<Fallback>(fallback)) : it->second;
}

template <class Key, class Value, class Hash, class Eq, class Fallback>
Value get_or(const OrderedMap<Key, Value, Hash, Eq>& map, const Key& key, Fallback&& fallback) {
    const Value* v = map.find(key);
    return v != nullptr ? *v : static_cast<Value>(std::forward<Fallback>(fallback));
}

// A pointer to the mapped value, or nullptr ("absent is normal" without an exception).
template <class Map, class Key>
auto find_ptr(const Map& map, const Key& key) -> const typename Map::mapped_type* {
    auto it = map.find(key);
    return it == map.end() ? nullptr : &it->second;
}

template <class Key, class Value, class Hash, class Eq>
const Value* find_ptr(const OrderedMap<Key, Value, Hash, Eq>& map, const Key& key) {
    return map.find(key);
}

}  // namespace cb
