// java/decompiler/fast_sets.hpp -- the decompiler's bit sets: a set of the elements of a universe given at the start (or growing as new ones are named), kept as
// blocks of 32 bits; union, intersection, difference and subset tests run on whole blocks. FastFixedSet has a universe that never grows, FastSet one that
// does, FastSparseSet adds a chain of the non-empty blocks so a set with few elements is walked in few steps. SFormsFastMapDirect maps a variable (an index,
// a stack slot, a field) to the sparse set of the versions of it that are live, the map the SSA construction carries from node to node.
//
//     FastSparseSetFactory<int> factory(first_64_ints);
//     FastSparseSet<int>* versions = factory.spawn_empty_set();      // an object of the run's arena, as the original's `new`
//     versions->add(3);
//     for (auto it = versions->iterator(); it.has_next();) use(it.next());
//
// Ported from Fernflower (de.fernflower.util: FastSetFactory, FastFixedSetFactory, FastSparseSetFactory, SFormsFastMapDirect), Copyright 2000-2015
// JetBrains s.r.o., Apache License 2.0 (see NOTICE in this folder).
//
// Notes on the port:
//  * Sets, factories and the map are objects of the run's arena (ArenaObject): a pointer is the original's reference, sets are shared and compared by
//    identity as the original does (SFormsFastMapDirect's copy constructor copies the pointers, its get_copy() copies the sets too). A set keeps a pointer
//    to its factory, which must outlive it (make<Factory>(...), or a local one whose sets are not used after it).
//  * The Java nested classes FastSetFactory.FastSet / FastSetIterator are namespace-level here (FastSet<E>, FastSetIterator<E>), by the naming rule.
//  * `union` is a C++ keyword: the method is union_ (the original's union).
//  * The class templates' member functions are defined at the end of this header; only SFormsFastMapDirect needs
//    a .cpp (port/D/fast_sets.cpp).
//  * The original's iterator's next() returns null past the end: here E{}. toPlainSet()'s java.util.HashSet is a JavaHashSet<E, Hash> (the hash is the
//    caller's: JavaIntHash for the Integer sets); getLoad's int[2] is a std::array.
//  * A Java `Set<E>` / `Collection<E>` parameter (addAll, removeAll, the factories' constructors) is a std::vector<E> in the set's iteration order.
//  * FastSparseSet.EMPTY_ARRAY (an empty array of sets) is not needed: the map's arrays are vectors.
#pragma once

#include <array>
#include <bit>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include "java/decompiler/context.hpp"
#include "java/decompiler/exprents.hpp"
#include "java/decompiler/java_util.hpp"
#include "java/decompiler/keyed_list.hpp"

namespace cb::java::decompiler {

// HashSet<Integer> / HashMap<Integer, V> iterated in Java's own order.
using IntHashSet = JavaHashSet<int, JavaIntHash>;
template <class V>
using IntHashMap = JavaHashMap<int, V, JavaIntHash>;

// An element's place in a set: [block, mask] (the original's int[2]).
using FastSetPointer = std::array<std::int32_t, 2>;
// A set's blocks: arrays of the arena's current region (context.hpp), as short-lived as the sets.
using IntArray = ArenaVector<std::int32_t>;

// ---- FastSetFactory / FastSet: the universe grows as elements are met

template <class E> class FastSet;
template <class E> class FastSetIterator;

template <class E>
class FastSetFactory : public ArenaObject {
public:
    explicit FastSetFactory(const std::vector<E>& set);
    FastSet<E>* spawn_empty_set();
    int get_last_block() const { return last_block_; }
    int get_last_mask() const { return last_mask_; }

private:
    friend class FastSet<E>;
    friend class FastSetIterator<E>;
    FastSetPointer add_element(const E& element);   // names a new element
    KeyedList<FastSetPointer, E>& get_internal_values_collection() { return col_values_internal_; }

    KeyedList<FastSetPointer, E> col_values_internal_;   // element -> [block, mask], in the order they were named
    int last_block_ = -1;
    int last_mask_ = -1;
};

template <class E>
class FastSet : public ArenaObject {
public:
    static constexpr bool holds_only_arena_memory = true;   // (pointers and ArenaVector arrays only: never destroyed one by one)
    FastSet<E>* get_copy() const;
    // add / remove / contains of an element the factory does not know yet name it (as the original does).
    void add(const E& element);
    void set_all_elements();
    void add_all(const std::vector<E>& set);
    void remove(const E& element);
    void remove_all(const std::vector<E>& set);
    bool contains(const E& element);
    bool contains(const FastSet<E>* set) const;
    void union_(const FastSet<E>* set);
    void intersection(const FastSet<E>* set);
    void symdiff(const FastSet<E>* set);
    void complement(const FastSet<E>* set);
    bool equals(const FastSet<E>* o) const;
    int get_cardinality() const;   // 0, 1, or 2 for two or more
    int size() const;
    bool is_empty() const;
    FastSetIterator<E> iterator();
    template <class Hash = JavaIntHash>
    JavaHashSet<E, Hash> to_plain_set() const;
    std::string to_binary() const;
    std::array<int, 2> get_load() const;   // {blocks, blocks not empty}
    FastSetFactory<E>* get_factory() const { return factory_; }

private:
    friend class FastSetFactory<E>;
    friend class FastSetIterator<E>;
public:  // (made by spawn_empty_set / get_copy: they put it in the arena)
    explicit FastSet(FastSetFactory<E>* factory);
private:
private:
    IntArray& ensure_capacity(int index);
    IntArray& get_data() { return data_; }
    const IntArray& get_data() const { return data_; }
    void set_data(IntArray data) { data_ = std::move(data); }

    FastSetFactory<E>* factory_;
    KeyedList<FastSetPointer, E>* col_values_internal_;
    IntArray data_;
};

template <class E>
class FastSetIterator {
public:
    bool has_next();
    E next();
    void remove();   // removes the element next() returned from the set

private:
    friend class FastSet<E>;
    explicit FastSetIterator(FastSet<E>* set);
    KeyedList<FastSetPointer, E>* col_values_internal_;
    FastSet<E>* set_;
    int size_;
    int pointer_ = -1;
    int next_pointer_ = -1;
};

// ---- FastFixedSetFactory / FastFixedSet: the universe is given at the start and fixed (every operation takes two sets of the same factory)

template <class E> class FastFixedSet;
template <class E> class FastFixedSetIterator;

template <class E>
class FastFixedSetFactory : public ArenaObject {
public:
    explicit FastFixedSetFactory(const std::vector<E>& set);
    FastFixedSet<E>* spawn_empty_set();
    FastFixedSet<E>* create_empty_set() { return spawn_empty_set(); }

private:
    friend class FastFixedSet<E>;
    friend class FastFixedSetIterator<E>;
    int get_data_length() const { return data_length_; }
    KeyedList<FastSetPointer, E>& get_internal_values_collection() { return col_values_internal_; }

    KeyedList<FastSetPointer, E> col_values_internal_;
    int data_length_ = 1;
};

template <class E>
class FastFixedSet : public ArenaObject {
public:
    static constexpr bool holds_only_arena_memory = true;   // (pointers and ArenaVector arrays only: never destroyed one by one)
    FastFixedSet<E>* get_copy() const;
    void set_all_elements();
    void add(const E& element);
    void add_all(const std::vector<E>& set);
    void remove(const E& element);
    void remove_all(const std::vector<E>& set);
    bool contains(const E& element) const;
    bool contains(const FastFixedSet<E>* set) const;
    void union_(const FastFixedSet<E>* set);
    void intersection(const FastFixedSet<E>* set);
    void symdiff(const FastFixedSet<E>* set);
    void complement(const FastFixedSet<E>* set);
    bool equals(const FastFixedSet<E>* o) const;
    bool is_empty() const;
    FastFixedSetIterator<E> iterator();
    template <class Hash = JavaIntHash>
    JavaHashSet<E, Hash> to_plain_set() const;   // (the original's toPlainCollection(new HashSet))
    std::vector<E> to_plain_list() const;        // the elements in the order they were named
    std::string to_binary() const;
    std::string to_string() const;   // "{a,b}", the last named first
    FastFixedSetFactory<E>* get_factory() const { return factory_; }

private:
    friend class FastFixedSetFactory<E>;
    friend class FastFixedSetIterator<E>;
public:  // (made by spawn_empty_set / get_copy: they put it in the arena)
    explicit FastFixedSet(FastFixedSetFactory<E>* factory);
private:
private:
    IntArray& get_data() { return data_; }
    const IntArray& get_data() const { return data_; }
    void set_data(IntArray data) { data_ = std::move(data); }

    FastFixedSetFactory<E>* factory_;
    KeyedList<FastSetPointer, E>* col_values_internal_;
    IntArray data_;
};

template <class E>
class FastFixedSetIterator {
public:
    bool has_next();
    E next();
    void remove();

private:
    friend class FastFixedSet<E>;
    explicit FastFixedSetIterator(FastFixedSet<E>* set);
    int get_next_index(int index) const;
    KeyedList<FastSetPointer, E>* col_values_internal_;
    IntArray* data_;
    int size_;
    int pointer_ = -1;
    int next_pointer_ = -1;
};

// ---- FastSparseSetFactory / FastSparseSet: a growing universe, and a chain of the non-empty blocks (next[i]: the block after i that is not empty, 0 for none)

template <class E> class FastSparseSet;
template <class E> class FastSparseSetIterator;

template <class E>
class FastSparseSetFactory : public ArenaObject {
public:
    explicit FastSparseSetFactory(const std::vector<E>& set);
    FastSparseSet<E>* spawn_empty_set();
    int get_last_block() const { return last_block_; }
    int get_last_mask() const { return last_mask_; }

private:
    friend class FastSparseSet<E>;
    friend class FastSparseSetIterator<E>;
    FastSetPointer add_element(const E& element);
    KeyedList<FastSetPointer, E>& get_internal_values_collection() { return col_values_internal_; }

    KeyedList<FastSetPointer, E> col_values_internal_;
    int last_block_ = -1;
    int last_mask_ = -1;
};

template <class E>
class FastSparseSet : public ArenaObject {
public:
    static constexpr bool holds_only_arena_memory = true;   // (pointers and ArenaVector arrays only: never destroyed one by one)
    FastSparseSet<E>* get_copy() const;   // a new set of the arena, the data copied
    void add(const E& element);
    void set_all_elements();
    void add_all(const std::vector<E>& set);
    void remove(const E& element);
    void remove_all(const std::vector<E>& set);
    bool contains(const E& element);
    bool contains(const FastSparseSet<E>* set) const;
    void union_(const FastSparseSet<E>* set);
    void intersection(const FastSparseSet<E>* set);
    void symdiff(const FastSparseSet<E>* set);
    void complement(const FastSparseSet<E>* set);
    bool equals(const FastSparseSet<E>* o) const;
    int get_cardinality() const;   // 0, 1, or 2 for two or more
    bool is_empty() const;
    FastSparseSetIterator<E> iterator();
    template <class Hash = JavaIntHash>
    JavaHashSet<E, Hash> to_plain_set() const;
    std::string to_string() const;   // the plain set's toString (the elements in their hash order)
    std::string to_binary() const;
    std::array<int, 2> get_load() const;
    FastSparseSetFactory<E>* get_factory() const { return factory_; }

private:
    friend class FastSparseSetFactory<E>;
    friend class FastSparseSetIterator<E>;
public:  // (made by spawn_empty_set / get_copy: they put it in the arena)
    explicit FastSparseSet(FastSparseSetFactory<E>* factory);
public:  // (made by spawn_empty_set / get_copy: they put it in the arena)
    FastSparseSet(FastSparseSetFactory<E>* factory, IntArray data, IntArray next);
private:
private:
    IntArray& ensure_capacity(int index);   // data and next grow together
    void set_next();                                         // rebuilds the chain
    static void change_next(IntArray& arrnext, int key, int oldnext, int newnext);
    IntArray& get_data() { return data_; }
    const IntArray& get_data() const { return data_; }
    IntArray& get_next() { return next_; }
    const IntArray& get_next() const { return next_; }

    FastSparseSetFactory<E>* factory_;
    KeyedList<FastSetPointer, E>* col_values_internal_;
    IntArray data_;
    IntArray next_;
};

template <class E>
class FastSparseSetIterator {
public:
    bool has_next();
    E next();
    void remove();

private:
    friend class FastSparseSet<E>;
    explicit FastSparseSetIterator(FastSparseSet<E>* set);
    int get_next_index(int index) const;
    KeyedList<FastSetPointer, E>* col_values_internal_;
    IntArray* data_;
    IntArray* next_;
    int size_;
    int pointer_ = -1;
    int next_pointer_ = -1;
};

// ---- SFormsFastMapDirect: variable -> the set of its live versions

// A map from a variable key to a sparse set of the Integers (versions). The key is a local variable index (>= 0), a stack slot (>= VarExprent::STACK_BASE,
// stored shifted down) or a field variable (negative, stored negated): three arrays, each with its own chain of the filled places.
class SFormsFastMapDirect : public ArenaObject {
public:
    static constexpr bool holds_only_arena_memory = true;   // (pointers and ArenaVector arrays only: never destroyed one by one)
    using SparseSet = FastSparseSet<int>;
    using Entry = std::pair<int, SparseSet*>;   // the original's Map.Entry<Integer, FastSparseSet<Integer>>

    SFormsFastMapDirect() = default;
    // The original's copy constructor: the same sets (shared), the arrays copied.
    SFormsFastMapDirect(const SFormsFastMapDirect& map) = default;
    SFormsFastMapDirect& operator=(const SFormsFastMapDirect&) = delete;
    // The original's getCopy: the sets are copied too.
    SFormsFastMapDirect* get_copy() const;

    int size() const { return size_; }
    bool is_empty() const { return size_ == 0; }
    void put(int key, SparseSet* value) { put_internal(key, value, false); }
    void remove(int key) { put_internal(key, nullptr, true); }
    void remove_all_fields();   // the negative keys
    void put_internal(int key, SparseSet* value, bool remove);
    bool contains_key(int key) const { return get(key) != nullptr; }
    SparseSet* get(int key) const;   // null for a key that is not in the map
    // Each of these takes another map (null throws std::runtime_error, as the original's NullPointerException).
    void complement(const SFormsFastMapDirect* map);
    void intersection(const SFormsFastMapDirect* map);
    void union_(const SFormsFastMapDirect* map);
    std::string to_string() const;
    // A new list of the entries: the arrays from the last (fields) to the first, each in key order.
    std::vector<Entry> entry_list() const;

private:
    static void change_next(ArenaVector<int>& arrnext, int key, int oldnext, int newnext);
    ArenaVector<SparseSet*>& ensure_capacity(int index, int size, bool exact);

    int size_ = 0;
    ArenaVector<SparseSet*> elements_[3];
    ArenaVector<int> next_[3];
};


// ---- the class templates' member functions

namespace fast_sets_detail {

inline std::int32_t shl1(std::int32_t v) { return static_cast<std::int32_t>(static_cast<std::uint32_t>(v) << 1); }
// lastmask | (lastmask - 1) with the wrap of Java's int
inline std::int32_t all_below_and_self(std::int32_t m) {
    return static_cast<std::int32_t>(static_cast<std::uint32_t>(m) | (static_cast<std::uint32_t>(m) - 1u));
}
inline std::string binary_string(std::int32_t v) {   // Integer.toBinaryString
    std::uint32_t u = static_cast<std::uint32_t>(v);
    if (u == 0) return "0";
    std::string s;
    while (u != 0) {
        s.insert(s.begin(), static_cast<char>('0' + (u & 1u)));
        u >>= 1;
    }
    return s;
}
template <class E>
std::string element_string(const E& e) {
    if constexpr (std::is_arithmetic_v<E>) return std::to_string(e);
    else if constexpr (requires { e->to_string(); }) return e->to_string();
    else return "?";
}
// A set made with a private constructor, owned by the run's arena.
template <class T, class... A>
T* adopt(A&&... args) {
    return DecompilerContext::current().arena->make<T>(std::forward<A>(args)...);
}
// The element's [block, mask]; std::runtime_error for one the (fixed) universe does not have (the original's NullPointerException).
template <class E>
const FastSetPointer& index_of(const KeyedList<FastSetPointer, E>& col, const E& element) {
    const FastSetPointer* p = col.find(element);
    if (!p) throw std::runtime_error("element not in the fixed set's universe");
    return *p;
}
inline int cardinality_of(const IntArray& intdata) {
    bool found = false;
    for (int i = static_cast<int>(intdata.size()) - 1; i >= 0; i--) {
        const std::int32_t block = intdata[static_cast<std::size_t>(i)];
        if (block != 0) {
            if (found) return 2;
            if ((block & (block - 1)) == 0) found = true;
            else return 2;
        }
    }
    return found ? 1 : 0;
}
inline bool data_equal(const IntArray& a, const IntArray& b) {
    const IntArray* longdata = &b;
    const IntArray* shortdata = &a;
    if (a.size() > b.size()) {
        shortdata = &b;
        longdata = &a;
    }
    for (int i = static_cast<int>(shortdata->size()) - 1; i >= 0; i--)
        if ((*shortdata)[static_cast<std::size_t>(i)] != (*longdata)[static_cast<std::size_t>(i)]) return false;
    for (int i = static_cast<int>(longdata->size()) - 1; i >= static_cast<int>(shortdata->size()); i--)
        if ((*longdata)[static_cast<std::size_t>(i)] != 0) return false;
    return true;
}
inline bool data_contains(const IntArray& intdata, const IntArray& extdata) {
    const int minlength = static_cast<int>(std::min(extdata.size(), intdata.size()));
    for (int i = minlength - 1; i >= 0; i--)
        if ((extdata[static_cast<std::size_t>(i)] & ~intdata[static_cast<std::size_t>(i)]) != 0) return false;
    for (int i = static_cast<int>(extdata.size()) - 1; i >= minlength; i--)
        if (extdata[static_cast<std::size_t>(i)] != 0) return false;
    return true;
}
inline std::string binary_of(const IntArray& d) {
    std::string s;
    for (std::int32_t v : d) s += " " + binary_string(v);
    return s;
}
inline std::array<int, 2> load_of(const IntArray& d) {
    int notempty = 0;
    for (std::int32_t v : d)
        if (v != 0) notempty++;
    return {static_cast<int>(d.size()), notempty};
}
// the elements named in a growing universe that data marks, last named first (toPlainSet)
template <class E, class Set>
void collect_marked(const KeyedList<FastSetPointer, E>& col, const IntArray& data, Set& set) {
    int size = static_cast<int>(data.size()) * 32;
    if (size > static_cast<int>(col.size())) size = static_cast<int>(col.size());
    for (int i = size - 1; i >= 0; i--) {
        const FastSetPointer& index = col[static_cast<std::size_t>(i)];
        if ((data[static_cast<std::size_t>(index[0])] & index[1]) != 0) set.add(*col.key(static_cast<std::size_t>(i)));
    }
}

}  // namespace fast_sets_detail

// FastSetFactory

template <class E>
FastSetFactory<E>::FastSetFactory(const std::vector<E>& set) {
    int block = -1;
    int mask = -1;
    int index = 0;
    for (const E& element : set) {
        block = index / 32;
        if (index % 32 == 0) mask = 1;
        else mask = fast_sets_detail::shl1(mask);
        col_values_internal_.put_with_key(FastSetPointer{block, mask}, element);
        index++;
    }
    last_block_ = block;
    last_mask_ = mask;
}

template <class E>
FastSetPointer FastSetFactory<E>::add_element(const E& element) {
    if (last_mask_ == -1 || last_mask_ == INT32_MIN) {
        last_mask_ = 1;
        last_block_++;
    } else {
        last_mask_ = fast_sets_detail::shl1(last_mask_);
    }
    const FastSetPointer pointer{last_block_, last_mask_};
    col_values_internal_.put_with_key(pointer, element);
    return pointer;
}

template <class E>
FastSet<E>* FastSetFactory<E>::spawn_empty_set() {
    return fast_sets_detail::adopt<FastSet<E>>(this);
}

// FastSet

template <class E>
FastSet<E>::FastSet(FastSetFactory<E>* factory)
    : factory_(factory), col_values_internal_(&factory->get_internal_values_collection()), data_(static_cast<std::size_t>(factory->get_last_block() + 1)) {}

template <class E>
FastSet<E>* FastSet<E>::get_copy() const {
    FastSet<E>* copy = fast_sets_detail::adopt<FastSet<E>>(factory_);
    copy->set_data(data_);
    return copy;
}

template <class E>
IntArray& FastSet<E>::ensure_capacity(int index) {
    std::size_t newlength = data_.size();
    if (newlength == 0) newlength = 1;
    while (newlength <= static_cast<std::size_t>(index)) newlength *= 2;
    data_.resize(newlength, 0);
    return data_;
}

template <class E>
void FastSet<E>::add(const E& element) {
    FastSetPointer index;
    if (const FastSetPointer* p = col_values_internal_->find(element)) index = *p;
    else index = factory_->add_element(element);
    if (index[0] >= static_cast<int>(data_.size())) ensure_capacity(index[0]);
    data_[static_cast<std::size_t>(index[0])] |= index[1];
}

template <class E>
void FastSet<E>::set_all_elements() {
    const int lastblock = factory_->get_last_block();
    const int lastmask = factory_->get_last_mask();
    if (lastblock < 0) throw std::out_of_range("FastSet::set_all_elements: empty universe");
    if (lastblock >= static_cast<int>(data_.size())) ensure_capacity(lastblock);
    for (int i = lastblock - 1; i >= 0; i--) data_[static_cast<std::size_t>(i)] = -1;
    data_[static_cast<std::size_t>(lastblock)] = fast_sets_detail::all_below_and_self(lastmask);
}

template <class E>
void FastSet<E>::add_all(const std::vector<E>& set) {
    for (const E& element : set) add(element);
}

template <class E>
void FastSet<E>::remove(const E& element) {
    FastSetPointer index;
    if (const FastSetPointer* p = col_values_internal_->find(element)) index = *p;
    else index = factory_->add_element(element);
    if (index[0] < static_cast<int>(data_.size())) data_[static_cast<std::size_t>(index[0])] &= ~index[1];
}

template <class E>
void FastSet<E>::remove_all(const std::vector<E>& set) {
    for (const E& element : set) remove(element);
}

template <class E>
bool FastSet<E>::contains(const E& element) {
    FastSetPointer index;
    if (const FastSetPointer* p = col_values_internal_->find(element)) index = *p;
    else index = factory_->add_element(element);
    return index[0] >= static_cast<int>(data_.size()) ? false : ((data_[static_cast<std::size_t>(index[0])] & index[1]) != 0);
}

template <class E>
bool FastSet<E>::contains(const FastSet<E>* set) const {
    return fast_sets_detail::data_contains(data_, set->get_data());
}

template <class E>
void FastSet<E>::union_(const FastSet<E>* set) {
    const IntArray extdata = set->get_data();
    const int minlength = static_cast<int>(std::min(extdata.size(), data_.size()));
    for (int i = minlength - 1; i >= 0; i--) data_[static_cast<std::size_t>(i)] |= extdata[static_cast<std::size_t>(i)];
    for (int i = static_cast<int>(extdata.size()) - 1; i >= minlength; i--) {
        if (extdata[static_cast<std::size_t>(i)] != 0) {
            ensure_capacity(static_cast<int>(extdata.size()) - 1);
            data_[static_cast<std::size_t>(i)] = extdata[static_cast<std::size_t>(i)];
        }
    }
}

template <class E>
void FastSet<E>::intersection(const FastSet<E>* set) {
    const IntArray extdata = set->get_data();
    const int minlength = static_cast<int>(std::min(extdata.size(), data_.size()));
    for (int i = minlength - 1; i >= 0; i--) data_[static_cast<std::size_t>(i)] &= extdata[static_cast<std::size_t>(i)];
    for (int i = static_cast<int>(data_.size()) - 1; i >= minlength; i--) data_[static_cast<std::size_t>(i)] = 0;
}

template <class E>
void FastSet<E>::symdiff(const FastSet<E>* set) {
    const IntArray extdata = set->get_data();
    const int minlength = static_cast<int>(std::min(extdata.size(), data_.size()));
    for (int i = minlength - 1; i >= 0; i--) data_[static_cast<std::size_t>(i)] ^= extdata[static_cast<std::size_t>(i)];
    for (int i = static_cast<int>(extdata.size()) - 1; i >= minlength; i--) {
        if (extdata[static_cast<std::size_t>(i)] != 0) {
            ensure_capacity(static_cast<int>(extdata.size()) - 1);
            data_[static_cast<std::size_t>(i)] = extdata[static_cast<std::size_t>(i)];
        }
    }
}

template <class E>
void FastSet<E>::complement(const FastSet<E>* set) {
    const IntArray extdata = set->get_data();
    const int minlength = static_cast<int>(std::min(extdata.size(), data_.size()));
    for (int i = minlength - 1; i >= 0; i--) data_[static_cast<std::size_t>(i)] &= ~extdata[static_cast<std::size_t>(i)];
}

template <class E>
bool FastSet<E>::equals(const FastSet<E>* o) const {
    if (o == this) return true;
    if (o == nullptr) return false;
    return fast_sets_detail::data_equal(data_, o->get_data());
}

template <class E>
int FastSet<E>::get_cardinality() const {
    return fast_sets_detail::cardinality_of(data_);
}

template <class E>
int FastSet<E>::size() const {
    int size = 0;
    for (std::int32_t v : data_) size += std::popcount(static_cast<std::uint32_t>(v));
    return size;
}

template <class E>
bool FastSet<E>::is_empty() const {
    for (int i = static_cast<int>(data_.size()) - 1; i >= 0; i--)
        if (data_[static_cast<std::size_t>(i)] != 0) return false;
    return true;
}

template <class E>
FastSetIterator<E> FastSet<E>::iterator() {
    return FastSetIterator<E>(this);
}

template <class E>
template <class Hash>
JavaHashSet<E, Hash> FastSet<E>::to_plain_set() const {
    JavaHashSet<E, Hash> set;
    fast_sets_detail::collect_marked<E>(*col_values_internal_, data_, set);
    return set;
}

template <class E>
std::string FastSet<E>::to_binary() const {
    return fast_sets_detail::binary_of(data_);
}

template <class E>
std::array<int, 2> FastSet<E>::get_load() const {
    return fast_sets_detail::load_of(data_);
}

// FastSetIterator

template <class E>
FastSetIterator<E>::FastSetIterator(FastSet<E>* set) : col_values_internal_(set->col_values_internal_), set_(set) {
    size_ = static_cast<int>(col_values_internal_->size());
    const int datasize = static_cast<int>(set->data_.size()) * 32;
    if (datasize < size_) size_ = datasize;
}

template <class E>
bool FastSetIterator<E>::has_next() {
    next_pointer_ = pointer_;
    while (++next_pointer_ < size_) {
        const FastSetPointer& index = (*col_values_internal_)[static_cast<std::size_t>(next_pointer_)];
        if ((set_->data_[static_cast<std::size_t>(index[0])] & index[1]) != 0) return true;
    }
    next_pointer_ = -1;
    return false;
}

template <class E>
E FastSetIterator<E>::next() {
    if (next_pointer_ >= 0) {
        pointer_ = next_pointer_;
    } else {
        while (++pointer_ < size_) {
            const FastSetPointer& index = (*col_values_internal_)[static_cast<std::size_t>(pointer_)];
            if ((set_->data_[static_cast<std::size_t>(index[0])] & index[1]) != 0) break;
        }
    }
    next_pointer_ = -1;
    return pointer_ < size_ ? *col_values_internal_->key(static_cast<std::size_t>(pointer_)) : E{};
}

template <class E>
void FastSetIterator<E>::remove() {
    const FastSetPointer& index = (*col_values_internal_)[static_cast<std::size_t>(pointer_)];
    set_->data_[static_cast<std::size_t>(index[0])] &= ~index[1];
    pointer_--;
}

// FastFixedSetFactory

template <class E>
FastFixedSetFactory<E>::FastFixedSetFactory(const std::vector<E>& set) {
    data_length_ = static_cast<int>(set.size()) / 32 + 1;
    int index = 0;
    int mask = 1;
    for (const E& element : set) {
        const int block = index / 32;
        if (index % 32 == 0) mask = 1;
        col_values_internal_.put_with_key(FastSetPointer{block, mask}, element);
        index++;
        mask = fast_sets_detail::shl1(mask);
    }
}

template <class E>
FastFixedSet<E>* FastFixedSetFactory<E>::spawn_empty_set() {
    return fast_sets_detail::adopt<FastFixedSet<E>>(this);
}

// FastFixedSet

template <class E>
FastFixedSet<E>::FastFixedSet(FastFixedSetFactory<E>* factory)
    : factory_(factory), col_values_internal_(&factory->get_internal_values_collection()), data_(static_cast<std::size_t>(factory->get_data_length())) {}

template <class E>
FastFixedSet<E>* FastFixedSet<E>::get_copy() const {
    FastFixedSet<E>* copy = fast_sets_detail::adopt<FastFixedSet<E>>(factory_);
    copy->set_data(data_);
    return copy;
}

template <class E>
void FastFixedSet<E>::set_all_elements() {
    if (col_values_internal_->size() == 0) throw std::out_of_range("FastFixedSet::set_all_elements: empty universe");
    const FastSetPointer lastindex = (*col_values_internal_)[col_values_internal_->size() - 1];
    for (int i = lastindex[0] - 1; i >= 0; i--) data_[static_cast<std::size_t>(i)] = -1;
    data_.at(static_cast<std::size_t>(lastindex[0])) = fast_sets_detail::all_below_and_self(lastindex[1]);
}

template <class E>
void FastFixedSet<E>::add(const E& element) {
    const FastSetPointer index = fast_sets_detail::index_of(*col_values_internal_, element);
    data_.at(static_cast<std::size_t>(index[0])) |= index[1];
}

template <class E>
void FastFixedSet<E>::add_all(const std::vector<E>& set) {
    for (const E& element : set) add(element);
}

template <class E>
void FastFixedSet<E>::remove(const E& element) {
    const FastSetPointer index = fast_sets_detail::index_of(*col_values_internal_, element);
    data_.at(static_cast<std::size_t>(index[0])) &= ~index[1];
}

template <class E>
void FastFixedSet<E>::remove_all(const std::vector<E>& set) {
    for (const E& element : set) remove(element);
}

template <class E>
bool FastFixedSet<E>::contains(const E& element) const {
    const FastSetPointer index = fast_sets_detail::index_of(*col_values_internal_, element);
    return (data_.at(static_cast<std::size_t>(index[0])) & index[1]) != 0;
}

template <class E>
bool FastFixedSet<E>::contains(const FastFixedSet<E>* set) const {
    const IntArray& extdata = set->get_data();
    for (int i = static_cast<int>(data_.size()) - 1; i >= 0; i--)
        if ((extdata.at(static_cast<std::size_t>(i)) & ~data_[static_cast<std::size_t>(i)]) != 0) return false;
    return true;
}

template <class E>
void FastFixedSet<E>::union_(const FastFixedSet<E>* set) {
    const IntArray& extdata = set->get_data();
    for (int i = static_cast<int>(data_.size()) - 1; i >= 0; i--) data_[static_cast<std::size_t>(i)] |= extdata.at(static_cast<std::size_t>(i));
}

template <class E>
void FastFixedSet<E>::intersection(const FastFixedSet<E>* set) {
    const IntArray& extdata = set->get_data();
    for (int i = static_cast<int>(data_.size()) - 1; i >= 0; i--) data_[static_cast<std::size_t>(i)] &= extdata.at(static_cast<std::size_t>(i));
}

template <class E>
void FastFixedSet<E>::symdiff(const FastFixedSet<E>* set) {
    const IntArray& extdata = set->get_data();
    for (int i = static_cast<int>(data_.size()) - 1; i >= 0; i--) data_[static_cast<std::size_t>(i)] ^= extdata.at(static_cast<std::size_t>(i));
}

template <class E>
void FastFixedSet<E>::complement(const FastFixedSet<E>* set) {
    const IntArray& extdata = set->get_data();
    for (int i = static_cast<int>(data_.size()) - 1; i >= 0; i--) data_[static_cast<std::size_t>(i)] &= ~extdata.at(static_cast<std::size_t>(i));
}

template <class E>
bool FastFixedSet<E>::equals(const FastFixedSet<E>* o) const {
    if (o == this) return true;
    if (o == nullptr) return false;
    const IntArray& extdata = o->get_data();
    for (int i = static_cast<int>(data_.size()) - 1; i >= 0; i--)
        if (data_[static_cast<std::size_t>(i)] != extdata.at(static_cast<std::size_t>(i))) return false;
    return true;
}

template <class E>
bool FastFixedSet<E>::is_empty() const {
    for (int i = static_cast<int>(data_.size()) - 1; i >= 0; i--)
        if (data_[static_cast<std::size_t>(i)] != 0) return false;
    return true;
}

template <class E>
FastFixedSetIterator<E> FastFixedSet<E>::iterator() {
    return FastFixedSetIterator<E>(this);
}

template <class E>
template <class Hash>
JavaHashSet<E, Hash> FastFixedSet<E>::to_plain_set() const {
    JavaHashSet<E, Hash> set;
    for (std::size_t bindex = 0; bindex < data_.size(); bindex++) {
        std::uint32_t block = static_cast<std::uint32_t>(data_[bindex]);
        if (block != 0) {
            std::size_t index = bindex << 5;
            for (int i = 31; i >= 0; i--) {
                if ((block & 1u) != 0) set.add(*col_values_internal_->key(index));
                index++;
                block >>= 1;
            }
        }
    }
    return set;
}

template <class E>
std::vector<E> FastFixedSet<E>::to_plain_list() const {
    std::vector<E> list;
    for (std::size_t bindex = 0; bindex < data_.size(); bindex++) {
        std::uint32_t block = static_cast<std::uint32_t>(data_[bindex]);
        if (block != 0) {
            std::size_t index = bindex << 5;
            for (int i = 31; i >= 0; i--) {
                if ((block & 1u) != 0) list.push_back(*col_values_internal_->key(index));
                index++;
                block >>= 1;
            }
        }
    }
    return list;
}

template <class E>
std::string FastFixedSet<E>::to_binary() const {
    return fast_sets_detail::binary_of(data_);
}

template <class E>
std::string FastFixedSet<E>::to_string() const {
    std::string buffer = "{";
    bool first = true;
    for (int i = static_cast<int>(col_values_internal_->size()) - 1; i >= 0; i--) {
        const FastSetPointer& index = (*col_values_internal_)[static_cast<std::size_t>(i)];
        if ((data_[static_cast<std::size_t>(index[0])] & index[1]) != 0) {
            if (first) first = false;
            else buffer += ",";
            buffer += fast_sets_detail::element_string(*col_values_internal_->key(static_cast<std::size_t>(i)));
        }
    }
    buffer += "}";
    return buffer;
}

// FastFixedSetIterator

template <class E>
FastFixedSetIterator<E>::FastFixedSetIterator(FastFixedSet<E>* set)
    : col_values_internal_(set->col_values_internal_), data_(&set->data_), size_(static_cast<int>(set->col_values_internal_->size())) {}

template <class E>
int FastFixedSetIterator<E>::get_next_index(int index) const {
    index++;
    int ret = index;
    int bindex = index / 32;
    int dindex = index % 32;
    while (bindex < static_cast<int>(data_->size())) {
        std::uint32_t block = static_cast<std::uint32_t>((*data_)[static_cast<std::size_t>(bindex)]);
        if (block != 0) {
            block >>= dindex;
            while (dindex < 32) {
                if ((block & 1u) != 0) return ret;
                block >>= 1;
                dindex++;
                ret++;
            }
        } else {
            ret += (32 - dindex);
        }
        dindex = 0;
        bindex++;
    }
    return -1;
}

template <class E>
bool FastFixedSetIterator<E>::has_next() {
    next_pointer_ = get_next_index(pointer_);
    return next_pointer_ >= 0;
}

template <class E>
E FastFixedSetIterator<E>::next() {
    if (next_pointer_ >= 0) {
        pointer_ = next_pointer_;
    } else {
        pointer_ = get_next_index(pointer_);
        if (pointer_ == -1) pointer_ = size_;
    }
    next_pointer_ = -1;
    return pointer_ < size_ ? *col_values_internal_->key(static_cast<std::size_t>(pointer_)) : E{};
}

template <class E>
void FastFixedSetIterator<E>::remove() {
    const FastSetPointer& index = (*col_values_internal_)[static_cast<std::size_t>(pointer_)];
    (*data_)[static_cast<std::size_t>(index[0])] &= ~index[1];
}

// FastSparseSetFactory

template <class E>
FastSparseSetFactory<E>::FastSparseSetFactory(const std::vector<E>& set) {
    int block = -1;
    int mask = -1;
    int index = 0;
    for (const E& element : set) {
        block = index / 32;
        if (index % 32 == 0) mask = 1;
        else mask = fast_sets_detail::shl1(mask);
        col_values_internal_.put_with_key(FastSetPointer{block, mask}, element);
        index++;
    }
    last_block_ = block;
    last_mask_ = mask;
}

template <class E>
FastSetPointer FastSparseSetFactory<E>::add_element(const E& element) {
    if (last_mask_ == -1 || last_mask_ == INT32_MIN) {
        last_mask_ = 1;
        last_block_++;
    } else {
        last_mask_ = fast_sets_detail::shl1(last_mask_);
    }
    const FastSetPointer pointer{last_block_, last_mask_};
    col_values_internal_.put_with_key(pointer, element);
    return pointer;
}

template <class E>
FastSparseSet<E>* FastSparseSetFactory<E>::spawn_empty_set() {
    return fast_sets_detail::adopt<FastSparseSet<E>>(this);
}

// FastSparseSet

template <class E>
FastSparseSet<E>::FastSparseSet(FastSparseSetFactory<E>* factory)
    : factory_(factory), col_values_internal_(&factory->get_internal_values_collection()) {
    const std::size_t length = static_cast<std::size_t>(factory->get_last_block() + 1);
    data_.assign(length, 0);
    next_.assign(length, 0);
}

template <class E>
FastSparseSet<E>::FastSparseSet(FastSparseSetFactory<E>* factory, IntArray data, IntArray next)
    : factory_(factory), col_values_internal_(&factory->get_internal_values_collection()), data_(std::move(data)), next_(std::move(next)) {}

template <class E>
FastSparseSet<E>* FastSparseSet<E>::get_copy() const {
    return fast_sets_detail::adopt<FastSparseSet<E>>(factory_, data_, next_);
}

template <class E>
IntArray& FastSparseSet<E>::ensure_capacity(int index) {
    std::size_t newlength = data_.size();
    if (newlength == 0) newlength = 1;
    while (newlength <= static_cast<std::size_t>(index)) newlength *= 2;
    data_.resize(newlength, 0);
    next_.resize(newlength, 0);
    return data_;
}

template <class E>
void FastSparseSet<E>::add(const E& element) {
    FastSetPointer index;
    if (const FastSetPointer* p = col_values_internal_->find(element)) index = *p;
    else index = factory_->add_element(element);
    const int block = index[0];
    if (block >= static_cast<int>(data_.size())) ensure_capacity(block);
    data_[static_cast<std::size_t>(block)] |= index[1];
    change_next(next_, block, next_[static_cast<std::size_t>(block)], block);
}

template <class E>
void FastSparseSet<E>::set_all_elements() {
    const int lastblock = factory_->get_last_block();
    const int lastmask = factory_->get_last_mask();
    if (lastblock < 0) throw std::out_of_range("FastSparseSet::set_all_elements: empty universe");
    if (lastblock >= static_cast<int>(data_.size())) ensure_capacity(lastblock);
    for (int i = lastblock - 1; i >= 0; i--) {
        data_[static_cast<std::size_t>(i)] = -1;
        next_[static_cast<std::size_t>(i)] = i + 1;
    }
    data_[static_cast<std::size_t>(lastblock)] = fast_sets_detail::all_below_and_self(lastmask);
    next_[static_cast<std::size_t>(lastblock)] = 0;
}

template <class E>
void FastSparseSet<E>::add_all(const std::vector<E>& set) {
    for (const E& element : set) add(element);
}

template <class E>
void FastSparseSet<E>::remove(const E& element) {
    FastSetPointer index;
    if (const FastSetPointer* p = col_values_internal_->find(element)) index = *p;
    else index = factory_->add_element(element);
    const int block = index[0];
    if (block < static_cast<int>(data_.size())) {
        data_[static_cast<std::size_t>(block)] &= ~index[1];
        if (data_[static_cast<std::size_t>(block)] == 0) change_next(next_, block, block, next_[static_cast<std::size_t>(block)]);
    }
}

template <class E>
void FastSparseSet<E>::remove_all(const std::vector<E>& set) {
    for (const E& element : set) remove(element);
}

template <class E>
bool FastSparseSet<E>::contains(const E& element) {
    FastSetPointer index;
    if (const FastSetPointer* p = col_values_internal_->find(element)) index = *p;
    else index = factory_->add_element(element);
    return index[0] >= static_cast<int>(data_.size()) ? false : ((data_[static_cast<std::size_t>(index[0])] & index[1]) != 0);
}

template <class E>
bool FastSparseSet<E>::contains(const FastSparseSet<E>* set) const {
    return fast_sets_detail::data_contains(data_, set->get_data());
}

template <class E>
void FastSparseSet<E>::set_next() {
    int link = 0;
    for (int i = static_cast<int>(data_.size()) - 1; i >= 0; i--) {
        next_[static_cast<std::size_t>(i)] = link;
        if (data_[static_cast<std::size_t>(i)] != 0) link = i;
    }
}

template <class E>
void FastSparseSet<E>::change_next(IntArray& arrnext, int key, int oldnext, int newnext) {
    for (int i = key - 1; i >= 0; i--) {
        if (arrnext[static_cast<std::size_t>(i)] == oldnext) arrnext[static_cast<std::size_t>(i)] = newnext;
        else break;
    }
}

template <class E>
void FastSparseSet<E>::union_(const FastSparseSet<E>* set) {
    const IntArray extdata = set->get_data();
    const IntArray extnext = set->get_next();
    const int intlength = static_cast<int>(data_.size());
    int pointer = 0;
    do {
        if (pointer >= intlength) ensure_capacity(static_cast<int>(extdata.size()) - 1);
        const bool nextrec = (data_.at(static_cast<std::size_t>(pointer)) == 0);
        data_[static_cast<std::size_t>(pointer)] |= extdata.at(static_cast<std::size_t>(pointer));
        if (nextrec) change_next(next_, pointer, next_[static_cast<std::size_t>(pointer)], pointer);
        pointer = extnext.at(static_cast<std::size_t>(pointer));
    } while (pointer != 0);
}

template <class E>
void FastSparseSet<E>::intersection(const FastSparseSet<E>* set) {
    const IntArray extdata = set->get_data();
    const int minlength = static_cast<int>(std::min(extdata.size(), data_.size()));
    for (int i = minlength - 1; i >= 0; i--) data_[static_cast<std::size_t>(i)] &= extdata[static_cast<std::size_t>(i)];
    for (int i = static_cast<int>(data_.size()) - 1; i >= minlength; i--) data_[static_cast<std::size_t>(i)] = 0;
    set_next();
}

template <class E>
void FastSparseSet<E>::symdiff(const FastSparseSet<E>* set) {
    const IntArray extdata = set->get_data();
    const int minlength = static_cast<int>(std::min(extdata.size(), data_.size()));
    for (int i = minlength - 1; i >= 0; i--) data_[static_cast<std::size_t>(i)] ^= extdata[static_cast<std::size_t>(i)];
    for (int i = static_cast<int>(extdata.size()) - 1; i >= minlength; i--) {
        if (extdata[static_cast<std::size_t>(i)] != 0) {
            ensure_capacity(static_cast<int>(extdata.size()) - 1);
            data_[static_cast<std::size_t>(i)] = extdata[static_cast<std::size_t>(i)];
        }
    }
    set_next();
}

template <class E>
void FastSparseSet<E>::complement(const FastSparseSet<E>* set) {
    const IntArray extdata = set->get_data();
    const int extlength = static_cast<int>(extdata.size());
    int pointer = 0;
    do {
        if (pointer >= extlength) break;
        data_.at(static_cast<std::size_t>(pointer)) &= ~extdata[static_cast<std::size_t>(pointer)];
        if (data_[static_cast<std::size_t>(pointer)] == 0) change_next(next_, pointer, pointer, next_[static_cast<std::size_t>(pointer)]);
        pointer = next_[static_cast<std::size_t>(pointer)];
    } while (pointer != 0);
}

template <class E>
bool FastSparseSet<E>::equals(const FastSparseSet<E>* o) const {
    if (o == this) return true;
    if (o == nullptr) return false;
    return fast_sets_detail::data_equal(data_, o->get_data());
}

template <class E>
int FastSparseSet<E>::get_cardinality() const {
    return fast_sets_detail::cardinality_of(data_);
}

template <class E>
bool FastSparseSet<E>::is_empty() const {
    return data_.empty() || (next_[0] == 0 && data_[0] == 0);
}

template <class E>
FastSparseSetIterator<E> FastSparseSet<E>::iterator() {
    return FastSparseSetIterator<E>(this);
}

template <class E>
template <class Hash>
JavaHashSet<E, Hash> FastSparseSet<E>::to_plain_set() const {
    JavaHashSet<E, Hash> set;
    fast_sets_detail::collect_marked<E>(*col_values_internal_, data_, set);
    return set;
}

template <class E>
std::string FastSparseSet<E>::to_string() const {
    std::string s = "[";
    bool first = true;
    for (const E& e : to_plain_set<JavaIntHash>().items()) {
        if (!first) s += ", ";
        first = false;
        s += fast_sets_detail::element_string(e);
    }
    return s + "]";
}

template <class E>
std::string FastSparseSet<E>::to_binary() const {
    return fast_sets_detail::binary_of(data_);
}

template <class E>
std::array<int, 2> FastSparseSet<E>::get_load() const {
    return fast_sets_detail::load_of(data_);
}

// FastSparseSetIterator

template <class E>
FastSparseSetIterator<E>::FastSparseSetIterator(FastSparseSet<E>* set)
    : col_values_internal_(set->col_values_internal_), data_(&set->data_), next_(&set->next_), size_(static_cast<int>(set->col_values_internal_->size())) {}

template <class E>
int FastSparseSetIterator<E>::get_next_index(int index) const {
    index++;
    int bindex = static_cast<int>(static_cast<std::uint32_t>(index) >> 5);
    int dindex = index & 0x1F;
    while (bindex < static_cast<int>(data_->size())) {
        std::uint32_t block = static_cast<std::uint32_t>((*data_)[static_cast<std::size_t>(bindex)]);
        if (block != 0) {
            block >>= dindex;
            while (dindex < 32) {
                if ((block & 1u) != 0) return (bindex << 5) + dindex;
                block >>= 1;
                dindex++;
            }
        }
        dindex = 0;
        bindex = (*next_)[static_cast<std::size_t>(bindex)];
        if (bindex == 0) break;
    }
    return -1;
}

template <class E>
bool FastSparseSetIterator<E>::has_next() {
    next_pointer_ = get_next_index(pointer_);
    return next_pointer_ >= 0;
}

template <class E>
E FastSparseSetIterator<E>::next() {
    if (next_pointer_ >= 0) {
        pointer_ = next_pointer_;
    } else {
        pointer_ = get_next_index(pointer_);
        if (pointer_ == -1) pointer_ = size_;
    }
    next_pointer_ = -1;
    return pointer_ < size_ ? *col_values_internal_->key(static_cast<std::size_t>(pointer_)) : E{};
}

template <class E>
void FastSparseSetIterator<E>::remove() {
    const FastSetPointer& index = (*col_values_internal_)[static_cast<std::size_t>(pointer_)];
    (*data_)[static_cast<std::size_t>(index[0])] &= ~index[1];
}

}  // namespace cb::java::decompiler
