// spread.cc -- a call that spreads a record into keywords, `ir.Cast(x=x, type=t, **p)` with p a dict of fixed
// keys (`p = dict(line=.., col=..)`, or what `self.pos(n)` returns), seen as the call it is:
//
//     ir.Cast(x=x, type=t, line=p['line'], col=p['col'])
//
// The keys are guessed from the source: a dict() of keywords or a literal of constant keys, followed through a
// local bound once, a function or method of the module whose every return is such a dict (`self.pos(n)`),
// and a parameter, through every call of its function in the module. The guess is only a guess: the call is
// remembered (Program::spread_checks), and the inference refuses it unless the value spread is a record whose
// keys are exactly those, every one always there -- then the two calls pass the same keywords, the same values.
//
// A spread of a name rewrites in place: the name's value is loaded where it was (a local is no different read
// once or twice). A spread of anything else (`**self.pos(n)`) is evaluated once, into a local, by statements
// put before the statement holding the call, with everything the statement evaluates ahead of it -- the
// receivers, arguments and list items on the way down from the statement's value to the call, then the call's
// own arguments before the spread -- going into locals first, in their order, so nothing runs in a different
// order. The way down goes through calls and list / tuple displays only: a call under a branch (`if`-else,
// and / or), in a comprehension or a lambda runs a different number of times, and is left as it is; so is a
// call with a keyword it also gives by name, a *args, a lambda or a generator among its arguments. (A function
// looked up as name.attr is looked up after the hoisted values: the typed path resolves such lookups once, as
// they do not change.) The Python source is not changed: this is the translator's view of it.
#include <algorithm>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include "astutil.hpp"
#include "spread.hpp"

namespace pyengine {

using namespace pyfront;

namespace {

using Keys = std::vector<std::string>;

struct Scope {
    FunctionDef* fd = nullptr;
    ClassDef* cls = nullptr;              // a method's class
};

struct Spreader {
    Module& tree;
    std::vector<SpreadCheck>& out;
    std::map<std::string, FunctionDef*> functions;                      // module-level functions
    std::map<std::pair<const ClassDef*, std::string>, FunctionDef*> methods;
    std::vector<Scope> scopes;                                          // every function, its class
    int temps = 0;

    Spreader(Module& t, std::vector<SpreadCheck>& o) : tree(t), out(o) {}

    static std::vector<std::string> params(const FunctionDef* fd) {
        std::vector<std::string> ps;
        if (!fd->args) return ps;
        for (auto* a : fd->args->posonlyargs) ps.push_back(a->arg);
        for (auto* a : fd->args->args) ps.push_back(a->arg);
        return ps;
    }
    static bool is_param(const FunctionDef* fd, const std::string& n) {
        if (!fd->args) return false;
        auto ps = params(fd);
        if (std::find(ps.begin(), ps.end(), n) != ps.end()) return true;
        for (auto* a : fd->args->kwonlyargs) if (a->arg == n) return true;
        return (fd->args->vararg && fd->args->vararg->arg == n) || (fd->args->kwarg && fd->args->kwarg->arg == n);
    }

    void index(std::vector<Stmt*>& body, ClassDef* cls, bool top) {
        for (auto* st : body) {
            if (st->kind == StmtKind::FunctionDef) {
                auto* fd = static_cast<FunctionDef*>(st);
                if (top) functions[fd->name] = fd;
                if (cls) methods[{cls, fd->name}] = fd;
                scopes.push_back({fd, cls});
                index(fd->body, nullptr, false);
                continue;
            }
            if (st->kind == StmtKind::AsyncFunctionDef) continue;
            if (auto* c = as<ClassDef>(st)) {
                index(c->body, c, false);
                continue;
            }
            for (auto* l : stmt_lists(st)) index(*const_cast<std::vector<Stmt*>*>(l), cls, top);
        }
    }

    const Scope* scope_of(const FunctionDef* fd) const {
        for (auto& s : scopes) if (s.fd == fd) return &s;
        return nullptr;
    }

    // the one assignment binding n in fd (none: bound otherwise, or more than once)
    static const Expr* bound_once(const FunctionDef* fd, const std::string& n) {
        int count = 0;
        const Expr* value = nullptr;
        for (const AnyNode& x : walk(AnyNode(static_cast<const Stmt*>(fd)))) {
            if (x.p == fd) continue;
            if (auto* nm = as<Name>(x.expr()); nm && nm->id == n && nm->ctx != ExprCtx::Load) count++;
            if (x.cat == AnyNode::Arg && static_cast<const arg*>(x.p)->arg == n) count++;   // (a nested def's or lambda's parameter)
            if (x.cat == AnyNode::Handler) {
                auto* h = static_cast<const ExceptHandler*>(x.p);
                if (h->has_name && h->name == n) count++;
            }
            if (auto* st = x.stmt()) {
                if (st->kind == StmtKind::Global || st->kind == StmtKind::Nonlocal) return nullptr;
                if ((st->kind == StmtKind::FunctionDef || st->kind == StmtKind::AsyncFunctionDef) && static_cast<const FunctionDef*>(st)->name == n) count++;
                if (auto* a = as<Assign>(st); a && a->targets.size() == 1)
                    if (auto* t = as<Name>(a->targets[0]); t && t->id == n) value = a->value;
            }
        }
        return count == 1 ? value : nullptr;
    }

    static std::optional<Keys> agree(std::optional<Keys> a, const std::optional<Keys>& b) {
        if (!a) return b;
        if (!b) return a;
        Keys x = *a, y = *b;
        std::sort(x.begin(), x.end());
        std::sort(y.begin(), y.end());
        if (x != y) return Keys{"\x01"};   // (disagreement: no keys)
        return a;
    }
    static bool bad(const std::optional<Keys>& k) { return !k || (k->size() == 1 && (*k)[0] == "\x01"); }

    FunctionDef* callee(const Expr* func, const Scope& sc) {
        if (auto* nm = as<Name>(func)) {
            auto it = functions.find(nm->id);
            return it != functions.end() && !is_param(sc.fd, nm->id) && !bound_once(sc.fd, nm->id) ? it->second : nullptr;
        }
        if (auto* at = as<Attribute>(func)) {
            auto* self = as<Name>(at->value);
            auto ps = params(sc.fd);
            if (!sc.cls || !self || ps.empty() || self->id != ps[0]) return nullptr;
            auto it = methods.find({sc.cls, at->attr});
            return it != methods.end() ? it->second : nullptr;
        }
        return nullptr;
    }

    std::optional<Keys> keys_of(const Expr* e, const Scope& sc, int depth) {
        if (!e || depth > 5) return std::nullopt;
        if (auto* c = as<Call>(e)) {
            if (auto* fn = as<Name>(c->func); fn && fn->id == "dict" && c->args.empty() && !functions.count("dict")) {
                Keys k;
                for (auto* kw : c->keywords) {
                    if (!kw->has_arg) return std::nullopt;
                    k.push_back(kw->arg);
                }
                return k;
            }
            FunctionDef* f = callee(c->func, sc);
            const Scope* fs = f ? scope_of(f) : nullptr;
            if (!fs) return std::nullopt;
            std::optional<Keys> k;
            for (const AnyNode& x : walk(AnyNode(static_cast<const Stmt*>(f))))
                if (auto* r = as<Return>(x.stmt())) {
                    k = agree(k, keys_of(r->value, *fs, depth + 1));
                    if (bad(k)) return std::nullopt;
                }
            return k;
        }
        if (auto* ie = as<IfExp>(e)) {   // (both branches: the same keys)
            auto k = agree(keys_of(ie->body, sc, depth + 1), keys_of(ie->orelse, sc, depth + 1));
            if (bad(k) || !keys_of(ie->body, sc, depth + 1) || !keys_of(ie->orelse, sc, depth + 1)) return std::nullopt;
            return k;
        }
        if (auto* d = as<Dict>(e)) {
            Keys k;
            for (auto* key : d->keys) {
                std::string s;
                if (!key || !is_str_const(key, &s)) return std::nullopt;
                k.push_back(s);
            }
            return k;
        }
        if (auto* nm = as<Name>(e)) {
            if (is_param(sc.fd, nm->id)) return param_keys(sc, nm->id, depth);
            if (const Expr* v = bound_once(sc.fd, nm->id)) return keys_of(v, sc, depth + 1);
        }
        return std::nullopt;
    }

    // a parameter's keys: what every call of its function in the module passes it
    std::optional<Keys> param_keys(const Scope& sc, const std::string& name, int depth) {
        auto ps = params(sc.fd);
        auto pi = std::find(ps.begin(), ps.end(), name);
        if (pi == ps.end()) return std::nullopt;
        size_t index = pi - ps.begin();
        bool method = sc.cls && !sc.fd->decorator_list.size();
        if (sc.cls && !method) return std::nullopt;
        if (method && index == 0) return std::nullopt;
        std::optional<Keys> k;
        int calls = 0;
        for (auto& caller : scopes)
            for (const AnyNode& x : walk(AnyNode(static_cast<const Stmt*>(caller.fd)))) {
                auto* c = as<Call>(x.expr());
                if (!c || callee(c->func, caller) != sc.fd) continue;
                const Expr* given = nullptr;
                size_t pos = method ? index - 1 : index;
                for (auto* a : c->args) if (a->kind == ExprKind::Starred) return std::nullopt;
                if (pos < c->args.size()) given = c->args[pos];
                for (auto* kw : c->keywords) {
                    if (!kw->has_arg) return std::nullopt;
                    if (kw->arg == name) given = kw->value;
                }
                if (!given) return std::nullopt;   // (its default)
                calls++;
                k = agree(k, keys_of(given, caller, depth + 1));
                if (bad(k)) return std::nullopt;
            }
        return calls ? k : std::nullopt;
    }

    Name* name(const std::string& id, ExprCtx ctx, const Pos& pos) {
        auto* n = tree.make<Name>();
        n->id = id;
        n->ctx = ctx;
        n->pos = pos;
        return n;
    }

    // the keywords k=var['k'] in place of the spread keyword at index i
    void spread_in_place(Call* c, size_t i, const std::string& var, const Keys& keys) {
        std::vector<keyword*> kws;
        const Name* first = nullptr;
        for (auto& k : keys) {
            auto* sub = tree.make<Subscript>();
            sub->pos = c->keywords[i]->value->pos;
            auto* v = name(var, ExprCtx::Load, sub->pos);
            if (!first) first = v;
            sub->value = v;
            auto* key = tree.make<Constant>();
            key->pos = sub->pos;
            key->value.kind = ConstValue::Kind::Str;
            key->value.text = k;
            sub->slice = key;
            auto* kw = tree.make<keyword>();
            kw->pos = c->keywords[i]->pos;
            kw->arg = k;
            kw->has_arg = true;
            kw->value = sub;
            kws.push_back(kw);
        }
        c->keywords.erase(c->keywords.begin() + i);
        c->keywords.insert(c->keywords.begin() + i, kws.begin(), kws.end());
        out.push_back({c, first, keys});
    }

    // the call's one spread keyword (none: none, several, a *args, a keyword it also gives by name)
    std::optional<size_t> spread_of(const Call* c, const Keys* keys) {
        if (auto* fn = as<Name>(c->func); fn && fn->id == "dict") return std::nullopt;
        std::optional<size_t> at;
        for (auto* a : c->args) if (a->kind == ExprKind::Starred) return std::nullopt;
        for (size_t i = 0; i < c->keywords.size(); i++) {
            if (c->keywords[i]->has_arg) continue;
            if (at) return std::nullopt;
            at = i;
        }
        if (at && keys)
            for (auto* kw : c->keywords)
                if (kw->has_arg && std::find(keys->begin(), keys->end(), kw->arg) != keys->end()) return std::nullopt;
        return at;
    }

    static bool simple(const Expr* e) { return e->kind == ExprKind::Name || e->kind == ExprKind::Constant; }

    // a spread of a name, anywhere in the function's own statements
    void names_in(Scope& sc) {
        for (const AnyNode& x : walk(AnyNode(static_cast<const Stmt*>(sc.fd)))) {
            auto* c = const_cast<Call*>(as<Call>(x.expr()));
            if (!c) continue;
            auto at = spread_of(c, nullptr);
            if (!at) continue;
            auto* v = as<Name>(c->keywords[*at]->value);
            if (!v) continue;
            // (only this function's own names: a nested def's calls are its own scope's)
            if (!owned_by(sc.fd, c)) continue;
            auto keys = keys_of(v, sc, 0);
            if (bad(keys) || keys->empty() || !spread_of(c, &*keys)) continue;
            spread_in_place(c, *at, v->id, *keys);
        }
    }

    // c is in fd's own body, not in a nested def / lambda / class
    bool owned_by(const FunctionDef* fd, const Call* c) {
        std::function<bool(const AnyNode&)> in = [&](const AnyNode& n) -> bool {
            if (n.p == c) return true;
            std::vector<AnyNode> kids;
            child_nodes(n, kids);
            for (auto& k : kids) {
                if (auto* st = k.stmt(); st && (st->kind == StmtKind::FunctionDef || st->kind == StmtKind::AsyncFunctionDef || st->kind == StmtKind::ClassDef)) continue;
                if (auto* e = k.expr(); e && e->kind == ExprKind::Lambda) continue;
                if (in(k)) return true;
            }
            return false;
        };
        for (auto* st : fd->body) if (in(AnyNode(st))) return true;
        return false;
    }

    // a call to rewrite by hoisting: its spread value is not a name, its keys are known, no lambda or generator
    // among its arguments (one would become a value of its own), its function a name or x.attr
    bool is_target(Call* t, Scope& sc, Keys& keys) {
        auto at = spread_of(t, nullptr);
        if (!at || t->keywords[*at]->value->kind == ExprKind::Name) return false;
        if (!as<Name>(t->func) && !as<Attribute>(t->func)) return false;
        for (auto* a : t->args) if (a->kind == ExprKind::Lambda || a->kind == ExprKind::GeneratorExp) return false;
        for (auto* kw : t->keywords) if (kw->value->kind == ExprKind::Lambda || kw->value->kind == ExprKind::GeneratorExp) return false;
        auto k = keys_of(t->keywords[*at]->value, sc, 0);
        if (bad(k) || k->empty() || !spread_of(t, &*k)) return false;
        keys = *k;
        return true;
    }

    // the first call to rewrite in the expression at slot, in evaluation order, reached through calls and list /
    // tuple displays only (not a branch, a loop of a comprehension, a lambda); before: the slots evaluated
    // ahead of it on the way (a receiver x of x.f(...), the arguments and items before it)
    bool find_target(Expr** slot, std::vector<Expr**>& before, Call*& target, Keys& keys, Scope& sc) {
        Expr* e = *slot;
        auto lazy = [](const Expr* x) { return x->kind == ExprKind::Lambda || x->kind == ExprKind::GeneratorExp || x->kind == ExprKind::Starred; };
        size_t mark = before.size();
        auto down = [&](Expr** child) -> int {   // 1 found, 0 not (child joins `before`), -1 stop
            if (find_target(child, before, target, keys, sc)) return 1;
            if (lazy(*child)) return -1;
            before.push_back(child);
            return 0;
        };
        if (auto* c = as<Call>(e)) {
            if (is_target(c, sc, keys)) {
                target = c;
                return true;
            }
            if (auto* fa = as<Attribute>(c->func)) {
                if (!simple(fa->value)) before.push_back(&fa->value);   // (the receiver: evaluated first)
            } else if (!as<Name>(c->func)) {
                return false;
            }
            for (auto*& a : c->args) {
                int r = down(&a);
                if (r == 1) return true;
                if (r < 0) break;
            }
            for (auto* kw : c->keywords) {
                int r = down(&kw->value);
                if (r == 1) return true;
                if (r < 0) break;
            }
        } else if (auto* l = as<List>(e)) {
            for (auto*& x : l->elts) {
                int r = down(&x);
                if (r == 1) return true;
                if (r < 0) break;
            }
        } else if (auto* t = as<Tuple>(e)) {
            for (auto*& x : t->elts) {
                int r = down(&x);
                if (r == 1) return true;
                if (r < 0) break;
            }
        }
        before.resize(mark);
        return false;
    }

    void hoist_in(std::vector<Stmt*>& body, Scope& sc, std::set<const Stmt*>& done) {
        for (size_t i = 0; i < body.size();) {
            Stmt* st = body[i];
            if (st->kind == StmtKind::FunctionDef || st->kind == StmtKind::AsyncFunctionDef || st->kind == StmtKind::ClassDef) {
                i++;
                continue;
            }
            if (done.insert(st).second)
                for (auto* l : stmt_lists(st)) hoist_in(*const_cast<std::vector<Stmt*>*>(l), sc, done);
            Expr** v = nullptr;   // (the statement's first evaluation: its value; an assignment's targets come after)
            if (auto* r = as<Return>(st)) v = &r->value;
            else if (auto* a = as<Assign>(st)) v = &a->value;
            else if (auto* x = as<ExprStmt>(st)) v = &x->value;
            std::vector<Expr**> before;
            Call* c = nullptr;
            Keys keys;
            if (!v || !*v || !find_target(v, before, c, keys, sc)) {
                i++;
                continue;
            }
            // what runs before the call into locals, in order; then its arguments before the spread; then the
            // spread value. Every hoisted value runs once, in its place in the order.
            std::vector<Stmt*> pre;
            int n = ++temps;
            int k = 0;
            auto bind = [&](Expr*& e, const std::string& id) {
                auto* a = tree.make<Assign>();
                a->pos = st->pos;
                a->targets.push_back(name(id, ExprCtx::Store, e->pos));
                a->value = e;
                pre.push_back(a);
                e = name(id, ExprCtx::Load, e->pos);
            };
            auto temp = [&] { return "_arg" + std::to_string(n) + "_" + std::to_string(k++); };
            for (Expr** b : before)
                if (!simple(*b)) bind(*b, temp());
            if (auto* fa = as<Attribute>(c->func); fa && !simple(fa->value)) bind(fa->value, temp());
            auto at = spread_of(c, nullptr);
            for (auto*& a : c->args)
                if (!simple(a)) bind(a, temp());
            for (size_t j = 0; j < *at; j++)
                if (!simple(c->keywords[j]->value)) bind(c->keywords[j]->value, temp());
            std::string var = "_kw" + std::to_string(n);
            bind(c->keywords[*at]->value, var);
            // (keywords after the spread are evaluated after it: they stay where they are)
            spread_in_place(c, *at, var, keys);
            body.insert(body.begin() + i, pre.begin(), pre.end());
            // (the hoisted statements are looked at next: a value hoisted may hold another spread)
        }
    }

    void run() {
        index(tree.body, nullptr, true);
        for (auto& sc : scopes) names_in(sc);
        std::set<const Stmt*> done;
        for (auto& sc : scopes) hoist_in(sc.fd->body, sc, done);
    }
};

}  // namespace

void spread_record_keywords(Module& tree, std::vector<SpreadCheck>& out) {
    Spreader(tree, out).run();
}

}  // namespace pyengine
