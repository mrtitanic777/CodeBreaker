// java/decompiler/java_util.hpp -- java.util behaviour the decompiler's output depends on: the order a HashMap or HashSet iterates its keys in (by bucket,
// then by when each key went in, the table doubling as it fills), and a stable sort for the original's Collections.sort calls.
//
//     JavaHashMap<VarVersionPair, std::string, VvHash> names;   // iterates as java.util.HashMap<VarVersionPair, String> does
//     names.put({1, 0}, "var1");
//     for (const auto& [pair, name] : names.entries()) ...
//
// Java's HashMap puts a key in bucket (h ^ (h >>> 16)) & (capacity - 1); a bucket keeps its keys in the order they went in; the table starts at 16 (or
// the next power of two of an initial capacity) and doubles when the size passes three quarters of it, or when a bucket of a table smaller than 64
// reaches nine keys. (A table of 64 or more turns a crowded bucket into a tree, whose order is not kept here: it needs nine keys in one bucket.)
#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <list>
#include <unordered_map>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace cb::java::decompiler {

std::int32_t java_string_hash(std::string_view utf8);   // (cfg.cpp)

struct JavaIntHash {
    std::int32_t operator()(int v) const { return v; }
};
struct JavaStringHash {
    std::int32_t operator()(const std::string& s) const { return java_string_hash(s); }
};

template <class K, class V, class Hash, class Less = std::less<K>>
class JavaHashMap {
public:
    JavaHashMap() = default;
    // new HashMap<>(initialCapacity)
    explicit JavaHashMap(int initial_capacity) : initial_capacity_(table_size_for(std::max(0, initial_capacity))) {}

    // new HashMap<>(other): sized for it, its entries put in its order. (The sizing is the JDK's since 19, which the reference runs on:
    // putMapEntries pre-sizes to ceil(size / 0.75); JDK 8 to size / 0.75 + 1, a table twice as big at some sizes and so another order.)
    static JavaHashMap copy_of(const JavaHashMap& other) {
        JavaHashMap m;
        if (other.size() > 0) m.initial_capacity_ = table_size_for(static_cast<int>(std::ceil(static_cast<double>(other.size()) / 0.75)));
        for (const auto& [k, v] : other.entries()) m.put(k, v);
        return m;
    }

    std::size_t size() const { return index_.size(); }
    bool empty() const { return index_.empty(); }
    bool contains_key(const K& k) const { return index_.contains(k); }
    const V* get(const K& k) const {
        const auto it = index_.find(k);
        return it == index_.end() ? nullptr : &slots_[it->second].value;
    }
    V* get(const K& k) {
        const auto it = index_.find(k);
        return it == index_.end() ? nullptr : &slots_[it->second].value;
    }
    // Returns the value it replaced, if any.
    std::optional<V> put(const K& k, V v) {
        if (const auto it = index_.find(k); it != index_.end()) {
            std::optional<V> old = std::move(slots_[it->second].value);
            slots_[it->second].value = std::move(v);
            return old;
        }
        if (capacity_ == 0) resize(initial_capacity_ > 0 ? initial_capacity_ : 16);
        const std::int32_t h = spread(Hash{}(k));
        // a bucket of nine in a small table: the table doubles instead of becoming a tree
        const int in_bucket = counts_[bucket(h, capacity_)]++;
        slots_.push_back({k, std::move(v), h, next_seq_++, true});
        index_[k] = slots_.size() - 1;
        if (in_bucket >= 8 && capacity_ < 64) resize(capacity_ * 2);
        if (static_cast<double>(index_.size()) > static_cast<double>(capacity_) * 0.75) resize(capacity_ * 2);
        return std::nullopt;
    }
    std::optional<V> remove(const K& k) {
        const auto it = index_.find(k);
        if (it == index_.end()) return std::nullopt;
        Slot& s = slots_[it->second];
        s.live = false;
        --counts_[bucket(s.hash, capacity_)];
        std::optional<V> old = std::move(s.value);
        index_.erase(it);
        return old;
    }
    void clear() {
        slots_.clear();
        index_.clear();
        std::fill(counts_.begin(), counts_.end(), 0);
        // (the table keeps its size, as Java's does)
    }
    // The entries in Java's iteration order.
    std::vector<std::pair<K, V>> entries() const {
        std::vector<const Slot*> live;
        for (const Slot& s : slots_)
            if (s.live) live.push_back(&s);
        const std::size_t cap = capacity_ == 0 ? 16 : capacity_;
        std::stable_sort(live.begin(), live.end(), [cap](const Slot* a, const Slot* b) {
            const auto ba = bucket(a->hash, cap), bb = bucket(b->hash, cap);
            return ba != bb ? ba < bb : a->seq < b->seq;
        });
        std::vector<std::pair<K, V>> out;
        for (const Slot* s : live) out.emplace_back(s->key, s->value);
        return out;
    }
    std::vector<K> keys() const {
        std::vector<K> out;
        for (auto& [k, v] : entries()) out.push_back(k);
        return out;
    }
    std::vector<V> values() const {
        std::vector<V> out;
        for (auto& [k, v] : entries()) out.push_back(v);
        return out;
    }

private:
    struct Slot {
        K key;
        V value;
        std::int32_t hash;
        std::uint64_t seq;
        bool live;
    };
    static std::int32_t spread(std::int32_t h) { return static_cast<std::int32_t>(static_cast<std::uint32_t>(h) ^ (static_cast<std::uint32_t>(h) >> 16)); }
    static std::size_t bucket(std::int32_t h, std::size_t cap) { return static_cast<std::uint32_t>(h) & (cap - 1); }
    static std::size_t table_size_for(int cap) {
        std::size_t n = 1;
        while (static_cast<int>(n) < cap) n <<= 1;
        return n;
    }

    void resize(std::size_t cap) {
        capacity_ = cap;
        counts_.assign(cap, 0);
        for (const Slot& s : slots_)
            if (s.live) ++counts_[bucket(s.hash, cap)];
    }

    std::vector<Slot> slots_;
    std::vector<int> counts_;   // live keys per bucket of the current table
    std::map<K, std::size_t, Less> index_;
    std::size_t capacity_ = 0, initial_capacity_ = 0;
    std::uint64_t next_seq_ = 0;
};

template <class K, class Hash, class Less = std::less<K>>
class JavaHashSet {
public:
    JavaHashSet() = default;
    explicit JavaHashSet(int initial_capacity) : map_(initial_capacity) {}
    // new HashSet<>(collection): sized as the JDK since 19 sizes it (HashMap.newHashMap(max(n, 12)): ceil(max(n, 12) / 0.75); JDK 8's
    // max(n / .75 + 1, 16) makes a table twice as big at some sizes, so another order), then each added in order.
    template <class Range> static JavaHashSet of(const Range& items) {
        const int n = static_cast<int>(std::size(items));
        JavaHashSet s(static_cast<int>(std::ceil(static_cast<double>(std::max(n, 12)) / 0.75)));
        for (const auto& x : items) s.add(x);
        return s;
    }
    bool add(const K& k) { return !map_.put(k, true).has_value(); }
    bool remove(const K& k) { return map_.remove(k).has_value(); }
    bool contains(const K& k) const { return map_.contains_key(k); }
    std::size_t size() const { return map_.size(); }
    bool empty() const { return map_.empty(); }
    void clear() { map_.clear(); }
    std::vector<K> items() const { return map_.keys(); }

private:
    JavaHashMap<K, bool, Hash, Less> map_;
};

// LinkedHashSet, and every HashSet of identity-hashed objects (made LinkedHashSet in the reference): iterates in insertion order; a key added again keeps
// its place; a removed one re-added goes last.
template <class K, class Hash = std::hash<K>, class Eq = std::equal_to<K>>
class LinkedSet {
    // The keys in a vector in the order they went in, a removed one marked dead (its place given back when the set next grows: a removal never moves
    // the others, so it is safe while iterating); looked up by a scan while small (most of these sets are a node's few edges), by a hash index past 16.
public:
    LinkedSet() = default;
    LinkedSet(std::initializer_list<K> items) { for (const K& k : items) add(k); }
    template <class Range> static LinkedSet of(const Range& items) {
        LinkedSet s;
        for (const auto& k : items) s.add(k);
        return s;
    }
    LinkedSet(const LinkedSet& o) {
        keys_.reserve(o.count_);
        for (const K& k : o) add(k);
    }
    LinkedSet& operator=(const LinkedSet& o) {
        if (this != &o) {
            clear();
            for (const K& k : o) add(k);
        }
        return *this;
    }
    LinkedSet(LinkedSet&&) noexcept = default;
    LinkedSet& operator=(LinkedSet&&) noexcept = default;

    bool add(const K& k) {
        if (find(k) != kNone) return false;
        if (keys_.size() >= 2 * count_ + 16) compact();
        keys_.push_back(k);
        live_.push_back(1);
        ++count_;
        if (index_) index_->emplace(k, keys_.size() - 1);
        else if (count_ > kScanLimit) build_index();
        return true;
    }
    template <class Range> bool add_all(const Range& items) {
        bool changed = false;
        for (const auto& k : items) changed |= add(k);
        return changed;
    }
    bool remove(const K& k) {
        const std::size_t at = find(k);
        if (at == kNone) return false;
        live_[at] = 0;
        --count_;
        if (index_) index_->erase(k);
        return true;
    }
    template <class Range> bool remove_all(const Range& items) {
        bool changed = false;
        for (const auto& k : items) changed |= remove(k);
        return changed;
    }
    template <class Pred> bool remove_if(Pred pred) {
        bool changed = false;
        for (std::size_t i = 0; i < keys_.size(); ++i) {
            if (live_[i] && pred(keys_[i])) {
                live_[i] = 0;
                --count_;
                if (index_) index_->erase(keys_[i]);
                changed = true;
            }
        }
        return changed;
    }
    bool retain_all(const LinkedSet& keep) { return remove_if([&](const K& k) { return !keep.contains(k); }); }
    bool contains(const K& k) const { return find(k) != kNone; }
    std::size_t size() const { return count_; }
    bool empty() const { return count_ == 0; }
    void clear() {
        keys_.clear();
        live_.clear();
        index_.reset();
        count_ = 0;
    }
    const K& first() const { return *begin(); }

    class iterator {
    public:
        using value_type = K;
        using difference_type = std::ptrdiff_t;
        using reference = const K&;
        using pointer = const K*;
        using iterator_category = std::forward_iterator_tag;
        iterator() = default;
        iterator(const LinkedSet* s, std::size_t at) : s_(s), at_(at) { skip(); }
        const K& operator*() const { return s_->keys_[at_]; }
        const K* operator->() const { return &s_->keys_[at_]; }
        iterator& operator++() {
            ++at_;
            skip();
            return *this;
        }
        iterator operator++(int) {
            iterator t = *this;
            ++*this;
            return t;
        }
        bool operator==(const iterator& o) const { return at_ == o.at_; }
        bool operator!=(const iterator& o) const { return at_ != o.at_; }

    private:
        void skip() {
            while (at_ < s_->keys_.size() && !s_->live_[at_]) ++at_;
        }
        const LinkedSet* s_ = nullptr;
        std::size_t at_ = 0;
    };
    iterator begin() const { return iterator(this, 0); }
    iterator end() const { return iterator(this, keys_.size()); }
    std::vector<K> items() const {
        std::vector<K> out;
        out.reserve(count_);
        for (const K& k : *this) out.push_back(k);
        return out;
    }
    friend bool operator==(const LinkedSet& a, const LinkedSet& b) {   // Set.equals: the same members, any order
        if (a.size() != b.size()) return false;
        for (const K& k : a) if (!b.contains(k)) return false;
        return true;
    }

private:
    static constexpr std::size_t kNone = static_cast<std::size_t>(-1);
    static constexpr std::size_t kScanLimit = 16;

    std::size_t find(const K& k) const {
        if (index_) {
            const auto it = index_->find(k);
            return it == index_->end() ? kNone : it->second;
        }
        const Eq eq;
        for (std::size_t i = 0; i < keys_.size(); ++i) {
            if (live_[i] && eq(keys_[i], k)) return i;
        }
        return kNone;
    }
    void build_index() {
        index_ = std::make_unique<std::unordered_map<K, std::size_t, Hash, Eq>>();
        index_->reserve(keys_.size() * 2);
        for (std::size_t i = 0; i < keys_.size(); ++i) {
            if (live_[i]) index_->emplace(keys_[i], i);
        }
    }
    // (only from add: an iteration that adds is not one Java allows either)
    void compact() {
        std::size_t to = 0;
        for (std::size_t i = 0; i < keys_.size(); ++i) {
            if (!live_[i]) continue;
            if (to != i) keys_[to] = std::move(keys_[i]);
            live_[to] = 1;
            ++to;
        }
        keys_.resize(to);
        live_.resize(to);
        if (index_) build_index();
    }

    std::vector<K> keys_;
    std::vector<std::uint8_t> live_;
    std::size_t count_ = 0;
    std::unique_ptr<std::unordered_map<K, std::size_t, Hash, Eq>> index_;   // (made when the set grows past kScanLimit: an empty one allocates)
};

// LinkedHashMap, and every HashMap keyed by identity-hashed objects: iterates in insertion order; put on a present key keeps its place.
template <class K, class V, class Hash = std::hash<K>, class Eq = std::equal_to<K>>
class LinkedMap {
public:
    LinkedMap() = default;
    LinkedMap(const LinkedMap& o) { for (const auto& [k, v] : o.order_) put(k, v); }
    LinkedMap& operator=(const LinkedMap& o) {
        if (this != &o) { clear(); for (const auto& [k, v] : o.order_) put(k, v); }
        return *this;
    }
    LinkedMap(LinkedMap&&) noexcept = default;
    LinkedMap& operator=(LinkedMap&&) noexcept = default;

    std::optional<V> put(const K& k, V v) {
        if (const auto it = index_.find(k); it != index_.end()) {
            std::optional<V> old = std::move(it->second->second);
            it->second->second = std::move(v);
            return old;
        }
        order_.emplace_back(k, std::move(v));
        index_.emplace(k, std::prev(order_.end()));
        return std::nullopt;
    }
    V* get(const K& k) {
        const auto it = index_.find(k);
        return it == index_.end() ? nullptr : &it->second->second;
    }
    const V* get(const K& k) const {
        const auto it = index_.find(k);
        return it == index_.end() ? nullptr : &it->second->second;
    }
    // get, or put a default-made value first (the original's `if (map.get(k) == null) map.put(k, new ...)`).
    V& get_or_put(const K& k) {
        if (V* v = get(k)) return *v;
        put(k, V{});
        return order_.back().second;
    }
    bool contains_key(const K& k) const { return index_.contains(k); }
    std::optional<V> remove(const K& k) {
        const auto it = index_.find(k);
        if (it == index_.end()) return std::nullopt;
        std::optional<V> old = std::move(it->second->second);
        order_.erase(it->second);
        index_.erase(it);
        return old;
    }
    std::size_t size() const { return order_.size(); }
    bool empty() const { return order_.empty(); }
    void clear() { order_.clear(); index_.clear(); }
    auto begin() { return order_.begin(); }
    auto end() { return order_.end(); }
    auto begin() const { return order_.begin(); }
    auto end() const { return order_.end(); }
    std::vector<K> keys() const {
        std::vector<K> out;
        for (const auto& [k, v] : order_) out.push_back(k);
        return out;
    }
    std::vector<V> values() const {
        std::vector<V> out;
        for (const auto& [k, v] : order_) out.push_back(v);
        return out;
    }

private:
    std::list<std::pair<K, V>> order_;
    std::unordered_map<K, typename std::list<std::pair<K, V>>::iterator, Hash, Eq> index_;
};

// Collections.sort / List.sort: stable, so it is std::stable_sort with the comparator's "less" (cmp < 0). (Java's TimSort and a stable sort agree for a
// consistent comparator; the original's inconsistent ones are ported where they are used.)
template <class T, class Cmp>
void java_sort(std::vector<T>& v, Cmp cmp) {
    std::stable_sort(v.begin(), v.end(), [&](const T& a, const T& b) { return cmp(a, b) < 0; });
}

}  // namespace cb::java::decompiler
