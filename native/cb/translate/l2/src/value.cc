#include "value.hpp"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <string_view>

#include "pystr.hpp"

namespace py {

const V NONE;

void raise(const char* type, const std::string& msg) { throw PyError(type, msg); }

V::V(const char* s) : k(K::Str), i(0), h(std::make_shared<StrH>(s)) {}
V::V(const std::string& s) : k(K::Str), i(0), h(std::make_shared<StrH>(s)) {}
V::V(std::string&& s) : k(K::Str), i(0), h(std::make_shared<StrH>(std::move(s))) {}

V V::list(std::vector<V> items) {
    V v;
    v.k = K::List;
    auto p = std::make_shared<SeqH>();
    p->v = std::move(items);
    v.h = p;
    return v;
}
V V::tuple(std::vector<V> items) {
    V v = list(std::move(items));
    v.k = K::Tuple;
    return v;
}
V V::set(std::vector<V> items) {
    V v;
    v.k = K::Set;
    v.h = std::make_shared<SeqH>();
    for (auto& x : items) set_add(v, x);
    return v;
}
V V::dict() {
    V v;
    v.k = K::Dict;
    v.h = std::make_shared<DictH>();
    return v;
}

static const std::string EMPTY_STR;

const std::string& V::str() const {
    if (k != K::Str) raise("TypeError", "expected str, got " + repr(*this));
    return static_cast<StrH*>(h.get())->s;
}
std::string& V::mstr() { return static_cast<StrH*>(h.get())->s; }
std::vector<V>& V::seq() const {
    if (k != K::List && k != K::Tuple && k != K::Set) raise("TypeError", "not a sequence: " + repr(*this));
    return static_cast<SeqH*>(h.get())->v;
}
DictH& V::dict_() const {
    if (k != K::Dict) raise("AttributeError", "not a dict: " + repr(*this));
    return *static_cast<DictH*>(h.get());
}
int64_t V::as_int() const {
    if (k == K::Int) return i;
    if (k == K::Bool) return b ? 1 : 0;
    if (k == K::Float) return (int64_t)f;
    raise("TypeError", "not an int: " + repr(*this));
}
double V::as_float() const {
    if (k == K::Float) return f;
    if (k == K::Int) return u64 ? (double)(uint64_t)i : (double)i;
    if (k == K::Bool) return b ? 1.0 : 0.0;
    raise("TypeError", "not a number");
}
size_t V::size() const {
    switch (k) {
        case K::Str: return str().size();
        case K::List: case K::Tuple: case K::Set: return seq().size();
        case K::Dict: return dict_().items.size();
        default: raise("TypeError", "object has no len()");
    }
}

static const size_t INDEXED = 24;

// ---- the hash index of a large dict or set ------------------------------------------------------------
static inline size_t slot_of(size_t h, size_t mask) {
    uint64_t x = (uint64_t)h * 0x9E3779B97F4A7C15ull;
    return (size_t)(x ^ (x >> 29)) & mask;
}
static void idx_fill(Index& ix) {   // the slot table from the hashes held
    size_t cap = 64;
    while (cap < ix.hs.size() * 2 + 2) cap <<= 1;
    ix.slots.assign(cap, -1);
    for (size_t n = 0; n < ix.hs.size(); n++) {
        size_t i = slot_of(ix.hs[n], cap - 1);
        while (ix.slots[i] >= 0) i = (i + 1) & (cap - 1);
        ix.slots[i] = (int32_t)n;
    }
}
template <class HashAt>
static void idx_make(Index& ix, size_t count, HashAt at) {
    ix.hs.resize(count);
    for (size_t n = 0; n < count; n++) ix.hs[n] = at(n);
    idx_fill(ix);
    ix.made_for = count;
}
template <class Eq>
static int idx_find(const Index& ix, size_t h, Eq same) {
    const size_t mask = ix.slots.size() - 1;
    for (size_t i = slot_of(h, mask); ix.slots[i] >= 0; i = (i + 1) & mask) {
        size_t n = (size_t)ix.slots[i];
        if (ix.hs[n] == h && same(n)) return (int)n;
    }
    return -1;
}

int DictH::find(const V& key) const {
    if (items.size() > INDEXED) {
        if (idx.made_for != items.size()) idx_make(idx, items.size(), [&](size_t n) { return hash(items[n].first); });
        return idx_find(idx, hash(key), [&](size_t n) { return eq(items[n].first, key); });
    }
    if (key.k == K::Str) return find(key.str().c_str());
    for (size_t n = 0; n < items.size(); n++)
        if (eq(items[n].first, key)) return (int)n;
    return -1;
}

int SeqH::find(const V& item) const {
    if (v.size() > INDEXED) {
        if (idx.made_for != v.size()) idx_make(idx, v.size(), [&](size_t n) { return hash(v[n]); });
        return idx_find(idx, hash(item), [&](size_t n) { return eq(v[n], item); });
    }
    for (size_t n = 0; n < v.size(); n++)
        if (eq(v[n], item)) return (int)n;
    return -1;
}

int DictH::find(const char* key) const { return findn(key, std::strlen(key)); }

int DictH::findn(const char* key, size_t len) const {
    if (items.size() > INDEXED) {
        if (idx.made_for != items.size()) idx_make(idx, items.size(), [&](size_t n) { return hash(items[n].first); });
        const size_t h = std::hash<std::string_view>()(std::string_view(key, len));
        return idx_find(idx, h, [&](size_t n) {
            const V& k = items[n].first;
            return k.k == K::Str && k.str().size() == len && std::memcmp(k.str().data(), key, len) == 0;
        });
    }
    for (size_t n = 0; n < items.size(); n++) {
        const V& k = items[n].first;
        if (k.k == K::Str) {
            const std::string& s = static_cast<StrH*>(k.h.get())->s;
            if (s.size() == len && (len == 0 || s[0] == key[0]) && std::memcmp(s.data(), key, len) == 0) return (int)n;
        }
    }
    return -1;
}

const V& V::get(const V& key) const {
    if (k != K::Dict) return NONE;
    auto& d = dict_();
    int n = d.find(key);
    return n < 0 ? NONE : d.items[n].second;
}
const V& V::getn(const char* key, size_t len) const {
    if (k != K::Dict) return NONE;
    auto& d = dict_();
    int n = d.findn(key, len);
    return n < 0 ? NONE : d.items[n].second;
}
const V& V::get(const char* key) const {
    if (k != K::Dict) return NONE;
    auto& d = dict_();
    int n = d.find(key);
    return n < 0 ? NONE : d.items[n].second;
}
const V& V::at(const char* key) const {
    auto& d = dict_();
    int n = d.find(key);
    if (n < 0) raise("KeyError", std::string("'") + key + "'");
    return d.items[n].second;
}
const V& V::at(const V& key) const {
    if (k == K::List || k == K::Tuple) {
        int64_t n = key.as_int();
        auto& s = seq();
        if (n < 0) n += (int64_t)s.size();
        if (n < 0 || n >= (int64_t)s.size()) raise("IndexError", "list index out of range");
        return s[(size_t)n];
    }
    auto& d = dict_();
    int n = d.find(key);
    if (n < 0) raise("KeyError", repr(key));
    return d.items[n].second;
}
bool V::has(const char* key) const { return k == K::Dict && dict_().find(key) >= 0; }
bool V::has(const V& key) const { return k == K::Dict && dict_().find(key) >= 0; }
static void indexed_append(Index& idx, size_t old, const V& key) {
    if (idx.made_for == old) {                           // (kept up as it grows)
        idx.hs.push_back(hash(key));
        if (idx.hs.size() * 2 + 2 > idx.slots.size()) idx_fill(idx);
        else {
            const size_t mask = idx.slots.size() - 1;
            size_t i = slot_of(idx.hs.back(), mask);
            while (idx.slots[i] >= 0) i = (i + 1) & mask;
            idx.slots[i] = (int32_t)old;
        }
        idx.made_for = old + 1;
    }
}

void V::set(const V& key, V value) const {
    auto& d = dict_();
    int n = d.find(key);
    if (n < 0) {
        size_t old = d.items.size();
        d.items.emplace_back(key, std::move(value));
        indexed_append(d.idx, old, key);
    } else
        d.items[n].second = std::move(value);
}
void V::set(const char* key, V value) const { set(V(key), std::move(value)); }
void V::put(V key, V value) const {
    auto& d = dict_();
    int n = d.find(key);
    if (n < 0) {
        size_t old = d.items.size();
        d.items.emplace_back(std::move(key), std::move(value));
        indexed_append(d.idx, old, d.items.back().first);
    } else
        d.items[n].second = std::move(value);
}
V& V::setdefault(const V& key, V dflt) const {
    auto& d = dict_();
    int n = d.find(key);
    if (n < 0) {
        size_t old = d.items.size();
        d.items.emplace_back(key, std::move(dflt));
        indexed_append(d.idx, old, key);
        return d.items.back().second;
    }
    return d.items[n].second;
}
bool V::erase(const V& key) const {
    auto& d = dict_();
    int n = d.find(key);
    if (n < 0) return false;
    d.items.erase(d.items.begin() + n);
    d.idx.made_for = (size_t)-1;
    return true;
}
std::vector<std::pair<V, V>>& V::items() const { return dict_().items; }

bool truthy(const V& v) {
    switch (v.k) {
        case K::None: return false;
        case K::Bool: return v.b;
        case K::Int: return v.i != 0;
        case K::Float: return v.f != 0.0;
        case K::Str: return !v.str().empty();
        case K::List: case K::Tuple: case K::Set: return !v.seq().empty();
        case K::Dict: return !v.dict_().items.empty();
    }
    return false;
}

static bool numeric(const V& v) { return v.k == K::Int || v.k == K::Bool || v.k == K::Float; }

bool eq(const V& a, const V& b) {
    if (numeric(a) && numeric(b)) {
        if (a.k == K::Float || b.k == K::Float) return a.as_float() == b.as_float();
        if (a.k == K::Int && b.k == K::Int) return a.i == b.i && a.u64 == b.u64;
        return a.as_int() == b.as_int();
    }
    if (a.k != b.k) return false;
    switch (a.k) {
        case K::None: return true;
        case K::Str: return a.h == b.h || a.str() == b.str();
        case K::List: case K::Tuple: {
            if (a.h == b.h) return true;
            auto &x = a.seq(), &y = b.seq();
            if (x.size() != y.size()) return false;
            for (size_t n = 0; n < x.size(); n++)
                if (!eq(x[n], y[n])) return false;
            return true;
        }
        case K::Set: {
            auto &x = a.seq(), &y = b.seq();
            if (x.size() != y.size()) return false;
            for (auto& e : x)
                if (!contains(b, e)) return false;
            return true;
        }
        case K::Dict: {
            if (a.h == b.h) return true;
            auto &x = a.items(), &y = b.items();
            if (x.size() != y.size()) return false;
            for (auto& kv : x) {
                int n = b.dict_().find(kv.first);
                if (n < 0 || !eq(kv.second, y[n].second)) return false;
            }
            return true;
        }
        default: return false;
    }
}

bool operator==(const V& a, const char* s) { return a.k == K::Str && a.str() == s; }

static int rank(const V& v) {
    switch (v.k) {
        case K::None: return 0;
        case K::Bool: case K::Int: case K::Float: return 1;
        case K::Str: return 2;
        case K::Tuple: return 3;
        case K::List: return 4;
        default: return 5;
    }
}

bool lt(const V& a, const V& b) {
    if (numeric(a) && numeric(b)) {
        if (a.k == K::Float || b.k == K::Float) return a.as_float() < b.as_float();
        if (a.u64 != b.u64) return b.u64;
        if (a.u64) return (uint64_t)a.as_int() < (uint64_t)b.as_int();
        return a.as_int() < b.as_int();
    }
    if (a.k == b.k && a.k == K::Str) return a.str() < b.str();
    if (a.k == b.k && (a.k == K::Tuple || a.k == K::List)) {
        auto &x = a.seq(), &y = b.seq();
        size_t n = 0;
        while (n < x.size() && n < y.size() && eq(x[n], y[n])) n++;
        if (n < x.size() && n < y.size()) return lt(x[n], y[n]);
        return x.size() < y.size();
    }
    if (a.k == K::None && b.k == K::None) return false;
    // (Python raises TypeError comparing these; a deterministic order instead)
    if (rank(a) != rank(b)) raise("TypeError", "'<' not supported between " + repr(a) + " and " + repr(b));
    return false;
}

size_t hash(const V& v) {
    switch (v.k) {
        case K::None: return 0x9e3779b9;
        case K::Bool: case K::Int: return std::hash<int64_t>()(v.as_int());
        case K::Float: {
            double f = v.f;
            if (f == std::floor(f) && std::fabs(f) < 9e18) return std::hash<int64_t>()((int64_t)f);
            return std::hash<double>()(f);
        }
        case K::Str: return std::hash<std::string_view>()(std::string_view(v.str()));
        case K::Tuple: case K::List: {
            size_t h = 0x345678;
            for (auto& x : v.seq()) h = h * 1000003 ^ hash(x);
            return h;
        }
        default: return std::hash<const void*>()(v.id());
    }
}

bool contains(const V& c, const V& item) {
    switch (c.k) {
        case K::Set: return static_cast<SeqH*>(c.h.get())->find(item) >= 0;
        case K::List: case K::Tuple:
            for (auto& x : c.seq())
                if (eq(x, item)) return true;
            return false;
        case K::Dict: return c.dict_().find(item) >= 0;
        case K::Str: return item.k == K::Str && c.str().find(item.str()) != std::string::npos;
        case K::None: raise("TypeError", "argument of type 'NoneType' is not iterable");
        default: raise("TypeError", "argument is not iterable");
    }
}

std::string repr(const V& v) {
    switch (v.k) {
        case K::None: return "None";
        case K::Bool: return v.b ? "True" : "False";
        case K::Int: return v.u64 ? std::to_string((uint64_t)v.i) : std::to_string(v.i);
        case K::Float: return float_repr(v.f);
        case K::Str: return str_repr(v.str());
        case K::List: case K::Tuple: case K::Set: {
            auto& s = v.seq();
            if (v.k == K::Set && s.empty()) return "set()";
            std::string out = v.k == K::List ? "[" : v.k == K::Tuple ? "(" : "{";
            for (size_t n = 0; n < s.size(); n++) {
                if (n) out += ", ";
                out += repr(s[n]);
            }
            if (v.k == K::Tuple && s.size() == 1) out += ",";
            out += v.k == K::List ? "]" : v.k == K::Tuple ? ")" : "}";
            return out;
        }
        case K::Dict: {
            std::string out = "{";
            bool first = true;
            for (auto& kv : v.items()) {
                if (!first) out += ", ";
                first = false;
                out += repr(kv.first) + ": " + repr(kv.second);
            }
            return out + "}";
        }
    }
    return "?";
}

std::string str_of(const V& v) { return v.k == K::Str ? v.str() : repr(v); }

void sort_values(std::vector<V>& v) { std::stable_sort(v.begin(), v.end(), [](const V& a, const V& b) { return lt(a, b); }); }

V sorted(const V& it) {
    std::vector<V> out;
    if (it.k == K::Dict)
        for (auto& kv : it.items()) out.push_back(kv.first);
    else if (it.k != K::None)
        out = it.seq();
    sort_values(out);
    return V::list(std::move(out));
}

V copy_dict(const V& d) {
    V out = V::dict();
    if (d.k == K::Dict) out.items() = d.items();
    return out;
}

V set_add(const V& s, const V& item) {
    if (!contains(s, item)) {
        auto* h = static_cast<SeqH*>(s.h.get());
        size_t old = h->v.size();
        h->v.push_back(item);
        indexed_append(h->idx, old, item);
    }
    return s;
}
bool set_discard(const V& s, const V& item) {
    int n = static_cast<SeqH*>(s.h.get())->find(item);
    if (n < 0) return false;
    s.seq().erase(s.seq().begin() + n);
    static_cast<SeqH*>(s.h.get())->idx.made_for = (size_t)-1;
    return true;
}

}  // namespace py
