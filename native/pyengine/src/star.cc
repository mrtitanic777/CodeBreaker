// star.cc -- a call that spreads a tuple into positional arguments, `Entry(*e)` with e a tuple of fixed length,
// seen as the call it is:
//
//     Entry(e[0], e[1], e[2])
//
// The length is guessed from the source: the positional parameters the call leaves to the spread (the
// callee's, when it is a function, a class or a method of the program without *args of its own), else the
// length of a module-level tuple the spread name is bound to once. The guess is only a guess: the call is
// remembered (Program::star_checks), and the inference refuses it unless the value spread is a tuple of
// exactly that many items -- then the two calls pass the same arguments, the same values. Only a spread of a
// name is rewritten (its value is loaded where it was: a name read once or several times is no different);
// a call that also names one of those parameters as a keyword, or spreads twice, is left as it is.
#include <string>
#include <vector>

#include "program.hpp"

namespace pyengine {

using namespace pyfront;

namespace {

// fd's own calls (not those of a nested def, class or lambda)
void own_calls(const AnyNode& n, std::vector<Call*>& out, bool top) {
    if (!top) {
        if (auto* st = n.stmt(); st && (st->kind == StmtKind::FunctionDef || st->kind == StmtKind::AsyncFunctionDef || st->kind == StmtKind::ClassDef)) return;
        if (auto* e = n.expr(); e && e->kind == ExprKind::Lambda) return;
    }
    if (auto* c = as<Call>(n.expr())) out.push_back(const_cast<Call*>(c));
    std::vector<AnyNode> kids;
    child_nodes(n, kids);
    for (auto& k : kids) own_calls(k, out, false);
}

}  // namespace

// the positional parameters a call's callee takes (-1: not known, or it takes *args)
int Program::positional_arity(Fn* f, const A::Call* c) {
    auto of = [](Fn* g, int skip) { return !g || !g->varargs.empty() ? -1 : (int)g->params.size() - skip; };
    if (auto* at = as<Attribute>(c->func)) {
        auto* self = as<pyfront::Name>(at->value);
        if (f->cls && self && !f->params.empty() && self->id == f->params[0] && f->kind == "method") {
            Fn* g = find_method(f->cls->qname, at->attr);
            if (!g) return -1;
            if (g->kind == "method" || g->kind == "classmethod") return of(g, 1);
            if (g->kind == "static") return of(g, 0);
            return -1;
        }
    }
    Ref r = resolve_expr_name(f->module, c->func);
    if (r.k == "fn") {
        Fn** g = fns.get(r.a);
        return g && (*g)->kind == "function" ? of(*g, 0) : -1;
    }
    if (r.k == "class") {
        Fn* init = find_method(r.a, "__init__");
        return init ? of(init, 1) : 0;
    }
    return -1;
}

void Program::star_calls() {
    std::vector<Fn*> all;
    for (auto& [_, f] : fns) all.push_back(f);
    for (auto& [_, f] : nested) all.push_back(f);
    for (Fn* f : all) {
        std::vector<Call*> calls;
        own_calls(AnyNode(static_cast<const A::Stmt*>(f->node)), calls, true);
        for (Call* c : calls) {
            int star = -1, stars = 0;
            for (size_t i = 0; i < c->args.size(); i++)
                if (c->args[i]->kind == ExprKind::Starred) star = (int)i, stars++;
            if (stars != 1) continue;
            auto* var = as<pyfront::Name>(cast<Starred>(c->args[star])->value);
            if (!var) continue;
            bool kwspread = false;
            for (auto* kw : c->keywords) kwspread |= !kw->has_arg;
            if (kwspread) continue;
            int n = -1;
            int p = positional_arity(f, c);
            if (p >= 0 && c->keywords.empty()) n = p - (int)c->args.size() + 1;   // (keywords: left alone)
            if (n < 0) {           // a module-level tuple, bound once
                const std::vector<const A::Expr*>* as_ = f->module->assigns.get(var->id);
                bool local = false;   // (a parameter or a name the function binds)
                for (auto& q : f->params) local |= q == var->id;
                for (const AnyNode& x : walk(AnyNode(static_cast<const A::Stmt*>(f->node))))
                    if (auto* nm = as<pyfront::Name>(x.expr()); nm && nm->id == var->id && nm->ctx != ExprCtx::Load) local = true;
                if (!local && as_ && as_->size() == 1 && (*as_)[0])
                    if (auto* t = as<pyfront::Tuple>((*as_)[0])) {
                        bool plain = true;
                        for (auto* e : t->elts) plain &= e->kind != ExprKind::Starred;
                        if (plain) n = (int)t->elts.size();
                    }
            }
            if (n < 1) continue;
            std::vector<Expr*> items;
            const pyfront::Name* first = nullptr;
            A::Module& tree = *f->module->tree;
            for (int i = 0; i < n; i++) {
                auto* sub = tree.make<Subscript>();
                sub->pos = c->args[star]->pos;
                auto* v = tree.make<pyfront::Name>();
                v->id = var->id;
                v->pos = sub->pos;
                if (!first) first = v;
                sub->value = v;
                auto* k = tree.make<Constant>();
                k->pos = sub->pos;
                k->value.kind = ConstValue::Kind::Int;
                k->value.text = std::to_string(i);
                k->value.i64 = i;
                k->value.fits_i64 = true;
                sub->slice = k;
                items.push_back(sub);
            }
            c->args.erase(c->args.begin() + star);
            c->args.insert(c->args.begin() + star, items.begin(), items.end());
            star_checks[c] = {first, n};
        }
    }
}

}  // namespace pyengine
