// callbacks.cc -- a function passed where the typed path takes a one-argument lambda, seen as that lambda:
//
//     re.sub(r"\[(\d+)\]", one, text)          ->  re.sub(r"\[(\d+)\]", lambda m: one(m), text)
//     PATTERN.sub(one, text)                    ->  PATTERN.sub(lambda m: one(m), text)
//     sorted(xs, key=_rank)                     ->  sorted(xs, key=lambda m: _rank(m))
//
// (re.sub / re.subn of the re module, .sub / .subn of a module-level `P = re.compile(...)`, the key= of the
// builtins sorted / min / max and of a .sort.) The function is a def taking one argument (any others have
// defaults): a nested def of the function making the call, or a def of the module, its name bound by that def
// alone (nothing else in the function or the module binds it, so the lambda calls the very function the name
// gave). Calling it through the lambda is calling it: the same argument, the same result. The Python source is
// not changed: this is the translator's view of it.
#include <functional>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "astutil.hpp"
#include "pyfront/ast.hpp"

namespace pyengine {

using namespace pyfront;

namespace {

// the names an expression (or a with-item, a comprehension ...) stores
void stores_in(const AnyNode& x, std::map<std::string, int>& n) {
    if (auto* nm = as<Name>(x.expr()); nm && nm->ctx != ExprCtx::Load) n[nm->id]++;
    if (x.cat == AnyNode::Pattern) n["*"]++;   // (a match pattern's names: left alone)
    std::vector<AnyNode> kids;
    child_nodes(x, kids);
    for (auto& k : kids) if (!k.stmt()) stores_in(k, n);
}

// how many times each name is bound in a body's own statements (not inside nested defs / classes, whose names
// count), and the defs among the bindings
void binds_of(const std::vector<Stmt*>& body, std::map<std::string, int>& n, std::map<std::string, const FunctionDef*>& defs) {
    for (auto* st : body) {
        if (st->kind == StmtKind::FunctionDef || st->kind == StmtKind::AsyncFunctionDef) {
            auto* f = static_cast<const FunctionDef*>(st);
            n[f->name]++;
            if (st->kind == StmtKind::FunctionDef) defs[f->name] = f;
            continue;
        }
        if (auto* c = as<ClassDef>(st)) {
            n[c->name]++;
            continue;
        }
        if (st->kind == StmtKind::Global || st->kind == StmtKind::Nonlocal) n["*"]++;
        if (auto* im = as<Import>(st))
            for (auto* a : im->names) n[a->has_asname ? a->asname : a->name.substr(0, a->name.find('.'))]++;
        if (auto* im = as<ImportFrom>(st))
            for (auto* a : im->names) n[a->has_asname ? a->asname : a->name]++;
        std::vector<AnyNode> kids;   // (the statement's own expressions; its blocks are looked at below)
        child_nodes(AnyNode(st), kids);
        for (auto& k : kids) {
            if (k.stmt()) continue;
            if (k.cat == AnyNode::Handler) {
                auto* h = static_cast<const ExceptHandler*>(k.p);
                if (h->has_name) n[h->name]++;
                continue;
            }
            stores_in(k, n);
        }
        for (auto* l : stmt_lists(st)) binds_of(*l, n, defs);
    }
}

// a def callable with one argument: its first parameter positional, without a default; the others defaulted
bool one_argument(const FunctionDef* f) {
    const arguments* a = f->args;
    if (!a || !a->posonlyargs.empty() || a->args.empty() || a->kwarg) return false;
    if (a->defaults.size() + 1 < a->args.size()) return false;   // (a second parameter without a default)
    if (a->defaults.size() >= a->args.size()) return false;      // (the first one has one: called with none)
    for (auto* d : a->kw_defaults) if (!d) return false;
    return true;
}

struct Pass {
    Module& tree;
    std::map<std::string, int> mod_binds;
    std::map<std::string, const FunctionDef*> mod_defs;
    std::set<std::string> patterns;      // module-level P = re.compile(...)
    bool re_module = false;

    explicit Pass(Module& t) : tree(t) {}

    static bool is_re_compile(const Expr* e) {
        auto* c = as<Call>(e);
        auto* fa = c ? as<Attribute>(c->func) : nullptr;
        auto* m = fa ? as<Name>(fa->value) : nullptr;
        return m && m->id == "re" && fa->attr == "compile";
    }

    void setup() {
        binds_of(tree.body, mod_binds, mod_defs);
        for (auto* st : tree.body) {
            if (auto* im = as<Import>(st))
                for (auto* a : im->names) re_module |= a->name == "re" && !a->has_asname;
            if (auto* a = as<Assign>(st); a && a->targets.size() == 1)
                if (auto* t = as<Name>(a->targets[0]); t && is_re_compile(a->value)) patterns.insert(t->id);
        }
        re_module = re_module && mod_binds["re"] == 1;
        for (auto it = patterns.begin(); it != patterns.end();) it = mod_binds[*it] == 1 ? std::next(it) : patterns.erase(it);
    }

    // the callback slot of a call, if it is one of those
    Expr** slot_of(Call* c, const std::map<std::string, int>& own) {
        auto free_name = [&](const std::string& n) { return !own.count(n) && !mod_binds.count(n); };
        auto local = [&](const std::string& n) { return !own.count(n); };   // (a module-level name the function leaves alone)
        if (auto* fa = as<Attribute>(c->func)) {
            auto* recv = as<Name>(fa->value);
            if ((fa->attr == "sub" || fa->attr == "subn") && recv) {
                if (recv->id == "re" && re_module && local("re")) return c->args.size() >= 3 ? &c->args[1] : nullptr;
                if (patterns.count(recv->id) && local(recv->id)) return c->args.size() >= 2 ? &c->args[0] : nullptr;
            }
            if (fa->attr == "sort")
                for (auto* kw : c->keywords) if (kw->has_arg && kw->arg == "key") return &kw->value;
            return nullptr;
        }
        if (auto* fn = as<Name>(c->func); fn && (fn->id == "sorted" || fn->id == "min" || fn->id == "max") && free_name(fn->id))
            for (auto* kw : c->keywords) if (kw->has_arg && kw->arg == "key") return &kw->value;
        return nullptr;
    }

    Lambda* lambda_calling(const std::string& f, const Pos& pos) {
        std::string m = f == "m" ? "m_" : "m";
        auto* a = tree.make<arguments>();
        auto* p = tree.make<arg>();
        p->arg = m;
        p->pos = pos;
        a->args.push_back(p);
        auto* call = tree.make<Call>();
        call->pos = pos;
        auto* fn = tree.make<Name>();
        fn->id = f;
        fn->pos = pos;
        call->func = fn;
        auto* x = tree.make<Name>();
        x->id = m;
        x->pos = pos;
        call->args.push_back(x);
        auto* l = tree.make<Lambda>();
        l->pos = pos;
        l->args = a;
        l->body = call;
        return l;
    }

    void function(FunctionDef* fd) {
        std::map<std::string, int> own;
        std::map<std::string, const FunctionDef*> defs;
        binds_of(fd->body, own, defs);
        if (fd->args) {
            for (auto* a : fd->args->posonlyargs) own[a->arg]++;
            for (auto* a : fd->args->args) own[a->arg]++;
            for (auto* a : fd->args->kwonlyargs) own[a->arg]++;
            if (fd->args->vararg) own[fd->args->vararg->arg]++;
            if (fd->args->kwarg) own[fd->args->kwarg->arg]++;
        }
        if (own.count("*")) return;   // (global / nonlocal: names shared with another scope)
        std::vector<Call*> calls;
        std::function<void(const AnyNode&, bool)> collect = [&](const AnyNode& n, bool top) {
            if (!top) {
                if (auto* st = n.stmt(); st && (st->kind == StmtKind::FunctionDef || st->kind == StmtKind::AsyncFunctionDef || st->kind == StmtKind::ClassDef)) return;
                if (auto* e = n.expr(); e && e->kind == ExprKind::Lambda) return;
            }
            if (auto* c = as<Call>(n.expr())) calls.push_back(const_cast<Call*>(c));
            std::vector<AnyNode> kids;
            child_nodes(n, kids);
            for (auto& k : kids) collect(k, false);
        };
        collect(AnyNode(static_cast<const Stmt*>(fd)), true);
        for (Call* c : calls) {
            Expr** slot = slot_of(c, own);
            auto* nm = slot ? as<Name>(*slot) : nullptr;
            if (!nm) continue;
            const FunctionDef* target = nullptr;
            auto o = own.find(nm->id);
            if (o != own.end()) {
                auto d = defs.find(nm->id);
                if (o->second == 1 && d != defs.end()) target = d->second;
            } else {
                auto mb = mod_binds.find(nm->id);
                auto md = mod_defs.find(nm->id);
                if (mb != mod_binds.end() && mb->second == 1 && md != mod_defs.end()) target = md->second;
            }
            if (!target || !one_argument(target)) continue;
            *slot = lambda_calling(nm->id, nm->pos);
        }
        // the nested defs' own calls
        for (auto& [_, d] : defs) function(const_cast<FunctionDef*>(d));
    }

    void classes(std::vector<Stmt*>& body) {
        for (auto* st : body) {
            if (st->kind == StmtKind::FunctionDef) function(static_cast<FunctionDef*>(st));
            else if (auto* c = as<ClassDef>(st)) classes(c->body);
            else if (st->kind != StmtKind::AsyncFunctionDef)
                for (auto* l : stmt_lists(st)) classes(*const_cast<std::vector<Stmt*>*>(l));
        }
    }

    void run() {
        setup();
        classes(tree.body);
    }
};

}  // namespace

void callbacks_as_lambdas(Module& tree) {
    Pass(tree).run();
}

}  // namespace pyengine
