// program.hpp -- the modules being translated, their symbols, and the facts inference gathers about
// them (the old front end's program.py).
//
//   params[fn]    each parameter's type: its annotation, else the join of every call site's argument
//   returns[fn]   the join of every value it returns
//   attrs[cls]    each attribute: the join of every value stored to it
//   globals[mod]  each module-level name
//   containers    per container object (sites joined by union-find): a list's or set's element, a
//                 dict's keys and values, a record's fields and which fields every creation sets
#pragma once
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "astutil.hpp"
#include "pytypes.hpp"

namespace pyengine {

// A dict with Python's order: insertion order kept, a key set again keeps its first place.
template <class V> class OMap {
public:
    std::vector<std::pair<std::string, V>> items;
    V* get(const std::string& k) {
        auto it = idx_.find(k);
        return it == idx_.end() ? nullptr : &items[it->second].second;
    }
    const V* get(const std::string& k) const {
        auto it = idx_.find(k);
        return it == idx_.end() ? nullptr : &items[it->second].second;
    }
    bool has(const std::string& k) const { return idx_.count(k) != 0; }
    V& operator[](const std::string& k) {
        auto it = idx_.find(k);
        if (it != idx_.end()) return items[it->second].second;
        idx_[k] = items.size();
        items.emplace_back(k, V());
        return items.back().second;
    }
    void set(const std::string& k, V v) { (*this)[k] = std::move(v); }
    size_t size() const { return items.size(); }
    bool empty() const { return items.empty(); }
    auto begin() { return items.begin(); }
    auto end() { return items.end(); }
    auto begin() const { return items.begin(); }
    auto end() const { return items.end(); }
private:
    std::unordered_map<std::string, size_t> idx_;
};

// A resolution: what a name, attribute or call refers to, as the old front end's tuples
// ('fn', qname), ('field', cls, attr), ('tuple_index', None) ...  `a`..`c` unset = None.
struct Res {
    std::string k;                       // empty: no resolution
    std::optional<std::string> a, b, c;
    int n = 0;                           // the tuple's length
    Res() = default;
    explicit Res(std::string k_) : k(std::move(k_)), n(1) {}
    Res(std::string k_, std::optional<std::string> a_) : k(std::move(k_)), a(std::move(a_)), n(2) {}
    Res(std::string k_, std::optional<std::string> a_, std::optional<std::string> b_)
        : k(std::move(k_)), a(std::move(a_)), b(std::move(b_)), n(3) {}
    Res(std::string k_, std::optional<std::string> a_, std::optional<std::string> b_, std::optional<std::string> c_)
        : k(std::move(k_)), a(std::move(a_)), b(std::move(b_)), c(std::move(c_)), n(4) {}
    explicit operator bool() const { return !k.empty(); }
    bool is(const char* kk, const char* aa) const { return k == kk && a && *a == aa && n == 2; }
    std::string A() const { return a.value_or(""); }
    std::string B() const { return b.value_or(""); }
};

struct Issue {
    std::string cls, detail;
    int line = 0;
    std::string reason() const { return cls + (detail.empty() ? "" : ":" + detail); }
};

struct Module;
struct Class;

struct Fn {
    std::string qname, name;
    const A::FunctionDef* node = nullptr;
    Module* module = nullptr;
    Class* cls = nullptr;
    Fn* parent = nullptr;                // (new) the function a nested function is defined in
    std::string kind = "function";       // function method static classmethod property async
    std::vector<std::string> decorators, params, kwonly;
    OMap<const A::Expr*> annotations;
    OMap<const A::Expr*> defaults;
    std::string varargs, kwargs;         // empty: none
    int lines = 0;
    std::set<std::string> none_aware;
    std::set<std::string> index_params;
    bool generator = false;              // (new) the body yields
    bool has_nested = false;             // (new) it defines nested functions
    Fn* spec_of = nullptr;               // (new) a specialization: the function it is a copy of
    std::string spec_sig;                //       and the kinds of the arguments it is for
    std::vector<std::string> bound() const;
    Fn(std::string q, const A::FunctionDef* n, Module* m, Class* c);
};

struct Class {
    std::string qname, name;
    const A::ClassDef* node = nullptr;
    Module* module = nullptr;
    std::vector<std::string> bases;
    std::string exception;               // the builtin exception it derives from
    OMap<Fn*> methods;
    OMap<const A::Expr*> class_attrs;
    std::vector<std::string> subclasses;
    std::vector<std::string> decorators;
    std::string foreign_base;            // a base the program does not have
};

// an import or a module-level name's resolution
struct Ref {
    std::string k;                       // module obj extern stdmod fn class global stdobj missing builtin exc const
    std::string a, b;
    explicit operator bool() const { return !k.empty(); }
};

struct Module {
    std::string name, path, source;
    std::unique_ptr<A::Module> tree;
    std::vector<size_t> lines;           // line starts (source segments)
    OMap<Fn*> functions;
    OMap<Class*> classes;
    OMap<std::vector<const A::Expr*>> assigns;   // nullptr: an unpacking
    OMap<Ref> imports;
    std::vector<const A::Stmt*> statements;
    std::set<std::string> shadowed;      // a def's name an import after it rebinds: the name is the import's
    bool is_package = false;
    bool has_docstring = false;
};

extern const std::set<std::string> STDLIB, EXCEPTIONS;

class Program {
public:
    Program();
    ~Program();
    Program(const Program&) = delete;
    Program& operator=(const Program&) = delete;
    // loading
    Module* add_module(const std::string& name, const std::string& path, const std::string& source, std::string* err = nullptr);
    void add_tree(const std::string& root, const std::string& package);
    void index();

    std::vector<std::unique_ptr<Module>> owned_modules;
    std::vector<std::unique_ptr<Fn>> owned_fns;
    std::vector<std::unique_ptr<Class>> owned_classes;
    OMap<Module*> modules;
    OMap<Fn*> fns;
    OMap<Class*> classes;

    // facts
    std::unordered_map<std::string, std::unordered_map<std::string, T>> params;
    std::unordered_map<std::string, T> returns;
    std::unordered_map<std::string, OMap<T>> attrs;
    std::unordered_map<std::string, std::unordered_map<std::string, T>> globals;
    // containers (sites from 1; index 0 unused)
    std::vector<int> parent;
    std::vector<TK> ckind;
    std::vector<T> elem, keys;
    std::vector<OMap<T>> fields;
    std::vector<std::optional<std::set<std::string>>> required;
    std::unordered_map<int, std::string> dynamic;
    std::unordered_map<int, std::vector<std::string>> hints;
    std::unordered_map<const void*, int> site_of_node;
    std::vector<std::pair<std::string, int>> site_where;
    std::vector<const void*> site_node;
    std::unordered_set<int> fixed, mutated;
    std::unordered_map<int, std::string> poisoned_sites;
    std::unordered_map<const void*, std::vector<std::string>> literal_keys;
    std::unordered_map<int, std::set<std::string>> spread_keys;
    std::unordered_map<std::string, T> class_attr_types;   // "cls\x1fname"
    std::unordered_map<std::string, T> default_types;      // "fn\x1fparam"
    std::unordered_map<const void*, T> ann_cache;
    std::unordered_map<std::string, std::string> crashes;
    std::unordered_set<const void*> builder_targets;
    std::map<std::pair<std::string, std::string>, std::pair<std::string, std::string>> param_poison;
    std::map<std::pair<std::string, std::string>, std::string> poisoned;
    std::string int_policy = "strict";
    int round = 0;
    bool changed = false;
    int suspend = 0;
    std::unordered_set<std::string>* reads = nullptr;
    std::unordered_set<std::string> dirty;

    // type lookups
    T annotation_type(Fn* f, const std::string& name);
    T ann_to_type(Module* m, const A::Expr* a);
    T param_type(Fn* f, const std::string& name);
    T demand_type(Fn* f, const std::string& name);      // (new) demand.cc
    bool defines_operator(const std::string& op);      // (new) demand.cc: a class of the program has __op__ / __rop__
    T global_type(const std::string& mod, const std::string& name);
    Fn* find_method(const std::string& cq, const std::string& name);
    const std::set<std::string>& assigned_attrs(const std::string& cq);
    std::set<std::string> assigned_attrs_any(const std::string& cq);
    std::string attr_owner(const std::string& cq, const std::string& attr);      // empty: none
    std::optional<T> attr_type(const std::string& cq, const std::string& attr);  // none: no such attribute
    T class_attr_type(const std::string& cq, const std::string& attr);
    const std::vector<A::Stmt*>* block_of(const A::Stmt* st) const;
    void map_blocks(const A::FunctionDef* node);

    // solving
    int solve(int max_rounds = 40, bool log = false);

    Ref resolve_global(Module* m, const std::string& name);
    Ref resolve_expr_name(Module* m, const A::Expr* node);
    Ref module_ref(const std::string& name);
    std::string from_base(Module* m, const A::ImportFrom* st);

    // containers
    int find(int s);
    T settle(const T& old, const T& nw, int depth = 0);
    void touch(const std::string& key);
    void read(const std::string& key);
    T ret(const std::string& fq);
    const std::set<std::string>& fn_locals(Fn* f);
    const std::vector<AnyNode>& access_nodes(AnyNode node);
    int new_site(const void* node, TK kind, std::pair<std::string, int> where);
    int unite(int a, int b);
    void mark_mutated(int s);
    void grow(std::vector<T>& table, int s, const T& t);
    void grow_field(int s, const std::string& key, const T& t);
    void created(int s, const std::vector<std::string>& keys);
    void make_dynamic(int s, const std::string& why);
    bool is_record(int s);
    void hint(int s, const std::string& name);

    T join(const T& a, const T& b);
    T variant_join(const T& a, const T& b);   // (new)
    int no_variants = 0;                      // > 0: the classic join (where a clash is a signal, not a refusal)
    std::string common_base(const std::string& x, const std::string& y);
    std::vector<std::string> mro(const std::string& cq);
    std::vector<std::string> descendants(const std::string& cq);
    T canon(const T& t);

    // (new) the engine's extensions; off reproduces the old engine exactly
    bool extended = true;
    // (new) contracts: the facts the classic rules settle are frozen before the demand-typed code
    // runs; a contribution that would change one is vetoed and refused where it is made
    bool demand_enabled = false;
    bool frozen = false;
    int frozen_sites = 0;
    std::unordered_map<std::string, std::unordered_map<std::string, T>> frozen_params;
    std::unordered_map<std::string, OMap<T>> frozen_attrs;
    std::vector<std::string> vetoes;
    void freeze();
    // (new) hand-written type hints (native/pyengine/hints/types.json): function -> parameter -> a
    // Python annotation's text; applied when the classic rules have settled, to parameters they left
    // untyped, as if the source had the annotation
    std::map<std::string, std::map<std::string, std::string>> type_hints;
    // (new) functions a native format answers (answers.cc): qualified name -> the question; their return
    // type is the answer's, fixed before solving and never replaced by their (empty) bodies
    std::map<std::string, std::string> native_bound;
    std::map<std::string, std::string> native_where;   // its place function (ask(q, _where)): called with self, spread after the bytes
    struct NativeArgsOf {   // args_of=f inlined: the bytes and the place, f's expressions of the stub's first parameter
        const pyfront::Expr* data = nullptr;
        std::vector<const pyfront::Expr*> place;
    };
    std::map<std::string, NativeArgsOf> native_args_of;
    std::map<std::string, std::string> native_data;   // where its call's bytes come from: "self:<attr>", "arg:<i>", "none"
    std::vector<std::unique_ptr<pyfront::Module>> hint_trees;
    int apply_type_hints();
    // (new) MonkeyType-style .pyi stubs (--stubs DIR): the recorded parameter types of a run of the
    // package, read as hints. Applied after types.json, to parameters the classic rules left untyped;
    // where the rules typed one, inference wins and a differing stub is reported as a conflict; a stub
    // type the engine cannot model is ignored with the reason.
    struct StubNote { std::string fn, param, stub, status, detail; };   // status: applied | agree | conflict | ignored
    std::map<std::string, std::map<std::string, std::string>> stub_hints;   // function -> parameter -> annotation text
    std::vector<StubNote> stub_notes;
    int stub_files = 0;
    int apply_stub_hints();
    void load_stub(const std::string& module, const std::string& source);
    // (new) a method reached by getattr(self, "x_" + type(e).__name__)(e): its parameter is an instance of the class
    // its name ends in (x_Unary: ir.Unary) -- the dispatch's key is that class's name
    std::map<std::string, std::pair<std::string, T>> dispatch_param;
    std::map<std::pair<std::string, std::string>, std::pair<Fn*, std::string>> dispatch_alias;   // (method, param) -> its dispatcher's param
    void dispatch_hints();
    // (new) getattr(obj, 'name'[, default]) with a constant name: 1 the attribute is always there (a
    // method, a class attribute, or set at the top of __init__ in obj's class or a base), 2 it is never
    // there (no class of the hierarchy, up or down, has or stores it), 0 it may or may not be
    int getattr_choice(const std::string& cq, const std::string& attr);
    const pyfront::Attribute* getattr_attr(const pyfront::Call* n, const std::string& attr);   // obj.name, made once
    std::unordered_map<const void*, std::unique_ptr<pyfront::Attribute>> getattr_synth;
    // (new) nested functions: kept beside fns (not in it: the old tables stay as they were)
    OMap<Fn*> nested;
    std::unordered_map<const void*, Fn*> nested_by_node;
    std::unordered_map<std::string, std::unordered_map<std::string, T>> closure_vars;   // definer -> name -> every value bound
    T closure_var(Fn* f, const std::string& name);
    Fn* any_fn(const std::string& q);
    // (new) per-caller specialization: a function whose callers pass kinds that do not join is
    // copied per argument signature (written as C++ overloads of one name)
    std::set<std::string> spec_candidates;
    OMap<Fn*> specs;
    std::map<std::string, std::vector<Fn*>> specs_of;
    Fn* spec_for(Fn* f, const std::string& sig);
    void index_fn(Fn* f);
    void index_nested(Fn* parent);
    std::unordered_set<int> unfrozen;     // frozen-era sites whose items had no single type: recomputed
    // (new) sites fixed from the start: a native format's answers (answers.cc) -- code that would change one
    // (add or pop a key, use it as a dict) is refused, the answer's type stays what the format gives
    std::unordered_set<int> pinned;
    bool site_frozen(int root) const { return pinned.count(root) || (frozen && root < frozen_sites && !unfrozen.count(root)); }
    void veto(const std::string& why) { vetoes.push_back(why); }

private:
    void index_stmt(Module* m, const A::Stmt* st);
    T ann(Module* m, const A::Expr* a);
    std::unordered_map<std::string, std::set<std::string>> assigned_attrs_;
    std::unordered_map<std::string, std::set<std::string>> fn_locals_;
    std::unordered_map<const void*, std::vector<AnyNode>> access_;
    std::unordered_map<const void*, const std::vector<A::Stmt*>*> blocks_;
    std::vector<std::unique_ptr<A::Module>> ann_trees_;   // string annotations, parsed
};

std::string tname(const T& t);
bool variant_ok(const T& t);
std::set<std::string> assigned_names(const A::FunctionDef* fn);
std::vector<std::string> target_names(const A::Expr* t);
std::string deco_name(const A::Expr* d);

// answers.cc: the @native.ask / @native.answered functions typed by native/formats/**/questions.cc;
// how many were bound
size_t bind_native_answers(Program& p, const std::string& formats_dir, FILE* log);
// the type of one call's answer (sites of its own; the same on every round)
T native_answer_at(Program& p, const std::string& fq, const void* call);
// native.call("q", ...)'s type: the question's registered answer (BOTTOM if it has none)
T native_question_type(Program& p, const std::string& question);

}  // namespace pyengine
