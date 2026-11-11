// codebreaker/decompile/l2tree.hpp -- helpers for reading and rewriting an L2 document held as a translate::ir::Val
// tree (frame16, shape16).
//
// The Python passes work on the document as json.loads gives it: dicts and lists that are shared, so a node changed
// through one reference is changed wherever it is held. py::dict / py::list are shared the same way (a copy of the
// handle is the same container), so the passes keep the Python's own shape; deepcopy() below is copy.deepcopy.
//
// Reading follows the Python's `x.get(k)` / `(x.get(k) or {}).get(j)` idioms: a lookup in something that is not a
// dict gives None.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <variant>

#include "codebreaker/translate/ir.hpp"
#include "py.hpp"

namespace codebreaker::decompile::l2tree {

using translate::ir::Dict;
using translate::ir::Val;
using List = Val::List;

inline bool is_dict(const Val& v) { return std::holds_alternative<Dict>(v.v); }
inline bool is_list(const Val& v) { return std::holds_alternative<List>(v.v); }
inline bool is_str(const Val& v) { return std::holds_alternative<py::str>(v.v); }
inline bool is_int(const Val& v) { return std::holds_alternative<int64_t>(v.v); }
inline const Dict& as_dict(const Val& v) { return std::get<Dict>(v.v); }
inline const List& as_list(const Val& v) { return std::get<List>(v.v); }

// The dict itself, or (for None or anything else) a fresh empty one: `x or {}`.
inline Dict dict_or_empty(const Val& v) { return is_dict(v) ? as_dict(v) : Dict(); }
// The list itself, or a fresh empty one: `x or []`.
inline List list_or_empty(const Val& v) { return is_list(v) ? as_list(v) : List(); }

// d.get(key): None where it is absent (or `v` is no dict).
inline Val get(const Val& v, const char* key) {
    if (!is_dict(v)) return Val();
    auto got = as_dict(v).get(py::str(key));
    return got ? *got : Val();
}
inline Val get(const Dict& d, const char* key) {
    auto got = d.get(py::str(key));
    return got ? *got : Val();
}
// d[key]: KeyError where it is absent.
inline const Val& at(const Dict& d, const char* key) { return d.at(py::str(key)); }

// Python truth.
inline bool truthy(const Val& v) {
    struct T {
        bool operator()(std::monostate) const { return false; }
        bool operator()(bool b) const { return b; }
        bool operator()(int64_t i) const { return i != 0; }
        bool operator()(double d) const { return d != 0.0; }
        bool operator()(const py::str& s) const { return s.size() != 0; }
        bool operator()(const translate::ir::TypeP& t) const { return static_cast<bool>(t); }
        bool operator()(const List& l) const { return !l.empty(); }
        bool operator()(const Dict& d) const { return !d.empty(); }
    };
    return std::visit(T(), v.v);
}

// x == "text" for a Val
inline bool is_text(const Val& v, const char* text) { return is_str(v) && std::get<py::str>(v.v) == py::str(text); }
// the text of a str Val, if it is one
inline std::optional<py::str> text_of(const Val& v) {
    if (is_str(v)) return std::get<py::str>(v.v);
    return std::nullopt;
}
// x == n for an int, bool or float Val (Python's numeric equality)
inline bool equals_number(const Val& v, int64_t n) {
    if (auto i = std::get_if<int64_t>(&v.v)) return *i == n;
    if (auto b = std::get_if<bool>(&v.v)) return (*b ? 1 : 0) == n;
    if (auto d = std::get_if<double>(&v.v)) return *d == static_cast<double>(n);
    return false;
}

// Python's == of two Vals (dict equality ignores order).
inline bool equal(const Val& a, const Val& b);
inline bool equal_number(const Val& a, const Val& b) {
    auto num = [](const Val& v, double& out) {
        if (auto i = std::get_if<int64_t>(&v.v)) { out = static_cast<double>(*i); return true; }
        if (auto bo = std::get_if<bool>(&v.v)) { out = *bo ? 1.0 : 0.0; return true; }
        if (auto d = std::get_if<double>(&v.v)) { out = *d; return true; }
        return false;
    };
    double x = 0, y = 0;
    if (!num(a, x) || !num(b, y)) return false;
    // (two ints compare exactly)
    if (auto i = std::get_if<int64_t>(&a.v)) if (auto j = std::get_if<int64_t>(&b.v)) return *i == *j;
    return x == y;
}
inline bool equal(const Val& a, const Val& b) {
    if (a.is_none() || b.is_none()) return a.is_none() && b.is_none();
    if (is_str(a) || is_str(b)) return is_str(a) && is_str(b) && std::get<py::str>(a.v) == std::get<py::str>(b.v);
    if (is_list(a) || is_list(b)) {
        if (!(is_list(a) && is_list(b))) return false;
        const List &x = as_list(a), &y = as_list(b);
        if (x.size() != y.size()) return false;
        for (int64_t i = 0; i < x.size(); i++) if (!equal(x[i], y[i])) return false;
        return true;
    }
    if (is_dict(a) || is_dict(b)) {
        if (!(is_dict(a) && is_dict(b))) return false;
        const Dict &x = as_dict(a), &y = as_dict(b);
        if (x.size() != y.size()) return false;
        for (const auto& kv : x.items()) {
            auto o = y.get(std::get<0>(kv));
            if (!o || !equal(std::get<1>(kv), *o)) return false;
        }
        return true;
    }
    if (std::holds_alternative<translate::ir::TypeP>(a.v) || std::holds_alternative<translate::ir::TypeP>(b.v)) {
        const auto* x = std::get_if<translate::ir::TypeP>(&a.v);
        const auto* y = std::get_if<translate::ir::TypeP>(&b.v);
        return x && y && *x == *y;
    }
    return equal_number(a, b);
}

// copy.deepcopy of a tree: new dicts and lists all the way down.
inline Val deepcopy(const Val& v) {
    if (auto d = std::get_if<Dict>(&v.v)) {
        Dict out;
        for (const auto& kv : d->items()) out.set(std::get<0>(kv), deepcopy(std::get<1>(kv)));
        return Val(out);
    }
    if (auto l = std::get_if<List>(&v.v)) {
        List out;
        for (const Val& x : l->vec()) out.append(deepcopy(x));
        return Val(out);
    }
    return v;
}
inline Dict deepcopy(const Dict& d) { return as_dict(deepcopy(Val(d))); }

// Python's `for n in _walk(tree): ...` with the body run on each dict node before its members are read: f(node) is
// called on every dict (a node before its members), and the members are read after f has run, so a node f rewrites
// in place is walked as rewritten. (`_walk` in frame16 / shape16.)
inline void walk(const Val& n, const std::function<void(const Dict&)>& f) {
    if (auto d = std::get_if<Dict>(&n.v)) {
        f(*d);
        for (const Val& v : d->values()) walk(v, f);
    } else if (auto l = std::get_if<List>(&n.v)) {
        // (a list is read by index over its live size, as Python's iterator is)
        for (int64_t i = 0; i < l->size(); i++) walk((*l)[i], f);
    }
}
// The same nodes, collected first: `list(_walk(n))`.
inline py::list<Dict> nodes(const Val& n) {
    py::list<Dict> out;
    walk(n, [&](const Dict& d) { out.append(d); });
    return out;
}

}  // namespace codebreaker::decompile::l2tree
