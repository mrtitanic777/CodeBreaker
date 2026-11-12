// codebreaker/translate/backends/cpp03.hpp -- COMPAT SHIM over cb/translate/backends/cpp03.hpp (docs/CPP_DESIGN.md 6.3); deleted with its last
// py:: consumer.
//
// The module is native/cb/translate/backends/cpp03*.{hpp,cpp}: the translator's C++03 / C++17 back end, over the typed IR tree (an arena of
// nodes), as the converter's `BodyTranslator` (cpp03::Translator). Its writers and its body source do not cross to the old IR, so the old
// `Context`, `Writer` and `Bodies` are gone: the old Java transpiler's shim (java/transpile.hpp) hands the new translator the lowering of
// javac's trees instead (java_units below, and the project's IR fixes), and reads the answers through the new body-source interface.
//
// What is left here is what the old callers still use on the OLD tree:
//   * Decline, the exception the C back end (c_java) throws for a body it does not write;
//   * java_units, javac's dumps lowered by the old front end (frontends/java/lower, not yet refactored);
//   * the tree questions the C back end asks of old nodes (_strip_paren, _operands, _order_matters, _constant, _leaves): the same rules as
//     cb::translate::backends::cpp03's functions of the same names over the typed tree, kept as they were for old nodes until c_java is
//     refactored (it will call the new ones);
//   * _float, which crosses to the new float_literal.
#pragma once

#include <set>
#include <string>

#include "cb/translate/backends/cpp03.hpp"
#include "codebreaker/translate/frontends/java/lower.hpp"
#include "codebreaker/translate/ir.hpp"
#include "py.hpp"

namespace codebreaker::translate::backends::cpp03 {

namespace ir = codebreaker::translate::ir;
namespace lower = codebreaker::translate::frontends::java::lower;

// This back end does not write the method; the transpiler's text path does.
struct Decline : py::Exception {
    explicit Decline(const py::str& m) : py::Exception(m) {}
    explicit Decline(const char* m) : py::Exception(m) {}
    const char* type() const override { return "Decline"; }
};

// Run the Java front end's dumps ({path: tree}, what frontends.java.dump returns) through the lowering. -> [ir.Unit]
inline py::list<ir::NodeP> java_units(const py::dict<py::str, ir::Dict>& trees) {
    std::vector<py::str> paths = trees.keys();
    std::sort(paths.begin(), paths.end());
    py::list<ir::NodeP> out;
    for (const py::str& p : paths) out.append(lower::lower(trees.at(p), p));
    return out;
}

namespace legacy_ {

template <class T> bool is_a(const ir::NodeP& n) { return dynamic_cast<const T*>(n.get()) != nullptr; }
template <class T> py::ref<T> as_a(const ir::NodeP& n) { return std::dynamic_pointer_cast<T>(n); }

inline bool pure_owner(const std::optional<py::str>& owner) {
    static const std::set<std::string> owners = {"java.lang.String", "java.lang.Math",      "java.lang.Integer",   "java.lang.Float",
                                                 "java.lang.Double", "java.lang.Long",      "java.lang.Character", "java.lang.Boolean",
                                                 "java.lang.Class",  "java.lang.reflect.Constructor"};
    return owner && owners.count(owner->utf8()) != 0;
}

inline bool leaves_walk(const ir::NodeP& n, int64_t loops, const py::set<py::str>& labels) {
    if (is_a<ir::Return>(n)) return true;
    if (auto b = as_a<ir::Break>(n)) return (b->label && py::truthy(*b->label)) ? !labels.contains(*b->label) : loops == 0;
    if (auto c = as_a<ir::Continue>(n)) return (c->label && py::truthy(*c->label)) ? !labels.contains(*c->label) : loops == 0;
    if (is_a<ir::Lambda>(n) || is_a<ir::ClassDecl>(n) || is_a<ir::LocalClass>(n)) return false;
    int64_t inner = loops + (is_a<ir::While>(n) || is_a<ir::DoWhile>(n) || is_a<ir::For>(n) || is_a<ir::ForEach>(n) || is_a<ir::Switch>(n));
    py::set<py::str> labs = labels;
    if (auto lb = as_a<ir::Labeled>(n)) {
        labs = py::set<py::str>::from(labels);
        if (lb->label) labs.add(*lb->label);
    }
    for (const ir::NodeP& c : n->children()) {
        if (leaves_walk(c, inner, labs)) return true;
    }
    return false;
}

}  // namespace legacy_

inline ir::NodeP _strip_paren(ir::NodeP e) {
    while (auto p = legacy_::as_a<ir::Paren>(e)) e = p->x;
    return e;
}

// The operands of `e` whose evaluation order Java fixes (left to right) and C++ does not.
inline py::list<ir::NodeP> _operands(const ir::NodeP& e) {
    namespace lg = legacy_;
    py::list<ir::NodeP> out;
    if (auto c = lg::as_a<ir::Call>(e)) {
        if (c->target && !(lg::is_a<ir::This>(c->target) || lg::is_a<ir::Super>(c->target) || lg::is_a<ir::TypeName>(c->target))) out.append(c->target);
        if (c->args) out.extend(*c->args);
    } else if (auto n = lg::as_a<ir::New>(e)) {
        if (n->args) out.extend(*n->args);
    } else if (auto b = lg::as_a<ir::Binary>(e)) {
        if (!(b->op && (*b->op == "&&" || *b->op == "||"))) {
            out.append(b->left);
            out.append(b->right);
        }
    } else if (auto c2 = lg::as_a<ir::Concat>(e)) {
        if (c2->parts) out.extend(*c2->parts);
    } else if (auto a = lg::as_a<ir::Assign>(e)) {
        if (auto ix = lg::as_a<ir::Index>(_strip_paren(a->target))) {   // a[i] = v: the index before the value
            if (std::holds_alternative<py::list<ir::NodeP>>(ix->index)) throw py::AttributeError("'list' object has no attribute 'children'");
            out.append(std::get<ir::NodeP>(ix->index));
            out.append(a->value);
        }
    }
    return out;
}

inline bool _constant(const ir::NodeP& n_) {
    namespace lg = legacy_;
    ir::NodeP n = _strip_paren(n_);
    if (lg::is_a<ir::Lit>(n) || lg::is_a<ir::ClassLit>(n) || lg::is_a<ir::This>(n) || lg::is_a<ir::TypeName>(n)) return true;
    auto sel = lg::as_a<ir::Select>(n);
    return sel && lg::is_a<ir::TypeName>(sel->obj) && sel->sym && sel->sym->kind == py::str("enumconst");
}

// Does evaluating `n` change anything (a call into the program, an allocation, an assignment, ++/--)? The names it assigns, and reads.
inline bool _effects(const ir::NodeP& n) {
    namespace lg = legacy_;
    for (const ir::NodeP& x : ir::walk(n)) {
        if (lg::is_a<ir::Assign>(x) || lg::is_a<ir::New>(x)) return true;
        if (auto u = lg::as_a<ir::Unary>(x)) {
            if (u->op && (*u->op == "++" || *u->op == "--")) return true;
        }
        if (auto c = lg::as_a<ir::Call>(x)) {
            if (!(c->sym && lg::pure_owner(c->sym->owner))) return true;
        }
    }
    return false;
}

inline py::set<py::str> _written(const ir::NodeP& n) {
    namespace lg = legacy_;
    py::set<py::str> out;
    for (const ir::NodeP& x : ir::walk(n)) {
        ir::NodeP target;
        if (auto a = lg::as_a<ir::Assign>(x)) target = a->target;
        else if (auto u = lg::as_a<ir::Unary>(x)) {
            if (u->op && (*u->op == "++" || *u->op == "--")) target = u->x;
        }
        if (target) target = _strip_paren(target);
        if (auto nm = lg::as_a<ir::Name>(target)) {
            if (nm->sym) out.add(nm->sym->name);
        } else if (auto sel = lg::as_a<ir::Select>(target)) {
            if (sel->sym) out.add(sel->sym->name);
        }
    }
    return out;
}

inline py::set<py::str> _read(const ir::NodeP& n) {
    namespace lg = legacy_;
    py::set<py::str> out;
    for (const ir::NodeP& x : ir::walk(n)) {
        if (auto nm = lg::as_a<ir::Name>(x)) {
            if (nm->sym) out.add(nm->sym->name);
        } else if (auto sel = lg::as_a<ir::Select>(x)) {
            if (sel->sym) out.add(sel->sym->name);
        }
    }
    return out;
}

inline bool _order_matters(const py::list<ir::NodeP>& nodes) {
    py::list<ir::NodeP> eff;
    for (const ir::NodeP& n : nodes) {
        if (_effects(n)) eff.append(n);
    }
    if (py::len(eff) >= 2) return true;
    if (py::len(eff) == 1) {
        py::set<py::str> w = _written(eff[0]);
        if (w.empty()) return false;
        for (const ir::NodeP& n : nodes) {
            if (n != eff[0] && !(w & _read(n)).empty()) return true;
        }
    }
    return false;
}

// Whether any of these statement trees returns, or breaks or continues out of them.
inline bool _leaves(const py::list<ir::NodeP>& stmts) {
    for (const ir::NodeP& s : stmts) {
        if (s && legacy_::leaves_walk(s, 0, py::set<py::str>())) return true;
    }
    return false;
}

// A float literal as C++ source writes it (the new float_literal, for a value of the old IR).
inline py::str _float(const ir::Val& v, const py::str& suffix) {
    cb::translate::ir::LitValue value;
    if (std::holds_alternative<bool>(v.v)) value = std::get<bool>(v.v);
    else if (std::holds_alternative<int64_t>(v.v)) value = static_cast<std::int64_t>(std::get<int64_t>(v.v));
    else if (std::holds_alternative<double>(v.v)) value = std::get<double>(v.v);
    else if (std::holds_alternative<py::str>(v.v)) value = std::get<py::str>(v.v).utf8();
    try {
        return cb::compat::to_py(cb::translate::backends::cpp03::float_literal(value, suffix.utf8()));
    } catch (const cb::translate::backends::cpp03::Decline& e) {
        throw Decline(py::str(std::string(e.what())));
    }
}

}  // namespace codebreaker::translate::backends::cpp03
