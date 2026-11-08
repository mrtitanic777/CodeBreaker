// codebreaker/translate/frontends/python/methods.cpp -- typed C++ from
// codebreaker.translate.frontends.python.methods.

#include "codebreaker/translate/frontends/python/methods.hpp"

#include <algorithm>

#include "codebreaker/translate/frontends/python/structfmt.hpp"

namespace codebreaker::translate::frontends::python::methods {

namespace A = pyfront;
namespace P = pytypes;
using program::Big;
using program::Bound;
using program::k_in;
using program::k_is;
using program::lit_eq;
using program::Program;
using program::str_starts;
using program::Tup;
using program::TupItem;

namespace {

// ---------------------------------------------------------------------------------------- helpers
Tup rt(const char* kind) { return Tup(Str(kind)); }
Tup rt(const char* kind, std::initializer_list<TupItem> items) { return Tup(Str(kind), std::vector<TupItem>(items)); }
TupItem rs(const Str& s) { return TupItem(s); }

Str i2s(int64_t v) { return py::to_str(v); }
Big bmax(const Big& a, const Big& b) { return a > b ? a : b; }
Big bmin(const Big& a, const Big& b) { return a < b ? a : b; }

T t_site(const Str& kind, int64_t site) { return T(kind, {}, std::nullopt, std::nullopt, std::nullopt, site); }

// an argument that the Python reads through without checking it is not None (AttributeError if it is)
const T& dv(const std::optional<T>& o) {
    if (!o) {
        throw py::AttributeError("'NoneType' object has no attribute 'kind'");
    }
    return *o;
}

const T& arg_at(const Args& args, int64_t i) {
    int64_t n = static_cast<int64_t>(args.size());
    if (i < -n || i >= n) {
        throw py::IndexError("list index out of range");
    }
    return dv(args[static_cast<size_t>(i < 0 ? i + n : i)]);
}

bool kw_has(const Kw& kw, const char* k) { return kw.contains(std::optional<Str>(Str(k))); }
const std::optional<T>& kw_at(const Kw& kw, const Str& k) { return kw.at(std::optional<Str>(k)); }

std::optional<Str> str_of(const A::Expr* n) { return program::str_const(n); }
std::optional<Big> intc(const A::Expr* n) { return program::int_const(n); }

Str opt_repr(const std::optional<Str>& s) { return s ? program::repr_str(*s) : Str("None"); }

TR issue(Eval& ev, const A::Node* n, const char* cls, const Str& detail) { return TR(ev.issue(n, Str(cls), detail)); }

// the element of a list/set type
T elem_of(Eval& ev, const T& t) { return ev.p.elem[static_cast<size_t>(ev.p.find(*t.site))]; }

const A::Expr* kw_value_node(const A::Call* n, const char* name) {
    for (const A::keyword* k : n->keywords) {
        if (k->has_arg && k->arg == name) {
            return k->value;
        }
    }
    return nullptr;
}

// _kind_ok(t, want): `want` is kinds joined by '|', a trailing '?' meaning the argument may be missing
bool kind_ok(const std::optional<T>& t, const char* want) {
    if (!t) {
        return false;
    }
    if (k_in(*t, {"bottom", "top"})) {
        return true;
    }
    Str k = t->kind;
    if (lit_eq(k, "bool")) {
        k = Str("int");
    }
    std::string w = want;
    while (!w.empty() && w.back() == '?') {
        w.pop_back();
    }
    size_t b = 0;
    for (;;) {
        size_t bar = w.find('|', b);
        std::string one = w.substr(b, bar == std::string::npos ? std::string::npos : bar - b);
        if (lit_eq(k, one.c_str()) || (one == "bytes" && lit_eq(k, "bytearray"))) {
            return true;
        }
        if (bar == std::string::npos) {
            break;
        }
        b = bar + 1;
    }
    return false;
}

bool ends_q(const std::string& w) { return !w.empty() && w.back() == '?'; }

// ---------------------------------------------------------------------------------------- _SIMPLE
struct Simple {
    std::vector<std::string> sig;
    T res;
};

const std::unordered_map<Str, Simple, py::str_hash>& SIMPLE() {
    static const std::unordered_map<Str, Simple, py::str_hash> table = [] {
        std::unordered_map<Str, Simple, py::str_hash> t;
        const T S = P::STR(), B = P::BOOL(), I = P::SIZE();
        (void)I;
        auto add = [&](const char* name, std::vector<std::string> sig, const T& res) { t.insert_or_assign(Str(name), Simple{std::move(sig), res}); };
        add("str.upper", {}, S); add("str.lower", {}, S); add("str.title", {}, S); add("str.capitalize", {}, S);
        add("str.strip", {"str?"}, S); add("str.lstrip", {"str?"}, S); add("str.rstrip", {"str?"}, S);
        add("str.startswith", {"str|tuple"}, B); add("str.endswith", {"str|tuple"}, B);
        add("str.find", {"str", "int?", "int?"}, P::FIND()); add("str.rfind", {"str", "int?", "int?"}, P::FIND());
        add("str.index", {"str"}, P::SIZE()); add("str.rindex", {"str"}, P::SIZE()); add("str.count", {"str"}, P::SIZE());
        add("str.replace", {"str", "str"}, S); add("str.isdigit", {}, B); add("str.isalpha", {}, B);
        add("str.isalnum", {}, B); add("str.isspace", {}, B); add("str.isupper", {}, B); add("str.islower", {}, B);
        add("str.isidentifier", {}, B); add("str.ljust", {"int", "str?"}, S); add("str.rjust", {"int", "str?"}, S);
        add("str.center", {"int", "str?"}, S); add("str.zfill", {"int"}, S);
        add("str.removeprefix", {"str"}, S); add("str.removesuffix", {"str"}, S);
        add("str.encode", {"str?", "str?"}, P::BYTES()); add("str.isprintable", {}, B); add("str.isascii", {}, B);
        add("bytes.upper", {}, P::BYTES()); add("bytes.lower", {}, P::BYTES());
        add("bytes.strip", {"bytes?"}, P::BYTES()); add("bytes.lstrip", {"bytes?"}, P::BYTES());
        add("bytes.rstrip", {"bytes?"}, P::BYTES());
        add("bytes.startswith", {"bytes|tuple"}, B); add("bytes.endswith", {"bytes|tuple"}, B);
        add("bytes.find", {"bytes|int", "int?", "int?"}, P::FIND()); add("bytes.rfind", {"bytes|int", "int?", "int?"}, P::FIND());
        add("bytes.index", {"bytes|int", "int?", "int?"}, P::SIZE()); add("bytes.count", {"bytes|int"}, P::SIZE());
        add("bytes.replace", {"bytes", "bytes"}, P::BYTES()); add("bytes.isalnum", {}, B); add("bytes.isalpha", {}, B);
        add("bytes.isdigit", {}, B); add("bytes.isspace", {}, B); add("bytes.isupper", {}, B); add("bytes.islower", {}, B);
        add("bytes.hex", {}, S); add("bytes.ljust", {"int", "bytes?"}, P::BYTES()); add("bytes.rjust", {"int", "bytes?"}, P::BYTES());
        add("bytes.decode", {"str?", "str?"}, S);
        return t;
    }();
    return table;
}

const char* const ENCODINGS[] = {"latin1", "latin-1", "iso-8859-1", "ascii", "utf-8", "utf8", "cp437", "cp1252"};
const char* const ERRORS[] = {"strict", "replace", "ignore"};

// _simple: the result type, or the refusal
T simple(Eval& ev, const A::Call* n, const Str& name, const Args& args, const Kw& kw) {
    const Simple& sg = SIMPLE().at(name);
    bool enc_form = lit_eq(name, "bytes.decode") || lit_eq(name, "str.encode");
    if (kw.size() != 0) {
        bool ok = enc_form;
        if (ok) {
            for (const std::optional<Str>& k : kw) {
                if (!(k && (lit_eq(*k, "errors") || lit_eq(*k, "encoding")))) {
                    ok = false;
                }
            }
        }
        if (!ok) {
            return ev.issue(n, Str("syntax"), Str("keyword argument to ") + name);
        }
    }
    if (args.size() > sg.sig.size()) {
        return ev.issue(n, Str("conflict"), Str("too many arguments to ") + name);
    }
    for (size_t i = 0; i < sg.sig.size(); ++i) {
        const std::string& w = sg.sig[i];
        if (i >= args.size()) {
            if (!ends_q(w)) {
                return ev.issue(n, Str("conflict"), Str("missing argument to ") + name);
            }
            continue;
        }
        if (!kind_ok(args[i], w.c_str())) {
            return ev.issue(n, Str("conflict"), name + Str(" of a ") + (args[i] ? args[i]->kind : Str("lambda")));
        }
    }
    if (enc_form) {
        std::vector<std::optional<Str>> consts;
        for (const A::Expr* a : n->args) {
            consts.push_back(str_of(a));
        }
        bool has_encoding_kw = false;
        for (const A::keyword* k : n->keywords) {
            consts.push_back(str_of(k->value));
            if (k->has_arg && k->arg == "encoding") {
                has_encoding_kw = true;
            }
        }
        std::optional<Str> enc = (!n->args.empty() || has_encoding_kw) ? consts[0] : std::optional<Str>(Str("utf-8"));
        bool enc_ok = false;
        if (enc) {
            Str low = enc->lower();
            for (const char* e : ENCODINGS) {
                if (lit_eq(low, e)) {
                    enc_ok = true;
                }
            }
        }
        if (!enc_ok) {
            return ev.issue(n, Str("stdlib"), name + Str("(") + opt_repr(enc) + Str(")"));
        }
        bool bad = false;
        for (size_t i = 1; i < consts.size(); ++i) {
            bool in = false;
            if (consts[i]) {
                for (const char* e : ERRORS) {
                    if (lit_eq(*consts[i], e)) {
                        in = true;
                    }
                }
            }
            if (!in) {
                bad = true;
            }
        }
        if (bad) {
            Str lst = Str("[");
            for (size_t i = 1; i < consts.size(); ++i) {
                if (i > 1) {
                    lst += Str(", ");
                }
                lst += opt_repr(consts[i]);
            }
            lst += Str("]");
            return ev.issue(n, Str("stdlib"), name + Str(" errors=") + lst);
        }
    }
    if ((str_starts(name, "str.startswith") || str_starts(name, "str.endswith") || lit_eq(name, "bytes.startswith") ||
         lit_eq(name, "bytes.endswith")) &&
        !args.empty() && args[0] && k_is(*args[0], "tuple")) {
        const char* want = str_starts(name, "str.") ? "str" : "bytes";
        for (const T& a : args[0]->args) {
            if (!kind_ok(a, want)) {
                return ev.issue(n, Str("conflict"), name + Str(" of a tuple holding ") + a.kind);
            }
        }
    }
    return sg.res;
}

// _sort_key
TR sort_key(Eval& ev, const A::Call* n, const T& e, const Kw& kw, const State& state, const Str& name, const T& result) {
    for (const std::optional<Str>& k : kw) {
        if (!(k && (lit_eq(*k, "key") || lit_eq(*k, "reverse")))) {
            return issue(ev, n, "syntax", name + Str("(") + (k ? *k : Str("None")) + Str("=)"));
        }
    }
    const A::Expr* key = kw_value_node(n, "key");
    T kt = e;
    if (key != nullptr) {
        kt = ev.lambda_type(key, {e}, state);
    }
    if (!k_in(kt, {"int", "str", "float", "bytes", "tuple", "bottom", "top", "bool"})) {
        return issue(ev, n, "conflict", Str("sorting by a ") + kt.kind);
    }
    if (lit_eq(name, "sorted")) {
        return TR(result, rt("builtin", {rs(Str("sorted"))}));
    }
    return TR(result, rt("bmethod", {rs(name)}));
}

// _lazy: a lambda or generator argument where this call does not take one
std::optional<TR> lazy(Eval& ev, const A::Call* n, const Args& args, const Kw& kw, bool ok) {
    if (!ok) {
        bool any = false;
        for (const std::optional<T>& a : args) {
            if (!a) {
                any = true;
            }
        }
        for (const std::optional<Str>& k : kw) {
            if (!kw.at(k)) {
                any = true;
            }
        }
        if (any) {
            return issue(ev, n, "dynamic", Str("a lambda or generator as an argument"));
        }
    }
    return std::nullopt;
}

bool lazy_ok(const Str& attr) {
    for (const char* w : {"join", "extend", "update", "sort"}) {
        if (lit_eq(attr, w)) {
            return true;
        }
    }
    return false;
}

bool lazy_builtin(const Str& name) {
    for (const char* w : {"sorted", "min", "max", "sum", "any", "all", "list", "set", "enumerate"}) {
        if (lit_eq(name, w)) {
            return true;
        }
    }
    return false;
}

// the element type of the first argument of join/extend/update-style calls: a generator argument or an iterable
std::pair<T, bool> first_iteration(Eval& ev, const A::Call* n, const std::optional<T>& a, const State& state,
                                   bool missing_is_bottom, std::optional<Big>* cnt = nullptr) {
    if (!a && !n->args.empty() && n->args[0]->kind == A::ExprKind::GeneratorExp) {
        auto r = ev.gen_elem(A::cast<A::GeneratorExp>(n->args[0]), state);
        return {r.first, true};
    }
    if (a) {
        auto r = ev.iteration(n->args[0], *a, state);
        if (cnt) {
            *cnt = r.second;
        }
        return {r.first, true};
    }
    (void)missing_is_bottom;
    return {P::BOTTOM(), false};
}

}  // namespace

// ---------------------------------------------------------------------------------------- call
TR call(Eval& ev, const A::Call* n, const T& recv, const Str& attr, const Args& args, const Kw& kw, const State& state) {
    if (auto bad = lazy(ev, n, args, kw, lazy_ok(attr))) {
        return *bad;
    }
    Str k = recv.kind;
    if (lit_eq(k, "bool")) {
        k = Str("int");
    }
    Str name = ((lit_eq(k, "bytearray") && SIMPLE().count(Str("bytes.") + attr)) ? Str("bytes") : k) + Str(".") + attr;
    if (SIMPLE().count(name)) {
        T t = simple(ev, n, name, args, kw);
        if (lit_eq(k, "bytearray") && k_is(t, "bytes") && !lit_eq(attr, "decode") && !lit_eq(attr, "hex")) {
            t = T(PS("bytearray"));
        }
        return TR(t, rt("bmethod", {rs(name)}));
    }
    Program& pr = ev.p;
    if (lit_eq(k, "str")) {
        if (lit_eq(attr, "split")) {
            if (args.size() > 2 || (!args.empty() && !kind_ok(args[0], "str|none")) || kw.size() != 0) {
                return issue(ev, n, "syntax", Str("str.") + attr + Str(" arguments"));
            }
            int64_t s = pr.new_site(n, Str("list"), ev.where(n));
            pr.grow(pr.elem, s, P::STR());
            return TR(t_site(PS("list"), s), rt("bmethod", {rs(Str("str.") + attr)}));
        }
        if (lit_eq(attr, "splitlines")) {
            int64_t s = pr.new_site(n, Str("list"), ev.where(n));
            pr.grow(pr.elem, s, P::STR());
            return TR(t_site(PS("list"), s), rt("bmethod", {rs(Str("str.splitlines"))}));
        }
        if (lit_eq(attr, "join")) {
            std::optional<T> a = args.empty() ? std::nullopt : args[0];
            if (!a && !(!n->args.empty() && n->args[0]->kind == A::ExprKind::GeneratorExp)) {
                return issue(ev, n, "conflict", Str("str.join of nothing"));
            }
            T e = first_iteration(ev, n, a, state, false).first;
            if (!k_in(e, {"str", "bottom", "top"})) {
                return issue(ev, n, "conflict", Str("str.join of ") + e.kind + Str(" items"));
            }
            return TR(P::STR(), rt("bmethod", {rs(Str("str.join"))}));
        }
        if (lit_eq(attr, "partition") || lit_eq(attr, "rpartition")) {
            return TR(P::tuple_of({P::STR(), P::STR(), P::STR()}), rt("bmethod", {rs(Str("str.") + attr)}));
        }
        if (lit_eq(attr, "format")) {
            return issue(ev, n, "syntax", Str("str.format"));
        }
    }
    if (lit_eq(k, "bytes") || lit_eq(k, "bytearray")) {
        if (lit_eq(attr, "join")) {
            std::optional<T> a = args.empty() ? std::nullopt : args[0];
            if (!a && !(!n->args.empty() && n->args[0]->kind == A::ExprKind::GeneratorExp)) {
                return issue(ev, n, "conflict", Str("bytes.join of nothing"));
            }
            T e = first_iteration(ev, n, a, state, false).first;
            if (!k_in(e, {"bytes", "bytearray", "bottom", "top"})) {
                return issue(ev, n, "conflict", Str("bytes.join of ") + e.kind + Str(" items"));
            }
            return TR(T(k), rt("bmethod", {rs(Str("bytes.join"))}));
        }
        if (lit_eq(attr, "split")) {
            int64_t s = pr.new_site(n, Str("list"), ev.where(n));
            pr.grow(pr.elem, s, T(k));
            return TR(t_site(PS("list"), s), rt("bmethod", {rs(Str("bytes.split"))}));
        }
        if (lit_eq(k, "bytearray") && lit_eq(attr, "append")) {
            if (args.empty() || !kind_ok(args[0], "int")) {
                return issue(ev, n, "conflict", Str("bytearray.append"));
            }
            return TR(P::NONE(), rt("bmethod", {rs(Str("bytearray.append"))}));
        }
        if (lit_eq(k, "bytearray") && lit_eq(attr, "extend")) {
            if (args.empty() || !args[0] || !k_in(*args[0], {"bytes", "bytearray", "bottom", "top"})) {
                return issue(ev, n, "syntax",
                             Str("bytearray.extend of a ") + (!args.empty() && args[0] ? args[0]->kind : Str("?")));
            }
            return TR(P::NONE(), rt("bmethod", {rs(Str("bytearray.extend"))}));
        }
    }
    if (lit_eq(k, "list")) {
        int64_t s = *recv.site;
        T e = elem_of(ev, recv);
        if (lit_eq(attr, "append")) {
            if (args.size() != 1) {
                return issue(ev, n, "conflict", Str("list.append arity"));
            }
            T item = ev.fit(n, dv(args[0]), Str("list item"));
            pr.grow(pr.elem, s, item);
            return TR(P::NONE(), rt("bmethod", {rs(Str("list.append"))}));
        }
        if (lit_eq(attr, "extend")) {
            std::optional<T> a = args.empty() ? std::nullopt : args[0];
            T et = P::BOTTOM();
            if (!a && !n->args.empty() && n->args[0]->kind == A::ExprKind::GeneratorExp) {
                et = ev.gen_elem(A::cast<A::GeneratorExp>(n->args[0]), state).first;
            } else if (a) {
                et = ev.iteration(n->args.at(0), *a, state).first;
            }
            pr.grow(pr.elem, s, et);
            return TR(P::NONE(), rt("bmethod", {rs(Str("list.extend"))}));
        }
        if (lit_eq(attr, "insert")) {
            pr.grow(pr.elem, s, arg_at(args, 1));
            return TR(P::NONE(), rt("bmethod", {rs(Str("list.insert"))}));
        }
        if (lit_eq(attr, "pop")) {
            return TR(e, rt("bmethod", {rs(Str("list.pop"))}));
        }
        if (lit_eq(attr, "index") || lit_eq(attr, "count")) {
            return TR(P::SIZE(), rt("bmethod", {rs(Str("list.") + attr)}));
        }
        if (lit_eq(attr, "remove")) {
            return TR(P::NONE(), rt("bmethod", {rs(Str("list.remove"))}));
        }
        if (lit_eq(attr, "reverse") || lit_eq(attr, "clear")) {
            return TR(P::NONE(), rt("bmethod", {rs(Str("list.") + attr)}));
        }
        if (lit_eq(attr, "copy")) {
            int64_t s2 = pr.new_site(n, Str("list"), ev.where(n));
            pr.grow(pr.elem, s2, e);
            return TR(t_site(PS("list"), s2), rt("bmethod", {rs(Str("list.copy"))}));
        }
        if (lit_eq(attr, "sort")) {
            return sort_key(ev, n, e, kw, state, Str("list.sort"), P::NONE());
        }
    }
    if (lit_eq(k, "dict")) {
        int64_t s = pr.find(*recv.site);
        std::optional<Str> key = n->args.empty() ? std::nullopt : str_of(n->args[0]);
        if (lit_eq(attr, "get")) {
            T dflt = args.size() > 1 ? dv(args[1]) : P::NONE();
            if (key && !pr.dynamic.contains(s)) {
                py::dict<Str, T>& fs = pr.fields[static_cast<size_t>(s)];
                T ft = fs.contains(*key) ? fs.at(*key) : P::BOTTOM();
                T r = pr.join(ft, dflt);
                if (k_is(r, "top")) {
                    return issue(ev, n, "conflict", Str("dict.get default: ") + (r.why ? *r.why : Str()));
                }
                return TR(r, rt("bmethod", {rs(Str("record.get"))}));
            }
            if (!key) {
                pr.make_dynamic(s, Str("get of a computed key"));
            }
            if (!args.empty()) {
                pr.grow(pr.keys, s, dv(args[0]));
            }
            T r = pr.join(pr.elem[static_cast<size_t>(s)], dflt);
            if (k_is(r, "top")) {
                return issue(ev, n, "conflict", Str("dict.get default: ") + (r.why ? *r.why : Str()));
            }
            return TR(r, rt("bmethod", {rs(Str("dict.get"))}));
        }
        pr.make_dynamic(s, Str("dict.") + attr);
        if (lit_eq(attr, "setdefault")) {
            if (args.size() != 2) {
                return issue(ev, n, "conflict", Str("dict.setdefault arity"));
            }
            pr.grow(pr.keys, s, dv(args[0]));
            pr.grow(pr.elem, s, dv(args[1]));
            return TR(pr.elem[static_cast<size_t>(s)], rt("bmethod", {rs(Str("dict.setdefault"))}));
        }
        if (lit_eq(attr, "items")) {
            return TR(T(PS("iter"), {P::tuple_of({pr.keys[static_cast<size_t>(s)], pr.elem[static_cast<size_t>(s)]})}, P::MEM()),
                      rt("bmethod", {rs(Str("dict.items"))}));
        }
        if (lit_eq(attr, "keys")) {
            return TR(T(PS("iter"), {pr.keys[static_cast<size_t>(s)]}, P::MEM()), rt("bmethod", {rs(Str("dict.keys"))}));
        }
        if (lit_eq(attr, "values")) {
            return TR(T(PS("iter"), {pr.elem[static_cast<size_t>(s)]}, P::MEM()), rt("bmethod", {rs(Str("dict.values"))}));
        }
        if (lit_eq(attr, "pop")) {
            if (!args.empty()) {
                pr.grow(pr.keys, s, dv(args[0]));
            }
            T r = args.size() < 2 ? pr.elem[static_cast<size_t>(s)] : pr.join(pr.elem[static_cast<size_t>(s)], dv(args[1]));
            return TR(r, rt("bmethod", {rs(Str("dict.pop"))}));
        }
        if (lit_eq(attr, "update")) {
            if (args.size() == 1 && args[0] && k_is(*args[0], "dict")) {
                int64_t o = pr.find(*args[0]->site);
                pr.make_dynamic(o, Str("dict.update"));
                pr.grow(pr.keys, s, pr.keys[static_cast<size_t>(o)]);
                pr.grow(pr.elem, s, pr.elem[static_cast<size_t>(o)]);
                return TR(P::NONE(), rt("bmethod", {rs(Str("dict.update"))}));
            }
            return issue(ev, n, "syntax", Str("dict.update arguments"));
        }
        if (lit_eq(attr, "copy")) {
            int64_t s2 = pr.new_site(n, Str("dict"), ev.where(n));
            pr.make_dynamic(s2, Str("dict.copy"));
            pr.grow(pr.keys, s2, pr.keys[static_cast<size_t>(s)]);
            pr.grow(pr.elem, s2, pr.elem[static_cast<size_t>(s)]);
            return TR(t_site(PS("dict"), s2), rt("bmethod", {rs(Str("dict.copy"))}));
        }
        if (lit_eq(attr, "clear")) {
            return TR(P::NONE(), rt("bmethod", {rs(Str("dict.clear"))}));
        }
    }
    if (lit_eq(k, "set")) {
        int64_t s = *recv.site;
        if (lit_eq(attr, "add")) {
            pr.grow(pr.elem, s, arg_at(args, 0));
            return TR(P::NONE(), rt("bmethod", {rs(Str("set.add"))}));
        }
        if (lit_eq(attr, "discard") || lit_eq(attr, "remove")) {
            return TR(P::NONE(), rt("bmethod", {rs(Str("set.") + attr)}));
        }
        if (lit_eq(attr, "update")) {
            T et = P::BOTTOM();
            if (!args.empty() && args[0]) {
                et = ev.iteration(n->args.at(0), *args[0], state).first;
            }
            pr.grow(pr.elem, s, et);
            return TR(P::NONE(), rt("bmethod", {rs(Str("set.update"))}));
        }
        if (lit_eq(attr, "issubset") || lit_eq(attr, "issuperset") || lit_eq(attr, "isdisjoint")) {
            return TR(P::BOOL(), rt("bmethod", {rs(Str("set.") + attr)}));
        }
    }
    if (lit_eq(k, "int")) {
        if (lit_eq(attr, "bit_length")) {
            return TR(P::INT(Big(0), recv.fits_i64() ? Bound(Big(64)) : Bound()), rt("bmethod", {rs(Str("int.bit_length"))}));
        }
        if (lit_eq(attr, "to_bytes")) {
            return TR(P::BYTES(), rt("bmethod", {rs(Str("int.to_bytes"))}));
        }
    }
    if (lit_eq(k, "fn") && str_starts(*recv.name, "builtin:")) {
        Str full = recv.name->slice_from(8) + Str(".") + attr;
        if (lit_eq(full, "int.from_bytes")) {
            return from_bytes(ev, n, args, kw);
        }
        if (lit_eq(full, "bytes.fromhex")) {
            return TR(P::BYTES(), rt("std", {rs(Str("bytes.fromhex"))}));
        }
        return issue(ev, n, "stdlib", full);
    }
    return issue(ev, n, "stdlib", k + Str(".") + attr);
}

TR from_bytes(Eval& ev, const A::Call* n, const Args& args, const Kw& kw) {
    if (args.size() != 1 && args.size() != 2) {
        return issue(ev, n, "conflict", Str("int.from_bytes arity"));
    }
    const std::optional<T>& b = args[0];
    std::optional<Str> order = n->args.size() > 1 ? str_of(n->args[1]) : str_of(kw_value_node(n, "byteorder"));
    if (n->args.size() < 2 && !kw_has(kw, "byteorder")) {
        order = Str("big");
    }
    if (!(order && (lit_eq(*order, "little") || lit_eq(*order, "big")))) {
        return issue(ev, n, "dynamic", Str("int.from_bytes byte order"));
    }
    const A::Expr* signed_ = kw_value_node(n, "signed");
    if (signed_ != nullptr) {
        auto* c = A::as<A::Constant>(signed_);
        if (!(c && (c->value.kind == A::ConstValue::Kind::True || c->value.kind == A::ConstValue::Kind::False))) {
            return issue(ev, n, "dynamic", Str("int.from_bytes signed=<computed>"));
        }
    }
    bool sg = signed_ != nullptr && A::cast<A::Constant>(signed_)->value.kind == A::ConstValue::Kind::True;
    if (!b || !k_in(*b, {"bytes", "bytearray", "bottom", "top"})) {
        return issue(ev, n, "conflict", Str("int.from_bytes of a ") + (b ? b->kind : Str("?")));
    }
    if (k_in(*b, {"bottom", "top"})) {
        return TR(*b);
    }
    if (!b->hi) {
        return TR(P::INT(sg ? Bound() : Bound(Big(0)), Bound()), rt("std", {rs(Str("int.from_bytes"))}));
    }
    Big bits = *b->hi * Big(8);
    // (a width past 128 bits is unbounded either way: any large count gives the same type)
    int64_t nb = bits > Big(1000000) ? 1000000 : bits.i64();
    return TR((sg && nb != 0) ? P::s(nb) : P::u(nb), rt("std", {rs(Str("int.from_bytes"))}));
}

// ---------------------------------------------------------------------------------------- builtins
namespace {

TR is_instance(Eval& ev, const A::Call* n, const std::optional<T>& t);

const std::vector<std::pair<const char*, std::vector<const char*>>>& ISTYPES() {
    static const std::vector<std::pair<const char*, std::vector<const char*>>> v = {
        {"int", {"int", "bool"}}, {"str", {"str"}}, {"bytes", {"bytes"}}, {"bytearray", {"bytearray"}},
        {"bool", {"bool"}},       {"float", {"float"}}, {"list", {"list"}}, {"dict", {"dict"}},
        {"tuple", {"tuple"}},     {"set", {"set"}}};
    return v;
}

const std::vector<const char*>* istype(const std::string& id) {
    for (const auto& kv : ISTYPES()) {
        if (id == kv.first) {
            return &kv.second;
        }
    }
    return nullptr;
}

bool in_kinds(const T& t, const std::vector<const char*>& ks) {
    for (const char* k : ks) {
        if (lit_eq(t.kind, k)) {
            return true;
        }
    }
    return false;
}

}  // namespace

TR builtin(Eval& ev, const A::Call* n, const Str& name, const Args& args, const Kw& kw, const State& state) {
    Program& pr = ev.p;
    if (infer::DYNAMIC_BUILTINS().count(name)) {
        return issue(ev, n, "dynamic", name);
    }
    bool sort_like = lit_eq(name, "sorted") || lit_eq(name, "min") || lit_eq(name, "max");
    if (auto bad = lazy(ev, n, args, sort_like ? Kw() : kw, lazy_builtin(name))) {
        return *bad;
    }
    const std::optional<T> a0 = args.empty() ? std::nullopt : args[0];
#define IS(x) lit_eq(name, x)
    if (IS("len")) {
        if (args.size() != 1 || !a0) {
            return issue(ev, n, "conflict", Str("len arity"));
        }
        const Str& k = a0->kind;
        if (lit_eq(k, "dict")) {
            pr.make_dynamic(*a0->site, Str("len"));
        }
        if (lit_eq(k, "tuple")) {
            return TR(P::const_int(Big(static_cast<int64_t>(a0->args.size()))), rt("builtin", {rs(Str("len"))}));
        }
        if (lit_eq(k, "bytes") || lit_eq(k, "bytearray")) {
            return TR(P::INT(Big(0), !a0->hi ? P::MEM() : *a0->hi), rt("builtin", {rs(Str("len"))}));
        }
        if (k_in(*a0, {"str", "list", "dict", "set", "bottom", "top"})) {
            return TR(P::SIZE(), rt("builtin", {rs(Str("len"))}));
        }
        return issue(ev, n, "conflict", Str("len of a ") + k);
    }
    if (IS("range")) {
        bool bad = kw.size() != 0 || args.size() < 1 || args.size() > 3;
        if (!bad) {
            for (const std::optional<T>& a : args) {
                if (!a || !k_in(*a, {"int", "bottom", "top", "bool"})) {
                    bad = true;
                }
            }
        }
        if (bad) {
            return issue(ev, n, "conflict", Str("range arguments"));
        }
        for (const std::optional<T>& a : args) {
            if (k_is(*a, "bottom")) {
                return TR(P::BOTTOM());
            }
        }
        for (const std::optional<T>& a : args) {
            if (k_is(*a, "top")) {
                return TR(*a);
            }
        }
        std::vector<T> as;
        for (const std::optional<T>& a : args) {
            as.push_back(k_is(*a, "bool") ? P::INT(Big(0), Big(1)) : *a);
        }
        T lo = as.size() == 1 ? P::const_int(Big(0)) : as[0];
        T hi = as.size() == 1 ? as[0] : as[1];
        std::optional<Big> step = as.size() == 3 ? intc(n->args[2]) : std::optional<Big>(Big(1));
        if (!step || *step == Big(0)) {
            return issue(ev, n, "syntax", Str("range with a computed step"));
        }
        T e = P::BOTTOM();
        if (*step > Big(0)) {
            e = P::INT(lo.lo, !hi.hi ? Bound() : Bound(*hi.hi - Big(1)));
        } else {
            e = P::INT(!hi.lo ? Bound() : Bound(*hi.lo + Big(1)), lo.hi);
        }
        return TR(T(PS("iter"), {e}, ev.range_count(n)), rt("builtin", {rs(Str("range"))}));
    }
    if (IS("enumerate")) {
        if (!a0) {
            return issue(ev, n, "conflict", Str("enumerate arguments"));
        }
        auto ec = ev.iteration(n->args[0], *a0, state);
        T e = ec.first;
        std::optional<Big> cnt = ec.second;
        T start = args.size() > 1 ? dv(args[1]) : (kw_has(kw, "start") ? dv(kw_at(kw, Str("start"))) : P::const_int(Big(0)));
        if (!k_is(start, "int")) {
            return issue(ev, n, "conflict", Str("enumerate start"));
        }
        T idx = P::iadd(start, P::INT(Big(0), (cnt ? *cnt : P::MEM()) - Big(1)));
        return TR(T(PS("iter"), {P::tuple_of({idx, e})}, cnt), rt("builtin", {rs(Str("enumerate"))}));
    }
    if (IS("zip")) {
        bool any_lazy = false;
        for (const std::optional<T>& a : args) {
            if (!a) {
                any_lazy = true;
            }
        }
        if (kw.size() != 0 || any_lazy) {
            return issue(ev, n, "syntax", Str("zip arguments"));
        }
        std::vector<T> es;
        for (size_t i = 0; i < n->args.size() && i < args.size(); ++i) {
            es.push_back(ev.iteration(n->args[i], *args[i], state).first);
        }
        return TR(T(PS("iter"), {P::tuple_of(es)}, P::MEM()), rt("builtin", {rs(Str("zip"))}));
    }
    if (IS("reversed")) {
        if (!a0) {
            return issue(ev, n, "conflict", Str("reversed arguments"));
        }
        if (k_is(*a0, "dict")) {
            return issue(ev, n, "syntax", Str("reversed of a dict"));
        }
        auto ec = ev.iteration(n->args[0], *a0, state);
        return TR(T(PS("iter"), {ec.first}, ec.second), rt("builtin", {rs(Str("reversed"))}));
    }
    if (IS("sorted")) {
        T e = P::BOTTOM();
        if (!a0 && !n->args.empty() && n->args[0]->kind == A::ExprKind::GeneratorExp) {
            e = ev.gen_elem(A::cast<A::GeneratorExp>(n->args[0]), state).first;
        } else if (!a0) {
            return issue(ev, n, "conflict", Str("sorted arguments"));
        } else {
            e = ev.iteration(n->args[0], *a0, state).first;
            if (k_is(*a0, "set")) {
                std::vector<infer::Issue> keep;
                for (const infer::Issue& i : ev.issues) {
                    if (!(lit_eq(i.detail, "set iteration order") && i.line == n->pos.lineno)) {
                        keep.push_back(i);
                    }
                }
                ev.issues = keep;
            }
        }
        int64_t s = pr.new_site(n, Str("list"), ev.where(n));
        pr.grow(pr.elem, s, e);
        return sort_key(ev, n, e, kw, state, Str("sorted"), t_site(PS("list"), s));
    }
    if (IS("min") || IS("max")) {
        for (const std::optional<Str>& k : kw) {
            if (!(k && (lit_eq(*k, "key") || lit_eq(*k, "default")))) {
                return issue(ev, n, "syntax", name + Str("(") + (k ? *k : Str("None")) + Str("=)"));
            }
        }
        if (kw_has(kw, "key")) {
            return issue(ev, n, "syntax", name + Str(" with a key"));
        }
        std::vector<std::optional<T>> ts;
        if (args.size() == 1) {
            T e = P::BOTTOM();
            if (!a0 && n->args[0]->kind == A::ExprKind::GeneratorExp) {
                e = ev.gen_elem(A::cast<A::GeneratorExp>(n->args[0]), state).first;
            } else {
                e = ev.iteration(n->args[0], dv(a0), state).first;
            }
            if (kw_has(kw, "default")) {
                e = pr.join(e, dv(kw_at(kw, Str("default"))));
            }
            ts.push_back(e);
        } else {
            ts = args;
        }
        T r = P::BOTTOM();
        for (const std::optional<T>& t : ts) {
            if (!t) {
                return issue(ev, n, "conflict", name + Str(" arguments"));
            }
            r = pr.join(r, k_is(*t, "bool") ? P::INT(Big(0), Big(1)) : *t);
        }
        if (k_is(r, "top")) {
            return issue(ev, n, "conflict", name + Str(" of mixed types: ") + (r.why ? *r.why : Str()));
        }
        if (!k_in(r, {"int", "str", "float", "bytes", "tuple", "bottom"})) {
            return issue(ev, n, "conflict", name + Str(" of ") + r.kind);
        }
        if (k_is(r, "int") && ts.size() > 1) {
            bool lo_none = false, all_lo_none = true, hi_none = false, all_hi_none = true;
            for (const std::optional<T>& t : ts) {
                if (!t->lo) lo_none = true; else all_lo_none = false;
                if (!t->hi) hi_none = true; else all_hi_none = false;
            }
            if (IS("min")) {
                Bound lo, hi;
                if (!lo_none) {
                    Big m = *ts[0]->lo;
                    for (const auto& t : ts) m = bmin(m, *t->lo);
                    lo = m;
                }
                if (!all_hi_none) {
                    std::optional<Big> m;
                    for (const auto& t : ts) if (t->hi) m = m ? bmin(*m, *t->hi) : *t->hi;
                    hi = m;
                }
                r = P::INT(lo, hi);
            } else {
                Bound lo, hi;
                if (!all_lo_none) {
                    std::optional<Big> m;
                    for (const auto& t : ts) if (t->lo) m = m ? bmax(*m, *t->lo) : *t->lo;
                    lo = m;
                }
                if (!hi_none) {
                    Big m = *ts[0]->hi;
                    for (const auto& t : ts) m = bmax(m, *t->hi);
                    hi = m;
                }
                r = P::INT(lo, hi);
            }
        }
        return TR(r, rt("builtin", {rs(name + (args.size() == 1 ? Str("_iter") : Str()))}));
    }
    if (IS("abs")) {
        if (!a0) {
            return issue(ev, n, "conflict", Str("abs"));
        }
        if (k_is(*a0, "int")) {
            if (a0->lo && *a0->lo >= Big(0)) {
                return TR(*a0, rt("builtin", {rs(Str("abs"))}));
            }
            Bound m = (!a0->lo || !a0->hi) ? Bound() : Bound(bmax(a0->lo->abs(), a0->hi->abs()));
            return TR(P::INT(Big(0), m), rt("builtin", {rs(Str("abs"))}));
        }
        if (k_is(*a0, "float")) {
            return TR(P::FLOAT(), rt("builtin", {rs(Str("abs"))}));
        }
        return k_in(*a0, {"bottom", "top"}) ? TR(*a0) : issue(ev, n, "conflict", Str("abs of ") + a0->kind);
    }
    if (IS("sum")) {
        T e = P::BOTTOM();
        std::optional<Big> cnt;
        if (!a0 && !n->args.empty() && n->args[0]->kind == A::ExprKind::GeneratorExp) {
            e = ev.gen_elem(A::cast<A::GeneratorExp>(n->args[0]), state).first;
            cnt = P::MEM();
        } else if (a0) {
            auto ec = ev.iteration(n->args[0], *a0, state);
            e = ec.first;
            cnt = ec.second;
        } else {
            return issue(ev, n, "conflict", Str("sum"));
        }
        if (k_is(e, "bool")) {
            e = P::INT(Big(0), Big(1));
        }
        if (k_is(e, "int")) {
            if (!cnt) {
                cnt = P::MEM();
            }
            T r = P::imul(e, P::INT(Big(0), *cnt));
            r = P::INT(r.lo ? Bound(bmin(*r.lo, Big(0))) : Bound(), r.hi ? Bound(bmax(*r.hi, Big(0))) : Bound());
            if (args.size() > 1) {
                r = P::iadd(r, dv(args[1]));
            }
            return TR(r, rt("builtin", {rs(Str("sum"))}));
        }
        if (k_is(e, "float")) {
            return TR(P::FLOAT(), rt("builtin", {rs(Str("sum"))}));
        }
        return k_in(e, {"bottom", "top"}) ? TR(e) : issue(ev, n, "conflict", Str("sum of ") + e.kind);
    }
    if (IS("any") || IS("all")) {
        if (!a0 && !n->args.empty() && n->args[0]->kind == A::ExprKind::GeneratorExp) {
            ev.gen_elem(A::cast<A::GeneratorExp>(n->args[0]), state);
        } else if (a0) {
            ev.iteration(n->args[0], *a0, state);
        }
        return TR(P::BOOL(), rt("builtin", {rs(name)}));
    }
    if (IS("int")) {
        if (kw.size() != 0) {
            return issue(ev, n, "syntax", Str("int keywords"));
        }
        if (args.empty()) {
            return TR(P::const_int(Big(0)), rt("builtin", {rs(Str("int"))}));
        }
        const T& a = dv(a0);
        if (k_in(a, {"int", "bool"})) {
            return TR(k_is(a, "int") ? a : P::INT(Big(0), Big(1)), rt("builtin", {rs(Str("int"))}));
        }
        if (k_is(a, "str")) {
            if (args.size() == 2 && !intc(n->args[1])) {
                return issue(ev, n, "dynamic", Str("int() with a computed base"));
            }
            return TR(P::INT(), rt("builtin", {rs(Str("int_str"))}));
        }
        if (k_is(a, "float")) {
            return TR(P::INT(), rt("builtin", {rs(Str("int_float"))}));
        }
        return k_in(a, {"bottom", "top"}) ? TR(a) : issue(ev, n, "conflict", Str("int() of ") + a.kind);
    }
    if (IS("float")) {
        if (a0 && k_is(*a0, "int")) {
            ev.exact_float(n, *a0);
        }
        return TR(P::FLOAT(), rt("builtin", {rs(Str("float"))}));
    }
    if (IS("str")) {
        if (args.empty()) {
            return TR(P::STR(), rt("builtin", {rs(Str("str"))}));
        }
        ev.check_str(n, dv(a0), false);
        return TR(P::STR(), rt("builtin", {rs(Str("str"))}));
    }
    if (IS("repr")) {
        ev.check_str(n, dv(a0), true);
        return TR(P::STR(), rt("builtin", {rs(Str("repr"))}));
    }
    if (IS("bool")) {
        return TR(P::BOOL(), rt("builtin", {rs(Str("bool"))}));
    }
    if (IS("bytes") || IS("bytearray")) {
        if (args.empty()) {
            return TR(T(name, {}, std::nullopt, Big(0)), rt("builtin", {rs(name)}));
        }
        const T& a = dv(a0);
        if (k_is(a, "int")) {
            return TR(T(name, {}, std::nullopt, a.hi), rt("builtin", {rs(name + Str("_zeros"))}));
        }
        if (k_in(a, {"bytes", "bytearray"})) {
            return TR(T(name, {}, std::nullopt, a.hi), rt("builtin", {rs(name + Str("_copy"))}));
        }
        if (k_is(a, "list")) {
            T e = elem_of(ev, a);
            if (!k_in(e, {"int", "bottom", "top"})) {
                return issue(ev, n, "conflict", name + Str(" of a list of ") + e.kind);
            }
            return TR(T(name), rt("builtin", {rs(name + Str("_list"))}));
        }
        if (k_is(a, "str")) {
            return issue(ev, n, "syntax", name + Str("(str, encoding)"));
        }
        if (k_is(a, "iter")) {
            return TR(T(name), rt("builtin", {rs(name + Str("_iter"))}));
        }
        return k_in(a, {"bottom", "top"}) ? TR(a) : issue(ev, n, "conflict", name + Str(" of ") + a.kind);
    }
    if (IS("list")) {
        int64_t s = pr.new_site(n, Str("list"), ev.where(n));
        if (!args.empty()) {
            T e = P::BOTTOM();
            if (!a0 && n->args[0]->kind == A::ExprKind::GeneratorExp) {
                e = ev.gen_elem(A::cast<A::GeneratorExp>(n->args[0]), state).first;
            } else {
                e = ev.iteration(n->args[0], dv(a0), state).first;
            }
            pr.grow(pr.elem, s, e);
        }
        return TR(t_site(PS("list"), s), rt("builtin", {rs(Str("list"))}));
    }
    if (IS("set")) {
        int64_t s = pr.new_site(n, Str("set"), ev.where(n));
        if (!args.empty()) {
            T e = P::BOTTOM();
            if (!a0 && n->args[0]->kind == A::ExprKind::GeneratorExp) {
                e = ev.gen_elem(A::cast<A::GeneratorExp>(n->args[0]), state).first;
            } else {
                e = ev.iteration(n->args[0], dv(a0), state).first;
                if (a0 && k_is(*a0, "set")) {
                    std::vector<infer::Issue> keep;
                    for (const infer::Issue& i : ev.issues) {
                        if (!(lit_eq(i.detail, "set iteration order") && i.line == n->pos.lineno)) {
                            keep.push_back(i);
                        }
                    }
                    ev.issues = keep;
                }
            }
            pr.grow(pr.elem, s, e);
        }
        return TR(t_site(PS("set"), s), rt("builtin", {rs(Str("set"))}));
    }
    if (IS("frozenset")) {
        return issue(ev, n, "syntax", Str("frozenset"));
    }
    if (IS("tuple")) {
        if (a0 && k_is(*a0, "tuple")) {
            return TR(*a0, rt("builtin", {rs(Str("tuple"))}));
        }
        return issue(ev, n, "conflict", Str("tuple() of a sequence (its length is not in the type)"));
    }
    if (IS("dict")) {
        int64_t s = pr.new_site(n, Str("dict"), ev.where(n));
        std::vector<Str> keys;
        for (const A::keyword* k : n->keywords) {
            if (!k->has_arg) {
                T vt = ev.ev(k->value, state);
                ev.spread(s, vt, n);
            } else {
                T ft = ev.fit(k->value, dv(kw_at(kw, Str(k->arg))), Str("field ") + Str(k->arg));
                pr.grow_field(s, Str(k->arg), ft);
                keys.push_back(Str(k->arg));
            }
        }
        if (!args.empty()) {
            if (k_is(dv(a0), "dict")) {
                ev.spread(s, *a0, n);
            } else {
                return issue(ev, n, "syntax", Str("dict() of a ") + a0->kind);
            }
        }
        if (ev.live()) {
            std::vector<Str> all = keys;
            if (pr.spread_keys.contains(s)) {
                for (const Str& k : pr.spread_keys.at(s)) {
                    all.push_back(k);
                }
            }
            pr.literal_keys[n] = all;
            if (ev.builder != static_cast<const A::Node*>(n)) {
                pr.created(s, program::StrSet(all.begin(), all.end()));
            }
        }
        return TR(t_site(PS("dict"), s), rt("builtin", {rs(Str("dict"))}));
    }
    if (IS("print")) {
        for (const std::optional<Str>& k : kw) {
            if (!(k && (lit_eq(*k, "end") || lit_eq(*k, "sep") || lit_eq(*k, "file") || lit_eq(*k, "flush")))) {
                return issue(ev, n, "syntax", Str("print keywords"));
            }
        }
        if (kw_has(kw, "file")) {
            return issue(ev, n, "stdlib", Str("print(file=)"));
        }
        for (size_t i = 0; i < n->args.size() && i < args.size(); ++i) {
            if (args[i]) {
                ev.check_str(n->args[i], *args[i], false);
            }
        }
        return TR(P::NONE(), rt("builtin", {rs(Str("print"))}));
    }
    if (IS("open")) {
        std::optional<Str> mode = n->args.size() > 1 ? str_of(n->args[1]) : str_of(kw_value_node(n, "mode"));
        if (mode && lit_eq(*mode, "rb")) {
            return TR(T(PS("extern"), {}, std::nullopt, std::nullopt, Str("file")), rt("builtin", {rs(Str("open_rb"))}));
        }
        return issue(ev, n, "stdlib", Str("open(") + opt_repr(mode) + Str(")"));
    }
    if (IS("ord")) {
        return TR(P::INT(Big(0), Big(0x10FFFF)), rt("builtin", {rs(Str("ord"))}));
    }
    if (IS("chr")) {
        if (a0 && !k_in(*a0, {"int", "bottom", "top"})) {
            return issue(ev, n, "conflict", Str("chr of ") + a0->kind);
        }
        return TR(P::STR(), rt("builtin", {rs(Str("chr"))}));
    }
    if (IS("hex") || IS("oct") || IS("bin")) {
        return TR(P::STR(), rt("builtin", {rs(name)}));
    }
    if (IS("divmod")) {
        if (args.size() == 2 && args[0] && args[1] && k_is(*args[0], "int") && k_is(*args[1], "int")) {
            return TR(P::tuple_of({P::ifloordiv(*args[0], *args[1]), P::imod(*args[0], *args[1])}), rt("builtin", {rs(Str("divmod"))}));
        }
        return issue(ev, n, "conflict", Str("divmod arguments"));
    }
    if (IS("pow")) {
        if (args.size() == 3 && args[0] && args[1] && args[2] && k_is(*args[0], "int") && k_is(*args[1], "int") &&
            k_is(*args[2], "int") && args[2]->positive()) {
            return TR(P::INT(Big(0), !args[2]->hi ? Bound() : Bound(*args[2]->hi - Big(1))), rt("builtin", {rs(Str("pow3"))}));
        }
        if (args.size() == 2) {
            return TR(ev.binop(n, A::OpKind::Pow, dv(args[0]), dv(args[1]), n->args[0], n->args[1]), rt("builtin", {rs(Str("pow"))}));
        }
        return issue(ev, n, "conflict", Str("pow arguments"));
    }
    if (IS("round")) {
        if (args.size() == 1) {
            return TR(P::INT(), rt("builtin", {rs(Str("round"))}));
        }
        return TR(P::FLOAT(), rt("builtin", {rs(Str("round2"))}));
    }
    if (IS("format")) {
        std::optional<Str> spec = n->args.size() > 1 ? str_of(n->args[1]) : std::optional<Str>(Str());
        if (!spec) {
            return issue(ev, n, "dynamic", Str("format with a computed spec"));
        }
        ev.check_format(n, dv(a0), *spec);
        return TR(P::STR(), rt("builtin", {rs(Str("format"))}));
    }
    if (IS("isinstance")) {
        return is_instance(ev, n, a0);
    }
    if (IS("super")) {
        if (!args.empty()) {
            return issue(ev, n, "syntax", Str("super(arguments)"));
        }
        return TR(T(PS("extern"), {}, std::nullopt, std::nullopt, Str("super")), rt("builtin", {rs(Str("super"))}));
    }
    if (IS("map") || IS("filter")) {
        return issue(ev, n, "dynamic", name);
    }
    if (IS("object") || IS("staticmethod") || IS("classmethod") || IS("property")) {
        return issue(ev, n, "dynamic", name);
    }
#undef IS
    return issue(ev, n, "stdlib", Str("builtins.") + name);
}

namespace {

// isinstance with a statically known answer (the value's type is one kind) is that answer; one
// that depends on the run-time class is refused.
TR is_instance(Eval& ev, const A::Call* n, const std::optional<T>& t) {
    if (!t || n->args.size() != 2) {
        return issue(ev, n, "conflict", Str("isinstance arguments"));
    }
    if (k_in(*t, {"bottom", "top"})) {
        return TR(P::BOOL());
    }
    const A::Expr* spec = n->args[1];
    std::vector<const A::Expr*> names;
    if (auto* tu = A::as<A::Tuple>(spec)) {
        for (const A::Expr* x : tu->elts) {
            names.push_back(x);
        }
    } else {
        names.push_back(spec);
    }
    bool answer = false;
    for (const A::Expr* x : names) {
        auto* nm = A::as<A::Name>(x);
        const std::vector<const char*>* ist = nm ? istype(nm->id) : nullptr;
        if (nm && ist) {
            if (in_kinds(*t, *ist)) {
                answer = true;
            } else if (k_is(*t, "opt") && in_kinds(t->args[0], *ist)) {
                return issue(ev, n, "dynamic", Str("isinstance of an optional value (use `is not None`)"));
            }
        } else if (nm && k_is(*t, "inst")) {
            std::optional<Str> cq = ev.lookup_class(x);
            if (!cq) {
                return issue(ev, n, "dynamic", Str("isinstance ") + program::unparse(x));
            }
            std::vector<Str> mro_t = ev.p.mro(*t->name);
            std::vector<Str> mro_c = ev.p.mro(*cq);
            if (std::find(mro_t.begin(), mro_t.end(), *cq) != mro_t.end()) {
                answer = true;
            } else if (std::find(mro_c.begin(), mro_c.end(), *t->name) != mro_c.end()) {
                return issue(ev, n, "dynamic", Str("isinstance of a subclass (run-time class)"));
            }
        } else if (nm) {
            if (!ev.lookup_class(x) && !istype(nm->id)) {
                return issue(ev, n, "dynamic", Str("isinstance ") + program::unparse(x));
            }
        } else {
            return issue(ev, n, "dynamic", Str("isinstance ") + program::unparse(x));
        }
    }
    return TR(P::BOOL(), rt("isinstance", {TupItem(answer)}));
}

TR struct_call(Eval& ev, const A::Call* n, const Str& name, const Args& args, const Kw& kw) {
    // fn = name.split('.', 1)[1]
    const std::u32string& u = name.u32();
    Str fn = Str(u.substr(u.find(U'.') + 1));
    if (kw.size() != 0) {
        return issue(ev, n, "syntax", name + Str(" keywords"));
    }
    if (lit_eq(fn, "error")) {
        return TR(T(PS("inst"), {}, std::nullopt, std::nullopt, Str("exc:struct.error")), rt("exc", {rs(Str("struct.error"))}));
    }
    std::optional<Str> fmt = n->args.empty() ? std::nullopt : str_of(n->args[0]);
    if (!fmt) {
        return issue(ev, n, "dynamic", name + Str(" with a computed format"));
    }
    auto items = structfmt::parse(*fmt);
    if (!items) {
        return issue(ev, n, "stdlib", name + Str("(") + program::repr_str(*fmt) + Str(")"));
    }
    py::list<structfmt::Item> vals = structfmt::values(*items);
    if (lit_eq(fn, "calcsize")) {
        return TR(P::const_int(Big(structfmt::size(*items))), rt("std", {rs(name)}));
    }
    auto from_u64 = [](uint64_t v) { return v <= static_cast<uint64_t>(INT64_MAX) ? Big(static_cast<int64_t>(v)) : Big::from_decimal(std::to_string(v)); };
    if (lit_eq(fn, "unpack") || lit_eq(fn, "unpack_from")) {
        if ((lit_eq(fn, "unpack") && args.size() != 2) || (lit_eq(fn, "unpack_from") && args.size() != 2 && args.size() != 3)) {
            return issue(ev, n, "conflict", name + Str(" arity"));
        }
        const std::optional<T>& b = args[1];
        if (!b || !k_in(*b, {"bytes", "bytearray", "bottom", "top"})) {
            return issue(ev, n, "conflict", name + Str(" of a ") + (b ? b->kind : Str("?")));
        }
        if (args.size() == 3 && args[2] && !k_in(*args[2], {"int", "bottom", "top"})) {
            return issue(ev, n, "conflict", name + Str(" offset"));
        }
        std::vector<T> ts;
        for (const structfmt::Item& it : vals) {
            const Str& kind = std::get<2>(it);
            if (lit_eq(kind, "int")) {
                ts.push_back(P::INT(Big(std::get<3>(it)), from_u64(std::get<4>(it))));
            } else if (lit_eq(kind, "bytes")) {
                ts.push_back(P::bytes_of(Big(std::get<1>(it))));
            } else if (lit_eq(kind, "bool")) {
                ts.push_back(P::BOOL());
            } else {
                ts.push_back(P::FLOAT());
            }
        }
        return TR(P::tuple_of(ts), rt("std", {rs(name), rs(*fmt)}));
    }
    if (lit_eq(fn, "pack") || lit_eq(fn, "pack_into")) {
        size_t k0 = lit_eq(fn, "pack") ? 1 : 3;
        std::vector<std::optional<T>> given;
        for (size_t i = k0; i < args.size(); ++i) {
            given.push_back(args[i]);
        }
        if (given.size() != static_cast<size_t>(vals.size())) {
            return issue(ev, n, "conflict",
                         name + Str(": ") + i2s(static_cast<int64_t>(given.size())) + Str(" values for ") + program::repr_str(*fmt));
        }
        if (lit_eq(fn, "pack_into") && (args.size() < 2 || !args[1] || !k_in(*args[1], {"bytearray", "bottom", "top"}))) {
            return issue(ev, n, "conflict", Str("pack_into a ") + (args.size() >= 2 && args[1] ? args[1]->kind : Str("?")));
        }
        for (size_t i = 0; i < given.size(); ++i) {
            if (!given[i]) {
                return issue(ev, n, "conflict", name + Str(" value"));
            }
            const structfmt::Item& it = vals[static_cast<int64_t>(i)];
            const Str& kind = std::get<2>(it);
            std::vector<const char*> want;
            if (lit_eq(kind, "int")) want = {"int", "bool"};
            else if (lit_eq(kind, "bytes")) want = {"bytes", "bytearray"};
            else if (lit_eq(kind, "bool")) want = {"bool", "int"};
            else want = {"float", "int"};
            want.push_back("bottom");
            want.push_back("top");
            if (!in_kinds(*given[i], want)) {
                return issue(ev, n, "conflict", name + Str(" of a ") + given[i]->kind + Str(" for ") + std::get<0>(it));
            }
        }
        return TR(lit_eq(fn, "pack") ? P::bytes_of(Big(structfmt::size(*items))) : P::NONE(), rt("std", {rs(name), rs(*fmt)}));
    }
    return issue(ev, n, "stdlib", name);
}

}  // namespace

// ---------------------------------------------------------------------------------------- stdlib
TR std_call(Eval& ev, const A::Call* n, const Str& name, const Args& args, const Kw& kw, const State& state) {
    (void)state;
    Program& pr = ev.p;
    if (auto bad = lazy(ev, n, args, kw, false)) {
        return *bad;
    }
    if (str_starts(name, "struct.")) {
        return struct_call(ev, n, name, args, kw);
    }
    struct Path {
        const char* name;
        int nargs;  // -1: one or more
        T res;
    };
    static const std::vector<Path> PATH = {
        {"os.path.join", -1, P::STR()},     {"os.path.basename", 1, P::STR()}, {"os.path.dirname", 1, P::STR()},
        {"os.path.splitext", 1, P::tuple_of({P::STR(), P::STR()})},           {"os.path.exists", 1, P::BOOL()},
        {"os.path.isfile", 1, P::BOOL()},   {"os.path.isdir", 1, P::BOOL()},   {"os.path.getsize", 1, P::SIZE()},
        {"os.path.split", 1, P::tuple_of({P::STR(), P::STR()})},              {"os.path.normcase", 1, P::STR()}};
    for (const Path& pe : PATH) {
        if (!lit_eq(name, pe.name)) {
            continue;
        }
        if (kw.size() != 0) {
            return issue(ev, n, "syntax", name + Str(" keywords"));
        }
        auto str_like = [](const std::optional<T>& a) { return a && k_in(*a, {"str", "bottom", "top"}); };
        if (pe.nargs == -1) {
            bool bad = args.empty();
            for (const auto& a : args) {
                if (!str_like(a)) {
                    bad = true;
                }
            }
            if (bad) {
                return issue(ev, n, "conflict", name + Str(" arguments"));
            }
        } else {
            bool bad = static_cast<int>(args.size()) != pe.nargs;
            for (const auto& a : args) {
                if (!str_like(a)) {
                    bad = true;
                }
            }
            if (bad) {
                return issue(ev, n, "conflict", name + Str(" arguments"));
            }
        }
        return TR(pe.res, rt("std", {rs(name)}));
    }
    if (lit_eq(name, "os.listdir")) {
        int64_t s = pr.new_site(n, Str("list"), ev.where(n));
        pr.grow(pr.elem, s, P::STR());
        return TR(t_site(PS("list"), s), rt("std", {rs(name)}));
    }
    if (lit_eq(name, "binascii.crc32") || lit_eq(name, "zlib.crc32")) {
        if (args.empty() || !args[0] || !k_in(*args[0], {"bytes", "bytearray", "bottom", "top"})) {
            return issue(ev, n, "conflict", name + Str(" arguments"));
        }
        if (args.size() > 1 && (!args[1] || !args[1]->fits_i64())) {
            return issue(ev, n, "int-range", name + Str(" start value"));
        }
        return TR(P::u(32), rt("std", {rs(name)}));
    }
    if (lit_eq(name, "bisect.bisect_left") || lit_eq(name, "bisect.bisect_right") || lit_eq(name, "bisect.bisect")) {
        if (args.size() != 2 || kw.size() != 0 || !args[0] || !k_is(*args[0], "list")) {
            return issue(ev, n, "syntax", name + Str(" arguments"));
        }
        return TR(P::SIZE(), rt("std", {rs(name)}));
    }
    if (lit_eq(name, "math.sqrt")) {
        return TR(P::FLOAT(), rt("std", {rs(name)}));
    }
    if (lit_eq(name, "math.floor") || lit_eq(name, "math.ceil")) {
        return TR(P::INT(), rt("std", {rs(name)}));
    }
    // top = name.split('.')[0]
    const std::u32string& u = name.u32();
    size_t dot = u.find(U'.');
    Str top = dot == std::u32string::npos ? name : Str(u.substr(0, dot));
    if (!lit_eq(top, "re")) {
        return issue(ev, n, "stdlib", name);
    }
    size_t last = u.rfind(U'.');
    return issue(ev, n, "stdlib", Str("re.") + (last == std::u32string::npos ? name : Str(u.substr(last + 1))));
}

}  // namespace codebreaker::translate::frontends::python::methods
