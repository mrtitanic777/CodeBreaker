// cb/translate/frontends/python/infer_eval.cpp -- the interpreter's bookkeeping: the flow state, what a node was, what a
// function contributes to the program's facts, and one function's run from its parameters to its return.
#include <algorithm>

#include "cb/core/error.hpp"
#include "cb/translate/frontends/python/infer_util.hpp"
#include "cb/translate/frontends/python/pyast.hpp"

namespace cb::translate::frontends::python::infer {

// ---- reports -----------------------------------------------------------------------------------------------

std::string_view issue_class_word(IssueClass cls) {
    switch (cls) {
        case IssueClass::Untyped: return "untyped";
        case IssueClass::Conflict: return "conflict";
        case IssueClass::IntRange: return "int-range";
        case IssueClass::Dynamic: return "dynamic";
        case IssueClass::Stdlib: return "stdlib";
        case IssueClass::Extern: return "extern";
        case IssueClass::Syntax: return "syntax";
        case IssueClass::Unresolved: return "unresolved";
    }
    return "";
}

std::string Issue::reason() const {
    std::string word(issue_class_word(cls));
    return detail.empty() ? word : word + ":" + detail;
}

Res Binding::resolution() const {
    if (source == Source::Builtin) return Res::of(ResKind::Builtin, name);
    if (source == Source::Exception) return Res::of(ResKind::Exc, name);
    if (source == Source::Constant) return Res::of(ResKind::Const, name);
    switch (ref.kind) {
        case Ref::Kind::Function: return Res::of(ResKind::Fn, ref.name);
        case Ref::Kind::Class: return Res::of(ResKind::Class, ref.name);
        case Ref::Kind::Global: return Res::of(ResKind::Global, ref.name, ref.member);
        case Ref::Kind::Module: return Res::of(ResKind::Module, ref.name);
        case Ref::Kind::StdModule: return Res::of(ResKind::StdModule, ref.name);
        case Ref::Kind::StdObject: return Res::of(ResKind::StdObj, ref.name);
        case Ref::Kind::Missing: return Res::of(ResKind::Missing, ref.name, ref.member);
        case Ref::Kind::Extern:
        case Ref::Kind::Member: return Res::of(ResKind::Extern, ref.name, ref.member);
    }
    return Res::of(ResKind::Extern, ref.name);
}

bool has_top(const Type& t, int depth) {
    if (t.kind == Kind::Top) return true;
    if (depth >= 6) return false;
    return std::ranges::any_of(t.args, [depth](const Type& a) { return has_top(a, depth + 1); });
}

bool is_one_of(const Type& t, std::initializer_list<Kind> kinds) { return std::ranges::find(kinds, t.kind) != kinds.end(); }

const std::set<std::string>& builtin_functions() {
    static const std::set<std::string> names = {
        "len", "range", "enumerate", "zip", "isinstance", "int", "str", "bytes", "bytearray", "bool", "float", "min",
        "max", "abs", "sorted", "reversed", "sum", "any", "all", "list", "dict", "set", "tuple", "print", "open",
        "ord", "chr", "hex", "oct", "bin", "repr", "divmod", "round", "pow", "format", "super", "iter", "next", "map",
        "filter", "getattr", "setattr", "hasattr", "type", "id", "hash", "vars", "globals", "locals", "callable",
        "issubclass", "object", "frozenset", "delattr", "exec", "eval", "compile", "input", "memoryview",
        "staticmethod", "classmethod", "property"};
    return names;
}

const std::set<std::string>& dynamic_builtins() {
    static const std::set<std::string> names = {"getattr", "setattr", "hasattr", "delattr", "type", "id", "hash",
                                                "vars", "globals", "locals", "callable", "issubclass", "exec",
                                                "eval", "compile", "iter", "next", "memoryview", "input"};
    return names;
}

const Keyword* find_keyword(const Kw& kw, std::string_view name) {
    for (const Keyword& k : kw) {
        if (k.name && *k.name == name) return &k;
    }
    return nullptr;
}

std::optional<Wide> int_constant(const syntax::Expr* e) {
    if (const auto* c = syntax::as<syntax::Constant>(e)) {
        if (c->value.kind == syntax::ConstValue::Kind::Int) return Wide::from_decimal(c->value.text);
        return std::nullopt;
    }
    if (const auto* u = syntax::as<syntax::UnaryOp>(e)) {
        if (u->op == syntax::UnaryOpKind::USub) {
            if (std::optional<Wide> v = int_constant(u->operand)) return -*v;
        }
    }
    return std::nullopt;
}

std::vector<Type> tuple_slice(const std::vector<Type>& items, const std::optional<Wide>& from, const std::optional<Wide>& to) {
    const auto n = static_cast<std::int64_t>(items.size());
    const auto clamp = [n](const std::optional<Wide>& bound, std::int64_t fallback) -> std::int64_t {
        if (!bound) return fallback;
        if (bound->sign() < 0) {
            const Wide from_end = *bound + Wide(n);
            return from_end.sign() < 0 ? 0 : from_end.to_i64().value_or(0);
        }
        return *bound > Wide(n) ? n : bound->to_i64().value_or(n);
    };
    const std::int64_t lo = clamp(from, 0);
    const std::int64_t hi = clamp(to, n);
    std::vector<Type> out;
    for (std::int64_t i = lo; i < hi; ++i) out.push_back(items[static_cast<std::size_t>(i)]);
    return out;
}

// ---- State ---------------------------------------------------------------------------------------------------

const Type* State::find(const std::string& name) const {
    for (const auto& [key, type] : items_) {
        if (key == name) return &type;
    }
    return nullptr;
}

const Type& State::at(const std::string& name) const {
    const Type* t = find(name);
    CB_ASSERT(t != nullptr, "a local the flow does not bind: " + name);
    return *t;
}

void State::set(const std::string& name, const Type& type) {
    for (auto& [key, value] : items_) {
        if (key == name) {
            value = type;
            return;
        }
    }
    items_.emplace_back(name, type);
}

void State::pop(const std::string& name) {
    const auto it = std::ranges::find_if(items_, [&](const auto& kv) { return kv.first == name; });
    if (it != items_.end()) items_.erase(it);
}

bool State::same_keys(const State& other) const {
    if (items_.size() != other.items_.size()) return false;
    return std::ranges::all_of(items_, [&](const auto& kv) { return other.find(kv.first) != nullptr; });
}

bool Eval::MemoKey::operator==(const MemoKey& other) const { return st == other.st && items == other.items; }

std::size_t Eval::MemoKeyHash::operator()(const MemoKey& key) const {
    std::size_t h = std::hash<const void*>()(key.st);
    for (const auto& [name, type] : key.items) {
        h = h * 1000003u ^ std::hash<std::string>()(name);
        h = h * 1000003u ^ type.hash();
    }
    return h;
}

// ---- Eval ----------------------------------------------------------------------------------------------------

Eval::Eval(Program& program, Fn* function, bool record_types, const Library& library)
    : p(program),
      fn(function),
      m(function != nullptr ? function->module : nullptr),
      record(record_types),
      lib(library),
      ret(P::bottom()),
      yielded(P::bottom()),
      cls(function != nullptr ? function->cls : nullptr) {}

Type Eval::note(const syntax::Node* node, const Type& t, const std::optional<Res>& resolution) {
    const void* key = node;
    const auto joined = [this, &t](std::unordered_map<const void*, Type>& table, const void* k) {
        const auto it = table.find(k);
        table.insert_or_assign(k, it == table.end() ? t : p.join(it->second, t));
    };
    joined(seen, key);
    if (record && quiet == 0) {
        joined(types, key);
        if (resolution) res[key] = *resolution;
    }
    return t;
}

Type Eval::issue(const syntax::Node* node, IssueClass issue_class, const std::string& detail) {
    if (record && quiet == 0) {
        issues.push_back(Issue{issue_class, detail, node != nullptr ? static_cast<std::uint32_t>(node->pos.lineno) : 0});
    }
    return P::top(std::string(issue_class_word(issue_class)) + ":" + detail);
}

void Eval::contribute_param(const std::string& fn_name, const std::string& param, const Type& t) {
    // a value with no single type is the caller's refusal, not the callee's: it is not passed on (but remembered: a
    // parameter only such callers reach is untyped because of them)
    const program::NameKey key{fn_name, param};
    if (live() && t.kind == Kind::Top && !p.param_poison.contains(key)) {
        p.param_poison[key] = program::Poison{fn != nullptr ? fn->qname : "?", t.why.value_or("")};
    }
    if (!live() || is_one_of(t, {Kind::Bottom, Kind::Top}) || (is_one_of(t, {Kind::Tuple, Kind::Opt}) && has_top(t))) return;
    const Fn& f = *p.fns.at(fn_name);
    OrderedMap<std::string, Type>& facts = p.params[fn_name];
    const Type old_type = get_or(facts, param, P::bottom());
    Type new_type;
    if (f.annotations.contains(param)) {
        const Type annotated = p.annotation_type(f, param);
        if (annotated.kind == Kind::Int && t.kind == Kind::Int) {   // the annotation's kind, the call sites' range
            new_type = old_type.kind != Kind::Bottom ? p.join(old_type, t) : t;
        } else {
            new_type = annotated;
        }
    } else {
        new_type = p.join(old_type, t);
    }
    if (p.round > 3 && new_type.kind == Kind::Int && old_type.kind == Kind::Int) new_type = P::widen_int(old_type, new_type);
    if (new_type != old_type) {
        facts.insert_or_assign(param, new_type);
        p.touch(FactKey::params(fn_name));
    }
}

Type Eval::arg_fit(const syntax::Node* node, const Fn& f, const std::string& param, const Type& t0) {
    const Type t = fit(node, t0, "argument " + param);
    const bool aware = f.none_aware.contains(param);
    if (t.kind == Kind::Opt && !aware) {
        issue(node, IssueClass::Conflict, "passes a value that may be None as " + param + " of " + f.name);
        return t.args[0];
    }
    if (t.kind == Kind::None && !aware) {
        issue(node, IssueClass::Conflict, "passes None as " + param + " of " + f.name);
        return P::bottom();
    }
    return t;
}

Type Eval::fit(const syntax::Node* node, const Type& t, const std::string& what) {
    if (t.kind == Kind::Int && !t.fits_i64()) {
        if (p.int_policy == program::IntPolicy::Strict) {
            issue(node, IssueClass::IntRange, what + " in [" + bound_text(t.lo) + ", " + bound_text(t.hi) + "]");
        }
        return P::meet(t, P::i64_min(), P::i64_max());
    }
    if (is_one_of(t, {Kind::Tuple, Kind::Opt}) &&
        std::ranges::any_of(t.args, [](const Type& a) { return a.kind == Kind::Int && !a.fits_i64(); })) {
        Type fitted{.kind = t.kind};
        for (const Type& a : t.args) fitted.args.push_back(fit(node, a, what));
        return fitted;
    }
    return t;
}

void Eval::contribute_attr(const std::string& class_name, const std::string& attr, const Type& t) {
    const bool partial = t.kind == Kind::Top || (is_one_of(t, {Kind::Tuple, Kind::Opt}) && has_top(t));
    if (live() && partial) {
        const std::string owner = p.attr_owner(class_name, attr).value_or(class_name);
        const program::NameKey key{owner, attr};
        if (!p.poisoned.contains(key)) {
            p.poisoned[key] = fn != nullptr ? fn->qname : "?";
            p.touch(FactKey::attrs(owner));
        }
        return;
    }
    if (!live() || is_one_of(t, {Kind::Bottom, Kind::Top}) || partial) return;
    const std::string owner = p.attr_owner(class_name, attr).value_or(class_name);
    OrderedMap<std::string, Type>& facts = p.attrs[owner];
    const Type old_type = get_or(facts, attr, P::bottom());
    Type new_type = p.join(old_type, t);
    if (p.round > 3 && new_type.kind == Kind::Int && old_type.kind == Kind::Int) new_type = P::widen_int(old_type, new_type);
    if (new_type != old_type) {
        facts.insert_or_assign(attr, new_type);
        p.touch(FactKey::attrs(owner));
    }
}

// ---- the function --------------------------------------------------------------------------------------------

Eval& Eval::run() {
    Fn& f = *fn;
    const syntax::FunctionDef* node = f.node;
    locals = p.fn_locals(f);
    State state;
    if (f.varargs || f.kwargs) issue(node, IssueClass::Dynamic, f.varargs ? "*args" : "**kwargs");
    if (f.kind == FnKind::Async) issue(node, IssueClass::Dynamic, "async");
    for (const std::string& d : f.decorators) {
        if (d != "staticmethod" && d != "property") issue(node, IssueClass::Dynamic, "decorator " + d);
    }
    std::vector<std::string> all = f.params;
    all.insert(all.end(), f.kwonly.begin(), f.kwonly.end());
    for (std::size_t i = 0; i < all.size(); ++i) {
        Type t;
        if (i == 0 && (f.kind == FnKind::Method || f.kind == FnKind::Property)) {
            t = inst_type(f.cls->qname);
        } else if (i == 0 && f.kind == FnKind::ClassMethod) {
            t = named_type(Kind::Cls, f.cls->qname);
        } else {
            t = p.param_type(f, all[i]);
        }
        state.set(all[i], t);
    }
    for (const pyast::AnyNode& an : pyast::walk(pyast::AnyNode(static_cast<const syntax::Stmt*>(node)))) {
        const syntax::Expr* e = an.expr();
        if (e != nullptr && e->kind == syntax::ExprKind::YieldFrom) {
            issue(e, IssueClass::Dynamic, "yield from");
            break;
        }
        if (e != nullptr && e->kind == syntax::ExprKind::Yield) is_generator = true;
    }
    const Flow out = block(node->body, state);
    if (out && returns_value) ret = p.join(ret, P::none());
    if (!returns_value) ret = P::none();
    // a generator function's result is the sequence of what it yields (its `return` value, which StopIteration carries, is not read)
    if (is_generator) ret = P::iter_of(yielded);   // (while the callers have not told the parameters yet the item is still bottom, which joins with whatever comes)
    if (ret.kind == Kind::Int && !ret.fits_i64()) ret = fit(node, ret, "return value");
    if (live()) {
        const Type old_type = get_or(p.returns, f.qname, P::bottom());
        Type new_type = p.join(old_type, ret);
        if (p.round > 3 && new_type.kind == Kind::Int && old_type.kind == Kind::Int) new_type = P::widen_int(old_type, new_type);
        if (new_type != old_type) {
            p.returns[f.qname] = new_type;
            p.touch(FactKey::returns(f.qname));
        }
    }
    return *this;
}

}  // namespace cb::translate::frontends::python::infer
