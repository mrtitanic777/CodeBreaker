// infer.cc -- the abstract interpreter (infer.py, rule for rule; the extensions are marked "(new)").
#include "infer.hpp"

#include <algorithm>

#include "pyfront/pyfront.hpp"

namespace pyengine {

using namespace pyfront;
static const int MAX_LOOP = 10;

static const std::set<std::string> BUILTIN_FUNCS = [] {
    std::set<std::string> s;
    for (const char* w : {"len", "range", "enumerate", "zip", "isinstance", "int", "str", "bytes", "bytearray", "bool", "float",
                          "min", "max", "abs", "sorted", "reversed", "sum", "any", "all", "list", "dict", "set", "tuple", "print",
                          "open", "ord", "chr", "hex", "oct", "bin", "repr", "divmod", "round", "pow", "format", "super", "iter",
                          "next", "map", "filter", "getattr", "setattr", "hasattr", "type", "id", "hash", "vars", "globals",
                          "locals", "callable", "issubclass", "object", "frozenset", "delattr", "exec", "eval", "compile",
                          "input", "memoryview", "staticmethod", "classmethod", "property"})
        s.insert(w);
    return s;
}();

std::string fmt_big(const std::optional<Big>& b) { return bstr(b); }

std::string py_repr(const std::string& s) {
    bool sq = s.find('\'') != std::string::npos, dq = s.find('"') != std::string::npos;
    char q = (sq && !dq) ? '"' : '\'';
    std::string out(1, q);
    for (unsigned char c : s) {
        if (c == (unsigned char)q || c == '\\') {
            out += '\\';
            out += (char)c;
        } else if (c == '\n') out += "\\n";
        else if (c == '\r') out += "\\r";
        else if (c == '\t') out += "\\t";
        else if (c < 0x20 || c == 0x7f) {
            char buf[8];
            snprintf(buf, sizeof buf, "\\x%02x", c);
            out += buf;
        } else out += (char)c;
    }
    return out + q;
}
std::string py_repr_list(const std::vector<std::string>& v) {
    std::string s = "[";
    for (size_t i = 0; i < v.size(); i++) s += (i ? ", " : "") + py_repr(v[i]);
    return s + "]";
}

static bool has_top(const T& t, int depth = 0) {
    if (t.kind == TK::Top) return true;
    if (depth >= 6) return false;
    for (auto& a : t.args())
        if (has_top(a, depth + 1)) return true;
    return false;
}

static std::string rsplit_last(const std::string& q) {
    auto p = q.rfind('.');
    return p == std::string::npos ? q : q.substr(p + 1);
}

// ------------------------------------------------------------------------------------ State
const T* State::get(const std::string& k) const {
    auto it = std::lower_bound(v.begin(), v.end(), k, [](const std::pair<std::string, T>& a, const std::string& b) { return a.first < b; });
    return (it != v.end() && it->first == k) ? &it->second : nullptr;
}
void State::set(const std::string& k, const T& t) {
    auto it = std::lower_bound(v.begin(), v.end(), k, [](const std::pair<std::string, T>& a, const std::string& b) { return a.first < b; });
    if (it != v.end() && it->first == k) it->second = t;
    else v.insert(it, {k, t});
}
void State::erase(const std::string& k) {
    auto it = std::lower_bound(v.begin(), v.end(), k, [](const std::pair<std::string, T>& a, const std::string& b) { return a.first < b; });
    if (it != v.end() && it->first == k) v.erase(it);
}

// ------------------------------------------------------------------------------------ Eval
Eval::Eval(Program& p_, Fn* fn_, bool record_) : p(p_), fn(fn_), m(fn_ ? fn_->module : nullptr), record(record_), cls(fn_ ? fn_->cls : nullptr) {}

T Eval::note(const void* node, const T& t, const Res* r) {
    auto it = seen.find(node);
    if (it == seen.end()) seen.emplace(node, t);
    else it->second = p.join(it->second, t);
    if (record && !quiet) {
        auto jt = types.find(node);
        if (jt == types.end()) types.emplace(node, t);
        else jt->second = p.join(jt->second, t);
        if (r && *r) res[node] = *r;
    }
    return t;
}

T Eval::issue(AnyNode node, const std::string& c, const std::string& detail) {
    if (record && !quiet) issues.push_back(Issue{c, detail, lineno(node)});
    return top(c + ":" + detail);
}

void Eval::contribute_param(const std::string& fq, const std::string& name, const T& t) {
    if (live() && t.kind == TK::Top && !p.param_poison.count({fq, name}))
        p.param_poison[{fq, name}] = {fn ? fn->qname : "?", t.why()};
    if (!live() || t.kind == TK::Bottom || t.kind == TK::Top || ((t.kind == TK::Tuple || t.kind == TK::Opt) && has_top(t))) return;
    Fn* f = p.any_fn(fq);
    auto& ps = p.params[fq];
    T old = ps.count(name) ? ps[name] : BOTTOM();
    T nw;
    if (f->annotations.has(name)) {
        T at = p.annotation_type(f, name);
        if (at.kind == TK::Int && t.kind == TK::Int) nw = old.kind != TK::Bottom ? p.join(old, t) : t;
        else nw = at;
    } else {
        nw = p.join(old, t);
    }
    if (p.round > 3 && nw.kind == TK::Int && old.kind == TK::Int) nw = widen_int(old, nw);
    if (nw != old && p.frozen && old.kind != TK::Bottom) {   // (new) the parameter's contract is fixed
        p.veto("passes " + t.repr() + " as " + name + " of " + f->name + ", which takes " + old.repr());
        return;
    }
    if (nw != old) {
        ps[name] = nw;
        p.touch("p:" + fq);
    }
}

T Eval::arg_fit(AnyNode node, Fn* f, const std::string& name, const T& t0) {
    T t = fit(node, t0, "argument " + name);
    if (t.kind == TK::Opt && !f->none_aware.count(name)) {
        issue(node, "conflict", "passes a value that may be None as " + name + " of " + f->name);
        return t.args()[0];
    }
    if (t.kind == TK::None && !f->none_aware.count(name)) {
        issue(node, "conflict", "passes None as " + name + " of " + f->name);
        return BOTTOM();
    }
    return t;
}

T Eval::fit(AnyNode node, const T& t, const std::string& what) {
    if (t.kind == TK::Int && !t.fits_i64()) {
        if (p.int_policy == "strict") issue(node, "int-range", what + " in [" + bstr(t.lo()) + ", " + bstr(t.hi()) + "]");
        return meet(t, I64_MIN, I64_MAX);
    }
    if (t.kind == TK::Tuple || t.kind == TK::Opt) {
        bool any = false;
        for (auto& a : t.args())
            if (a.kind == TK::Int && !a.fits_i64()) any = true;
        if (any) {
            std::vector<T> a;
            for (auto& x : t.args()) a.push_back(fit(node, x, what));
            return T(t.kind, a);
        }
    }
    return t;
}

void Eval::contribute_attr(const std::string& cq, const std::string& attr, const T& t) {
    bool bad = t.kind == TK::Top || ((t.kind == TK::Tuple || t.kind == TK::Opt) && has_top(t));
    if (live() && bad) {
        std::string owner = p.attr_owner(cq, attr);
        if (owner.empty()) owner = cq;
        if (p.frozen && !p.poisoned.count({owner, attr})) {
            auto fa = p.frozen_attrs.find(owner);
            if (fa != p.frozen_attrs.end() && fa->second.has(attr)) {
                p.veto("stores a value of no single type to " + attr + ", typed by other code");
                return;
            }
        }
        if (!p.poisoned.count({owner, attr})) {
            p.poisoned[{owner, attr}] = fn ? fn->qname : "?";
            p.touch("a:" + owner);
        }
        return;
    }
    if (!live() || t.kind == TK::Bottom || bad) return;
    std::string owner = p.attr_owner(cq, attr);
    if (owner.empty()) owner = cq;
    auto& a = p.attrs[owner];
    T old = a.has(attr) ? *a.get(attr) : BOTTOM();
    T nw = p.join(old, t);
    if (p.round > 3 && nw.kind == TK::Int && old.kind == TK::Int) nw = widen_int(old, nw);
    if (nw != old && p.frozen && old.kind != TK::Bottom) {   // (new) the attribute's type is fixed
        p.veto("stores " + t.repr() + " to " + attr + ", which holds " + old.repr());
        return;
    }
    if (nw != old) {
        p.attrs[owner].set(attr, nw);
        p.touch("a:" + owner);
    }
}

// ------------------------------------------------------------------------------------ the function
Eval& Eval::run() {
    Fn* f = fn;
    const FunctionDef* node = f->node;
    locals = p.fn_locals(f);
    State state;
    if (!f->varargs.empty() || !f->kwargs.empty()) issue(node, "dynamic", !f->varargs.empty() ? "*args" : "**kwargs");
    if (f->kind == "async") issue(node, "dynamic", "async");
    // (new) decorators that leave the function's own behaviour as written: native_first answers by
    // the C++ the gate already holds equal to this body; the caches only remember results
    static const std::set<std::string> TRANSPARENT = {"native_first", "lru_cache", "cache"};
    bool native = p.native_bound.count(f->qname) != 0;   // (answered by a native format: answers.cc; lowered as its call)
    for (auto& d : f->decorators)
        if (d != "staticmethod" && d != "property" && !(p.extended && TRANSPARENT.count(d)) && !(native && (d == "ask" || d == "answered")))
            issue(node, "dynamic", "decorator " + d);
    std::vector<std::string> all = f->params;
    all.insert(all.end(), f->kwonly.begin(), f->kwonly.end());
    for (size_t i = 0; i < all.size(); i++) {
        T t;
        if (i == 0 && (f->kind == "method" || f->kind == "property")) t = named(TK::Inst, f->cls->qname);
        else if (i == 0 && f->kind == "classmethod") t = named(TK::Cls, f->cls->qname);
        else t = p.param_type(f, all[i]);
        state.set(all[i], t);
    }
    if (auto ai = p.native_args_of.find(f->qname); ai != p.native_args_of.end()) {   // args_of inlined: its expressions here
        ev(ai->second.data, state);
        for (auto* e : ai->second.place) ev(e, state);
    }
    bool gen = p.extended && f->generator;
    for (const AnyNode& n : walk(AnyNode(node))) {
        const A::Expr* e = n.expr();
        if (e && (e->kind == ExprKind::Yield || e->kind == ExprKind::YieldFrom)) {
            if (!gen) issue(n, "dynamic", "generator");
            else if (e->kind == ExprKind::YieldFrom) issue(n, "syntax", "yield from");
            break;
        }
    }
    SP out = block(node->body, state);
    if (gen) {   // (new) calling it makes an iterator over what it yields (consumed where it is made)
        if (returns_value) issue(node, "syntax", "a generator that returns a value");
        returns_value = true;
        ret_t = iter_of(yield_t, MEM);
        out.reset();
    }
    if (out && returns_value) ret_t = p.join(ret_t, NONE());
    if (!returns_value) ret_t = NONE();
    if (ret_t.kind == TK::Int && !ret_t.fits_i64()) ret_t = fit(node, ret_t, "return value");
    if (live() && f->has_nested) {   // (new) what the nested functions see of this one's names
        for (auto& [k, v] : state.v)
            if (!bound_types.count(k)) bound_types.emplace(k, v);
        auto& cv = p.closure_vars[f->qname];
        for (auto& [k, v] : bound_types) {
            if (v.kind == TK::Bottom) continue;
            auto it = cv.find(k);
            T old = it == cv.end() ? BOTTOM() : it->second;
            T nw = p.join(old, v);
            if (!(nw == old)) {
                cv[k] = nw;
                p.touch("cv:" + f->qname);
            }
        }
    }
    if (live() && !p.native_bound.count(f->qname)) {   // (a native answer's type stays the answer's: answers.cc)
        const std::string& fq = f->qname;
        T old = p.returns.count(fq) ? p.returns[fq] : BOTTOM();
        T nw = p.join(old, ret_t);
        if (p.round > 3 && nw.kind == TK::Int && old.kind == TK::Int) nw = widen_int(old, nw);
        if (nw != old) {
            p.returns[fq] = nw;
            p.touch("r:" + fq);
        }
    }
    return *this;
}

// ------------------------------------------------------------------------------------ statements
SP Eval::block(const std::vector<A::Stmt*>& stmts, SP state) {
    for (auto* st : stmts) {
        if (!state) return std::nullopt;
        state = stmt(st, *state);
        if (!p.vetoes.empty()) {   // (new) a contract this statement breaks: its function is refused here
            issue(st, "conflict", p.vetoes.front());
            p.vetoes.clear();
        }
    }
    return state;
}

static bool std_exception(const A::Expr* n) {
    auto* a = as<Attribute>(n);
    if (!a) return false;
    auto* v = as<pyfront::Name>(a->value);
    if (!v) return false;
    static const std::set<std::pair<std::string, std::string>> ok = {
        {"struct", "error"}, {"subprocess", "TimeoutExpired"}, {"subprocess", "CalledProcessError"}, {"json", "JSONDecodeError"}, {"zlib", "error"}};
    return ok.count({v->id, a->attr}) != 0;
}

SP Eval::stmt(const A::Stmt* st, const State& state) {
    switch (st->kind) {
        case StmtKind::Pass: return state;
        case StmtKind::Expr: {
            auto* e = cast<ExprStmt>(st);
            if (is_const(e->value)) return state;
            ev(e->value, state);
            return after_access(e->value, state);
        }
        case StmtKind::Assign: return s_Assign(cast<pyfront::Assign>(st), state);
        case StmtKind::AnnAssign: {
            auto* a = cast<AnnAssign>(st);
            if (!a->value) return state;
            T t = ev(a->value, state);
            if (as<pyfront::Name>(a->target)) {
                T at = p.ann_to_type(m, a->annotation);
                if (at.kind != TK::Bottom && at.kind != TK::Top && at.kind != TK::Int) {
                    if ((t.kind == TK::List || t.kind == TK::Dict || t.kind == TK::Set) && at.kind == t.kind) p.join(at, t);
                    t = at;
                }
            }
            return assign(a->target, t, state, a->value);
        }
        case StmtKind::AugAssign: return s_AugAssign(cast<AugAssign>(st), state);
        case StmtKind::Return: {
            auto* r = cast<pyfront::Return>(st);
            if (r->value) {
                T t = ev(r->value, state);
                returns_value = true;
                if (live()) ret_t = p.join(ret_t, t);
            } else if (live()) {
                ret_t = p.join(ret_t, NONE());
            }
            return std::nullopt;
        }
        case StmtKind::Raise: {
            auto* r = cast<Raise>(st);
            if (r->exc) {
                T t = ev(r->exc, state);
                if (t.kind != TK::Inst && t.kind != TK::Cls && t.kind != TK::Bottom && t.kind != TK::Top && t.kind != TK::Extern)
                    issue(st, "conflict", std::string("raise of a ") + kind_name(t.kind));
            }
            if (r->cause) issue(st, "syntax", "raise from");
            return std::nullopt;
        }
        case StmtKind::Assert: {
            auto* a = cast<pyfront::Assert>(st);
            ev(a->test, state);
            if (a->msg) ev(a->msg, state);
            return refine(a->test, state, true);
        }
        case StmtKind::Global: issue(st, "dynamic", "global"); return state;
        case StmtKind::Nonlocal: issue(st, "dynamic", "nonlocal"); return state;
        case StmtKind::Delete: issue(st, "dynamic", "del"); return state;
        case StmtKind::FunctionDef:
        case StmtKind::AsyncFunctionDef: return s_FunctionDef(cast<FunctionDef>(st), state);
        case StmtKind::ClassDef: issue(st, "syntax", "nested class"); return state;
        case StmtKind::Import: return s_Import(cast<Import>(st), state);
        case StmtKind::ImportFrom: return s_ImportFrom(cast<ImportFrom>(st), state);
        case StmtKind::If: {
            auto* i = cast<pyfront::If>(st);
            ev(i->test, state);
            SP s2 = after_access(i->test, state);
            SP a = block(i->body, refine(i->test, s2, true));
            SP b = block(i->orelse, refine(i->test, s2, false));
            return join_states(a, b);
        }
        case StmtKind::With: return s_With(cast<With>(st), state);
        case StmtKind::Try: return s_Try(cast<pyfront::Try>(st), state);
        case StmtKind::Break:
            if (!loops.empty()) loops.back().first.push_back(state);
            return std::nullopt;
        case StmtKind::Continue:
            if (!loops.empty()) loops.back().second.push_back(state);
            return std::nullopt;
        case StmtKind::While: return s_While(cast<pyfront::While>(st), state);
        case StmtKind::For:
        case StmtKind::AsyncFor: return s_For(cast<pyfront::For>(st), state);
        default: {
            std::string n = type_name(st);
            std::transform(n.begin(), n.end(), n.begin(), ::tolower);
            issue(st, "syntax", n);
            return state;
        }
    }
}

SP Eval::s_FunctionDef(const A::FunctionDef* st, const State& state0) {
    auto it = p.nested_by_node.find(st);
    if (!p.extended || it == p.nested_by_node.end()) {
        issue(st, "syntax", "nested function");
        return state0;
    }
    // (new) a closure: its name holds it; its defaults are evaluated here, as Python does
    Fn* nf = it->second;
    for (auto& [name, d] : nf->defaults) {
        T t = (d->kind == ExprKind::List || d->kind == ExprKind::Dict || d->kind == ExprKind::Set || d->kind == ExprKind::Call)
                  ? top("mutable default") : ev(d, state0);
        if (!live()) continue;
        std::string key = nf->qname + "\x1f" + name;
        T old = p.default_types.count(key) ? p.default_types[key] : BOTTOM();
        T nw = p.join(old, t);
        if (!(nw == old)) {
            p.default_types[key] = nw;
            p.touch("d");
        }
    }
    State state = state0;
    T ct = named(TK::Closure, nf->qname);
    state.set(st->name, ct);
    if (fn && fn->has_nested && live()) {
        auto bt = bound_types.find(st->name);
        if (bt == bound_types.end()) bound_types.emplace(st->name, ct);
        else bt->second = p.join(bt->second, ct);
    }
    return state;
}

SP Eval::s_Assign(const A::Assign* st, const State& state) {
    bool builderish = st->targets.size() == 1 && as<pyfront::Name>(st->targets[0]) &&
                      (st->value->kind == ExprKind::Dict || st->value->kind == ExprKind::Call);
    if (builderish) builder = st->value;
    T t = ev(st->value, state);
    SP s = after_access(st->value, state);
    State cur = *s;
    for (auto* tgt : st->targets) cur = assign(tgt, t, cur, st->value);
    if (builderish) builder_keys(st, t);
    return cur;
}

SP Eval::s_AugAssign(const A::AugAssign* st, const State& state) {
    T cur = ev(st->target, state);
    T val = ev(st->value, state);
    T t = binop(st, st->op, cur, val, st->target, st->value).t;
    note(st, t);
    return assign(st->target, t, state, nullptr, true);
}

SP Eval::s_Import(const A::Import* st, const State& state0) {
    State state = state0;
    for (auto* a : st->names) {
        std::string top_ = a->name.substr(0, a->name.find('.'));
        std::string local = a->has_asname ? a->asname : top_;
        local_imports[local] = p.module_ref(a->has_asname ? a->name : top_);
        state.erase(local);
    }
    return state;
}

SP Eval::s_ImportFrom(const A::ImportFrom* st, const State& state0) {
    State state = state0;
    std::string base = p.from_base(m, st);
    for (auto* a : st->names) {
        std::string local = a->has_asname ? a->asname : a->name;
        std::string full = base.empty() ? a->name : base + "." + a->name;
        if (p.modules.has(full)) {
            local_imports[local] = Ref{"module", full, ""};
        } else if (p.modules.has(base)) {
            Ref r = p.resolve_global(*p.modules.get(base), a->name);
            local_imports[local] = r ? r : Ref{"missing", full, ""};
        } else {
            std::string top_ = full.substr(0, full.find('.'));
            local_imports[local] = STDLIB.count(top_) ? Ref{"stdobj", full, ""} : Ref{"extern", full, ""};
        }
        state.erase(local);
    }
    return state;
}

SP Eval::s_With(const A::With* st, const State& state0) {
    State state = state0;
    for (auto* item : st->items) {
        T t = ev(item->context_expr, state);
        bool ok = t.kind == TK::Extern && (t.name() == "file" || (p.extended && t.name().rfind("file:", 0) == 0));
        if (!ok) issue(st, "syntax", "with");
        if (item->optional_vars) state = assign(item->optional_vars, t, state, nullptr);
    }
    return block(st->body, state);
}

SP Eval::s_Try(const A::Try* st, const State& state) {
    if (!st->finalbody.empty()) issue(st, "syntax", "finally");
    if (!st->orelse.empty()) issue(st, "syntax", "try-else");
    SP body = block(st->body, state);
    SP entry = body ? join_states(state, body) : SP(state);
    std::vector<SP> outs = {body};
    for (auto* h : st->handlers) {
        State hs = *entry;
        if (h->type) {
            std::vector<const A::Expr*> names;
            if (auto* tu = as<pyfront::Tuple>(h->type)) names.assign(tu->elts.begin(), tu->elts.end());
            else names.push_back(h->type);
            for (auto* n : names) {
                auto* nm = as<pyfront::Name>(n);
                bool ok = nm && (EXCEPTIONS.count(nm->id) || !lookup_class(n).empty());
                if (!ok && !std_exception(n)) issue(AnyNode(h, AnyNode::Handler), "dynamic", "except " + unparse(n));
            }
            if (h->has_name) {
                issue(AnyNode(h, AnyNode::Handler), "syntax", "except as");
                hs.set(h->name, named(TK::Extern, "exception"));
            }
        }
        outs.push_back(block(h->body, hs));
    }
    if (!st->orelse.empty()) outs[0] = outs[0] ? block(st->orelse, outs[0]) : std::nullopt;
    SP r;
    for (auto& o : outs) r = join_states(r, o);
    return r;
}

SP Eval::s_While(const A::While* st, const State& state) {
    if (!st->orelse.empty()) issue(st, "syntax", "while-else");
    auto body = [this, st](const State& head) -> SP {
        ev(st->test, head);
        SP h2 = after_access(st->test, head);
        return block(st->body, refine(st->test, h2, true));
    };
    auto exit = [this, st](const State& head) -> SP { return refine(st->test, head, false); };
    auto bound = [this, st](const State& head, const State& init) { return while_bound(st, head, init); };
    return loop(st, state, body, exit, bound);
}

SP Eval::s_For(const A::For* st, const State& state0) {
    if (!st->orelse.empty()) issue(st, "syntax", "for-else");
    T it = ev(st->iter, state0);
    auto [elem, n] = iteration(st->iter, it, state0);
    SP state = after_access(st->iter, state0);
    T elem_t = elem;
    auto body = [this, st, elem_t](const State& head) -> SP {
        State h = assign(st->target, elem_t, head, nullptr);
        return block(st->body, h);
    };
    auto exit = [](const State& head) -> SP { return head; };
    std::optional<Big> nn = n;
    auto bound = [nn](const State&, const State&) { return nn; };
    return loop(st, *state, body, exit, bound);
}

// ------------------------------------------------------------------------------------ loops
SP Eval::loop(const A::Stmt* st, const State& init, const std::function<SP(const State&)>& body,
              const std::function<SP(const State&)>& exit_refine,
              const std::function<std::optional<Big>(const State&, const State&)>& bound) {
    std::string memo_key;
    if (quiet) {
        char buf[32];
        snprintf(buf, sizeof buf, "%p|", (const void*)st);
        memo_key = buf;
        for (auto& [k, v] : init.v) memo_key += k + "=" + key_of(p.canon(v)) + ";";
        auto it = loop_memo.find(memo_key);
        if (it != loop_memo.end()) return it->second;
    }
    State head = init;
    quiet++;
    p.suspend++;
    try {
        head = fixpoint(st, init, head, body, bound);
    } catch (...) {
        quiet--;
        p.suspend--;
        throw;
    }
    quiet--;
    p.suspend--;
    loops.emplace_back();
    body(head);
    auto bc = loops.back();
    loops.pop_back();
    SP out = exit_refine(head);
    for (auto& b : bc.first) out = join_states(out, b);
    if (!memo_key.empty() && out) loop_memo[memo_key] = *out;
    return out;
}

static State merge_head(const State& head, State nw) {
    for (auto& [k, v] : head.v)
        if (!nw.has(k)) nw.set(k, v);
    return nw;
}

State Eval::fixpoint(const A::Stmt* st, const State& init, State head, const std::function<SP(const State&)>& body,
                     const std::function<std::optional<Big>(const State&, const State&)>& bound) {
    bool widened = false;
    for (int i = 0; i < MAX_LOOP; i++) {
        loops.emplace_back();
        SP back = body(head);
        auto bc = loops.back();
        loops.pop_back();
        for (auto& c : bc.second) back = join_states(back, c);
        State nw = back ? *join_states(init, back) : init;
        nw = merge_head(head, nw);
        if (i >= 1) {
            State out = nw;
            for (auto& [k, v] : nw.v) {
                const T* o = head.get(k);
                if (o && o->kind == TK::Int && v.kind == TK::Int && *o != v) out.set(k, widen_int(*o, v));
            }
            nw = out;
            widened = true;
        }
        if (same_state(nw, head)) break;
        if (i == MAX_LOOP - 1) {
            State fixed = nw;
            for (auto& [k, v] : nw.v) {
                const T* h = head.get(k);
                if (h && p.canon(*h) != p.canon(v)) fixed.set(k, v.kind == TK::Int ? INT() : top("a value a loop keeps changing"));
            }
            nw = fixed;
        }
        head = nw;
    }
    if (widened) {
        loops.emplace_back();
        SP back = body(head);
        auto bc = loops.back();
        loops.pop_back();
        for (auto& c : bc.second) back = join_states(back, c);
        State nw = back ? *join_states(init, back) : init;
        nw = merge_head(head, nw);
        State h2;
        for (auto& [k, v] : nw.v) h2.set(k, narrow(head.get(k), v));
        head = counter_rule(st, h2, init, bound);
    }
    return head;
}

T Eval::narrow(const T* old, const T& nw) {
    if (!old || old->kind != TK::Int || nw.kind != TK::Int) return nw;
    OBig lo = !old->lo() ? nw.lo() : old->lo();
    OBig hi = !old->hi() ? nw.hi() : old->hi();
    if (nw.lo() && !old->lo()) lo = nw.lo();
    if (nw.hi() && !old->hi()) hi = nw.hi();
    return INT(lo, hi);
}

bool Eval::same_state(const State& a, const State& b) {
    if (a.v.size() != b.v.size()) return false;
    for (size_t i = 0; i < a.v.size(); i++) {
        if (a.v[i].first != b.v[i].first) return false;
        if (p.canon(a.v[i].second) != p.canon(b.v[i].second)) return false;
    }
    return true;
}

SP Eval::join_states(const SP& a, const SP& b) {
    if (!a) return b ? SP(*b) : std::nullopt;
    if (!b) return a;
    State out = *a;
    for (auto& [k, v] : b->v) {
        const T* cur = out.get(k);
        out.set(k, cur ? p.join(*cur, v) : v);
    }
    return out;
}

State Eval::counter_rule(const A::Stmt* st, State head, const State& init,
                         const std::function<std::optional<Big>(const State&, const State&)>& bound) {
    std::vector<std::string> loose;
    for (auto& [k, v] : head.v)
        if (v.kind == TK::Int && (!v.hi() || !v.lo())) loose.push_back(k);
    if (loose.empty()) return head;
    std::optional<Big> n = bound(head, init);
    if (!n) return head;
    const std::vector<A::Stmt*>* body = nullptr;
    if (st->kind == StmtKind::For || st->kind == StmtKind::AsyncFor) body = &cast<pyfront::For>(st)->body;
    else if (auto* w = as<pyfront::While>(st)) body = &w->body;
    if (!body) return head;
    for (auto& k : loose) {
        auto step = step_bound(*body, k);
        const T* v0 = init.get(k);
        if (!step || !v0 || v0->kind != TK::Int) continue;
        auto [up, down] = *step;
        OBig lo = down.sign() == 0 ? v0->lo() : (!v0->lo() ? OBig() : OBig(*v0->lo() - down * *n));
        OBig hi = up.sign() == 0 ? v0->hi() : (!v0->hi() ? OBig() : OBig(*v0->hi() + up * *n));
        T cur = *head.get(k);
        head.set(k, INT(cur.lo() ? cur.lo() : lo, cur.hi() ? cur.hi() : hi));
    }
    return head;
}

std::optional<std::pair<Big, Big>> Eval::step_bound(const std::vector<A::Stmt*>& stmts, const std::string& var) {
    Big up(0), down(0);
    for (auto* st : stmts) {
        auto r = step_one(st, var);
        if (!r) return std::nullopt;
        up = up + r->first;
        down = down + r->second;
    }
    return std::make_pair(up, down);
}

static const T* seen_of(const std::unordered_map<const void*, T>& seen, const void* n) {
    auto it = seen.find(n);
    return it == seen.end() ? nullptr : &it->second;
}

std::optional<std::pair<Big, Big>> Eval::step_one(const A::Stmt* st, const std::string& var) {
    if (auto* a = as<AugAssign>(st)) {
        auto* tn = as<pyfront::Name>(a->target);
        if (tn && tn->id == var) {
            const T* t = seen_of(seen, a->value);
            if (!t || t->kind != TK::Int || !t->lo() || !t->hi()) return std::nullopt;
            if (a->op == OpKind::Add) return std::make_pair(std::max(*t->hi(), Big(0)), std::max(-*t->lo(), Big(0)));
            if (a->op == OpKind::Sub) return std::make_pair(std::max(-*t->lo(), Big(0)), std::max(*t->hi(), Big(0)));
            return std::nullopt;
        }
    }
    if (st->kind == StmtKind::For || st->kind == StmtKind::AsyncFor || st->kind == StmtKind::While) {
        const std::vector<A::Stmt*>& body = st->kind == StmtKind::While ? cast<pyfront::While>(st)->body : cast<pyfront::For>(st)->body;
        auto inner = step_bound(body, var);
        if (!inner) return std::nullopt;
        if (inner->first.sign() == 0 && inner->second.sign() == 0) return std::make_pair(Big(0), Big(0));
        std::optional<Big> n;
        if (st->kind != StmtKind::While) n = range_count(cast<pyfront::For>(st)->iter);
        if (!n) return std::nullopt;
        return std::make_pair(inner->first * *n, inner->second * *n);
    }
    bool found_aug = false;
    for (const AnyNode& n : walk(st)) {
        auto* nm = as<pyfront::Name>(n.expr());
        if (nm && nm->id == var && nm->ctx == ExprCtx::Store) return std::nullopt;
        auto* a = as<AugAssign>(n.stmt());
        if (a) {
            auto* tn = as<pyfront::Name>(a->target);
            if (tn && tn->id == var) {
                found_aug = true;
                break;
            }
        }
    }
    if (!found_aug) return std::make_pair(Big(0), Big(0));
    if (auto* i = as<pyfront::If>(st)) {
        auto a = step_bound(i->body, var);
        auto b = step_bound(i->orelse, var);
        if (!a || !b) return std::nullopt;
        return std::make_pair(std::max(a->first, b->first), std::max(a->second, b->second));
    }
    if (st->kind == StmtKind::Try) {
        auto* t = cast<pyfront::Try>(st);
        if (t->finalbody.empty()) {
            std::vector<std::optional<std::pair<Big, Big>>> parts = {step_bound(t->body, var)};
            for (auto* h : t->handlers) parts.push_back(step_bound(h->body, var));
            Big u(0), d(0);
            for (auto& x : parts) {
                if (!x) return std::nullopt;
                u = u + x->first;
                d = d + x->second;
            }
            return std::make_pair(u, d);
        }
    }
    return std::nullopt;
}

std::optional<Big> Eval::range_count(const A::Expr* it) {
    const T* t = seen_of(seen, it);
    auto* c = as<pyfront::Call>(it);
    auto* fnm = c ? as<pyfront::Name>(c->func) : nullptr;
    if (fnm && fnm->id == "range") {
        std::vector<const T*> args;
        for (auto* a : c->args) args.push_back(seen_of(seen, a));
        for (auto* a : args)
            if (!a || a->kind != TK::Int) return std::nullopt;
        if (args.size() == 1) return !args[0]->hi() ? std::nullopt : std::optional<Big>(std::max(*args[0]->hi(), Big(0)));
        if (args.size() >= 2) {
            Big step(1);
            if (c->args.size() == 3) {
                if (!int_const(c->args[2], &step)) return std::nullopt;
            }
            if (step.sign() == 0) return std::nullopt;
            const T* a = args[0];
            const T* b = args[1];
            if (step.sign() > 0) {
                if (!b->hi() || !a->lo()) return std::nullopt;
                return std::max(floordiv(*b->hi() - *a->lo() + step - Big(1), step), Big(0));
            }
            if (!a->hi() || !b->lo()) return std::nullopt;
            return std::max(floordiv(*a->hi() - *b->lo() - step - Big(1), -step), Big(0));
        }
    }
    if (t) {
        TK k = t->kind;
        if (k == TK::List || k == TK::Tuple || k == TK::Str || k == TK::Bytes || k == TK::Bytearray || k == TK::Dict || k == TK::Set)
            return MEM;
    }
    if (c) {
        std::string name;
        if (auto* n = as<pyfront::Name>(c->func)) name = n->id;
        else if (auto* a = as<Attribute>(c->func)) name = a->attr;
        static const std::set<std::string> iters = {"enumerate", "zip", "reversed", "sorted", "items", "keys", "values"};
        if (iters.count(name)) return MEM;
    }
    return std::nullopt;
}

std::optional<Big> Eval::while_bound(const A::While* st, const State& head, const State& init) {
    if (auto* b = as<BoolOp>(st->test)) {
        if (b->op == BoolOpKind::And) {
            for (auto* v : b->values) {
                auto r = cmp_bound(v, st, head, init);
                if (r) return r;
            }
            return std::nullopt;
        }
    }
    return cmp_bound(st->test, st, head, init);
}

// least growth along paths: inf where there are none
struct PInt {
    bool inf = false;
    Big v;
};
static PInt pmin(const PInt& a, const PInt& b) {
    if (a.inf) return b;
    if (b.inf) return a;
    return a.v <= b.v ? a : b;
}
static PInt padd(const Big& a, const PInt& b) { return b.inf ? b : PInt{false, a + b.v}; }

struct PathsResult {
    bool bad = false;
    PInt acc, cont;
};

static bool grows_only(const A::Stmt* st, const std::string& var, const std::unordered_map<const void*, T>& seen) {
    std::set<const void*> aug;
    auto all = walk(st);
    for (auto& n : all)
        if (auto* a = as<AugAssign>(n.stmt())) aug.insert(a->target);
    for (auto& n : all) {
        auto* nm = as<pyfront::Name>(n.expr());
        if (nm && nm->id == var && nm->ctx == ExprCtx::Store && !aug.count(nm)) return false;
        if (auto* a = as<AugAssign>(n.stmt())) {
            auto* tn = as<pyfront::Name>(a->target);
            if (tn && tn->id == var) {
                const T* t = seen_of(seen, a->value);
                if (a->op != OpKind::Add || !t || t->kind != TK::Int || !t->lo() || t->lo()->sign() < 0) return false;
            }
        }
        if (n.stmt() && n.stmt()->kind == StmtKind::Continue && !(st->kind == StmtKind::For || st->kind == StmtKind::While))
            return false;
    }
    return true;
}

static PathsResult paths(const std::vector<A::Stmt*>& stmts, const std::string& var, const std::unordered_map<const void*, T>& seen) {
    PathsResult r;
    Big acc(0);
    PInt cont{true, Big(0)};
    for (auto* st : stmts) {
        if (st->kind == StmtKind::Break || st->kind == StmtKind::Return || st->kind == StmtKind::Raise) {
            r.acc = PInt{true, Big(0)};
            r.cont = cont;
            return r;
        }
        if (st->kind == StmtKind::Continue) {
            r.acc = PInt{true, Big(0)};
            r.cont = pmin(cont, PInt{false, acc});
            return r;
        }
        auto* a = as<AugAssign>(st);
        auto* tn = a ? as<pyfront::Name>(a->target) : nullptr;
        if (tn && tn->id == var) {
            const T* t = seen_of(seen, a->value);
            if (a->op != OpKind::Add || !t || t->kind != TK::Int || !t->lo() || t->lo()->sign() < 0) {
                r.bad = true;
                return r;
            }
            acc = acc + *t->lo();
        } else if (auto* i = as<pyfront::If>(st)) {
            PathsResult x = paths(i->body, var, seen), y = paths(i->orelse, var, seen);
            if (x.bad || y.bad) {
                r.bad = true;
                return r;
            }
            cont = pmin(cont, pmin(padd(acc, x.cont), padd(acc, y.cont)));
            PInt fall = pmin(x.acc, y.acc);
            if (fall.inf) {
                r.acc = fall;
                r.cont = cont;
                return r;
            }
            acc = acc + fall.v;
        } else if (!grows_only(st, var, seen)) {
            r.bad = true;
            return r;
        }
    }
    r.acc = PInt{false, acc};
    r.cont = cont;
    return r;
}

std::optional<Big> Eval::cmp_bound(const A::Expr* t, const A::While* st, const State& head, const State& init) {
    auto* c = as<Compare>(t);
    if (!c || c->ops.size() != 1) return std::nullopt;
    auto* ln = as<pyfront::Name>(c->left);
    if (!ln) return std::nullopt;
    const std::string& var = ln->id;
    CmpOpKind op = c->ops[0];
    if (op != CmpOpKind::Lt && op != CmpOpKind::LtE) return std::nullopt;
    PathsResult pr = paths(st->body, var, seen);
    PInt d = pr.bad ? PInt{false, Big(0)} : pmin(pr.acc, pr.cont);
    if (!d.inf && d.v.sign() == 0) return std::nullopt;
    if (d.inf) return Big(1);
    const T* x = seen_of(seen, c->comparators[0]);
    const T* v0 = init.get(var);
    if (!x || x->kind != TK::Int || !x->hi() || !v0 || v0->kind != TK::Int || !v0->lo()) return std::nullopt;
    return std::max(floordiv(*x->hi() - *v0->lo(), d.v) + Big(2), Big(0));
}

std::pair<T, std::optional<Big>> Eval::iteration(const A::Expr* node, const T& t, const State& state) {
    switch (t.kind) {
        case TK::Bottom: return {BOTTOM(), std::nullopt};
        case TK::Top: return {t, std::nullopt};
        case TK::List:
        case TK::Set:
            if (t.kind == TK::Set) issue(node, "dynamic", "set iteration order");
            return {p.elem[p.find(t.site)], MEM};
        case TK::Dict: {
            int s = p.find(t.site);
            p.make_dynamic(s, "iterated");
            return {p.keys[s], MEM};
        }
        case TK::Tuple: {
            T e = BOTTOM();
            for (auto& a : t.args()) e = p.join(e, a);
            if (e.kind == TK::Top) return {issue(node, "conflict", "iterating a tuple of mixed items"), std::nullopt};
            return {e, Big((int64_t)t.args().size())};
        }
        case TK::Str: return {STR(), MEM};
        case TK::Bytes:
        case TK::Bytearray: return {u(8), t.hi() ? *t.hi() : MEM};
        case TK::Iter: return {t.args()[0], t.lo()};
        case TK::Extern:
            if (p.extended && t.name().rfind("file:", 0) == 0 && t.name().find('r') != std::string::npos)
                return {t.name().find('b') != std::string::npos ? BYTES() : STR(), MEM};   // (new) a file's lines
            break;
        default: break;
    }
    return {issue(node, "conflict", std::string("iterating a ") + kind_name(t.kind)), std::nullopt};
}

// ------------------------------------------------------------------------------------ assignment
State Eval::assign(const A::Expr* tgt, const T& t0, State state, const A::Expr* value_node, bool aug) {
    T t = t0;
    if (auto* nm = as<pyfront::Name>(tgt)) {
        state.set(nm->id, t);
        if (fn && fn->has_nested && live()) {
            auto bt = bound_types.find(nm->id);
            if (bt == bound_types.end()) bound_types.emplace(nm->id, t);
            else bt->second = p.join(bt->second, t);
        }
        Res r("local");
        note(tgt, t, &r);
        if (t.kind == TK::List || t.kind == TK::Dict || t.kind == TK::Set) p.hint(t.site, nm->id);
        return state;
    }
    if (auto* at = as<Attribute>(tgt)) {
        T ot = ev(at->value, state);
        if (ot.kind == TK::Inst) {
            t = fit(tgt, t, "attribute " + at->attr);
            contribute_attr(ot.name(), at->attr, t);
            Res r("field", ot.name(), at->attr);
            note(tgt, t, &r);
            if (t.kind == TK::List || t.kind == TK::Dict || t.kind == TK::Set) p.hint(t.site, at->attr);
        } else if (ot.kind == TK::Bottom) {
        } else if (ot.kind == TK::Top) {
            note(tgt, ot);
        } else {
            note(tgt, issue(tgt, "dynamic", std::string("attribute store on a ") + kind_name(ot.kind)));
        }
        return state;
    }
    if (auto* sub = as<Subscript>(tgt)) {
        T ot = ev(sub->value, state);
        if (auto* sl = as<pyfront::Slice>(sub->slice)) {
            ev_slice_parts(sl, state);
            if (ot.kind == TK::List && t.kind == TK::List) p.grow(p.elem, ot.site, p.elem[p.find(t.site)]);
            else if (ot.kind == TK::Bytearray && (t.kind == TK::Bytes || t.kind == TK::Bytearray)) {
            } else if (ot.kind != TK::Bottom)
                issue(tgt, "syntax", std::string("slice assignment to a ") + kind_name(ot.kind));
            Res r("slice_store", std::string(kind_name(ot.kind)));
            note(tgt, t, &r);
            return state;
        }
        T kt = ev(sub->slice, state);
        if (ot.kind == TK::List) {
            p.grow(p.elem, ot.site, t);
            need_int_index(sub->slice, kt);
            Res r("index_store", std::string("list"));
            note(tgt, t, &r);
        } else if (ot.kind == TK::Bytearray) {
            if (t.kind != TK::Int && t.kind != TK::Bottom) issue(tgt, "conflict", std::string("bytearray item of ") + kind_name(t.kind));
            Res r("index_store", std::string("bytearray"));
            note(tgt, u(8), &r);
        } else if (ot.kind == TK::Dict) {
            std::string key;
            bool has_key = is_str_const(sub->slice, &key);
            int s = p.find(ot.site);
            t = fit(tgt, t, "field " + (has_key ? key : std::string("None")));
            if (has_key) {
                p.grow_field(s, key, t);
                if ((aug || !p.builder_targets.count(tgt)) && live()) p.mark_mutated(s);
            } else {
                p.make_dynamic(s, "a key that is not a constant string");
                p.grow(p.keys, s, kt);
                p.grow(p.elem, s, t);
            }
            Res r("dict_store", has_key ? std::optional<std::string>(key) : std::nullopt);
            note(tgt, t, &r);
        } else if (ot.kind == TK::Bottom || ot.kind == TK::Top) {
        } else {
            issue(tgt, "conflict", std::string("item store on a ") + kind_name(ot.kind));
        }
        return state;
    }
    const std::vector<pyfront::Expr*>* elts = nullptr;
    if (auto* tu = as<pyfront::Tuple>(tgt)) elts = &tu->elts;
    else if (auto* l = as<pyfront::List>(tgt)) elts = &l->elts;
    if (elts) {
        for (auto* e : *elts)
            if (e->kind == ExprKind::Starred) {
                issue(tgt, "syntax", "starred unpacking");
                return state;
            }
        if (t.kind == TK::Tuple) {
            if (t.args().size() != elts->size()) {
                issue(tgt, "conflict", "unpacking " + std::to_string(t.args().size()) + " items into " + std::to_string(elts->size()));
                return state;
            }
            for (size_t i = 0; i < elts->size(); i++) state = assign((*elts)[i], t.args()[i], state, nullptr);
            Res r("unpack");
            note(tgt, t, &r);
            return state;
        }
        if (t.kind == TK::Bottom) {
            for (auto* e : *elts) state = assign(e, BOTTOM(), state, nullptr);
            return state;
        }
        if (t.kind == TK::Top) {
            for (auto* e : *elts) state = assign(e, t, state, nullptr);
            return state;
        }
        issue(tgt, "conflict", std::string("unpacking a ") + kind_name(t.kind) + " (its length is not known to the type)");
        for (auto* e : *elts) state = assign(e, top("unpack"), state, nullptr);
        return state;
    }
    issue(tgt, "syntax", "assignment to " + type_name(tgt));
    return state;
}

static bool reads_fields_only(const A::Expr* value, const std::string& name) {
    std::set<const void*> ok;
    auto all = walk(value);
    for (auto& n : all) {
        auto* sub = as<Subscript>(n.expr());
        if (sub && sub->ctx == ExprCtx::Load)
            if (auto* v = as<pyfront::Name>(sub->value))
                if (v->id == name && is_str_const(sub->slice)) ok.insert(sub->value);
    }
    for (auto& n : all) {
        auto* nm = as<pyfront::Name>(n.expr());
        if (nm && nm->id == name && !ok.count(nm)) return false;
    }
    return true;
}

void Eval::builder_keys(const A::Assign* st, const T& t) {
    if (t.kind != TK::Dict) return;
    const std::string& name = cast<pyfront::Name>(st->targets[0])->id;
    const std::vector<A::Stmt*>* blk = p.block_of(st);
    if (!blk) return;
    std::vector<std::string> keys;
    auto lk = p.literal_keys.find(st->value);
    if (lk != p.literal_keys.end()) keys = lk->second;
    size_t i = std::find(blk->begin(), blk->end(), st) - blk->begin() + 1;
    while (i < blk->size()) {
        const A::Stmt* s2 = (*blk)[i];
        auto* a2 = as<pyfront::Assign>(s2);
        bool ok = false;
        if (a2 && a2->targets.size() == 1) {
            auto* sub = as<Subscript>(a2->targets[0]);
            if (sub) {
                auto* v = as<pyfront::Name>(sub->value);
                std::string key;
                if (v && v->id == name && is_str_const(sub->slice, &key) && reads_fields_only(a2->value, name)) {
                    keys.push_back(key);
                    p.builder_targets.insert(sub);
                    ok = true;
                }
            }
        }
        if (!ok) {
            bool uses = false;
            for (auto& n : walk(s2)) {
                auto* nm = as<pyfront::Name>(n.expr());
                if (nm && nm->id == name) {
                    uses = true;
                    break;
                }
            }
            if (uses) break;
        }
        i++;
    }
    if (live()) p.created(t.site, keys);
}

// ------------------------------------------------------------------------------------ refinement
SP Eval::refine(const A::Expr* test, const SP& state, bool truth) {
    if (!state) return std::nullopt;
    if (auto* u = as<UnaryOp>(test)) {
        if (u->op == UnaryOpKind::Not) return refine(u->operand, state, !truth);
    }
    if (auto* b = as<BoolOp>(test)) {
        if ((b->op == BoolOpKind::And) == truth) {
            SP s = state;
            for (auto* v : b->values) s = refine(v, s, truth);
            return s;
        }
        SP outs;
        SP s = state;
        for (auto* v : b->values) {
            outs = join_states(outs, refine(v, s, truth));
            s = refine(v, s, !truth);
        }
        return outs;
    }
    if (auto* nm = as<pyfront::Name>(test)) {
        const T* t = state->get(nm->id);
        if (t) {
            State s = *state;
            if (t->kind == TK::Opt && truth) s.set(nm->id, t->args()[0]);
            else if (t->kind == TK::Opt && !truth && t->args()[0].kind == TK::Inst) s.set(nm->id, NONE());
            else if (t->kind == TK::Int && !truth) s.set(nm->id, meet(*t, Big(0), Big(0)));
            return s;
        }
    }
    if (auto* c = as<pyfront::Call>(test)) {
        auto* f = as<pyfront::Name>(c->func);
        if (f && f->id == "isinstance" && c->args.size() == 2 && as<pyfront::Name>(c->args[0])) {
            const std::string& nm = cast<pyfront::Name>(c->args[0])->id;
            const T* t = state->get(nm);
            if (p.extended && t && t->kind == TK::Variant) {   // (new) the alternatives that pass the test
                std::set<TK> want;
                std::vector<const A::Expr*> specs;
                if (auto* tu = as<pyfront::Tuple>(c->args[1])) specs.assign(tu->elts.begin(), tu->elts.end());
                else specs.push_back(c->args[1]);
                for (auto* sp : specs)
                    if (auto* sn = as<pyfront::Name>(sp)) {
                        if (sn->id == "int") want.insert({TK::Int, TK::Bool});
                        else if (sn->id == "str") want.insert(TK::Str);
                        else if (sn->id == "float") want.insert(TK::Float);
                        else if (sn->id == "bool") want.insert(TK::Bool);
                        else if (sn->id == "bytes") want.insert(TK::Bytes);
                    }
                T keep = BOTTOM();
                for (auto& alt : t->args())
                    if (want.count(alt.kind) == (truth ? 1u : 0u)) keep = keep.kind == TK::Bottom ? alt : p.variant_join(keep, alt);
                State s = *state;
                if (keep.kind != TK::Bottom) s.set(nm, keep);
                return s;
            }
            return state;
        }
    }
    if (auto* c = as<Compare>(test)) {
        if (!c->ops.empty()) {
            const A::Expr* left = c->left;
            State s = *state;
            for (size_t i = 0; i < c->ops.size(); i++) {
                s = refine_cmp(left, c->ops[i], c->comparators[i], s, truth);
                if (!truth && c->ops.size() > 1) return s;
                left = c->comparators[i];
            }
            return s;
        }
    }
    return state;
}

static T bound_by(const T& v, const std::string& o, const T& other) {
    if (o == "<") return meet(v, {}, other.hi() ? OBig(*other.hi() - Big(1)) : OBig());
    if (o == "<=") return meet(v, {}, other.hi());
    if (o == ">") return meet(v, other.lo() ? OBig(*other.lo() + Big(1)) : OBig(), {});
    if (o == ">=") return meet(v, other.lo(), {});
    if (o == "==") return meet(v, other.lo(), other.hi());
    return v;
}

State Eval::refine_cmp(const A::Expr* left, CmpOpKind op, const A::Expr* right, State state, bool truth) {
    auto* ln = as<pyfront::Name>(left);
    if ((op == CmpOpKind::Is || op == CmpOpKind::IsNot) && is_none_const(right) && ln && state.has(ln->id)) {
        bool is_none = (op == CmpOpKind::Is) == truth;
        T t = *state.get(ln->id);
        state.set(ln->id, is_none ? NONE() : strip_opt(t));
        return state;
    }
    if ((op == CmpOpKind::Eq || op == CmpOpKind::NotEq) && is_none_const(right) && ln && state.has(ln->id))
        return refine_cmp(left, op == CmpOpKind::Eq ? CmpOpKind::Is : CmpOpKind::IsNot, right, state, truth);
    std::string o;
    switch (op) {
        case CmpOpKind::Lt: o = "<"; break;
        case CmpOpKind::LtE: o = "<="; break;
        case CmpOpKind::Gt: o = ">"; break;
        case CmpOpKind::GtE: o = ">="; break;
        case CmpOpKind::Eq: o = "=="; break;
        case CmpOpKind::NotEq: o = "!="; break;
        default: return state;
    }
    if (!truth) {
        static const std::map<std::string, std::string> inv = {{"<", ">="}, {"<=", ">"}, {">", "<="}, {">=", "<"}, {"==", "!="}, {"!=", "=="}};
        o = inv.at(o);
    }
    const T* lt = seen_of(seen, left);
    const T* rt = seen_of(seen, right);
    if (ln && state.has(ln->id) && state.get(ln->id)->kind == TK::Int && rt && rt->kind == TK::Int) {
        state.set(ln->id, bound_by(*state.get(ln->id), o, *rt));
    }
    auto* rn = as<pyfront::Name>(right);
    if (rn && state.has(rn->id) && state.get(rn->id)->kind == TK::Int && lt && lt->kind == TK::Int) {
        static const std::map<std::string, std::string> flip = {{"<", ">"}, {"<=", ">="}, {">", "<"}, {">=", "<="}, {"==", "=="}, {"!=", "!="}};
        state.set(rn->id, bound_by(*state.get(rn->id), flip.at(o), *lt));
    }
    return state;
}

SP Eval::after_access(const A::Expr* node, const SP& state0) {
    if (!state0) return std::nullopt;
    State state = *state0;
    for (const AnyNode& n : p.access_nodes(node)) {
        if (auto* sub = as<Subscript>(n.expr())) {
            auto* sn = as<pyfront::Name>(sub->slice);
            if (sn && state.has(sn->id)) {
                const T* st = seen_of(seen, sub->value);
                if (st && (st->kind == TK::Bytes || st->kind == TK::Bytearray || st->kind == TK::List || st->kind == TK::Str ||
                           st->kind == TK::Tuple)) {
                    T v = *state.get(sn->id);
                    if (v.kind == TK::Int) {
                        Big topb = (st->kind != TK::Bytes && st->kind != TK::Bytearray) || !st->hi() ? MEM : *st->hi();
                        state.set(sn->id, meet(v, -topb, topb - Big(1)));
                    }
                }
            }
        } else if (auto* c = as<pyfront::Call>(n.expr())) {
            std::string fq = res_fn(c);
            if (!fq.empty()) {
                Fn* f = *p.fns.get(fq);
                auto names = f->bound();
                for (size_t i = 0; i < c->args.size() && i < names.size(); i++) {
                    auto* an = as<pyfront::Name>(c->args[i]);
                    if (f->index_params.count(names[i]) && an && state.has(an->id) && state.get(an->id)->kind == TK::Int)
                        state.set(an->id, meet(*state.get(an->id), -MEM, MEM));
                }
            } else if (auto* at = as<Attribute>(c->func)) {
                if ((at->attr == "unpack_from" || at->attr == "pack_into") && c->args.size() >= 3) {
                    auto* an = as<pyfront::Name>(c->args[2]);
                    if (an && state.has(an->id) && state.get(an->id)->kind == TK::Int)
                        state.set(an->id, meet(*state.get(an->id), -MEM, MEM));
                }
            }
        }
    }
    return state;
}

std::string Eval::res_fn(const A::Expr* call) {
    const Res* r = nullptr;
    if (record) {
        auto it = res.find(call);
        if (it != res.end()) r = &it->second;
    }
    if (!r) {
        auto it = res_quiet.find(call);
        if (it != res_quiet.end()) r = &it->second;
    }
    if (r && (r->k == "fn" || r->k == "method") && p.fns.has(r->A())) return r->A();
    return "";
}

// ------------------------------------------------------------------------------------ expressions
T Eval::ev(const A::Expr* node, const State& state) {
    TR r = ev_inner(node, state);
    if (r.r && node->kind == ExprKind::Call && (r.r.k == "fn" || r.r.k == "method")) res_quiet[node] = r.r;
    return note(node, r.t, r.r ? &r.r : nullptr);
}

static const std::map<std::string, T>& std_attrs() {
    static const std::map<std::string, T> m = {
        {"sys.maxsize", const_int(I64_MAX)}, {"os.sep", STR()}, {"os.linesep", STR()}, {"os.pathsep", STR()}, {"math.pi", FLOAT()},
        {"string.hexdigits", STR()}, {"string.digits", STR()}, {"string.ascii_letters", STR()}, {"string.printable", STR()},
        {"struct.error", named(TK::Cls, "exc:struct.error")}};
    return m;
}

Ref Eval::lookup(const std::string& name) {
    auto it = local_imports.find(name);
    if (it != local_imports.end()) return it->second;
    if (m) {
        Ref r = p.resolve_global(m, name);
        if (r) return r;
    }
    if (BUILTIN_FUNCS.count(name)) return Ref{"builtin", name, ""};
    if (EXCEPTIONS.count(name)) return Ref{"exc", name, ""};
    if (name == "True" || name == "False" || name == "None" || name == "__name__" || name == "__file__" || name == "__doc__")
        return Ref{"const", name, ""};
    return Ref{};
}

std::string Eval::lookup_class(const A::Expr* n) {
    if (auto* nm = as<pyfront::Name>(n)) {
        Ref r = lookup(nm->id);
        if (r.k == "class") return r.a;
    }
    return "";
}

Res Eval::ref_res(const Ref& r) {
    if (r.k == "global" || r.k == "obj") return Res(r.k, r.a, r.b);
    return Res(r.k, r.a);
}

T Eval::ref_type(AnyNode n, const Ref& r) {
    const std::string& k = r.k;
    if (k == "global") return p.global_type(r.a, r.b);
    if (k == "fn") return named(TK::Fn, r.a);
    if (k == "class") return named(TK::Cls, r.a);
    if (k == "module") return named(TK::Module, r.a);
    if (k == "stdmod") return named(TK::Module, "std:" + r.a);
    if (k == "stdobj") return named(TK::Extern, "std:" + r.a);
    if (k == "builtin") return named(TK::Fn, "builtin:" + r.a);
    if (k == "exc") return named(TK::Cls, "exc:" + r.a);
    if (k == "const") {
        if (r.a == "__doc__") return m && m->has_docstring ? STR() : NONE();
        if (r.a == "__file__" || r.a == "__name__") return issue(n, "dynamic", r.a);
        return BOTTOM();
    }
    if (k == "extern") return issue(n, "extern", r.a.substr(0, r.a.find('.')));
    if (k == "missing") return issue(n, "unresolved", r.a);
    return issue(n, "unresolved", "('" + k + "', '" + r.a + "')");
}

static std::string lower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

TR Eval::ev_inner(const A::Expr* node, const State& state) {
    switch (node->kind) {
        case ExprKind::Constant: {
            auto& v = cast<Constant>(node)->value;
            switch (v.kind) {
                case ConstValue::Kind::None: return NONE();
                case ConstValue::Kind::True:
                case ConstValue::Kind::False: return BOOL();
                case ConstValue::Kind::Int: return const_int(v.fits_i64 ? Big(v.i64) : Big::from_decimal(v.text));
                case ConstValue::Kind::Float: return FLOAT();
                case ConstValue::Kind::Str: return STR();
                case ConstValue::Kind::Bytes: return bytes_of(Big((int64_t)v.text.size()));
                case ConstValue::Kind::Ellipsis: return issue(node, "syntax", "constant ellipsis");
                default: return issue(node, "syntax", "constant complex");
            }
        }
        case ExprKind::JoinedStr: {
            for (auto* v : cast<JoinedStr>(node)->values) {
                auto* fv = as<FormattedValue>(v);
                if (!fv) continue;
                T t = ev(fv->value, state);
                if (fv->format_spec) {
                    bool all_const = true;
                    std::string spec;
                    for (auto* x : cast<JoinedStr>(fv->format_spec)->values) {
                        std::string s;
                        if (is_str_const(x, &s)) spec += s;
                        else all_const = false;
                    }
                    if (!all_const) issue(fv, "dynamic", "computed format spec");
                    else check_format(fv, t, spec);
                } else if (fv->conversion != -1 && fv->conversion != 114 && fv->conversion != 115) {
                    issue(fv, "syntax", std::string("f-string conversion !") + (char)fv->conversion);
                } else {
                    check_str(fv, t, fv->conversion == 114);
                }
            }
            return STR();
        }
        case ExprKind::FormattedValue: return ev(cast<FormattedValue>(node)->value, state);
        case ExprKind::Name: {
            const std::string& name = cast<pyfront::Name>(node)->id;
            if (const T* t = state.get(name)) return TR(*t, Res("local"));
            if (locals.count(name)) return TR(BOTTOM(), Res("local"));
            for (Fn* up = fn ? fn->parent : nullptr; up; up = up->parent)   // (new) a name of an enclosing function
                if (p.fn_locals(up).count(name)) return TR(p.closure_var(up, name), Res("free", up->qname));
            Ref r = lookup(name);
            if (!r) return issue(node, "unresolved", name);
            return TR(ref_type(node, r), ref_res(r));
        }
        case ExprKind::Attribute: {
            auto* n = cast<Attribute>(node);
            T ot = ev(n->value, state);
            TK k = ot.kind;
            if (k == TK::Bottom || k == TK::Top) return ot;
            if (k == TK::Inst) {
                const std::string& cq = ot.name();
                Fn* f = p.find_method(cq, n->attr);
                if (f) {
                    if (f->kind == "property") return TR(p.ret(f->qname), Res("property", f->qname));
                    return TR(named(TK::Fn, "bound:" + f->qname), Res("bound", f->qname));
                }
                auto t = p.attr_type(cq, n->attr);
                if (!t) return issue(node, "unresolved", rsplit_last(cq) + "." + n->attr);
                std::string owner = p.attr_owner(cq, n->attr);
                return TR(*t, Res("field", owner.empty() ? cq : owner, n->attr));
            }
            if (k == TK::Module || (k == TK::Extern && ot.name().rfind("std:", 0) == 0)) {
                if (ot.name().rfind("std:", 0) == 0) {
                    std::string full = ot.name().substr(4) + "." + n->attr;
                    auto& sa = std_attrs();
                    auto it = sa.find(full);
                    if (it != sa.end()) return TR(it->second, Res("stdattr", full));
                    if (p.extended) {   // (new) the re module's flags
                        static const std::map<std::string, int> RE_FLAGS = {{"re.I", 2}, {"re.IGNORECASE", 2}, {"re.M", 8}, {"re.MULTILINE", 8},
                                                                           {"re.S", 16}, {"re.DOTALL", 16}, {"re.X", 64}, {"re.VERBOSE", 64},
                                                                           {"re.A", 256}, {"re.ASCII", 256}};
                        auto rf = RE_FLAGS.find(full);
                        if (rf != RE_FLAGS.end()) return TR(const_int(Big(rf->second)), Res("stdattr", full));
                    }
                    return TR(named(TK::Extern, "std:" + full), Res("stdattr", full));
                }
                Module* mod = *p.modules.get(ot.name());
                Ref r = p.resolve_global(mod, n->attr);
                if (!r) {
                    std::string full = ot.name() + "." + n->attr;
                    if (p.modules.has(full)) return TR(named(TK::Module, full), Res("module", full));
                    return issue(node, "unresolved", full);
                }
                return TR(ref_type(node, r), ref_res(r));
            }
            if (k == TK::Cls) {
                if (ot.name().rfind("exc:", 0) == 0) return issue(node, "dynamic", "exception class attribute");
                Class* c = *p.classes.get(ot.name());
                Fn* f = p.find_method(ot.name(), n->attr);
                if (f) {
                    if (f->kind == "static") return TR(named(TK::Fn, f->qname), Res("fn", f->qname));
                    return issue(node, "dynamic", "unbound method");
                }
                for (auto& cq : p.mro(ot.name()))
                    if ((*p.classes.get(cq))->class_attrs.has(n->attr))
                        return TR(p.class_attr_type(cq, n->attr), Res("classattr", cq, n->attr));
                return issue(node, "unresolved", c->name + "." + n->attr);
            }
            if (k == TK::Extern) return ot;
            if (k == TK::Str || k == TK::Bytes || k == TK::Bytearray || k == TK::List || k == TK::Dict || k == TK::Set || k == TK::Int ||
                k == TK::Float || k == TK::Tuple) {
                std::string kn = kind_name(k);
                return TR(named(TK::Fn, "bmethod:" + kn + "." + n->attr), Res("bmethod", kn + "." + n->attr));
            }
            if (k == TK::Opt) return issue(node, "conflict", "attribute ." + n->attr + " of a value that may be None");
            return issue(node, "conflict", "attribute ." + n->attr + " of a " + kind_name(k));
        }
        case ExprKind::Subscript: {
            auto* n = cast<Subscript>(node);
            T ot = ev(n->value, state);
            TK k = ot.kind;
            if (auto* sl = as<pyfront::Slice>(n->slice)) {
                auto parts = ev_slice_parts(sl, state);
                const A::Expr* pn[3] = {sl->lower, sl->upper, sl->step};
                for (int i = 0; i < 3; i++) {
                    TK pk = parts[i].kind;
                    if (pn[i] && pk != TK::Int && pk != TK::Bottom && pk != TK::Top && pk != TK::None)
                        issue(pn[i], "conflict", std::string("slice bound of ") + kind_name(pk));
                }
                if (sl->step) {
                    Big sv;
                    if (!int_const(sl->step, &sv) || sv.sign() == 0) issue(node, "syntax", "slice with a computed step");
                }
                if (k == TK::Bytes || k == TK::Bytearray) return TR(T(k, {}, {}, slice_cap(sl, ot.hi())), Res("slice", std::string(kind_name(k))));
                if (k == TK::Str) return TR(STR(), Res("slice", std::string("str")));
                if (k == TK::List) {
                    int s = p.new_site(node, TK::List, where(node));
                    p.grow(p.elem, s, p.elem[p.find(ot.site)]);
                    return TR(container(TK::List, s), Res("slice", std::string("list")));
                }
                if (k == TK::Tuple) {
                    Big a, b;
                    bool ha = sl->lower && int_const(sl->lower, &a), hb = sl->upper && int_const(sl->upper, &b);
                    if (!sl->step && (!sl->lower || ha) && (!sl->upper || hb)) {
                        int64_t n_ = (int64_t)ot.args().size();
                        auto norm = [n_](const Big& x) {
                            int64_t v = x.fits_i64() ? x.i64() : (x.sign() < 0 ? -n_ - 1 : n_ + 1);
                            if (v < 0) v += n_;
                            return std::clamp<int64_t>(v, 0, n_);
                        };
                        int64_t lo = ha ? norm(a) : 0, hi = hb ? norm(b) : n_;
                        std::vector<T> part;
                        for (int64_t i = lo; i < hi; i++) part.push_back(ot.args()[i]);
                        std::optional<std::string> ra, rb;
                        if (ha) ra = a.str();
                        if (hb) rb = b.str();
                        return TR(tuple_of(part), Res("tuple_slice", ra, rb));
                    }
                    return issue(node, "conflict", "computed slice of a tuple");
                }
                if (k == TK::Bottom || k == TK::Top) return ot;
                return issue(node, "conflict", std::string("slice of a ") + kind_name(k));
            }
            T it = ev(n->slice, state);
            if (k == TK::Bytes || k == TK::Bytearray) {
                need_int_index(n->slice, it);
                return TR(u(8), Res("index", std::string(kind_name(k))));
            }
            if (k == TK::Str) {
                need_int_index(n->slice, it);
                return TR(STR(), Res("index", std::string("str")));
            }
            if (k == TK::List) {
                need_int_index(n->slice, it);
                return TR(p.elem[p.find(ot.site)], Res("index", std::string("list")));
            }
            if (k == TK::Tuple) {
                Big i;
                if (!int_const(n->slice, &i)) {
                    T e = BOTTOM();
                    for (auto& a : ot.args()) e = p.join(e, a);
                    if (e.kind == TK::Top) return issue(node, "conflict", "computed index into a tuple of mixed items");
                    need_int_index(n->slice, it);
                    return TR(e, Res("tuple_index", std::nullopt));
                }
                int64_t len = (int64_t)ot.args().size();
                if (!(Big(-len) <= i && i < Big(len))) return issue(node, "conflict", "tuple index " + i.str() + " out of range");
                int64_t ii = i.i64();
                return TR(ot.args()[ii < 0 ? ii + len : ii], Res("tuple_index", i.str()));
            }
            if (k == TK::Dict) {
                int s = p.find(ot.site);
                std::string key;
                bool hk = is_str_const(n->slice, &key);
                if (hk && !p.dynamic.count(s)) {
                    T ft = p.fields[s].has(key) ? *p.fields[s].get(key) : BOTTOM();
                    return TR(ft, Res("field_get", key));
                }
                if (!hk) p.make_dynamic(s, "a key that is not a constant string");
                p.grow(p.keys, s, it);
                return TR(p.elem[s], Res("dict_get", std::nullopt));
            }
            if (k == TK::Bottom || k == TK::Top) return ot;
            if (k == TK::Extern && ot.name() == "std:os.environ") {
                if (p.extended) {   // (new) os.environ[k]
                    if (it.kind != TK::Str && it.kind != TK::Bottom && it.kind != TK::Top) return issue(node, "conflict", "os.environ of a non-str");
                    return TR(STR(), Res("index", std::string("os.environ")));
                }
                return issue(node, "stdlib", "os.environ");
            }
            if (p.extended && k == TK::Extern && ot.name().rfind("re.Match", 0) == 0) {   // (new) m[g] is m.group(g)
                std::string pat = ot.name().substr(ot.name().find('', 9) + 1);
                bool verbose = ot.name().substr(9, ot.name().find('', 9) - 9) == "x";
                return ext_match_index(n, pat, verbose);
            }
            if (k == TK::Opt) return issue(node, "conflict", "subscript of a value that may be None");
            return issue(node, "conflict", std::string("subscript of a ") + kind_name(k));
        }
        case ExprKind::BinOp: {
            auto* n = cast<BinOp>(node);
            T a = ev(n->left, state);
            T b = ev(n->right, state);
            return binop(node, n->op, a, b, n->left, n->right);
        }
        case ExprKind::UnaryOp: {
            auto* n = cast<UnaryOp>(node);
            T t = ev(n->operand, state);
            if (n->op == UnaryOpKind::Not) return BOOL();
            if (t.kind == TK::Bottom || t.kind == TK::Top) return t;
            if (t.kind == TK::Bool) t = INT(Big(0), Big(1));
            if (t.kind == TK::Int) {
                if (n->op == UnaryOpKind::USub) return ineg(t);
                if (n->op == UnaryOpKind::UAdd) return t;
                if (n->op == UnaryOpKind::Invert) return iinvert(t);
            }
            if (t.kind == TK::Float && (n->op == UnaryOpKind::USub || n->op == UnaryOpKind::UAdd)) return FLOAT();
            return issue(node, "conflict", std::string(name_of(n->op)) + " of " + kind_name(t.kind));
        }
        case ExprKind::BoolOp: {
            auto* n = cast<BoolOp>(node);
            std::vector<T> ts;
            SP s = state;
            for (auto* v : n->values) {
                T t = ev(v, s ? *s : State());
                ts.push_back(t);
                s = refine(v, s, n->op == BoolOpKind::And);
            }
            T j = BOTTOM();
            p.no_variants++;   // (new) and/or: a clash only means the value is used for its truth
            for (auto& t : ts) j = p.join(j, t);
            p.no_variants--;
            bool allb = true;
            for (auto& t : ts)
                if (t.kind != TK::Bool && t.kind != TK::Bottom) allb = false;
            if (allb) return BOOL();
            if (n->op == BoolOpKind::Or && ts.size() == 2 && ts[0].kind == TK::Opt) {
                T j2 = p.join(strip_opt(ts[0]), ts[1]);
                if (j2.kind != TK::Top) return j2;
            }
            if (j.kind == TK::Top) return TR(BOOL(), Res("mixedbool", j.why()));
            return j;
        }
        case ExprKind::Compare: {
            auto* n = cast<Compare>(node);
            T left = ev(n->left, state);
            for (size_t i = 0; i < n->ops.size(); i++) {
                T right = ev(n->comparators[i], state);
                check_compare(node, n->ops[i], left, right);
                left = right;
            }
            return BOOL();
        }
        case ExprKind::IfExp: {
            auto* n = cast<IfExp>(node);
            ev(n->test, state);
            SP st1 = refine(n->test, state, true), st2 = refine(n->test, state, false);
            T a = ev(n->body, st1 ? *st1 : State());
            T b = ev(n->orelse, st2 ? *st2 : State());
            T j = p.join(a, b);
            if (j.kind == TK::Top) return issue(node, "conflict", j.why());
            return j;
        }
        case ExprKind::NamedExpr:
            ev(cast<NamedExpr>(node)->value, state);
            return issue(node, "syntax", "walrus");
        case ExprKind::Lambda: return issue(node, "dynamic", "first-class lambda");
        case ExprKind::Starred:
            ev(cast<Starred>(node)->value, state);
            return issue(node, "syntax", "starred");
        case ExprKind::Await: return issue(node, "dynamic", "await");
        case ExprKind::Yield: {
            T yt = cast<pyfront::Yield>(node)->value ? ev(cast<pyfront::Yield>(node)->value, state) : NONE();
            if (p.extended && fn && fn->generator && live()) yield_t = p.join(yield_t, fit(node, yt, "yielded value"));
            return p.extended && fn && fn->generator ? NONE() : BOTTOM();
        }
        case ExprKind::YieldFrom:
            if (cast<YieldFrom>(node)->value) ev(cast<YieldFrom>(node)->value, state);
            return BOTTOM();
        case ExprKind::Tuple: {
            auto* n = cast<pyfront::Tuple>(node);
            std::vector<T> ts;
            for (auto* e : n->elts) ts.push_back(ev(e, state));
            for (auto* e : n->elts)
                if (e->kind == ExprKind::Starred) return issue(node, "syntax", "starred in a tuple");
            return tuple_of(ts);
        }
        case ExprKind::List: {
            int s = p.new_site(node, TK::List, where(node));
            for (auto* e : cast<pyfront::List>(node)->elts) {
                T t = ev(e, state);
                p.grow(p.elem, s, t);
            }
            return container(TK::List, s);
        }
        case ExprKind::Set: {
            int s = p.new_site(node, TK::Set, where(node));
            for (auto* e : cast<pyfront::Set>(node)->elts) {
                T t = ev(e, state);
                p.grow(p.elem, s, t);
            }
            return container(TK::Set, s);
        }
        case ExprKind::Dict: {
            auto* n = cast<pyfront::Dict>(node);
            int s = p.new_site(node, TK::Dict, where(node));
            std::vector<std::string> keys;
            for (size_t i = 0; i < n->values.size(); i++) {
                T vt = ev(n->values[i], state);
                if (!n->keys[i]) {
                    spread(s, vt, node);
                    continue;
                }
                T kt = ev(n->keys[i], state);
                std::string key;
                if (is_str_const(n->keys[i], &key)) {
                    p.grow_field(s, key, fit(n->values[i], vt, "field " + key));
                    keys.push_back(key);
                } else {
                    p.make_dynamic(s, "a key that is not a constant string");
                    p.grow(p.keys, s, kt);
                    p.grow(p.elem, s, vt);
                }
            }
            if (live()) {
                std::vector<std::string> lk = keys;
                auto sk = p.spread_keys.find(s);
                if (sk != p.spread_keys.end()) lk.insert(lk.end(), sk->second.begin(), sk->second.end());
                p.literal_keys[node] = lk;
                if (builder != node) p.created(s, lk);
            }
            return container(TK::Dict, s);
        }
        case ExprKind::ListComp: {
            auto* n = cast<ListComp>(node);
            State st = comp_scope(n->generators, state);
            int s = p.new_site(node, TK::List, where(node));
            p.grow(p.elem, s, ev(n->elt, st));
            return container(TK::List, s);
        }
        case ExprKind::SetComp: {
            auto* n = cast<SetComp>(node);
            State st = comp_scope(n->generators, state);
            int s = p.new_site(node, TK::Set, where(node));
            p.grow(p.elem, s, ev(n->elt, st));
            return container(TK::Set, s);
        }
        case ExprKind::DictComp: {
            auto* n = cast<DictComp>(node);
            State st = comp_scope(n->generators, state);
            int s = p.new_site(node, TK::Dict, where(node));
            p.make_dynamic(s, "a comprehension");
            p.grow(p.keys, s, ev(n->key, st));
            p.grow(p.elem, s, ev(n->value, st));
            return container(TK::Dict, s);
        }
        case ExprKind::GeneratorExp: {
            auto* n = cast<GeneratorExp>(node);
            State st = comp_scope(n->generators, state);
            T e = ev(n->elt, st);
            return iter_of(e, MEM);
        }
        case ExprKind::Call: {
            auto* n = cast<pyfront::Call>(node);
            if (p.extended) {   // (new) getattr with a constant name: the attribute, or the default
                int c = getattr_form(n, state);
                if (c == 1) return ev(p.getattr_attr(n, getattr_name(n)), state);
                if (c == 2) return ev(n->args[2], state);
            }
            std::optional<T> recv;
            auto* fa = as<Attribute>(n->func);
            if (fa) recv = ev(fa->value, state);
            T ft = !recv ? ev(n->func, state) : BOTTOM();
            for (auto* a : n->args)
                if (auto* sa = as<Starred>(a)) {
                    ev(sa->value, state);
                    return issue(node, "dynamic", "*args call");
                }
            auto* fname = as<pyfront::Name>(n->func);
            for (auto* k : n->keywords)
                if (!k->has_arg && !(fname && fname->id == "dict")) {
                    ev(k->value, state);
                    return issue(node, "dynamic", "**kwargs call");
                }
            auto lazy = [](const A::Expr* e) { return e->kind == ExprKind::Lambda || e->kind == ExprKind::GeneratorExp; };
            std::vector<std::optional<T>> args;
            for (auto* a : n->args) args.push_back(lazy(a) ? std::nullopt : std::optional<T>(ev(a, state)));
            OMap<std::optional<T>> kw;
            for (auto* k : n->keywords) kw.set(k->has_arg ? k->arg : "", lazy(k->value) ? std::nullopt : std::optional<T>(ev(k->value, state)));
            if (recv) return method_call(n, *recv, fa->attr, args, kw, state);
            return call_value(n, ft, args, kw, state);
        }
        default: {
            return issue(node, "syntax", lower(name_of(node->kind)));
        }
    }
}

std::optional<Big> Eval::slice_cap(const A::Slice* sl, const std::optional<Big>& cap) {
    const A::Expr *lo = sl->lower, *hi = sl->upper;
    if (sl->step) return cap;
    Big hv, lv;
    if (hi && int_const(hi, &hv) && hv.sign() >= 0) {
        if (!lo) return !cap ? hv : std::min(*cap, hv);
        if (int_const(lo, &lv) && lv.sign() >= 0) return std::max(hv - lv, Big(0));
    }
    if (auto* b = as<BinOp>(hi)) {
        Big r;
        if (b->op == OpKind::Add && lo && same_tree(b->left, lo) && int_const(b->right, &r) && r.sign() >= 0) return r;
    }
    return cap;
}

std::vector<T> Eval::ev_slice_parts(const A::Slice* sl, const State& state) {
    std::vector<T> out;
    for (const A::Expr* part : {sl->lower, sl->upper, sl->step}) out.push_back(part ? ev(part, state) : NONE());
    return out;
}

void Eval::need_int_index(const A::Expr* node, const T& t) {
    if (t.kind != TK::Int && t.kind != TK::Bottom && t.kind != TK::Top && t.kind != TK::Bool)
        issue(node, "conflict", std::string("index of ") + kind_name(t.kind));
}

std::pair<std::string, int> Eval::where(const A::Node* n) { return {m ? m->name : "?", n ? n->pos.lineno : 0}; }

static const char* op_class(OpKind op) { return name_of(op); }

TR Eval::binop(AnyNode n, OpKind op, T a, T b, const A::Expr* ln, const A::Expr* rn) {
    if (a.kind == TK::Bottom || b.kind == TK::Bottom) return BOTTOM();
    if (a.kind == TK::Top) return a;
    if (b.kind == TK::Top) return b;
    TK ka = a.kind, kb = b.kind;
    if (ka == TK::Bool) {
        ka = TK::Int;
        a = INT(Big(0), Big(1));
    }
    if (kb == TK::Bool) {
        kb = TK::Int;
        b = INT(Big(0), Big(1));
    }
    if (ka == TK::Int && kb == TK::Int) {
        switch (op) {
            case OpKind::Add: return iadd(a, b);
            case OpKind::Sub: return isub(a, b);
            case OpKind::Mult: return imul(a, b);
            case OpKind::FloorDiv: return ifloordiv(a, b);
            case OpKind::Mod: return imod(a, b);
            case OpKind::LShift: return ishl(a, b);
            case OpKind::RShift: return ishr(a, b);
            case OpKind::BitAnd: return iand(a, b);
            case OpKind::BitOr: return ior(a, b);
            case OpKind::BitXor: return ior(a, b);
            case OpKind::Pow:
                if (b.lo() && b.lo()->sign() >= 0) return ipow_t(a, b);
                return issue(n, "conflict", "int ** a possibly negative int (a float)");
            case OpKind::Div:
                exact_float(n, a);
                exact_float(n, b);
                return FLOAT();
            default: break;
        }
    }
    if ((ka == TK::Int || ka == TK::Float) && (kb == TK::Int || kb == TK::Float)) {
        if (ka == TK::Int) exact_float(n, a);
        if (kb == TK::Int) exact_float(n, b);
        if (op == OpKind::Add || op == OpKind::Sub || op == OpKind::Mult || op == OpKind::Div || op == OpKind::FloorDiv ||
            op == OpKind::Mod || op == OpKind::Pow)
            return FLOAT();
    }
    if (ka == kb && (ka == TK::Str || ka == TK::Bytes || ka == TK::Bytearray) && op == OpKind::Add) {
        if (ka == TK::Str) return STR();
        OBig hi;
        if (a.hi() && b.hi()) hi = *a.hi() + *b.hi();
        return T(ka, {}, {}, hi);
    }
    if (ka == TK::Bytearray && kb == TK::Bytes && op == OpKind::Add) return T(TK::Bytearray);
    if (ka == TK::Bytes && kb == TK::Bytearray && op == OpKind::Add) return T(TK::Bytes);
    if ((ka == TK::Str || ka == TK::Bytes) && kb == TK::Int && op == OpKind::Mult) return ka == TK::Bytes ? T(TK::Bytes) : STR();
    if (ka == TK::Str && op == OpKind::Mod) {
        check_percent(n, ln, rn, b);
        return STR();
    }
    if (ka == TK::Bytes && op == OpKind::Mod) return issue(n, "syntax", "bytes %-formatting");
    if (ka == TK::List && kb == TK::List && op == OpKind::Add) {
        int s = p.new_site(n.p, TK::List, where(n.p));
        p.grow(p.elem, s, p.elem[p.find(a.site)]);
        p.grow(p.elem, s, p.elem[p.find(b.site)]);
        return container(TK::List, s);
    }
    if (ka == TK::List && kb == TK::Int && op == OpKind::Mult) {
        int s = p.new_site(n.p, TK::List, where(n.p));
        p.grow(p.elem, s, p.elem[p.find(a.site)]);
        return container(TK::List, s);
    }
    if (ka == TK::Tuple && kb == TK::Tuple && op == OpKind::Add) {
        std::vector<T> all = a.args();
        all.insert(all.end(), b.args().begin(), b.args().end());
        return tuple_of(all);
    }
    if (ka == TK::Set && kb == TK::Set && (op == OpKind::BitOr || op == OpKind::BitAnd || op == OpKind::Sub || op == OpKind::BitXor)) {
        int s = p.new_site(n.p, TK::Set, where(n.p));
        p.grow(p.elem, s, p.elem[p.find(a.site)]);
        p.grow(p.elem, s, p.elem[p.find(b.site)]);
        return container(TK::Set, s);
    }
    return issue(n, "conflict", std::string(kind_name(ka)) + " " + op_class(op) + " " + kind_name(kb));
}

void Eval::exact_float(AnyNode n, const T& t) {
    static const Big lim = Big::pow2(53);
    if (t.kind == TK::Int && !(t.lo() && t.hi() && -lim <= *t.lo() && *t.hi() <= lim))
        issue(n, "int-range", "an int beyond 2**53 meets a float");
}

void Eval::check_percent(AnyNode n, const A::Expr* ln, const A::Expr*, const T& b) {
    std::string fmt;
    if (!is_str_const(ln, &fmt)) {
        issue(n, "dynamic", "% with a computed format");
        return;
    }
    struct Spec {
        std::string width, prec;
        char conv;
    };
    std::vector<Spec> specs;
    size_t i = 0;
    while (i < fmt.size()) {
        if (fmt[i] != '%') {
            i++;
            continue;
        }
        size_t j = i + 1;
        while (j < fmt.size() && std::string("-+ #0").find(fmt[j]) != std::string::npos) j++;
        Spec sp;
        if (j < fmt.size() && fmt[j] == '*') sp.width = fmt.substr(j++, 1);
        else {
            size_t k = j;
            while (k < fmt.size() && isdigit((unsigned char)fmt[k])) k++;
            sp.width = fmt.substr(j, k - j);
            j = k;
        }
        if (j + 1 < fmt.size() && fmt[j] == '.' && (fmt[j + 1] == '*' || isdigit((unsigned char)fmt[j + 1]))) {
            if (fmt[j + 1] == '*') {
                sp.prec = "*";
                j += 2;
            } else {
                size_t k = j + 1;
                while (k < fmt.size() && isdigit((unsigned char)fmt[k])) k++;
                sp.prec = fmt.substr(j + 1, k - j - 1);
                j = k;
            }
        }
        if (j < fmt.size() && (isalpha((unsigned char)fmt[j]) || fmt[j] == '%')) {
            sp.conv = fmt[j];
            specs.push_back(sp);
            i = j + 1;
        } else {
            i++;
        }
    }
    for (auto& sp : specs)
        if (std::string("dixXosrcfeEgG%").find(sp.conv) == std::string::npos || sp.width == "*" || sp.prec == "*")
            issue(n, "syntax", std::string("percent conversion %") + sp.conv);
    if (fmt.find("%(") != std::string::npos) issue(n, "syntax", "percent with a mapping");
    size_t nvals = 0;
    for (auto& sp : specs)
        if (sp.conv != '%') nvals++;
    std::vector<T> items = b.kind == TK::Tuple ? b.args() : std::vector<T>{b};
    if (b.kind == TK::Tuple && items.size() != nvals)
        issue(n, "conflict", "% with " + std::to_string(items.size()) + " values for " + std::to_string(nvals) + " conversions");
    for (auto& t : items) {
        TK k = t.kind;
        if ((k == TK::List || k == TK::Dict || k == TK::Set || k == TK::Inst || k == TK::Opt) &&
            !(k == TK::Opt && (t.args()[0].kind == TK::Int || t.args()[0].kind == TK::Str)))
            issue(n, "syntax", std::string("% of a ") + kind_name(k));
    }
}

void Eval::check_str(AnyNode node, const T& t, bool as_repr) {
    TK k = t.kind;
    if (k == TK::Variant) return;   // (new) each alternative prints
    if (k == TK::Int || k == TK::Str || k == TK::Bool || k == TK::None || k == TK::Bytes || k == TK::Bytearray || k == TK::Bottom ||
        k == TK::Top || k == TK::Float)
        return;
    if (k == TK::Opt && (t.args()[0].kind == TK::Int || t.args()[0].kind == TK::Str || t.args()[0].kind == TK::Bool)) return;
    if ((k == TK::Tuple || k == TK::List) && as_repr) return;
    issue(node, "syntax", std::string("str() of a ") + kind_name(k));
}

static bool format_spec_ok(const std::string& s) {
    auto rest_ok = [&](size_t i) {
        if (i < s.size() && (s[i] == '+' || s[i] == '-' || s[i] == ' ')) i++;
        if (i < s.size() && s[i] == '#') i++;
        if (i < s.size() && s[i] == '0') i++;
        while (i < s.size() && isdigit((unsigned char)s[i])) i++;
        if (i < s.size() && s[i] == ',') i++;
        if (i + 1 < s.size() && s[i] == '.' && isdigit((unsigned char)s[i + 1])) {
            i++;
            while (i < s.size() && isdigit((unsigned char)s[i])) i++;
        }
        if (i < s.size() && std::string("bdoxXcsfeEg%").find(s[i]) != std::string::npos) i++;
        return i == s.size();
    };
    auto align = [](char c) { return c == '<' || c == '>' || c == '^'; };
    if (rest_ok(0)) return true;
    if (!s.empty() && align(s[0]) && rest_ok(1)) return true;
    if (s.size() >= 2 && s[0] != '\n') {
        // one character (a code point) of fill, then the alignment
        size_t clen = 1;
        unsigned char c0 = (unsigned char)s[0];
        if (c0 >= 0xF0) clen = 4;
        else if (c0 >= 0xE0) clen = 3;
        else if (c0 >= 0xC0) clen = 2;
        if (clen < s.size() && align(s[clen]) && rest_ok(clen + 1)) return true;
    }
    return false;
}

void Eval::check_format(AnyNode node, const T& t, const std::string& spec) {
    if (!format_spec_ok(spec)) {
        issue(node, "syntax", "format spec " + py_repr(spec));
        return;
    }
    bool last_ok = spec.empty() || std::string("fFeEg%").find(spec.back()) != std::string::npos;
    if ((t.kind == TK::Int || t.kind == TK::Bool) && last_ok) return;
    if (t.kind != TK::Int && t.kind != TK::Str && t.kind != TK::Float && t.kind != TK::Bool && t.kind != TK::Bottom && t.kind != TK::Top)
        issue(node, "syntax", std::string("format of a ") + kind_name(t.kind));
}

void Eval::check_compare(AnyNode n, CmpOpKind op, const T& a, const T& b) {
    if (a.kind == TK::Bottom || a.kind == TK::Top || b.kind == TK::Bottom || b.kind == TK::Top) return;
    if (op == CmpOpKind::Is || op == CmpOpKind::IsNot) {
        if (!(b.kind == TK::None || a.kind == TK::None)) issue(n, "dynamic", std::string("identity comparison of ") + kind_name(a.kind));
        return;
    }
    if (op == CmpOpKind::In || op == CmpOpKind::NotIn) {
        auto* cmp = static_cast<const Compare*>(n.p);
        if (b.kind == TK::List || b.kind == TK::Set) {
            comparable(n, a, p.elem[p.find(b.site)]);
        } else if (b.kind == TK::Dict) {
            int s = p.find(b.site);
            bool sc = is_str_const(cmp->left);
            if (!(sc && !p.dynamic.count(s))) {
                if (!sc) p.make_dynamic(s, "membership of a computed key");
            }
        } else if (b.kind == TK::Tuple) {
            for (auto& x : b.args()) comparable(n, a, x);
        } else if (b.kind == TK::Str || b.kind == TK::Bytes || b.kind == TK::Bytearray) {
            bool bb = (a.kind == TK::Bytes || a.kind == TK::Bytearray) && (b.kind == TK::Bytes || b.kind == TK::Bytearray);
            if (!(a.kind == b.kind || (a.kind == TK::Int && b.kind != TK::Str) || bb))
                issue(n, "conflict", std::string(kind_name(a.kind)) + " in " + kind_name(b.kind));
        } else {
            issue(n, "conflict", std::string("membership in a ") + kind_name(b.kind));
        }
        return;
    }
    if (op == CmpOpKind::Eq || op == CmpOpKind::NotEq) {
        comparable(n, a, b, true);
        return;
    }
    comparable(n, a, b, false, true);
}

void Eval::comparable(AnyNode n, const T& a, const T& b, bool eq, bool order) {
    TK ka = a.kind, kb = b.kind;
    if (ka == TK::Bottom || kb == TK::Bottom || ka == TK::Top || kb == TK::Top) return;
    if (ka == TK::Bool) ka = TK::Int;
    if (kb == TK::Bool) kb = TK::Int;
    if (ka == kb && (ka == TK::Int || ka == TK::Str || ka == TK::Float || ka == TK::Bytes || ka == TK::Bytearray || ka == TK::None)) return;
    if ((ka == TK::Bytes && kb == TK::Bytearray) || (ka == TK::Bytearray && kb == TK::Bytes)) return;
    if ((ka == TK::Int && kb == TK::Float) || (ka == TK::Float && kb == TK::Int)) {
        exact_float(n, ka == TK::Int ? a : b);
        return;
    }
    if (ka == TK::Tuple && kb == TK::Tuple && a.args().size() == b.args().size()) {
        for (size_t i = 0; i < a.args().size(); i++) comparable(n, a.args()[i], b.args()[i], eq, order);
        return;
    }
    if (eq && (ka == TK::Opt || kb == TK::Opt)) {
        T ia = strip_opt(a), ib = strip_opt(b);
        if (ib.kind == TK::None || ia.kind == TK::None) return;
        comparable(n, ia, ib, eq, order);
        return;
    }
    if (eq && ka == TK::List && kb == TK::List) {
        comparable(n, p.elem[p.find(a.site)], p.elem[p.find(b.site)], eq);
        return;
    }
    if (eq && (ka == TK::Variant || kb == TK::Variant)) {   // (new) == against one of its alternatives
        const T& v = ka == TK::Variant ? a : b;
        const T& o = ka == TK::Variant ? b : a;
        for (auto& alt : v.args())
            if (alt.kind == o.kind || (alt.kind == TK::Bool && o.kind == TK::Int) || (alt.kind == TK::Int && o.kind == TK::Bool)) return;
        issue(n, "conflict", "comparing a variant with a " + std::string(kind_name(o.kind)));
        return;
    }
    if (eq && ka == TK::Inst && kb == TK::Inst) {
        issue(n, "dynamic", "equality of instances");
        return;
    }
    if (eq) {
        issue(n, "conflict", std::string("comparing ") + kind_name(ka) + " with " + kind_name(kb) + " (always unequal in Python)");
        return;
    }
    issue(n, "conflict", std::string("ordering ") + kind_name(ka) + " against " + kind_name(kb));
}

void Eval::spread(int s, const T& vt, AnyNode n) {
    if (vt.kind != TK::Dict) {
        if (vt.kind != TK::Bottom && vt.kind != TK::Top) issue(n, "conflict", std::string("** of a ") + kind_name(vt.kind));
        return;
    }
    int src = p.find(vt.site);
    if (p.dynamic.count(src)) {
        p.make_dynamic(s, "filled from a dict");
        p.grow(p.keys, s, p.keys[src]);
        p.grow(p.elem, s, p.elem[src]);
        return;
    }
    OMap<T> fs = p.fields[src];
    for (auto& [key, t] : fs) p.grow_field(s, key, t);
    p.spread_keys[s] = p.required[src] ? *p.required[src] : std::set<std::string>();
}

State Eval::comp_scope(const std::vector<A::comprehension*>& gens, const State& state) {
    State st = state;
    for (auto* g : gens) {
        if (g->is_async) issue(AnyNode(g, AnyNode::Comprehension), "dynamic", "async comprehension");
        T it = ev(g->iter, st);
        auto [elem, n] = iteration(g->iter, it, st);
        st = assign(g->target, elem, st, nullptr);
        for (auto* cond : g->ifs) {
            ev(cond, st);
            SP r = refine(cond, st, true);
            if (r) st = *r;
        }
    }
    return st;
}

// ------------------------------------------------------------------------------------ calls
TR Eval::call_value(const A::Call* n, const T& ft, const std::vector<std::optional<T>>& args, const OMap<std::optional<T>>& kw,
                    const State& state) {
    TK k = ft.kind;
    if (k == TK::Bottom || k == TK::Top) return ft;
    if (k == TK::Fn) {
        const std::string& name = ft.name();
        if (name.rfind("builtin:", 0) == 0) return builtin(n, name.substr(8), args, kw, state);
        if (name.rfind("bound:", 0) == 0) return call_fn(n, *p.fns.get(name.substr(6)), args, kw, true);
        if (!p.fns.has(name)) {
            auto c = name.find(':');
            return issue(n, "dynamic", "a method kept as a value (" + (c == std::string::npos ? name : name.substr(c + 1)) + ")");
        }
        return call_fn(n, *p.fns.get(name), args, kw);
    }
    if (k == TK::Cls) {
        if (ft.name().rfind("exc:", 0) == 0) {
            for (auto& a : args)
                if (a && a->kind != TK::Str && a->kind != TK::Bottom && a->kind != TK::Top && a->kind != TK::Int)
                    issue(n, "syntax", ft.name().substr(4) + " with a " + kind_name(a->kind));
            if (args.size() > 1) issue(n, "syntax", "exception with several arguments");
            return TR(named(TK::Inst, ft.name()), Res("exc", ft.name().substr(4)));
        }
        return construct(n, ft.name(), args, kw);
    }
    if (k == TK::Extern) {
        if (ft.name().rfind("std:", 0) == 0) return std_call(n, ft.name().substr(4), args, kw, state);
        return issue(n, "extern", ft.name().substr(0, ft.name().find('.')));
    }
    if (k == TK::Closure) {   // (new) a nested function, called where it is in scope
        Fn* nf = p.nested.has(ft.name()) ? *p.nested.get(ft.name()) : nullptr;
        if (!nf) return issue(n, "unresolved", ft.name());
        TR r = call_fn(n, nf, args, kw);
        r.r = Res("closure", ft.name());
        return r;
    }
    return issue(n, "conflict", std::string("calling a ") + kind_name(k));
}

TR Eval::construct(const A::Call* n, const std::string& cq, const std::vector<std::optional<T>>& args,
                   const OMap<std::optional<T>>& kw) {
    Class* c = *p.classes.get(cq);
    if (!c->foreign_base.empty()) return issue(n, "extern", "class derived from " + c->foreign_base);
    Fn* init = p.find_method(cq, "__init__");
    if (init) {
        bind_args(n, init, args, kw, true);
    } else if (!c->exception.empty() && args.size() <= 1 && kw.empty()) {
        if (!args.empty() && args[0] && args[0]->kind != TK::Str && args[0]->kind != TK::Bottom && args[0]->kind != TK::Top)
            issue(n, "syntax", c->name + " with a " + kind_name(args[0]->kind));
    } else if (!args.empty() || !kw.empty()) {
        issue(n, "conflict", "arguments to a class with no __init__");
    }
    return TR(named(TK::Inst, cq), Res("ctor", cq));
}

static std::string sig_of(const T& t) {
    switch (t.kind) {
        case TK::Inst: return t.name().substr(t.name().rfind('.') + 1);
        case TK::Tuple: {
            std::string s = "tuple(";
            for (size_t i = 0; i < t.args().size(); i++) s += (i ? "," : "") + sig_of(t.args()[i]);
            return s + ")";
        }
        case TK::Opt: return "optional(" + sig_of(t.args()[0]) + ")";
        default: return kind_name(t.kind);
    }
}

TR Eval::call_fn(const A::Call* n, Fn* f, const std::vector<std::optional<T>>& args, const OMap<std::optional<T>>& kw, bool bound) {
    if (f->kind == "property") return issue(n, "conflict", "calling a property");
    if (p.extended && f->qname == "codebreaker.formats.native.call" && !n->args.empty()) {   // native.call("q", data, ...)
        std::string q;
        if (is_str_const(n->args[0], &q)) {
            T t = native_question_type(p, q);
            if (t.kind != TK::Bottom) return TR(t, Res("native_call", q));
        }
    }
    if (p.native_bound.count(f->qname)) {   // a native answer: one type for every caller (answers.cc), no copies
        bind_args(n, f, args, kw, bound);
        auto wi = p.native_where.find(f->qname);
        if (wi != p.native_where.end() && f->cls) {   // its place function, called with self
            Fn** wf = p.fns.get(wi->second);
            if (wf) bind_args(n, *wf, {std::optional<T>(named(TK::Inst, f->cls->qname))}, {}, false);
        }
        return TR(p.ret(f->qname), Res(bound ? "method" : "fn", f->qname));
    }
    if (p.demand_enabled && !bound && p.spec_candidates.count(f->qname)) {   // (new) this caller's own copy
        std::string sig;
        for (auto& a : args) sig += (sig.empty() ? "" : ",") + (a ? sig_of(*a) : std::string("lambda"));
        for (auto& [k, v] : kw) sig += (sig.empty() ? "" : ",") + k + "=" + (v ? sig_of(*v) : std::string("lambda"));
        if (sig.find("bottom") != std::string::npos || sig.find("top") != std::string::npos) return BOTTOM();
        Fn* c = p.spec_for(f, sig);
        bind_args(n, c, args, kw, false);
        T r = p.ret(c->qname);
        if (r.kind == TK::Top) r = top("calls-refused:" + f->name);
        return TR(r, Res("fn", c->qname));
    }
    bind_args(n, f, args, kw, bound);
    std::vector<Fn*> targets = {f};
    if (bound && f->cls) {
        for (auto& d : p.descendants(f->cls->qname)) {
            Fn** o = (*p.classes.get(d))->methods.get(f->name);
            if (o) {
                bind_args(n, *o, args, kw, true);
                targets.push_back(*o);
            }
        }
    }
    T r = BOTTOM();
    for (Fn* t : targets) r = p.join(r, p.ret(t->qname));
    if (r.kind == TK::Top) r = top("calls-refused:" + f->name);
    return TR(r, Res(bound ? "method" : "fn", f->qname));
}

void Eval::bind_args(const A::Call* n, Fn* f, const std::vector<std::optional<T>>& args, const OMap<std::optional<T>>& kw, bool bound) {
    bool methodish = f->kind == "method" || f->kind == "property" || f->kind == "classmethod";
    std::vector<std::string> names = (bound || !methodish) ? f->bound() : f->params;
    if ((f->kind == "method" || f->kind == "property") && !bound) names = f->params;
    if (!f->varargs.empty() || !f->kwargs.empty()) return;
    if (args.size() > names.size()) {
        issue(n, "conflict", "too many arguments to " + f->name);
        return;
    }
    std::set<std::string> given;
    for (size_t i = 0; i < args.size(); i++) {
        given.insert(names[i]);
        if (args[i]) contribute_param(f->qname, names[i], arg_fit(n, f, names[i], *args[i]));
        else issue(n, "dynamic", "lambda or generator passed to " + f->name);
    }
    for (auto& [k, t] : kw) {
        if (std::find(names.begin(), names.end(), k) == names.end() && std::find(f->kwonly.begin(), f->kwonly.end(), k) == f->kwonly.end()) {
            issue(n, "conflict", "no parameter " + (k.empty() ? std::string("None") : k) + " of " + f->name);
            continue;
        }
        given.insert(k);
        if (t) contribute_param(f->qname, k, arg_fit(n, f, k, *t));
    }
    std::vector<std::string> all = names;
    all.insert(all.end(), f->kwonly.begin(), f->kwonly.end());
    for (auto& name : all)
        if (!given.count(name) && !f->defaults.has(name)) issue(n, "conflict", "missing argument " + name + " to " + f->name);
}

TR Eval::method_call(const A::Call* n, const T& recv, const std::string& attr, const std::vector<std::optional<T>>& args,
                     const OMap<std::optional<T>>& kw, const State& state) {
    TK k = recv.kind;
    if (k == TK::Bottom || k == TK::Top) return recv;
    if (k == TK::Inst) {
        if (recv.name().rfind("exc:", 0) == 0) return issue(n, "dynamic", "method of an exception");
        Fn* f = p.find_method(recv.name(), attr);
        if (!f) {
            auto t = p.attr_type(recv.name(), attr);
            if (t) return issue(n, "dynamic", "calling an attribute (" + attr + ")");
            return issue(n, "unresolved", rsplit_last(recv.name()) + "." + attr + "()");
        }
        if (f->kind == "static") return call_fn(n, f, args, kw);
        return call_fn(n, f, args, kw, true);
    }
    if (k == TK::Module) {
        if (recv.name().rfind("std:", 0) == 0) return std_call(n, recv.name().substr(4) + "." + attr, args, kw, state);
        Module* mod = *p.modules.get(recv.name());
        Ref r = p.resolve_global(mod, attr);
        if (!r) return issue(n, "unresolved", recv.name() + "." + attr);
        T ft = ref_type(n->func, r);
        return call_value(n, ft, args, kw, state);
    }
    if (k == TK::Cls) {
        if (recv.name().rfind("exc:", 0) == 0) return issue(n, "dynamic", "exception class method");
        Fn* f = p.find_method(recv.name(), attr);
        if (f && f->kind == "static") return call_fn(n, f, args, kw);
        if (f && f->kind == "method" && !args.empty() && args[0] && args[0]->kind == TK::Inst) {
            auto mro = p.mro(args[0]->name());
            auto* a0 = as<pyfront::Name>(n->args[0]);
            if (std::find(mro.begin(), mro.end(), recv.name()) != mro.end() && a0 && fn && !fn->params.empty() && a0->id == fn->params[0]) {
                if (attr == "__init__" && !(fn->name == "__init__" && first_stmt(n)))
                    return issue(n, "syntax", "base __init__ called other than first in __init__");
                std::vector<std::optional<T>> rest(args.begin() + 1, args.end());
                bind_args(n, f, rest, kw, true);
                return TR(p.ret(f->qname), Res("basecall", f->qname));
            }
        }
        return issue(n, "dynamic", rsplit_last(recv.name()) + "." + attr + " through the class");
    }
    if (k == TK::Extern) {
        if (recv.name().rfind("std:", 0) == 0) return std_call(n, recv.name().substr(4) + "." + attr, args, kw, state);
        if (recv.name() == "file") {
            if (p.extended) {
                bool handled = false;
                TR r = ext_method(n, recv, attr, args, kw, state, handled);
                if (handled) return r;
            }
            if (attr == "read" && args.empty()) return TR(BYTES(), Res("bmethod", std::string("file.read")));
            return issue(n, "stdlib", "file." + attr);
        }
        if (recv.name() == "super") return super_call(n, attr, args, kw);
        if (p.extended) {
            bool handled = false;
            TR r = ext_method(n, recv, attr, args, kw, state, handled);
            if (handled) return r;
        }
        return issue(n, "extern", recv.name().substr(0, recv.name().find('.')));
    }
    if (k == TK::Opt) return issue(n, "conflict", "." + attr + "() of a value that may be None");
    return bmethod(n, recv, attr, args, kw, state);
}

bool Eval::first_stmt(const A::Call* call) {
    std::vector<const A::Stmt*> body;
    for (auto* s : fn->node->body) {
        auto* e = as<ExprStmt>(s);
        if (e && is_const(e->value)) continue;
        body.push_back(s);
    }
    if (body.empty()) return false;
    auto* e = as<ExprStmt>(body[0]);
    return e && e->value == call;
}

TR Eval::super_call(const A::Call* n, const std::string& attr, const std::vector<std::optional<T>>& args,
                    const OMap<std::optional<T>>& kw) {
    if (!cls || cls->bases.empty()) return issue(n, "dynamic", "super() without a base of the program");
    Fn* f = p.find_method(cls->bases[0], attr);
    if (!f) return issue(n, "unresolved", "super()." + attr);
    bind_args(n, f, args, kw, true);
    return TR(p.ret(f->qname), Res("super", f->qname));
}

T Eval::lambda_type(const A::Expr* lam, const std::vector<T>& params, const State& state) {
    auto* l = as<pyfront::Lambda>(lam);
    if (!l) return ev(lam, state);
    const arguments* a = l->args;
    if (a->vararg || a->kwarg || !a->kwonlyargs.empty() || a->args.size() != params.size()) return issue(lam, "dynamic", "lambda signature");
    State st = state;
    for (size_t i = 0; i < a->args.size(); i++) {
        st.set(a->args[i]->arg, params[i]);
        note(a->args[i], params[i]);
    }
    T t = ev(l->body, st);
    note(lam, t);
    return t;
}

std::pair<T, State> Eval::gen_elem(const A::GeneratorExp* g, const State& state) {
    State st = comp_scope(g->generators, state);
    return {ev(g->elt, st), st};
}

// ------------------------------------------------------------------------------------ module level
ModuleEval::ModuleEval(Program& p_, Module* mod, bool record_) : Eval(p_, nullptr, record_) { m = mod; }

T ModuleEval::safe_ev(const A::Expr* v) {
    try {
        return ev(v, State());
    } catch (const std::exception& e) {
        return top(std::string("frontend-error:") + e.what());
    }
}

Eval& ModuleEval::run() {
    auto& g = p.globals[m->name];
    for (auto& [name, values] : m->assigns) {
        T t = BOTTOM();
        for (auto* v : values) t = p.join(t, v ? safe_ev(v) : top("module-level unpacking"));
        T old = g.count(name) ? g[name] : BOTTOM();
        T nw = p.join(old, t);
        if (nw != old) {
            p.globals[m->name][name] = nw;
            p.touch("g:" + m->name);
        }
    }
    for (auto& [_, c] : m->classes) {
        for (auto& [name, v] : c->class_attrs) {
            T t = safe_ev(v);
            std::string key = c->qname + "\x1f" + name;
            T old = p.class_attr_types.count(key) ? p.class_attr_types[key] : BOTTOM();
            T nw = p.join(old, t);
            if (nw != old) {
                p.class_attr_types[key] = nw;
                p.touch("ca");
            }
        }
    }
    std::vector<Fn*> fs;
    for (auto& [_, f] : m->functions) fs.push_back(f);
    for (auto& [_, c] : m->classes)
        for (auto& [__, f] : c->methods) fs.push_back(f);
    for (Fn* f : fs) {
        for (auto& [name, d] : f->defaults) {
            T t;
            if (d->kind == ExprKind::List || d->kind == ExprKind::Dict || d->kind == ExprKind::Set || d->kind == ExprKind::Call)
                t = top("mutable default");
            else
                t = safe_ev(d);
            std::string key = f->qname + "\x1f" + name;
            T old = p.default_types.count(key) ? p.default_types[key] : BOTTOM();
            T nw = p.join(old, t);
            if (nw != old) {
                p.default_types[key] = nw;
                p.touch("d");
            }
        }
    }
    return *this;
}

// ------------------------------------------------------------------------------------ solving
static bool intersects(const std::unordered_set<std::string>& a, const std::unordered_set<std::string>& b) {
    const auto& small = a.size() < b.size() ? a : b;
    const auto& big = a.size() < b.size() ? b : a;
    for (auto& x : small)
        if (big.count(x)) return true;
    return false;
}

int Program::solve(int max_rounds, bool log) {
    for (auto& [_, f] : fns) map_blocks(f->node);
    std::unordered_map<std::string, std::unordered_set<std::string>> deps;
    std::optional<std::unordered_set<std::string>> prev;
    for (int r = 0; r < max_rounds + 20; r++) {
        round = r;
        changed = false;
        dirty.clear();
        int ran = 0;
        for (auto& [_, mod] : modules) {
            std::string key = "mod\x1f" + mod->name;
            if (prev && !intersects(deps[key], *prev)) continue;
            std::unordered_set<std::string> rd;
            reads = &rd;
            suspend = 0;
            ModuleEval(*this, mod).run();
            deps[key] = std::move(rd);
            reads = nullptr;
        }
        for (auto& [q, f] : fns) {
            if (crashes.count(q)) continue;
            if (prev && !intersects(deps[q], *prev)) continue;
            ran++;
            std::unordered_set<std::string> rd;
            reads = &rd;
            suspend = 0;
            try {
                Eval(*this, f).run();
            } catch (const std::exception& e) {
                crashes[q] = std::string("RuntimeError: ") + e.what();
            }
            deps[q] = std::move(rd);
            reads = nullptr;
        }
        std::vector<std::pair<std::string, Fn*>> spec_list(specs.begin(), specs.end());
        for (auto& [q, f] : spec_list) {   // (new) the specializations (a new one runs at once)
            if (crashes.count(q)) continue;
            auto di = deps.find(q);
            if (prev && di != deps.end() && !intersects(di->second, *prev)) continue;
            ran++;
            std::unordered_set<std::string> rd;
            reads = &rd;
            suspend = 0;
            try {
                Eval(*this, f).run();
            } catch (const std::exception& e) {
                crashes[q] = std::string("RuntimeError: ") + e.what();
            }
            deps[q] = std::move(rd);
            reads = nullptr;
        }
        for (auto& [q, f] : nested) {
            if (crashes.count(q)) continue;
            if (prev && !intersects(deps[q], *prev)) continue;
            ran++;
            std::unordered_set<std::string> rd;
            reads = &rd;
            suspend = 0;
            try {
                Eval(*this, f).run();
            } catch (const std::exception& e) {
                crashes[q] = std::string("RuntimeError: ") + e.what();
            }
            deps[q] = std::move(rd);
            reads = nullptr;
        }
        if (log) fprintf(stderr, "round %d: ran %d, %zu facts changed\n", r, ran, dirty.size());
        if (dirty.empty() && extended && !demand_enabled) {
            // (new) the classic rules have settled: fix their facts as contracts, then let the
            // parameters no caller types take what their uses demand, and run everything again
            freeze();
            apply_type_hints();
            demand_enabled = true;
            vetoes.clear();
            prev.reset();
            continue;
        }
        if (dirty.empty()) return r + 1;
        std::unordered_set<std::string> pv = dirty;
        for (auto& k : dirty)
            if (k.rfind("s:", 0) == 0) pv.insert("s:" + std::to_string(find(std::stoi(k.substr(2)))));
        prev = std::move(pv);
    }
    return max_rounds;
}

}  // namespace pyengine
