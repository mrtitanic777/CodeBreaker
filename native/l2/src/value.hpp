// value.hpp -- a value with Python's semantics: what an L2 document and the C back end's facts are
// made of (json.loads' dicts, lists, strings, numbers; and the tuples, sets and dicts of any key the
// facts hold). Containers are shared, so a dict is one object wherever it is held -- Python's `is`
// and id() are the pointer (py::id). Dicts keep their keys in insertion order, as Python's do.
#pragma once
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace py {

enum class K : uint8_t { None, Bool, Int, Float, Str, List, Tuple, Dict, Set };

struct Heap {
    virtual ~Heap() {}
};

struct V;
struct StrH;
struct SeqH;
struct DictH;

// The exceptions Python would raise where the C back end's Python raised them (KeyError of d['k'],
// ValueError of int('x', 0) ...): the C interface hands back their names, the shim raises the same.
struct PyError : std::runtime_error {
    std::string type;
    PyError(const std::string& t, const std::string& m) : std::runtime_error(m), type(t) {}
};
[[noreturn]] void raise(const char* type, const std::string& msg);

struct V {
    K k = K::None;
    bool u64 = false;          // an int of 2**63 .. 2**64-1, held as its uint64 bits in `i`
    union {
        bool b;
        int64_t i;
        double f;
    };
    std::shared_ptr<Heap> h;   // the string, the sequence or the dict

    V() : i(0) {}
    V(std::nullptr_t) : i(0) {}
    V(bool v) : k(K::Bool), i(0) { b = v; }
    V(int v) : k(K::Int), i(v) {}
    V(long v) : k(K::Int), i(v) {}
    V(long long v) : k(K::Int), i(v) {}
    V(unsigned v) : k(K::Int), i(v) {}
    V(unsigned long long v) : k(K::Int), u64(v > (unsigned long long)INT64_MAX), i((int64_t)v) {}
    V(double v) : k(K::Float), f(v) {}
    V(const char* s);
    V(const std::string& s);
    V(std::string&& s);

    static V list(std::vector<V> items = {});
    static V tuple(std::vector<V> items = {});
    static V set(std::vector<V> items = {});
    static V dict();

    bool is_none() const { return k == K::None; }
    bool is_bool() const { return k == K::Bool; }
    bool is_int() const { return k == K::Int; }             // (not bool: isinstance(v, int) is is_intlike)
    bool is_intlike() const { return k == K::Int || k == K::Bool; }
    bool is_float() const { return k == K::Float; }
    bool is_str() const { return k == K::Str; }
    bool is_list() const { return k == K::List; }
    bool is_tuple() const { return k == K::Tuple; }
    bool is_dict() const { return k == K::Dict; }
    bool is_set() const { return k == K::Set; }
    bool is_seq() const { return k == K::List || k == K::Tuple; }

    const std::string& str() const;
    std::string& mstr();
    std::vector<V>& seq() const;                            // a list's, tuple's or set's items
    DictH& dict_() const;
    int64_t as_int() const;                                 // an int or a bool as a number
    double as_float() const;
    size_t size() const;                                    // len()
    const void* id() const { return h.get(); }              // id() of a container or string

    // dict access by key
    const V& get(const V& key) const;                       // d.get(key) (None when absent or not a dict)
    const V& get(const char* key) const;
    const V& getn(const char* key, size_t len) const;
    template <size_t N>
    const V& get(const char (&key)[N]) const {
        return getn(key, N - 1);
    }
    const V& at(const char* key) const;                     // d[key] (KeyError when absent)
    const V& at(const V& key) const;
    bool has(const char* key) const;                        // key in d
    bool has(const V& key) const;
    void set(const V& key, V value) const;                  // d[key] = value
    void set(const char* key, V value) const;
    void put(V key, V value) const;                         // d[key] = value, the key and value moved in
    V& setdefault(const V& key, V dflt) const;
    bool erase(const V& key) const;                         // d.pop(key, None) is not None-ness aside
    std::vector<std::pair<V, V>>& items() const;

    // sequence access
    const V& operator[](size_t n) const { return seq()[n]; }
    void append(V v) const { seq().push_back(std::move(v)); }
};

struct StrH : Heap {
    std::string s;
    explicit StrH(std::string v) : s(std::move(v)) {}
};
// (a large dict or set is looked up through a hash index, made when first needed and made again
// when its size has changed: keys are never changed in place)
// (an open-addressing table of item numbers; hs holds each item's hash, so growing never hashes keys again)
struct Index {
    std::vector<int32_t> slots;   // -1 empty, else the item's number
    std::vector<size_t> hs;       // the hash of each item
    size_t made_for = (size_t)-1;
};
struct SeqH : Heap {
    std::vector<V> v;
    mutable Index idx;
    int find(const V& item) const;               // (a set's)
};
struct DictH : Heap {
    std::vector<std::pair<V, V>> items;
    mutable Index idx;
    int find(const V& key) const;
    int find(const char* key) const;
    int findn(const char* key, size_t len) const;
};

extern const V NONE;

bool truthy(const V& v);
bool eq(const V& a, const V& b);                            // Python ==
inline bool operator==(const V& a, const V& b) { return eq(a, b); }
inline bool operator!=(const V& a, const V& b) { return !eq(a, b); }
bool operator==(const V& a, const char* s);
inline bool operator!=(const V& a, const char* s) { return !(a == s); }
bool lt(const V& a, const V& b);                            // Python < (sorted's order)
size_t hash(const V& v);
bool contains(const V& container, const V& item);           // item in container (seq, set, dict, str)
std::string repr(const V& v);                               // Python repr()
std::string str_of(const V& v);                             // Python str()
V sorted(const V& iterable);                                // a sorted list
void sort_values(std::vector<V>& v);
inline const V& or_(const V& a, const V& b) { return truthy(a) ? a : b; }
V copy_dict(const V& d);                                    // dict(d): a new dict, the same values
V set_add(const V& s, const V& item);                       // s.add(item) (returns s)
bool set_discard(const V& s, const V& item);

}  // namespace py
