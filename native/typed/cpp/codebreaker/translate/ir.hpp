// codebreaker/translate/ir.hpp -- typed C++ port of codebreaker.translate.ir.
//
// ir.py -- the translator's intermediate representation: one typed tree for every source language.
//
// Front ends build it, passes rewrite it, back ends read it (docs/TRANSLATOR_DESIGN.md). It keeps the
// source's intent -- a lambda stays a lambda, a for-each stays a for-each, string concatenation is a
// Concat -- and leaves lowering to the back end, which knows what its target can say.
//
// Every name is resolved: a Name or a Call carries the Sym it refers to, as the source language's own
// compiler decided (overloads, implicit `this`, static or not). Every expression carries its Type. Every
// node carries the line and column it came from, so ledgers and the judge point at the original.
//
// A construct a front end cannot lower yet becomes Unsupported (with its kind), never a guess: a back end
// that meets one declines the method, and the old path handles it.
//
// Typing notes (how the Python's classes are typed here):
//   * The Python makes every node class with the factories _node / _expr / _stmt from a list of slot names.
//     Here each node kind is a real class with its slots as typed members. Kind<D, Base> is the shared part
//     of those factories: it gives each class slots(), fields(), children() and repr() from the one list
//     of slots the class states in each_slot(). new_node(name) is `getattr(ir, name)()`.
//   * A node's slots are all None until set (Node.__init__ pops each from the keywords with a None default),
//     so a slot that is not a node, a type or a symbol is a std::optional of its type, and a node, type or
//     symbol slot is a null pointer for None. line and col default to 0, origin to None.
//   * What each slot holds is as the front ends fill it: a node slot holds a node (py::ref<Node>), a slot
//     named in `fields` that holds a list holds a list of nodes, mods/flags/comments are lists of str, and so
//     on. Index.index is the one slot that holds either a node or a list of nodes (the C# front end gives
//     the list for an index with several arguments): a std::variant. Where the Python holds a value of
//     no fixed type -- an int, float, bool, str or None (Lit.value), the details of a Type (`info`), the
//     dict of a binary origin or a unit's compiler target -- the value is a Val, a variant of exactly those
//     kinds (and lists and dicts of them, and a Type). Python tuples that are only ever read (Type.args,
//     Sym.params, refs, flags) are lists.
//   * walk() and unsupported() are generators / list comprehensions in the Python; here they return lists
//     (the nodes are not changed while the Python iterates them).
//   * Type.__eq__ / __hash__ are Type::operator== and Type::hash(); hash() is not Python's hash() value
//     (only equal types hash alike). Identifier spellings that are C++ keywords or macros get a
//     trailing underscore: Case.default -> default_, MachineFragment.asm -> asm_, Binary.signed -> signed_,
//     Sym.static -> static_, ir.NULL -> NULL_, ir.OVERFLOW -> OVERFLOW_ (a macro of <math.h>). The slot names the repr and slots() print are the Python's.
//   * Python ints are int64_t here.
#pragma once

#include <algorithm>
#include <cstdint>
#include <optional>
#include <tuple>
#include <variant>

#include "py.hpp"

namespace codebreaker::translate::ir {

struct Type;
struct Sym;
struct Node;
using TypeP = py::ref<Type>;
using SymP = py::ref<Sym>;
using NodeP = py::ref<Node>;

// ---------------------------------------------------------------------------------------------- Val
// A value of no fixed type: None, bool, int, float, str, a Type, a list or a dict (str keys) of Vals.
struct Val {
    using List = py::list<Val>;
    using Dict = py::dict<py::str, Val>;
    std::variant<std::monostate, bool, int64_t, double, py::str, TypeP, List, Dict> v;

    Val() {}
    Val(std::nullptr_t) {}
    Val(bool x) : v(x) {}
    Val(int x) : v(int64_t(x)) {}
    Val(int64_t x) : v(x) {}
    Val(double x) : v(x) {}
    Val(const char* x) : v(py::str(x)) {}
    Val(const py::str& x) : v(x) {}
    Val(const TypeP& x) : v(x) {}
    Val(const List& x) : v(x) {}
    Val(const Dict& x) : v(x) {}

    bool is_none() const { return std::holds_alternative<std::monostate>(v); }
    // Python's repr() of the value
    py::str repr() const;
};
using Dict = Val::Dict;

// ---------------------------------------------------------------------------------------------- types
// Language-neutral primitive kinds. Java's char is u16, byte i8; C's are mapped by width and sign.
// ('char' is C's plain char: its signedness is stated per target in `info['signed']`, never assumed)
extern const py::list<py::str> PRIMS;

// kind: one of PRIMS, or
//   'void', 'null', 'any' (unresolved), 'dynamic' (known only at run time);
//   'class'    a nominal type: `name` qualified, `args` its type arguments;
//   'typevar', 'wildcard' (`bound`);
//   'array'    `elem`, and info['count'] for a fixed-size array (C);
//   and the binary-facing kinds of docs/L2_BINARY_TYPES.md (agreed with the decompiler, 2026-10-19):
//   'ptr'      `elem` what it points at; info: space (SPACES), seg (a near pointer's segment),
//              ref (True: a C++ reference -- its reads and bindings explicit, as for a pointer);
//   'selector' info['of']: what segment it names;
//   'fn'       info: conv, distance, pops, loadds, params [{name, type, at}], result {type, at},
//              preserves, variadic;
//   'fnptr'    info: space, fn (an 'fn' Type);
//   'record'   an explicit layout: `name`, info: size, packed, role (struct union frame segment),
//              fields [{name, offset, type, pad}].
// Kind-specific details live in `info` (a dict), with the names the schema uses
// (codebreaker/translate/l2.schema.json).
struct Type : py::object {
    py::str kind;
    std::optional<py::str> name;
    py::list<TypeP> args;
    TypeP elem;
    std::optional<std::tuple<py::str, TypeP>> bound;   // a wildcard's (mode, Type)
    Dict info;

    explicit Type(const py::str& kind, const std::optional<py::str>& name = std::nullopt,
                  const py::list<TypeP>& args = py::list<TypeP>(), const TypeP& elem = nullptr,
                  const std::optional<std::tuple<py::str, TypeP>>& bound = std::nullopt, const Dict& info = Dict());

    // (kind, name, args, elem, bound, repr(sorted(info.items())))
    py::str info_key() const;
    // Type.__eq__
    bool operator==(const Type& o) const;
    bool operator!=(const Type& o) const { return !(*this == o); }
    // Type.__hash__: hash((kind, name, args, elem))
    size_t hash() const;
    // Type.__repr__
    py::str repr() const;
    bool is_prim() const;
    bool is_ref() const;
    // The unqualified name of a class (`java.util.List` -> `List`), else the kind.
    py::str simple() const;
    std::tuple<int64_t, TypeP> dims() const;
};

// a == b for two Types that may be None
bool same(const TypeP& a, const TypeP& b);

// docs/L2_BINARY_TYPES.md: a pointer's address space, and the memory model of an allocation
extern const py::list<py::str> SPACES;
extern const py::list<py::str> MEMORY;
extern const py::list<py::str> OVERFLOW_;      // an operator's integer rule; None: the source language's own

TypeP pointer_to(const TypeP& t, const py::str& space = py::str("flat"), const Val& seg = Val());
TypeP array_of(TypeP t, int64_t dims = 1);

extern const TypeP VOID;
extern const TypeP NULL_;
extern const TypeP ANY;
extern const TypeP BOOL;
extern const TypeP I32;

// ---------------------------------------------------------------------------------------------- symbols
// What a name refers to. kind: 'local', 'param', 'field', 'method', 'ctor', 'class', 'enumconst',
// 'typevar', 'package', and 'property' / 'event' (C#: a member read and written through accessors).
// owner: the qualified name of the declaring type (or method, for a local).
// For a method or constructor: params (Types), ret (Type), varargs; refs: how each parameter is passed
// where any is not by value ('ref', 'out', 'in'; '' for by value). flags: what else the source says of
// it ('extension': a static method called as if on its first argument; 'indexer'; 'operator';
// 'readonly'; 'virtual', 'abstract', 'override').
struct Sym : py::object {
    py::str kind;
    py::str name;
    std::optional<py::str> owner;
    bool static_ = false;
    TypeP type;
    py::list<TypeP> params;
    TypeP ret;
    bool varargs = false;
    py::list<py::str> refs;
    py::list<py::str> flags;

    Sym(const py::str& kind, const py::str& name, const std::optional<py::str>& owner = std::nullopt, bool static_ = false,
        const TypeP& type = nullptr, const py::list<TypeP>& params = py::list<TypeP>(), const TypeP& ret = nullptr,
        bool varargs = false, const py::list<py::str>& refs = py::list<py::str>(),
        const py::list<py::str>& flags = py::list<py::str>());

    py::str repr() const;
    std::optional<py::str> owner_simple() const;
};

// ---------------------------------------------------------------------------------------------- repr / children of a slot
// Python's repr() of a slot value
inline py::str repr_of(const TypeP& v);
inline py::str repr_of(const SymP& v);
inline py::str repr_of(const NodeP& v);
template <class T> py::str repr_of(const std::optional<T>& v);
template <class T> py::str repr_of(const py::list<T>& xs);
template <class K, class V> py::str repr_of(const py::dict<K, V>& d);
template <class T> py::str repr_of(const std::variant<T, py::list<T>>& v);
inline py::str repr_of(const py::str& v) { return py::repr(v); }
inline py::str repr_of(bool v) { return py::repr(v); }
inline py::str repr_of(int64_t v) { return py::repr(v); }
inline py::str repr_of(const Val& v) { return v.repr(); }
template <class T> py::str repr_of(const std::optional<T>& v) { return v ? repr_of(*v) : py::str("None"); }
template <class T> py::str repr_of(const py::list<T>& xs) {
    py::str out("[");
    for (int64_t i = 0; i < py::len(xs); ++i) out = out + py::str(i ? ", " : "") + repr_of(xs[i]);
    return out + py::str("]");
}
template <class K, class V> py::str repr_of(const py::dict<K, V>& d) {
    py::str out("{");
    bool first = true;
    for (const auto& kv : d.items()) {
        out = out + py::str(first ? "" : ", ") + repr_of(std::get<0>(kv)) + py::str(": ") + repr_of(std::get<1>(kv));
        first = false;
    }
    return out + py::str("}");
}
template <class T> py::str repr_of(const std::variant<T, py::list<T>>& v) {
    return std::visit([](const auto& x) { return repr_of(x); }, v);
}

// the nodes a `fields` slot holds, appended to `out` (what Node.children() yields for it)
inline void collect_children(const NodeP& v, py::list<NodeP>& out);
inline void collect_children(const std::optional<py::list<NodeP>>& v, py::list<NodeP>& out);
inline void collect_children(const std::variant<NodeP, py::list<NodeP>>& v, py::list<NodeP>& out);
// (a slot that cannot hold nodes: nothing)
template <class T> void collect_children(const T&, py::list<NodeP>&) {}

// ---------------------------------------------------------------------------------------------- nodes
// origin: where in a binary the node came from (L2_BINARY_TYPES.md section 8):
// {'module', 'segment', 'offset'} or {'module', 'va'}; None for source
struct Node : py::object {
    int64_t line = 0;
    int64_t col = 0;
    std::optional<Dict> origin;

    virtual py::str class_name() const = 0;
    // Every slot of the class and its bases below Node (an Expr's `type` included).
    virtual py::list<py::str> slots() const = 0;
    // The child slots, in source order (walk() follows them).
    virtual py::list<py::str> fields() const = 0;
    virtual py::str repr() const = 0;
    virtual py::list<NodeP> children() const = 0;

    // (the slots a base class below Node adds: Expr.type)
    template <class F> void each_base(F&&) {}
};

// Node.__init__ with an unknown keyword: TypeError
[[noreturn]] void unknown_fields(const py::str& class_name, const py::list<py::str>& names);

// The shared part of _node(): a node class D (final) deriving from Base, which states its slots in
//     template <class F> void each_slot(F&& f)       f(name, member, rank)\n// where rank is 0 for a slot that is not in `fields`, else its place in `fields` (true: in slot order; a\n// class whose `fields` are ordered otherwise, New, says the places).
template <class D, class Base> struct Kind : Base {
    py::str class_name() const override { return py::str(D::NAME); }
    py::list<py::str> slots() const override {
        py::list<py::str> out;
        const_cast<Kind*>(this)->each([&](const char* name, auto&, int) { out.append(py::str(name)); });
        return out;
    }
    py::list<py::str> fields() const override {
        std::vector<std::pair<int, py::str>> found;
        const_cast<Kind*>(this)->each([&](const char* name, auto&, int rank) {
            if (rank) found.emplace_back(rank, py::str(name));
        });
        std::stable_sort(found.begin(), found.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
        py::list<py::str> out;
        for (const auto& p : found) out.append(p.second);
        return out;
    }
    py::str repr() const override {
        py::str body;
        bool first = true;
        const_cast<Kind*>(this)->each([&](const char* name, auto& v, int) {
            body = body + py::str(first ? "" : ", ") + py::str(name) + py::str("=") + repr_of(v);
            first = false;
        });
        return py::str(D::NAME) + py::str("(") + body + py::str(")");
    }
    py::list<NodeP> children() const override {
        std::vector<std::pair<int, py::list<NodeP>>> found;
        const_cast<Kind*>(this)->each([&](const char*, auto& v, int rank) {
            if (!rank) return;
            py::list<NodeP> some;
            collect_children(v, some);
            found.emplace_back(rank, some);
        });
        std::stable_sort(found.begin(), found.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
        py::list<NodeP> out;
        for (const auto& p : found) out.extend(p.second);
        return out;
    }
    template <class F> void each(F&& f) {
        this->each_base(f);
        static_cast<D*>(this)->each_slot(f);
    }
};

using Strs = std::optional<py::list<py::str>>;
using Nodes = std::optional<py::list<NodeP>>;
using Types = std::optional<py::list<TypeP>>;
using OptStr = std::optional<py::str>;
using OptInt = std::optional<int64_t>;
using OptBool = std::optional<bool>;

inline void collect_children(const NodeP& v, py::list<NodeP>& out) {
    if (v) out.append(v);
}
inline void collect_children(const Nodes& v, py::list<NodeP>& out) {
    if (!v) return;
    for (const NodeP& x : *v) {
        if (x) out.append(x);
    }
}
inline void collect_children(const std::variant<NodeP, py::list<NodeP>>& v, py::list<NodeP>& out) {
    if (std::holds_alternative<NodeP>(v)) collect_children(std::get<NodeP>(v), out);
    else collect_children(Nodes(std::get<py::list<NodeP>>(v)), out);
}
inline py::str repr_of(const TypeP& v) { return v ? v->repr() : py::str("None"); }
inline py::str repr_of(const SymP& v) { return v ? v->repr() : py::str("None"); }
inline py::str repr_of(const NodeP& v) { return v ? v->repr() : py::str("None"); }

struct Stmt : Node {};
struct Expr : Node {
    TypeP type;
    template <class F> void each_base(F&& f) { f("type", type, false); }
};

// -- declarations
// a whole program, or one binary's module
// (header: its declarations as text, where they are not yet a tree -- l2.module, the one module model)
struct Module final : Kind<Module, Node> {
    static constexpr const char* NAME = "Module";
    Nodes units;
    OptStr header;
    template <class F> void each_slot(F&& f) { f("units", units, true); f("header", header, false); }
};
// (lang: the source language -- 'java', 'csharp', ... -- whose semantics the unit's library calls, string
// conversions (Concat) and enum and struct kinds carry; a back end maps them by it)
// (target: a C/C++ unit's target and the widths its compiler gave pointer, long and plain char -- a dict)
struct Unit final : Kind<Unit, Node> {
    static constexpr const char* NAME = "Unit";
    OptStr path;
    OptStr package;
    Strs imports;
    Nodes types;
    OptStr lang;
    std::optional<Dict> target;
    template <class F> void each_slot(F&& f) {
        f("path", path, false); f("package", package, false); f("imports", imports, false); f("types", types, true);
        f("lang", lang, false); f("target", target, false);
    }
};
// (kind also: struct -- a value type, copied on assignment --, 'record struct', delegate -- a named function
// type whose one member is its Invoke --; tparams: the type parameters with their bounds, as TypeParam)
// (layout: a C/C++ struct or union whose bytes the source can see -- its 'record' Type: offsets, size)
// (a C struct, a frame, a data segment with a fixed layout is a 'record' Type, beside ClassDecl)
// kind: class interface enum record annotation; outer: the qualified name of the enclosing type
struct ClassDecl final : Kind<ClassDecl, Node> {
    static constexpr const char* NAME = "ClassDecl";
    OptStr name;
    OptStr qname;
    OptStr kind;
    Strs mods;
    Strs typeparams;
    TypeP extends;
    Types implements;
    Nodes members;
    OptStr outer;
    Nodes tparams;
    TypeP layout;
    template <class F> void each_slot(F&& f) {
        f("name", name, false); f("qname", qname, false); f("kind", kind, false); f("mods", mods, false);
        f("typeparams", typeparams, false); f("extends", extends, false); f("implements", implements, false);
        f("members", members, true); f("outer", outer, false); f("tparams", tparams, false); f("layout", layout, false);
    }
};
struct FieldDecl final : Kind<FieldDecl, Node> {
    static constexpr const char* NAME = "FieldDecl";
    SymP sym;
    TypeP type;
    NodeP init;
    Strs mods;
    template <class F> void each_slot(F&& f) {
        f("sym", sym, false); f("type", type, false); f("init", init, true); f("mods", mods, false);
    }
};
// kind: method ctor static_init init, or function (a free function: C, a decompiled binary). A function
// whose type carries a calling convention (an 'fn' Type) has it on sym.type, where callers read it too.
// comments: the text of each comment written above it (a decompiler's warnings). text: its source as
// text where its tree is not yet written (the C a function store holds).
struct MethodDecl final : Kind<MethodDecl, Node> {
    static constexpr const char* NAME = "MethodDecl";
    SymP sym;
    OptStr name;
    OptStr kind;
    Nodes params;
    TypeP ret;
    NodeP body;
    Strs mods;
    Types throws;
    Strs typeparams;
    Strs comments;
    OptStr text;
    Nodes tparams;
    template <class F> void each_slot(F&& f) {
        f("sym", sym, false); f("name", name, false); f("kind", kind, false); f("params", params, true);
        f("ret", ret, false); f("body", body, true); f("mods", mods, false); f("throws", throws, false);
        f("typeparams", typeparams, false); f("comments", comments, false); f("text", text, false);
        f("tparams", tparams, false);
    }
};
struct Param final : Kind<Param, Node> {
    static constexpr const char* NAME = "Param";
    SymP sym;
    TypeP type;
    Strs mods;
    template <class F> void each_slot(F&& f) { f("sym", sym, false); f("type", type, false); f("mods", mods, false); }
};
// (value: an enum that is a named integer -- C, C# -- gives each constant's number; Java's are objects)
struct EnumConst final : Kind<EnumConst, Node> {
    static constexpr const char* NAME = "EnumConst";
    SymP sym;
    Nodes args;
    NodeP body;
    SymP ctor;
    OptInt value;
    template <class F> void each_slot(F&& f) {
        f("sym", sym, false); f("args", args, true); f("body", body, true); f("ctor", ctor, false); f("value", value, false);
    }
};
// a member read and written through accessors (C# properties and indexers): accessors are MethodDecls of
// kind get / set / init, each with body None where the compiler supplies it (an auto-property's field)
struct Property final : Kind<Property, Node> {
    static constexpr const char* NAME = "Property";
    SymP sym;
    TypeP type;
    Strs mods;
    Nodes params;
    Nodes accessors;
    NodeP init;
    template <class F> void each_slot(F&& f) {
        f("sym", sym, false); f("type", type, false); f("mods", mods, false); f("params", params, true);
        f("accessors", accessors, true); f("init", init, true);
    }
};
// a type parameter: bounds (Types it must extend or implement), flags (class, struct, new)
struct TypeParam final : Kind<TypeParam, Node> {
    static constexpr const char* NAME = "TypeParam";
    OptStr name;
    Types bounds;
    Strs flags;
    OptStr variance;
    template <class F> void each_slot(F&& f) {
        f("name", name, false); f("bounds", bounds, false); f("flags", flags, false); f("variance", variance, false);
    }
};

// -- statements
struct Block final : Kind<Block, Stmt> {
    static constexpr const char* NAME = "Block";
    Nodes stmts;
    template <class F> void each_slot(F&& f) { f("stmts", stmts, true); }
};
struct Local final : Kind<Local, Stmt> {
    static constexpr const char* NAME = "Local";
    SymP sym;
    TypeP type;
    NodeP init;
    Strs mods;
    template <class F> void each_slot(F&& f) {
        f("sym", sym, false); f("type", type, false); f("init", init, true); f("mods", mods, false);
    }
};
struct ExprStmt final : Kind<ExprStmt, Stmt> {
    static constexpr const char* NAME = "ExprStmt";
    NodeP expr;
    template <class F> void each_slot(F&& f) { f("expr", expr, true); }
};
struct If final : Kind<If, Stmt> {
    static constexpr const char* NAME = "If";
    NodeP cond;
    NodeP then;
    NodeP orelse;
    template <class F> void each_slot(F&& f) { f("cond", cond, true); f("then", then, true); f("orelse", orelse, true); }
};
struct While final : Kind<While, Stmt> {
    static constexpr const char* NAME = "While";
    NodeP cond;
    NodeP body;
    template <class F> void each_slot(F&& f) { f("cond", cond, true); f("body", body, true); }
};
struct DoWhile final : Kind<DoWhile, Stmt> {
    static constexpr const char* NAME = "DoWhile";
    NodeP body;
    NodeP cond;
    template <class F> void each_slot(F&& f) { f("body", body, true); f("cond", cond, true); }
};
struct For final : Kind<For, Stmt> {
    static constexpr const char* NAME = "For";
    Nodes init;
    NodeP cond;
    Nodes update;
    NodeP body;
    template <class F> void each_slot(F&& f) {
        f("init", init, true); f("cond", cond, true); f("update", update, true); f("body", body, true);
    }
};
// var: a Local without init
struct ForEach final : Kind<ForEach, Stmt> {
    static constexpr const char* NAME = "ForEach";
    NodeP var;
    NodeP iterable;
    NodeP body;
    template <class F> void each_slot(F&& f) { f("var", var, true); f("iterable", iterable, true); f("body", body, true); }
};
struct Switch final : Kind<Switch, Stmt> {
    static constexpr const char* NAME = "Switch";
    NodeP expr;
    Nodes cases;
    template <class F> void each_slot(F&& f) { f("expr", expr, true); f("cases", cases, true); }
};
// body: [Stmt]; falls through as in C
struct Case final : Kind<Case, Stmt> {
    static constexpr const char* NAME = "Case";
    Nodes labels;
    OptBool default_;
    Nodes body;
    template <class F> void each_slot(F&& f) { f("labels", labels, true); f("default", default_, false); f("body", body, true); }
};
struct Break final : Kind<Break, Stmt> {
    static constexpr const char* NAME = "Break";
    OptStr label;
    template <class F> void each_slot(F&& f) { f("label", label, false); }
};
struct Continue final : Kind<Continue, Stmt> {
    static constexpr const char* NAME = "Continue";
    OptStr label;
    template <class F> void each_slot(F&& f) { f("label", label, false); }
};
struct Return final : Kind<Return, Stmt> {
    static constexpr const char* NAME = "Return";
    NodeP expr;
    template <class F> void each_slot(F&& f) { f("expr", expr, true); }
};
struct Throw final : Kind<Throw, Stmt> {
    static constexpr const char* NAME = "Throw";
    NodeP expr;
    template <class F> void each_slot(F&& f) { f("expr", expr, true); }
};
struct Try final : Kind<Try, Stmt> {
    static constexpr const char* NAME = "Try";
    Nodes resources;
    NodeP body;
    Nodes catches;
    NodeP final;
    template <class F> void each_slot(F&& f) {
        f("resources", resources, true); f("body", body, true); f("catches", catches, true); f("final", final, true);
    }
};
// several types: a multi-catch
struct Catch final : Kind<Catch, Stmt> {
    static constexpr const char* NAME = "Catch";
    SymP sym;
    Types types;
    NodeP body;
    template <class F> void each_slot(F&& f) { f("sym", sym, false); f("types", types, false); f("body", body, true); }
};
struct Labeled final : Kind<Labeled, Stmt> {
    static constexpr const char* NAME = "Labeled";
    OptStr label;
    NodeP body;
    template <class F> void each_slot(F&& f) { f("label", label, false); f("body", body, true); }
};
// C, and whatever structuring leaves in a binary
struct Goto final : Kind<Goto, Stmt> {
    static constexpr const char* NAME = "Goto";
    OptStr label;
    template <class F> void each_slot(F&& f) { f("label", label, false); }
};
// What stays assembly (L2_BINARY_TYPES.md section 7), as a statement, a member or a whole function:
// origin, length in bytes, reason (hand-written, register-arguments, ds-left-changed, ...), why, asm;
// name (the function it stands for), syntax (the assembler: wasm, masm, nasm), within (the fragment
// whose asm holds it too: a run of functions assembled as one file).
struct MachineFragment final : Kind<MachineFragment, Stmt> {
    static constexpr const char* NAME = "MachineFragment";
    OptInt length;
    OptStr reason;
    OptStr why;
    OptStr asm_;
    OptStr name;
    OptStr syntax;
    OptStr within;
    template <class F> void each_slot(F&& f) {
        f("length", length, false); f("reason", reason, false); f("why", why, false); f("asm", asm_, false);
        f("name", name, false); f("syntax", syntax, false); f("within", within, false);
    }
};
struct Sync final : Kind<Sync, Stmt> {
    static constexpr const char* NAME = "Sync";
    NodeP lock;
    NodeP body;
    template <class F> void each_slot(F&& f) { f("lock", lock, true); f("body", body, true); }
};
struct Empty final : Kind<Empty, Stmt> {
    static constexpr const char* NAME = "Empty";
    template <class F> void each_slot(F&&) {}
};
// a comment at its place among the statements (a warning)
struct Comment final : Kind<Comment, Stmt> {
    static constexpr const char* NAME = "Comment";
    OptStr text;
    template <class F> void each_slot(F&& f) { f("text", text, false); }
};
struct LocalClass final : Kind<LocalClass, Stmt> {
    static constexpr const char* NAME = "LocalClass";
    NodeP decl;
    template <class F> void each_slot(F&& f) { f("decl", decl, true); }
};
struct Assert final : Kind<Assert, Stmt> {
    static constexpr const char* NAME = "Assert";
    NodeP cond;
    NodeP detail;
    template <class F> void each_slot(F&& f) { f("cond", cond, true); f("detail", detail, true); }
};
// this(...) / super(...)
struct CtorCall final : Kind<CtorCall, Stmt> {
    static constexpr const char* NAME = "CtorCall";
    OptStr kind;
    SymP sym;
    Nodes args;
    NodeP target;
    template <class F> void each_slot(F&& f) {
        f("kind", kind, false); f("sym", sym, false); f("args", args, true); f("target", target, true);
    }
};
// a switch expression's arm gives its value
struct Yield final : Kind<Yield, Stmt> {
    static constexpr const char* NAME = "Yield";
    NodeP expr;
    template <class F> void each_slot(F&& f) { f("expr", expr, true); }
};

// -- expressions
// value: int, float, bool, str, None (null); text: as spelled
struct Lit final : Kind<Lit, Expr> {
    static constexpr const char* NAME = "Lit";
    Val value;
    OptStr text;
    template <class F> void each_slot(F&& f) { f("value", value, false); f("text", text, false); }
};
// a local or parameter
struct Name final : Kind<Name, Expr> {
    static constexpr const char* NAME = "Name";
    SymP sym;
    template <class F> void each_slot(F&& f) { f("sym", sym, false); }
};
// a field: obj is an Expr, This, or TypeName for a static
struct Select final : Kind<Select, Expr> {
    static constexpr const char* NAME = "Select";
    NodeP obj;
    SymP sym;
    template <class F> void each_slot(F&& f) { f("obj", obj, true); f("sym", sym, false); }
};
// a type used as a qualifier (Block.stone); .type says which
struct TypeName final : Kind<TypeName, Expr> {
    static constexpr const char* NAME = "TypeName";
    template <class F> void each_slot(F&&) {}
};
struct This final : Kind<This, Expr> {
    static constexpr const char* NAME = "This";
    TypeP qualifier;
    template <class F> void each_slot(F&& f) { f("qualifier", qualifier, false); }
};
struct Super final : Kind<Super, Expr> {
    static constexpr const char* NAME = "Super";
    TypeP qualifier;
    template <class F> void each_slot(F&& f) { f("qualifier", qualifier, false); }
};
// target None: own method, implicit
struct Call final : Kind<Call, Expr> {
    static constexpr const char* NAME = "Call";
    NodeP target;
    SymP sym;
    Nodes args;
    Types typeargs;
    template <class F> void each_slot(F&& f) {
        f("target", target, true); f("sym", sym, false); f("args", args, true); f("typeargs", typeargs, false);
    }
};
// memory: MEMORY; body: an anonymous ClassDecl
struct New final : Kind<New, Expr> {
    static constexpr const char* NAME = "New";
    SymP sym;
    Nodes args;
    NodeP body;
    NodeP outer;
    OptStr memory;
    template <class F> void each_slot(F&& f) {
        f("sym", sym, false); f("args", args, 2); f("body", body, 3); f("outer", outer, 1); f("memory", memory, false);
    }
};
// elem: the element Type
struct NewArray final : Kind<NewArray, Expr> {
    static constexpr const char* NAME = "NewArray";
    TypeP elem;
    Nodes dims;
    Nodes init;
    template <class F> void each_slot(F&& f) { f("elem", elem, false); f("dims", dims, true); f("init", init, true); }
};
// sym: an indexer (C#), when not an array; index: a node, or the list of nodes of an index with several
// arguments
struct Index final : Kind<Index, Expr> {
    static constexpr const char* NAME = "Index";
    NodeP arr;
    std::variant<NodeP, py::list<NodeP>> index;
    SymP sym;
    template <class F> void each_slot(F&& f) { f("arr", arr, true); f("index", index, true); f("sym", sym, false); }
};
// an array's length
struct Length final : Kind<Length, Expr> {
    static constexpr const char* NAME = "Length";
    NodeP arr;
    template <class F> void each_slot(F&& f) { f("arr", arr, true); }
};
// op: - + ! ~ ++ --
struct Unary final : Kind<Unary, Expr> {
    static constexpr const char* NAME = "Unary";
    OptStr op;
    NodeP x;
    OptBool postfix;
    OptStr overflow;
    template <class F> void each_slot(F&& f) {
        f("op", op, false); f("x", x, true); f("postfix", postfix, false); f("overflow", overflow, false);
    }
};
// (signed: for a comparison in a binary, from the instruction (jl vs jb), not the operand types)
// op: + - * / % << >> >>> & | ^ && || == != < <= > >=
// (and ??: the left operand unless it is null, the right evaluated only then)
struct Binary final : Kind<Binary, Expr> {
    static constexpr const char* NAME = "Binary";
    OptStr op;
    NodeP left;
    NodeP right;
    OptStr overflow;
    OptBool signed_;
    template <class F> void each_slot(F&& f) {
        f("op", op, false); f("left", left, true); f("right", right, true); f("overflow", overflow, false);
        f("signed", signed_, false);
    }
};
// string concatenation, flattened
struct Concat final : Kind<Concat, Expr> {
    static constexpr const char* NAME = "Concat";
    Nodes parts;
    template <class F> void each_slot(F&& f) { f("parts", parts, true); }
};
// op None, or the compound operator
struct Assign final : Kind<Assign, Expr> {
    static constexpr const char* NAME = "Assign";
    OptStr op;
    NodeP target;
    NodeP value;
    OptStr overflow;
    template <class F> void each_slot(F&& f) {
        f("op", op, false); f("target", target, true); f("value", value, true); f("overflow", overflow, false);
    }
};
struct Cond final : Kind<Cond, Expr> {
    static constexpr const char* NAME = "Cond";
    NodeP cond;
    NodeP then;
    NodeP orelse;
    template <class F> void each_slot(F&& f) { f("cond", cond, true); f("then", then, true); f("orelse", orelse, true); }
};
// .type is the target type
// (implicit: True for a conversion the source language makes by itself -- C's promotions, array decay)
struct Cast final : Kind<Cast, Expr> {
    static constexpr const char* NAME = "Cast";
    NodeP x;
    OptStr overflow;
    OptBool implicit;
    template <class F> void each_slot(F&& f) { f("x", x, true); f("overflow", overflow, false); f("implicit", implicit, false); }
};
// test: a Type; binding: a pattern's Local
struct InstanceOf final : Kind<InstanceOf, Expr> {
    static constexpr const char* NAME = "InstanceOf";
    NodeP x;
    TypeP test;
    NodeP binding;
    template <class F> void each_slot(F&& f) { f("x", x, true); f("test", test, false); f("binding", binding, false); }
};
// X.class; of: a Type
struct ClassLit final : Kind<ClassLit, Expr> {
    static constexpr const char* NAME = "ClassLit";
    TypeP of;
    template <class F> void each_slot(F&& f) { f("of", of, false); }
};
// body: an Expr or a Block
struct Lambda final : Kind<Lambda, Expr> {
    static constexpr const char* NAME = "Lambda";
    Nodes params;
    NodeP body;
    template <class F> void each_slot(F&& f) { f("params", params, true); f("body", body, true); }
};
// mode: invoke / new
struct MethodRef final : Kind<MethodRef, Expr> {
    static constexpr const char* NAME = "MethodRef";
    NodeP target;
    SymP sym;
    OptStr mode;
    template <class F> void each_slot(F&& f) { f("target", target, true); f("sym", sym, false); f("mode", mode, false); }
};
// cases: [Case]; each arm ends in a Yield or a Throw
struct SwitchExpr final : Kind<SwitchExpr, Expr> {
    static constexpr const char* NAME = "SwitchExpr";
    NodeP expr;
    Nodes cases;
    template <class F> void each_slot(F&& f) { f("expr", expr, true); f("cases", cases, true); }
};
// a constant code or data address: by symbol, never a number
struct AddressOf final : Kind<AddressOf, Expr> {
    static constexpr const char* NAME = "AddressOf";
    SymP sym;
    template <class F> void each_slot(F&& f) { f("sym", sym, false); }
};
// selector:offset, typed ptr far (MK_FP)
struct FarAddress final : Kind<FarAddress, Expr> {
    static constexpr const char* NAME = "FarAddress";
    NodeP selector;
    NodeP offset;
    template <class F> void each_slot(F&& f) { f("selector", selector, true); f("offset", offset, true); }
};
// `width` bytes of x from byte `byte`
struct Piece final : Kind<Piece, Expr> {
    static constexpr const char* NAME = "Piece";
    NodeP x;
    OptInt byte;
    OptInt width;
    template <class F> void each_slot(F&& f) { f("x", x, true); f("byte", byte, false); f("width", width, false); }
};
// hi:lo as one wider value
struct Join final : Kind<Join, Expr> {
    static constexpr const char* NAME = "Join";
    NodeP hi;
    NodeP lo;
    template <class F> void each_slot(F&& f) { f("hi", hi, true); f("lo", lo, true); }
};
// kept: back ends print grouping the source chose
struct Paren final : Kind<Paren, Expr> {
    static constexpr const char* NAME = "Paren";
    NodeP x;
    template <class F> void each_slot(F&& f) { f("x", x, true); }
};
// a construct the front end cannot lower yet
struct Unsupported final : Kind<Unsupported, Expr> {
    static constexpr const char* NAME = "Unsupported";
    OptStr kind;
    OptStr raw;
    template <class F> void each_slot(F&& f) { f("kind", kind, false); f("raw", raw, false); }
};

// `getattr(ir, class_name)()`: a new node of that kind, every slot None. AttributeError for an unknown name.
NodeP new_node(const py::str& class_name);

extern const py::dict<py::str, py::str> OPERATORS;
extern const py::dict<py::str, std::tuple<py::str, bool>> UNARY;

// Every node under (and including) `node`, depth first, in source order.
py::list<NodeP> walk(const NodeP& node);

// The Unsupported nodes under `node` -- what keeps it from going through the new path.
py::list<py::ref<Unsupported>> unsupported(const NodeP& node);

}  // namespace codebreaker::translate::ir




