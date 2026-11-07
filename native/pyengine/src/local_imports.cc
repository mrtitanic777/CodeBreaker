// local_imports.cc -- a function's own imports (`from codebreaker.base import settings` inside a def, the
// program's way around import cycles) seen as the module's, so the names resolve as they would at module
// level: the typed path reads the module's imports, and a name a function imports for itself was a local of
// no type (its calls refused as dynamic).
//
// An import inside a function (at any depth of its statements, not in a nested def or class) moves to the
// module's top when every name it binds
//   - is bound nowhere else in that function (no assignment, parameter, loop variable, other import), and
//   - is bound at module level by nothing, or by the same import (the same module, name and level).
// It is then the same name for the same object; only when the import runs differs, which the typed C++ does
// not see (its modules are all there). Identical imports of several functions move once. The Python
// source is not changed: this is the translator's view of it.
#include <map>
#include <set>
#include <string>
#include <vector>

#include "astutil.hpp"
#include "pyfront/ast.hpp"

namespace pyengine {

using namespace pyfront;

namespace {

// what an import binds: name -> a key naming exactly what it is bound to
void bound_by(const Stmt* st, std::vector<std::pair<std::string, std::string>>& out) {
    if (auto* im = as<Import>(st)) {
        for (auto* a : im->names) {
            if (a->has_asname) out.push_back({a->asname, "import " + a->name + " as " + a->asname});
            else out.push_back({a->name.substr(0, a->name.find('.')), "import " + a->name.substr(0, a->name.find('.'))});
        }
    } else if (auto* f = as<ImportFrom>(st)) {
        for (auto* a : f->names) {
            if (a->name == "*") {
                out.push_back({"*", "*"});
                continue;
            }
            std::string n = a->has_asname ? a->asname : a->name;
            out.push_back({n, "from " + std::string(f->level, '.') + f->module + " import " + a->name + " as " + n});
        }
    }
}

// every name a function binds, how many times (imports counted apart), its parameters among them
void binds_in(const std::vector<Stmt*>& body, std::map<std::string, int>& n, std::vector<Stmt*>& imports) {
    for (auto* st : body) {
        if (st->kind == StmtKind::Import || st->kind == StmtKind::ImportFrom) {
            imports.push_back(st);
            continue;
        }
        if (st->kind == StmtKind::FunctionDef || st->kind == StmtKind::AsyncFunctionDef) {
            n[static_cast<FunctionDef*>(st)->name]++;
            continue;
        }
        if (auto* c = as<ClassDef>(st)) {
            n[c->name]++;
            continue;
        }
        if (st->kind == StmtKind::Global || st->kind == StmtKind::Nonlocal) n["*"]++;   // (names shared with another scope: left alone)
        for (const AnyNode& x : walk(AnyNode(st))) {
            if (auto* nm = as<Name>(x.expr()); nm && nm->ctx != ExprCtx::Load) n[nm->id]++;
            if (x.cat == AnyNode::Handler) {
                auto* h = static_cast<const ExceptHandler*>(x.p);
                if (h->has_name) n[h->name]++;
            }
        }
        for (auto* l : stmt_lists(st)) binds_in(*const_cast<std::vector<Stmt*>*>(l), n, imports);
    }
}

void count_params(const arguments* a, std::map<std::string, int>& n) {
    if (!a) return;
    for (auto* x : a->posonlyargs) n[x->arg]++;
    for (auto* x : a->args) n[x->arg]++;
    for (auto* x : a->kwonlyargs) n[x->arg]++;
    if (a->vararg) n[a->vararg->arg]++;
    if (a->kwarg) n[a->kwarg->arg]++;
}

// replace the statement st (somewhere in body's statement lists) by `pass`
bool remove_stmt(Module& tree, std::vector<Stmt*>& body, const Stmt* st) {
    for (auto*& s : body) {
        if (s == st) {
            auto* p = tree.make<Pass>();
            p->pos = s->pos;
            s = p;
            return true;
        }
        if (s->kind == StmtKind::FunctionDef || s->kind == StmtKind::AsyncFunctionDef || s->kind == StmtKind::ClassDef) continue;
        for (auto* l : stmt_lists(s))
            if (remove_stmt(tree, *const_cast<std::vector<Stmt*>*>(l), st)) return true;
    }
    return false;
}

}  // namespace

void hoist_local_imports(Module& tree) {
    // the module level's bindings: names -> what binds them ("" for anything but an import)
    std::map<std::string, std::set<std::string>> module_binds;
    for (auto* st : tree.body) {
        std::vector<std::pair<std::string, std::string>> b;
        bound_by(st, b);
        for (auto& [n, key] : b) module_binds[n].insert(key);
        if (st->kind == StmtKind::Import || st->kind == StmtKind::ImportFrom) continue;
        if (st->kind == StmtKind::FunctionDef || st->kind == StmtKind::AsyncFunctionDef) module_binds[static_cast<FunctionDef*>(st)->name].insert("");
        else if (auto* c = as<ClassDef>(st)) module_binds[c->name].insert("");
        else
            for (const AnyNode& x : walk(AnyNode(st)))
                if (auto* nm = as<Name>(x.expr()); nm && nm->ctx != ExprCtx::Load) module_binds[nm->id].insert("");
    }
    // nested imports at module level (in an if / try) bind too
    for (auto* st : tree.body)
        if (st->kind != StmtKind::FunctionDef && st->kind != StmtKind::AsyncFunctionDef && st->kind != StmtKind::ClassDef)
            for (auto* l : stmt_lists(st))
                for (auto* s2 : *l) {
                    std::vector<std::pair<std::string, std::string>> b;
                    bound_by(s2, b);
                    for (auto& [n, key] : b) module_binds[n].insert("");
                }
    std::vector<FunctionDef*> fns;
    for (auto* st : tree.body) {
        if (st->kind == StmtKind::FunctionDef || st->kind == StmtKind::AsyncFunctionDef) fns.push_back(static_cast<FunctionDef*>(st));
        else if (auto* c = as<ClassDef>(st))
            for (auto* s : c->body)
                if (s->kind == StmtKind::FunctionDef || s->kind == StmtKind::AsyncFunctionDef) fns.push_back(static_cast<FunctionDef*>(s));
    }
    std::vector<Stmt*> hoisted;
    std::set<std::string> done;       // the keys moved already
    for (FunctionDef* f : fns) {
        std::map<std::string, int> n;
        std::vector<Stmt*> imports;
        count_params(f->args, n);
        binds_in(f->body, n, imports);
        if (n.count("*")) continue;
        std::map<std::string, int> by_import;
        for (auto* st : imports) {
            std::vector<std::pair<std::string, std::string>> b;
            bound_by(st, b);
            for (auto& [nm, key] : b) by_import[nm]++;
        }
        for (auto* st : imports) {
            std::vector<std::pair<std::string, std::string>> b;
            bound_by(st, b);
            bool ok = !b.empty();
            for (auto& [nm, key] : b) {
                if (nm == "*" || n.count(nm) || by_import[nm] != 1) ok = false;
                auto mb = module_binds.find(nm);
                if (mb != module_binds.end() && (mb->second.size() != 1 || *mb->second.begin() != key)) ok = false;
            }
            if (!ok) continue;
            remove_stmt(tree, f->body, st);
            bool all_done = true;
            for (auto& [nm, key] : b) {
                all_done = all_done && (done.count(key) || module_binds.count(nm));
                module_binds[nm].insert(key);
            }
            if (!all_done) hoisted.push_back(st);
            for (auto& [nm, key] : b) done.insert(key);
        }
        if (f->body.empty()) f->body.push_back(tree.make<Pass>());
    }
    if (hoisted.empty()) return;
    // after the docstring and the __future__ imports
    size_t at = 0;
    if (!tree.body.empty())
        if (auto* es = as<ExprStmt>(tree.body[0]))
            if (auto* c = as<Constant>(es->value); c && c->value.kind == ConstValue::Kind::Str) at = 1;
    while (at < tree.body.size()) {
        auto* f = as<ImportFrom>(tree.body[at]);
        if (!f || f->module != "__future__") break;
        at++;
    }
    tree.body.insert(tree.body.begin() + (long long)at, hoisted.begin(), hoisted.end());
}

}  // namespace pyengine
