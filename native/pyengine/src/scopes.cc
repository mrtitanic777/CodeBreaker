// scopes.cc -- the scope pass (see scopes.hpp).
#include "scopes.hpp"

#include "pyfront/pyfront.hpp"

namespace pyengine {

using namespace pyfront;

bool Scopes::is_builtin(const std::string& name) {
    static const std::set<std::string> B = {
        "abs", "all", "any", "ascii", "bin", "bool", "breakpoint", "bytearray", "bytes", "callable", "chr", "classmethod", "compile",
        "complex", "delattr", "dict", "dir", "divmod", "enumerate", "eval", "exec", "filter", "float", "format", "frozenset", "getattr",
        "globals", "hasattr", "hash", "help", "hex", "id", "input", "int", "isinstance", "issubclass", "iter", "len", "list", "locals",
        "map", "max", "memoryview", "min", "next", "object", "oct", "open", "ord", "pow", "print", "property", "range", "repr",
        "reversed", "round", "set", "setattr", "slice", "sorted", "staticmethod", "str", "sum", "super", "tuple", "type", "vars", "zip",
        "__import__", "__name__", "__file__", "__doc__", "__build_class__", "NotImplemented", "Ellipsis", "True", "False", "None",
        "Exception", "BaseException", "ValueError", "KeyError", "IndexError", "TypeError", "RuntimeError", "OSError", "IOError",
        "LookupError", "ArithmeticError", "ZeroDivisionError", "OverflowError", "NotImplementedError", "AssertionError", "StopIteration",
        "EOFError", "FileNotFoundError", "AttributeError", "UnicodeDecodeError", "UnicodeEncodeError", "PermissionError", "TimeoutError",
        "UnicodeError", "SystemExit", "KeyboardInterrupt", "MemoryError", "RecursionError", "ImportError", "ModuleNotFoundError",
        "NameError", "IsADirectoryError", "NotADirectoryError", "FileExistsError", "ConnectionError", "BrokenPipeError",
        "InterruptedError", "ChildProcessError", "ProcessLookupError", "BlockingIOError", "GeneratorExit", "StopAsyncIteration",
        "Warning", "UserWarning", "DeprecationWarning", "RuntimeWarning", "SyntaxError", "BufferError", "EnvironmentError"};
    return B.count(name) != 0;
}

Scope* Scopes::open(Scope::Kind k, const std::string& name, AnyNode node, Scope* parent) {
    owned_.push_back(std::make_unique<Scope>());
    Scope* s = owned_.back().get();
    s->kind = k;
    s->name = name;
    s->node = node;
    s->parent = parent;
    if (parent) parent->children.push_back(s);
    if (node.p) opens_[node.p] = s;
    return s;
}

void Scopes::build(const A::Module& m) {
    owned_.clear();
    opens_.clear();
    where_.clear();
    root_ = open(Scope::Module, "<module>", AnyNode(), nullptr);
    for (auto* s : m.body) visit_stmt(s, root_);
    analyze(root_);
}

const Scope* Scopes::scope_of(const void* node) const {
    auto it = opens_.find(node);
    return it == opens_.end() ? nullptr : it->second;
}
const Scope* Scopes::enclosing(const void* name_node) const {
    auto it = where_.find(name_node);
    return it == where_.end() ? nullptr : it->second;
}

void Scopes::bind_target(const A::Expr* t, Scope* sc) {
    if (!t) return;
    switch (t->kind) {
        case ExprKind::Name:
            sc->bound.insert(cast<pyfront::Name>(t)->id);
            where_[t] = sc;
            break;
        case ExprKind::Tuple:
            for (auto* e : cast<pyfront::Tuple>(t)->elts) bind_target(e, sc);
            break;
        case ExprKind::List:
            for (auto* e : cast<pyfront::List>(t)->elts) bind_target(e, sc);
            break;
        case ExprKind::Starred: bind_target(cast<Starred>(t)->value, sc); break;
        default: visit_expr(t, sc);   // attribute / subscript targets read their object
    }
}

void Scopes::visit_args(const A::arguments* a, Scope* outer, Scope* inner) {
    if (!a) return;
    for (auto* d : a->defaults) visit_expr(d, outer);
    for (auto* d : a->kw_defaults)
        if (d) visit_expr(d, outer);
    auto ann = [&](const arg* x) {
        if (x && x->annotation) visit_expr(x->annotation, outer);
    };
    for (auto* x : a->posonlyargs) {
        inner->params.insert(x->arg);
        ann(x);
    }
    for (auto* x : a->args) {
        inner->params.insert(x->arg);
        ann(x);
    }
    if (a->vararg) {
        inner->params.insert(a->vararg->arg);
        ann(a->vararg);
    }
    for (auto* x : a->kwonlyargs) {
        inner->params.insert(x->arg);
        ann(x);
    }
    if (a->kwarg) {
        inner->params.insert(a->kwarg->arg);
        ann(a->kwarg);
    }
}

void Scopes::visit_comp(AnyNode node, const char* name, const std::vector<A::comprehension*>& gens, std::vector<const A::Expr*> elts, Scope* sc) {
    // the first iterable is evaluated in the enclosing scope
    if (!gens.empty()) visit_expr(gens[0]->iter, sc);
    Scope* c = open(Scope::Comprehension, name, node, sc);
    for (size_t i = 0; i < gens.size(); i++) {
        if (i > 0) visit_expr(gens[i]->iter, c);
        bind_target(gens[i]->target, c);
        for (auto* cond : gens[i]->ifs) visit_expr(cond, c);
    }
    for (auto* e : elts) visit_expr(e, c);
}

void Scopes::visit_pattern(const A::Pattern* p, Scope* sc) {
    if (!p) return;
    switch (p->kind) {
        case PatternKind::MatchValue: visit_expr(cast<MatchValue>(p)->value, sc); break;
        case PatternKind::MatchSequence:
            for (auto* x : cast<MatchSequence>(p)->patterns) visit_pattern(x, sc);
            break;
        case PatternKind::MatchMapping: {
            auto* m = cast<MatchMapping>(p);
            for (auto* k : m->keys) visit_expr(k, sc);
            for (auto* x : m->patterns) visit_pattern(x, sc);
            if (m->has_rest) sc->bound.insert(m->rest);
            break;
        }
        case PatternKind::MatchClass: {
            auto* m = cast<MatchClass>(p);
            visit_expr(m->cls, sc);
            for (auto* x : m->patterns) visit_pattern(x, sc);
            for (auto* x : m->kwd_patterns) visit_pattern(x, sc);
            break;
        }
        case PatternKind::MatchStar:
            if (cast<MatchStar>(p)->has_name) sc->bound.insert(cast<MatchStar>(p)->name);
            break;
        case PatternKind::MatchAs: {
            auto* m = cast<MatchAs>(p);
            visit_pattern(m->pattern, sc);
            if (m->has_name) sc->bound.insert(m->name);
            break;
        }
        case PatternKind::MatchOr:
            for (auto* x : cast<MatchOr>(p)->patterns) visit_pattern(x, sc);
            break;
        default: break;
    }
}

void Scopes::visit_stmt(const A::Stmt* s, Scope* sc) {
    switch (s->kind) {
        case StmtKind::FunctionDef:
        case StmtKind::AsyncFunctionDef: {
            auto* f = cast<FunctionDef>(s);
            for (auto* d : f->decorator_list) visit_expr(d, sc);
            if (f->returns) visit_expr(f->returns, sc);
            sc->bound.insert(f->name);
            Scope* fs = open(Scope::Function, f->name, AnyNode(s), sc);
            visit_args(f->args, sc, fs);
            for (auto* b : f->body) visit_stmt(b, fs);
            return;
        }
        case StmtKind::ClassDef: {
            auto* c = cast<ClassDef>(s);
            for (auto* d : c->decorator_list) visit_expr(d, sc);
            for (auto* b : c->bases) visit_expr(b, sc);
            for (auto* k : c->keywords) visit_expr(k->value, sc);
            sc->bound.insert(c->name);
            Scope* cs = open(Scope::Class, c->name, AnyNode(s), sc);
            for (auto* b : c->body) visit_stmt(b, cs);
            return;
        }
        case StmtKind::Global:
            for (auto& n : cast<Global>(s)->names) sc->globals.insert(n);
            return;
        case StmtKind::Nonlocal:
            for (auto& n : cast<Nonlocal>(s)->names) sc->nonlocals.insert(n);
            return;
        case StmtKind::Import:
            for (auto* a : cast<Import>(s)->names) {
                std::string l = a->has_asname ? a->asname : a->name.substr(0, a->name.find('.'));
                sc->bound.insert(l);
                sc->imports.insert(l);
            }
            return;
        case StmtKind::ImportFrom:
            for (auto* a : cast<ImportFrom>(s)->names) {
                if (a->name == "*") continue;
                std::string l = a->has_asname ? a->asname : a->name;
                sc->bound.insert(l);
                sc->imports.insert(l);
            }
            return;
        case StmtKind::Assign: {
            auto* a = cast<pyfront::Assign>(s);
            visit_expr(a->value, sc);
            for (auto* t : a->targets) bind_target(t, sc);
            return;
        }
        case StmtKind::AugAssign: {
            auto* a = cast<AugAssign>(s);
            visit_expr(a->value, sc);
            if (auto* n = as<pyfront::Name>(a->target)) sc->used.insert(n->id);
            bind_target(a->target, sc);
            return;
        }
        case StmtKind::AnnAssign: {
            auto* a = cast<AnnAssign>(s);
            if (a->value) visit_expr(a->value, sc);
            visit_expr(a->annotation, sc);
            if (a->value || as<pyfront::Name>(a->target)) bind_target(a->target, sc);
            return;
        }
        case StmtKind::For:
        case StmtKind::AsyncFor: {
            auto* f = cast<pyfront::For>(s);
            visit_expr(f->iter, sc);
            bind_target(f->target, sc);
            for (auto* b : f->body) visit_stmt(b, sc);
            for (auto* b : f->orelse) visit_stmt(b, sc);
            return;
        }
        case StmtKind::With:
        case StmtKind::AsyncWith: {
            auto* w = cast<With>(s);
            for (auto* it : w->items) {
                visit_expr(it->context_expr, sc);
                if (it->optional_vars) bind_target(it->optional_vars, sc);
            }
            for (auto* b : w->body) visit_stmt(b, sc);
            return;
        }
        case StmtKind::Try:
        case StmtKind::TryStar: {
            auto* t = cast<pyfront::Try>(s);
            for (auto* b : t->body) visit_stmt(b, sc);
            for (auto* h : t->handlers) {
                if (h->type) visit_expr(h->type, sc);
                if (h->has_name) sc->bound.insert(h->name);
                for (auto* b : h->body) visit_stmt(b, sc);
            }
            for (auto* b : t->orelse) visit_stmt(b, sc);
            for (auto* b : t->finalbody) visit_stmt(b, sc);
            return;
        }
        case StmtKind::Delete:
            for (auto* t : cast<Delete>(s)->targets) bind_target(t, sc);
            return;
        case StmtKind::Match: {
            auto* m = cast<Match>(s);
            visit_expr(m->subject, sc);
            for (auto* c : m->cases) {
                visit_pattern(c->pattern, sc);
                if (c->guard) visit_expr(c->guard, sc);
                for (auto* b : c->body) visit_stmt(b, sc);
            }
            return;
        }
        default: break;
    }
    // every other statement: its expressions and nested statements
    std::vector<AnyNode> kids;
    child_nodes(AnyNode(s), kids);
    for (auto& k : kids) {
        if (k.expr()) visit_expr(k.expr(), sc);
        else if (k.stmt()) visit_stmt(k.stmt(), sc);
    }
}

void Scopes::visit_expr(const A::Expr* e, Scope* sc) {
    if (!e) return;
    switch (e->kind) {
        case ExprKind::Name: {
            auto* n = cast<pyfront::Name>(e);
            where_[e] = sc;
            if (n->ctx == ExprCtx::Load) sc->used.insert(n->id);
            else sc->bound.insert(n->id);
            return;
        }
        case ExprKind::NamedExpr: {
            auto* w = cast<NamedExpr>(e);
            visit_expr(w->value, sc);
            Scope* target = sc;   // a walrus in a comprehension binds in the enclosing function
            while (target->kind == Scope::Comprehension && target->parent) target = target->parent;
            if (auto* n = as<pyfront::Name>(w->target)) {
                target->bound.insert(n->id);
                where_[w->target] = target;
                if (target != sc) sc->nonlocals.insert(n->id);
            }
            return;
        }
        case ExprKind::Lambda: {
            auto* l = cast<pyfront::Lambda>(e);
            Scope* ls = open(Scope::Lambda, "<lambda>", AnyNode(e), sc);
            visit_args(l->args, sc, ls);
            visit_expr(l->body, ls);
            return;
        }
        case ExprKind::ListComp: visit_comp(AnyNode(e), "<listcomp>", cast<ListComp>(e)->generators, {cast<ListComp>(e)->elt}, sc); return;
        case ExprKind::SetComp: visit_comp(AnyNode(e), "<setcomp>", cast<SetComp>(e)->generators, {cast<SetComp>(e)->elt}, sc); return;
        case ExprKind::GeneratorExp: visit_comp(AnyNode(e), "<genexpr>", cast<GeneratorExp>(e)->generators, {cast<GeneratorExp>(e)->elt}, sc); return;
        case ExprKind::DictComp:
            visit_comp(AnyNode(e), "<dictcomp>", cast<DictComp>(e)->generators, {cast<DictComp>(e)->key, cast<DictComp>(e)->value}, sc);
            return;
        default: break;
    }
    std::vector<AnyNode> kids;
    child_nodes(AnyNode(e), kids);
    for (auto& k : kids) {
        if (k.expr()) visit_expr(k.expr(), sc);
        else if (k.cat == AnyNode::Keyword) visit_expr(static_cast<const keyword*>(k.p)->value, sc);
        else if (k.cat == AnyNode::Arguments) visit_args(static_cast<const arguments*>(k.p), sc, sc);
    }
}

NameRes Scopes::resolve_in(const Scope* s, const std::string& name) const {
    NameRes r;
    if (!s) return r;
    if (s->globals.count(name)) {
        r.kind = NameRes::Global;
        r.owner = root_;
        return r;
    }
    if (s->nonlocals.count(name) || (!s->binds(name) && s->kind != Scope::Module)) {
        // free: the nearest enclosing function scope binding it (classes skipped)
        for (const Scope* up = s->parent; up; up = up->parent) {
            if (up->kind == Scope::Class) continue;
            if (up->kind == Scope::Module) break;
            if (up->binds(name)) {
                r.kind = NameRes::Free;
                r.owner = up;
                return r;
            }
            if (up->globals.count(name)) break;
        }
        if (s->nonlocals.count(name)) return r;   // a nonlocal with no binding: unresolved (a syntax error in Python)
        if (root_->binds(name)) {
            r.kind = NameRes::Global;
            r.owner = root_;
            return r;
        }
        if (is_builtin(name)) {
            r.kind = NameRes::Builtin;
            return r;
        }
        return r;
    }
    if (s->binds(name)) {
        r.kind = s->kind == Scope::Module ? NameRes::Global : NameRes::Local;
        r.owner = s;
        return r;
    }
    if (is_builtin(name)) r.kind = NameRes::Builtin;
    return r;
}

NameRes Scopes::resolve(const A::Name* n) const { return resolve_in(enclosing(n), n->id); }

void Scopes::analyze(Scope* s) {
    for (Scope* c : s->children) analyze(c);
    if (s->kind == Scope::Module) return;
    // names this scope (or an inner one) needs from outside
    std::set<std::string> want = s->used;
    want.insert(s->nonlocals.begin(), s->nonlocals.end());
    for (Scope* c : s->children)
        for (auto& f : c->free)
            if (!s->binds(f) || s->kind == Scope::Class) want.insert(f);
    for (auto& name : want) {
        if (s->kind != Scope::Class && s->binds(name) && !s->nonlocals.count(name)) continue;
        NameRes r = resolve_in(s, name);
        if (r.kind == NameRes::Free) {
            s->free.insert(name);
            const_cast<Scope*>(r.owner)->cells.insert(name);
        }
    }
}

}  // namespace pyengine
