// codebreaker/translate/frontends/clang/j2x.cpp -- typed C++ port of
// codebreaker.translate.frontends.clang.j2x (see j2x.hpp).

#include "codebreaker/translate/frontends/clang/j2x.hpp"

#include <algorithm>
#include <vector>

namespace codebreaker::translate::frontends::clang::j2x {

using ir::NodeP;
using ir::Nodes;
using ir::SymP;
using ir::TypeP;
using ir::Val;

namespace {

const py::list<py::str> STRINGS_ = {py::str("JavaString"), py::str("JCat"), py::str("std.string"), py::str("std.basic_string"),
                                    py::str("StringBuilder")};

// _MATH: the runtime's helper -> Java's Math method
const py::dict<py::str, py::str> MATH_NAMES = {{py::str("javaMathSqrt"), py::str("sqrt")},
                                               {py::str("javaMathRound"), py::str("round")},
                                               {py::str("javaMathSignum"), py::str("signum")},
                                               {py::str("javaMathRandom"), py::str("random")}};

[[noreturn]] void none_attr(const char* attr) {
    throw py::AttributeError(std::string("'NoneType' object has no attribute '") + attr + "'");
}
[[noreturn]] void not_iterable() { throw py::TypeError("'NoneType' object is not iterable"); }

template <class T> py::ref<T> as(const NodeP& n) { return std::dynamic_pointer_cast<T>(n); }
template <class T> bool is_a(const NodeP& n) { return dynamic_cast<const T*>(n.get()) != nullptr; }

TypeP mk(const char* kind, const std::optional<py::str>& name = std::nullopt, const py::list<TypeP>& args = py::list<TypeP>(),
         const TypeP& elem = nullptr, const ir::Dict& info = ir::Dict()) {
    return std::make_shared<ir::Type>(py::str(kind), name, args, elem, std::nullopt, info);
}

// `x.type` of a node (AttributeError for None, and for a node that has no type slot)
TypeP type_of(const NodeP& n) {
    if (!n) none_attr("type");
    auto e = as<ir::Expr>(n);
    if (!e) throw py::AttributeError("'" + n->class_name().utf8() + "' object has no attribute 'type'");
    return e->type;
}

// `t is not None and t.kind == kind`
bool kind_is(const TypeP& t, const char* kind) { return t && t->kind == py::str(kind); }

bool truthy(const Val& v) {
    return std::visit(
        [](const auto& x) -> bool {
            using T = std::decay_t<decltype(x)>;
            if constexpr (std::is_same_v<T, std::monostate>) return false;
            else if constexpr (std::is_same_v<T, TypeP>) return true;
            else return py::truthy(x);
        },
        v.v);
}

// `t.info.get('ref')` is truthy
bool is_ref(const TypeP& t) {
    auto r = t->info.get(py::str("ref"));
    return r && truthy(*r);
}

bool in_strings(const std::optional<py::str>& name) { return name && STRINGS_.contains(*name); }

bool has_mod(const ir::Strs& mods, const char* m) { return mods && mods->contains(py::str(m)); }

// the slots of a node, by class (calls f(name, member, rank))
template <class T, class F> bool each_as(ir::Node& n, F& f) {
    if (T* p = dynamic_cast<T*>(&n)) {
        p->each(f);
        return true;
    }
    return false;
}
template <class... T, class F> bool each_class(ir::Node& n, F& f) { return (each_as<T>(n, f) || ...); }
template <class F> void each_slot_of(ir::Node& n, F&& f) {
    each_class<ir::Module, ir::Unit, ir::ClassDecl, ir::FieldDecl, ir::MethodDecl, ir::Param, ir::EnumConst,
               ir::Property, ir::TypeParam, ir::Block, ir::Local, ir::ExprStmt, ir::If, ir::While, ir::DoWhile,
               ir::For, ir::ForEach, ir::Switch, ir::Case, ir::Break, ir::Continue, ir::Return, ir::Throw, ir::Try,
               ir::Catch, ir::Labeled, ir::Goto, ir::MachineFragment, ir::Sync, ir::Empty, ir::Comment,
               ir::LocalClass, ir::Assert, ir::CtorCall, ir::Yield, ir::Lit, ir::Name, ir::Select, ir::TypeName,
               ir::This, ir::Super, ir::Call, ir::New, ir::NewArray, ir::Index, ir::Length, ir::Unary, ir::Binary,
               ir::Concat, ir::Assign, ir::Cond, ir::Cast, ir::InstanceOf, ir::ClassLit, ir::Lambda, ir::MethodRef,
               ir::SwitchExpr, ir::AddressOf, ir::FarAddress, ir::Piece, ir::Join, ir::Paren, ir::Unsupported>(n, f);
}

// a new node of class T at the position of e
template <class T> py::ref<T> at(const NodeP& e) {
    auto n = std::make_shared<T>();
    n->line = e->line;
    n->col = e->col;
    return n;
}

// ---- _first_comma
using Found = std::optional<std::tuple<NodeP, Setter>>;

void visit(const NodeP& node, const Setter& setter, Found& found);

// One field's work, run when its turn comes (the slot is read then).
struct Step {
    int rank;
    std::function<void()> run;
};

void add_step(std::vector<Step>& out, int rank, NodeP& m, Found& found) {
    out.push_back({rank, [&m, &found] {
                       if (m) visit(m, [&m](const NodeP& x) { m = x; }, found);
                   }});
}
void visit_list(const py::list<NodeP>& v, Found& found) {
    for (int64_t k = 0; k < py::len(v); ++k) {
        NodeP x = v[k];
        if (x) visit(x, [v, k](const NodeP& y) { v[k] = y; }, found);
    }
}
void add_step(std::vector<Step>& out, int rank, Nodes& m, Found& found) {
    out.push_back({rank, [&m, &found] {
                       if (m) visit_list(*m, found);
                   }});
}
void add_step(std::vector<Step>& out, int rank, std::variant<NodeP, py::list<NodeP>>& m, Found& found) {
    out.push_back({rank, [&m, &found] {
                       if (std::holds_alternative<NodeP>(m)) {
                           NodeP v = std::get<NodeP>(m);
                           if (v) visit(v, [&m](const NodeP& x) { m = x; }, found);
                       } else {
                           visit_list(std::get<py::list<NodeP>>(m), found);
                       }
                   }});
}
template <class T> void add_step(std::vector<Step>&, int, T&, Found&) {}

void visit(const NodeP& node, const Setter& setter, Found& found) {
    if (found) return;
    if (auto p = as<ir::Paren>(node)) {
        visit(p->x, setter, found);
        return;
    }
    if (auto b = as<ir::Binary>(node)) {
        if (b->op && *b->op == py::str(",")) {
            found = std::make_tuple(node, setter);
            return;
        }
    }
    if (!node) return;
    std::vector<Step> steps;
    each_slot_of(*node, [&](const char*, auto& member, auto rank) {
        if (static_cast<int>(rank) != 0) add_step(steps, static_cast<int>(rank), member, found);
    });
    std::stable_sort(steps.begin(), steps.end(), [](const Step& a, const Step& b) { return a.rank < b.rank; });
    for (const Step& s : steps) s.run();
}

}  // namespace

const TypeP STRING = mk("class", py::str("java.lang.String"));
const TypeP SYSTEM = mk("class", py::str("java.lang.System"));
const TypeP PRINTSTREAM = mk("class", py::str("java.io.PrintStream"));
const TypeP MATH = mk("class", py::str("java.lang.Math"));

TypeP lift_type(const TypeP& t) {
    if (!t) return t;
    const py::str& k = t->kind;
    if (k == py::str("class")) {
        if (in_strings(t->name)) return STRING;
        if (t->name && *t->name == py::str("JArray") && py::truthy(t->args)) {
            return mk("array", std::nullopt, py::list<TypeP>(), lift_type(t->args[0]));
        }
        if (t->name && *t->name == py::str("System._PrintStream")) return PRINTSTREAM;
        py::list<TypeP> args;
        for (const TypeP& a : t->args) args.append(lift_type(a));
        return mk("class", t->name, args);
    }
    if (k == py::str("ptr")) {
        TypeP e = lift_type(t->elem);
        if (kind_is(e, "array") || ir::same(e, STRING)) return e;   // (a pointer to an array or String: the reference)
        return mk("ptr", std::nullopt, py::list<TypeP>(), e, t->info.copy());
    }
    if (k == py::str("array")) {
        if (kind_is(t->elem, "char")) return STRING;   // a string literal's char[N]
        return mk("array", std::nullopt, py::list<TypeP>(), lift_type(t->elem), t->info.copy());
    }
    return t;
}

NodeP strip_(const NodeP& e_) {
    NodeP e = e_;
    while (auto p = as<ir::Paren>(e)) e = p->x;
    return e;
}

NodeP value_(const NodeP& a) {
    NodeP x = strip_(a);
    auto u = as<ir::Unary>(x);
    if (u && u->op && *u->op == py::str("&") && u->type && is_ref(u->type)) return u->x;
    return a;
}

py::list<NodeP> commas_(const NodeP& e_) {
    NodeP e = strip_(e_);
    auto b = as<ir::Binary>(e);
    if (b && b->op && *b->op == py::str(",")) return commas_(b->left) + commas_(b->right);
    return py::list<NodeP>{e};
}

std::optional<std::tuple<NodeP, Setter>> first_comma_(const NodeP& stmt) {
    Found found;
    visit(stmt, Setter(), found);
    return found;
}

// ------------------------------------------------------------------------------------------ Lift
Lift::Lift() {}

void Lift::names(const py::ref<ir::Unit>& u) {
    if (!u) none_attr("types");
    if (!u->types) not_iterable();
    std::vector<py::ref<ir::ClassDecl>> cs;
    for (const NodeP& t : *u->types) {
        if (auto c = as<ir::ClassDecl>(t)) cs.push_back(c);
    }
    for (const auto& c : cs) {
        if (!c->members) not_iterable();
        py::set<py::str> names;
        for (const NodeP& m : *c->members) {
            auto f = as<ir::FieldDecl>(m);
            if (f && f->sym) names.add(f->sym->name);
        }
        for (const NodeP& m : *c->members) {
            auto f = as<ir::FieldDecl>(m);
            if (f && f->sym && has_mod(f->mods, "static") && f->sym->name.startswith(py::str("s_")) &&
                !names.contains(f->sym->name.slice_from(2))) {
                renamed[std::make_tuple(c->qname, f->sym->name)] = f->sym->name.slice_from(2);
            }
        }
    }
}

py::ref<ir::Unit> Lift::unit(const py::ref<ir::Unit>& u) {
    if (!u) none_attr("types");
    if (!u->types) not_iterable();
    py::list<NodeP> lifted;
    for (const NodeP& t : *u->types) lifted.append(node(t));
    u->types = lifted;
    std::vector<py::ref<ir::ClassDecl>> cs;
    for (const NodeP& t : *u->types) {
        if (auto c = as<ir::ClassDecl>(t)) cs.push_back(c);
    }
    for (const auto& c : cs) {
        py::list<NodeP> members;
        if (!c->members) not_iterable();
        for (const NodeP& m : *c->members) {
            auto md = as<ir::MethodDecl>(m);
            if (md && md->kind && *md->kind == py::str("dtor")) {
                bool empty = !ir::truthy(md->body);
                if (!empty) {
                    auto b = as<ir::Block>(ir::node_of(md->body));
                    if (!b) throw py::AttributeError(std::holds_alternative<ir::NodeP>(md->body)
                                                         ? "'" + std::get<ir::NodeP>(md->body)->class_name().utf8() + "' object has no attribute 'stmts'"
                                                         : std::string("'list' object has no attribute 'stmts'"));
                    empty = !b->stmts || py::len(*b->stmts) == 0;
                }
                if (empty) continue;   // (Java has no destructor; an empty one says nothing)
            }
            if (md && md->name && *md->name == py::str("initStatics") && has_mod(md->mods, "static")) md->kind = py::str("static_init");
            members.append(m);
        }
        c->members = members;
    }
    return u;
}

SymP Lift::sym(const SymP& s) {
    if (!s) return nullptr;
    py::str name = s->name;
    auto it = renamed.find(std::make_tuple(s->owner, s->name));
    if (it != renamed.end()) name = it->second;
    py::list<TypeP> params;
    for (const TypeP& p : s->params) params.append(lift_type(p));
    return std::make_shared<ir::Sym>(s->kind, name, s->owner, s->static_, lift_type(s->type), params, lift_type(s->ret), s->varargs);
}

py::list<NodeP> Lift::node(const py::list<NodeP>& n) {
    py::list<NodeP> out;
    for (const NodeP& x : n) out.append(node(x));
    return out;
}

NodeP Lift::node(const NodeP& n) {
    if (!n) return n;
    if (auto b = as<ir::Block>(n)) {
        if (!b->stmts) not_iterable();
        py::list<NodeP> lifted;
        for (const NodeP& s : *b->stmts) lifted.append(node(s));
        b->stmts = fold_temps(lifted);
        return n;
    }
    each_slot_of(*n, [&](const char*, auto& m, auto) {
        using M = std::decay_t<decltype(m)>;
        if constexpr (std::is_same_v<M, NodeP>) {
            if (m) m = node(m);
        } else if constexpr (std::is_same_v<M, Nodes>) {
            if (m) m = node(*m);
        } else if constexpr (std::is_same_v<M, std::variant<NodeP, py::list<NodeP>>>) {
            if (std::holds_alternative<NodeP>(m)) {
                if (std::get<NodeP>(m)) m = node(std::get<NodeP>(m));
            } else {
                m = node(std::get<py::list<NodeP>>(m));
            }
        } else if constexpr (std::is_same_v<M, TypeP>) {
            if (m) m = lift_type(m);
        } else if constexpr (std::is_same_v<M, SymP>) {
            if (m) m = sym(m);
        } else if constexpr (std::is_same_v<M, Val>) {
            if (const TypeP* t = std::get_if<TypeP>(&m.v)) {
                if (*t) m = Val(lift_type(*t));
            }
        }
    });
    if (auto p = as<ir::Lit>(n)) return x_Lit(p);
    if (auto p = as<ir::Cast>(n)) return x_Cast(p);
    if (auto p = as<ir::Select>(n)) return x_Select(p);
    if (auto p = as<ir::Name>(n)) return x_Name(p);
    if (auto p = as<ir::Unary>(n)) return x_Unary(p);
    if (auto p = as<ir::New>(n)) return x_New(p);
    if (auto p = as<ir::Call>(n)) return x_Call(p);
    if (auto p = as<ir::Binary>(n)) return x_Binary(p);
    return n;
}

// ---- expressions (children already lifted)
NodeP Lift::x_Lit(const py::ref<ir::Lit>& e) {
    if (std::holds_alternative<py::str>(e->value.v)) e->type = STRING;
    return e;
}

NodeP Lift::x_Cast(const py::ref<ir::Cast>& e) {
    NodeP x = strip_(e->x);
    bool lit_str = false;
    if (auto l = as<ir::Lit>(x)) lit_str = std::holds_alternative<py::str>(l->value.v);
    if (ir::same(e->type, STRING) || lit_str || (ir::same(type_of(x), STRING) && kind_is(e->type, "ptr"))) {
        return x;   // (a string's decay or conversion: the string)
    }
    if (kind_is(e->type, "array") && type_of(x) && kind_is(type_of(x), "array")) return x;
    return e;
}

NodeP Lift::x_Select(const py::ref<ir::Select>& e) {
    NodeP obj = strip_(e->obj);
    if (e->sym && e->sym->name == py::str("_s") && (ir::same(type_of(obj), STRING) || is_a<ir::Concat>(obj))) return obj;
    if (e->sym && e->sym->name == py::str("length") && type_of(obj) && kind_is(type_of(obj), "array")) {
        auto n = at<ir::Length>(e);
        n->arr = obj;
        n->type = ir::I32;
        return n;
    }
    auto u = as<ir::Unary>(obj);
    if (u && u->op && *u->op == py::str("*") && type_of(u->x) && kind_is(type_of(u->x), "array") && e->sym &&
        e->sym->name == py::str("length")) {
        auto n = at<ir::Length>(e);
        n->arr = u->x;
        n->type = ir::I32;
        return n;
    }
    return e;
}

NodeP Lift::x_Name(const py::ref<ir::Name>& e) {
    const SymP& s = e->sym;
    if (s && s->kind == py::str("global") && s->owner && *s->owner == py::str("System") &&
        (s->name == py::str("out") || s->name == py::str("err"))) {
        auto tn = std::make_shared<ir::TypeName>();
        tn->type = SYSTEM;
        auto n = at<ir::Select>(e);
        n->obj = tn;
        n->sym = std::make_shared<ir::Sym>(py::str("field"), s->name, std::optional<py::str>(py::str("java.lang.System")), true, PRINTSTREAM);
        n->type = PRINTSTREAM;
        return n;
    }
    return e;
}

NodeP Lift::x_Unary(const py::ref<ir::Unary>& e) {
    auto array_or_string = [&](const TypeP& t) { return kind_is(t, "array") || ir::same(t, STRING); };
    if (e->op && *e->op == py::str("*") && type_of(e->x) && array_or_string(type_of(e->x))) {
        return e->x;   // (*arr / *str: the reference itself)
    }
    if (e->op && *e->op == py::str("&") && e->type && is_ref(e->type) && type_of(e->x) && array_or_string(type_of(e->x))) {
        return e->x;
    }
    return e;
}

NodeP Lift::x_New(const py::ref<ir::New>& e) {
    TypeP t = e->type;
    TypeP base = kind_is(t, "ptr") ? t->elem : t;
    std::optional<py::str> owner = e->sym ? e->sym->owner : std::nullopt;
    if (in_strings(owner) || ir::same(base, STRING)) {
        if (!py::truthy(e->args)) {
            auto n = at<ir::Lit>(e);
            n->value = Val(py::str(""));
            n->type = STRING;
            return n;
        }
        if (py::len(*e->args) == 1) {
            NodeP a = strip_(value_((*e->args)[0]));
            if (ir::same(type_of(a), STRING) || is_a<ir::Concat>(a) || is_a<ir::Lit>(a)) {
                if (owner && *owner == py::str("JCat")) {
                    auto n = at<ir::Concat>(e);
                    n->parts = py::list<NodeP>();
                    n->type = STRING;
                    return n;
                }
                return a;
            }
        }
    }
    if (owner && *owner == py::str("JCat") && !py::truthy(e->args)) {
        auto n = at<ir::Concat>(e);
        n->parts = py::list<NodeP>();
        n->type = STRING;
        return n;
    }
    if ((owner && py::truthy(*owner) && owner->startswith(py::str("JArray"))) || kind_is(base, "array")) {
        TypeP at_ = kind_is(base, "array") ? base : lift_type(mk("class", py::str("JArray")));
        int64_t nargs = e->args ? py::len(*e->args) : 0;
        if (nargs == 1) {
            TypeP a0 = type_of((*e->args)[0]);
            if (a0 && (a0->kind == py::str("i32") || a0->kind == py::str("i64") || a0->kind == py::str("u32") || a0->kind == py::str("u64"))) {
                auto n = at<ir::NewArray>(e);
                n->elem = at_->elem;
                n->dims = py::list<NodeP>{(*e->args)[0]};
                n->type = at_;
                return n;
            }
        }
    }
    return e;
}

NodeP Lift::x_Call(const py::ref<ir::Call>& e) {
    const SymP& s = e->sym;
    std::optional<py::str> name = s ? std::optional<py::str>(s->name) : std::nullopt;
    py::list<NodeP> args;
    if (e->args) {
        for (const NodeP& a : *e->args) args.append(value_(a));   // (the runtime's helpers take const T&: the value)
    }
    NodeP target = e->target ? strip_(e->target) : NodeP();
    auto is = [&](const char* n) { return name && *name == py::str(n); };
    if (is("operator+") &&
        ((s->owner && *s->owner == py::str("JCat")) || is_a<ir::Concat>(target) ||
         (target && ir::same(type_of(target), STRING) && (!s->owner || *s->owner == py::str("JCat"))))) {
        NodeP t = strip_(e->target);
        py::list<NodeP> parts;
        if (auto c = as<ir::Concat>(t)) {
            if (!c->parts) not_iterable();
            parts = c->parts->copy();
        } else {
            parts = py::list<NodeP>{t};
        }
        NodeP a = py::truthy(args) ? strip_(args[0]) : NodeP();
        if (auto u = as<ir::Unary>(a)) {
            if (u->op && *u->op == py::str("&")) a = strip_(u->x);
        }
        parts.append(a);
        auto n = at<ir::Concat>(e);
        n->parts = parts;
        n->type = STRING;
        return n;
    }
    if (is("operator[]") && e->target && type_of(e->target) && kind_is(type_of(e->target), "array")) {
        auto n = at<ir::Index>(e);
        n->arr = e->target;
        n->index = args[0];
        n->type = e->type;
        return n;
    }
    if (!e->target && (is("j2xToInt") || is("j2xToLong"))) {
        auto n = at<ir::Cast>(e);
        n->x = args[0];
        n->type = mk(is("j2xToInt") ? "i32" : "i64");
        return n;
    }
    if (!e->target && is("j2xUshr")) {
        auto n = at<ir::Binary>(e);
        n->op = py::str(">>>");
        n->left = args[0];
        n->right = args[1];
        n->type = e->type;
        return n;
    }
    if (!e->target && name && MATH_NAMES.contains(*name)) {
        auto tn = std::make_shared<ir::TypeName>();
        tn->type = MATH;
        auto n = at<ir::Call>(e);
        n->target = tn;
        n->sym = std::make_shared<ir::Sym>(py::str("method"), MATH_NAMES.at(*name), std::optional<py::str>(py::str("java.lang.Math")), true,
                                           nullptr, s->params, s->ret);
        n->args = args;
        n->typeargs = py::list<TypeP>();
        n->type = e->type;
        return n;
    }
    if ((is("println") || is("print")) && e->target && ir::same(type_of(e->target), PRINTSTREAM)) {
        if (py::truthy(args)) {
            NodeP a = strip_(args[0]);
            if (auto u = as<ir::Unary>(a)) {
                if (u->op && *u->op == py::str("&")) a = u->x;
            }
            args = py::list<NodeP>{a};
        }
        py::list<TypeP> ptypes;
        for (const NodeP& a : args) ptypes.append(type_of(a));
        auto n = at<ir::Call>(e);
        n->target = e->target;
        n->sym = std::make_shared<ir::Sym>(py::str("method"), *name, std::optional<py::str>(py::str("java.io.PrintStream")), false, nullptr,
                                           ptypes, ir::VOID);
        n->args = args;
        n->typeargs = py::list<TypeP>();
        n->type = ir::VOID;
        return n;
    }
    return e;
}

NodeP Lift::x_Binary(const py::ref<ir::Binary>& e) { return e; }

// ---- the sequencing temporaries
py::list<NodeP> Lift::fold_temps(const py::list<NodeP>& stmts) {
    py::list<NodeP> out = stmts.copy();
    int64_t i = 0;
    while (i < py::len(out)) {
        py::dict<py::str, int64_t> temps;
        int64_t j = i;
        while (j < py::len(out)) {
            auto loc = as<ir::Local>(out[j]);
            if (!(loc && loc->sym && loc->sym->name.startswith(py::str("j2xo")))) break;
            temps.set(loc->sym->name, j);
            j += 1;
        }
        if (!temps.empty() && j < py::len(out)) {
            NodeP host = out[j];
            auto comma = first_comma_(host);
            if (comma) {
                py::list<NodeP> parts = commas_(std::get<0>(*comma));
                py::list<NodeP> assigns = parts.slice(0, py::len(parts) - 1);
                NodeP last = parts[-1];
                bool all = true;
                for (const NodeP& a : assigns) {
                    auto as_ = as<ir::Assign>(a);
                    bool ok = as_ && !as_->op;
                    if (ok) {
                        auto nm = as<ir::Name>(strip_(as_->target));
                        ok = nm != nullptr;
                        if (ok) {
                            if (!nm->sym) none_attr("name");
                            ok = temps.contains(nm->sym->name);
                        }
                    }
                    if (!ok) {
                        all = false;
                        break;
                    }
                }
                if (all && py::len(assigns) == temps.size()) {
                    py::list<NodeP> order;
                    for (const NodeP& a : assigns) {
                        auto as_ = as<ir::Assign>(a);
                        auto nm = as<ir::Name>(strip_(as_->target));
                        auto loc = as<ir::Local>(out[temps.at(nm->sym->name)]);
                        loc->init = as_->value;
                        order.append(loc);
                    }
                    out.set_slice(i, j, order);
                    if (!std::get<1>(*comma)) throw py::TypeError("'NoneType' object is not callable");
                    std::get<1>(*comma)(last);
                    i += py::len(order);
                    continue;
                }
            }
        }
        i += 1;
    }
    return out;
}

py::list<py::ref<ir::Unit>> lift(const py::list<py::ref<ir::Unit>>& units) {
    Lift lf;
    for (const auto& u : units) lf.names(u);
    for (const auto& u : units) lf.unit(u);
    return units;
}

}  // namespace codebreaker::translate::frontends::clang::j2x
