// cb/compat/old_l2_tree.hpp -- the old IR tree (codebreaker/translate/ir.hpp: py::ref nodes) as L2 documents (the old
// ir::Dict), kept for the ports that still hold trees of the old kind (docs/CPP_DESIGN.md 6.3).
//
// This is the tree half of the old translate/l2.cpp, moved here when l2 became cb/translate/l2 (typed nodes, the Json edge)
// and the old header became a shim: reading and writing an old tree is the one thing the new module cannot do for it. It is
// deleted with the last port that holds an old tree (translate/ir is the next module to leave; its shim then needs none of
// it). Nothing outside the shims includes it.
#pragma once

#include <cmath>
#include <map>
#include <string>

#include "codebreaker/translate/ir.hpp"
#include "py.hpp"

namespace codebreaker::translate::l2 {

namespace ir = codebreaker::translate::ir;
using ir::Dict;
using ir::NodeP;
using ir::SymP;
using ir::TypeP;
using ir::Val;

// what the Python front end says of a class type: a record (a value struct, or shared when `ref`), held by reference or not,
// an exception (made and thrown as a value)
inline const py::list<py::str> CLASS_FACTS_ = {py::str("record"), py::str("ref"), py::str("exception")};

// (defined below: the helpers use them before their place)
inline Dict to_json(const NodeP& node);
inline NodeP from_json(const Dict& d);
inline NodeP from_json(const Val& d);

namespace old_l2_detail {
inline py::str K(const char* s) { return py::str(s); }
inline py::str utf8_str(const std::string& s) { return py::bytes(s).decode("utf-8"); }
}  // namespace old_l2_detail
using namespace old_l2_detail;

namespace old_l2_detail {

// ---------------------------------------------------------------------------------------------- Val helpers
inline bool val_none(const Val& v) { return v.is_none(); }

template <class T> bool holds(const Val& v) { return std::holds_alternative<T>(v.v); }
template <class T> const T& as(const Val& v) { return std::get<T>(v.v); }

// Python truthiness of a value
inline bool truthy(const Val& v) {
    return std::visit(
        [](const auto& x) -> bool {
            using T = std::decay_t<decltype(x)>;
            if constexpr (std::is_same_v<T, std::monostate>) return false;
            else if constexpr (std::is_same_v<T, bool>) return x;
            else if constexpr (std::is_same_v<T, int64_t>) return x != 0;
            else if constexpr (std::is_same_v<T, double>) return x != 0.0;
            else if constexpr (std::is_same_v<T, py::str>) return py::len(x) != 0;
            else if constexpr (std::is_same_v<T, ir::TypeP>) return true;
            else return !x.empty();   // a list or a dict
        },
        v.v);
}

// `v != 1` (the default `rank` of an array)
inline bool not_one(const Val& v) {
    if (holds<bool>(v)) return (as<bool>(v) ? 1 : 0) != 1;
    if (holds<int64_t>(v)) return as<int64_t>(v) != 1;
    if (holds<double>(v)) return as<double>(v) != 1.0;
    return true;
}

inline bool dhas(const Dict& d, const char* key) { return d.contains(K(key)); }
inline Val dget(const Dict& d, const char* key) { return d.get(K(key), Val()); }          // d.get(key)
inline Val dget(const Dict& d, const char* key, const Val& dflt) { return d.get(K(key), dflt); }
inline Val dat(const Dict& d, const char* key) { return d.at(K(key)); }                   // d[key]: KeyError
inline void dset(const Dict& d, const char* key, const Val& v) { d.set(K(key), v); }

[[noreturn]] inline void type_error(const char* what, const Val& v) {
    throw py::TypeError(py::str(what) + v.repr());
}

inline const Dict& dict_of(const Val& v, const char* what) {
    if (!holds<Dict>(v)) type_error(what, v);
    return as<Dict>(v);
}

inline py::str str_of(const Val& v, const char* what) {
    if (!holds<py::str>(v)) type_error(what, v);
    return as<py::str>(v);
}

inline std::optional<py::str> opt_str_of(const Val& v, const char* what) {
    if (val_none(v)) return std::nullopt;
    return str_of(v, what);
}

inline py::list<py::str> strs_of(const Val& v, const char* what) {
    if (!holds<Val::List>(v)) type_error(what, v);
    py::list<py::str> out;
    for (const Val& x : as<Val::List>(v)) out.append(str_of(x, what));
    return out;
}

inline int64_t int_of(const Val& v, const char* what) {
    if (val_none(v)) return 0;
    if (!holds<int64_t>(v)) type_error(what, v);
    return as<int64_t>(v);
}

// a Type (or None) held in a Val: AttributeError for anything else (the Python reads .kind of it)
inline TypeP type_in(const Val& v) {
    if (val_none(v)) return nullptr;
    if (holds<ir::TypeP>(v)) return as<ir::TypeP>(v);
    throw py::AttributeError(py::str("object has no attribute 'kind': ") + v.repr());
}

// `for x in v`: the items of a list (TypeError for anything else)
inline const Val::List& list_in(const Val& v) {
    if (!holds<Val::List>(v)) type_error("object is not iterable: ", v);
    return as<Val::List>(v);
}

// dict(v): a copy of a dict; an empty str or list is an empty dict; a longer str is a ValueError (its characters are not
// pairs) and any other value a TypeError
inline Dict dict_copy_of(const Val& v) {
    if (holds<Dict>(v)) return as<Dict>(v).copy();
    if (holds<py::str>(v)) {
        if (py::len(as<py::str>(v))) throw py::ValueError("dictionary update sequence element #0 has length 1; 2 is required");
        return Dict();
    }
    if (holds<Val::List>(v) && as<Val::List>(v).empty()) return Dict();
    type_error("cannot convert to a dict: ", v);
}

// dict(p, type=t): a copy of p with its 'type' set (a key already there keeps its place)
inline Dict with_type(const Val& p, const Val& t) {
    Dict q = dict_of(p, "dict() argument is not a mapping: ").copy();
    dset(q, "type", t);
    return q;
}

}  // namespace old_l2_detail
using namespace old_l2_detail;

// ---------------------------------------------------------------------------------------------- types
inline Val type_json(const TypeP& t) {
    if (!t) return Val();
    const py::str& k = t->kind;
    const Dict& info = t->info;
    Dict d;
    if (ir::PRIMS.contains(k) || k == K("void")) {
        dset(d, "kind", Val(k));
        if (dhas(info, "signed")) dset(d, "signed", dat(info, "signed"));
        if (dhas(info, "enc")) dset(d, "enc", dat(info, "enc"));      // a character, not a number (C#'s char beside ushort)
        return Val(d);
    }
    if (k == K("null") || k == K("any") || k == K("dynamic")) {
        dset(d, "kind", Val(k));
        return Val(d);
    }
    if (k == K("class")) {
        dset(d, "kind", Val("class"));
        dset(d, "name", t->name ? Val(*t->name) : Val());
        if (py::len(t->args)) {
            Val::List args;
            for (const TypeP& a : t->args) args.append(type_json(a));
            dset(d, "args", Val(args));
        }
        if (truthy(dget(info, "nullable"))) dset(d, "nullable", Val(true));
        for (const py::str& key : CLASS_FACTS_) {                    // (the Python front end's: kept as given, False too)
            if (info.contains(key)) d.set(key, info.at(key));
        }
        return Val(d);
    }
    if (k == K("typevar")) {
        dset(d, "kind", Val("typevar"));
        dset(d, "name", t->name ? Val(*t->name) : Val());
        return Val(d);
    }
    if (k == K("wildcard")) {
        dset(d, "kind", Val("wildcard"));
        if (t->bound) {
            dset(d, "bound", Val(std::get<0>(*t->bound)));
            dset(d, "type", type_json(std::get<1>(*t->bound)));
        }
        return Val(d);
    }
    if (k == K("array")) {
        dset(d, "kind", Val("array"));
        dset(d, "elem", type_json(t->elem));
        if (dhas(info, "count")) dset(d, "count", dat(info, "count"));
        if (dhas(info, "rank") && not_one(dat(info, "rank"))) dset(d, "rank", dat(info, "rank"));
        return Val(d);
    }
    if (k == K("ptr")) {
        dset(d, "kind", Val("ptr"));
        dset(d, "space", dget(info, "space", Val("flat")));
        dset(d, "to", type_json(t->elem));
        if (dhas(info, "seg")) dset(d, "seg", dat(info, "seg"));
        if (truthy(dget(info, "ref"))) dset(d, "ref", Val(true));
        return Val(d);
    }
    if (k == K("selector")) {
        dset(d, "kind", Val("selector"));
        if (dhas(info, "of")) dset(d, "of", dat(info, "of"));
        return Val(d);
    }
    if (k == K("fn")) {
        dset(d, "kind", Val("fn"));
        dset(d, "conv", dget(info, "conv", Val("cdecl")));
        Val::List params;
        if (dhas(info, "params")) {
            for (const Val& p : list_in(dat(info, "params"))) {
                Dict pd = dict_of(p, "dict() argument is not a mapping: ");
                params.append(Val(with_type(p, type_json(type_in(dat(pd, "type"))))));
            }
        }
        dset(d, "params", Val(params));
        Dict result;
        if (dhas(info, "result")) result = dict_copy_of(dat(info, "result"));
        else dset(result, "at", Val("none"));
        dset(d, "result", Val(result));
        if (dhas(result, "type")) dset(result, "type", type_json(type_in(dat(result, "type"))));
        static const char* const keys[] = {"distance", "pops", "loadds", "preserves", "variadic"};
        for (const char* key : keys) {
            if (dhas(info, key)) dset(d, key, dat(info, key));
        }
        return Val(d);
    }
    if (k == K("fnptr")) {
        dset(d, "kind", Val("fnptr"));
        dset(d, "space", dget(info, "space", Val("flat")));
        dset(d, "fn", type_json(type_in(dat(info, "fn"))));
        return Val(d);
    }
    if (k == K("record")) {
        dset(d, "kind", Val("record"));
        dset(d, "size", dat(info, "size"));
        Val::List fields;
        if (dhas(info, "fields")) {
            for (const Val& f : list_in(dat(info, "fields"))) {
                Dict fd = dict_of(f, "dict() argument is not a mapping: ");
                fields.append(Val(with_type(f, type_json(type_in(dat(fd, "type"))))));
            }
        }
        dset(d, "fields", Val(fields));
        if (t->name && py::len(*t->name)) dset(d, "name", Val(*t->name));
        static const char* const keys[] = {"role", "packed"};
        for (const char* key : keys) {
            if (dhas(info, key)) dset(d, key, dat(info, key));
        }
        return Val(d);
    }
    throw py::ValueError(py::str("no L2 form for type kind ") + py::repr(k));
}

inline TypeP type_from(const Val& dv) {
    if (val_none(dv)) return nullptr;
    const Dict& d = dict_of(dv, "L2 type is not a dict: ");
    Val kv = dat(d, "kind");
    if (!holds<py::str>(kv)) throw py::ValueError(py::str("no IR form for L2 type kind ") + kv.repr());
    const py::str& k = as<py::str>(kv);
    auto make = [](const py::str& kind, const std::optional<py::str>& name, const py::list<TypeP>& args, const TypeP& elem,
                   const std::optional<std::tuple<py::str, TypeP>>& bound, const Dict& info) {
        return std::make_shared<ir::Type>(kind, name, args, elem, bound, info);
    };
    py::list<TypeP> none_args;
    if (ir::PRIMS.contains(k) || k == K("void")) {
        Dict info;
        if (dhas(d, "signed")) dset(info, "signed", dat(d, "signed"));
        if (dhas(d, "enc")) dset(info, "enc", dat(d, "enc"));
        return make(k, std::nullopt, none_args, nullptr, std::nullopt, info);
    }
    if (k == K("null") || k == K("any") || k == K("dynamic")) return make(k, std::nullopt, none_args, nullptr, std::nullopt, Dict());
    if (k == K("class")) {
        Dict info;
        if (truthy(dget(d, "nullable"))) dset(info, "nullable", Val(true));
        for (const py::str& key : CLASS_FACTS_) {
            if (d.contains(key)) info.set(key, d.at(key));
        }
        std::optional<py::str> name = opt_str_of(dat(d, "name"), "class type name is not a str: ");
        py::list<TypeP> args;
        if (dhas(d, "args")) {
            for (const Val& a : list_in(dat(d, "args"))) args.append(type_from(a));
        }
        return make(K("class"), name, args, nullptr, std::nullopt, info);
    }
    if (k == K("typevar")) {
        return make(K("typevar"), opt_str_of(dat(d, "name"), "typevar name is not a str: "), none_args, nullptr, std::nullopt, Dict());
    }
    if (k == K("wildcard")) {
        std::optional<std::tuple<py::str, TypeP>> bound;
        if (dhas(d, "bound")) {
            py::str mode = str_of(dat(d, "bound"), "wildcard bound is not a str: ");
            bound = std::make_tuple(mode, type_from(dat(d, "type")));
        }
        return make(K("wildcard"), std::nullopt, none_args, nullptr, bound, Dict());
    }
    if (k == K("array")) {
        TypeP elem = type_from(dat(d, "elem"));
        Dict info;
        if (dhas(d, "count")) dset(info, "count", dat(d, "count"));
        if (dhas(d, "rank")) dset(info, "rank", dat(d, "rank"));
        return make(K("array"), std::nullopt, none_args, elem, std::nullopt, info);
    }
    if (k == K("ptr")) {
        Dict extra;
        static const char* const keys[] = {"seg", "ref"};
        for (const char* key : keys) {
            Val v = dget(d, key);
            if (!val_none(v) && !(holds<bool>(v) && !as<bool>(v))) dset(extra, key, v);
        }
        TypeP elem = type_from(dat(d, "to"));
        Dict info;
        dset(info, "space", dat(d, "space"));
        info.update(extra);
        return make(K("ptr"), std::nullopt, none_args, elem, std::nullopt, info);
    }
    if (k == K("selector")) {
        Dict info;
        if (dhas(d, "of")) dset(info, "of", dat(d, "of"));
        return make(K("selector"), std::nullopt, none_args, nullptr, std::nullopt, info);
    }
    if (k == K("fn")) {
        Dict info;
        static const char* const keys[] = {"conv", "distance", "pops", "loadds", "preserves", "variadic"};
        for (const char* key : keys) {
            if (dhas(d, key)) dset(info, key, dat(d, key));
        }
        Val::List params;
        for (const Val& p : list_in(dat(d, "params"))) {
            Dict pd = dict_of(p, "dict() argument is not a mapping: ");
            params.append(Val(with_type(p, Val(type_from(dat(pd, "type"))))));
        }
        dset(info, "params", Val(params));
        Dict res = dict_copy_of(dat(d, "result"));
        if (dhas(res, "type")) dset(res, "type", Val(type_from(dat(res, "type"))));
        dset(info, "result", Val(res));
        return make(K("fn"), std::nullopt, none_args, nullptr, std::nullopt, info);
    }
    if (k == K("fnptr")) {
        Dict info;
        dset(info, "space", dat(d, "space"));
        dset(info, "fn", Val(type_from(dat(d, "fn"))));
        return make(K("fnptr"), std::nullopt, none_args, nullptr, std::nullopt, info);
    }
    if (k == K("record")) {
        Dict info;
        static const char* const keys[] = {"size", "role", "packed"};
        for (const char* key : keys) {
            if (dhas(d, key)) dset(info, key, dat(d, key));
        }
        Val::List fields;
        for (const Val& f : list_in(dat(d, "fields"))) {
            Dict fd = dict_of(f, "dict() argument is not a mapping: ");
            fields.append(Val(with_type(f, Val(type_from(dat(fd, "type"))))));
        }
        dset(info, "fields", Val(fields));
        return make(K("record"), opt_str_of(dget(d, "name"), "record name is not a str: "), none_args, nullptr, std::nullopt, info);
    }
    throw py::ValueError(py::str("no IR form for L2 type kind ") + py::repr(k));
}

// ---------------------------------------------------------------------------------------------- symbols
inline Val sym_json(const SymP& s) {
    if (!s) return Val();
    Dict d;
    dset(d, "kind", Val(s->kind));
    dset(d, "name", Val(s->name));
    if (s->owner) dset(d, "owner", Val(*s->owner));
    if (s->static_) dset(d, "static", Val(true));
    if (s->type) dset(d, "type", type_json(s->type));
    if (py::len(s->params) || s->kind == K("method") || s->kind == K("ctor") || s->kind == K("function")) {
        Val::List ps;
        for (const TypeP& p : s->params) ps.append(type_json(p));
        dset(d, "params", Val(ps));
    }
    if (s->ret) dset(d, "ret", type_json(s->ret));
    if (s->varargs) dset(d, "varargs", Val(true));
    bool any_ref = false;
    for (const py::str& r : s->refs) any_ref = any_ref || py::len(r) != 0;
    if (any_ref) {
        Val::List rs;
        for (const py::str& r : s->refs) rs.append(Val(r));
        dset(d, "refs", Val(rs));
    }
    if (py::len(s->flags)) {
        Val::List fs;
        for (const py::str& f : s->flags) fs.append(Val(f));
        dset(d, "flags", Val(fs));
    }
    return Val(d);
}

inline SymP sym_from(const Val& dv) {
    if (val_none(dv)) return nullptr;
    const Dict& d = dict_of(dv, "L2 symbol is not a dict: ");
    py::str kind = str_of(dat(d, "kind"), "symbol kind is not a str: ");
    py::str name = str_of(dat(d, "name"), "symbol name is not a str: ");
    std::optional<py::str> owner = opt_str_of(dget(d, "owner"), "symbol owner is not a str: ");
    bool static_ = truthy(dget(d, "static", Val(false)));
    TypeP type = type_from(dget(d, "type"));
    py::list<TypeP> params;
    if (dhas(d, "params")) {
        for (const Val& p : list_in(dat(d, "params"))) params.append(type_from(p));
    }
    TypeP ret = type_from(dget(d, "ret"));
    bool varargs = truthy(dget(d, "varargs", Val(false)));
    py::list<py::str> refs, flags;
    if (dhas(d, "refs")) refs = strs_of(dat(d, "refs"), "symbol refs are not str: ");
    if (dhas(d, "flags")) flags = strs_of(dat(d, "flags"), "symbol flags are not str: ");
    return std::make_shared<ir::Sym>(kind, name, owner, static_, type, params, ret, varargs, refs, flags);
}

// ---------------------------------------------------------------------------------------------- nodes
namespace old_l2_detail {

// _value(v) of a Val: types in lists are made into their L2 form, JSON has no NaN/inf (a Lit keeps them
// as text), a dict is kept as it is
inline Val value_of(const Val& v) {
    if (holds<ir::TypeP>(v)) return type_json(as<ir::TypeP>(v));
    if (holds<Val::List>(v)) {
        Val::List out;
        for (const Val& x : as<Val::List>(v)) out.append(value_of(x));
        return Val(out);
    }
    if (holds<double>(v) && !std::isfinite(as<double>(v))) return Val(py::repr(as<double>(v)));
    return v;
}

// -- to_json: one overload per type a slot has. `keep`: an empty list is written (a slot of `fields`, or
// params / args); else it is left out.
inline void put(const Dict& d, const py::str& key, bool keep, const NodeP& v);
inline void put(const Dict& d, const py::str& key, bool keep, const TypeP& v) {
    (void)keep;
    if (v) d.set(key, type_json(v));
}
inline void put(const Dict& d, const py::str& key, bool keep, const SymP& v) {
    (void)keep;
    if (v) d.set(key, sym_json(v));
}
inline void put(const Dict& d, const py::str& key, bool, const py::str& v) { d.set(key, Val(v)); }
inline void put(const Dict& d, const py::str& key, bool, int64_t v) { d.set(key, Val(v)); }
inline void put(const Dict& d, const py::str& key, bool, bool v) { d.set(key, Val(v)); }
inline void put(const Dict& d, const py::str& key, bool, const Dict& v) { d.set(key, Val(v)); }
inline void put(const Dict& d, const py::str& key, bool keep, const Val& v) {
    if (val_none(v)) return;
    if (!keep && holds<Val::List>(v) && as<Val::List>(v).empty()) return;
    d.set(key, value_of(v));
}
inline void put(const Dict& d, const py::str& key, bool keep, const py::list<NodeP>& v);
inline void put(const Dict& d, const py::str& key, bool keep, const py::list<TypeP>& v) {
    if (!keep && v.empty()) return;
    Val::List out;
    for (const TypeP& x : v) out.append(type_json(x));
    d.set(key, Val(out));
}
inline void put(const Dict& d, const py::str& key, bool keep, const std::variant<py::list<TypeP>, py::list<Val>>& v);
inline void put(const Dict& d, const py::str& key, bool keep, const py::list<py::str>& v) {
    if (!keep && v.empty()) return;
    Val::List out;
    for (const py::str& x : v) out.append(Val(x));
    d.set(key, Val(out));
}
template <class T> void put(const Dict& d, const py::str& key, bool keep, const std::optional<T>& v) {
    if (v) put(d, key, keep, *v);
}
inline void put(const Dict& d, const py::str& key, bool keep, const std::variant<py::list<TypeP>, py::list<Val>>& v) {
    if (std::holds_alternative<py::list<TypeP>>(v)) {
        put(d, key, keep, std::get<py::list<TypeP>>(v));
        return;
    }
    const py::list<Val>& l = std::get<py::list<Val>>(v);
    if (!keep && l.empty()) return;
    d.set(key, value_of(Val(l)));
}
inline void put(const Dict& d, const py::str& key, bool keep, const ir::NodeOrList& v) {
    if (std::holds_alternative<NodeP>(v)) put(d, key, keep, std::get<NodeP>(v));
    else put(d, key, keep, std::get<py::list<NodeP>>(v));
}

inline Dict to_json_impl(const NodeP& node);

inline void put(const Dict& d, const py::str& key, bool, const NodeP& v) {
    if (v) d.set(key, Val(to_json_impl(v)));
}
inline void put(const Dict& d, const py::str& key, bool keep, const py::list<NodeP>& v) {
    if (!keep && v.empty()) return;
    Val::List out;
    for (const NodeP& x : v) out.append(x ? Val(to_json_impl(x)) : Val());
    d.set(key, Val(out));
}

template <class D> Dict to_json_as(const NodeP& node) {
    D& n = static_cast<D&>(*node);
    Dict d;
    dset(d, "node", Val(node->class_name()));
    if (n.line) dset(d, "line", Val(n.line));
    if (n.col) dset(d, "col", Val(n.col));
    if (n.origin) dset(d, "origin", Val(*n.origin));
    n.each([&](const char* name, auto& v, int rank) {
        py::str key(name);
        bool keep = rank != 0 || key == K("params") || key == K("args");
        put(d, key, keep, v);
    });
    return d;
}

// -- from_json: the value of a slot, in the kind the slot is typed with
inline NodeP node_in(const Val& v);

inline void assign(NodeP& m, const Val& v) {
    if (val_none(v)) {
        m = nullptr;
        return;
    }
    m = node_in(v);
}
inline void assign(TypeP& m, const Val& v) { m = type_from(v); }
inline void assign(SymP& m, const Val& v) { m = sym_from(v); }
inline void assign(py::str& m, const Val& v) { m = str_of(v, "slot is not a str: "); }
inline void assign(ir::OptStr& m, const Val& v) { m = opt_str_of(v, "slot is not a str: "); }
inline void assign(ir::OptInt& m, const Val& v) {
    if (val_none(v)) m.reset();
    else m = int_of(v, "slot is not an int: ");
}
inline void assign(ir::OptBool& m, const Val& v) {
    if (val_none(v)) m.reset();
    else if (holds<bool>(v)) m = as<bool>(v);
    else type_error("slot is not a bool: ", v);
}
inline void assign(ir::Strs& m, const Val& v) {
    if (val_none(v)) m.reset();
    else m = strs_of(v, "slot is not a list of str: ");
}
inline void assign(ir::Nodes& m, const Val& v) {
    if (val_none(v)) {
        m.reset();
        return;
    }
    py::list<NodeP> out;
    for (const Val& x : list_in(v)) out.append(val_none(x) ? nullptr : node_in(x));   // (None stays None)
    m = out;
}
inline void assign(ir::Types& m, const Val& v) {
    if (val_none(v)) {
        m.reset();
        return;
    }
    py::list<TypeP> out;
    for (const Val& x : list_in(v)) out.append(type_from(x));
    m = out;
}
inline void assign(Val& m, const Val& v) { m = v; }
inline void assign(std::optional<Dict>& m, const Val& v) {
    if (val_none(v)) m.reset();
    else m = dict_of(v, "slot is not a dict: ");
}
inline void assign(ir::NodeOrList& m, const Val& v) {
    if (holds<Val::List>(v)) {
        py::list<NodeP> out;
        for (const Val& x : as<Val::List>(v)) out.append(val_none(x) ? nullptr : node_in(x));
        m = out;
    } else {
        NodeP n;
        assign(n, v);
        m = n;
    }
}

// Call.typeargs: a list of type dicts is Types; any other list (a struct format, a count) is kept as the values it is
inline void assign(ir::TypeArgs& m, const Val& v) {
    if (val_none(v)) {
        m.reset();
        return;
    }
    const Val::List& items = list_in(v);
    bool all_types = true;
    for (const Val& x : items) all_types = all_types && holds<Dict>(x) && as<Dict>(x).contains(K("kind"));
    if (all_types) {
        py::list<TypeP> out;
        for (const Val& x : items) out.append(type_from(x));
        m = out;
    } else {
        m = items.copy();
    }
}

template <class D> NodeP from_json_as(const Dict& d) {
    auto n = std::make_shared<D>();
    n->line = int_of(dget(d, "line", Val(0)), "line is not an int: ");
    n->col = int_of(dget(d, "col", Val(0)), "col is not an int: ");
    Val origin = dget(d, "origin");
    if (!val_none(origin)) n->origin = dict_of(origin, "origin is not a dict: ");
    n->each([&](const char* name, auto& slot, int) {
        py::str key(name);
        if (!d.contains(key)) return;
        assign(slot, d.at(key));
    });
    return n;
}

#define L2_NODE_KINDS(X)                                                                                              \
    X(Module) X(Unit) X(ClassDecl) X(FieldDecl) X(MethodDecl) X(Param) X(EnumConst) X(Property) X(TypeParam)         \
    X(Block) X(Local) X(ExprStmt) X(If) X(While) X(DoWhile) X(For) X(ForEach) X(Switch) X(Case) X(Break) X(Continue) \
    X(Return) X(Throw) X(Try) X(Catch) X(Labeled) X(Goto) X(MachineFragment) X(Sync) X(Empty) X(Comment)            \
    X(LocalClass) X(Assert) X(CtorCall) X(Yield) X(Lit) X(Name) X(Select) X(TypeName) X(This) X(Super) X(Call)       \
    X(New) X(NewArray) X(Index) X(Length) X(Unary) X(Binary) X(Concat) X(Assign) X(Cond) X(Cast) X(InstanceOf)      \
    X(ClassLit) X(Lambda) X(MethodRef) X(SwitchExpr) X(AddressOf) X(FarAddress) X(Piece) X(Join) X(Paren)           \
    X(Unsupported)

// _NODES: the class named in a document -> how to write it and how to read it
struct NodeKind {
    Dict (*to)(const NodeP&);
    NodeP (*from)(const Dict&);
};
inline const std::map<std::string, NodeKind>& node_kinds() {
    static const std::map<std::string, NodeKind> table = {
#define L2_ENTRY(C) {#C, {&to_json_as<ir::C>, &from_json_as<ir::C>}},
        L2_NODE_KINDS(L2_ENTRY)
#undef L2_ENTRY
    };
    return table;
}

inline Dict to_json_impl(const NodeP& node) {
    if (!node) throw py::AttributeError("'NoneType' object has no attribute 'line'");
    auto it = node_kinds().find(node->class_name().utf8());
    if (it == node_kinds().end()) throw py::KeyError(py::repr(node->class_name()));
    return it->second.to(node);
}

inline NodeP node_in(const Val& v) {
    if (!holds<Dict>(v) || !as<Dict>(v).contains(K("node")))
        throw py::TypeError(py::str("a node slot holds a document without 'node': ") + v.repr());
    return from_json(as<Dict>(v));
}

}  // namespace old_l2_detail
using namespace old_l2_detail;

inline Dict to_json(const NodeP& node) { return to_json_impl(node); }

inline NodeP from_json(const Dict& d) {
    Val name = dat(d, "node");
    if (!holds<py::str>(name)) throw py::KeyError(name.repr());
    auto it = node_kinds().find(as<py::str>(name).utf8());
    if (it == node_kinds().end()) throw py::KeyError(py::repr(as<py::str>(name)));
    return it->second.from(d);
}

inline NodeP from_json(const Val& d) {
    return from_json(dict_of(d, "an L2 document is not a dict: "));
}
}  // namespace codebreaker::translate::l2
