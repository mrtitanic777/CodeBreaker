// codebreaker/java/projects/minecraft/render_fix.cpp -- the legacy IR form (patch) of the render-fix pass, over the OLD
// translate IR (py::ref nodes). COMPAT(py): remove when translate/ir and translate/rewrite are refactored and
// their shims land; the tree-shape pass then lives only in cb/java/projects/minecraft/render_fix.cpp (over the new IR).
// The text form (process / run / stranded) forwards to the new module (see the header).

#include "codebreaker/java/projects/minecraft/render_fix.hpp"

#include <array>
#include <map>
#include <memory>
#include <set>

#include "codebreaker/translate/ir.hpp"
#include "codebreaker/translate/rewrite.hpp"

namespace codebreaker::java::projects::minecraft::render_fix {

namespace ir = codebreaker::translate::ir;
namespace rw = codebreaker::translate::rewrite;

const int64_t _BUDGET = 8;

bool _budget(const py::list<ir::NodeP>& ss, int64_t i);
bool _hurt(const ir::NodeP& s);

namespace {

[[noreturn]] void none_attr(const char* type, const char* attr) {
    throw py::AttributeError(std::string("'") + type + "' object has no attribute '" + attr + "'");
}

template <class T> py::ref<T> as(const ir::NodeP& n) { return std::dynamic_pointer_cast<T>(n); }
template <class T> py::ref<T> as(const ir::NodeOrList& v) { return std::dynamic_pointer_cast<T>(ir::node_of(v)); }   // a list is no node

// A body slot (NodeOrList) as the node ir.walk() takes; a bare list of statements has no .children().
ir::NodeP walkable(const ir::NodeOrList& v) {
    if (std::holds_alternative<py::list<ir::NodeP>>(v)) throw py::AttributeError("'list' object has no attribute 'children'");
    return std::get<ir::NodeP>(v);
}

// `x == 0` for the value of a Lit: an int, a float or a bool equal to zero (None and a str are not)
bool equals_zero(const ir::Val& v) {
    if (std::holds_alternative<int64_t>(v.v)) return std::get<int64_t>(v.v) == 0;
    if (std::holds_alternative<double>(v.v)) return std::get<double>(v.v) == 0.0;
    if (std::holds_alternative<bool>(v.v)) return !std::get<bool>(v.v);
    return false;
}

}  // namespace

Result patch(const py::list<ir::NodeP>& units) {
    // class name (None possible) -> [update, hurt]
    std::map<std::optional<py::str>, std::array<int64_t, 2>> per;
    for (const auto& cm : rw::methods(units)) {
        const py::ref<ir::ClassDecl>& cls = std::get<0>(cm);
        const py::ref<ir::MethodDecl>& m = std::get<1>(cm);
        for (ir::Nodes ss : rw::stmt_lists(walkable(m->body)).copy()) {
            if (!ss) {
                throw py::TypeError("object of type 'NoneType' has no len()");
            }
            int64_t i = 0;
            while (i < py::len(*ss)) {
                if (i + 2 < py::len(*ss) && _budget(*ss, i)) {
                    per[cls->name][0] += 1;
                }
                if (i < py::len(*ss) && _hurt((*ss)[i])) {
                    per[cls->name][1] += 1;
                }
                i += 1;
            }
        }
    }
    for (const auto& kv : per) {
        if (!kv.first) {
            throw py::TypeError("'<' not supported between instances of 'NoneType' and 'str'");   // sorted / c + '.cpp'
        }
    }
    Result res;
    for (const auto& kv : per) {
        res.files.append(Touched{*kv.first + py::str(".cpp"), kv.second[0], kv.second[1]});
        res.update += kv.second[0];
        res.hurt += kv.second[1];
    }
    return res;
}

bool _budget(const py::list<ir::NodeP>& ss, int64_t i) {
    ir::NodeP a_n = ss[i], b_n = ss[i + 1], loop_n = ss[i + 2];
    auto a = as<ir::Local>(a_n);
    auto b = as<ir::Local>(b_n);
    auto loop = as<ir::For>(loop_n);
    if (!(a && b && loop)) {
        return false;
    }
    ir::NodeP size_b = b->init ? rw::strip(b->init) : ir::NodeP();
    auto size_call = as<ir::Call>(size_b);
    if (!(size_call && size_call->sym && size_call->sym->name == py::str("size"))) {
        return false;
    }
    auto loop_body = as<ir::Block>(loop->body);
    ir::Nodes body = loop_body ? loop_body->stmts : ir::Nodes();
    if (!body || py::len(*body) == 0 || py::len(*body) != 5 || !as<ir::Local>((*body)[0]) || !as<ir::If>((*body)[1])) {
        return false;
    }
    auto w = as<ir::Local>((*body)[0]);
    auto test = as<ir::If>((*body)[1]);
    std::set<py::str> calls;
    for (const ir::NodeP& n : ir::walk(test->cond)) {
        auto c = as<ir::Call>(n);
        if (c && c->sym) {
            calls.insert(c->sym->name);
        }
    }
    if (!calls.count(py::str("distanceToEntitySquared"))) {
        return false;
    }
    py::list<ir::NodeP> then;
    if (auto tb = as<ir::Block>(test->then)) {
        if (!tb->stmts) {
            throw py::TypeError("'NoneType' object is not iterable");
        }
        then = *tb->stmts;
    } else {
        then.append(ir::node_of(test->then));   // a bare list is no Return either
    }
    bool has_return = false;
    for (const ir::NodeP& x : then) {
        if (as<ir::Return>(x)) {
            has_return = true;
            break;
        }
    }
    if (!has_return) {
        return false;
    }
    ir::NodeP get_n = w->init ? rw::strip(w->init) : ir::NodeP();
    while (auto cast = as<ir::Cast>(get_n)) {
        get_n = rw::strip(cast->x);
    }
    auto get = as<ir::Call>(get_n);
    if (!(get && get->sym && get->sym->name == py::str("get"))) {
        return false;
    }
    if (!get->args) {
        throw py::TypeError("object of type 'NoneType' has no len()");
    }
    if (py::len(*get->args) != 1) {
        return false;
    }
    auto idx = as<ir::Binary>(rw::strip((*get->args)[0]));
    if (!(idx && idx->op && *idx->op == py::str("-") && rw::is_name(idx->left, a->sym))) {
        return false;
    }
    ir::TypeP a_type = a->type;
    b->init = rw::lit(ir::Val(_BUDGET), a_type);          // rebuild at most _BUDGET chunks this frame
    a->init = size_b;                                     // A = this.F.size(), now inside the loop
    idx->right = rw::lit(ir::Val(int64_t(1)), a_type);    // the last (nearest, sorted) one
    auto ret = std::make_shared<ir::Return>();
    ir::NodeP empty = rw::if_(rw::binary(py::str("=="), rw::name(a->sym, a_type), rw::lit(ir::Val(int64_t(0)), a_type), ir::BOOL), ret);
    auto kept = std::make_shared<ir::Block>();
    kept->stmts = body->slice(3, 5);
    ir::NodeP present = rw::if_(rw::binary(py::str("!="), rw::name(w->sym, w->type), rw::null(), ir::BOOL), kept);
    loop_body->stmts = py::list<ir::NodeP>{a, empty, w, (*body)[2], present};
    ss.pop(i);                                            // A moved into the loop
    return true;
}

bool _hurt(const ir::NodeP& s_n) {
    auto s = as<ir::If>(s_n);
    if (!s) {
        return false;
    }
    auto then = as<ir::Block>(s->then);
    if (!then || !then->stmts || py::len(*then->stmts) == 0) {
        return false;
    }
    ir::NodeP c_n = rw::strip(s->cond);
    auto c = as<ir::Binary>(c_n);
    if (!(c && c->op && *c->op == py::str(">="))) {
        return false;
    }
    auto left = as<ir::Name>(rw::strip(c->left));
    auto right = as<ir::Lit>(rw::strip(c->right));
    if (!(left && right && equals_zero(right->value))) {
        return false;
    }
    auto first = as<ir::ExprStmt>((*then->stmts)[0]);
    if (!first) {
        return false;
    }
    auto d = as<ir::Assign>(rw::strip(first->expr));
    if (!(d && d->op && *d->op == py::str("/") && rw::is_name(d->target, left->sym))) {
        return false;
    }
    ir::NodeP v_n = rw::strip(d->value);
    while (auto cast = as<ir::Cast>(v_n)) {
        v_n = rw::strip(cast->x);
    }
    auto v = as<ir::Select>(v_n);
    if (!(v && v->sym && v->sym->name == py::str("maxHurtTime"))) {
        return false;
    }
    auto hurt = std::make_shared<ir::Select>();
    hurt->obj = v->obj;
    hurt->sym = std::make_shared<ir::Sym>(py::str("field"), py::str("hurtTime"), v->sym->owner, false, ir::I32);
    hurt->type = ir::I32;
    s->cond = rw::binary(py::str("&&"), s->cond, rw::binary(py::str(">"), hurt, rw::lit(ir::Val(int64_t(0)), ir::I32), ir::BOOL), ir::BOOL);
    return true;
}

}  // namespace codebreaker::java::projects::minecraft::render_fix
