// demand.cc -- (new) a parameter no typed call reaches takes the type its uses demand.
//
// The old engine typed a parameter only from annotations and callers; one untyped entry point
// (argv, an argparse namespace, a dispatch table) left every function below it untyped too -- its
// largest refusal class. Here a parameter that is never rebound in its function and that no caller
// types gets the type its own uses require, when they agree:
//
//   passed to a typed parameter     f(p) where f's parameter is typed: that type
//   attributes and methods          p.attr / p.method(): the one class hierarchy of the program
//                                   that has all of them (its most basic class); none or several
//                                   such hierarchies: no demand
//   str-only operations             open(p), os.path.*(p), os.* paths, p.startswith('x'),
//                                   p == 'x', p + 'x', p.split('x') ... (b'x': bytes)
//   range(p)                        an int
//
// A demand is a contract, like an annotation: the function is written for that type, and a caller
// that passes another kind is refused where it calls (as the old engine's contracts refuse it).
#include <algorithm>

#include "infer.hpp"
#include "pyfront/pyfront.hpp"

namespace pyengine {

using namespace pyfront;

struct DemandUse {
    enum Kind { ArgTo, Attr, Method, Str, Bytes, Int } kind;
    Fn* callee = nullptr;
    std::string name;     // the callee's parameter / the attribute
};

struct DemandInfo {
    bool rebound = false;
    std::vector<DemandUse> uses;
};

static const std::set<std::string> STR_PATH_FUNCS = {
    "os.path.join", "os.path.basename", "os.path.dirname", "os.path.splitext", "os.path.exists", "os.path.isfile", "os.path.isdir",
    "os.path.getsize", "os.path.split", "os.path.normcase", "os.path.abspath", "os.path.realpath", "os.path.normpath", "os.path.expanduser",
    "os.path.relpath", "os.path.isabs", "os.path.getmtime", "os.listdir", "os.makedirs", "os.remove", "os.unlink", "os.rmdir", "os.walk",
    "shutil.rmtree", "shutil.copyfile", "shutil.copy", "shutil.copy2", "shutil.move"};
static const std::set<std::string> STR_ONLY_METHODS = {"isidentifier", "isprintable", "format", "casefold", "encode", "removeprefix", "removesuffix"};
static const std::set<std::string> BYTES_ONLY_METHODS = {"decode", "hex"};
static const std::set<std::string> SEQ_LITERAL_METHODS = {"startswith", "endswith", "split", "rsplit", "replace", "find", "rfind", "index",
                                                         "count", "strip", "lstrip", "rstrip", "partition", "rpartition"};

// the module-qualified name of a std call's function: os.path.join for os.path.join(...), as the module imported it
static std::string std_name(Program& p, Module* m, const A::Expr* f) {
    std::vector<std::string> parts;
    const A::Expr* e = f;
    while (auto* at = as<Attribute>(e)) {
        parts.push_back(at->attr);
        e = at->value;
    }
    auto* nm = as<pyfront::Name>(e);
    if (!nm) return "";
    Ref r = p.resolve_global(m, nm->id);
    std::string head;
    if (r.k == "stdmod" || r.k == "stdobj") head = r.a;
    else if (!r && nm->id == "open") head = "builtins.open";
    else return "";
    for (auto it = parts.rbegin(); it != parts.rend(); ++it) head += "." + *it;
    return head;
}

static const DemandInfo& demand_info(Program& p, Fn* f, const std::string& param) {
    static std::unordered_map<std::string, DemandInfo> cache;
    std::string key = f->qname + "\x1f" + param;
    auto it = cache.find(key);
    if (it != cache.end()) return it->second;
    DemandInfo info;
    ParentMap pm;
    pm.build(AnyNode(f->node));
    for (const AnyNode& n : walk(AnyNode(f->node))) {
        auto* nm = as<pyfront::Name>(n.expr());
        if (!nm || nm->id != param) continue;
        if (nm->ctx != ExprCtx::Load) {
            info.rebound = true;
            break;
        }
        AnyNode up = pm.parent(nm);
        const A::Expr* ue = up.expr();
        if (auto* c = as<pyfront::Call>(ue)) {
            // p as an argument
            size_t pos = 0;
            bool found = false;
            for (; pos < c->args.size(); pos++)
                if (c->args[pos] == nm) {
                    found = true;
                    break;
                }
            if (!found) continue;
            Fn* callee = nullptr;
            bool bound = false;
            if (auto* fn = as<pyfront::Name>(c->func)) {
                Ref r = p.resolve_global(f->module, fn->id);
                if (r.k == "fn") callee = *p.fns.get(r.a);
                else if (r.k == "class") {
                    callee = p.find_method(r.a, "__init__");
                    bound = true;
                }
            } else if (auto* at = as<Attribute>(c->func)) {
                auto* self = as<pyfront::Name>(at->value);
                if (self && f->cls && !f->params.empty() && self->id == f->params[0] && f->kind == "method") {
                    callee = p.find_method(f->cls->qname, at->attr);
                    bound = callee && callee->kind != "static";
                } else {
                    Ref r = p.resolve_expr_name(f->module, at);
                    if (r.k == "fn") callee = *p.fns.get(r.a);
                }
            }
            std::string sn = std_name(p, f->module, c->func);
            if (callee && !callee->varargs.size() && !callee->kwargs.size()) {
                auto names = bound ? callee->bound() : callee->params;
                if (callee->kind == "static") names = callee->params;
                if (pos < names.size()) info.uses.push_back({DemandUse::ArgTo, callee, names[pos]});
            } else if (STR_PATH_FUNCS.count(sn) || sn == "builtins.open") {
                info.uses.push_back({DemandUse::Str});
            } else if (auto* fn = as<pyfront::Name>(c->func)) {
                if (fn->id == "range" && !p.resolve_global(f->module, "range")) info.uses.push_back({DemandUse::Int});
            }
            continue;
        }
        if (auto* at = as<Attribute>(ue)) {
            if (at->value != nm) continue;
            AnyNode up2 = pm.parent(at);
            auto* call = as<pyfront::Call>(up2.expr());
            bool is_call = call && call->func == at;
            if (is_call) {
                std::string lit;
                bool str_arg = !call->args.empty() && is_str_const(call->args[0], &lit);
                auto* bc = !call->args.empty() ? as<Constant>(call->args[0]) : nullptr;
                bool bytes_arg = bc && bc->value.kind == ConstValue::Kind::Bytes;
                if (STR_ONLY_METHODS.count(at->attr)) info.uses.push_back({DemandUse::Str});
                else if (BYTES_ONLY_METHODS.count(at->attr)) info.uses.push_back({DemandUse::Bytes});
                else if (SEQ_LITERAL_METHODS.count(at->attr) && str_arg) info.uses.push_back({DemandUse::Str});
                else if (SEQ_LITERAL_METHODS.count(at->attr) && bytes_arg) info.uses.push_back({DemandUse::Bytes});
                else info.uses.push_back({DemandUse::Method, nullptr, at->attr});
            } else {
                info.uses.push_back({DemandUse::Attr, nullptr, at->attr});
            }
            continue;
        }
        if (auto* cmp = as<Compare>(ue)) {
            std::vector<const A::Expr*> all = {cmp->left};
            all.insert(all.end(), cmp->comparators.begin(), cmp->comparators.end());
            bool eqish = true;
            for (auto op : cmp->ops)
                if (op != CmpOpKind::Eq && op != CmpOpKind::NotEq) eqish = false;
            if (!eqish) continue;
            for (auto* x : all) {
                if (x == nm) continue;
                if (is_str_const(x)) info.uses.push_back({DemandUse::Str});
                else if (auto* c = as<Constant>(x)) {
                    if (c->value.kind == ConstValue::Kind::Bytes) info.uses.push_back({DemandUse::Bytes});
                }
            }
            continue;
        }
        if (auto* b = as<BinOp>(ue)) {
            const A::Expr* other = b->left == nm ? b->right : b->left;
            if (b->op == OpKind::Add && is_str_const(other)) info.uses.push_back({DemandUse::Str});
            continue;
        }
    }
    return cache[key] = std::move(info);
}

// the classes on which `attr` resolves (its own, inherited, or set by a method), as attribute or method
static bool has_member(Program& p, const std::string& cq, const std::string& attr, bool method) {
    Fn* f = p.find_method(cq, attr);
    if (method) return f != nullptr && f->kind != "property";
    if (f) return f->kind == "property";
    return !p.attr_owner(cq, attr).empty();
}

T Program::demand_type(Fn* f, const std::string& name) {
    static thread_local int depth = 0;
    if (depth > 8) return BOTTOM();
    const DemandInfo& info = demand_info(*this, f, name);
    if (info.rebound || info.uses.empty()) return BOTTOM();
    depth++;
    T t = BOTTOM();
    std::vector<std::pair<std::string, bool>> members;
    bool bad = false;
    for (auto& u : info.uses) {
        T c;
        switch (u.kind) {
            case DemandUse::Str: c = STR(); break;
            case DemandUse::Bytes: c = BYTES(); break;
            case DemandUse::Int: c = INT(); break;
            case DemandUse::ArgTo: {
                if (u.callee == f) continue;
                T pt = param_type(u.callee, u.name);
                if (pt.kind == TK::List || pt.kind == TK::Dict || pt.kind == TK::Set || pt.kind == TK::Top || pt.kind == TK::Bottom ||
                    pt.kind == TK::None)
                    continue;
                c = pt.kind == TK::Opt ? pt.args()[0] : pt;
                break;
            }
            case DemandUse::Attr: members.emplace_back(u.name, false); continue;
            case DemandUse::Method: members.emplace_back(u.name, true); continue;
        }
        if (c.kind == TK::Int) c = INT();   // the callee's range is not the caller's contract
        if (t.kind == TK::Bottom) t = c;
        else if (t.kind == TK::Int && c.kind == TK::Int) t = INT();
        else if (!(t == c)) {
            if (t.kind == TK::Inst && c.kind == TK::Inst) {
                std::string b = common_base(t.name(), c.name());
                if (b.empty()) bad = true;
                else t = named(TK::Inst, b);
            } else
                bad = true;
        }
    }
    depth--;
    if (bad) return BOTTOM();
    if (!members.empty()) {
        if (t.kind != TK::Bottom && t.kind != TK::Inst) return BOTTOM();   // a str with attributes of a class
        std::vector<std::string> fits;
        for (auto& [cq, _] : classes) {
            bool all = true;
            for (auto& [a, meth] : members)
                if (!has_member(*this, cq, a, meth)) {
                    all = false;
                    break;
                }
            if (all) fits.push_back(cq);
        }
        // the most basic classes that fit (a subclass of a fitting class fits too)
        std::vector<std::string> roots;
        for (auto& c : fits) {
            bool under = false;
            auto m = mro(c);
            for (size_t i = 1; i < m.size(); i++)
                if (std::find(fits.begin(), fits.end(), m[i]) != fits.end()) under = true;
            if (!under) roots.push_back(c);
        }
        if (roots.size() != 1) return BOTTOM();
        if (t.kind == TK::Inst) {
            auto m = mro(t.name());
            if (std::find(m.begin(), m.end(), roots[0]) == m.end()) return BOTTOM();
            return t;
        }
        return named(TK::Inst, roots[0]);
    }
    return t;
}

}  // namespace pyengine
