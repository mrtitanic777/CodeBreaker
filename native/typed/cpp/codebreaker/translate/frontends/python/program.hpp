// codebreaker/translate/frontends/python/program.hpp -- typed C++ from
// codebreaker.translate.frontends.python.program.
//
// program.py -- the modules being translated, their symbols, and the facts inference gathers about them.
//
// A Program is a set of Python modules parsed with CPython's `ast`, each with its symbol table (functions,
// classes, module-level names, imports resolved to the program's own modules or to outside libraries).
// The facts are what inference (infer.py) learns, program-wide, to a fixpoint:
//
//     params[fn]      the type of each parameter: its annotation, else the join of every call site's argument
//     returns[fn]     the join of every value it returns
//     attrs[cls]      each attribute of a class: the join of every value stored to it
//     globals[mod]    each module-level name
//     containers      per container object class (sites joined by union-find): a list's or set's element,
//                     a dict's keys and values, a record's fields and which fields every creation sets
//
// Notes on the port:
//   * the Python `ast` is native/pyfront's tree (CPython 3.14's shape, dump-identical); a node's identity
//     (Python's id(node)) is its address, a `const pyfront::Node*`; ast.walk / iter_child_nodes /
//     the `ast` helpers come from native/pyengine's astutil (AnyNode);
//   * a Python tuple of strings (an import's ('obj', module, name), a resolution ('fn', qname), a result's
//     ('tuple_index', 3)) is a `Tup`: its kind and its items, each an item a str, an int, a bool or None
//     (std::variant -- the Python mixes them in one tuple);
//   * the container tables (parent, ckind, elem, keys, fields, required) are vectors indexed by site; sites
//     start at 1, index 0 is unused;
//   * a frozenset of keys (`required`, `spread_keys`) is a std::set<py::str>: its iteration order is sorted
//     (CPython's depends on string hashes and is unspecified);
//   * the facts Python keeps as dicts stay ordered dicts (py::dict, insertion order); `union` is `union_`.
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

#include "codebreaker/translate/frontends/python/pytypes.hpp"
#include "py.hpp"
#include "py_os.hpp"
#include "pyfront/pyfront.hpp"
#include "../../../../../../pyengine/src/astutil.hpp"

// a str literal built once (every use of the same literal site shares one py::str)
#ifndef PS
#define PS(lit) ([]() -> const ::py::str& { static const ::py::str s_(lit); return s_; }())
#endif

namespace codebreaker::translate::frontends::python::infer {
class Eval;
}

namespace codebreaker::translate::frontends::python::program {

namespace A = pyfront;
namespace P = pytypes;
using T = pytypes::T;
using Big = pytypes::Big;
using Bound = pytypes::Bound;
using Str = py::str;
using StrSet = std::set<py::str>;

// Python's SyntaxError (ast.parse of a file that is not Python)
struct SyntaxError : py::Exception {
    SyntaxError() : py::Exception() {}
    explicit SyntaxError(const std::string& m) : py::Exception(m) {}
    const char* type() const override { return "SyntaxError"; }
};

struct Module;
struct Class;
struct Fn;
class Program;

// ------------------------------------------------------------------------------------ small helpers
// s == "literal", with no allocation (the literals are ASCII)
bool lit_eq(const Str& s, const char* lit);
// t.kind == "literal"
bool k_is(const T& t, const char* lit);
// t.kind in ("a", "b", ...)
bool k_in(const T& t, std::initializer_list<const char*> kinds);
// s.startswith("literal")
bool str_starts(const Str& s, const char* lit);
// s.rsplit('.', 1)[-1]
Str last_part(const Str& s);
// a Python indexed read: IndexError past the end (negative counts from the end)
template <class V> const V& at(const std::vector<V>& v, int64_t i) {
    int64_t n = static_cast<int64_t>(v.size());
    if (i < -n || i >= n) {
        throw py::IndexError("list index out of range");
    }
    return v[static_cast<size_t>(i < 0 ? i + n : i)];
}

// ------------------------------------------------------------------------------------ tuples
// one item of a result tuple: None, a str, an int, a bool
using TupItem = std::variant<std::monostate, Str, Big, bool>;

// A tuple (kind, item, ...): an import ('obj', module, name), a name's resolution ('fn', qname) ('global', mod,
// name), a result's resolution ('field', cls, attr) ('tuple_slice', a, b) ('isinstance', answer) ...
struct Tup {
    Str kind;
    std::vector<TupItem> items;
    Tup() = default;
    explicit Tup(const Str& kind) : kind(kind) {}
    Tup(const Str& kind, std::vector<TupItem> items) : kind(kind), items(std::move(items)) {}
    // item i (1-based, as the Python's r[1], r[2]) when it is a str
    const Str& s(size_t i) const;
    bool is(const char* lit) const { return lit_eq(kind, lit); }
    bool in(std::initializer_list<const char*> kinds) const;
    bool operator==(const Tup& o) const;
    bool operator!=(const Tup& o) const { return !(*this == o); }
};
using Ref = Tup;
using Res = Tup;
TupItem none_item();
inline TupItem item(const Str& s) { return TupItem(s); }
inline TupItem item(const Big& b) { return TupItem(b); }
inline TupItem item_bool(bool b) { return TupItem(b); }

// ------------------------------------------------------------------------------------ facts' keys
// what a running evaluation reads / a round changes: ('p', fn) ('d',) ('g', module) ('s', root) ('a', class)
// ('ca',) ('r', fn)  -- tag 'C' is the Python's ('ca',)
struct FactKey {
    char tag = 0;
    Str name;
    int64_t site = 0;
    bool operator==(const FactKey& o) const { return tag == o.tag && site == o.site && name == o.name; }
};
struct FactKeyHash {
    size_t operator()(const FactKey& k) const {
        return std::hash<int64_t>()(k.site) * 31 + static_cast<size_t>(k.tag) + py::str_hash()(k.name) * 1000003u;
    }
};
using FactSet = std::unordered_set<FactKey, FactKeyHash>;
FactKey fk(char tag, const Str& name = Str());
FactKey fk_site(int64_t site);

// ------------------------------------------------------------------------------------ symbols
// A function or method of the program.
struct Fn {
    Str qname;
    const A::FunctionDef* node;
    Module* module;
    Class* cls;                                  // the Class it is a method of
    Str name;
    Str kind;                                    // function method static classmethod property async
    std::vector<Str> decorators;
    std::vector<Str> params;
    std::vector<Str> kwonly;
    py::dict<Str, const A::Expr*> annotations;   // parameter -> its annotation (those that have one)
    py::dict<Str, const A::Expr*> defaults;      // parameter -> its default value
    std::optional<Str> varargs;
    std::optional<Str> kwargs;
    int64_t lines = 0;
    StrSet none_aware;                           // set by Program.index

    Fn(const Str& qname, const A::FunctionDef* node, Module* module, Class* cls = nullptr);
    // the parameters a caller passes (a method's self, a classmethod's cls not among them)
    std::vector<Str> bound() const;
    Str repr() const { return Str("<Fn ") + qname + Str(">"); }
};

struct Class {
    Str qname;
    const A::ClassDef* node;
    Module* module;
    Str name;
    std::vector<const A::Expr*> base_nodes;
    std::vector<Str> bases;                      // qualified names of the program's classes it derives from
    std::optional<Str> exception;                // the builtin exception it derives from, if it is one
    py::dict<Str, Fn*> methods;
    py::dict<Str, const A::Expr*> class_attrs;   // name -> the value node assigned in the class body
    std::vector<Str> subclasses;
    std::vector<Str> decorators;
    std::optional<Str> foreign_base;             // a base the program does not have (a library class)

    Class(const Str& qname, const A::ClassDef* node, Module* module);
};

struct Module {
    Str name;
    std::optional<Str> path;
    Str source;
    std::unique_ptr<A::Module> tree;
    py::dict<Str, Fn*> functions;
    py::dict<Str, Class*> classes;
    // name -> the value nodes assigned at module level (nullptr: an unpacking)
    py::dict<Str, std::vector<const A::Expr*>> assigns;
    // local name -> ('module', qname) | ('obj', module, name) | ('extern', name) | ('stdmod', name)
    py::dict<Str, Ref> imports;
    std::vector<const A::Stmt*> statements;      // module-level statements that are not defs, imports or names
    bool is_package = false;

    Module(const Str& name, const std::optional<Str>& path, const Str& source);
    // ast.get_docstring(tree) is not None
    bool has_docstring() const;
};

// the names of the builtin exceptions the program's classes may derive from
const std::set<Str>& EXCEPTIONS();
const std::set<Str>& STDLIB();

// _deco_name
Str deco_name(const A::Expr* d);
// _names: the names a target binds
std::vector<Str> target_names(const A::Expr* t);
// ast.unparse (the expression kinds a message names; others as <Kind>)
Str unparse(const A::Expr* e);
// _int_const-style helpers over a node
std::optional<Str> str_const(const A::Expr* n);
std::optional<Big> int_const(const A::Expr* n);
bool is_none_const(const A::Expr* e);

// ------------------------------------------------------------------------------------ the program
class Program {
  public:
    Program();
    ~Program();
    Program(const Program&) = delete;
    Program& operator=(const Program&) = delete;

    py::dict<Str, Module*> modules;              // qualified module name -> Module
    py::dict<Str, Fn*> fns;                      // qualname -> Fn
    py::dict<Str, Class*> classes;               // qualname -> Class
    // facts
    py::dict<Str, py::dict<Str, T>> params;      // fn qname -> {param: T}
    py::dict<Str, T> returns;                    // fn qname -> T
    py::dict<Str, py::dict<Str, T>> attrs;       // class qname -> {attr: T}
    py::dict<Str, py::dict<Str, T>> globals;     // module -> {name: T}
    py::dict<Str, bool> raises;                  // fn qname -> True when it can raise (C++ needs nothing more)
    // containers: union-find over sites (1-based; index 0 unused)
    std::vector<int64_t> parent;
    std::vector<Str> ckind;                      // root -> 'list' | 'dict' | 'set'
    std::vector<T> elem;                         // list/set element; dict values when used as a dict
    std::vector<T> keys;                         // dict keys
    std::vector<py::dict<Str, T>> fields;        // root -> {key: T} (insertion order)
    std::vector<std::optional<StrSet>> required; // root -> keys every creation sets (None: no creation seen)
    py::dict<int64_t, Str> dynamic;              // root -> why it is a dict and not a record
    py::dict<int64_t, std::vector<Str>> hints;   // root -> [name hints]
    std::unordered_map<const void*, int64_t> site_of_node;
    // site -> (module name, line) it was made at, None for a site made with no place
    std::vector<std::optional<std::tuple<Str, int64_t>>> site_where;
    std::vector<const A::Node*> site_node;       // site -> the AST node that is it
    std::unordered_set<int64_t> fixed;           // sites from an annotation: their element types do not grow
    bool changed = false;
    std::unordered_map<Str, std::set<Str>, py::str_hash> index_params;   // fn qname -> {param} it indexes with at once
    std::unordered_map<const void*, std::vector<Str>> literal_keys;      // dict display node -> its constant keys
    py::dict<int64_t, StrSet> spread_keys;       // site -> keys a ** spread brought, set by every creation
    py::dict<int64_t, std::set<int64_t>> spread_from;
    int64_t round = 0;
    py::dict<std::tuple<Str, Str>, T> class_attr_types;  // (class, name) -> the type of a class-level value
    py::dict<std::tuple<Str, Str>, T> default_types;     // (fn, param) -> the type of its default
    std::unordered_map<const void*, T> ann_cache;
    py::dict<Str, Str> crashes;                  // fn -> the front end error that stopped it
    std::unordered_set<const void*> builder_targets;   // item stores that fill a record right after its creation
    std::unordered_set<int64_t> mutated;
    // (fn, param) -> (a caller, why) that passed a value of no type
    py::dict<std::tuple<Str, Str>, std::tuple<Str, Str>> param_poison;
    py::dict<std::tuple<Str, Str>, Str> poisoned;        // (class, attribute) -> a function that stores it a value of no type
    py::dict<int64_t, Str> poisoned_sites;       // container root -> a function that adds it an item of no type
    Str int_policy;                              // 'strict' | 'checked': ints beyond int64 raise OverflowError in C++
    FactSet* reads = nullptr;                    // the facts the running evaluation reads (worklist solving)
    FactSet dirty;                               // the facts changed this round
    int64_t suspend = 0;                         // > 0 while a loop body runs toward its fixpoint

    // -- loading
    Module& add_module(const Str& name, const std::optional<Str>& path = std::nullopt,
                       const std::optional<Str>& source = std::nullopt);
    // Every .py file under `root` (the folder of package `package`).
    void add_tree(const Str& root, const Str& package);
    // Build every module's symbol table, then resolve class bases and imports.
    void index();

    // -- type lookups
    T annotation_type(const Fn& f, const Str& name);
    T ann_to_type(Module& m, const A::Expr* a, const A::Expr* site_node);
    T param_type(const Fn& f, const Str& name);
    T global_type(const Str& mod, const Str& name);
    Fn* find_method(const Str& cq, const Str& name);
    const StrSet& assigned_attrs(const Str& cq);
    // attributes assigned on instances by the class or any class derived from it
    StrSet assigned_attrs_any(const Str& cq);
    // the most basic class that has the attribute (where C++ declares the member)
    std::optional<Str> attr_owner(const Str& cq, const Str& attr);
    // None: no such attribute
    std::optional<T> attr_type(const Str& cq, const Str& attr);
    T class_attr_type(const Str& cq, const Str& attr);
    const std::vector<A::Stmt*>* block_of(const A::Stmt* st) const;
    void map_blocks(const A::FunctionDef* node);

    // -- solving
    // Run every function, then round after round only those that read a fact the last round changed,
    // until none changed. -> rounds run
    int64_t solve(int64_t max_rounds = 40, const std::function<void(const Str&)>& log = nullptr);
    // the recording pass of one function -> infer.Eval (types, resolutions, issues)
    std::shared_ptr<infer::Eval> evaluate(Fn& f);

    // What a module-level name refers to: ('fn', qname) ('class', qname) ('global', module, name)
    // ('module', qname) ('stdmod', name) ('extern', name), or None.
    std::optional<Ref> resolve_global(Module& m, const Str& name);
    std::optional<Ref> resolve_expr_name(Module& m, const A::Expr* node);
    Ref module_ref(const Str& name);                         // _module_ref
    Str from_base(const Module& m, const A::ImportFrom* st); // _from_base

    // -- containers
    int64_t find(int64_t s);
    // A fact that keeps growing round after round: its integer bounds widened (to thresholds), a type
    // that keeps nesting deeper given up -- so the rounds end.
    T settle(const T& old, const T& new_, int64_t depth = 0);
    // a fact changed: what read it runs again
    void touch(const FactKey& key);
    void read(const FactKey& key);
    T ret(const Str& fq);
    const std::set<Str>& fn_locals(const Fn& f);
    // the subscripts and calls of an expression or statement, outside nested scopes (cached)
    const std::vector<const A::Expr*>& access_nodes(const A::Node* node);
    // The container site of an AST node (the same node is the same site every round).
    int64_t new_site(const A::Node* node, const Str& kind, const std::optional<std::tuple<Str, int64_t>>& where = std::nullopt);
    int64_t union_(int64_t a, int64_t b);
    void mark_mutated(int64_t s);
    // Join `t` into a container fact; note a change. (Not while a loop is still being run to its fixpoint:
    // its heads are not final, and a widened bound would stick.)
    int64_t grow(std::vector<T>& table, int64_t s, const T& t);
    void grow_field(int64_t s, const Str& key, const T& t);
    void created(int64_t s, const StrSet& keys);
    void make_dynamic(int64_t s, const Str& why);
    bool is_record(int64_t s);
    void hint(int64_t s, const Str& name);

    // -- the join
    T join(const T& a, const T& b);
    std::optional<Str> common_base(const Str& x, const Str& y);
    std::vector<Str> mro(const Str& cq);
    std::vector<Str> descendants(const Str& cq);
    // `t` with every container named by its root site (for comparing facts between rounds)
    T canon(const T& t);

  private:
    void index_stmt(Module& m, const A::Stmt* st);
    T ann_(Module& m, const A::Expr* a);

    std::vector<std::unique_ptr<Module>> owned_modules_;
    std::vector<std::unique_ptr<Fn>> owned_fns_;
    std::vector<std::unique_ptr<Class>> owned_classes_;
    std::vector<std::unique_ptr<A::Module>> keep_;      // _keep: trees parsed from string annotations
    std::unordered_map<const void*, const std::vector<A::Stmt*>*> blocks_;
    std::unordered_map<Str, StrSet, py::str_hash> assigned_attrs_;
    std::unordered_map<Str, std::set<Str>, py::str_hash> fn_locals_;
    std::unordered_map<const void*, std::vector<const A::Expr*>> access_;
};

}  // namespace codebreaker::translate::frontends::python::program
