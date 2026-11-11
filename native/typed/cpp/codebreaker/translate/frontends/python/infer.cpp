// codebreaker/translate/frontends/python/infer.cpp -- typed C++ from
// codebreaker.translate.frontends.python.infer.

#include "codebreaker/translate/frontends/python/infer.hpp"

#include <algorithm>

#include "codebreaker/translate/frontends/python/methods.hpp"

namespace codebreaker::translate::frontends::python::infer {

using program::at;
using program::fk;
using program::k_in;
using program::k_is;
using program::last_part;
using program::lit_eq;
using program::str_starts;
using program::TupItem;
using pyengine::AnyNode;

namespace {

// ---------------------------------------------------------------------------------------- small helpers
// a bound as `'%s' % lo` writes it
Str bs(const Bound& b) { return b ? Str(b->str()) : Str("None"); }

Str i2s(int64_t v) { return py::to_str(v); }

Big bmax(const Big& a, const Big& b) { return a > b ? a : b; }
Big bmin(const Big& a, const Big& b) { return a < b ? a : b; }

T t_inst(const Str& name) { return T(PS("inst"), {}, std::nullopt, std::nullopt, name); }
T t_named(const Str& kind, const Str& name) { return T(kind, {}, std::nullopt, std::nullopt, name); }
T t_site(const Str& kind, int64_t site) { return T(kind, {}, std::nullopt, std::nullopt, std::nullopt, site); }

Tup rt(const char* kind) { return Tup(Str(kind)); }
Tup rt(const char* kind, std::initializer_list<TupItem> items) { return Tup(Str(kind), std::vector<TupItem>(items)); }
TupItem rs(const Str& s) { return TupItem(s); }
TupItem rn() { return program::none_item(); }
TupItem rb(bool b) { return TupItem(b); }
TupItem ri(const Big& b) { return TupItem(b); }
TupItem ropt(const std::optional<Big>& b) { return b ? TupItem(*b) : rn(); }
TupItem ropt_s(const std::optional<Str>& s) { return s ? TupItem(*s) : rn(); }

// _str_const / _int_const
std::optional<Str> str_const(const A::Expr* n) { return program::str_const(n); }
std::optional<Big> int_const(const A::Expr* n) { return program::int_const(n); }

// a Python tuple slice args[a:b] (a, b ints or None)
std::vector<T> py_tuple_slice(const std::vector<T>& v, const std::optional<Big>& a, const std::optional<Big>& b) {
    int64_t n = static_cast<int64_t>(v.size());
    auto clamp = [&](const std::optional<Big>& x, int64_t dflt) -> int64_t {
        if (!x) {
            return dflt;
        }
        if (*x < Big(0)) {
            Big y = *x + Big(n);
            return y < Big(0) ? 0 : static_cast<int64_t>(y.i64());
        }
        return *x > Big(n) ? n : static_cast<int64_t>(x->i64());
    };
    int64_t lo = clamp(a, 0), hi = clamp(b, n);
    std::vector<T> out;
    for (int64_t i = lo; i < hi; ++i) {
        out.push_back(v[static_cast<size_t>(i)]);
    }
    return out;
}

bool is_name(const A::Expr* e, const char* id) {
    auto* n = A::as<A::Name>(e);
    return n && n->id == id;
}

const std::vector<std::string>& unused_() {
    static const std::vector<std::string> v;
    return v;
}

}  // namespace

const std::set<Str>& BUILTIN_FUNCS() {
    static const std::set<Str> v = [] {
        const char* names =
            "len range enumerate zip isinstance int str bytes bytearray bool float min max abs "
            "sorted reversed sum any all list dict set tuple print open ord chr hex oct bin repr divmod round pow format "
            "super iter next map filter getattr setattr hasattr type id hash vars globals locals callable issubclass "
            "object frozenset delattr exec eval compile input memoryview staticmethod classmethod property";
        std::set<Str> out;
        std::string cur;
        for (const char* p = names;; ++p) {
            if (*p == ' ' || *p == 0) {
                if (!cur.empty()) {
                    out.insert(Str(cur));
                }
                cur.clear();
                if (*p == 0) {
                    break;
                }
            } else {
                cur.push_back(*p);
            }
        }
        return out;
    }();
    return v;
}

const std::set<Str>& DYNAMIC_BUILTINS() {
    static const std::set<Str> v = [] {
        const char* names =
            "getattr setattr hasattr delattr type id hash vars globals locals callable issubclass "
            "exec eval compile iter next memoryview input";
        std::set<Str> out;
        std::string cur;
        for (const char* p = names;; ++p) {
            if (*p == ' ' || *p == 0) {
                if (!cur.empty()) {
                    out.insert(Str(cur));
                }
                cur.clear();
                if (*p == 0) {
                    break;
                }
            } else {
                cur.push_back(*p);
            }
        }
        return out;
    }();
    return v;
}

bool has_top(const T& t, int64_t depth) {
    if (k_is(t, "top")) {
        return true;
    }
    if (depth < 6) {
        for (const T& a : t.args) {
            if (has_top(a, depth + 1)) {
                return true;
            }
        }
    }
    return false;
}

std::set<Str> assigned_names(const A::FunctionDef* fn_node) {
    std::set<Str> out;
    std::vector<AnyNode> todo;
    for (const A::Stmt* s : fn_node->body) {
        todo.push_back(AnyNode(s));
    }
    while (!todo.empty()) {
        AnyNode n = todo.back();
        todo.pop_back();
        if (const A::Stmt* s = n.stmt()) {
            using K = A::StmtKind;
            if (s->kind == K::FunctionDef || s->kind == K::AsyncFunctionDef) {
                out.insert(Str(A::cast<A::FunctionDef>(s)->name));
                continue;
            }
            if (s->kind == K::ClassDef) {
                out.insert(Str(A::cast<A::ClassDef>(s)->name));
                continue;
            }
            if (s->kind == K::Import) {
                for (const A::alias* a : A::cast<A::Import>(s)->names) {
                    std::string nm = a->has_asname ? a->asname : a->name;
                    size_t dot = nm.find('.');
                    out.insert(Str(dot == std::string::npos ? nm : nm.substr(0, dot)));
                }
            } else if (s->kind == K::ImportFrom) {
                for (const A::alias* a : A::cast<A::ImportFrom>(s)->names) {
                    std::string nm = a->has_asname ? a->asname : a->name;
                    size_t dot = nm.find('.');
                    out.insert(Str(dot == std::string::npos ? nm : nm.substr(0, dot)));
                }
            }
        } else if (const A::Expr* e = n.expr()) {
            if (e->kind == A::ExprKind::Lambda) {
                continue;
            }
            if (auto* nm = A::as<A::Name>(e); nm && nm->ctx != A::ExprCtx::Load) {
                out.insert(Str(nm->id));
            }
            using K = A::ExprKind;
            if (e->kind == K::ListComp || e->kind == K::SetComp || e->kind == K::DictComp || e->kind == K::GeneratorExp) {
                continue;  // their targets are their own scope
            }
        } else if (n.cat == AnyNode::Handler) {
            auto* h = static_cast<const A::ExceptHandler*>(n.p);
            if (h->has_name) {
                out.insert(Str(h->name));
            }
        }
        pyengine::child_nodes(n, todo);
    }
    return out;
}

// ---------------------------------------------------------------------------------------- State
const T* State::find(const Str& name) const {
    for (const auto& kv : v_) {
        if (kv.first == name) {
            return &kv.second;
        }
    }
    return nullptr;
}

const T& State::at(const Str& name) const {
    const T* t = find(name);
    if (!t) {
        throw py::KeyError(name.utf8());
    }
    return *t;
}

void State::set(const Str& name, const T& t) {
    for (auto& kv : v_) {
        if (kv.first == name) {
            kv.second = t;
            return;
        }
    }
    v_.emplace_back(name, t);
}

void State::pop(const Str& name) {
    for (size_t i = 0; i < v_.size(); ++i) {
        if (v_[i].first == name) {
            v_.erase(v_.begin() + static_cast<std::ptrdiff_t>(i));
            return;
        }
    }
}

bool State::same_keys(const State& o) const {
    if (v_.size() != o.v_.size()) {
        return false;
    }
    for (const auto& kv : v_) {
        if (!o.find(kv.first)) {
            return false;
        }
    }
    return true;
}

bool Eval::MemoKey::operator==(const MemoKey& o) const {
    if (st != o.st || items.size() != o.items.size()) {
        return false;
    }
    for (size_t i = 0; i < items.size(); ++i) {
        if (!(items[i].first == o.items[i].first) || !(items[i].second == o.items[i].second)) {
            return false;
        }
    }
    return true;
}

size_t Eval::MemoKeyHash::operator()(const MemoKey& k) const {
    size_t h = std::hash<const void*>()(k.st);
    for (const auto& kv : k.items) {
        h = h * 1000003u ^ py::str_hash()(kv.first);
        h = h * 1000003u ^ static_cast<size_t>(kv.second.hash());
    }
    return h;
}

// ---------------------------------------------------------------------------------------- Eval
Eval::Eval(Program& prog, Fn* fn_, bool record_)
    : p(prog), fn(fn_), m(fn_ ? fn_->module : nullptr), record(record_), ret(P::BOTTOM()), cls(fn_ ? fn_->cls : nullptr) {}

Eval::~Eval() = default;

T Eval::note(const A::Node* node, const T& t, const std::optional<Res>& r) {
    const void* k = node;
    auto it = seen.find(k);
    if (it == seen.end()) {
        seen.insert_or_assign(k, t);
    } else {
        T j = p.join(it->second, t);
        seen.insert_or_assign(k, j);
    }
    if (record && !quiet) {
        auto old = types.find(k);
        if (old == types.end()) {
            types.insert_or_assign(k, t);
        } else {
            T j = p.join(old->second, t);
            types.insert_or_assign(k, j);
        }
        if (r) {
            res[k] = *r;
        }
    }
    return t;
}

T Eval::issue(const A::Node* node, const Str& cls_, const Str& detail) {
    if (record && !quiet) {
        issues.emplace_back(cls_, detail, node ? static_cast<int64_t>(node->pos.lineno) : 0);
    }
    return P::top(cls_ + Str(":") + detail);
}

void Eval::contribute_param(const Str& fq, const Str& name, const T& t) {
    // a value with no single type is the caller's refusal, not the callee's: it is not passed on
    // (but remembered: a parameter only such callers reach is untyped because of them)
    if (live() && k_is(t, "top") && !p.param_poison.contains(std::make_tuple(fq, name))) {
        p.param_poison.set(std::make_tuple(fq, name),
                           std::make_tuple(fn ? fn->qname : Str("?"), t.why ? *t.why : Str()));
    }
    if (!live() || k_in(t, {"bottom", "top"}) || (k_in(t, {"tuple", "opt"}) && has_top(t))) {
        return;
    }
    Fn* f = p.fns.at(fq);
    py::dict<Str, T> ps = p.params[fq];
    T old = ps.contains(name) ? ps.at(name) : P::BOTTOM();
    T new_ = P::BOTTOM();
    if (f->annotations.contains(name)) {
        T at_ = p.annotation_type(*f, name);
        if (k_is(at_, "int") && k_is(t, "int")) {  // the annotation's kind, the call sites' range
            new_ = !k_is(old, "bottom") ? p.join(old, t) : t;
        } else {
            new_ = at_;
        }
    } else {
        new_ = p.join(old, t);
    }
    if (p.round > 3 && k_is(new_, "int") && k_is(old, "int")) {
        new_ = P::widen_int(old, new_);
    }
    if (new_ != old) {
        ps.set(name, new_);
        p.touch(fk('p', fq));
    }
}

T Eval::arg_fit(const A::Node* node, const Fn& f, const Str& name, const T& t0) {
    T t = fit(node, t0, Str("argument ") + name);
    bool aware = f.none_aware.count(name) != 0;
    if (k_is(t, "opt") && !aware) {
        issue(node, Str("conflict"), Str("passes a value that may be None as ") + name + Str(" of ") + f.name);
        return t.args[0];
    }
    if (k_is(t, "none") && !aware) {
        issue(node, Str("conflict"), Str("passes None as ") + name + Str(" of ") + f.name);
        return P::BOTTOM();
    }
    return t;
}

T Eval::fit(const A::Node* node, const T& t, const Str& what) {
    if (k_is(t, "int") && !t.fits_i64()) {
        if (lit_eq(p.int_policy, "strict")) {
            issue(node, Str("int-range"), what + Str(" in [") + bs(t.lo) + Str(", ") + bs(t.hi) + Str("]"));
        }
        return P::meet(t, P::I64_MIN(), P::I64_MAX());
    }
    if (k_in(t, {"tuple", "opt"})) {
        bool any = false;
        for (const T& a : t.args) {
            if (k_is(a, "int") && !a.fits_i64()) {
                any = true;
                break;
            }
        }
        if (any) {
            std::vector<T> as;
            for (const T& a : t.args) {
                as.push_back(fit(node, a, what));
            }
            return T(t.kind, as);
        }
    }
    return t;
}

void Eval::contribute_attr(const Str& cq, const Str& attr, const T& t) {
    if (live() && (k_is(t, "top") || (k_in(t, {"tuple", "opt"}) && has_top(t)))) {
        std::optional<Str> o = p.attr_owner(cq, attr);
        Str owner = o ? *o : cq;
        auto key = std::make_tuple(owner, attr);
        if (!p.poisoned.contains(key)) {
            p.poisoned.set(key, fn ? fn->qname : Str("?"));
            p.touch(fk('a', owner));
        }
        return;
    }
    if (!live() || k_in(t, {"bottom", "top"}) || (k_in(t, {"tuple", "opt"}) && has_top(t))) {
        return;
    }
    std::optional<Str> o = p.attr_owner(cq, attr);
    Str owner = o ? *o : cq;
    py::dict<Str, T> a = p.attrs[owner];
    T old = a.contains(attr) ? a.at(attr) : P::BOTTOM();
    T new_ = p.join(old, t);
    if (p.round > 3 && k_is(new_, "int") && k_is(old, "int")) {
        new_ = P::widen_int(old, new_);
    }
    if (new_ != old) {
        a.set(attr, new_);
        p.touch(fk('a', owner));
    }
}

// ---------------------------------------------------------------------------------------- the function
Eval& Eval::run() {
    Fn& f = *fn;
    const A::FunctionDef* node = f.node;
    locals = p.fn_locals(f);
    State state;
    if (f.varargs || f.kwargs) {
        issue(node, Str("dynamic"), f.varargs ? Str("*args") : Str("**kwargs"));
    }
    if (lit_eq(f.kind, "async")) {
        issue(node, Str("dynamic"), Str("async"));
    }
    for (const Str& d : f.decorators) {
        if (!lit_eq(d, "staticmethod") && !lit_eq(d, "property")) {
            issue(node, Str("dynamic"), Str("decorator ") + d);
        }
    }
    std::vector<Str> all = f.params;
    all.insert(all.end(), f.kwonly.begin(), f.kwonly.end());
    for (size_t i = 0; i < all.size(); ++i) {
        const Str& name = all[i];
        T t = P::BOTTOM();
        if (i == 0 && (lit_eq(f.kind, "method") || lit_eq(f.kind, "property"))) {
            t = t_inst(f.cls->qname);
        } else if (i == 0 && lit_eq(f.kind, "classmethod")) {
            t = t_named(Str("cls"), f.cls->qname);
        } else {
            t = p.param_type(f, name);
        }
        state.set(name, t);
    }
    for (const AnyNode& an : pyengine::walk(AnyNode(static_cast<const A::Stmt*>(node)))) {
        if (const A::Expr* e = an.expr()) {
            if (e->kind == A::ExprKind::Yield || e->kind == A::ExprKind::YieldFrom) {
                issue(e, Str("dynamic"), Str("generator"));
                break;
            }
        }
    }
    SP out = block(node->body, state);
    if (out && returns_value) {
        ret = p.join(ret, P::NONE());
    }
    if (!returns_value) {
        ret = P::NONE();
    }
    if (k_is(ret, "int") && !ret.fits_i64()) {
        ret = fit(node, ret, Str("return value"));
    }
    if (live()) {
        const Str& fq = f.qname;
        T old = p.returns.contains(fq) ? p.returns.at(fq) : P::BOTTOM();
        T new_ = p.join(old, ret);
        if (p.round > 3 && k_is(new_, "int") && k_is(old, "int")) {
            new_ = P::widen_int(old, new_);
        }
        if (new_ != old) {
            p.returns.set(fq, new_);
            p.touch(fk('r', fq));
        }
    }
    return *this;
}

// ---------------------------------------------------------------------------------------- statements
SP Eval::block(const std::vector<A::Stmt*>& stmts, SP state) {
    for (const A::Stmt* st : stmts) {
        if (!state) {
            return std::nullopt;
        }
        state = stmt(st, *state);
    }
    return state;
}

SP Eval::stmt(const A::Stmt* st, const State& state) { return s_dispatch(st, state); }

SP Eval::s_dispatch(const A::Stmt* st, const State& state) {
    using K = A::StmtKind;
    switch (st->kind) {
        case K::Pass: return state;
        case K::Expr: return s_Expr(A::cast<A::ExprStmt>(st), state);
        case K::Assign: return s_Assign(A::cast<A::Assign>(st), state);
        case K::AnnAssign: return s_AnnAssign(A::cast<A::AnnAssign>(st), state);
        case K::AugAssign: return s_AugAssign(A::cast<A::AugAssign>(st), state);
        case K::Return: return s_Return(A::cast<A::Return>(st), state);
        case K::Raise: return s_Raise(A::cast<A::Raise>(st), state);
        case K::Assert: return s_Assert(A::cast<A::Assert>(st), state);
        case K::Global: issue(st, Str("dynamic"), Str("global")); return state;
        case K::Nonlocal: issue(st, Str("dynamic"), Str("nonlocal")); return state;
        case K::Delete: issue(st, Str("dynamic"), Str("del")); return state;
        case K::FunctionDef:
        case K::AsyncFunctionDef: issue(st, Str("syntax"), Str("nested function")); return state;
        case K::ClassDef: issue(st, Str("syntax"), Str("nested class")); return state;
        case K::Import: return s_Import(A::cast<A::Import>(st), state);
        case K::ImportFrom: return s_ImportFrom(A::cast<A::ImportFrom>(st), state);
        case K::If: return s_If(A::cast<A::If>(st), state);
        case K::With: return s_With(A::cast<A::With>(st), state);
        case K::Try: return s_Try(A::cast<A::Try>(st), state);
        case K::Break: return s_Break(st, state);
        case K::Continue: return s_Continue(st, state);
        case K::While: return s_While(A::cast<A::While>(st), state);
        case K::For:
        case K::AsyncFor: return s_For(A::cast<A::For>(st), state);
        default: {
            // type(st).__name__.lower()
            std::string nm = A::name_of(st->kind);
            for (char& c : nm) {
                c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
            }
            issue(st, Str("syntax"), Str(nm));
            return state;
        }
    }
}

SP Eval::s_Expr(const A::ExprStmt* st, const State& state) {
    if (A::as<A::Constant>(st->value)) {
        return state;
    }
    ev(st->value, state);
    return after_access(st->value, state);
}

SP Eval::s_Assign(const A::Assign* st, const State& state) {
    bool builder_form = st->targets.size() == 1 && A::as<A::Name>(st->targets[0]) &&
                        (A::as<A::Dict>(st->value) || A::as<A::Call>(st->value));
    if (builder_form) {
        builder = static_cast<const A::Node*>(st->value);  // its creation keys are counted by builder_keys
    }
    T t = ev(st->value, state);
    SP cur = after_access(st->value, state);
    State s = *cur;
    for (const A::Expr* tgt : st->targets) {
        s = assign(tgt, t, s, st->value);
    }
    if (builder_form) {
        builder_keys(st, t);
    }
    return s;
}

SP Eval::s_AnnAssign(const A::AnnAssign* st, const State& state) {
    if (!st->value) {
        return state;
    }
    T t = ev(st->value, state);
    if (A::as<A::Name>(st->target)) {
        T at_ = p.ann_to_type(*m, st->annotation, st->annotation);
        if (!k_in(at_, {"bottom", "top", "int"})) {
            // (the Python joins the two for their effect on the sites, and takes the annotation either way)
            if (k_in(t, {"list", "dict", "set"}) && at_.kind == t.kind) {
                p.join(at_, t);
            }
            t = at_;
        }
    }
    return assign(st->target, t, state, st->value);
}

SP Eval::s_AugAssign(const A::AugAssign* st, const State& state) {
    T cur = ev(st->target, state);
    T val = ev(st->value, state);
    T t = binop(st, st->op, cur, val, st->target, st->value);
    note(st, t);
    return assign(st->target, t, state, nullptr, true);
}

SP Eval::s_Return(const A::Return* st, const State& state) {
    (void)state;
    if (st->value) {
        T t = ev(st->value, state);
        returns_value = true;
        if (live()) {
            ret = p.join(ret, t);
        }
    } else if (live()) {
        ret = p.join(ret, P::NONE());
    }
    return std::nullopt;
}

SP Eval::s_Raise(const A::Raise* st, const State& state) {
    if (st->exc) {
        T t = ev(st->exc, state);
        if (!k_in(t, {"inst", "cls", "bottom", "top"}) && !k_is(t, "extern")) {
            issue(st, Str("conflict"), Str("raise of a ") + t.kind);
        }
    }
    if (st->cause) {
        issue(st, Str("syntax"), Str("raise from"));
    }
    return std::nullopt;
}

SP Eval::s_Assert(const A::Assert* st, const State& state) {
    ev(st->test, state);
    if (st->msg) {
        ev(st->msg, state);
    }
    return refine(st->test, state, true);
}

SP Eval::s_Import(const A::Import* st, const State& state) {
    State s = state;
    for (const A::alias* a : st->names) {
        std::string top = a->name.substr(0, a->name.find('.'));
        Str local = a->has_asname ? Str(a->asname) : Str(top);
        local_imports.set(local, p.module_ref(a->has_asname ? Str(a->name) : Str(top)));
        s.pop(local);
    }
    return s;
}

SP Eval::s_ImportFrom(const A::ImportFrom* st, const State& state) {
    State s = state;
    Str base = p.from_base(*m, st);
    for (const A::alias* a : st->names) {
        Str local = a->has_asname ? Str(a->asname) : Str(a->name);
        Str full = !base.empty() ? base + Str(".") + Str(a->name) : Str(a->name);
        if (p.modules.contains(full)) {
            local_imports.set(local, rt("module", {rs(full)}));
        } else if (p.modules.contains(base)) {
            std::optional<Ref> r = p.resolve_global(*p.modules.at(base), Str(a->name));
            local_imports.set(local, r ? *r : rt("missing", {rs(full)}));
        } else {
            const std::u32string& u = full.u32();
            size_t dot = u.find(U'.');
            Str top = dot == std::u32string::npos ? full : Str(u.substr(0, dot));
            local_imports.set(local, program::STDLIB().count(top) ? rt("stdobj", {rs(full)}) : rt("extern", {rs(full)}));
        }
        s.pop(local);
    }
    return s;
}

SP Eval::s_If(const A::If* st, const State& state) {
    ev(st->test, state);
    SP cur = after_access(st->test, state);
    SP a = block(st->body, refine(st->test, cur, true));
    SP b = block(st->orelse, refine(st->test, cur, false));
    return join_states(a, b);
}

SP Eval::s_With(const A::With* st, const State& state) {
    State s = state;
    for (const A::withitem* item : st->items) {
        T t = ev(item->context_expr, s);
        if (!(k_is(t, "extern") && t.name && lit_eq(*t.name, "file"))) {
            issue(st, Str("syntax"), Str("with"));
        }
        if (item->optional_vars) {
            s = assign(item->optional_vars, t, s, nullptr);
        }
    }
    return block(st->body, s);
}

bool Eval::std_exception(const A::Expr* n) {
    auto* at_ = A::as<A::Attribute>(n);
    if (!(at_ && A::as<A::Name>(at_->value))) {
        return false;
    }
    const std::string& v = A::cast<A::Name>(at_->value)->id;
    const std::string& a = at_->attr;
    return (v == "struct" && a == "error") || (v == "subprocess" && a == "TimeoutExpired") ||
           (v == "subprocess" && a == "CalledProcessError") || (v == "json" && a == "JSONDecodeError") ||
           (v == "zlib" && a == "error");
}

SP Eval::s_Try(const A::Try* st, const State& state) {
    if (!st->finalbody.empty()) {
        issue(st, Str("syntax"), Str("finally"));
    }
    if (!st->orelse.empty()) {
        issue(st, Str("syntax"), Str("try-else"));
    }
    SP body = block(st->body, state);
    // a handler can be entered from anywhere in the body: its state is what the body had at entry
    // joined with every state along the body (approximated by entry joined with the end of the body)
    SP entry = body ? join_states(SP(state), body) : SP(state);
    std::vector<SP> outs = {body};
    for (const A::ExceptHandler* h : st->handlers) {
        State hs = *entry;
        if (h->type) {
            std::vector<const A::Expr*> names;
            if (auto* tu = A::as<A::Tuple>(h->type)) {
                for (const A::Expr* x : tu->elts) {
                    names.push_back(x);
                }
            } else {
                names.push_back(h->type);
            }
            for (const A::Expr* n : names) {
                auto* nm = A::as<A::Name>(n);
                bool ok = nm && (program::EXCEPTIONS().count(Str(nm->id)) || lookup_class(n).has_value());
                if (!ok && !std_exception(n)) {
                    issue(h, Str("dynamic"), Str("except ") + program::unparse(n));
                }
            }
            if (h->has_name) {
                issue(h, Str("syntax"), Str("except as"));
                hs.set(Str(h->name), t_named(Str("extern"), Str("exception")));
            }
        }
        outs.push_back(block(h->body, hs));
    }
    if (!st->orelse.empty()) {
        outs[0] = outs[0] ? block(st->orelse, outs[0]) : std::nullopt;
    }
    SP res_ = std::nullopt;
    for (const SP& o : outs) {
        res_ = join_states(res_, o);
    }
    return res_;
}

SP Eval::s_Break(const A::Stmt* st, const State& state) {
    (void)st;
    if (!loops.empty()) {
        loops.back().first.push_back(state);
    }
    return std::nullopt;
}

SP Eval::s_Continue(const A::Stmt* st, const State& state) {
    (void)st;
    if (!loops.empty()) {
        loops.back().second.push_back(state);
    }
    return std::nullopt;
}

SP Eval::s_While(const A::While* st, const State& state) {
    if (!st->orelse.empty()) {
        issue(st, Str("syntax"), Str("while-else"));
    }
    BodyFn body = [this, st](const State& head) -> SP {
        ev(st->test, head);
        SP h2 = after_access(st->test, head);
        return block(st->body, refine(st->test, h2, true));
    };
    BodyFn exit_refine = [this, st](const State& head) -> SP { return refine(st->test, SP(head), false); };
    BoundFn bound = [this, st](const State& head, const State& init) -> std::optional<Big> { return while_bound(st, head, init); };
    return loop(st, state, body, exit_refine, bound);
}

SP Eval::s_For(const A::For* st, const State& state) {
    if (!st->orelse.empty()) {
        issue(st, Str("syntax"), Str("for-else"));
    }
    T it = ev(st->iter, state);
    auto [elem, n] = iteration(st->iter, it, state);
    SP cur = after_access(st->iter, state);
    BodyFn body = [this, st, elem = elem](const State& head) -> SP {
        State h = assign(st->target, elem, head, nullptr);
        return block(st->body, h);
    };
    BodyFn exit_refine = [](const State& head) -> SP { return head; };
    std::optional<Big> nn = n;
    BoundFn bound = [nn](const State&, const State&) -> std::optional<Big> { return nn; };
    return loop(st, *cur, body, exit_refine, bound);
}

// ---------------------------------------------------------------------------------------- loops
namespace {

// restores the counters a loop's fixpoint raised, also when it throws
struct QuietGuard {
    int64_t& quiet;
    int64_t& suspend;
    QuietGuard(int64_t& q, int64_t& s) : quiet(q), suspend(s) {
        ++quiet;
        ++suspend;
    }
    ~QuietGuard() {
        --quiet;
        --suspend;
    }
};

}  // namespace

SP Eval::loop(const A::Stmt* st, const State& init, const BodyFn& body, const BodyFn& exit_refine, const BoundFn& bound) {
    std::optional<MemoKey> memo_key;
    if (quiet) {
        MemoKey k;
        k.st = st;
        for (const auto& kv : init.items()) {
            k.items.emplace_back(kv.first, p.canon(kv.second));
        }
        std::sort(k.items.begin(), k.items.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
        auto got = loop_memo.find(k);
        if (got != loop_memo.end()) {
            return SP(got->second);
        }
        memo_key = std::move(k);
    }
    State head = init;
    {
        QuietGuard guard(quiet, p.suspend);
        head = fixpoint(st, init, head, body, bound);
    }
    loops.emplace_back();
    body(head);  // the pass that counts
    auto lc = std::move(loops.back());
    loops.pop_back();
    SP out = exit_refine(head);
    for (const State& b : lc.first) {
        out = join_states(out, SP(b));
    }
    if (memo_key) {
        loop_memo[*memo_key] = *out;
    }
    return out;
}

State Eval::fixpoint(const A::Stmt* st, const State& init, State head, const BodyFn& body, const BoundFn& bound) {
    bool widened = false;
    for (int64_t i = 0; i < MAX_LOOP; ++i) {
        loops.emplace_back();
        SP back = body(head);
        auto lc = std::move(loops.back());
        loops.pop_back();
        for (const State& c : lc.second) {
            back = join_states(back, SP(c));
        }
        SP nw = back ? join_states(SP(init), back) : SP(init);
        State new_ = merge_head(head, *nw);
        if (i >= 1) {
            new_ = widen_states(head, new_);
            widened = true;
        }
        if (same_state(new_, head)) {
            break;
        }
        if (i == MAX_LOOP - 1) {  // it does not settle: what still moves is unknown
            for (auto& kv : new_.items()) {
                const T* h = head.find(kv.first);
                if (h && p.canon(*h) != p.canon(kv.second)) {
                    kv.second = k_is(kv.second, "int") ? P::INT() : P::top(Str("a value a loop keeps changing"));
                }
            }
        }
        head = new_;
    }
    if (widened) {  // narrowing: a decreasing pass
        loops.emplace_back();
        SP back = body(head);
        auto lc = std::move(loops.back());
        loops.pop_back();
        for (const State& c : lc.second) {
            back = join_states(back, SP(c));
        }
        SP nw = back ? join_states(SP(init), back) : SP(init);
        State new_ = merge_head(head, *nw);
        State narrowed;
        for (const auto& kv : new_.items()) {
            std::optional<T> n = narrow_one(head.find(kv.first), kv.second);
            narrowed.set(kv.first, *n);
        }
        head = counter_rule(st, narrowed, init, bound);
    }
    return head;
}

State Eval::merge_head(const State& head, State new_) {
    for (const auto& kv : head.items()) {
        if (!new_.contains(kv.first)) {
            new_.set(kv.first, kv.second);
        }
    }
    return new_;
}

std::optional<T> Eval::narrow_one(const T* old, const T& new_) {
    if (old == nullptr || !k_is(*old, "int") || !k_is(new_, "int")) {
        return new_;
    }
    Bound lo = !old->lo ? new_.lo : old->lo;
    Bound hi = !old->hi ? new_.hi : old->hi;
    if (new_.lo && !old->lo) {
        lo = new_.lo;
    }
    if (new_.hi && !old->hi) {
        hi = new_.hi;
    }
    return P::INT(lo, hi);
}

State Eval::widen_states(const State& old, const State& new_) {
    State out = new_;
    for (const auto& kv : new_.items()) {
        const T* o = old.find(kv.first);
        if (o != nullptr && k_is(*o, "int") && k_is(kv.second, "int") && *o != kv.second) {
            out.set(kv.first, P::widen_int(*o, kv.second));
        }
    }
    return out;
}

bool Eval::same_state(const State& a, const State& b) {
    if (!a.same_keys(b)) {
        return false;
    }
    for (const auto& kv : a.items()) {
        if (p.canon(kv.second) != p.canon(b.at(kv.first))) {
            return false;
        }
    }
    return true;
}

SP Eval::join_states(const SP& a, const SP& b) {
    if (!a) {
        return b ? SP(*b) : std::nullopt;
    }
    if (!b) {
        return SP(*a);
    }
    State out = *a;
    for (const auto& kv : b->items()) {
        const T* cur = out.find(kv.first);
        if (cur) {
            T j = p.join(*cur, kv.second);
            out.set(kv.first, j);
        } else {
            out.set(kv.first, kv.second);
        }
    }
    return out;
}

State Eval::counter_rule(const A::Stmt* st, State head, const State& init, const BoundFn& bound) {
    std::vector<Str> loose;
    for (const auto& kv : head.items()) {
        if (k_is(kv.second, "int") && (!kv.second.hi || !kv.second.lo)) {
            loose.push_back(kv.first);
        }
    }
    if (loose.empty()) {
        return head;
    }
    std::optional<Big> n = bound(head, init);
    if (!n) {
        return head;
    }
    const std::vector<A::Stmt*>* body = nullptr;
    if (auto* w = A::as<A::While>(st)) {
        body = &w->body;
    } else if (st->kind == A::StmtKind::For || st->kind == A::StmtKind::AsyncFor) {
        auto* f = A::cast<A::For>(st);
        body = &f->body;
    }
    for (const Str& k : loose) {
        auto step = step_bound(*body, k);
        const T* v0 = init.find(k);
        if (!step || v0 == nullptr || !k_is(*v0, "int")) {
            continue;
        }
        const Big& up = step->first;
        const Big& down = step->second;
        Bound lo = down == Big(0) ? v0->lo : (!v0->lo ? Bound() : Bound(*v0->lo - down * *n));
        Bound hi = up == Big(0) ? v0->hi : (!v0->hi ? Bound() : Bound(*v0->hi + up * *n));
        T cur = head.at(k);
        head.set(k, P::INT(cur.lo ? cur.lo : lo, cur.hi ? cur.hi : hi));
    }
    return head;
}

std::optional<std::pair<Big, Big>> Eval::step_bound(const std::vector<A::Stmt*>& stmts, const Str& var) {
    Big up(0), down(0);
    for (const A::Stmt* st : stmts) {
        auto r = step_one(st, var);
        if (!r) {
            return std::nullopt;
        }
        up = up + r->first;
        down = down + r->second;
    }
    return std::make_pair(up, down);
}

std::optional<std::pair<Big, Big>> Eval::step_one(const A::Stmt* st, const Str& var) {
    if (auto* aug = A::as<A::AugAssign>(st)) {
        auto* nm = A::as<A::Name>(aug->target);
        if (nm && Str(nm->id) == var) {
            auto it = seen.find(static_cast<const A::Node*>(aug->value));
            if (it == seen.end() || !k_is(it->second, "int") || !it->second.lo || !it->second.hi) {
                return std::nullopt;
            }
            const T& t = it->second;
            if (aug->op == A::OpKind::Add) {
                return std::make_pair(bmax(*t.hi, Big(0)), bmax(-*t.lo, Big(0)));
            }
            if (aug->op == A::OpKind::Sub) {
                return std::make_pair(bmax(-*t.lo, Big(0)), bmax(*t.hi, Big(0)));
            }
            return std::nullopt;
        }
    }
    if (st->kind == A::StmtKind::For || st->kind == A::StmtKind::AsyncFor || st->kind == A::StmtKind::While) {
        const std::vector<A::Stmt*>& body = st->kind == A::StmtKind::While ? A::cast<A::While>(st)->body : A::cast<A::For>(st)->body;
        auto inner = step_bound(body, var);
        if (!inner) {
            return std::nullopt;
        }
        if (inner->first == Big(0) && inner->second == Big(0)) {
            return std::make_pair(Big(0), Big(0));
        }
        std::optional<Big> n;
        if (st->kind != A::StmtKind::While) {
            n = range_count(A::cast<A::For>(st)->iter);
        }
        if (!n) {
            return std::nullopt;
        }
        return std::make_pair(inner->first * *n, inner->second * *n);
    }
    bool found_aug = false;
    for (const AnyNode& an : pyengine::walk(AnyNode(st))) {
        if (const A::Expr* e = an.expr()) {
            if (auto* nm = A::as<A::Name>(e); nm && Str(nm->id) == var && nm->ctx == A::ExprCtx::Store) {
                return std::nullopt;
            }
        } else if (const A::Stmt* s = an.stmt()) {
            if (auto* aug = A::as<A::AugAssign>(s)) {
                auto* nm = A::as<A::Name>(aug->target);
                if (nm && Str(nm->id) == var) {
                    found_aug = true;
                    break;
                }
            }
        }
    }
    if (!found_aug) {
        return std::make_pair(Big(0), Big(0));
    }
    if (auto* i = A::as<A::If>(st)) {
        auto a = step_bound(i->body, var);
        auto b = step_bound(i->orelse, var);
        if (!a || !b) {
            return std::nullopt;
        }
        return std::make_pair(bmax(a->first, b->first), bmax(a->second, b->second));
    }
    if (st->kind == A::StmtKind::Try && A::cast<A::Try>(st)->finalbody.empty()) {
        auto* tr = A::cast<A::Try>(st);
        std::vector<std::optional<std::pair<Big, Big>>> parts = {step_bound(tr->body, var)};
        for (const A::ExceptHandler* h : tr->handlers) {
            parts.push_back(step_bound(h->body, var));
        }
        Big up(0), down(0);
        for (const auto& x : parts) {
            if (!x) {
                return std::nullopt;
            }
            up = up + x->first;
            down = down + x->second;
        }
        return std::make_pair(up, down);
    }
    return std::nullopt;
}

std::optional<Big> Eval::range_count(const A::Expr* it) {
    auto sit = seen.find(static_cast<const A::Node*>(it));
    const T* t = sit == seen.end() ? nullptr : &sit->second;
    auto* call = A::as<A::Call>(it);
    if (call && is_name(call->func, "range")) {
        std::vector<const T*> args;
        for (const A::Expr* a : call->args) {
            auto ai = seen.find(static_cast<const A::Node*>(a));
            args.push_back(ai == seen.end() ? nullptr : &ai->second);
        }
        for (const T* a : args) {
            if (a == nullptr || !k_is(*a, "int")) {
                return std::nullopt;
            }
        }
        if (args.size() == 1) {
            return !args[0]->hi ? std::optional<Big>() : std::optional<Big>(bmax(*args[0]->hi, Big(0)));
        }
        if (args.size() >= 2) {
            std::optional<Big> step = call->args.size() == 3 ? int_const(call->args[2]) : std::optional<Big>(Big(1));
            if (!step || *step == Big(0)) {
                return std::nullopt;
            }
            const T& a = *args[0];
            const T& b = *args[1];
            if (*step > Big(0)) {
                if (!b.hi || !a.lo) {
                    return std::nullopt;
                }
                return bmax(floordiv(*b.hi - *a.lo + *step - Big(1), *step), Big(0));
            }
            if (!a.hi || !b.lo) {
                return std::nullopt;
            }
            return bmax(floordiv(*a.hi - *b.lo - *step - Big(1), -*step), Big(0));
        }
    }
    if (t != nullptr && k_in(*t, {"list", "tuple", "str", "bytes", "bytearray", "dict", "set"})) {
        return P::MEM();
    }
    if (call) {
        std::optional<std::string> name;
        if (auto* nm = A::as<A::Name>(call->func)) {
            name = nm->id;
        } else if (auto* at_ = A::as<A::Attribute>(call->func)) {
            name = at_->attr;
        }
        if (name) {
            for (const char* w : {"enumerate", "zip", "reversed", "sorted", "items", "keys", "values"}) {
                if (*name == w) {
                    return P::MEM();
                }
            }
        }
    }
    return std::nullopt;
}

std::optional<Big> Eval::while_bound(const A::While* st, const State& head, const State& init) {
    const A::Expr* t = st->test;
    if (auto* bo = A::as<A::BoolOp>(t); bo && bo->op == A::BoolOpKind::And) {
        for (const A::Expr* v : bo->values) {
            auto r = cmp_bound(v, st, head, init);
            if (r) {
                return r;
            }
        }
        return std::nullopt;
    }
    return cmp_bound(t, st, head, init);
}

std::optional<Big> Eval::cmp_bound(const A::Expr* t, const A::While* st, const State& head, const State& init) {
    (void)head;
    auto* cmp = A::as<A::Compare>(t);
    if (!(cmp && cmp->ops.size() == 1 && A::as<A::Name>(cmp->left))) {
        return std::nullopt;
    }
    Str var(A::cast<A::Name>(cmp->left)->id);
    A::CmpOpKind op = cmp->ops[0];
    if (!(op == A::CmpOpKind::Lt || op == A::CmpOpKind::LtE)) {
        return std::nullopt;
    }
    XInt d = min_step(st->body, var);
    if (!d.inf && d.v == Big(0)) {
        return std::nullopt;
    }
    if (d.inf) {
        return Big(1);
    }
    auto xi = seen.find(static_cast<const A::Node*>(cmp->comparators[0]));
    const T* x = xi == seen.end() ? nullptr : &xi->second;
    const T* v0 = init.find(var);
    if (x == nullptr || !k_is(*x, "int") || !x->hi || v0 == nullptr || !k_is(*v0, "int") || !v0->lo) {
        return std::nullopt;
    }
    return bmax(floordiv(*x->hi - *v0->lo, d.v) + Big(2), Big(0));
}


namespace {

using XI = Eval::XInt;
XI xi_int(const Big& v) { return XI{false, v}; }
XI xi_inf() { return XI{true, Big(0)}; }
XI xmin(const XI& a, const XI& b) {
    if (a.inf) {
        return b;
    }
    if (b.inf) {
        return a;
    }
    return a.v <= b.v ? a : b;
}
XI xadd(const XI& a, const XI& b) {
    if (a.inf || b.inf) {
        return xi_inf();
    }
    return xi_int(a.v + b.v);
}

}  // namespace

Eval::XInt Eval::min_step(const std::vector<A::Stmt*>& stmts, const Str& var) {
    auto r = paths(stmts, var);
    if (!r) {
        return xi_int(Big(0));
    }
    return xmin(r->first, r->second);
}

std::optional<std::pair<Eval::XInt, Eval::XInt>> Eval::paths(const std::vector<A::Stmt*>& stmts, const Str& var) {
    XI acc = xi_int(Big(0)), cont = xi_inf();
    for (const A::Stmt* st : stmts) {
        if (st->kind == A::StmtKind::Break || st->kind == A::StmtKind::Return || st->kind == A::StmtKind::Raise) {
            return std::make_pair(xi_inf(), cont);
        }
        if (st->kind == A::StmtKind::Continue) {
            return std::make_pair(xi_inf(), xmin(cont, acc));
        }
        auto* aug = A::as<A::AugAssign>(st);
        if (aug && A::as<A::Name>(aug->target) && Str(A::cast<A::Name>(aug->target)->id) == var) {
            auto it = seen.find(static_cast<const A::Node*>(aug->value));
            if (aug->op != A::OpKind::Add || it == seen.end() || !k_is(it->second, "int") || !it->second.lo ||
                *it->second.lo < Big(0)) {
                return std::nullopt;
            }
            acc = xadd(acc, xi_int(*it->second.lo));
        } else if (auto* iff = A::as<A::If>(st)) {
            auto a = paths(iff->body, var);
            auto b = paths(iff->orelse, var);
            if (!a || !b) {
                return std::nullopt;
            }
            cont = xmin(xmin(cont, xadd(acc, a->second)), xadd(acc, b->second));
            XI fall = xmin(a->first, b->first);
            if (fall.inf) {
                return std::make_pair(xi_inf(), cont);
            }
            acc = xadd(acc, fall);
        } else if (!grows_only(st, var)) {
            return std::nullopt;
        }
    }
    return std::make_pair(acc, cont);
}

bool Eval::grows_only(const A::Stmt* st, const Str& var) {
    std::vector<AnyNode> nodes = pyengine::walk(AnyNode(st));
    std::unordered_set<const void*> aug;
    for (const AnyNode& an : nodes) {
        if (const A::Stmt* s = an.stmt()) {
            if (auto* a = A::as<A::AugAssign>(s)) {
                aug.insert(static_cast<const A::Node*>(a->target));
            }
        }
    }
    bool is_loop = st->kind == A::StmtKind::For || st->kind == A::StmtKind::AsyncFor || st->kind == A::StmtKind::While;
    for (const AnyNode& an : nodes) {
        if (const A::Expr* e = an.expr()) {
            if (auto* nm = A::as<A::Name>(e); nm && Str(nm->id) == var && nm->ctx == A::ExprCtx::Store &&
                                               !aug.count(static_cast<const A::Node*>(e))) {
                return false;
            }
        } else if (const A::Stmt* s = an.stmt()) {
            if (auto* a = A::as<A::AugAssign>(s)) {
                if (auto* nm = A::as<A::Name>(a->target); nm && Str(nm->id) == var) {
                    auto it = seen.find(static_cast<const A::Node*>(a->value));
                    if (a->op != A::OpKind::Add || it == seen.end() || !k_is(it->second, "int") || !it->second.lo ||
                        *it->second.lo < Big(0)) {
                        return false;
                    }
                }
            }
            if (s->kind == A::StmtKind::Continue && !is_loop) {
                return false;
            }
        }
    }
    return true;
}

std::pair<T, std::optional<Big>> Eval::iteration(const A::Expr* node, const T& t, const State& state) {
    (void)state;
    if (k_is(t, "bottom")) {
        return {P::BOTTOM(), std::nullopt};
    }
    if (k_is(t, "top")) {
        return {t, std::nullopt};
    }
    if (k_in(t, {"list", "set"})) {
        if (k_is(t, "set")) {
            issue(node, Str("dynamic"), Str("set iteration order"));
        }
        return {p.elem[static_cast<size_t>(p.find(*t.site))], P::MEM()};
    }
    if (k_is(t, "dict")) {
        int64_t s = p.find(*t.site);
        p.make_dynamic(s, Str("iterated"));
        return {p.keys[static_cast<size_t>(s)], P::MEM()};
    }
    if (k_is(t, "tuple")) {
        T e = P::BOTTOM();
        for (const T& a : t.args) {
            e = p.join(e, a);
        }
        if (k_is(e, "top")) {
            return {issue(node, Str("conflict"), Str("iterating a tuple of mixed items")), std::nullopt};
        }
        return {e, Big(static_cast<int64_t>(t.args.size()))};
    }
    if (k_is(t, "str")) {
        return {P::STR(), P::MEM()};
    }
    if (k_in(t, {"bytes", "bytearray"})) {
        return {P::u(8), !t.hi ? std::optional<Big>(P::MEM()) : std::optional<Big>(*t.hi)};
    }
    if (k_is(t, "extern") && t.name && str_starts(*t.name, "iter:")) {
        // (the Python reads `self.p.iters`, which no Program has: a type of this name is never made)
        return {P::BOTTOM(), P::MEM()};
    }
    if (k_is(t, "iter")) {
        return {t.args[0], t.lo};
    }
    return {issue(node, Str("conflict"), Str("iterating a ") + t.kind), std::nullopt};
}

// ---------------------------------------------------------------------------------------- assignment
State Eval::assign(const A::Expr* tgt, const T& t0, State state, const A::Expr* value_node, bool aug) {
    (void)value_node;
    T t = t0;
    if (auto* nm = A::as<A::Name>(tgt)) {
        Str id(nm->id);
        state.set(id, t);
        note(tgt, t, rt("local"));
        if (k_in(t, {"list", "dict", "set"})) {
            p.hint(*t.site, id);
        }
        return state;
    }
    if (auto* at_ = A::as<A::Attribute>(tgt)) {
        T ot = ev(at_->value, state);
        Str attr(at_->attr);
        if (k_is(ot, "inst")) {
            t = fit(tgt, t, Str("attribute ") + attr);
            contribute_attr(*ot.name, attr, t);
            note(tgt, t, rt("field", {rs(*ot.name), rs(attr)}));
            if (k_in(t, {"list", "dict", "set"})) {
                p.hint(*t.site, attr);
            }
        } else if (k_is(ot, "bottom")) {
        } else if (k_is(ot, "top")) {
            note(tgt, ot);
        } else {
            note(tgt, issue(tgt, Str("dynamic"), Str("attribute store on a ") + ot.kind));
        }
        return state;
    }
    if (auto* sub = A::as<A::Subscript>(tgt)) {
        T ot = ev(sub->value, state);
        if (auto* sl = A::as<A::Slice>(sub->slice)) {
            ev_slice_parts(sl, state);
            if (k_is(ot, "list") && k_is(t, "list")) {
                T e = p.elem[static_cast<size_t>(p.find(*t.site))];
                p.grow(p.elem, *ot.site, e);
            } else if (k_is(ot, "bytearray") && k_in(t, {"bytes", "bytearray"})) {
            } else if (!k_is(ot, "bottom")) {
                issue(tgt, Str("syntax"), Str("slice assignment to a ") + ot.kind);
            }
            note(tgt, t, rt("slice_store", {rs(ot.kind)}));
            return state;
        }
        T kt = ev(sub->slice, state);
        if (k_is(ot, "list")) {
            p.grow(p.elem, *ot.site, t);
            need_int_index(sub->slice, kt);
            note(tgt, t, rt("index_store", {rs(Str("list"))}));
        } else if (k_is(ot, "bytearray")) {
            if (!k_is(t, "int") && !k_is(t, "bottom")) {
                issue(tgt, Str("conflict"), Str("bytearray item of ") + t.kind);
            }
            note(tgt, P::u(8), rt("index_store", {rs(Str("bytearray"))}));
        } else if (k_is(ot, "dict")) {
            std::optional<Str> key = str_const(sub->slice);
            int64_t s = p.find(*ot.site);
            t = fit(tgt, t, Str("field ") + (key ? *key : Str("None")));
            if (key) {
                p.grow_field(s, *key, t);
                if ((aug || !p.builder_targets.count(static_cast<const A::Node*>(tgt))) && live()) {
                    p.mark_mutated(s);
                }
            } else {
                p.make_dynamic(s, Str("a key that is not a constant string"));
                p.grow(p.keys, s, kt);
                p.grow(p.elem, s, t);
            }
            note(tgt, t, rt("dict_store", {ropt_s(key)}));
        } else if (k_in(ot, {"bottom", "top"})) {
        } else {
            issue(tgt, Str("conflict"), Str("item store on a ") + ot.kind);
        }
        return state;
    }
    const std::vector<A::Expr*>* elts = nullptr;
    if (auto* tu = A::as<A::Tuple>(tgt)) {
        elts = &tu->elts;
    } else if (auto* li = A::as<A::List>(tgt)) {
        elts = &li->elts;
    }
    if (elts) {
        for (const A::Expr* e : *elts) {
            if (A::as<A::Starred>(e)) {
                issue(tgt, Str("syntax"), Str("starred unpacking"));
                return state;
            }
        }
        if (k_is(t, "tuple")) {
            if (t.args.size() != elts->size()) {
                issue(tgt, Str("conflict"),
                      Str("unpacking ") + i2s(static_cast<int64_t>(t.args.size())) + Str(" items into ") +
                          i2s(static_cast<int64_t>(elts->size())));
                return state;
            }
            for (size_t i = 0; i < elts->size(); ++i) {
                state = assign((*elts)[i], t.args[i], state, nullptr);
            }
            note(tgt, t, rt("unpack"));
            return state;
        }
        if (k_is(t, "bottom")) {
            for (const A::Expr* e : *elts) {
                state = assign(e, P::BOTTOM(), state, nullptr);
            }
            return state;
        }
        if (k_is(t, "top")) {
            for (const A::Expr* e : *elts) {
                state = assign(e, t, state, nullptr);
            }
            return state;
        }
        issue(tgt, Str("conflict"), Str("unpacking a ") + t.kind + Str(" (its length is not known to the type)"));
        for (const A::Expr* e : *elts) {
            state = assign(e, P::top(Str("unpack")), state, nullptr);
        }
        return state;
    }
    issue(tgt, Str("syntax"), Str("assignment to ") + Str(A::name_of(tgt->kind)));
    return state;
}

// A record made empty (or partly) and filled by the statements right after it: the keys those
// statements set are set by every creation. (Called with the Assign; scans its block.)
void Eval::builder_keys(const A::Assign* st, const T& t) {
    if (!k_is(t, "dict")) {
        return;
    }
    Str name(A::cast<A::Name>(st->targets[0])->id);
    const std::vector<A::Stmt*>* block_ = p.block_of(st);
    if (block_ == nullptr) {
        return;
    }
    program::StrSet keys;
    auto lk = p.literal_keys.find(static_cast<const A::Node*>(st->value));
    if (lk != p.literal_keys.end()) {
        keys.insert(lk->second.begin(), lk->second.end());
    }
    size_t i = 0;
    while (i < block_->size() && (*block_)[i] != st) {
        ++i;
    }
    if (i == block_->size()) {
        throw py::ValueError("statement is not in list");
    }
    ++i;
    while (i < block_->size()) {
        const A::Stmt* s2 = (*block_)[i];
        auto* as2 = A::as<A::Assign>(s2);
        const A::Subscript* sub = nullptr;
        if (as2 && as2->targets.size() == 1) {
            sub = A::as<A::Subscript>(as2->targets[0]);
        }
        if (sub && A::as<A::Name>(sub->value) && Str(A::cast<A::Name>(sub->value)->id) == name && str_const(sub->slice) &&
            reads_fields_only(as2->value, name)) {
            keys.insert(*str_const(sub->slice));
            p.builder_targets.insert(static_cast<const A::Node*>(sub));
        } else {
            bool uses = false;
            for (const AnyNode& an : pyengine::walk(AnyNode(s2))) {
                if (auto* nm = A::as<A::Name>(an.expr()); nm && Str(nm->id) == name) {
                    uses = true;
                    break;
                }
            }
            if (uses) {
                break;
            }
        }
        ++i;
    }
    if (live()) {
        p.created(*t.site, keys);
    }
}

// `value` uses the record `name` only to read its fields by constant key
bool Eval::reads_fields_only(const A::Expr* value, const Str& name) {
    std::unordered_set<const void*> ok;
    std::vector<AnyNode> nodes = pyengine::walk(AnyNode(value));
    for (const AnyNode& an : nodes) {
        if (auto* sub = A::as<A::Subscript>(an.expr())) {
            auto* nm = A::as<A::Name>(sub->value);
            if (nm && Str(nm->id) == name && str_const(sub->slice) && sub->ctx == A::ExprCtx::Load) {
                ok.insert(static_cast<const A::Node*>(sub->value));
            }
        }
    }
    for (const AnyNode& an : nodes) {
        if (auto* nm = A::as<A::Name>(an.expr()); nm && Str(nm->id) == name && !ok.count(static_cast<const A::Node*>(nm))) {
            return false;
        }
    }
    return true;
}

// ---------------------------------------------------------------------------------------- refinement
SP Eval::refine(const A::Expr* test, const SP& state, bool truth) {
    if (!state) {
        return std::nullopt;
    }
    if (auto* un = A::as<A::UnaryOp>(test); un && un->op == A::UnaryOpKind::Not) {
        return refine(un->operand, state, !truth);
    }
    if (auto* bo = A::as<A::BoolOp>(test)) {
        if ((bo->op == A::BoolOpKind::And) == truth) {  // and/True, or/False: every part holds
            SP s = state;
            for (const A::Expr* v : bo->values) {
                s = refine(v, s, truth);
            }
            return s;
        }
        SP outs = std::nullopt;  // one of them failed: join each case
        SP s = state;
        for (const A::Expr* v : bo->values) {
            outs = join_states(outs, refine(v, s, truth));
            s = refine(v, s, !truth);
        }
        return outs;
    }
    if (auto* nm = A::as<A::Name>(test); nm && state->contains(Str(nm->id))) {
        Str id(nm->id);
        T t = state->at(id);
        State s = *state;
        if (k_is(t, "opt") && truth) {
            s.set(id, t.args[0]);
        } else if (k_is(t, "opt") && !truth && k_is(t.args[0], "inst")) {
            s.set(id, P::NONE());
        } else if (k_is(t, "int")) {
            if (!truth) {
                s.set(id, P::meet(t, Big(0), Big(0)));
            }
        }
        return s;
    }
    if (auto* call = A::as<A::Call>(test); call && is_name(call->func, "isinstance") && call->args.size() == 2 &&
                                          A::as<A::Name>(call->args[0])) {
        return state;
    }
    if (auto* cmp = A::as<A::Compare>(test); cmp && cmp->ops.size() >= 1) {
        const A::Expr* left = cmp->left;
        State s = *state;
        for (size_t i = 0; i < cmp->ops.size(); ++i) {
            const A::Expr* right = cmp->comparators[i];
            s = refine_cmp(left, cmp->ops[i], right, s, truth);
            if (!truth && cmp->ops.size() > 1) {
                return s;
            }
            left = right;
        }
        return s;
    }
    return state;
}

namespace {

// _bound
T bound_of(const T& v, const char* o, const T& other) {
    std::string op = o;
    if (op == "<") {
        return P::meet(v, std::nullopt, !other.hi ? Bound() : Bound(*other.hi - Big(1)));
    }
    if (op == "<=") {
        return P::meet(v, std::nullopt, other.hi);
    }
    if (op == ">") {
        return P::meet(v, !other.lo ? Bound() : Bound(*other.lo + Big(1)), std::nullopt);
    }
    if (op == ">=") {
        return P::meet(v, other.lo, std::nullopt);
    }
    if (op == "==") {
        return P::meet(v, other.lo, other.hi);
    }
    return v;
}

}  // namespace

State Eval::refine_cmp(const A::Expr* left, A::CmpOpKind op, const A::Expr* right, State state, bool truth) {
    using C = A::CmpOpKind;
    auto* lname = A::as<A::Name>(left);
    auto* rconst = A::as<A::Constant>(right);
    bool right_none = rconst && rconst->value.kind == A::ConstValue::Kind::None;
    if ((op == C::Is || op == C::IsNot) && right_none && lname && state.contains(Str(lname->id))) {
        bool is_none = (op == C::Is) == truth;
        Str id(lname->id);
        T t = state.at(id);
        state.set(id, is_none ? P::NONE() : P::strip_opt(t));
        return state;
    }
    if ((op == C::Eq || op == C::NotEq) && right_none && lname && state.contains(Str(lname->id))) {
        return refine_cmp(left, op == C::Eq ? C::Is : C::IsNot, right, state, truth);
    }
    const char* o = nullptr;
    switch (op) {
        case C::Lt: o = "<"; break;
        case C::LtE: o = "<="; break;
        case C::Gt: o = ">"; break;
        case C::GtE: o = ">="; break;
        case C::Eq: o = "=="; break;
        case C::NotEq: o = "!="; break;
        default: break;
    }
    if (o == nullptr) {
        return state;
    }
    if (!truth) {
        std::string s = o;
        if (s == "<") o = ">=";
        else if (s == "<=") o = ">";
        else if (s == ">") o = "<=";
        else if (s == ">=") o = "<";
        else if (s == "==") o = "!=";
        else if (s == "!=") o = "==";
    }
    auto li = seen.find(static_cast<const A::Node*>(left));
    auto ri_ = seen.find(static_cast<const A::Node*>(right));
    const T* lt = li == seen.end() ? nullptr : &li->second;
    const T* rt_ = ri_ == seen.end() ? nullptr : &ri_->second;
    if (lname && state.contains(Str(lname->id)) && k_is(state.at(Str(lname->id)), "int") && rt_ != nullptr &&
        k_is(*rt_, "int")) {
        Str id(lname->id);
        state.set(id, bound_of(state.at(id), o, *rt_));
    }
    auto* rname = A::as<A::Name>(right);
    if (rname && state.contains(Str(rname->id)) && k_is(state.at(Str(rname->id)), "int") && lt != nullptr &&
        k_is(*lt, "int")) {
        std::string s = o;
        const char* flip = s == "<" ? ">" : s == "<=" ? ">=" : s == ">" ? "<" : s == ">=" ? "<=" : s == "==" ? "==" : "!=";
        Str id(rname->id);
        state.set(id, bound_of(state.at(id), flip, *lt));
    }
    return state;
}

SP Eval::after_access(const A::Expr* node, const SP& state_) {
    if (!state_) {
        return std::nullopt;
    }
    State state = *state_;
    for (const A::Expr* n : p.access_nodes(node)) {
        if (auto* sub = A::as<A::Subscript>(n); sub && A::as<A::Name>(sub->slice) && state.contains(Str(A::cast<A::Name>(sub->slice)->id))) {
            auto si = seen.find(static_cast<const A::Node*>(sub->value));
            if (si != seen.end() && k_in(si->second, {"bytes", "bytearray", "list", "str", "tuple"})) {
                const T& st = si->second;
                Str id(A::cast<A::Name>(sub->slice)->id);
                T v = state.at(id);
                if (k_is(v, "int")) {
                    Big top = (!k_in(st, {"bytes", "bytearray"}) || !st.hi) ? P::MEM() : *st.hi;
                    state.set(id, P::meet(v, -top, top - Big(1)));
                }
            }
        } else if (auto* call = A::as<A::Call>(n)) {
            std::optional<Str> fq = res_fn(call);
            if (fq) {
                auto idx_it = p.index_params.find(*fq);
                Fn* f = p.fns.at(*fq);
                std::vector<Str> names = f->bound();
                for (size_t i = 0; i < call->args.size() && i < names.size(); ++i) {
                    const A::Expr* a = call->args[i];
                    auto* an = A::as<A::Name>(a);
                    if (idx_it != p.index_params.end() && idx_it->second.count(names[i]) && an &&
                        state.contains(Str(an->id)) && k_is(state.at(Str(an->id)), "int")) {
                        Str id(an->id);
                        state.set(id, P::meet(state.at(id), -P::MEM(), P::MEM()));
                    }
                }
            } else if (auto* fa = A::as<A::Attribute>(call->func);
                       fa && (fa->attr == "unpack_from" || fa->attr == "pack_into") && call->args.size() >= 3 &&
                       A::as<A::Name>(call->args[2]) && state.contains(Str(A::cast<A::Name>(call->args[2])->id)) &&
                       k_is(state.at(Str(A::cast<A::Name>(call->args[2])->id)), "int")) {
                Str id(A::cast<A::Name>(call->args[2])->id);
                state.set(id, P::meet(state.at(id), -P::MEM(), P::MEM()));
            }
        }
    }
    return state;
}

std::optional<Str> Eval::res_fn(const A::Expr* call) {
    const void* k = static_cast<const A::Node*>(call);
    const Res* r = nullptr;
    if (record) {
        auto it = res.find(k);
        if (it != res.end()) {
            r = &it->second;
        }
    }
    if (r == nullptr) {
        auto it = res_quiet.find(k);
        if (it != res_quiet.end()) {
            r = &it->second;
        }
    }
    if (r != nullptr && r->in({"fn", "method"})) {
        return r->s(1);
    }
    return std::nullopt;
}

// ---------------------------------------------------------------------------------------- expressions
T Eval::ev(const A::Expr* node, const State& state) {
    TR r = e_dispatch(node, state);
    if (r.res && A::as<A::Call>(node) && r.res->in({"fn", "method"})) {
        res_quiet[static_cast<const A::Node*>(node)] = *r.res;
    }
    return note(node, r.t, r.res);
}

TR Eval::e_dispatch(const A::Expr* n, const State& state) {
    using K = A::ExprKind;
    switch (n->kind) {
        case K::Constant: return e_Constant(A::cast<A::Constant>(n), state);
        case K::JoinedStr: return e_JoinedStr(A::cast<A::JoinedStr>(n), state);
        case K::FormattedValue: return TR(ev(A::cast<A::FormattedValue>(n)->value, state));
        case K::Name: return e_Name(A::cast<A::Name>(n), state);
        case K::Attribute: return e_Attribute(A::cast<A::Attribute>(n), state);
        case K::Subscript: return e_Subscript(A::cast<A::Subscript>(n), state);
        case K::BinOp: {
            auto* b = A::cast<A::BinOp>(n);
            T l = ev(b->left, state);
            T r = ev(b->right, state);
            return TR(binop(n, b->op, l, r, b->left, b->right));
        }
        case K::UnaryOp: return e_UnaryOp(A::cast<A::UnaryOp>(n), state);
        case K::BoolOp: return e_BoolOp(A::cast<A::BoolOp>(n), state);
        case K::Compare: return e_Compare(A::cast<A::Compare>(n), state);
        case K::IfExp: return e_IfExp(A::cast<A::IfExp>(n), state);
        case K::NamedExpr: {
            ev(A::cast<A::NamedExpr>(n)->value, state);
            return TR(issue(n, Str("syntax"), Str("walrus")));
        }
        case K::Lambda: return TR(issue(n, Str("dynamic"), Str("first-class lambda")));
        case K::Starred: {
            ev(A::cast<A::Starred>(n)->value, state);
            return TR(issue(n, Str("syntax"), Str("starred")));
        }
        case K::Await: return TR(issue(n, Str("dynamic"), Str("await")));
        case K::Yield: {
            if (auto* v = A::cast<A::Yield>(n)->value) {
                ev(v, state);
            }
            return TR(P::BOTTOM());
        }
        case K::YieldFrom: {
            if (auto* v = A::cast<A::YieldFrom>(n)->value) {
                ev(v, state);
            }
            return TR(P::BOTTOM());
        }
        case K::Tuple: return e_Tuple(A::cast<A::Tuple>(n), state);
        case K::List: return e_List(A::cast<A::List>(n), state);
        case K::Set: return e_Set(A::cast<A::Set>(n), state);
        case K::Dict: return e_Dict(A::cast<A::Dict>(n), state);
        case K::ListComp: {
            auto* c = A::cast<A::ListComp>(n);
            State st = comp_scope(c->generators, state);
            int64_t s = p.new_site(n, Str("list"), where(n));
            T et = ev(c->elt, st);
            p.grow(p.elem, s, et);
            return TR(t_site(PS("list"), s));
        }
        case K::SetComp: {
            auto* c = A::cast<A::SetComp>(n);
            State st = comp_scope(c->generators, state);
            int64_t s = p.new_site(n, Str("set"), where(n));
            T et = ev(c->elt, st);
            p.grow(p.elem, s, et);
            return TR(t_site(PS("set"), s));
        }
        case K::DictComp: {
            auto* c = A::cast<A::DictComp>(n);
            State st = comp_scope(c->generators, state);
            int64_t s = p.new_site(n, Str("dict"), where(n));
            p.make_dynamic(s, Str("a comprehension"));
            T kt = ev(c->key, st);
            p.grow(p.keys, s, kt);
            T vt = ev(c->value, st);
            p.grow(p.elem, s, vt);
            return TR(t_site(PS("dict"), s));
        }
        case K::GeneratorExp: {
            auto* c = A::cast<A::GeneratorExp>(n);
            State st = comp_scope(c->generators, state);
            T e = ev(c->elt, st);
            return TR(T(PS("iter"), {e}, P::MEM()));
        }
        case K::Call: return e_Call(A::cast<A::Call>(n), state);
        default: {
            // type(node).__name__.lower()
            std::string nm = A::name_of(n->kind);
            for (char& c : nm) {
                c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
            }
            return TR(issue(n, Str("syntax"), Str(nm)));
        }
    }
}

TR Eval::e_Constant(const A::Constant* n, const State& state) {
    (void)state;
    using CK = A::ConstValue::Kind;
    const A::ConstValue& v = n->value;
    switch (v.kind) {
        case CK::None: return TR(P::NONE());
        case CK::True:
        case CK::False: return TR(P::BOOL());
        case CK::Int: return TR(P::const_int(v.fits_i64 ? Big(v.i64) : Big::from_decimal(v.text)));
        case CK::Float: return TR(P::FLOAT());
        case CK::Str: return TR(P::STR());
        case CK::Bytes: return TR(P::bytes_of(Big(static_cast<int64_t>(v.text.size()))));
        case CK::Complex: return TR(issue(n, Str("syntax"), Str("constant complex")));
        case CK::Ellipsis: return TR(issue(n, Str("syntax"), Str("constant ellipsis")));
    }
    return TR(P::BOTTOM());
}

TR Eval::e_JoinedStr(const A::JoinedStr* n, const State& state) {
    for (const A::Expr* ve : n->values) {
        auto* v = A::as<A::FormattedValue>(ve);
        if (!v) {
            continue;
        }
        T t = ev(v->value, state);
        if (v->format_spec) {
            auto* spec_js = A::as<A::JoinedStr>(v->format_spec);
            std::optional<Str> spec = Str();
            if (spec_js) {
                for (const A::Expr* x : spec_js->values) {
                    if (!A::as<A::Constant>(x)) {
                        spec = std::nullopt;
                        break;
                    }
                }
                if (spec) {
                    Str joined;
                    for (const A::Expr* x : spec_js->values) {
                        joined += Str(A::cast<A::Constant>(x)->value.text);
                    }
                    spec = joined;
                }
            }
            if (!spec) {
                issue(v, Str("dynamic"), Str("computed format spec"));
            } else {
                check_format(v, t, *spec);
            }
        } else if (v->conversion != -1 && v->conversion != 114 && v->conversion != 115) {
            issue(v, Str("syntax"), Str("f-string conversion !") + Str(std::string(1, static_cast<char>(v->conversion))));
        } else {
            check_str(v, t, v->conversion == 114);
        }
    }
    return TR(P::STR());
}

void Eval::check_str(const A::Node* node, const T& t, bool as_repr) {
    if (k_in(t, {"int", "str", "bool", "none", "bytes", "bytearray", "bottom", "top"})) {
        return;
    }
    if (k_is(t, "float") && !as_repr) {
        return;
    }
    if (k_is(t, "float")) {
        return;
    }
    if (k_is(t, "opt") && k_in(t.args[0], {"int", "str", "bool"})) {
        return;
    }
    if (k_in(t, {"tuple", "list"}) && as_repr) {
        return;
    }
    issue(node, Str("syntax"), Str("str() of a ") + t.kind);
}

void Eval::check_format(const A::Node* node, const T& t, const Str& spec) {
    static const py::re::Pattern pat(Str("(?:.?[<>^])?[+\\- ]?#?0?\\d*,?(?:\\.\\d+)?[bdoxXcsfeEg%]?"), 0);
    if (!pat.fullmatch(spec)) {
        issue(node, Str("syntax"), Str("format spec ") + program::repr_str(spec));
        return;
    }
    // spec[-1:] in 'fFeEg%'  (an empty spec is in any string)
    bool floaty = spec.empty();
    if (!spec.empty()) {
        char32_t c = spec.at_(spec.size() - 1);
        floaty = c == U'f' || c == U'F' || c == U'e' || c == U'E' || c == U'g' || c == U'%';
    }
    if (k_in(t, {"int", "bool"}) && floaty) {
        return;
    }
    if (!k_in(t, {"int", "str", "float", "bool", "bottom", "top"})) {
        issue(node, Str("syntax"), Str("format of a ") + t.kind);
    }
}

TR Eval::e_Name(const A::Name* n, const State& state) {
    Str name(n->id);
    if (const T* t = state.find(name)) {
        return TR(*t, rt("local"));
    }
    if (locals.count(name)) {
        return TR(P::BOTTOM(), rt("local"));  // read before any assignment reaches here
    }
    std::optional<Ref> r = lookup(name);
    if (!r) {
        return TR(issue(n, Str("unresolved"), name));
    }
    return TR(ref_type(n, *r), *r);
}

std::optional<Ref> Eval::lookup(const Str& name) {
    if (local_imports.contains(name)) {
        return local_imports.at(name);
    }
    if (m != nullptr) {
        std::optional<Ref> r = p.resolve_global(*m, name);
        if (r) {
            return r;
        }
    }
    if (BUILTIN_FUNCS().count(name)) {
        return rt("builtin", {rs(name)});
    }
    if (program::EXCEPTIONS().count(name)) {
        return rt("exc", {rs(name)});
    }
    for (const char* c : {"True", "False", "None", "__name__", "__file__", "__doc__"}) {
        if (lit_eq(name, c)) {
            return rt("const", {rs(name)});
        }
    }
    return std::nullopt;
}

std::optional<Str> Eval::lookup_class(const A::Expr* n) {
    if (auto* nm = A::as<A::Name>(n)) {
        std::optional<Ref> r = lookup(Str(nm->id));
        if (r && r->is("class")) {
            return r->s(1);
        }
    }
    return std::nullopt;
}

T Eval::ref_type(const A::Node* n, const Ref& r) {
    const Str& k = r.kind;
    if (lit_eq(k, "global")) {
        return p.global_type(r.s(1), r.s(2));
    }
    if (lit_eq(k, "fn")) {
        return t_named(Str("fn"), r.s(1));
    }
    if (lit_eq(k, "class")) {
        return t_named(Str("cls"), r.s(1));
    }
    if (lit_eq(k, "module")) {
        return t_named(Str("module"), r.s(1));
    }
    if (lit_eq(k, "stdmod")) {
        return t_named(Str("module"), Str("std:") + r.s(1));
    }
    if (lit_eq(k, "stdobj")) {
        return t_named(Str("extern"), Str("std:") + r.s(1));
    }
    if (lit_eq(k, "builtin")) {
        return t_named(Str("fn"), Str("builtin:") + r.s(1));
    }
    if (lit_eq(k, "exc")) {
        return t_named(Str("cls"), Str("exc:") + r.s(1));
    }
    if (lit_eq(k, "const")) {
        if (lit_eq(r.s(1), "__doc__")) {
            return m->has_docstring() ? P::STR() : P::NONE();
        }
        if (lit_eq(r.s(1), "__file__") || lit_eq(r.s(1), "__name__")) {
            return issue(n, Str("dynamic"), r.s(1));
        }
        return P::BOTTOM();
    }
    if (lit_eq(k, "extern")) {
        const std::u32string& u = r.s(1).u32();
        size_t dot = u.find(U'.');
        return issue(n, Str("extern"), dot == std::u32string::npos ? r.s(1) : Str(u.substr(0, dot)));
    }
    if (lit_eq(k, "missing")) {
        return issue(n, Str("unresolved"), r.s(1));
    }
    // str(r): the tuple's repr
    Str text = Str("(") + program::repr_str(r.kind);
    for (const TupItem& it : r.items) {
        text += Str(", ");
        if (auto* s = std::get_if<Str>(&it)) {
            text += program::repr_str(*s);
        } else if (auto* b = std::get_if<Big>(&it)) {
            text += Str(b->str());
        } else if (auto* bo = std::get_if<bool>(&it)) {
            text += Str(*bo ? "True" : "False");
        } else {
            text += Str("None");
        }
    }
    text += r.items.empty() ? Str(",)") : Str(")");
    return issue(n, Str("unresolved"), text);
}

TR Eval::e_Attribute(const A::Attribute* n, const State& state) {
    T ot = ev(n->value, state);
    const Str& k = ot.kind;
    Str attr(n->attr);
    if (lit_eq(k, "bottom") || lit_eq(k, "top")) {
        return TR(ot);
    }
    if (lit_eq(k, "inst")) {
        const Str& cq = *ot.name;
        Fn* f = p.find_method(cq, attr);
        if (f != nullptr) {
            if (lit_eq(f->kind, "property")) {
                return TR(p.ret(f->qname), rt("property", {rs(f->qname)}));
            }
            return TR(t_named(Str("fn"), Str("bound:") + f->qname), rt("bound", {rs(f->qname)}));
        }
        std::optional<T> t = p.attr_type(cq, attr);
        if (!t) {
            return TR(issue(n, Str("unresolved"), last_part(cq) + Str(".") + attr));
        }
        std::optional<Str> owner = p.attr_owner(cq, attr);
        return TR(*t, rt("field", {rs(owner ? *owner : cq), rs(attr)}));
    }
    if (lit_eq(k, "module")) {
        if (str_starts(*ot.name, "std:")) {
            Str full = ot.name->slice_from(4) + Str(".") + attr;
            const auto& sa = STD_ATTRS();
            if (sa.contains(full)) {
                return TR(sa.at(full), rt("stdattr", {rs(full)}));
            }
            return TR(t_named(Str("extern"), Str("std:") + full), rt("stdattr", {rs(full)}));
        }
        Module& mod = *p.modules.at(*ot.name);
        std::optional<Ref> r = p.resolve_global(mod, attr);
        if (!r) {
            Str full = *ot.name + Str(".") + attr;
            if (p.modules.contains(full)) {
                return TR(t_named(Str("module"), full), rt("module", {rs(full)}));
            }
            return TR(issue(n, Str("unresolved"), full));
        }
        return TR(ref_type(n, *r), *r);
    }
    if (lit_eq(k, "cls")) {
        if (str_starts(*ot.name, "exc:")) {
            return TR(issue(n, Str("dynamic"), Str("exception class attribute")));
        }
        Class* c = p.classes.at(*ot.name);
        Fn* f = p.find_method(*ot.name, attr);
        if (f != nullptr) {
            if (lit_eq(f->kind, "static")) {
                return TR(t_named(Str("fn"), f->qname), rt("fn", {rs(f->qname)}));
            }
            return TR(issue(n, Str("dynamic"), Str("unbound method")));
        }
        for (const Str& cq : p.mro(*ot.name)) {
            if (p.classes.at(cq)->class_attrs.contains(attr)) {
                return TR(p.class_attr_type(cq, attr), rt("classattr", {rs(cq), rs(attr)}));
            }
        }
        return TR(issue(n, Str("unresolved"), c->name + Str(".") + attr));
    }
    if (lit_eq(k, "extern")) {
        if (str_starts(*ot.name, "std:")) {
            Str full = ot.name->slice_from(4) + Str(".") + attr;
            const auto& sa = STD_ATTRS();
            if (sa.contains(full)) {
                return TR(sa.at(full), rt("stdattr", {rs(full)}));
            }
            return TR(t_named(Str("extern"), Str("std:") + full), rt("stdattr", {rs(full)}));
        }
        return TR(ot);
    }
    if (k_in(ot, {"str", "bytes", "bytearray", "list", "dict", "set", "int", "float", "tuple"})) {
        return TR(t_named(Str("fn"), Str("bmethod:") + k + Str(".") + attr), rt("bmethod", {rs(k + Str(".") + attr)}));
    }
    if (lit_eq(k, "opt")) {
        return TR(issue(n, Str("conflict"), Str("attribute .") + attr + Str(" of a value that may be None")));
    }
    return TR(issue(n, Str("conflict"), Str("attribute .") + attr + Str(" of a ") + k));
}

TR Eval::e_Subscript(const A::Subscript* n, const State& state) {
    T ot = ev(n->value, state);
    const Str k = ot.kind;
    if (auto* sl = A::as<A::Slice>(n->slice)) {
        std::vector<T> parts = ev_slice_parts(sl, state);
        const A::Expr* exprs[3] = {sl->lower, sl->upper, sl->step};
        for (int i = 0; i < 3; ++i) {
            if (exprs[i] != nullptr && !k_in(parts[static_cast<size_t>(i)], {"int", "bottom", "top", "none"})) {
                issue(exprs[i], Str("conflict"), Str("slice bound of ") + parts[static_cast<size_t>(i)].kind);
            }
        }
        if (sl->step) {
            std::optional<Big> sv = int_const(sl->step);
            if (!sv || *sv == Big(0)) {
                issue(n, Str("syntax"), Str("slice with a computed step"));
            }
        }
        if (lit_eq(k, "bytes") || lit_eq(k, "bytearray")) {
            Bound cap = slice_cap(sl, ot.hi);
            return TR(T(k, {}, std::nullopt, cap), rt("slice", {rs(k)}));
        }
        if (lit_eq(k, "str")) {
            return TR(P::STR(), rt("slice", {rs(k)}));
        }
        if (lit_eq(k, "list")) {
            int64_t s = p.new_site(n, Str("list"), where(n));
            T e = p.elem[static_cast<size_t>(p.find(*ot.site))];
            p.grow(p.elem, s, e);
            return TR(t_site(PS("list"), s), rt("slice", {rs(Str("list"))}));
        }
        if (lit_eq(k, "tuple")) {
            std::optional<Big> a = int_const(sl->lower), b = int_const(sl->upper);
            if (!sl->step && (!sl->lower || a) && (!sl->upper || b)) {
                return TR(P::tuple_of(py_tuple_slice(ot.args, a, b)), rt("tuple_slice", {ropt(a), ropt(b)}));
            }
            return TR(issue(n, Str("conflict"), Str("computed slice of a tuple")));
        }
        if (lit_eq(k, "bottom") || lit_eq(k, "top")) {
            return TR(ot);
        }
        return TR(issue(n, Str("conflict"), Str("slice of a ") + k));
    }
    T it = ev(n->slice, state);
    if (lit_eq(k, "bytes") || lit_eq(k, "bytearray")) {
        need_int_index(n->slice, it);
        return TR(P::u(8), rt("index", {rs(k)}));
    }
    if (lit_eq(k, "str")) {
        need_int_index(n->slice, it);
        return TR(P::STR(), rt("index", {rs(k)}));
    }
    if (lit_eq(k, "list")) {
        need_int_index(n->slice, it);
        return TR(p.elem[static_cast<size_t>(p.find(*ot.site))], rt("index", {rs(k)}));
    }
    if (lit_eq(k, "tuple")) {
        std::optional<Big> i = int_const(n->slice);
        if (!i) {
            T e = P::BOTTOM();
            for (const T& a : ot.args) {
                e = p.join(e, a);
            }
            if (k_is(e, "top")) {
                return TR(issue(n, Str("conflict"), Str("computed index into a tuple of mixed items")));
            }
            need_int_index(n->slice, it);
            return TR(e, rt("tuple_index", {rn()}));
        }
        Big len(static_cast<int64_t>(ot.args.size()));
        if (!(-len <= *i && *i < len)) {
            return TR(issue(n, Str("conflict"), Str("tuple index ") + Str(i->str()) + Str(" out of range")));
        }
        int64_t ix = i->i64();
        return TR(at(ot.args, ix), rt("tuple_index", {ri(*i)}));
    }
    if (lit_eq(k, "dict")) {
        int64_t s = p.find(*ot.site);
        std::optional<Str> key = str_const(n->slice);
        if (key && !p.dynamic.contains(s)) {
            py::dict<Str, T>& fs = p.fields[static_cast<size_t>(s)];
            return TR(fs.contains(*key) ? fs.at(*key) : P::BOTTOM(), rt("field_get", {rs(*key)}));
        }
        if (!key) {
            p.make_dynamic(s, Str("a key that is not a constant string"));
        }
        p.grow(p.keys, s, it);
        return TR(p.elem[static_cast<size_t>(s)], rt("dict_get", {rn()}));
    }
    if (lit_eq(k, "bottom") || lit_eq(k, "top")) {
        return TR(ot);
    }
    if (lit_eq(k, "extern") && ot.name && lit_eq(*ot.name, "std:os.environ")) {
        return TR(issue(n, Str("stdlib"), Str("os.environ")));
    }
    if (lit_eq(k, "opt")) {
        return TR(issue(n, Str("conflict"), Str("subscript of a value that may be None")));
    }
    return TR(issue(n, Str("conflict"), Str("subscript of a ") + k));
}

// the most items a slice can hold: d[a:a+4] four, d[:8] eight, d[2:6] four
Bound Eval::slice_cap(const A::Slice* sl, const Bound& cap) {
    const A::Expr* lo = sl->lower;
    const A::Expr* hi = sl->upper;
    if (sl->step) {
        return cap;
    }
    std::optional<Big> hv = int_const(hi);
    std::optional<Big> lv = int_const(lo);
    if (hi != nullptr && hv && *hv >= Big(0)) {
        if (lo == nullptr) {
            return !cap ? Bound(*hv) : Bound(bmin(*cap, *hv));
        }
        if (lv && *lv >= Big(0)) {
            return bmax(*hv - *lv, Big(0));
        }
    }
    if (auto* hb = A::as<A::BinOp>(hi); hb && hb->op == A::OpKind::Add && lo != nullptr &&
                                         pyfront::dump(hb->left, false) == pyfront::dump(lo, false)) {
        std::optional<Big> rv = int_const(hb->right);
        if (rv && *rv >= Big(0)) {
            return *rv;
        }
    }
    return cap;
}

std::vector<T> Eval::ev_slice_parts(const A::Slice* sl, const State& state) {
    std::vector<T> out;
    for (const A::Expr* part : {sl->lower, sl->upper, sl->step}) {
        out.push_back(part != nullptr ? ev(part, state) : P::NONE());
    }
    return out;
}

void Eval::need_int_index(const A::Node* node, const T& t) {
    if (!k_in(t, {"int", "bottom", "top", "bool"})) {
        issue(node, Str("conflict"), Str("index of ") + t.kind);
    }
}

std::tuple<Str, int64_t> Eval::where(const A::Node* n) {
    return std::make_tuple(m != nullptr ? m->name : Str("?"), static_cast<int64_t>(n->pos.lineno));
}

// -- operators
T Eval::binop(const A::Node* n, A::OpKind op, T a, T b, const A::Expr* ln, const A::Expr* rn) {
    using O = A::OpKind;
    if (k_is(a, "bottom") || k_is(b, "bottom")) {
        return P::BOTTOM();
    }
    if (k_is(a, "top")) {
        return a;
    }
    if (k_is(b, "top")) {
        return b;
    }
    Str ka = a.kind, kb = b.kind;
    if (lit_eq(ka, "bool")) {
        ka = Str("int");
        a = P::INT(Big(0), Big(1));
    }
    if (lit_eq(kb, "bool")) {
        kb = Str("int");
        b = P::INT(Big(0), Big(1));
    }
    if (lit_eq(ka, "int") && lit_eq(kb, "int")) {
        switch (op) {
            case O::Add: return P::iadd(a, b);
            case O::Sub: return P::isub(a, b);
            case O::Mult: return P::imul(a, b);
            case O::FloorDiv: return P::ifloordiv(a, b);
            case O::Mod: return P::imod(a, b);
            case O::LShift: return P::ishl(a, b);
            case O::RShift: return P::ishr(a, b);
            case O::BitAnd: return P::iand(a, b);
            case O::BitOr: return P::ior(a, b);
            case O::BitXor: return P::ixor(a, b);
            case O::Pow:
                if (b.lo && *b.lo >= Big(0)) {
                    return P::ipow(a, b);
                }
                return issue(n, Str("conflict"), Str("int ** a possibly negative int (a float)"));
            case O::Div:
                exact_float(n, a);
                exact_float(n, b);
                return P::FLOAT();
            default: break;
        }
    }
    bool af = lit_eq(ka, "int") || lit_eq(ka, "float");
    bool bf = lit_eq(kb, "int") || lit_eq(kb, "float");
    if (af && bf) {
        if (lit_eq(ka, "int")) {
            exact_float(n, a);
        }
        if (lit_eq(kb, "int")) {
            exact_float(n, b);
        }
        if (op == O::Add || op == O::Sub || op == O::Mult || op == O::Div || op == O::FloorDiv || op == O::Mod || op == O::Pow) {
            return P::FLOAT();
        }
    }
    if (ka == kb && (lit_eq(ka, "str") || lit_eq(ka, "bytes") || lit_eq(ka, "bytearray")) && op == O::Add) {
        if (lit_eq(ka, "str")) {
            return P::STR();
        }
        return T(ka, {}, std::nullopt, (!a.hi || !b.hi) ? Bound() : Bound(*a.hi + *b.hi));
    }
    if (lit_eq(ka, "bytearray") && lit_eq(kb, "bytes") && op == O::Add) {
        return T(PS("bytearray"));
    }
    if (lit_eq(ka, "bytes") && lit_eq(kb, "bytearray") && op == O::Add) {
        return T(PS("bytes"));
    }
    if ((lit_eq(ka, "str") || lit_eq(ka, "bytes")) && lit_eq(kb, "int") && op == O::Mult) {
        return lit_eq(ka, "bytes") ? T(ka) : P::STR();
    }
    if (lit_eq(ka, "str") && op == O::Mod) {
        check_percent(n, ln, rn, b);
        return P::STR();
    }
    if (lit_eq(ka, "bytes") && op == O::Mod) {
        return issue(n, Str("syntax"), Str("bytes %-formatting"));
    }
    if (lit_eq(ka, "list") && lit_eq(kb, "list") && op == O::Add) {
        int64_t s = p.new_site(n, Str("list"), where(n));
        T ea = p.elem[static_cast<size_t>(p.find(*a.site))];
        p.grow(p.elem, s, ea);
        T eb = p.elem[static_cast<size_t>(p.find(*b.site))];
        p.grow(p.elem, s, eb);
        return t_site(PS("list"), s);
    }
    if (lit_eq(ka, "list") && lit_eq(kb, "int") && op == O::Mult) {
        int64_t s = p.new_site(n, Str("list"), where(n));
        T ea = p.elem[static_cast<size_t>(p.find(*a.site))];
        p.grow(p.elem, s, ea);
        return t_site(PS("list"), s);
    }
    if (lit_eq(ka, "tuple") && lit_eq(kb, "tuple") && op == O::Add) {
        std::vector<T> all = a.args;
        all.insert(all.end(), b.args.begin(), b.args.end());
        return P::tuple_of(all);
    }
    if (lit_eq(ka, "set") && lit_eq(kb, "set") && (op == O::BitOr || op == O::BitAnd || op == O::Sub || op == O::BitXor)) {
        int64_t s = p.new_site(n, Str("set"), where(n));
        T ea = p.elem[static_cast<size_t>(p.find(*a.site))];
        p.grow(p.elem, s, ea);
        T eb = p.elem[static_cast<size_t>(p.find(*b.site))];
        p.grow(p.elem, s, eb);
        return t_site(PS("set"), s);
    }
    return issue(n, Str("conflict"), ka + Str(" ") + Str(A::name_of(op)) + Str(" ") + kb);
}

void Eval::exact_float(const A::Node* n, const T& t) {
    if (k_is(t, "int")) {
        static const Big lim = Big::pow2(53);
        if (!(t.lo && t.hi && -lim <= *t.lo && *t.hi <= lim)) {
            issue(n, Str("int-range"), Str("an int beyond 2**53 meets a float"));
        }
    }
}

void Eval::check_percent(const A::Node* n, const A::Expr* ln, const A::Expr* rn, const T& b) {
    (void)rn;
    std::optional<Str> fmt = str_const(ln);
    if (!fmt) {
        issue(n, Str("dynamic"), Str("% with a computed format"));
        return;
    }
    static const py::re::Pattern pat(Str("%([-+ #0]*)(\\*|\\d+)?(?:\\.(\\*|\\d+))?([a-zA-Z%])"), 0);
    py::list<std::tuple<Str, Str, Str, Str>> specs = pat.findall_groups<Str, Str, Str, Str>(*fmt);
    for (const auto& s : specs) {
        const Str& width = std::get<1>(s);
        const Str& prec = std::get<2>(s);
        const Str& conv = std::get<3>(s);
        static const Str convs("dixXosrcfeEgG%");
        if (!convs.contains(conv) || width == Str("*") || prec == Str("*")) {
            issue(n, Str("syntax"), Str("percent conversion %") + conv);
        }
    }
    if (fmt->contains(Str("%("))) {
        issue(n, Str("syntax"), Str("percent with a mapping"));
    }
    int64_t nvals = 0;
    for (const auto& s : specs) {
        if (!(std::get<3>(s) == Str("%"))) {
            ++nvals;
        }
    }
    std::vector<T> items = k_is(b, "tuple") ? b.args : std::vector<T>{b};
    if (k_is(b, "tuple") && static_cast<int64_t>(items.size()) != nvals) {
        issue(n, Str("conflict"), Str("% with ") + i2s(static_cast<int64_t>(items.size())) + Str(" values for ") + i2s(nvals) + Str(" conversions"));
    }
    for (const T& t : items) {
        if (k_in(t, {"list", "dict", "set", "inst", "opt"}) && !(k_is(t, "opt") && k_in(t.args[0], {"int", "str"}))) {
            issue(n, Str("syntax"), Str("% of a ") + t.kind);
        }
    }
}

TR Eval::e_UnaryOp(const A::UnaryOp* n, const State& state) {
    T t = ev(n->operand, state);
    if (n->op == A::UnaryOpKind::Not) {
        return TR(P::BOOL());
    }
    if (k_in(t, {"bottom", "top"})) {
        return TR(t);
    }
    if (k_is(t, "bool")) {
        t = P::INT(Big(0), Big(1));
    }
    if (k_is(t, "int")) {
        if (n->op == A::UnaryOpKind::USub) {
            return TR(P::ineg(t));
        }
        if (n->op == A::UnaryOpKind::UAdd) {
            return TR(t);
        }
        if (n->op == A::UnaryOpKind::Invert) {
            return TR(P::iinvert(t));
        }
    }
    if (k_is(t, "float") && (n->op == A::UnaryOpKind::USub || n->op == A::UnaryOpKind::UAdd)) {
        return TR(P::FLOAT());
    }
    return TR(issue(n, Str("conflict"), Str(A::name_of(n->op)) + Str(" of ") + t.kind));
}

TR Eval::e_BoolOp(const A::BoolOp* n, const State& state) {
    std::vector<T> ts;
    SP s = state;
    for (const A::Expr* v : n->values) {
        T t = ev(v, *s);
        ts.push_back(t);
        s = refine(v, s, n->op == A::BoolOpKind::And);
    }
    // the value is one of the operands (Python): in a test only truth matters, so bool;
    // as a value, all operands must be of one type
    T j = P::BOTTOM();
    for (const T& t : ts) {
        j = p.join(j, t);
    }
    bool all_bool = true;
    for (const T& t : ts) {
        if (!k_in(t, {"bool", "bottom"})) {
            all_bool = false;
        }
    }
    if (all_bool) {
        return TR(P::BOOL());
    }
    if (n->op == A::BoolOpKind::Or && ts.size() == 2 && k_is(ts[0], "opt")) {
        T j2 = p.join(P::strip_opt(ts[0]), ts[1]);  // `x or default`: None never comes out
        if (!k_is(j2, "top")) {
            return TR(j2);
        }
    }
    if (k_is(j, "top")) {
        return TR(P::BOOL(), rt("mixedbool", {ropt_s(j.why)}));
    }
    return TR(j);
}

TR Eval::e_Compare(const A::Compare* n, const State& state) {
    T left = ev(n->left, state);
    for (size_t i = 0; i < n->ops.size(); ++i) {
        T right = ev(n->comparators[i], state);
        check_compare(n, n->ops[i], left, right);
        left = right;
    }
    return TR(P::BOOL());
}

void Eval::check_compare(const A::Compare* n, A::CmpOpKind op, const T& a, const T& b) {
    using C = A::CmpOpKind;
    if (k_in(a, {"bottom", "top"}) || k_in(b, {"bottom", "top"})) {
        return;
    }
    if (op == C::Is || op == C::IsNot) {
        if (!(k_is(b, "none") || k_is(a, "none"))) {
            issue(n, Str("dynamic"), Str("identity comparison of ") + a.kind);
        }
        return;
    }
    if (op == C::In || op == C::NotIn) {
        if (k_in(b, {"list", "set"})) {
            T e = p.elem[static_cast<size_t>(p.find(*b.site))];
            comparable(n, a, e);
        } else if (k_is(b, "dict")) {
            int64_t s = p.find(*b.site);
            if (!(str_const(n->left) && !p.dynamic.contains(s))) {
                if (!str_const(n->left)) {
                    p.make_dynamic(s, Str("membership of a computed key"));
                }
            }
        } else if (k_is(b, "tuple")) {
            for (const T& x : b.args) {
                comparable(n, a, x);
            }
        } else if (k_in(b, {"str", "bytes", "bytearray"})) {
            bool bb = k_in(a, {"bytes", "bytearray"}) && k_in(b, {"bytes", "bytearray"});
            if (!(a.kind == b.kind || (k_is(a, "int") && !k_is(b, "str")) || bb)) {
                issue(n, Str("conflict"), a.kind + Str(" in ") + b.kind);
            }
        } else {
            issue(n, Str("conflict"), Str("membership in a ") + b.kind);
        }
        return;
    }
    if (op == C::Eq || op == C::NotEq) {
        comparable(n, a, b, true);
        return;
    }
    comparable(n, a, b, false, true);
}

void Eval::comparable(const A::Node* n, const T& a, const T& b, bool eq, bool order) {
    Str ka = a.kind, kb = b.kind;
    if (lit_eq(ka, "bottom") || lit_eq(kb, "bottom") || lit_eq(ka, "top") || lit_eq(kb, "top")) {
        return;
    }
    if (lit_eq(ka, "bool")) {
        ka = Str("int");
    }
    if (lit_eq(kb, "bool")) {
        kb = Str("int");
    }
    if (ka == kb) {
        for (const char* k : {"int", "str", "float", "bytes", "bytearray", "none"}) {
            if (lit_eq(ka, k)) {
                return;
            }
        }
    }
    auto pair_is = [&](const char* x, const char* y) { return (lit_eq(ka, x) && lit_eq(kb, y)) || (lit_eq(ka, y) && lit_eq(kb, x)); };
    if (pair_is("bytes", "bytearray")) {
        return;
    }
    if (pair_is("int", "float")) {
        exact_float(n, lit_eq(ka, "int") ? a : b);
        return;
    }
    if (lit_eq(ka, "tuple") && lit_eq(kb, "tuple") && a.args.size() == b.args.size()) {
        for (size_t i = 0; i < a.args.size(); ++i) {
            comparable(n, a.args[i], b.args[i], eq, order);
        }
        return;
    }
    if (eq && (lit_eq(ka, "opt") || lit_eq(kb, "opt"))) {
        T ia = P::strip_opt(a), ib = P::strip_opt(b);
        if (k_is(ib, "none") || k_is(ia, "none")) {
            return;
        }
        comparable(n, ia, ib, eq, order);
        return;
    }
    if (eq && lit_eq(ka, "list") && lit_eq(kb, "list")) {
        T ea = p.elem[static_cast<size_t>(p.find(*a.site))];
        T eb = p.elem[static_cast<size_t>(p.find(*b.site))];
        comparable(n, ea, eb, eq);
        return;
    }
    if (eq && lit_eq(ka, "inst") && lit_eq(kb, "inst")) {
        issue(n, Str("dynamic"), Str("equality of instances"));
        return;
    }
    if (eq) {
        issue(n, Str("conflict"), Str("comparing ") + ka + Str(" with ") + kb + Str(" (always unequal in Python)"));
        return;
    }
    issue(n, Str("conflict"), Str("ordering ") + ka + Str(" against ") + kb);
}

TR Eval::e_IfExp(const A::IfExp* n, const State& state) {
    ev(n->test, state);
    T a = ev(n->body, *refine(n->test, state, true));
    T b = ev(n->orelse, *refine(n->test, state, false));
    T j = p.join(a, b);
    if (k_is(j, "top")) {
        return TR(issue(n, Str("conflict"), j.why ? *j.why : Str()));
    }
    return TR(j);
}

// -- displays
TR Eval::e_Tuple(const A::Tuple* n, const State& state) {
    std::vector<T> ts;
    for (const A::Expr* e : n->elts) {
        ts.push_back(ev(e, state));
    }
    for (const A::Expr* e : n->elts) {
        if (A::as<A::Starred>(e)) {
            return TR(issue(n, Str("syntax"), Str("starred in a tuple")));
        }
    }
    return TR(P::tuple_of(ts));
}

TR Eval::e_List(const A::List* n, const State& state) {
    int64_t s = p.new_site(n, Str("list"), where(n));
    for (const A::Expr* e : n->elts) {
        T t = ev(e, state);
        p.grow(p.elem, s, t);
    }
    return TR(t_site(PS("list"), s));
}

TR Eval::e_Set(const A::Set* n, const State& state) {
    int64_t s = p.new_site(n, Str("set"), where(n));
    for (const A::Expr* e : n->elts) {
        T t = ev(e, state);
        p.grow(p.elem, s, t);
    }
    return TR(t_site(PS("set"), s));
}

TR Eval::e_Dict(const A::Dict* n, const State& state) {
    int64_t s = p.new_site(n, Str("dict"), where(n));
    std::vector<Str> keys;
    for (size_t i = 0; i < n->keys.size(); ++i) {
        const A::Expr* k = n->keys[i];
        const A::Expr* v = n->values[i];
        T vt = ev(v, state);
        if (k == nullptr) {  // {**other}
            spread(s, vt, n);
            continue;
        }
        T kt = ev(k, state);
        std::optional<Str> key = str_const(k);
        if (key) {
            p.grow_field(s, *key, fit(v, vt, Str("field ") + *key));
            keys.push_back(*key);
        } else {
            p.make_dynamic(s, Str("a key that is not a constant string"));
            p.grow(p.keys, s, kt);
            p.grow(p.elem, s, vt);
        }
    }
    if (live()) {
        std::vector<Str> all = keys;
        if (p.spread_keys.contains(s)) {
            for (const Str& k : p.spread_keys.at(s)) {
                all.push_back(k);
            }
        }
        p.literal_keys[n] = all;
        if (builder != static_cast<const A::Node*>(n)) {
            p.created(s, program::StrSet(all.begin(), all.end()));
        }
    }
    return TR(t_site(PS("dict"), s));
}

void Eval::spread(int64_t s, const T& vt, const A::Node* n) {
    if (!k_is(vt, "dict")) {
        if (!k_in(vt, {"bottom", "top"})) {
            issue(n, Str("conflict"), Str("** of a ") + vt.kind);
        }
        return;
    }
    int64_t src = p.find(*vt.site);
    if (p.dynamic.contains(src)) {
        p.make_dynamic(s, Str("filled from a dict"));
        p.grow(p.keys, s, p.keys[static_cast<size_t>(src)]);
        p.grow(p.elem, s, p.elem[static_cast<size_t>(src)]);
        return;
    }
    for (const auto& kv : p.fields[static_cast<size_t>(src)].items()) {
        p.grow_field(s, std::get<0>(kv), std::get<1>(kv));
    }
    const std::optional<program::StrSet>& req = p.required[static_cast<size_t>(src)];
    p.spread_keys.set(s, req ? *req : program::StrSet());
    p.spread_from[s].insert(src);
}

State Eval::comp_scope(const std::vector<A::comprehension*>& gens, const State& state) {
    State st = state;
    for (const A::comprehension* g : gens) {
        if (g->is_async) {
            issue(g, Str("dynamic"), Str("async comprehension"));
        }
        T it = ev(g->iter, st);
        auto elem = iteration(g->iter, it, st);
        st = assign(g->target, elem.first, st, nullptr);
        for (const A::Expr* cond : g->ifs) {
            ev(cond, st);
            st = *refine(cond, st, true);
        }
    }
    return st;
}

// -- calls
TR Eval::e_Call(const A::Call* n, const State& state) {
    std::optional<T> recv;
    if (auto* fa = A::as<A::Attribute>(n->func)) {
        recv = ev(fa->value, state);
    }
    std::optional<T> ft;
    if (!recv) {
        ft = ev(n->func, state);
    }
    for (const A::Expr* a : n->args) {
        if (auto* st = A::as<A::Starred>(a)) {
            ev(st->value, state);
            return TR(issue(n, Str("dynamic"), Str("*args call")));
        }
    }
    for (const A::keyword* k : n->keywords) {
        if (!k->has_arg && !is_name(n->func, "dict")) {
            ev(k->value, state);
            return TR(issue(n, Str("dynamic"), Str("**kwargs call")));
        }
    }
    auto lazy = [](const A::Expr* e) { return e->kind == A::ExprKind::Lambda || e->kind == A::ExprKind::GeneratorExp; };
    Args args;
    for (const A::Expr* a : n->args) {
        args.push_back(!lazy(a) ? std::optional<T>(ev(a, state)) : std::nullopt);
    }
    Kw kw;
    for (const A::keyword* k : n->keywords) {
        std::optional<T> v = !lazy(k->value) ? std::optional<T>(ev(k->value, state)) : std::nullopt;
        kw.set(k->has_arg ? std::optional<Str>(Str(k->arg)) : std::nullopt, v);
    }
    if (recv) {
        return method_call(n, *recv, Str(A::cast<A::Attribute>(n->func)->attr), args, kw, state);
    }
    return call_value(n, *ft, args, kw, state);
}

TR Eval::call_value(const A::Call* n, const T& ft, const Args& args, const Kw& kw, const State& state) {
    const Str& k = ft.kind;
    if (lit_eq(k, "bottom") || lit_eq(k, "top")) {
        return TR(ft);
    }
    if (lit_eq(k, "fn")) {
        const Str& name = *ft.name;
        if (str_starts(name, "builtin:")) {
            return builtin(n, name.slice_from(8), args, kw, state);
        }
        if (str_starts(name, "bound:")) {
            return call_fn(n, *p.fns.at(name.slice_from(6)), args, kw, true);
        }
        if (!p.fns.contains(name)) {
            // name.split(':', 1)[-1]
            const std::u32string& u = name.u32();
            size_t colon = u.find(U':');
            Str tail = colon == std::u32string::npos ? name : Str(u.substr(colon + 1));
            return TR(issue(n, Str("dynamic"), Str("a method kept as a value (") + tail + Str(")")));
        }
        return call_fn(n, *p.fns.at(name), args, kw);
    }
    if (lit_eq(k, "cls")) {
        if (str_starts(*ft.name, "exc:")) {
            for (const std::optional<T>& a : args) {
                if (a && !k_in(*a, {"str", "bottom", "top", "int"})) {
                    issue(n, Str("syntax"), ft.name->slice_from(4) + Str(" with a ") + a->kind);
                }
            }
            if (args.size() > 1) {
                issue(n, Str("syntax"), Str("exception with several arguments"));
            }
            return TR(t_inst(*ft.name), rt("exc", {rs(ft.name->slice_from(4))}));
        }
        return construct(n, *ft.name, args, kw);
    }
    if (lit_eq(k, "extern")) {
        if (str_starts(*ft.name, "std:")) {
            return std_call(n, ft.name->slice_from(4), args, kw, state);
        }
        const std::u32string& u = ft.name->u32();
        size_t dot = u.find(U'.');
        return TR(issue(n, Str("extern"), dot == std::u32string::npos ? *ft.name : Str(u.substr(0, dot))));
    }
    return TR(issue(n, Str("conflict"), Str("calling a ") + k));
}

TR Eval::construct(const A::Call* n, const Str& cq, const Args& args, const Kw& kw) {
    Class* c = p.classes.at(cq);
    if (c->foreign_base) {
        return TR(issue(n, Str("extern"), Str("class derived from ") + *c->foreign_base));
    }
    Fn* init = p.find_method(cq, Str("__init__"));
    if (init != nullptr) {
        bind_args(n, *init, args, kw, true);
    } else if (c->exception && args.size() <= 1 && kw.size() == 0) {
        if (!args.empty() && args[0] && !k_in(*args[0], {"str", "bottom", "top"})) {
            issue(n, Str("syntax"), c->name + Str(" with a ") + args[0]->kind);
        }
    } else if (!args.empty() || kw.size() != 0) {
        issue(n, Str("conflict"), Str("arguments to a class with no __init__"));
    }
    return TR(t_inst(cq), rt("ctor", {rs(cq)}));
}

TR Eval::call_fn(const A::Call* n, Fn& f, const Args& args, const Kw& kw, bool bound) {
    if (lit_eq(f.kind, "property")) {
        return TR(issue(n, Str("conflict"), Str("calling a property")));
    }
    bind_args(n, f, args, kw, bound);
    std::vector<Fn*> targets = {&f};
    if (bound && f.cls != nullptr) {
        for (const Str& d : p.descendants(f.cls->qname)) {
            Class* dc = p.classes.at(d);
            if (dc->methods.contains(f.name)) {
                Fn* o = dc->methods.at(f.name);
                bind_args(n, *o, args, kw, true);
                targets.push_back(o);
            }
        }
    }
    T r = P::BOTTOM();
    for (Fn* t : targets) {
        T rr = p.ret(t->qname);
        r = p.join(r, rr);
    }
    if (k_is(r, "top")) {  // its result has no type: that is the callee's refusal
        r = P::top(Str("calls-refused:") + f.name);
    }
    return TR(r, rt(bound ? "method" : "fn", {rs(f.qname)}));
}

void Eval::bind_args(const A::Call* n, Fn& f, const Args& args, const Kw& kw, bool bound) {
    std::vector<Str> names = (bound || !(lit_eq(f.kind, "method") || lit_eq(f.kind, "property") || lit_eq(f.kind, "classmethod")))
                                 ? f.bound()
                                 : f.params;
    if ((lit_eq(f.kind, "method") || lit_eq(f.kind, "property")) && !bound) {
        names = f.params;
    }
    if (f.varargs || f.kwargs) {
        return;
    }
    if (args.size() > names.size()) {
        issue(n, Str("conflict"), Str("too many arguments to ") + f.name);
        return;
    }
    std::set<Str> given;
    for (size_t i = 0; i < args.size(); ++i) {
        given.insert(names[i]);
        if (args[i]) {
            contribute_param(f.qname, names[i], arg_fit(n, f, names[i], *args[i]));
        } else {
            issue(n, Str("dynamic"), Str("lambda or generator passed to ") + f.name);
        }
    }
    for (const std::optional<Str>& k : kw) {
        const std::optional<T>& t = kw.at(k);
        // `k not in names and k not in f.kwonly`: the key None (a ** spread) is in neither
        bool in_names = k && std::find(names.begin(), names.end(), *k) != names.end();
        bool in_kwonly = k && std::find(f.kwonly.begin(), f.kwonly.end(), *k) != f.kwonly.end();
        if (!in_names && !in_kwonly) {
            issue(n, Str("conflict"), Str("no parameter ") + (k ? *k : Str("None")) + Str(" of ") + f.name);
            continue;
        }
        given.insert(*k);
        if (t) {
            contribute_param(f.qname, *k, arg_fit(n, f, *k, *t));
        }
    }
    std::vector<Str> all = names;
    all.insert(all.end(), f.kwonly.begin(), f.kwonly.end());
    for (const Str& name : all) {
        if (!given.count(name) && !f.defaults.contains(name)) {
            issue(n, Str("conflict"), Str("missing argument ") + name + Str(" to ") + f.name);
        }
    }
}

TR Eval::method_call(const A::Call* n, const T& recv, const Str& attr, const Args& args, const Kw& kw, const State& state) {
    const Str& k = recv.kind;
    if (lit_eq(k, "bottom") || lit_eq(k, "top")) {
        return TR(recv);
    }
    if (lit_eq(k, "inst")) {
        if (str_starts(*recv.name, "exc:")) {
            return TR(issue(n, Str("dynamic"), Str("method of an exception")));
        }
        Fn* f = p.find_method(*recv.name, attr);
        if (f == nullptr) {
            std::optional<T> t = p.attr_type(*recv.name, attr);
            if (t) {
                return TR(issue(n, Str("dynamic"), Str("calling an attribute (") + attr + Str(")")));
            }
            return TR(issue(n, Str("unresolved"), last_part(*recv.name) + Str(".") + attr + Str("()")));
        }
        if (lit_eq(f->kind, "static")) {
            return call_fn(n, *f, args, kw);
        }
        return call_fn(n, *f, args, kw, true);
    }
    if (lit_eq(k, "module")) {
        if (str_starts(*recv.name, "std:")) {
            return std_call(n, recv.name->slice_from(4) + Str(".") + attr, args, kw, state);
        }
        Module& mod = *p.modules.at(*recv.name);
        std::optional<Ref> r = p.resolve_global(mod, attr);
        if (!r) {
            return TR(issue(n, Str("unresolved"), *recv.name + Str(".") + attr));
        }
        T ft = ref_type(A::cast<A::Attribute>(n->func), *r);
        return call_value(n, ft, args, kw, state);
    }
    if (lit_eq(k, "cls")) {
        if (str_starts(*recv.name, "exc:")) {
            return TR(issue(n, Str("dynamic"), Str("exception class method")));
        }
        Fn* f = p.find_method(*recv.name, attr);
        if (f != nullptr && lit_eq(f->kind, "static")) {
            return call_fn(n, *f, args, kw);
        }
        if (f != nullptr && lit_eq(f->kind, "method") && !args.empty() && args[0] && k_is(*args[0], "inst")) {
            std::vector<Str> m_ = p.mro(*args[0]->name);
            if (std::find(m_.begin(), m_.end(), *recv.name) != m_.end() && A::as<A::Name>(n->args[0]) && fn != nullptr &&
                !fn->params.empty() && Str(A::cast<A::Name>(n->args[0])->id) == fn->params[0]) {
                // Base.method(self, ...): the base's own method, not a virtual call
                if (lit_eq(attr, "__init__") && !(lit_eq(fn->name, "__init__") && first_stmt(n))) {
                    return TR(issue(n, Str("syntax"), Str("base __init__ called other than first in __init__")));
                }
                Args rest(args.begin() + 1, args.end());
                bind_args(n, *f, rest, kw, true);
                return TR(p.ret(f->qname), rt("basecall", {rs(f->qname)}));
            }
        }
        return TR(issue(n, Str("dynamic"), last_part(*recv.name) + Str(".") + attr + Str(" through the class")));
    }
    if (lit_eq(k, "extern")) {
        if (str_starts(*recv.name, "std:")) {
            return std_call(n, recv.name->slice_from(4) + Str(".") + attr, args, kw, state);
        }
        if (lit_eq(*recv.name, "file")) {
            return file_method(n, attr, args, kw);
        }
        if (lit_eq(*recv.name, "super")) {
            return super_call(n, attr, args, kw);
        }
        const std::u32string& u = recv.name->u32();
        size_t dot = u.find(U'.');
        return TR(issue(n, Str("extern"), dot == std::u32string::npos ? *recv.name : Str(u.substr(0, dot))));
    }
    if (lit_eq(k, "opt")) {
        return TR(issue(n, Str("conflict"), Str(".") + attr + Str("() of a value that may be None")));
    }
    return methods::call(*this, n, recv, attr, args, kw, state);
}

bool Eval::first_stmt(const A::Call* call) {
    std::vector<const A::Stmt*> body;
    for (const A::Stmt* s : fn->node->body) {
        auto* e = A::as<A::ExprStmt>(s);
        if (!(e && A::as<A::Constant>(e->value))) {
            body.push_back(s);
        }
    }
    if (body.empty()) {
        return false;
    }
    auto* e = A::as<A::ExprStmt>(body[0]);
    return e && e->value == call;
}

TR Eval::super_call(const A::Call* n, const Str& attr, const Args& args, const Kw& kw) {
    if (cls == nullptr || cls->bases.empty()) {
        return TR(issue(n, Str("dynamic"), Str("super() without a base of the program")));
    }
    const Str& base = cls->bases[0];
    Fn* f = p.find_method(base, attr);
    if (f == nullptr) {
        return TR(issue(n, Str("unresolved"), Str("super().") + attr));
    }
    bind_args(n, *f, args, kw, true);
    return TR(p.ret(f->qname), rt("super", {rs(f->qname)}));
}

TR Eval::file_method(const A::Call* n, const Str& attr, const Args& args, const Kw& kw) {
    (void)kw;
    if (lit_eq(attr, "read") && args.empty()) {
        return TR(P::BYTES(), rt("bmethod", {rs(Str("file.read"))}));
    }
    return TR(issue(n, Str("stdlib"), Str("file.") + attr));
}

TR Eval::builtin(const A::Call* n, const Str& name, const Args& args, const Kw& kw, const State& state) {
    return methods::builtin(*this, n, name, args, kw, state);
}

TR Eval::std_call(const A::Call* n, const Str& name, const Args& args, const Kw& kw, const State& state) {
    return methods::std_call(*this, n, name, args, kw, state);
}

T Eval::lambda_type(const A::Expr* lam, const std::vector<T>& params, const State& state) {
    auto* l = A::as<A::Lambda>(lam);
    if (!l) {
        return ev(lam, state);
    }
    const A::arguments* a = l->args;
    if (a->vararg || a->kwarg || !a->kwonlyargs.empty() || a->args.size() != params.size()) {
        return issue(lam, Str("dynamic"), Str("lambda signature"));
    }
    State st = state;
    for (size_t i = 0; i < params.size(); ++i) {
        st.set(Str(a->args[i]->arg), params[i]);
        note(a->args[i], params[i]);
    }
    T t = ev(l->body, st);
    note(lam, t);
    return t;
}

std::pair<T, State> Eval::gen_elem(const A::GeneratorExp* g, const State& state) {
    State st = comp_scope(g->generators, state);
    T e = ev(g->elt, st);
    return {e, st};
}

// ---------------------------------------------------------------------------------------- ModuleEval
ModuleEval::ModuleEval(Program& prog, Module* module, bool record_) : Eval(prog, nullptr, record_) { m = module; }

T ModuleEval::safe_ev(const A::Expr* v) {
    try {
        return ev(v, State());
    } catch (const py::Exception& e) {  // a front end bug: that name has no type
        return P::top(Str("frontend-error:") + Str(e.type()) + Str(": ") + Str(std::string(e.what())));
    } catch (const std::exception& e) {
        return P::top(Str("frontend-error:RuntimeError: ") + Str(std::string(e.what())));
    }
}

Eval& ModuleEval::run() {
    Module* mm = m;
    py::dict<Str, T> g = p.globals[mm->name];
    for (const Str& name : mm->assigns) {
        T t = P::BOTTOM();
        for (const A::Expr* v : mm->assigns.at(name)) {
            T vt = v != nullptr ? safe_ev(v) : P::top(Str("module-level unpacking"));
            t = p.join(t, vt);
        }
        T old = g.contains(name) ? g.at(name) : P::BOTTOM();
        T new_ = p.join(old, t);
        if (new_ != old) {
            g.set(name, new_);
            p.touch(fk('g', mm->name));
        }
    }
    for (Class* c : mm->classes.values()) {
        for (const Str& name : c->class_attrs) {
            T t = safe_ev(c->class_attrs.at(name));
            auto key = std::make_tuple(c->qname, name);
            T old = p.class_attr_types.contains(key) ? p.class_attr_types.at(key) : P::BOTTOM();
            T new_ = p.join(old, t);
            if (new_ != old) {
                p.class_attr_types.set(key, new_);
                p.touch(fk('C'));
            }
        }
    }
    std::vector<Fn*> fns = mm->functions.values();
    for (Class* c : mm->classes.values()) {
        for (Fn* f : c->methods.values()) {
            fns.push_back(f);
        }
    }
    for (Fn* f : fns) {
        for (const Str& name : f->defaults) {
            const A::Expr* d = f->defaults.at(name);
            T t = P::BOTTOM();
            if (A::as<A::List>(d) || A::as<A::Dict>(d) || A::as<A::Set>(d) || A::as<A::Call>(d)) {
                t = P::top(Str("mutable default"));
            } else {
                t = safe_ev(d);
            }
            auto key = std::make_tuple(f->qname, name);
            T old = p.default_types.contains(key) ? p.default_types.at(key) : P::BOTTOM();
            T new_ = p.join(old, t);
            if (new_ != old) {
                p.default_types.set(key, new_);
                p.touch(fk('d'));
            }
        }
    }
    return *this;
}

const py::dict<Str, T>& STD_ATTRS() {
    static const py::dict<Str, T> d = [] {
        py::dict<Str, T> out;
        out.set(Str("sys.maxsize"), P::const_int(P::I64_MAX()));
        out.set(Str("os.sep"), P::STR());
        out.set(Str("os.linesep"), P::STR());
        out.set(Str("os.pathsep"), P::STR());
        out.set(Str("math.pi"), P::FLOAT());
        out.set(Str("string.hexdigits"), P::STR());
        out.set(Str("string.digits"), P::STR());
        out.set(Str("string.ascii_letters"), P::STR());
        out.set(Str("string.printable"), P::STR());
        out.set(Str("struct.error"), t_named(Str("cls"), Str("exc:struct.error")));
        return out;
    }();
    return d;
}

}  // namespace codebreaker::translate::frontends::python::infer
