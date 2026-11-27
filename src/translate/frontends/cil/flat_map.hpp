// translate/frontends/cil/flat_map.hpp -- the lane's lookup tables: open addressing keyed by a pointer or an integer, the slots in one
// array. The lowering and the lifting look an object up for every instruction (its type, member, variable, span); a node-based
// std::unordered_map costs an allocation per entry and a byte-wise hash per look-up, a cost paid for every node made.
//
//     FlatMap<const IType*, ir::TypeP> types;
//     if (ir::TypeP* t = types.find(key)) ...;
//     auto [value, fresh] = types.get(key);   // made (default) when absent; the reference lasts until the next insertion
#pragma once

#include <algorithm>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <utility>
#include <vector>

namespace cb::translate::frontends::cil {

// K is a pointer or an unsigned integer; its zero (null) is the empty slot, so it is never a key.
template <class K, class V> class FlatMap {
    static_assert(std::is_pointer_v<K> || std::is_unsigned_v<K>);

public:
    V* find(K key) { return const_cast<V*>(std::as_const(*this).find(key)); }
    const V* find(K key) const {
        if (slots_.empty()) return nullptr;
        const std::size_t mask = slots_.size() - 1;
        for (std::size_t i = hash(key) & mask;; i = (i + 1) & mask) {
            const Slot& s = slots_[i];
            if (s.key == key) return &s.value;
            if (s.key == K{}) return nullptr;
        }
    }

    struct Got {
        V& value;
        bool fresh;
    };
    Got get(K key) {
        if ((size_ + 1) * 2 > slots_.size()) grow();
        const std::size_t mask = slots_.size() - 1;
        for (std::size_t i = hash(key) & mask;; i = (i + 1) & mask) {
            Slot& s = slots_[i];
            if (s.key == key) return {s.value, false};
            if (s.key == K{}) {
                s.key = key;
                ++size_;
                return {s.value, true};
            }
        }
    }

    std::size_t size() const { return size_; }
    // Empty again, in time proportional to what it held: the slots are wiped, or made fewer where they are many more than that needed
    // (a table that held one big function's objects does not cost a wipe of all of them for every small function after it).
    void clear() {
        if (size_ == 0) return;
        const std::size_t needed = std::bit_ceil(std::max<std::size_t>(kFirst, size_ * 4));
        if (slots_.size() > needed) slots_.assign(needed, Slot{});
        else std::fill(slots_.begin(), slots_.end(), Slot{});
        size_ = 0;
    }

private:
    static constexpr std::size_t kFirst = 16;
    struct Slot {
        K key{};
        V value{};
    };
    static std::size_t hash(K key) {
        std::uint64_t x;
        if constexpr (std::is_pointer_v<K>) x = reinterpret_cast<std::uintptr_t>(key);
        else x = static_cast<std::uint64_t>(key);
        x ^= x >> 33;
        x *= 0xff51afd7ed558ccdULL;
        x ^= x >> 33;
        return static_cast<std::size_t>(x);
    }
    void grow() {
        std::vector<Slot> old = std::move(slots_);
        slots_.assign(old.empty() ? kFirst : old.size() * 2, Slot{});
        const std::size_t mask = slots_.size() - 1;
        for (Slot& s : old) {
            if (s.key == K{}) continue;
            std::size_t i = hash(s.key) & mask;
            while (slots_[i].key != K{}) i = (i + 1) & mask;
            slots_[i] = std::move(s);
        }
    }
    std::vector<Slot> slots_;
    std::size_t size_ = 0;
};

}  // namespace cb::translate::frontends::cil
