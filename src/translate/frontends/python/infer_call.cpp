// translate/frontends/python/infer_call.cpp -- calls: of the program's functions and methods (arguments become the
// callee's parameter facts, the result is its return fact), constructors, base-class calls, and the hand-over of builtin
// and standard library calls to the Library.
#include <algorithm>

#include "translate/frontends/python/infer_util.hpp"
#include "translate/frontends/python/pyast.hpp"

namespace cb::translate::frontends::python::infer {

namespace {

// The library an `extern` name belongs to: the name up to its first dot.
std::string extern_library(const std::string& name) { return first_part(name); }

}  // namespace

TR Eval::e_call(const syntax::Call* n, const State& state) {
    std::optional<Type> receiver;
    if (const auto* method = syntax::as<syntax::Attribute>(n->func)) receiver = ev(method->value, state);
    std::optional<Type> callee;
    if (!receiver) callee = ev(n->func, state);
    for (const syntax::Expr* a : n->args) {
        if (const auto* starred = syntax::as<syntax::Starred>(a)) {
            ev(starred->value, state);
            return TR(issue(n, IssueClass::Dynamic, "*args call"));
        }
    }
    for (const syntax::keyword* k : n->keywords) {
        if (!k->has_arg && !is_name(n->func, "dict")) {
            ev(k->value, state);
            return TR(issue(n, IssueClass::Dynamic, "**kwargs call"));
        }
    }
    // a lambda or a generator argument is typed where the callee uses it, not here
    const auto lazy = [](const syntax::Expr* e) { return e->kind == syntax::ExprKind::Lambda || e->kind == syntax::ExprKind::GeneratorExp; };
    Args args;
    for (const syntax::Expr* a : n->args) args.push_back(!lazy(a) ? std::optional<Type>(ev(a, state)) : std::nullopt);
    Kw kw;
    for (const syntax::keyword* k : n->keywords) {
        kw.push_back(Keyword{k->has_arg ? std::optional<std::string>(k->arg) : std::nullopt,
                             !lazy(k->value) ? std::optional<Type>(ev(k->value, state)) : std::nullopt});
    }
    if (receiver) return method_call(n, *receiver, syntax::cast<syntax::Attribute>(n->func)->attr, args, kw, state);
    return call_value(n, *callee, args, kw, state);
}

TR Eval::call_value(const syntax::Call* n, const Type& callee, const Args& args, const Kw& kw, const State& state) {
    switch (callee.kind) {
        case Kind::Bottom:
        case Kind::Top: return TR(callee);
        case Kind::Fn: {
            const std::string& name = *callee.name;
            if (name.starts_with(kBuiltinPrefix)) return lib.builtin(*this, n, name.substr(kBuiltinPrefix.size()), args, kw, state);
            if (name.starts_with(kBoundPrefix)) return call_fn(n, *p.fns.at(name.substr(kBoundPrefix.size())), args, kw, true);
            if (!p.fns.contains(name)) {
                // a method of a builtin type kept as a value ("bmethod:str.split")
                const std::size_t colon = name.find(':');
                const std::string tail = colon == std::string::npos ? name : name.substr(colon + 1);
                return TR(issue(n, IssueClass::Dynamic, "a method kept as a value (" + tail + ")"));
            }
            return call_fn(n, *p.fns.at(name), args, kw);
        }
        case Kind::Cls: {
            if (callee.name->starts_with(kExcPrefix)) {
                const std::string exception = callee.name->substr(kExcPrefix.size());
                for (const std::optional<Type>& a : args) {
                    if (a && !is_one_of(*a, {Kind::Str, Kind::Bottom, Kind::Top, Kind::Int})) {
                        issue(n, IssueClass::Syntax, exception + " with a " + kind_text(*a));
                    }
                }
                if (args.size() > 1) issue(n, IssueClass::Syntax, "exception with several arguments");
                return TR(inst_type(*callee.name), Res::of(ResKind::Exc, exception));
            }
            return construct(n, *callee.name, args, kw);
        }
        case Kind::Extern:
            if (callee.name->starts_with(kStdPrefix)) return lib.std_call(*this, n, callee.name->substr(kStdPrefix.size()), args, kw, state);
            return TR(issue(n, IssueClass::Extern, extern_library(*callee.name)));
        default: break;
    }
    return TR(issue(n, IssueClass::Conflict, "calling a " + kind_text(callee)));
}

TR Eval::construct(const syntax::Call* n, const std::string& class_name, const Args& args, const Kw& kw) {
    const Class& c = *p.classes.at(class_name);
    if (c.foreign_base) return TR(issue(n, IssueClass::Extern, "class derived from " + *c.foreign_base));
    if (Fn* init = p.find_method(class_name, "__init__")) {
        bind_args(n, *init, args, kw, true);
    } else if (c.exception && args.size() <= 1 && kw.empty()) {
        if (!args.empty() && args[0] && !is_one_of(*args[0], {Kind::Str, Kind::Bottom, Kind::Top})) {
            issue(n, IssueClass::Syntax, c.name + " with a " + kind_text(*args[0]));
        }
    } else if (!args.empty() || !kw.empty()) {
        issue(n, IssueClass::Conflict, "arguments to a class with no __init__");
    }
    return TR(inst_type(class_name), Res::of(ResKind::Ctor, class_name));
}

TR Eval::call_fn(const syntax::Call* n, Fn& f, const Args& args, const Kw& kw, bool bound) {
    if (f.kind == FnKind::Property) return TR(issue(n, IssueClass::Conflict, "calling a property"));
    bind_args(n, f, args, kw, bound);
    std::vector<Fn*> targets = {&f};
    // a call through an instance may reach any override: each takes the same arguments
    if (bound && f.cls != nullptr) {
        for (const std::string& descendant : p.descendants(f.cls->qname)) {
            if (Fn* const* override_fn = p.classes.at(descendant)->methods.find(f.name)) {
                bind_args(n, **override_fn, args, kw, true);
                targets.push_back(*override_fn);
            }
        }
    }
    Type result = P::bottom();
    for (const Fn* target : targets) result = p.join(result, p.ret(target->qname));
    if (result.kind == Kind::Top) result = P::top("calls-refused:" + f.name);   // its result has no type: that is the callee's refusal
    return TR(result, Res::of(bound ? ResKind::Method : ResKind::Fn, f.qname));
}

void Eval::bind_args(const syntax::Call* n, Fn& f, const Args& args, const Kw& kw, bool bound) {
    const bool takes_self = f.kind == FnKind::Method || f.kind == FnKind::Property || f.kind == FnKind::ClassMethod;
    std::vector<std::string> names = (bound || !takes_self) ? f.bound() : f.params;
    if ((f.kind == FnKind::Method || f.kind == FnKind::Property) && !bound) names = f.params;
    if (f.varargs || f.kwargs) return;
    if (args.size() > names.size()) {
        issue(n, IssueClass::Conflict, "too many arguments to " + f.name);
        return;
    }
    std::set<std::string> given;
    for (std::size_t i = 0; i < args.size(); ++i) {
        given.insert(names[i]);
        if (args[i]) {
            contribute_param(f.qname, names[i], arg_fit(n, f, names[i], *args[i]));
        } else {
            issue(n, IssueClass::Dynamic, "lambda or generator passed to " + f.name);
        }
    }
    for (const Keyword& k : kw) {
        // the key nullopt (a ** spread) is in neither list
        const bool known = k.name && (std::ranges::find(names, *k.name) != names.end() || std::ranges::find(f.kwonly, *k.name) != f.kwonly.end());
        if (!known) {
            issue(n, IssueClass::Conflict, "no parameter " + k.name.value_or("None") + " of " + f.name);
            continue;
        }
        given.insert(*k.name);
        if (k.type) contribute_param(f.qname, *k.name, arg_fit(n, f, *k.name, *k.type));
    }
    std::vector<std::string> all = names;
    all.insert(all.end(), f.kwonly.begin(), f.kwonly.end());
    for (const std::string& name : all) {
        if (!given.contains(name) && !f.defaults.contains(name)) issue(n, IssueClass::Conflict, "missing argument " + name + " to " + f.name);
    }
}

TR Eval::method_call(const syntax::Call* n, const Type& recv, const std::string& attr, const Args& args, const Kw& kw, const State& state) {
    switch (recv.kind) {
        case Kind::Bottom:
        case Kind::Top: return TR(recv);
        case Kind::Inst: {
            if (recv.name->starts_with(kExcPrefix)) return TR(issue(n, IssueClass::Dynamic, "method of an exception"));
            Fn* f = p.find_method(*recv.name, attr);
            if (f == nullptr) {
                if (p.attr_type(*recv.name, attr)) return TR(issue(n, IssueClass::Dynamic, "calling an attribute (" + attr + ")"));
                return TR(issue(n, IssueClass::Unresolved, last_part(*recv.name) + "." + attr + "()"));
            }
            return call_fn(n, *f, args, kw, f->kind != FnKind::Static);
        }
        case Kind::Module: {
            if (recv.name->starts_with(kStdPrefix)) {
                return lib.std_call(*this, n, recv.name->substr(kStdPrefix.size()) + "." + attr, args, kw, state);
            }
            Module& module = *p.modules.at(*recv.name);
            const std::optional<Ref> r = p.resolve_global(module, attr);
            if (!r) return TR(issue(n, IssueClass::Unresolved, *recv.name + "." + attr));
            const Type callee = binding_type(syntax::cast<syntax::Attribute>(n->func), Binding{Binding::Source::Program, *r, {}});
            return call_value(n, callee, args, kw, state);
        }
        case Kind::Cls: {
            if (recv.name->starts_with(kExcPrefix)) return TR(issue(n, IssueClass::Dynamic, "exception class method"));
            Fn* f = p.find_method(*recv.name, attr);
            if (f != nullptr && f->kind == FnKind::Static) return call_fn(n, *f, args, kw);
            if (f != nullptr && f->kind == FnKind::Method && !args.empty() && args[0] && args[0]->kind == Kind::Inst) {
                // Base.method(self, ...): the base's own method, not a virtual call
                const std::vector<std::string> ancestors = p.mro(*args[0]->name);
                const auto* self_arg = !n->args.empty() ? syntax::as<syntax::Name>(n->args[0]) : nullptr;
                if (std::ranges::find(ancestors, *recv.name) != ancestors.end() && self_arg != nullptr && fn != nullptr &&
                    !fn->params.empty() && self_arg->id == fn->params[0]) {
                    if (attr == "__init__" && !(fn->name == "__init__" && first_stmt(n))) {
                        return TR(issue(n, IssueClass::Syntax, "base __init__ called other than first in __init__"));
                    }
                    const Args rest(args.begin() + 1, args.end());
                    bind_args(n, *f, rest, kw, true);
                    return TR(p.ret(f->qname), Res::of(ResKind::BaseCall, f->qname));
                }
            }
            return TR(issue(n, IssueClass::Dynamic, last_part(*recv.name) + "." + attr + " through the class"));
        }
        case Kind::Extern: {
            if (recv.name->starts_with(kStdPrefix)) {
                return lib.std_call(*this, n, recv.name->substr(kStdPrefix.size()) + "." + attr, args, kw, state);
            }
            if (*recv.name == "file") return file_method(n, attr, args, kw);
            if (*recv.name == "super") return super_call(n, attr, args, kw);
            return TR(issue(n, IssueClass::Extern, extern_library(*recv.name)));
        }
        case Kind::Opt: return TR(issue(n, IssueClass::Conflict, "." + attr + "() of a value that may be None"));
        default: break;
    }
    return lib.method(*this, n, recv, attr, args, kw, state);
}

bool Eval::first_stmt(const syntax::Call* call) {
    // the function's first statement that is not a docstring or a bare constant
    for (const syntax::Stmt* s : fn->node->body) {
        const auto* e = syntax::as<syntax::ExprStmt>(s);
        if (e != nullptr && syntax::as<syntax::Constant>(e->value) != nullptr) continue;
        return e != nullptr && e->value == call;
    }
    return false;
}

TR Eval::super_call(const syntax::Call* n, const std::string& attr, const Args& args, const Kw& kw) {
    if (cls == nullptr || cls->bases.empty()) return TR(issue(n, IssueClass::Dynamic, "super() without a base of the program"));
    Fn* f = p.find_method(cls->bases[0], attr);
    if (f == nullptr) return TR(issue(n, IssueClass::Unresolved, "super()." + attr));
    bind_args(n, *f, args, kw, true);
    return TR(p.ret(f->qname), Res::of(ResKind::Super, f->qname));
}

TR Eval::file_method(const syntax::Call* n, const std::string& attr, const Args& args, const Kw& kw) {
    (void)kw;
    if (attr == "read" && args.empty()) return TR(P::bytes(), Res::of(ResKind::BMethod, "file.read"));
    return TR(issue(n, IssueClass::Stdlib, "file." + attr));
}

// The type a key function (a lambda) returns for these parameter types.
Type Eval::lambda_type(const syntax::Expr* lam, const std::vector<Type>& params, const State& state) {
    const auto* l = syntax::as<syntax::Lambda>(lam);
    if (l == nullptr) return ev(lam, state);
    const syntax::arguments* a = l->args;
    if (a->vararg != nullptr || a->kwarg != nullptr || !a->kwonlyargs.empty() || a->args.size() != params.size()) {
        return issue(lam, IssueClass::Dynamic, "lambda signature");
    }
    State scope = state;
    for (std::size_t i = 0; i < params.size(); ++i) {
        scope.set(a->args[i]->arg, params[i]);
        note(a->args[i], params[i]);
    }
    const Type t = ev(l->body, scope);
    note(lam, t);
    return t;
}

std::pair<Type, State> Eval::gen_elem(const syntax::GeneratorExp* g, const State& state) {
    State scope = comp_scope(g->generators, state);
    Type element = ev(g->elt, scope);
    return {element, scope};
}

}  // namespace cb::translate::frontends::python::infer
