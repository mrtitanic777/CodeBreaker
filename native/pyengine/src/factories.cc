// factories.cc -- classes a module makes by calling a class factory, written out as the class statements
// they stand for, so the typed path sees classes (their attributes, their constructors) where the source
// has `Unary = _expr('Unary', ['op', 'x'], ['x'])`.
//
// A class factory is a module-level function that returns type(name, (base,), {...}) built from its
// parameters, directly or through `cls = type(...); return cls`; a wrapper is one whose body is
// `return factory(...)` (another wrapper's too). A module-level `X = f(args)` whose arguments, with the
// function's defaults, evaluate to a constant name equal to X, a tuple of bases named in the module, and a
// dict of constant keys whose values are constants (strings, None, numbers, tuples and lists of them) is
// replaced by
//
//     class X(base):
//         __slots__ = (...)
//         <key> = <value> ...
//
// which is what type() makes. When the class's bases (in this module) take their attributes through a
// keyword constructor -- `def __init__(self, **kw)` whose body is `self.a = kw.pop('a', default)`
// statements, one `for k in self.slots(): setattr(self, k, kw.pop(k, None))` (slots(): every __slots__
// entry of the classes below the one defining __init__) and `if kw: raise ...` -- the class also gets the
// constructor that one means for it, its keywords named:
//
//         def __init__(self, *, a=default, ..., s1=None, s2=None, ...):
//             self.a = a ...
//             self.s1 = s1 ...
//
// (Python rejects an unknown keyword by itself, as the `if kw: raise` did: a TypeError either way.)
// Anything else is left as it is.
#include <algorithm>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include "astutil.hpp"
#include "pyfront/ast.hpp"

namespace pyengine {

using namespace pyfront;

namespace {

// a value the arguments evaluate to: a constant, a tuple/list of them, or a name of the module (a base)
struct Val {
    enum K { Bad, None, Str, Int, Seq, Ref } k = Bad;
    std::string s;               // Str, Ref (the name)
    int64_t i = 0;
    std::vector<Val> items;      // Seq
    bool list = false;           // Seq: a list (else a tuple)
};

struct Factory {
    const FunctionDef* fd = nullptr;
    const Call* type_call = nullptr;     // a factory: type(name, bases, dict)
    const Call* inner = nullptr;         // a wrapper: return f(...)
};

using Env = std::map<std::string, Val>;

Val eval(const Expr* e, const Env& env);

Val eval_const(const Constant* c) {
    Val v;
    switch (c->value.kind) {
    case ConstValue::Kind::None: v.k = Val::None; break;
    case ConstValue::Kind::Str: v.k = Val::Str; v.s = c->value.text; break;
    case ConstValue::Kind::Int:
        if (c->value.fits_i64) { v.k = Val::Int; v.i = c->value.i64; }
        break;
    default: break;
    }
    return v;
}

Val eval(const Expr* e, const Env& env) {
    if (!e) return {};
    if (auto* c = as<Constant>(e)) return eval_const(c);
    if (auto* n = as<Name>(e)) {
        auto it = env.find(n->id);
        if (it != env.end()) return it->second;
        Val v;
        v.k = Val::Ref;
        v.s = n->id;
        return v;
    }
    auto seq = [&](const std::vector<Expr*>& elts, bool list) {
        Val v;
        v.k = Val::Seq;
        v.list = list;
        for (auto* x : elts) {
            Val y = eval(x, env);
            if (y.k == Val::Bad || y.k == Val::Ref) return Val{};
            v.items.push_back(y);
        }
        return v;
    };
    if (auto* l = as<List>(e)) return seq(l->elts, true);
    if (auto* t = as<Tuple>(e)) return seq(t->elts, false);
    if (auto* c = as<Call>(e)) {       // tuple(x) / list(x) of a sequence
        auto* f = as<Name>(c->func);
        if (f && (f->id == "tuple" || f->id == "list") && c->args.size() == 1 && c->keywords.empty() && !env.count(f->id)) {
            Val v = eval(c->args[0], env);
            if (v.k != Val::Seq) return {};
            v.list = f->id == "list";
            return v;
        }
        return {};
    }
    if (auto* x = as<IfExp>(e)) {      // a if b is (not) None else c
        auto* cmp = as<Compare>(x->test);
        if (!cmp || cmp->ops.size() != 1 || (cmp->ops[0] != CmpOpKind::Is && cmp->ops[0] != CmpOpKind::IsNot)) return {};
        auto* rc = as<Constant>(cmp->comparators[0]);
        if (!rc || rc->value.kind != ConstValue::Kind::None) return {};
        Val l = eval(cmp->left, env);
        if (l.k == Val::Bad || l.k == Val::Ref) return {};
        bool is_none = l.k == Val::None;
        bool take = cmp->ops[0] == CmpOpKind::Is ? is_none : !is_none;
        return eval(take ? x->body : x->orelse, env);
    }
    return {};
}

const Expr* docless(const std::vector<Stmt*>& body, size_t& at) {
    at = 0;
    if (!body.empty())
        if (auto* es = as<ExprStmt>(body[0]))
            if (auto* c = as<Constant>(es->value); c && c->value.kind == ConstValue::Kind::Str) at = 1;
    return nullptr;
}

std::optional<Factory> classify(const FunctionDef* fd) {
    size_t at;
    docless(fd->body, at);
    const auto& b = fd->body;
    auto is_type_call = [](const Expr* e) -> const Call* {
        auto* c = as<Call>(e);
        auto* f = c ? as<Name>(c->func) : nullptr;
        return f && f->id == "type" && c->args.size() == 3 && c->keywords.empty() ? c : nullptr;
    };
    Factory f;
    f.fd = fd;
    if (b.size() == at + 1) {
        auto* r = as<Return>(b[at]);
        if (!r) return std::nullopt;
        if (auto* tc = is_type_call(r->value)) { f.type_call = tc; return f; }
        if (auto* c = as<Call>(r->value); c && as<Name>(c->func)) { f.inner = c; return f; }
        return std::nullopt;
    }
    if (b.size() == at + 2) {
        auto* a = as<Assign>(b[at]);
        auto* r = as<Return>(b[at + 1]);
        auto* t = a && a->targets.size() == 1 ? as<Name>(a->targets[0]) : nullptr;
        auto* rn = r ? as<Name>(r->value) : nullptr;
        if (t && rn && t->id == rn->id)
            if (auto* tc = is_type_call(a->value)) { f.type_call = tc; return f; }
    }
    return std::nullopt;
}

// the parameters of fd bound to a call's arguments (evaluated in `outer`), its defaults filling the rest
bool bind(const FunctionDef* fd, const Call* call, const Env& outer, Env& env) {
    auto* a = fd->args;
    if (!a || a->vararg || a->kwarg || !a->posonlyargs.empty() || !a->kwonlyargs.empty()) return false;
    const auto& ps = a->args;
    if (call->args.size() > ps.size()) return false;
    for (size_t i = 0; i < call->args.size(); i++) {
        if (as<Starred>(call->args[i])) return false;
        env[ps[i]->arg] = eval(call->args[i], outer);
    }
    for (auto* k : call->keywords) {
        if (!k->has_arg) return false;
        bool found = false;
        for (auto* p : ps) found |= p->arg == k->arg;
        if (!found || env.count(k->arg)) return false;
        env[k->arg] = eval(k->value, outer);
    }
    size_t nd = a->defaults.size();
    for (size_t i = 0; i < ps.size(); i++) {
        if (env.count(ps[i]->arg)) continue;
        if (i + nd < ps.size()) return false;           // a parameter without an argument or a default
        env[ps[i]->arg] = eval(a->defaults[i + nd - ps.size()], {});
    }
    for (auto& [k, v] : env)
        if (v.k == Val::Bad) return false;
    return true;
}

struct Made {
    std::string name;
    std::vector<std::string> bases;
    std::vector<std::pair<std::string, Val>> attrs;     // the dict, in order
};

std::optional<Made> expand(const Call* call, const Env& outer, const std::map<std::string, Factory>& fs, int depth) {
    auto* fn = as<Name>(call->func);
    if (!fn || depth > 8) return std::nullopt;
    auto it = fs.find(fn->id);
    if (it == fs.end()) return std::nullopt;
    Env env;
    if (!bind(it->second.fd, call, outer, env)) return std::nullopt;
    if (it->second.inner) return expand(it->second.inner, env, fs, depth + 1);
    const Call* tc = it->second.type_call;
    Made m;
    Val name = eval(tc->args[0], env);
    if (name.k != Val::Str) return std::nullopt;
    m.name = name.s;
    auto* bt = as<Tuple>(tc->args[1]);
    if (!bt) return std::nullopt;
    for (auto* x : bt->elts) {
        Val b = eval(x, env);
        if (b.k != Val::Ref) return std::nullopt;
        m.bases.push_back(b.s);
    }
    auto* d = as<Dict>(tc->args[2]);
    if (!d) return std::nullopt;
    for (size_t i = 0; i < d->keys.size(); i++) {
        auto* k = as<Constant>(d->keys[i]);
        if (!k || k->value.kind != ConstValue::Kind::Str) return std::nullopt;
        Val v = eval(d->values[i], env);
        if (v.k == Val::Bad || v.k == Val::Ref) return std::nullopt;
        m.attrs.push_back({k->value.text, v});
    }
    return m;
}

// ------------------------------------------------------------------------------- the keyword constructor
struct KwInit {
    std::vector<std::pair<std::string, const Expr*>> pops;   // self.a = kw.pop('a', default)
    std::string defining_class;                              // the class whose __init__ it is
    const FunctionDef* fd = nullptr;
    std::vector<const Assign*> pop_stmts;                    // (the pops' statements, in order)
};

// `def __init__(self, **kw)` of the pattern above, or nothing
std::optional<KwInit> kw_init(const ClassDef* c) {
    for (auto* st : c->body) {
        auto* fd = st->kind == StmtKind::FunctionDef ? static_cast<const FunctionDef*>(st) : nullptr;
        if (!fd || fd->name != "__init__" || !fd->decorator_list.empty()) continue;
        auto* a = fd->args;
        if (!a || a->args.size() != 1 || !a->kwarg || a->vararg || !a->kwonlyargs.empty() || !a->posonlyargs.empty()) return std::nullopt;
        std::string self = a->args[0]->arg, kw = a->kwarg->arg;
        KwInit k;
        k.defining_class = c->name;
        k.fd = fd;
        bool loop = false, check = false;
        auto is_name = [](const Expr* e, const std::string& n) { auto* x = as<Name>(e); return x && x->id == n; };
        auto pop_of = [&](const Expr* e, std::string& key, const Expr*& dflt) {
            auto* call = as<Call>(e);
            auto* at = call ? as<Attribute>(call->func) : nullptr;
            if (!at || at->attr != "pop" || !is_name(at->value, kw) || call->args.size() != 2 || !call->keywords.empty()) return false;
            auto* kc = as<Constant>(call->args[0]);
            if (kc && kc->value.kind == ConstValue::Kind::Str) key = kc->value.text;
            else if (auto* kn = as<Name>(call->args[0])) key = "\x01" + kn->id;
            else return false;
            dflt = call->args[1];
            return true;
        };
        for (auto* s : fd->body) {
            if (auto* as_ = as<Assign>(s)) {
                auto* t = as_->targets.size() == 1 ? as<Attribute>(as_->targets[0]) : nullptr;
                std::string key;
                const Expr* d = nullptr;
                if (!t || !is_name(t->value, self) || !pop_of(as_->value, key, d) || key != t->attr) return std::nullopt;
                if (auto* dc = as<Constant>(d); !dc || eval_const(dc).k == Val::Bad) return std::nullopt;
                k.pops.push_back({key, d});
                k.pop_stmts.push_back(as_);
                continue;
            }
            if (s->kind == StmtKind::For && !loop) {
                auto* f = static_cast<const For*>(s);
                auto* var = as<Name>(f->target);
                auto* it = as<Call>(f->iter);
                auto* m = it ? as<Attribute>(it->func) : nullptr;
                if (!var || !m || m->attr != "slots" || !is_name(m->value, self) || !it->args.empty() || f->body.size() != 1 || !f->orelse.empty())
                    return std::nullopt;
                auto* es = as<ExprStmt>(f->body[0]);
                auto* sc = es ? as<Call>(es->value) : nullptr;
                auto* sn = sc ? as<Name>(sc->func) : nullptr;
                std::string key;
                const Expr* d = nullptr;
                if (!sn || sn->id != "setattr" || sc->args.size() != 3 || !is_name(sc->args[0], self) || !is_name(sc->args[1], var->id) ||
                    !pop_of(sc->args[2], key, d) || key != "\x01" + var->id)
                    return std::nullopt;
                auto* dc = as<Constant>(d);
                if (!dc || dc->value.kind != ConstValue::Kind::None) return std::nullopt;
                loop = true;
                continue;
            }
            if (auto* i = as<If>(s); i && is_name(i->test, kw) && i->orelse.empty() && i->body.size() == 1 && i->body[0]->kind == StmtKind::Raise) {
                check = true;
                continue;
            }
            return std::nullopt;
        }
        if (!loop || !check) return std::nullopt;
        return k;
    }
    return std::nullopt;
}

std::vector<std::string> str_items(const Val& v) {
    std::vector<std::string> out;
    for (auto& x : v.items)
        if (x.k == Val::Str) out.push_back(x.s);
    return out;
}

// a class's own __slots__ (a tuple or list of strings), or nothing
std::optional<std::vector<std::string>> own_slots(const ClassDef* c) {
    for (auto* st : c->body)
        if (auto* a = as<Assign>(st))
            if (a->targets.size() == 1)
                if (auto* t = as<Name>(a->targets[0]); t && t->id == "__slots__") {
                    Val v = eval(a->value, {});
                    if (v.k != Val::Seq || str_items(v).size() != v.items.size()) return std::nullopt;
                    return str_items(v);
                }
    return std::vector<std::string>{};
}

struct Builder {
    Module& tree;
    Pos pos;
    template <class T> T* mk() {
        T* n = tree.make<T>();
        n->pos = pos;
        return n;
    }
    Expr* name(const std::string& id, ExprCtx ctx = ExprCtx::Load) {
        auto* n = mk<Name>();
        n->id = id;
        n->ctx = ctx;
        return n;
    }
    Expr* value(const Val& v) {
        if (v.k == Val::Seq) {
            std::vector<Expr*> elts;
            for (auto& x : v.items) elts.push_back(value(x));
            if (v.list) { auto* l = mk<List>(); l->elts = elts; return l; }
            auto* t = mk<Tuple>();
            t->elts = elts;
            return t;
        }
        auto* c = mk<Constant>();
        if (v.k == Val::Str) { c->value.kind = ConstValue::Kind::Str; c->value.text = v.s; }
        else if (v.k == Val::Int) { c->value.kind = ConstValue::Kind::Int; c->value.i64 = v.i; c->value.fits_i64 = true; c->value.text = std::to_string(v.i); }
        else c->value.kind = ConstValue::Kind::None;
        return c;
    }
    Stmt* assign(Expr* target, Expr* v) {
        auto* a = mk<Assign>();
        a->targets.push_back(target);
        a->value = v;
        return a;
    }
    Expr* self_attr(const std::string& self, const std::string& attr) {
        auto* at = mk<Attribute>();
        at->value = name(self);
        at->attr = attr;
        at->ctx = ExprCtx::Store;
        return at;
    }
    arg* param(const std::string& n) {
        auto* a = mk<arg>();
        a->arg = n;
        return a;
    }
};

}  // namespace

// the keyword constructor's own pops as the keyword parameters they are: `self.a = kw.pop('a', d)` is
// `def __init__(self, *, a=d, **kw): self.a = a` -- the same call accepted, the same value stored, and the
// attribute typed by what callers pass instead of by the untyped **kw dict
void name_the_pops(Module& tree, const KwInit& k) {
    auto* fd = const_cast<FunctionDef*>(k.fd);
    for (size_t i = 0; i < k.pops.size(); i++) {
        auto* a = const_cast<Assign*>(k.pop_stmts[i]);
        auto* p = tree.make<arg>();
        p->pos = a->pos;
        p->arg = k.pops[i].first;
        fd->args->kwonlyargs.push_back(p);
        fd->args->kw_defaults.push_back(const_cast<Expr*>(k.pops[i].second));
        auto* n = tree.make<Name>();
        n->pos = a->value->pos;
        n->id = k.pops[i].first;
        a->value = n;
    }
}

void expand_class_factories(Module& tree) {
    std::map<std::string, Factory> fs;
    std::map<std::string, ClassDef*> classes;
    for (auto* st : tree.body) {
        if (st->kind == StmtKind::FunctionDef)
            if (auto f = classify(static_cast<FunctionDef*>(st))) fs[static_cast<FunctionDef*>(st)->name] = *f;
        if (auto* c = as<ClassDef>(st)) classes[c->name] = c;
    }
    // (a wrapper of something that is not a factory is not one)
    for (bool changed = true; changed;) {
        changed = false;
        for (auto it = fs.begin(); it != fs.end();) {
            auto* fn = it->second.inner ? as<Name>(it->second.inner->func) : nullptr;
            if (it->second.inner && (!fn || !fs.count(fn->id) || fn->id == it->first)) { it = fs.erase(it); changed = true; }
            else ++it;
        }
    }
    if (fs.empty()) return;
    std::set<std::string> assigned;     // a name bound twice at module level is left alone
    std::map<std::string, int> binds;
    for (auto* st : tree.body)
        if (auto* a = as<Assign>(st))
            for (auto* t : a->targets)
                if (auto* n = as<Name>(t)) binds[n->id]++;
    for (auto*& st : tree.body) {
        auto* a = as<Assign>(st);
        auto* t = a && a->targets.size() == 1 ? as<Name>(a->targets[0]) : nullptr;
        auto* call = a ? as<Call>(a->value) : nullptr;
        if (!t || !call || binds[t->id] != 1) continue;
        auto m = expand(call, {}, fs, 0);
        if (!m || m->name != t->id) continue;
        bool bases_ok = !m->bases.empty();
        for (auto& b : m->bases) bases_ok &= classes.count(b) > 0;
        if (!bases_ok) continue;
        Builder B{tree, st->pos};
        auto* c = B.mk<ClassDef>();
        c->name = m->name;
        for (auto& b : m->bases) c->bases.push_back(B.name(b));
        std::vector<std::string> slots;
        for (auto& [k, v] : m->attrs) {
            c->body.push_back(B.assign(B.name(k, ExprCtx::Store), B.value(v)));
            if (k == "__slots__") slots = str_items(v);
        }
        // the keyword constructor, from the first base chain (single inheritance) that has one
        std::vector<const ClassDef*> chain;     // the class's bases, nearest first
        for (std::string b = m->bases.size() == 1 ? m->bases[0] : ""; !b.empty();) {
            auto it = classes.find(b);
            if (it == classes.end()) { chain.clear(); break; }
            chain.push_back(it->second);
            b = it->second->bases.size() == 1 && as<Name>(it->second->bases[0]) ? as<Name>(it->second->bases[0])->id : "";
            if (it->second->bases.size() > 1) { chain.clear(); break; }
            if (b == "object") b.clear();
        }
        std::optional<KwInit> init;
        size_t at = 0;
        for (; at < chain.size() && !init; at++) init = kw_init(chain[at]);
        bool ok = init.has_value();
        std::vector<std::string> all;           // slots(): every class below the defining one, root first
        if (ok) {
            for (size_t i = at - 1; i-- > 0;) {
                auto s = own_slots(chain[i]);
                if (!s) { ok = false; break; }
                for (auto& x : *s) if (std::find(all.begin(), all.end(), x) == all.end()) all.push_back(x);
            }
            for (auto& x : slots) if (std::find(all.begin(), all.end(), x) == all.end()) all.push_back(x);
            for (auto& [k, d] : init->pops) if (std::find(all.begin(), all.end(), k) != all.end()) ok = false;
        }
        if (ok) {
            auto* fd = B.mk<FunctionDef>();
            fd->name = "__init__";
            fd->args = B.mk<arguments>();
            fd->args->args.push_back(B.param("self"));
            for (auto& [k, d] : init->pops) {
                fd->args->kwonlyargs.push_back(B.param(k));
                fd->args->kw_defaults.push_back(const_cast<Expr*>(d));
                fd->body.push_back(B.assign(B.self_attr("self", k), B.name(k)));
            }
            for (auto& s : all) {
                fd->args->kwonlyargs.push_back(B.param(s));
                fd->args->kw_defaults.push_back(B.value(Val{Val::None}));
                fd->body.push_back(B.assign(B.self_attr("self", s), B.name(s)));
            }
            c->body.push_back(fd);
        }
        if (c->body.empty()) c->body.push_back(B.mk<Pass>());
        st = c;
        classes[c->name] = c;
    }
    for (auto& [n, c] : classes)
        if (auto k = kw_init(c)) name_the_pops(tree, *k);
}

}  // namespace pyengine
