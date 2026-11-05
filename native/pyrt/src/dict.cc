// dict.cc -- dict (insertion ordered, CPython's compact layout: entries in order plus a hash index),
// set and frozenset (CPython's own open-addressing table, linear probes then perturbation, the same
// growth: so iterating a set of ints gives CPython's order), their methods and views, the hashes of
// tuples and frozensets as CPython computes them.
#include <algorithm>
#include <cstring>

#include "rt.hpp"

namespace py {

bool same_or_eq(const var& a, const var& b);

static bool key_eq(const var& a, int64_t ha, const var& b, int64_t hb) {
    if (ha != hb) return false;
    if (a.is_ref() && b.is_ref() && a.ref() == b.ref()) return true;
    if (a.is_ref() && b.is_ref() && a.ref()->type == T_str && b.ref()->type == T_str)
        return static_cast<StrObj*>(a.ref())->s == static_cast<StrObj*>(b.ref())->s;
    return same_or_eq(a, b);
}

// ------------------------------------------------------------------------------------------ dict
void DictObj::rebuild_(size_t cap) {
    // compact the entries, then index them
    if (used != (int64_t)entries.size()) {
        std::vector<Entry> live;
        live.reserve((size_t)used);
        for (auto& e : entries)
            if (!e.key.is_unbound()) live.push_back(std::move(e));
        entries.swap(live);
    }
    size_t n = 8;
    while (n < cap * 3 / 2 + 1) n <<= 1;
    index.assign(n, -1);
    size_t mask = n - 1;
    for (size_t i = 0; i < entries.size(); i++) {
        uint64_t perturb = (uint64_t)entries[i].hash;
        size_t j = (size_t)entries[i].hash & mask;
        while (index[j] != -1) {
            perturb >>= 5;
            j = (j * 5 + 1 + (size_t)perturb) & mask;
        }
        index[j] = (int32_t)i;
    }
}
int64_t DictObj::find(const var& key, int64_t h) const {
    if (index.empty()) return -1;
    size_t mask = index.size() - 1;
    uint64_t perturb = (uint64_t)h;
    size_t j = (size_t)h & mask;
    for (;;) {
        int32_t ix = index[j];
        if (ix == -1) return -1;
        if (ix >= 0) {
            const Entry& e = entries[(size_t)ix];
            if (!e.key.is_unbound() && key_eq(e.key, e.hash, key, h)) return ix;
        }
        perturb >>= 5;
        j = (j * 5 + 1 + (size_t)perturb) & mask;
    }
}
var* DictObj::get(const var& key) const {
    int64_t i = find(key, hash_(key));
    return i < 0 ? nullptr : const_cast<var*>(&entries[(size_t)i].value);
}
int64_t str_hash_(const StrObj* s);
var* DictObj::get(const char* name) const {
    if (index.empty()) return nullptr;
    // hash the C string as a str hashes (FNV-1a of its UTF-8)
    uint64_t hh = 1469598103934665603ULL;
    for (const unsigned char* p = (const unsigned char*)name; *p; p++) { hh ^= *p; hh *= 1099511628211ULL; }
    int64_t h = (int64_t)(hh >> 1);
    if (h == -1) h = -2;
    size_t mask = index.size() - 1;
    uint64_t perturb = (uint64_t)h;
    size_t j = (size_t)h & mask;
    for (;;) {
        int32_t ix = index[j];
        if (ix == -1) return nullptr;
        if (ix >= 0) {
            const Entry& e = entries[(size_t)ix];
            if (e.hash == h && e.key.is_ref() && e.key.ref()->type->layout == Layout::Str &&
                static_cast<StrObj*>(e.key.ref())->s == name)
                return const_cast<var*>(&e.value);
        }
        perturb >>= 5;
        j = (j * 5 + 1 + (size_t)perturb) & mask;
    }
}
static void bump(DictObj* d) {
    d->version++;
    if (d->owner_type) {
        // a class namespace changed: the class and its subclasses' caches
        d->owner_type->version++;
    }
}
void DictObj::set(const var& key, const var& v) {
    int64_t h = hash_(key);
    int64_t i = find(key, h);
    if (i >= 0) {
        entries[(size_t)i].value = v;
        if (owner_type) owner_type->version++;
        return;
    }
    if ((entries.size() + 1) * 3 >= index.size() * 2) rebuild_((size_t)used + 1);
    entries.push_back({h, key, v});
    used++;
    size_t mask = index.size() - 1;
    uint64_t perturb = (uint64_t)h;
    size_t j = (size_t)h & mask;
    while (index[j] >= 0) {
        perturb >>= 5;
        j = (j * 5 + 1 + (size_t)perturb) & mask;
    }
    index[j] = (int32_t)(entries.size() - 1);
    bump(this);
}
bool DictObj::del(const var& key) {
    int64_t h = hash_(key);
    int64_t i = find(key, h);
    if (i < 0) return false;
    // mark the index slot deleted
    size_t mask = index.size() - 1;
    uint64_t perturb = (uint64_t)h;
    size_t j = (size_t)h & mask;
    while (index[j] != (int32_t)i) {
        perturb >>= 5;
        j = (j * 5 + 1 + (size_t)perturb) & mask;
    }
    index[j] = -2;
    entries[(size_t)i].key = var();
    entries[(size_t)i].value = var();
    used--;
    bump(this);
    if (used == 0) { entries.clear(); index.assign(8, -1); }
    return true;
}
void DictObj::clear() {
    entries.clear();
    index.assign(8, -1);
    used = 0;
    bump(this);
}

var new_dict_obj(TypeObj* t) {
    auto* d = new DictObj();
    d->type = t ? t : T_dict;
    d->index.assign(8, -1);
    return var(static_cast<Obj*>(d));
}
var dict(std::initializer_list<std::pair<var, var>> items) {
    var d = new_dict_obj();
    auto* o = static_cast<DictObj*>(d.ref());
    for (auto& [k, v] : items) o->set(k, v);
    return d;
}
bool is_dict(const var& x) { return x.is_ref() && x.ref()->type->layout == Layout::Dict; }
static DictObj* D(const var& v) { return static_cast<DictObj*>(v.ref()); }

var dict_getitem_(const var& d, const var& key) {
    DictObj* o = D(d);
    if (var* v = o->get(key)) return *v;
    if (o->type != T_dict) {
        if (!o->default_factory.is_unbound() && !o->default_factory.is_none()) {
            var v = call0(o->default_factory);
            o->set(key, v);
            return v;
        }
        var m;
        if (lookup_special(d, "__missing__", m)) return call1(m, key);
    }
    raise(make_exc_args(E_KeyError, {key}));
}
void dict_setitem_(const var& d, const var& key, const var& v) { D(d)->set(key, v); }
void dict_delitem_(const var& d, const var& key) {
    if (!D(d)->del(key)) raise(make_exc_args(E_KeyError, {key}));
}
bool dict_contains_(const var& d, const var& key) { return D(d)->find(key, hash_(key)) >= 0; }
void dict_update_(const var& d, const var& m) {
    DictObj* o = D(d);
    if (is_dict(m)) {
        for (auto& e : D(m)->entries)
            if (!e.key.is_unbound()) o->set(e.key, e.value);
        return;
    }
    if (m.hasattr("keys")) {
        for (var k : each(m.call("keys"))) o->set(k, m.getitem(k));
        return;
    }
    int64_t i = 0;
    for (var item : each(m)) {
        std::vector<var> kv = collect(item);
        if (kv.size() != 2)
            raise(E_ValueError, "dictionary update sequence element #" + std::to_string(i) + " has length " + std::to_string(kv.size()) + "; 2 is required");
        o->set(kv[0], kv[1]);
        i++;
    }
}
bool dict_eq_(const var& a, const var& b, bool& ok) {
    ok = true;
    DictObj *x = D(a), *y = D(b);
    if (x->used != y->used) return false;
    for (auto& e : x->entries) {
        if (e.key.is_unbound()) continue;
        var* v = y->get(e.key);
        if (!v || !same_or_eq(e.value, *v)) return false;
    }
    return true;
}
std::string dict_repr_(const var& d) {
    DictObj* o = D(d);
    std::string s = "{";
    bool first = true;
    for (auto& e : o->entries) {
        if (e.key.is_unbound()) continue;
        if (!first) s += ", ";
        first = false;
        s += repr_(e.key) + ": " + repr_(e.value);
    }
    s += "}";
    if (o->ordered_kind == 3) return "defaultdict(" + repr_(o->default_factory) + ", " + s + ")";
    if (o->ordered_kind == 1) {
        if (o->used == 0) return "OrderedDict()";
        std::string r = "OrderedDict({";
        r = "OrderedDict(" + s + ")";
        return r;
    }
    if (o->ordered_kind == 2) return o->used == 0 ? "Counter()" : "Counter(" + s + ")";
    return s;
}

// iteration over a dict: keys, values or items, RuntimeError when the size changes meanwhile
var dict_iter_(const var& d, int which) {
    var keep = d;
    auto i = std::make_shared<size_t>(0);
    int64_t size0 = D(d)->used;
    return new_iter([keep, i, which, size0](var& out) {
        DictObj* o = D(keep);
        if (o->used != size0) raise(E_RuntimeError, "dictionary changed size during iteration");
        while (*i < o->entries.size() && o->entries[*i].key.is_unbound()) (*i)++;
        if (*i >= o->entries.size()) return false;
        auto& e = o->entries[(*i)++];
        if (which == 0) out = e.key;
        else if (which == 1) out = e.value;
        else out = new_tuple({e.key, e.value});
        return true;
    });
}

// views: dict.keys() / values() / items() -- iterable again, sized, keys and items support `in` and set ops
static TypeObj *T_keys, *T_values, *T_items;
struct ViewObj : Obj { var d; int which; };
static var view(const var& d, int which) {
    auto* v = new ViewObj();
    v->type = which == 0 ? T_keys : which == 1 ? T_values : T_items;
    v->d = d;
    v->which = which;
    return var(static_cast<Obj*>(v));
}

// ------------------------------------------------------------------------------------------ set
static const int LINEAR_PROBES = 9;
static const uint64_t PERTURB_SHIFT = 5;

bool SetObj::contains(const var& key, int64_t h) const {
    size_t mask = table.size() - 1;
    size_t i = (size_t)h & mask;
    uint64_t perturb = (uint64_t)h;
    for (;;) {
        const Slot* e = &table[i];
        if (e->key.is_unbound() && !e->dummy) return false;
        int probes = (i + LINEAR_PROBES <= mask) ? LINEAR_PROBES : 0;
        do {
            if (!e->key.is_unbound() && key_eq(e->key, e->hash, key, h)) return true;
            if (e->key.is_unbound() && !e->dummy) return false;
            e++;
        } while (probes--);
        perturb >>= PERTURB_SHIFT;
        i = (i * 5 + 1 + (size_t)perturb) & mask;
    }
}
bool SetObj::add(const var& key, int64_t h) {
    size_t mask = table.size() - 1;
    size_t i = (size_t)h & mask;
    uint64_t perturb = (uint64_t)h;
    Slot* freeslot = nullptr;
    for (;;) {
        Slot* e = &table[i];
        int probes = (i + LINEAR_PROBES <= mask) ? LINEAR_PROBES : 0;
        do {
            if (e->key.is_unbound() && !e->dummy) goto found_unused;
            if (!e->key.is_unbound()) {
                if (key_eq(e->key, e->hash, key, h)) return false;
            } else if (e->dummy && !freeslot) {
                freeslot = e;
            }
            e++;
        } while (probes--);
        perturb >>= PERTURB_SHIFT;
        i = (i * 5 + 1 + (size_t)perturb) & mask;
        continue;
    found_unused:
        if (freeslot) {
            freeslot->key = key;
            freeslot->hash = h;
            freeslot->dummy = false;
            used++;
            return true;
        }
        e->key = key;
        e->hash = h;
        fill++;
        used++;
        if ((size_t)fill * 5 >= mask * 3) resize_(used > 50000 ? used * 2 : used * 4);
        return true;
    }
}
void SetObj::resize_(int64_t minused) {
    size_t newsize = 8;
    while ((int64_t)newsize <= minused) newsize <<= 1;
    std::vector<Slot> old;
    old.swap(table);
    table.assign(newsize, Slot());
    size_t mask = newsize - 1;
    fill = used;
    for (auto& e : old) {
        if (e.key.is_unbound()) continue;
        // insert clean (no dummies, no duplicates)
        size_t i = (size_t)e.hash & mask;
        uint64_t perturb = (uint64_t)e.hash;
        for (;;) {
            Slot* s = &table[i];
            if (s->key.is_unbound()) { s->key = std::move(e.key); s->hash = e.hash; break; }
            if (i + LINEAR_PROBES <= mask) {
                bool placed = false;
                for (int j = 0; j < LINEAR_PROBES; j++) {
                    s++;
                    if (s->key.is_unbound()) { s->key = std::move(e.key); s->hash = e.hash; placed = true; break; }
                }
                if (placed) break;
            }
            perturb >>= PERTURB_SHIFT;
            i = (i * 5 + 1 + (size_t)perturb) & mask;
        }
    }
}
bool SetObj::discard(const var& key, int64_t h) {
    size_t mask = table.size() - 1;
    size_t i = (size_t)h & mask;
    uint64_t perturb = (uint64_t)h;
    for (;;) {
        Slot* e = &table[i];
        if (e->key.is_unbound() && !e->dummy) return false;
        int probes = (i + LINEAR_PROBES <= mask) ? LINEAR_PROBES : 0;
        do {
            if (!e->key.is_unbound() && key_eq(e->key, e->hash, key, h)) {
                e->key = var();
                e->dummy = true;
                used--;
                return true;
            }
            if (e->key.is_unbound() && !e->dummy) return false;
            e++;
        } while (probes--);
        perturb >>= PERTURB_SHIFT;
        i = (i * 5 + 1 + (size_t)perturb) & mask;
    }
}

var new_set(bool frozen) {
    auto* s = new SetObj();
    s->type = frozen ? T_frozenset : T_set;
    return var(static_cast<Obj*>(s));
}
static SetObj* SS(const var& v) { return static_cast<SetObj*>(v.ref()); }
static bool is_anyset(const var& x) { return x.is_ref() && (x.ref()->type->layout == Layout::Set || x.ref()->type->layout == Layout::FrozenSet); }
static void set_add(const var& s, const var& k) { SS(s)->add(k, hash_(k)); }
var set(std::initializer_list<var> items) {
    var s = new_set();
    for (auto& x : items) set_add(s, x);
    return s;
}
var set_display(std::initializer_list<var> items, std::initializer_list<int> starred) {
    var s = new_set();
    size_t i = 0;
    for (auto& x : items) {
        if (std::find(starred.begin(), starred.end(), (int)i) != starred.end()) for (var v : each(x)) set_add(s, v);
        else set_add(s, x);
        i++;
    }
    return s;
}
static void set_fill(const var& s, const var& it) {
    if (is_anyset(it)) {
        for (auto& e : SS(it)->table)
            if (!e.key.is_unbound()) SS(s)->add(e.key, e.hash);
        return;
    }
    if (is_dict(it)) {
        for (auto& e : D(it)->entries)
            if (!e.key.is_unbound()) SS(s)->add(e.key, e.hash);
        return;
    }
    for (var v : each(it)) set_add(s, v);
}
var frozenset_from(const var& iterable) {
    var s = new_set(true);
    set_fill(s, iterable);
    return s;
}
bool set_contains_(const var& s, const var& key) {
    int64_t h;
    try {
        h = hash_(key);
    } catch (error& e) {
        // a set as the key: looked up as a frozenset
        if (is_anyset(key) && isinstance_(e.value, E_TypeError)) return set_contains_(s, frozenset_from(key));
        throw;
    }
    return SS(s)->contains(key, h);
}
var set_iter_(const var& s) {
    var keep = s;
    auto i = std::make_shared<size_t>(0);
    int64_t size0 = SS(s)->used;
    return new_iter([keep, i, size0](var& out) {
        SetObj* o = SS(keep);
        if (o->used != size0) raise(E_RuntimeError, "Set changed size during iteration");
        while (*i < o->table.size() && o->table[*i].key.is_unbound()) (*i)++;
        if (*i >= o->table.size()) return false;
        out = o->table[(*i)++].key;
        return true;
    });
}
static std::vector<var> set_items(const var& s) {
    std::vector<var> v;
    for (auto& e : SS(s)->table)
        if (!e.key.is_unbound()) v.push_back(e.key);
    return v;
}
std::string set_repr_(const var& s) {
    bool frozen = s.ref()->type->layout == Layout::FrozenSet;
    std::string name = s.ref()->type->name;
    auto v = set_items(s);
    if (v.empty()) return name + "()";
    std::string r = "{";
    for (size_t i = 0; i < v.size(); i++) r += (i ? ", " : "") + repr_(v[i]);
    r += "}";
    if (frozen || s.ref()->type != T_set) return name + "(" + r + ")";
    return r;
}
static var set_like(const var& s, bool frozen) { return new_set(frozen); }
var set_binop_(const var& a, const var& b, int op, bool& ok) {
    enum { SUB = 1, AND = 9, OR = 10, XOR = 11 };
    ok = false;
    bool view_a = a.is_ref() && (a.ref()->type == T_keys || a.ref()->type == T_items);
    if (!(is_anyset(a) || view_a) || !(is_anyset(b) || (b.is_ref() && (b.ref()->type == T_keys || b.ref()->type == T_items)))) return var();
    ok = true;
    var x = a, y = b;
    if (!is_anyset(x)) { x = new_set(); set_fill(x, a); }
    if (!is_anyset(y)) { y = new_set(); set_fill(y, b); }
    bool frozen = x.ref()->type->layout == Layout::FrozenSet;
    var r = set_like(x, frozen);
    switch (op) {
    case OR:
        set_fill(r, x);
        set_fill(r, y);
        break;
    case AND:
        for (auto& e : SS(x)->table)
            if (!e.key.is_unbound() && SS(y)->contains(e.key, e.hash)) SS(r)->add(e.key, e.hash);
        break;
    case SUB:
        for (auto& e : SS(x)->table)
            if (!e.key.is_unbound() && !SS(y)->contains(e.key, e.hash)) SS(r)->add(e.key, e.hash);
        break;
    case XOR:
        for (auto& e : SS(x)->table)
            if (!e.key.is_unbound() && !SS(y)->contains(e.key, e.hash)) SS(r)->add(e.key, e.hash);
        for (auto& e : SS(y)->table)
            if (!e.key.is_unbound() && !SS(x)->contains(e.key, e.hash)) SS(r)->add(e.key, e.hash);
        break;
    default: ok = false; return var();
    }
    return r;
}
var dict_or_(const var& a, const var& b, bool& ok) {
    ok = is_dict(a) && is_dict(b);
    if (!ok) return var();
    var r = new_dict_obj(a.ref()->type == T_dict ? nullptr : nullptr);
    dict_update_(r, a);
    dict_update_(r, b);
    return r;
}
static bool subset(const var& a, const var& b) {
    for (auto& e : SS(a)->table)
        if (!e.key.is_unbound() && !SS(b)->contains(e.key, e.hash)) return false;
    return true;
}
bool set_eq_(const var& a, const var& b, bool& ok) {
    ok = true;
    return SS(a)->used == SS(b)->used && subset(a, b);
}
int set_cmp_(const var& a, const var& b, int op, bool& ok) {
    ok = true;
    enum { LT, LE, EQ, NE, GT, GE };
    int64_t na = SS(a)->used, nb = SS(b)->used;
    switch (op) {
    case EQ: return na == nb && subset(a, b);
    case NE: return !(na == nb && subset(a, b));
    case LE: return na <= nb && subset(a, b);
    case LT: return na < nb && subset(a, b);
    case GE: return na >= nb && subset(b, a);
    default: return na > nb && subset(b, a);
    }
}

// CPython's tuple hash (xxHash-based, 3.8+) and frozenset hash
int64_t tuple_hash_(const TupleObj* t) {
    if (t->h != -1) return t->h;
    const uint64_t P1 = 11400714785074694791ULL, P2 = 14029467366897019727ULL, P5 = 2870177450012600261ULL;
    uint64_t acc = P5;
    for (auto& x : t->v) {
        uint64_t lane = (uint64_t)hash_(x);
        acc += lane * P2;
        acc = (acc << 31) | (acc >> 33);
        acc *= P1;
    }
    acc += (uint64_t)t->v.size() ^ (P5 ^ 3527539UL);
    if (acc == (uint64_t)-1) acc = 1546275796;
    t->h = (int64_t)acc;
    return t->h;
}
static uint64_t shuffle_bits(uint64_t h) { return ((h ^ 89869747UL) ^ (h << 16)) * 3644798167UL; }
int64_t frozenset_hash_(const SetObj* s) {
    if (s->h != -1) return s->h;
    uint64_t hash = 0;
    for (auto& e : s->table)
        if (!e.key.is_unbound()) hash ^= shuffle_bits((uint64_t)e.hash);
    if ((s->table.size() - 1) & 1) {}   // (CPython also folds in the dummy-free entries only)
    hash ^= ((uint64_t)s->used + 1) * 1927868237UL;
    hash ^= (hash >> 11) ^ (hash >> 25);
    hash = hash * 69069U + 907133923UL;
    if (hash == (uint64_t)-1) hash = 590923713UL;
    s->h = (int64_t)hash;
    return s->h;
}

// ------------------------------------------------------------------------------------------ types
static var arg1(const CallArgs& a) { return a.pos.at(1); }

void init_dict() {
    TypeObj* t = T_dict;
    add_static(t, "__new__", [](const CallArgs& a) { return new_dict_obj(T(a.pos.at(0))); });
    add_method(t, "__init__", [](const CallArgs& a) {
        if (a.pos.size() > 2) raise(E_TypeError, "dict expected at most 1 argument, got " + std::to_string(a.pos.size() - 1));
        if (a.pos.size() == 2) dict_update_(a.pos[0], a.pos[1]);
        for (auto& [k, v] : a.kw) D(a.pos[0])->set(k, v);
        return None;
    });
    add_method(t, "__len__", [](const CallArgs& a) { return var(D(a.pos[0])->used); });
    add_method(t, "__getitem__", [](const CallArgs& a) { return dict_getitem_(a.pos[0], arg1(a)); });
    add_method(t, "__setitem__", [](const CallArgs& a) { D(a.pos[0])->set(arg1(a), a.pos.at(2)); return None; });
    add_method(t, "__delitem__", [](const CallArgs& a) { dict_delitem_(a.pos[0], arg1(a)); return None; });
    add_method(t, "__contains__", [](const CallArgs& a) { return var(dict_contains_(a.pos[0], arg1(a))); });
    add_method(t, "__iter__", [](const CallArgs& a) { return dict_iter_(a.pos[0], 0); });
    add_method(t, "__eq__", [](const CallArgs& a) {
        if (!is_dict(arg1(a))) return NotImplemented;
        bool ok;
        return var(dict_eq_(a.pos[0], a.pos[1], ok));
    });
    add_method(t, "__repr__", [](const CallArgs& a) { return new_str(dict_repr_(a.pos[0])); });
    add_method(t, "__or__", [](const CallArgs& a) { bool ok; var r = dict_or_(a.pos[0], arg1(a), ok); return ok ? r : NotImplemented; });
    add_method(t, "__ior__", [](const CallArgs& a) { dict_update_(a.pos[0], arg1(a)); return a.pos[0]; });
    add_method(t, "get", [](const CallArgs& a) {
        if (a.pos.size() < 2 || a.pos.size() > 3) raise(E_TypeError, "get expected at most 2 arguments, got " + std::to_string(a.pos.size() - 1));
        if (var* v = D(a.pos[0])->get(a.pos[1])) return *v;
        return a.pos.size() > 2 ? a.pos[2] : var(None);
    });
    add_method(t, "setdefault", [](const CallArgs& a) {
        DictObj* d = D(a.pos[0]);
        if (var* v = d->get(arg1(a))) return *v;
        var dflt = a.pos.size() > 2 ? a.pos[2] : var(None);
        d->set(a.pos[1], dflt);
        return dflt;
    });
    add_method(t, "pop", [](const CallArgs& a) {
        DictObj* d = D(a.pos[0]);
        if (var* v = d->get(arg1(a))) {
            var r = *v;
            d->del(a.pos[1]);
            return r;
        }
        if (a.pos.size() > 2) return a.pos[2];
        raise(make_exc_args(E_KeyError, {a.pos[1]}));
    });
    add_method(t, "popitem", [](const CallArgs& a) {
        DictObj* d = D(a.pos[0]);
        for (size_t i = d->entries.size(); i-- > 0;) {
            if (d->entries[i].key.is_unbound()) continue;
            var k = d->entries[i].key, v = d->entries[i].value;
            d->del(k);
            return new_tuple({k, v});
        }
        raise(E_KeyError, "popitem(): dictionary is empty");
    });
    add_method(t, "keys", [](const CallArgs& a) { return view(a.pos[0], 0); });
    add_method(t, "values", [](const CallArgs& a) { return view(a.pos[0], 1); });
    add_method(t, "items", [](const CallArgs& a) { return view(a.pos[0], 2); });
    add_method(t, "update", [](const CallArgs& a) {
        if (a.pos.size() > 1) dict_update_(a.pos[0], a.pos[1]);
        for (auto& [k, v] : a.kw) D(a.pos[0])->set(k, v);
        return None;
    });
    add_method(t, "clear", [](const CallArgs& a) { D(a.pos[0])->clear(); return None; });
    add_method(t, "copy", [](const CallArgs& a) {
        var r = new_dict_obj();
        dict_update_(r, a.pos[0]);
        return r;
    });
    add_classmethod(t, "fromkeys", [](const CallArgs& a) {
        var r = call0(a.pos[0]);
        var v = a.pos.size() > 2 ? a.pos[2] : var(None);
        for (var k : each(a.pos.at(1))) r.setitem(k, v);
        return r;
    });
    add_method(t, "__reversed__", [](const CallArgs& a) {
        std::vector<var> keys;
        for (auto& e : D(a.pos[0])->entries) if (!e.key.is_unbound()) keys.push_back(e.key);
        std::reverse(keys.begin(), keys.end());
        return iter(new_list(keys));
    });

    T_keys = new_type("dict_keys", {}, Layout::NativeData);
    T_values = new_type("dict_values", {}, Layout::NativeData);
    T_items = new_type("dict_items", {}, Layout::NativeData);
    for (TypeObj* vt : {T_keys, T_values, T_items}) {
        add_method(vt, "__iter__", [](const CallArgs& a) { auto* v = static_cast<ViewObj*>(a.pos[0].ref()); return dict_iter_(v->d, v->which); });
        add_method(vt, "__len__", [](const CallArgs& a) { return var(D(static_cast<ViewObj*>(a.pos[0].ref())->d)->used); });
        add_method(vt, "__contains__", [](const CallArgs& a) {
            auto* v = static_cast<ViewObj*>(a.pos[0].ref());
            if (v->which == 0) return var(dict_contains_(v->d, arg1(a)));
            if (v->which == 2) {
                auto kv = collect(arg1(a));
                if (kv.size() != 2) return var(false);
                var* x = D(v->d)->get(kv[0]);
                return var(x && same_or_eq(*x, kv[1]));
            }
            for (var x : each(dict_iter_(v->d, 1))) if (same_or_eq(x, arg1(a))) return var(true);
            return var(false);
        });
        add_method(vt, "__repr__", [](const CallArgs& a) {
            auto* v = static_cast<ViewObj*>(a.pos[0].ref());
            std::string r = std::string(v->type->name) + "([";
            int i = 0;
            for (var x : each(dict_iter_(v->d, v->which))) r += (i++ ? ", " : "") + repr_(x);
            return new_str(r + "])");
        });
        add_method(vt, "__reversed__", [](const CallArgs& a) {
            auto* v = static_cast<ViewObj*>(a.pos[0].ref());
            auto items = collect(dict_iter_(v->d, v->which));
            std::reverse(items.begin(), items.end());
            return iter(new_list(items));
        });
        add_method(vt, "isdisjoint", [](const CallArgs& a) {
            for (var x : each(a.pos[0])) if (in(x, arg1(a))) return var(false);
            return var(true);
        });
    }

    for (TypeObj* st : {T_set, T_frozenset}) {
        bool frozen = st == T_frozenset;
        add_static(st, "__new__", [frozen](const CallArgs& a) {
            TypeObj* cls = T(a.pos.at(0));
            var s = new_set(frozen);
            if (frozen && a.pos.size() > 1) set_fill(s, a.pos[1]);
            if (cls != (frozen ? T_frozenset : T_set)) { s.ref()->type = cls; incref(cls); }
            return s;
        });
        if (!frozen)
            add_method(st, "__init__", [](const CallArgs& a) {
                SS(a.pos[0])->clear();
                if (a.pos.size() > 1) set_fill(a.pos[0], a.pos[1]);
                return None;
            });
        add_method(st, "__len__", [](const CallArgs& a) { return var(SS(a.pos[0])->used); });
        add_method(st, "__contains__", [](const CallArgs& a) { return var(set_contains_(a.pos[0], arg1(a))); });
        add_method(st, "__iter__", [](const CallArgs& a) { return set_iter_(a.pos[0]); });
        add_method(st, "__repr__", [](const CallArgs& a) { return new_str(set_repr_(a.pos[0])); });
        add_method(st, "__eq__", [](const CallArgs& a) {
            if (!is_anyset(arg1(a))) return NotImplemented;
            bool ok;
            return var(set_eq_(a.pos[0], a.pos[1], ok));
        });
        add_method(st, "copy", [frozen](const CallArgs& a) { var r = new_set(frozen); set_fill(r, a.pos[0]); return r; });
        auto multi = [st](const char* name, int op) {
            add_method(st, name, [op](const CallArgs& a) {
                var r = a.pos[0];
                bool frozen = r.ref()->type->layout == Layout::FrozenSet;
                var cur = new_set(frozen);
                set_fill(cur, r);
                for (size_t i = 1; i < a.pos.size(); i++) {
                    var o = a.pos[i];
                    if (!is_anyset(o)) { var t2 = new_set(); set_fill(t2, o); o = t2; }
                    bool ok;
                    cur = set_binop_(cur, o, op, ok);
                }
                return cur;
            });
        };
        multi("union", 10);
        multi("intersection", 9);
        multi("difference", 1);
        multi("symmetric_difference", 11);
        add_method(st, "issubset", [](const CallArgs& a) {
            var o = arg1(a);
            if (!is_anyset(o)) { var t2 = new_set(); set_fill(t2, o); o = t2; }
            return var(subset(a.pos[0], o));
        });
        add_method(st, "issuperset", [](const CallArgs& a) {
            var o = arg1(a);
            if (!is_anyset(o)) { var t2 = new_set(); set_fill(t2, o); o = t2; }
            return var(subset(o, a.pos[0]));
        });
        add_method(st, "isdisjoint", [](const CallArgs& a) {
            for (var x : each(arg1(a))) if (set_contains_(a.pos[0], x)) return var(false);
            return var(true);
        });
        add_method(st, "__hash__", [](const CallArgs& a) {
            if (a.pos[0].ref()->type->layout != Layout::FrozenSet) raise(E_TypeError, "unhashable type: 'set'");
            return var(frozenset_hash_(SS(a.pos[0])));
        });
    }
    t = T_set;
    add_method(t, "add", [](const CallArgs& a) { set_add(a.pos[0], arg1(a)); return None; });
    add_method(t, "discard", [](const CallArgs& a) { SS(a.pos[0])->discard(arg1(a), hash_(arg1(a))); return None; });
    add_method(t, "remove", [](const CallArgs& a) {
        if (!SS(a.pos[0])->discard(arg1(a), hash_(arg1(a)))) raise(make_exc_args(E_KeyError, {arg1(a)}));
        return None;
    });
    add_method(t, "pop", [](const CallArgs& a) {
        SetObj* s = SS(a.pos[0]);
        if (s->used == 0) raise(E_KeyError, "pop from an empty set");
        size_t mask = s->table.size() - 1;
        size_t i = (size_t)s->finger & mask;
        while (s->table[i].key.is_unbound()) i = (i + 1) & mask;
        var k = s->table[i].key;
        s->table[i].key = var();
        s->table[i].dummy = true;
        s->used--;
        s->finger = (int64_t)i + 1;
        return k;
    });
    add_method(t, "clear", [](const CallArgs& a) { SS(a.pos[0])->clear(); return None; });
    add_method(t, "update", [](const CallArgs& a) {
        for (size_t i = 1; i < a.pos.size(); i++) set_fill(a.pos[0], a.pos[i]);
        return None;
    });
    auto inplace = [t](const char* name, int op) {
        add_method(t, name, [op](const CallArgs& a) {
            for (size_t i = 1; i < a.pos.size(); i++) {
                var o = a.pos[i];
                if (!is_anyset(o)) { var t2 = new_set(); set_fill(t2, o); o = t2; }
                bool ok;
                var r = set_binop_(a.pos[0], o, op, ok);
                SS(a.pos[0])->clear();
                set_fill(a.pos[0], r);
            }
            return None;
        });
    };
    inplace("intersection_update", 9);
    inplace("difference_update", 1);
    inplace("symmetric_difference_update", 11);
    add_method(t, "__ior__", [](const CallArgs& a) { set_fill(a.pos[0], arg1(a)); return a.pos[0]; });
    add_method(t, "__iand__", [](const CallArgs& a) { bool ok; var r = set_binop_(a.pos[0], arg1(a), 9, ok); SS(a.pos[0])->clear(); set_fill(a.pos[0], r); return a.pos[0]; });
    add_method(t, "__isub__", [](const CallArgs& a) {
        for (var x : each(arg1(a))) SS(a.pos[0])->discard(x, hash_(x));
        return a.pos[0];
    });
    add_method(t, "__ixor__", [](const CallArgs& a) { bool ok; var r = set_binop_(a.pos[0], arg1(a), 11, ok); SS(a.pos[0])->clear(); set_fill(a.pos[0], r); return a.pos[0]; });
}

}  // namespace py
