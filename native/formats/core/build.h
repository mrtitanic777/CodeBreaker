// build.h -- an answer built as a tree (a Value) and written at the end, for readers whose Python
// built dicts and lists as it went: put() is a dict assignment (a key met again keeps its place),
// emit() writes the tree as the pickle Out carries.  Text is UTF-8 (a lone surrogate as the
// three-byte form pickle's "surrogatepass" reads back).
#pragma once

#include <string>
#include <utility>

#include "pyvalue.h"

namespace cbf {

inline Value vnone() { return Value(); }
inline Value vint(int64_t i) {
    Value v;
    v.kind = Value::Int;
    v.i = i;
    return v;
}
inline Value vbool(bool b) {
    Value v;
    v.kind = Value::Bool;
    v.i = b ? 1 : 0;
    return v;
}
inline Value vstr(std::string s) {
    Value v;
    v.kind = Value::Str;
    v.s = std::move(s);
    return v;
}
inline Value vbytes(Bytes b) {
    Value v;
    v.kind = Value::Bytes;
    v.s = b.str();
    return v;
}
inline Value vcontainer(Value::Kind k) {
    Value v;
    v.kind = k;
    return v;
}
inline Value vlist() { return vcontainer(Value::List); }
inline Value vtuple() { return vcontainer(Value::Tuple); }
inline Value vdict() { return vcontainer(Value::Dict); }

inline bool same_key(const Value &a, const Value &b) {
    return a.kind == b.kind && (a.kind == Value::Str ? a.s == b.s : a.i == b.i);
}
// d[key] = v
inline void put(Value &d, Value key, Value v) {
    for (size_t k = 0; k + 1 < d.items.size(); k += 2)
        if (same_key(d.items[k], key)) {
            d.items[k + 1] = std::move(v);
            return;
        }
    d.items.push_back(std::move(key));
    d.items.push_back(std::move(v));
}
inline void put(Value &d, const char *key, Value v) { put(d, vstr(key), std::move(v)); }
inline void put(Value &d, int64_t key, Value v) { put(d, vint(key), std::move(v)); }
// d.get(key), or nullptr
inline Value *lookup(Value &d, const Value &key) {
    for (size_t k = 0; k + 1 < d.items.size(); k += 2)
        if (same_key(d.items[k], key)) return &d.items[k + 1];
    return nullptr;
}
inline void push(Value &list, Value v) { list.items.push_back(std::move(v)); }
inline Value vtuple_of(std::initializer_list<Value> xs) {
    Value t = vtuple();
    for (const Value &x : xs) t.items.push_back(x);
    return t;
}

inline void emit(Out &o, const Value &v) {
    switch (v.kind) {
    case Value::None: o.none(); break;
    case Value::Bool: o.boolean(v.i != 0); break;
    case Value::Int: o.integer(v.i); break;
    case Value::Str: o.text(v.s); break;
    case Value::Bytes: o.bytes(Bytes(v.s)); break;
    case Value::Tuple:
        o.begin_tuple();
        for (const Value &x : v.items) emit(o, x);
        o.end_tuple();
        break;
    case Value::List:
        o.begin_list();
        for (const Value &x : v.items) emit(o, x);
        o.end_list();
        break;
    case Value::Set:
        o.begin_set();
        for (const Value &x : v.items) emit(o, x);
        o.end_set();
        break;
    case Value::Dict:
        o.begin_dict();
        for (const Value &x : v.items) emit(o, x);
        o.end_dict();
        break;
    }
}

}  // namespace cbf
