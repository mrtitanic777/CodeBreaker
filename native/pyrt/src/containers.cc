// containers.cc -- list, tuple, range, slice and iteration: list methods with CPython's errors, the
// stable sort with key= and reverse=, index normalisation and slice arithmetic, the iterator protocol
// (iter / next / for), unpacking, the lazy builtins (enumerate, zip, map, filter, reversed), item
// access dispatch (getitem / setitem / delitem / in) and the repr of containers.
#include <algorithm>
#include <cstring>
#include <unordered_set>

#include "rt.hpp"

namespace py {

var new_list(std::vector<var> v) {
    auto* o = new ListObj();
    o->type = T_list;
    o->v = std::move(v);
    return var(static_cast<Obj*>(o));
}
var new_tuple(std::vector<var> v) {
    if (v.empty()) {
        static Obj* empty = [] { auto* t = new TupleObj(); t->type = T_tuple; incref(t); return t; }();
        return var(empty);
    }
    auto* o = new TupleObj();
    o->type = T_tuple;
    o->v = std::move(v);
    return var(static_cast<Obj*>(o));
}
var list(std::initializer_list<var> items) { return new_list(std::vector<var>(items)); }
var list_from(std::vector<var> items) { return new_list(std::move(items)); }
var tuple(std::initializer_list<var> items) { return new_tuple(std::vector<var>(items)); }
var tuple_from(std::vector<var> items) { return new_tuple(std::move(items)); }
bool is_list(const var& x) { return x.is_ref() && x.ref()->type->layout == Layout::List; }
bool is_tuple(const var& x) { return x.is_ref() && x.ref()->type->layout == Layout::Tuple; }
std::vector<var>& list_items(const var& l) { return static_cast<ListObj*>(l.ref())->v; }
const std::vector<var>& tuple_items(const var& t) { return static_cast<TupleObj*>(t.ref())->v; }
static std::vector<var>& L(const var& v) { return static_cast<ListObj*>(v.ref())->v; }

int64_t norm_index(int64_t i, int64_t n, const char* what) {
    if (i < 0) i += n;
    if (i < 0 || i >= n) raise(E_IndexError, std::string(what) + " index out of range");
    return i;
}
static int64_t slice_bound(const var& v, int64_t dflt, int64_t n, bool neg_step, bool is_stop) {
    if (v.is_none() || v.is_unbound()) return dflt;
    int64_t x;
    if (is_big(v)) x = to_big(v).sign() < 0 ? INT64_MIN / 2 : INT64_MAX / 2;
    else x = index_(v);
    if (x < 0) {
        x += n;
        if (x < 0) x = neg_step ? -1 : 0;
    } else if (x >= n) {
        x = neg_step ? n - 1 : n;
    }
    (void)is_stop;
    return x;
}
void slice_indices(const var& lo_v, const var& hi_v, const var& step_v, int64_t n, int64_t& lo, int64_t& hi, int64_t& st, int64_t& count) {
    st = step_v.is_none() || step_v.is_unbound() ? 1 : index_(step_v);
    if (st == 0) raise(E_ValueError, "slice step cannot be zero");
    bool neg = st < 0;
    lo = slice_bound(lo_v, neg ? n - 1 : 0, n, neg, false);
    hi = slice_bound(hi_v, neg ? -1 : n, n, neg, true);
    if (neg) count = lo > hi ? (lo - hi - 1) / (-st) + 1 : 0;
    else count = hi > lo ? (hi - lo - 1) / st + 1 : 0;
}
var slice_obj(const var& lo, const var& hi, const var& step) {
    auto* s = new SliceObj();
    s->type = T_slice;
    s->start = lo.is_unbound() ? var(None) : lo;
    s->stop = hi.is_unbound() ? var(None) : hi;
    s->step = step.is_unbound() ? var(None) : step;
    return var(static_cast<Obj*>(s));
}

// ------------------------------------------------------------------------------------------ range
var range(const var& stop) { return range(0, stop, 1); }
var range(const var& start, const var& stop, const var& step) {
    auto* r = new RangeObj();
    r->type = T_range;
    if (!is_int(start) || !is_int(stop) || !is_int(step))
        raise(E_TypeError, "'" + tname(!is_int(start) ? start : !is_int(stop) ? stop : step) + "' object cannot be interpreted as an integer");
    r->start = start.is_bool() ? var(start.small()) : start;
    r->stop = stop.is_bool() ? var(stop.small()) : stop;
    r->step = step.is_bool() ? var(step.small()) : step;
    if (to_big(step).sign() == 0) { delete r; raise(E_ValueError, "range() arg 3 must not be zero"); }
    r->small = !is_big(start) && !is_big(stop) && !is_big(step);
    if (r->small) { r->a = start.as_int64(); r->b = stop.as_int64(); r->s = step.as_int64(); }
    return var(static_cast<Obj*>(r));
}
static int64_t range_len(const RangeObj* r) {
    if (r->s > 0) return r->b > r->a ? (r->b - r->a - 1) / r->s + 1 : 0;
    return r->a > r->b ? (r->a - r->b - 1) / (-r->s) + 1 : 0;
}

// ------------------------------------------------------------------------------------------ iteration
var new_iter(std::function<bool(var&)> next) {
    auto* it = new IterObj();
    it->type = T_iter;
    it->next = std::move(next);
    return var(static_cast<Obj*>(it));
}
var dict_iter_(const var& d, int which);   // dict.cc: 0 keys, 1 values, 2 items
var set_iter_(const var& s);

var iter(const var& x) {
    if (x.is_ref()) {
        Obj* o = x.ref();
        TypeObj* t = o->type;
        if (t == T_iter || t == T_generator) return x;
        if (t->builtin || !t->lookup("__iter__")) {
            switch (t->layout) {
            case Layout::List: {
                var keep = x;
                auto i = std::make_shared<size_t>(0);
                return new_iter([keep, i](var& out) {
                    auto& v = L(keep);
                    if (*i >= v.size()) return false;
                    out = v[(*i)++];
                    return true;
                });
            }
            case Layout::Tuple: {
                var keep = x;
                auto i = std::make_shared<size_t>(0);
                return new_iter([keep, i](var& out) {
                    auto& v = tuple_items(keep);
                    if (*i >= v.size()) return false;
                    out = v[(*i)++];
                    return true;
                });
            }
            case Layout::Range: {
                auto* r = static_cast<RangeObj*>(o);
                if (r->small) {
                    auto cur = std::make_shared<int64_t>(r->a);
                    int64_t stop = r->b, step = r->s;
                    return new_iter([cur, stop, step](var& out) {
                        if (step > 0 ? *cur >= stop : *cur <= stop) return false;
                        out = *cur;
                        *cur += step;
                        return true;
                    });
                }
                auto cur = std::make_shared<var>(r->start);
                var stop = r->stop, step = r->step;
                bool up = to_big(step).sign() > 0;
                return new_iter([cur, stop, step, up](var& out) {
                    if (up ? !(*cur < stop) : !(*cur > stop)) return false;
                    out = *cur;
                    *cur = *cur + step;
                    return true;
                });
            }
            case Layout::Str: {
                var keep = x;
                auto i = std::make_shared<size_t>(0);
                return new_iter([keep, i](var& out) {
                    auto* s = static_cast<StrObj*>(keep.ref());
                    if (*i >= s->s.size()) return false;
                    size_t j = *i + 1;
                    while (j < s->s.size() && ((unsigned char)s->s[j] & 0xC0) == 0x80) j++;
                    out = new_str(s->s.substr(*i, j - *i));
                    *i = j;
                    return true;
                });
            }
            case Layout::Bytes: case Layout::ByteArray: {
                var keep = x;
                auto i = std::make_shared<size_t>(0);
                return new_iter([keep, i](var& out) {
                    auto& s = static_cast<BytesObj*>(keep.ref())->s;
                    if (*i >= s.size()) return false;
                    out = var((int64_t)(unsigned char)s[(*i)++]);
                    return true;
                });
            }
            case Layout::Dict: return dict_iter_(x, 0);
            case Layout::Set: case Layout::FrozenSet: return set_iter_(x);
            default: break;
            }
        }
        var m;
        if (lookup_special(x, "__iter__", m)) return call0(m);
        if (lookup_special(x, "__getitem__", m)) {
            auto i = std::make_shared<int64_t>(0);
            return new_iter([m, i](var& out) {
                try {
                    out = call1(m, var((*i)++));
                } catch (error& e) {
                    if (isinstance_(e.value, E_IndexError) || isinstance_(e.value, E_StopIteration)) return false;
                    throw;
                }
                return true;
            });
        }
    }
    raise(E_TypeError, "'" + tname(x) + "' object is not iterable");
}

bool gen_next_(const var& g, var& out);   // gen.cc
bool next_(const var& it, var& out) {
    if (it.is_ref()) {
        TypeObj* t = it.ref()->type;
        if (t == T_iter) return static_cast<IterObj*>(it.ref())->next(out);
        if (t == T_generator) return gen_next_(it, out);
    }
    var m;
    if (!lookup_special(it, "__next__", m)) raise(E_TypeError, "'" + tname(it) + "' object is not an iterator");
    try {
        out = call0(m);
        return true;
    } catch (error& e) {
        if (isinstance_(e.value, E_StopIteration)) return false;
        throw;
    }
}
var next(const var& it) {
    var out;
    if (!next_(it, out)) raise_stop(None);
    return out;
}
var next(const var& it, const var& dflt) {
    var out;
    if (!next_(it, out)) return dflt;
    return out;
}

each::each(const var& x) : src_(x) {
    if (x.is_ref() && x.ref()->type == T_list) mode_ = 1;
    else if (x.is_ref() && x.ref()->type == T_tuple) mode_ = 2;
    else if (x.is_ref() && x.ref()->type == T_range && static_cast<RangeObj*>(x.ref())->small) {
        auto* r = static_cast<RangeObj*>(x.ref());
        mode_ = 3;
        i_ = r->a;
        stop_ = r->b;
        step_ = r->s;
    } else it_ = iter(x);
}
bool each::advance_() {
    switch (mode_) {
    case 1: {
        auto& v = L(src_);
        if ((size_t)i_ >= v.size()) return false;
        cur_ = v[(size_t)i_++];
        return true;
    }
    case 2: {
        auto& v = tuple_items(src_);
        if ((size_t)i_ >= v.size()) return false;
        cur_ = v[(size_t)i_++];
        return true;
    }
    case 3:
        if (step_ > 0 ? i_ >= stop_ : i_ <= stop_) return false;
        cur_ = i_;
        i_ += step_;
        return true;
    default: return next_(it_, cur_);
    }
}

std::vector<var> collect(const var& x) {
    if (is_list(x) && x.ref()->type == T_list) return L(x);
    if (is_tuple(x) && x.ref()->type == T_tuple) return tuple_items(x);
    std::vector<var> out;
    for (var v : each(x)) out.push_back(v);
    return out;
}
std::vector<var> to_vector(const var& iterable) { return collect(iterable); }

std::vector<var> unpack(const var& x, size_t n) {
    std::vector<var> v;
    if (is_tuple(x)) v = tuple_items(x);
    else if (is_list(x)) v = L(x);
    else {
        var it = iter(x);
        var item;
        while (v.size() <= n && next_(it, item)) v.push_back(item);
        if (v.size() > n) raise(E_ValueError, "too many values to unpack (expected " + std::to_string(n) + ")");
    }
    if (v.size() < n) raise(E_ValueError, "not enough values to unpack (expected " + std::to_string(n) + ", got " + std::to_string(v.size()) + ")");
    if (v.size() > n) raise(E_ValueError, "too many values to unpack (expected " + std::to_string(n) + ")");
    return v;
}
std::vector<var> unpack_star(const var& x, size_t before, size_t after) {
    std::vector<var> v = collect(x);
    if (v.size() < before + after)
        raise(E_ValueError, "not enough values to unpack (expected at least " + std::to_string(before + after) + ", got " + std::to_string(v.size()) + ")");
    std::vector<var> out(v.begin(), v.begin() + (long long)before);
    out.push_back(new_list(std::vector<var>(v.begin() + (long long)before, v.end() - (long long)after)));
    out.insert(out.end(), v.end() - (long long)after, v.end());
    return out;
}

// ------------------------------------------------------------------------------------------ displays
var list_display(std::initializer_list<var> items, std::initializer_list<int> starred) {
    std::vector<var> out;
    size_t i = 0;
    for (auto& x : items) {
        bool st = std::find(starred.begin(), starred.end(), (int)i) != starred.end();
        if (st) for (var v : each(x)) out.push_back(v);
        else out.push_back(x);
        i++;
    }
    return new_list(std::move(out));
}
var tuple_display(std::initializer_list<var> items, std::initializer_list<int> starred) {
    var l = list_display(items, starred);
    return new_tuple(L(l));
}

// ------------------------------------------------------------------------------------------ sequences
var seq_concat_(const var& a, const var& b, bool& ok) {
    ok = true;
    Layout la = type_of(a)->layout, lb = type_of(b)->layout;
    if (la == Layout::Str && lb == Layout::Str) return new_str(str_utf8(a) + str_utf8(b));
    if (la == Layout::List && lb == Layout::List) {
        std::vector<var> v = L(a);
        v.insert(v.end(), L(b).begin(), L(b).end());
        return new_list(std::move(v));
    }
    if (la == Layout::Tuple && lb == Layout::Tuple) {
        std::vector<var> v = tuple_items(a);
        v.insert(v.end(), tuple_items(b).begin(), tuple_items(b).end());
        return new_tuple(std::move(v));
    }
    if ((la == Layout::Bytes || la == Layout::ByteArray) && is_bytes(b)) return new_bytes(raw_of(a) + raw_of(b), la == Layout::ByteArray);
    if ((la == Layout::Bytes || la == Layout::ByteArray) && b.is_ref() && b.ref()->type->name == "memoryview") return new_bytes(raw_of(a) + raw_of(b), la == Layout::ByteArray);
    ok = false;
    return var();
}
var seq_repeat_(const var& a, int64_t n, bool& ok) {
    ok = true;
    if (n < 0) n = 0;
    Layout la = type_of(a)->layout;
    if (la == Layout::Str) {
        const std::string& s = str_utf8(a);
        if (n > 0 && s.size() * (uint64_t)n > (1ULL << 34)) raise(E_MemoryError, "");
        std::string o;
        o.reserve(s.size() * (size_t)n);
        for (int64_t i = 0; i < n; i++) o += s;
        return new_str(o);
    }
    if (la == Layout::Bytes || la == Layout::ByteArray) {
        const std::string& s = raw_of(a);
        std::string o;
        for (int64_t i = 0; i < n; i++) o += s;
        return new_bytes(o, la == Layout::ByteArray);
    }
    if (la == Layout::List || la == Layout::Tuple) {
        const std::vector<var>& v = la == Layout::List ? L(a) : tuple_items(a);
        std::vector<var> o;
        o.reserve(v.size() * (size_t)n);
        for (int64_t i = 0; i < n; i++) o.insert(o.end(), v.begin(), v.end());
        return la == Layout::List ? new_list(std::move(o)) : new_tuple(std::move(o));
    }
    ok = false;
    return var();
}
bool same_or_eq(const var& a, const var& b);
int seq_compare_(const var& a, const var& b, int op, bool& ok) {
    ok = true;
    enum { LT, LE, EQ, NE, GT, GE };
    if (has_layout(a, Layout::Range)) {
        auto x = collect(a), y = collect(b);
        bool e = x.size() == y.size();
        for (size_t i = 0; e && i < x.size(); i++) e = x[i] == y[i];
        return e ? 1 : 0;
    }
    const std::vector<var>& x = is_list(a) ? L(a) : tuple_items(a);
    const std::vector<var>& y = is_list(b) ? L(b) : tuple_items(b);
    size_t i = 0;
    for (; i < x.size() && i < y.size(); i++)
        if (!same_or_eq(x[i], y[i])) break;
    if (i >= x.size() || i >= y.size()) {
        int64_t nx = (int64_t)x.size(), ny = (int64_t)y.size();
        switch (op) {
        case LT: return nx < ny;
        case LE: return nx <= ny;
        case EQ: return nx == ny;
        case NE: return nx != ny;
        case GT: return nx > ny;
        default: return nx >= ny;
        }
    }
    if (op == EQ) return 0;
    if (op == NE) return 1;
    var r;
    switch (op) {
    case LT: return x[i] < y[i];
    case LE: return x[i] <= y[i];
    case GT: return x[i] > y[i];
    default: return x[i] >= y[i];
    }
}

// ------------------------------------------------------------------------------------------ repr
static thread_local std::unordered_set<const Obj*>* t_repr_busy = nullptr;
std::string set_repr_(const var& s);
std::string dict_repr_(const var& d);
std::string container_repr_(const var& x, bool& ok) {
    ok = true;
    Layout l = type_of(x)->layout;
    if (l != Layout::List && l != Layout::Tuple && l != Layout::Dict && l != Layout::Set && l != Layout::FrozenSet) {
        ok = false;
        return "";
    }
    if (!t_repr_busy) t_repr_busy = new std::unordered_set<const Obj*>();
    const Obj* o = x.ref();
    if (t_repr_busy->count(o)) return l == Layout::List ? "[...]" : l == Layout::Dict ? "{...}" : "(...)";
    t_repr_busy->insert(o);
    struct done { const Obj* o; ~done() { t_repr_busy->erase(o); } } d{o};
    if (l == Layout::Dict) return dict_repr_(x);
    if (l == Layout::Set || l == Layout::FrozenSet) return set_repr_(x);
    const std::vector<var>& v = l == Layout::List ? L(x) : tuple_items(x);
    std::string s = l == Layout::List ? "[" : "(";
    for (size_t i = 0; i < v.size(); i++) {
        if (i) s += ", ";
        s += repr_(v[i]);
    }
    if (l == Layout::Tuple && v.size() == 1) s += ",";
    s += l == Layout::List ? "]" : ")";
    if (!x.ref()->type->builtin && l == Layout::List) return s;
    return s;
}

// ------------------------------------------------------------------------------------------ items
var str_getitem_(const var& self, const var& key);
var bytes_getitem_(const var& self, const var& key);
var dict_getitem_(const var& d, const var& key);
void dict_setitem_(const var& d, const var& key, const var& v);
void dict_delitem_(const var& d, const var& key);
bool dict_contains_(const var& d, const var& key);
bool set_contains_(const var& s, const var& key);

static var seq_getitem(const std::vector<var>& v, const var& key, bool list, const char* what) {
    if (key.is_small_int() || key.is_bool()) return v[(size_t)norm_index(key.small(), (int64_t)v.size(), what)];
    if (has_layout(key, Layout::Slice)) {
        auto* sl = static_cast<SliceObj*>(key.ref());
        int64_t lo, hi, st, cnt;
        slice_indices(sl->start, sl->stop, sl->step, (int64_t)v.size(), lo, hi, st, cnt);
        std::vector<var> o;
        o.reserve((size_t)cnt);
        for (int64_t k = 0, i = lo; k < cnt; k++, i += st) o.push_back(v[(size_t)i]);
        return list ? new_list(std::move(o)) : new_tuple(std::move(o));
    }
    if (is_big(key)) raise(E_IndexError, std::string("cannot fit 'int' into an index-sized integer"));
    var m;
    if (key.is_ref() && lookup_special(key, "__index__", m)) return seq_getitem(v, call0(m), list, what);
    raise(E_TypeError, std::string(list ? "list" : "tuple") + " indices must be integers or slices, not " + tname(key));
}

var var::getitem(const var& key) const {
    if (!is_ref()) raise(E_TypeError, "'" + tname(*this) + "' object is not subscriptable");
    Obj* o = ref();
    TypeObj* t = o->type;
    if (!t->builtin) {
        if (const var* g = t->lookup("__getitem__")) {
            if (!(g->is_ref() && g->ref()->type == T_native)) return call1(bind(*g, *this, t), key);
        } else if (t->layout == Layout::Type || t->layout == Layout::Object) {
            // Cls[x]: __class_getitem__
            if (t->layout == Layout::Type) {
                if (const var* cg = static_cast<TypeObj*>(o)->lookup("__class_getitem__")) return call1(bind(*cg, var(), static_cast<TypeObj*>(o)), key);
            }
        }
    }
    switch (t->layout) {
    case Layout::List: return seq_getitem(L(*this), key, true, "list");
    case Layout::Tuple: return seq_getitem(tuple_items(*this), key, false, "tuple");
    case Layout::Dict: return dict_getitem_(*this, key);
    case Layout::Str: return str_getitem_(*this, key);
    case Layout::Bytes: case Layout::ByteArray: return bytes_getitem_(*this, key);
    case Layout::Range: {
        auto* r = static_cast<RangeObj*>(o);
        if (has_layout(key, Layout::Slice)) {
            auto* sl = static_cast<SliceObj*>(key.ref());
            int64_t lo, hi, st, cnt;
            slice_indices(sl->start, sl->stop, sl->step, range_len(r), lo, hi, st, cnt);
            return range(r->a + lo * r->s, r->a + (lo + cnt * st) * r->s, r->s * st);
        }
        int64_t i = norm_index(index_(key), range_len(r), "range object");
        return var(r->a + i * r->s);
    }
    case Layout::Type: {
        auto* ty = static_cast<TypeObj*>(o);
        if (const var* cg = ty->lookup("__class_getitem__")) return call1(bind(*cg, var(), ty), key);
        // list[int], dict[str, int] ...: a generic alias; the class itself is enough here
        if (ty->builtin) return *this;
        break;
    }
    default: break;
    }
    if (const var* g = t->lookup("__getitem__")) return call1(bind(*g, *this, t), key);
    raise(E_TypeError, "'" + t->name + "' object is not subscriptable");
}

void var::setitem(const var& key, const var& v) const {
    if (!is_ref()) raise(E_TypeError, "'" + tname(*this) + "' object does not support item assignment");
    Obj* o = ref();
    TypeObj* t = o->type;
    if (!t->builtin)
        if (const var* s = t->lookup("__setitem__"); s && !(s->is_ref() && s->ref()->type == T_native)) {
            call_obj(bind(*s, *this, t), pack_args_(key, v));
            return;
        }
    switch (t->layout) {
    case Layout::List: {
        auto& vec = L(*this);
        if (key.is_small_int() || key.is_bool()) {
            vec[(size_t)norm_index(key.small(), (int64_t)vec.size(), "list assignment")] = v;
            return;
        }
        if (has_layout(key, Layout::Slice)) {
            auto* sl = static_cast<SliceObj*>(key.ref());
            int64_t lo, hi, st, cnt;
            slice_indices(sl->start, sl->stop, sl->step, (int64_t)vec.size(), lo, hi, st, cnt);
            std::vector<var> items = collect(v);
            if (st == 1) {
                vec.erase(vec.begin() + lo, vec.begin() + lo + cnt);
                vec.insert(vec.begin() + lo, items.begin(), items.end());
                return;
            }
            if ((int64_t)items.size() != cnt)
                raise(E_ValueError, "attempt to assign sequence of size " + std::to_string(items.size()) + " to extended slice of size " + std::to_string(cnt));
            for (int64_t k = 0, i = lo; k < cnt; k++, i += st) vec[(size_t)i] = items[(size_t)k];
            return;
        }
        vec[(size_t)norm_index(index_(key), (int64_t)vec.size(), "list assignment")] = v;
        return;
    }
    case Layout::Dict: dict_setitem_(*this, key, v); return;
    default: break;
    }
    if (const var* s = t->lookup("__setitem__")) {
        call_obj(bind(*s, *this, t), pack_args_(key, v));
        return;
    }
    raise(E_TypeError, "'" + t->name + "' object does not support item assignment");
}

void var::delitem(const var& key) const {
    Obj* o = ref();
    TypeObj* t = type_of(*this);
    if (!t->builtin)
        if (const var* s = t->lookup("__delitem__"); s && !(s->is_ref() && s->ref()->type == T_native)) {
            call1(bind(*s, *this, t), key);
            return;
        }
    switch (t->layout) {
    case Layout::List: {
        auto& vec = L(*this);
        if (has_layout(key, Layout::Slice)) {
            auto* sl = static_cast<SliceObj*>(key.ref());
            int64_t lo, hi, st, cnt;
            slice_indices(sl->start, sl->stop, sl->step, (int64_t)vec.size(), lo, hi, st, cnt);
            if (st == 1) { vec.erase(vec.begin() + lo, vec.begin() + lo + cnt); return; }
            std::vector<bool> drop(vec.size());
            for (int64_t k = 0, i = lo; k < cnt; k++, i += st) drop[(size_t)i] = true;
            std::vector<var> keep;
            for (size_t i = 0; i < vec.size(); i++) if (!drop[i]) keep.push_back(vec[i]);
            vec.swap(keep);
            return;
        }
        vec.erase(vec.begin() + norm_index(index_(key), (int64_t)vec.size(), "list assignment"));
        return;
    }
    case Layout::Dict: dict_delitem_(*this, key); return;
    default: break;
    }
    (void)o;
    if (const var* s = t->lookup("__delitem__")) {
        call1(bind(*s, *this, t), key);
        return;
    }
    raise(E_TypeError, "'" + t->name + "' object doesn't support item deletion");
}

var var::slice(const var& lo, const var& hi, const var& step) const { return getitem(slice_obj(lo, hi, step)); }

bool in(const var& x, const var& c) {
    if (!c.is_ref()) raise(E_TypeError, "argument of type '" + tname(c) + "' is not iterable");
    Obj* o = c.ref();
    TypeObj* t = o->type;
    if (!t->builtin)
        if (const var* m = t->lookup("__contains__"); m && !(m->is_ref() && m->ref()->type == T_native)) return call1(bind(*m, c, t), x).truthy();
    switch (t->layout) {
    case Layout::Str:
        if (!is_str(x)) raise(E_TypeError, "'in <string>' requires string as left operand, not " + tname(x));
        return str_utf8(c).find(str_utf8(x)) != std::string::npos;
    case Layout::List:
        for (auto& v : L(c)) if (same_or_eq(v, x)) return true;
        return false;
    case Layout::Tuple:
        for (auto& v : tuple_items(c)) if (same_or_eq(v, x)) return true;
        return false;
    case Layout::Dict: return dict_contains_(c, x);
    case Layout::Set: case Layout::FrozenSet: return set_contains_(c, x);
    case Layout::Bytes: case Layout::ByteArray: {
        const std::string& s = raw_of(c);
        if (is_int(x)) return s.find((char)x.as_int64()) != std::string::npos;
        return s.find(raw_of(x)) != std::string::npos;
    }
    case Layout::Range: {
        auto* r = static_cast<RangeObj*>(o);
        if (!is_int(x)) { for (var v : each(c)) if (v == x) return true; return false; }
        if (is_big(x)) return false;
        int64_t v = x.as_int64();
        if (r->s > 0 ? (v < r->a || v >= r->b) : (v > r->a || v <= r->b)) return false;
        return (v - r->a) % r->s == 0;
    }
    default: break;
    }
    if (const var* m = t->lookup("__contains__")) return call1(bind(*m, c, t), x).truthy();
    for (var v : each(c)) if (same_or_eq(v, x)) return true;
    return false;
}

int64_t len_(const var& x) {
    if (x.is_ref()) {
        Obj* o = x.ref();
        TypeObj* t = o->type;
        if (!t->builtin)
            if (const var* m = t->lookup("__len__"); m && !(m->is_ref() && m->ref()->type == T_native)) {
                var r = call0(bind(*m, x, t));
                int64_t n = index_(r);
                if (n < 0) raise(E_ValueError, "__len__() should return >= 0");
                return n;
            }
        switch (t->layout) {
        case Layout::Str: return static_cast<StrObj*>(o)->n;
        case Layout::List: return (int64_t)L(x).size();
        case Layout::Tuple: return (int64_t)tuple_items(x).size();
        case Layout::Dict: return static_cast<DictObj*>(o)->used;
        case Layout::Set: case Layout::FrozenSet: return static_cast<SetObj*>(o)->used;
        case Layout::Bytes: case Layout::ByteArray: return (int64_t)static_cast<BytesObj*>(o)->s.size();
        case Layout::Range: return range_len(static_cast<RangeObj*>(o));
        default: break;
        }
        if (const var* m = t->lookup("__len__")) return index_(call0(bind(*m, x, t)));
    }
    raise(E_TypeError, "object of type '" + tname(x) + "' has no len()");
}
var len(const var& x) { return len_(x); }

// ------------------------------------------------------------------------------------------ sort
static void sort_vec(std::vector<var>& v, const var& key, bool reverse) {
    std::vector<var> keys;
    bool has_key = !key.is_none() && !key.is_unbound();
    if (has_key) {
        keys.reserve(v.size());
        for (auto& x : v) keys.push_back(call1(key, x));
    }
    std::vector<size_t> idx(v.size());
    for (size_t i = 0; i < idx.size(); i++) idx[i] = i;
    const std::vector<var>& k = has_key ? keys : v;
    // reverse keeps the stability of equal items in their original order (as CPython)
    if (reverse) {
        std::reverse(idx.begin(), idx.end());
        std::stable_sort(idx.begin(), idx.end(), [&](size_t a, size_t b) { return k[a] < k[b]; });
        std::reverse(idx.begin(), idx.end());
    } else {
        std::stable_sort(idx.begin(), idx.end(), [&](size_t a, size_t b) { return k[a] < k[b]; });
    }
    std::vector<var> out;
    out.reserve(v.size());
    for (size_t i : idx) out.push_back(v[i]);
    v.swap(out);
}
var sorted_(const CallArgs& a) {
    if (a.pos.size() != 1) raise(E_TypeError, "sorted expected 1 argument, got " + std::to_string(a.pos.size()));
    std::vector<var> v = collect(a.pos[0]);
    sort_vec(v, kwarg(a, "key", None), kwarg(a, "reverse", false).truthy());
    return new_list(std::move(v));
}

// ------------------------------------------------------------------------------------------ list type
void init_containers() {
    TypeObj* t = T_list;
    add_static(t, "__new__", [](const CallArgs& a) {
        auto* o = new ListObj();
        o->type = T(a.pos.at(0));
        return var(static_cast<Obj*>(o));
    });
    add_method(t, "__init__", [](const CallArgs& a) {
        auto& v = L(a.pos[0]);
        v.clear();
        if (a.pos.size() > 1) v = collect(a.pos[1]);
        return None;
    });
    add_method(t, "__len__", [](const CallArgs& a) { return var((int64_t)L(a.pos[0]).size()); });
    add_method(t, "__getitem__", [](const CallArgs& a) { return seq_getitem(L(a.pos[0]), a.pos.at(1), true, "list"); });
    add_method(t, "__setitem__", [](const CallArgs& a) {
        var self = a.pos[0];
        TypeObj* keep = self.ref()->type;
        self.ref()->type = T_list;
        try { self.setitem(a.pos.at(1), a.pos.at(2)); } catch (...) { self.ref()->type = keep; throw; }
        self.ref()->type = keep;
        return None;
    });
    add_method(t, "__contains__", [](const CallArgs& a) {
        for (auto& v : L(a.pos[0])) if (same_or_eq(v, a.pos.at(1))) return var(true);
        return var(false);
    });
    add_method(t, "__iter__", [](const CallArgs& a) {
        var self = a.pos[0];
        auto i = std::make_shared<size_t>(0);
        return new_iter([self, i](var& out) {
            auto& v = L(self);
            if (*i >= v.size()) return false;
            out = v[(*i)++];
            return true;
        });
    });
    add_method(t, "__iadd__", [](const CallArgs& a) {
        auto& v = L(a.pos[0]);
        std::vector<var> add = collect(a.pos.at(1));
        v.insert(v.end(), add.begin(), add.end());
        return a.pos[0];
    });
    add_method(t, "__imul__", [](const CallArgs& a) {
        auto& v = L(a.pos[0]);
        int64_t n = a.pos.at(1).as_int64();
        std::vector<var> o;
        for (int64_t i = 0; i < n; i++) o.insert(o.end(), v.begin(), v.end());
        v.swap(o);
        return a.pos[0];
    });
    add_method(t, "__eq__", [](const CallArgs& a) {
        if (!is_list(a.pos.at(1))) return NotImplemented;
        bool ok;
        return var(seq_compare_(a.pos[0], a.pos[1], 2, ok) != 0);
    });
    add_method(t, "__add__", [](const CallArgs& a) {
        bool ok;
        var r = seq_concat_(a.pos[0], a.pos.at(1), ok);
        return ok ? r : NotImplemented;
    });
    add_method(t, "append", [](const CallArgs& a) {
        if (a.pos.size() != 2) raise(E_TypeError, "list.append() takes exactly one argument (" + std::to_string(a.pos.size() - 1) + " given)");
        L(a.pos[0]).push_back(a.pos[1]);
        return None;
    });
    add_method(t, "extend", [](const CallArgs& a) {
        auto& v = L(a.pos[0]);
        std::vector<var> add = collect(a.pos.at(1));
        v.insert(v.end(), add.begin(), add.end());
        return None;
    });
    add_method(t, "insert", [](const CallArgs& a) {
        auto& v = L(a.pos[0]);
        int64_t i = index_(a.pos.at(1)), n = (int64_t)v.size();
        if (i < 0) i = std::max<int64_t>(0, i + n);
        if (i > n) i = n;
        v.insert(v.begin() + i, a.pos.at(2));
        return None;
    });
    add_method(t, "pop", [](const CallArgs& a) {
        auto& v = L(a.pos[0]);
        if (v.empty()) raise(E_IndexError, "pop from empty list");
        int64_t i = a.pos.size() > 1 ? index_(a.pos[1]) : -1;
        if (i < 0) i += (int64_t)v.size();
        if (i < 0 || i >= (int64_t)v.size()) raise(E_IndexError, "pop index out of range");
        var x = v[(size_t)i];
        v.erase(v.begin() + i);
        return x;
    });
    add_method(t, "remove", [](const CallArgs& a) {
        auto& v = L(a.pos[0]);
        for (size_t i = 0; i < v.size(); i++)
            if (same_or_eq(v[i], a.pos.at(1))) { v.erase(v.begin() + (long long)i); return None; }
        raise(E_ValueError, "list.remove(x): x not in list");
    });
    add_method(t, "index", [](const CallArgs& a) {
        auto& v = L(a.pos[0]);
        int64_t n = (int64_t)v.size();
        int64_t lo = a.pos.size() > 2 ? index_(a.pos[2]) : 0, hi = a.pos.size() > 3 ? index_(a.pos[3]) : n;
        if (lo < 0) lo = std::max<int64_t>(0, lo + n);
        if (hi < 0) hi = std::max<int64_t>(0, hi + n);
        for (int64_t i = lo; i < std::min(hi, n); i++)
            if (same_or_eq(v[(size_t)i], a.pos.at(1))) return var(i);
        raise(E_ValueError, repr_(a.pos.at(1)) + " is not in list");
    });
    add_method(t, "count", [](const CallArgs& a) {
        int64_t n = 0;
        for (auto& x : L(a.pos[0])) if (same_or_eq(x, a.pos.at(1))) n++;
        return var(n);
    });
    add_method(t, "sort", [](const CallArgs& a) {
        if (a.pos.size() > 1) raise(E_TypeError, "sort() takes no positional arguments");
        auto& v = L(a.pos[0]);
        std::vector<var> work;
        work.swap(v);   // (empty while sorting, as CPython's)
        try {
            sort_vec(work, kwarg(a, "key", None), kwarg(a, "reverse", false).truthy());
        } catch (...) {
            v.swap(work);
            throw;
        }
        v.swap(work);
        return None;
    });
    add_method(t, "reverse", [](const CallArgs& a) { auto& v = L(a.pos[0]); std::reverse(v.begin(), v.end()); return None; });
    add_method(t, "copy", [](const CallArgs& a) { return new_list(L(a.pos[0])); });
    add_method(t, "clear", [](const CallArgs& a) { L(a.pos[0]).clear(); return None; });
    add_method(t, "__reversed__", [](const CallArgs& a) {
        var self = a.pos[0];
        auto i = std::make_shared<int64_t>((int64_t)L(self).size() - 1);
        return new_iter([self, i](var& out) {
            auto& v = L(self);
            if (*i < 0 || *i >= (int64_t)v.size()) return false;
            out = v[(size_t)(*i)--];
            return true;
        });
    });

    t = T_tuple;
    add_static(t, "__new__", [](const CallArgs& a) {
        TypeObj* cls = T(a.pos.at(0));
        std::vector<var> v;
        if (a.pos.size() > 1) v = collect(a.pos[1]);
        if (cls == T_tuple) return new_tuple(std::move(v));
        auto* o = new TupleObj();
        o->type = cls;
        o->v = std::move(v);
        return var(static_cast<Obj*>(o));
    });
    add_method(t, "__len__", [](const CallArgs& a) { return var((int64_t)tuple_items(a.pos[0]).size()); });
    add_method(t, "__getitem__", [](const CallArgs& a) { return seq_getitem(tuple_items(a.pos[0]), a.pos.at(1), false, "tuple"); });
    add_method(t, "__iter__", [](const CallArgs& a) {
        var self = a.pos[0];
        auto i = std::make_shared<size_t>(0);
        return new_iter([self, i](var& out) {
            auto& v = tuple_items(self);
            if (*i >= v.size()) return false;
            out = v[(*i)++];
            return true;
        });
    });
    add_method(t, "__contains__", [](const CallArgs& a) {
        for (auto& v : tuple_items(a.pos[0])) if (same_or_eq(v, a.pos.at(1))) return var(true);
        return var(false);
    });
    add_method(t, "__hash__", [](const CallArgs& a) { return var(hash_(new_tuple(tuple_items(a.pos[0])))); });
    add_method(t, "__eq__", [](const CallArgs& a) {
        if (!is_tuple(a.pos.at(1))) return NotImplemented;
        bool ok;
        return var(seq_compare_(a.pos[0], a.pos[1], 2, ok) != 0);
    });
    add_method(t, "__add__", [](const CallArgs& a) {
        bool ok;
        var r = seq_concat_(new_tuple(tuple_items(a.pos[0])), a.pos.at(1), ok);
        return ok ? r : NotImplemented;
    });
    add_method(t, "__repr__", [](const CallArgs& a) { bool ok; return new_str(container_repr_(new_tuple(tuple_items(a.pos[0])), ok)); });
    add_method(t, "index", [](const CallArgs& a) {
        auto& v = tuple_items(a.pos[0]);
        for (size_t i = 0; i < v.size(); i++) if (same_or_eq(v[i], a.pos.at(1))) return var((int64_t)i);
        raise(E_ValueError, "tuple.index(x): x not in tuple");
    });
    add_method(t, "count", [](const CallArgs& a) {
        int64_t n = 0;
        for (auto& x : tuple_items(a.pos[0])) if (same_or_eq(x, a.pos.at(1))) n++;
        return var(n);
    });

    t = T_range;
    add_static(t, "__new__", [](const CallArgs& a) {
        size_t n = a.pos.size() - 1;
        if (n == 1) return range(0, a.pos[1], 1);
        if (n == 2) return range(a.pos[1], a.pos[2], 1);
        if (n == 3) return range(a.pos[1], a.pos[2], a.pos[3]);
        raise(E_TypeError, "range expected at most 3 arguments, got " + std::to_string(n));
    });
    add_getter(t, "start", [](const var& r) { return static_cast<RangeObj*>(r.ref())->start; });
    add_getter(t, "stop", [](const var& r) { return static_cast<RangeObj*>(r.ref())->stop; });
    add_getter(t, "step", [](const var& r) { return static_cast<RangeObj*>(r.ref())->step; });
    add_method(t, "index", [](const CallArgs& a) {
        auto* r = static_cast<RangeObj*>(a.pos[0].ref());
        if (!in(a.pos.at(1), a.pos[0])) raise(E_ValueError, repr_(a.pos[1]) + " is not in range");
        return var((a.pos[1].as_int64() - r->a) / r->s);
    });
    add_method(t, "__reversed__", [](const CallArgs& a) {
        auto* r = static_cast<RangeObj*>(a.pos[0].ref());
        int64_t n = range_len(r);
        return iter(range(r->a + (n - 1) * r->s, r->a - r->s, -r->s));
    });

    t = T_slice;
    add_static(t, "__new__", [](const CallArgs& a) {
        size_t n = a.pos.size() - 1;
        if (n == 1) return slice_obj(None, a.pos[1], None);
        if (n == 2) return slice_obj(a.pos[1], a.pos[2], None);
        return slice_obj(a.pos.at(1), a.pos.at(2), a.pos.at(3));
    });
    add_getter(t, "start", [](const var& s) { return static_cast<SliceObj*>(s.ref())->start; });
    add_getter(t, "stop", [](const var& s) { return static_cast<SliceObj*>(s.ref())->stop; });
    add_getter(t, "step", [](const var& s) { return static_cast<SliceObj*>(s.ref())->step; });
    add_method(t, "indices", [](const CallArgs& a) {
        auto* s = static_cast<SliceObj*>(a.pos[0].ref());
        int64_t lo, hi, st, cnt;
        slice_indices(s->start, s->stop, s->step, a.pos.at(1).as_int64(), lo, hi, st, cnt);
        return new_tuple({var(lo), var(hi), var(st)});
    });

    // iterators are their own iterators
    add_method(T_iter, "__iter__", [](const CallArgs& a) { return a.pos[0]; });
    add_method(T_iter, "__next__", [](const CallArgs& a) { return next(a.pos[0]); });
}

}  // namespace py
