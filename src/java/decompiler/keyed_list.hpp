// java/decompiler/keyed_list.hpp -- a list whose elements may each have a key: kept in order like a vector, found by key like a map (the keys follow
// their elements as elements are inserted, removed or swapped). The decompiler keeps a class's fields and methods, a statement's successors and many
// more in one.
//
//     KeyedList<StructMethod*, std::string> methods;
//     methods.add_with_key(m, "run ()V");
//     methods.get_with_key("run ()V");   // m; a missing key: the element type's empty value
//
// Ported from Fernflower (de.fernflower.util.VBStyleCollection), Copyright 2000-2015 JetBrains s.r.o., Apache License 2.0 (see NOTICE in this folder).
// An element added without a key has none (std::nullopt).
#pragma once

#include <cstddef>
#include <optional>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <vector>

namespace cb::java::decompiler {

template <class E, class K>
class KeyedList {
public:
    std::size_t size() const { return items_.size(); }
    bool empty() const { return items_.empty(); }
    E& operator[](std::size_t i) { return items_[i]; }
    const E& operator[](std::size_t i) const { return items_[i]; }
    E& get(std::size_t i) { return items_.at(i); }
    const E& get(std::size_t i) const { return items_.at(i); }
    E& last() { return items_.back(); }
    auto begin() { return items_.begin(); }
    auto end() { return items_.end(); }
    auto begin() const { return items_.begin(); }
    auto end() const { return items_.end(); }
    const std::vector<E>& items() const { return items_; }

    void add(E element) {
        keys_.push_back(std::nullopt);
        items_.push_back(std::move(element));
    }
    void add(std::size_t index, E element) {
        shift_from(index, 1);
        keys_.insert(keys_.begin() + static_cast<std::ptrdiff_t>(index), std::nullopt);
        items_.insert(items_.begin() + static_cast<std::ptrdiff_t>(index), std::move(element));
    }
    void add_all(const std::vector<E>& elements) {
        for (const E& e : elements) add(e);
    }
    void add_with_key(E element, K key) {
        map_[key] = items_.size();
        items_.push_back(std::move(element));
        keys_.push_back(std::move(key));
    }
    void add_all_with_key(const KeyedList& other) {
        for (std::size_t i = 0; i < other.size(); ++i) {
            if (other.keys_[i]) add_with_key(other.items_[i], *other.keys_[i]);
            else add(other.items_[i]);   // (the original adds it under a null key)
        }
    }
    void add_with_key_and_index(std::size_t index, E element, K key) {
        shift_from(index, 1);
        map_[key] = index;
        items_.insert(items_.begin() + static_cast<std::ptrdiff_t>(index), std::move(element));
        keys_.insert(keys_.begin() + static_cast<std::ptrdiff_t>(index), std::move(key));
    }
    // Replaces the element of that key (returning the old one), or adds it when the key is new (returning nullopt).
    std::optional<E> put_with_key(E element, const K& key) {
        const auto it = map_.find(key);
        if (it == map_.end()) {
            add_with_key(std::move(element), key);
            return std::nullopt;
        }
        E old = std::move(items_[it->second]);
        items_[it->second] = std::move(element);
        return old;
    }
    void remove_with_key(const K& key) {
        const std::size_t index = map_.at(key);
        shift_from(index + 1, -1);
        items_.erase(items_.begin() + static_cast<std::ptrdiff_t>(index));
        keys_.erase(keys_.begin() + static_cast<std::ptrdiff_t>(index));
        map_.erase(key);
    }
    E remove(std::size_t index) {
        shift_from(index + 1, -1);
        if (keys_.at(index)) map_.erase(*keys_[index]);
        keys_.erase(keys_.begin() + static_cast<std::ptrdiff_t>(index));
        E old = std::move(items_[index]);
        items_.erase(items_.begin() + static_cast<std::ptrdiff_t>(index));
        return old;
    }
    // The element of that key; the element type's empty value when there is none (the original's null).
    E get_with_key(const K& key) const {
        const auto it = map_.find(key);
        return it == map_.end() ? E{} : items_[it->second];
    }
    const E* find(const K& key) const {
        const auto it = map_.find(key);
        return it == map_.end() ? nullptr : &items_[it->second];
    }
    std::size_t index_by_key(const K& key) const { return map_.at(key); }
    bool contains_key(const K& key) const { return map_.contains(key); }
    const std::optional<K>& key(std::size_t index) const { return keys_.at(index); }
    const std::vector<std::optional<K>>& keys() const { return keys_; }
    void clear() {
        map_.clear();
        keys_.clear();
        items_.clear();
    }
    void swap(std::size_t a, std::size_t b) {
        std::swap(items_.at(a), items_.at(b));
        std::swap(keys_[a], keys_[b]);
        if (keys_[a]) map_[*keys_[a]] = a;
        if (keys_[b]) map_[*keys_[b]] = b;
    }

private:
    void shift_from(std::size_t index, std::ptrdiff_t diff) {
        for (std::size_t i = keys_.size(); i-- > index;)
            if (keys_[i]) map_[*keys_[i]] = static_cast<std::size_t>(static_cast<std::ptrdiff_t>(i) + diff);
    }

    std::vector<E> items_;
    std::vector<std::optional<K>> keys_;
    std::unordered_map<K, std::size_t> map_;
};

// A member's key: "name descriptor".
inline std::string unique_key(const std::string& name, const std::string& descriptor) { return name + " " + descriptor; }

}  // namespace cb::java::decompiler
