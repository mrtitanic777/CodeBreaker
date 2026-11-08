// codebreaker/translate/rewrite.cpp -- typed C++ port of codebreaker.translate.rewrite (see rewrite.hpp).

#include "codebreaker/translate/rewrite.hpp"

#include <algorithm>
#include <vector>

namespace codebreaker::translate::rewrite {

namespace {

using ir::NodeP;
using ir::Nodes;
using ir::SymP;
using ir::TypeP;
using Fn = std::function<NodeP(const NodeP&)>;

[[noreturn]] void none_attr(const char* attr) {
    throw py::AttributeError(std::string("'NoneType' object has no attribute '") + attr + "'");
}

[[noreturn]] void not_iterable() { throw py::TypeError("'NoneType' object is not iterable"); }

// `sym.name` (AttributeError for None)
const py::str& name_of(const SymP& sym) {
    if (!sym) none_attr("name");
    return sym->name;
}

template <class T> bool is_a(const NodeP& n) { return dynamic_cast<const T*>(n.get()) != nullptr; }

// ---- the slots of a node, by name (Python's getattr / setattr over node.fields)

// calls f(name, member, rank) for every slot of the node, whatever its class
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

// One field's work, run when its turn comes (the slot is read then, as the Python reads it).
struct Step {
    int rank;
    std::function<void()> run;
};

NodeP replace_in(const NodeP& node, const Fn& fn);

// a node held by a slot: rebuild under it, then offer it to fn
template <class Set> void replace_child(const NodeP& v, const Fn& fn, Set&& set) {
    if (!v) return;
    replace_in(v, fn);
    NodeP r = fn(v);
    if (r) set(r);
}

void replace_list(const py::list<NodeP>& v, const Fn& fn) {
    for (int64_t i = 0; i < py::len(v); ++i) {
        NodeP x = v[i];
        replace_child(x, fn, [&](const NodeP& r) { v[i] = r; });
    }
}

void add_step(std::vector<Step>& out, int rank, NodeP& m, const Fn& fn) {
    out.push_back({rank, [&m, &fn] {
                       NodeP v = m;
                       replace_child(v, fn, [&](const NodeP& r) { m = r; });
                   }});
}
void add_step(std::vector<Step>& out, int rank, Nodes& m, const Fn& fn) {
    out.push_back({rank, [&m, &fn] {
                       if (m) replace_list(*m, fn);
                   }});
}
void add_step(std::vector<Step>& out, int rank, std::variant<NodeP, py::list<NodeP>>& m, const Fn& fn) {
    out.push_back({rank, [&m, &fn] {
                       if (std::holds_alternative<NodeP>(m)) {
                           NodeP v = std::get<NodeP>(m);
                           replace_child(v, fn, [&](const NodeP& r) { m = r; });
                       } else {
                           replace_list(std::get<py::list<NodeP>>(m), fn);
                       }
                   }});
}
// (a slot of `fields` always holds one of the three above)
template <class T> void add_step(std::vector<Step>&, int, T&, const Fn&) {}

NodeP replace_in(const NodeP& node, const Fn& fn) {
    if (!node) none_attr("fields");
    std::vector<Step> steps;
    each_slot_of(*node, [&](const char*, auto& member, auto rank) {
        if (static_cast<int>(rank) != 0) add_step(steps, static_cast<int>(rank), member, fn);
    });
    std::stable_sort(steps.begin(), steps.end(), [](const Step& a, const Step& b) { return a.rank < b.rank; });
    for (const Step& s : steps) s.run();
    return node;
}

py::ref<ir::ClassDecl> as_class(const NodeP& c) {
    if (!c) none_attr("members");
    auto k = std::dynamic_pointer_cast<ir::ClassDecl>(c);
    if (!k) throw py::AttributeError("'" + c->class_name().utf8() + "' object has no attribute 'members'");
    return k;
}

// classes(c): c, then the classes nested in its members, depth first
void classes(const py::ref<ir::ClassDecl>& c, std::vector<py::ref<ir::ClassDecl>>& out) {
    out.push_back(c);
    if (!c->members) not_iterable();
    for (const NodeP& m : *c->members) {
        if (auto k = std::dynamic_pointer_cast<ir::ClassDecl>(m)) classes(k, out);
    }
}

}  // namespace

NodeP strip(const NodeP& e_) {
    NodeP e = e_;
    while (auto p = std::dynamic_pointer_cast<ir::Paren>(e)) e = p->x;
    return e;
}

py::list<std::tuple<py::ref<ir::ClassDecl>, py::ref<ir::MethodDecl>>> methods(const py::list<NodeP>& units) {
    py::list<std::tuple<py::ref<ir::ClassDecl>, py::ref<ir::MethodDecl>>> out;
    for (const NodeP& un : units) {
        auto u = std::dynamic_pointer_cast<ir::Unit>(un);
        if (!u) {
            if (!un) none_attr("types");
            throw py::AttributeError("'" + un->class_name().utf8() + "' object has no attribute 'types'");
        }
        if (!u->types) not_iterable();
        for (const NodeP& c : *u->types) {
            std::vector<py::ref<ir::ClassDecl>> ks;
            classes(as_class(c), ks);
            for (const auto& k : ks) {
                if (!k->members) not_iterable();
                for (const NodeP& m : *k->members) {
                    auto md = std::dynamic_pointer_cast<ir::MethodDecl>(m);
                    if (md && md->body) out.append(std::make_tuple(k, md));
                }
            }
        }
    }
    return out;
}

py::list<Nodes> stmt_lists(const NodeP& node) {
    py::list<Nodes> out;
    for (const NodeP& n : ir::walk(node)) {
        if (auto b = std::dynamic_pointer_cast<ir::Block>(n)) out.append(b->stmts);
        else if (auto c = std::dynamic_pointer_cast<ir::Case>(n)) out.append(c->body);
    }
    return out;
}

NodeP replace(const NodeP& node, const std::function<NodeP(const NodeP&)>& fn) { return replace_in(node, fn); }

bool mentions(const NodeP& node, const SymP& sym) {
    for (const NodeP& n : ir::walk(node)) {
        auto nm = std::dynamic_pointer_cast<ir::Name>(n);
        if (nm && nm->sym && nm->sym->name == name_of(sym)) return true;
    }
    return false;
}

bool is_name(const NodeP& e_, const SymP& sym) {
    NodeP e = strip(e_);
    auto nm = std::dynamic_pointer_cast<ir::Name>(e);
    return nm && nm->sym && nm->sym->name == name_of(sym);
}

bool is_call(const NodeP& e_, const py::str& name, const std::optional<py::str>& on_field) {
    NodeP e = strip(e_);
    auto c = std::dynamic_pointer_cast<ir::Call>(e);
    if (!(c && c->sym && c->sym->name == name)) return false;
    if (!on_field) return true;
    NodeP t = c->target ? strip(c->target) : NodeP();
    auto sel = std::dynamic_pointer_cast<ir::Select>(t);
    return sel && is_a<ir::This>(sel->obj) && sel->sym && sel->sym->name == *on_field;
}

bool is_null_test(const NodeP& e_, const SymP& sym, const py::str& op) {
    NodeP e = strip(e_);
    auto b = std::dynamic_pointer_cast<ir::Binary>(e);
    if (!b) return false;
    if (!(b->op && *b->op == op)) return false;
    if (!is_name(b->left, sym)) return false;
    auto r = std::dynamic_pointer_cast<ir::Lit>(strip(b->right));
    return r && r->value.is_none();
}

// -- building
NodeP lit(const ir::Val& value, const TypeP& type_) {
    auto n = std::make_shared<ir::Lit>();
    n->value = value;
    n->type = type_;
    return n;
}

NodeP null() { return lit(ir::Val(), ir::NULL_); }

NodeP true_() { return lit(ir::Val(true), ir::BOOL); }

NodeP name(const SymP& sym, const TypeP& type_) {
    auto n = std::make_shared<ir::Name>();
    n->sym = sym;
    if (type_) {
        n->type = type_;
    } else {
        if (!sym) none_attr("type");
        n->type = sym->type;
    }
    return n;
}

NodeP binary(const py::str& op, const NodeP& left, const NodeP& right, const TypeP& type_) {
    auto n = std::make_shared<ir::Binary>();
    n->op = op;
    n->left = left;
    n->right = right;
    n->type = type_;
    return n;
}

NodeP if_(const NodeP& cond, const NodeP& then, const NodeP& orelse) {
    auto n = std::make_shared<ir::If>();
    n->cond = cond;
    if (is_a<ir::Block>(then)) {
        n->then = then;
    } else {
        auto b = std::make_shared<ir::Block>();
        b->stmts = py::list<NodeP>{then};
        n->then = b;
    }
    n->orelse = orelse;
    return n;
}

}  // namespace codebreaker::translate::rewrite
