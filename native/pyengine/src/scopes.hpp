// scopes.hpp -- names to their bindings over pyfront's tree, by CPython's symtable rules.
//
// Scopes: the module, each class body, each function and lambda, each comprehension. A name bound
// in a scope (assignment, augmented assignment, for / with / except targets, import, def, class,
// del, a walrus -- which binds in the enclosing function, not the comprehension --, match captures,
// parameters) is local there unless the scope says `global` or `nonlocal` for it. A name used but
// not bound is free: it resolves to the nearest enclosing function scope that binds it (class
// bodies are skipped, as Python skips them), else to the module, else to the builtins. A function
// whose locals an inner scope reads holds them as cells -- what a closure captures.
//
//     Scopes sc; sc.build(*module_tree);
//     const Scope* f = sc.scope_of(funcdef);        // the scope a def / lambda / class / comprehension opens
//     NameRes r = sc.resolve(name_node);            // Local, Free (r.owner binds it), Global, Builtin
//     f->free, f->cells                             // a closure's captures; what inner scopes capture
#pragma once
#include <map>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#include "astutil.hpp"

namespace pyengine {

struct Scope {
    enum Kind { Module, Class, Function, Lambda, Comprehension } kind = Module;
    std::string name;                         // the def's / class's name; "<lambda>", "<listcomp>" ...
    AnyNode node;
    Scope* parent = nullptr;
    std::vector<Scope*> children;
    std::set<std::string> params, bound, used, globals, nonlocals, imports;
    std::set<std::string> free;               // used here (or by an inner scope), bound in an enclosing function
    std::set<std::string> cells;              // bound here, captured by an inner scope
    bool is_function() const { return kind == Function || kind == Lambda || kind == Comprehension; }
    bool binds(const std::string& n) const { return (bound.count(n) || params.count(n)) && !globals.count(n) && !nonlocals.count(n); }
};

struct NameRes {
    enum Kind { Unresolved, Local, Free, Global, Builtin } kind = Unresolved;
    const Scope* owner = nullptr;             // the scope that binds the name (Local: the using scope)
};

class Scopes {
public:
    void build(const A::Module& m);
    const Scope* module() const { return root_; }
    const Scope* scope_of(const void* node) const;          // a def, lambda, class or comprehension node
    const Scope* enclosing(const void* name_node) const;    // the scope a Name occurs in
    NameRes resolve(const A::Name* n) const;
    NameRes resolve_in(const Scope* s, const std::string& name) const;
    const std::vector<std::unique_ptr<Scope>>& all() const { return owned_; }
    static bool is_builtin(const std::string& name);
private:
    std::vector<std::unique_ptr<Scope>> owned_;
    Scope* root_ = nullptr;
    std::unordered_map<const void*, Scope*> opens_;          // node -> the scope it opens
    std::unordered_map<const void*, Scope*> where_;          // Name node -> its scope
    Scope* open(Scope::Kind k, const std::string& name, AnyNode node, Scope* parent);
    void visit_stmt(const A::Stmt* s, Scope* sc);
    void visit_expr(const A::Expr* e, Scope* sc);
    void visit_args(const A::arguments* a, Scope* outer, Scope* inner);
    void bind_target(const A::Expr* t, Scope* sc);
    void visit_comp(AnyNode node, const char* name, const std::vector<A::comprehension*>& gens, std::vector<const A::Expr*> elts, Scope* sc);
    void visit_pattern(const A::Pattern* p, Scope* sc);
    void analyze(Scope* s);
};

}  // namespace pyengine
