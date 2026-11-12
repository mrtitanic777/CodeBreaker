// codebreaker/java/projects/minecraft/applet_ping.cpp -- the legacy IR form (patch) of the applet-ping pass, over the OLD
// translate IR (py::ref nodes). COMPAT(py): remove when translate/ir and translate/rewrite are refactored and
// their shims land; the tree-shape pass then lives only in cb/java/projects/minecraft/applet_ping.cpp (over the new IR).
// The text form (process / run / stranded) forwards to the new module (see the header).

#include "codebreaker/java/projects/minecraft/applet_ping.hpp"

#include <map>
#include <memory>
#include <set>

#include "codebreaker/translate/ir.hpp"
#include "codebreaker/translate/rewrite.hpp"

namespace codebreaker::java::projects::minecraft::applet_ping {

namespace ir = codebreaker::translate::ir;
namespace rw = codebreaker::translate::rewrite;

namespace {

[[noreturn]] void none_iter() { throw py::TypeError("'NoneType' object is not iterable"); }
[[noreturn]] void no_attr(const std::string& type, const char* attr) {
    throw py::AttributeError("'" + type + "' object has no attribute '" + attr + "'");
}

template <class T> py::ref<T> as(const ir::NodeP& n) { return std::dynamic_pointer_cast<T>(n); }

// `x.stmts` of a node that must be a Block (AttributeError otherwise); the optional list is None when unset.
const ir::Nodes& stmts_of(const ir::NodeP& n) {
    if (!n) no_attr("NoneType", "stmts");
    auto b = std::dynamic_pointer_cast<ir::Block>(n);
    if (!b) no_attr(n->class_name().utf8(), "stmts");
    return b->stmts;
}

// A body slot (NodeOrList) as the node ir.walk() takes; a bare list of statements has no .children().
ir::NodeP walkable(const ir::NodeOrList& v) {
    if (std::holds_alternative<py::list<ir::NodeP>>(v)) no_attr("list", "children");
    return std::get<ir::NodeP>(v);
}

}  // namespace

Result patch(const py::list<ir::NodeP>& units) {
    std::map<std::optional<py::str>, int64_t> per;   // class name (None possible) -> sites
    for (const auto& cm : rw::methods(units)) {
        const py::ref<ir::ClassDecl>& cls = std::get<0>(cm);
        const py::ref<ir::MethodDecl>& m = std::get<1>(cm);
        for (ir::Nodes ss : rw::stmt_lists(walkable(m->body))) {
            if (!ss) {
                none_iter();                               // enumerate(None)
            }
            for (int64_t i = 0; i < py::len(*ss); ++i) {
                auto s = as<ir::Try>((*ss)[i]);
                if (!s || s->final || (s->resources && py::len(*s->resources) > 0)) {
                    continue;
                }
                std::set<py::str> calls;
                for (const ir::NodeP& n : ir::walk(s->body)) {
                    auto c = as<ir::Call>(n);
                    if (c && c->sym) {
                        calls.insert(c->sym->name);
                    }
                }
                if (!(calls.count(py::str("getDocumentBase")) && calls.count(py::str("openStream")))) {
                    continue;
                }
                if (!s->catches) {
                    none_iter();
                }
                bool any_body = false;
                for (const ir::NodeP& cn : *s->catches) {
                    if (!cn) no_attr("NoneType", "body");
                    auto c = as<ir::Catch>(cn);
                    if (!c) no_attr(cn->class_name().utf8(), "body");
                    if (c->body) {
                        const ir::Nodes& cs = stmts_of(c->body);
                        if (cs && py::len(*cs) > 0) {
                            any_body = true;
                            break;
                        }
                    }
                }
                if (any_body) {
                    continue;
                }
                const ir::Nodes& tb = stmts_of(s->body);
                if (!tb) none_iter();
                py::list<ir::NodeP> guarded;
                for (const ir::NodeP& x : *tb) {
                    if (as<ir::If>(x)) {
                        guarded.append(x);
                    }
                }
                if (py::len(guarded) != 1) {
                    continue;
                }
                auto g = as<ir::If>(guarded[0]);
                if (ir::truthy(g->orelse)) {
                    continue;
                }
                if (std::holds_alternative<py::list<ir::NodeP>>(g->then)) {
                    continue;                              // [list]: one element, not an ExprStmt
                }
                const ir::NodeP& then = std::get<ir::NodeP>(g->then);
                py::list<ir::NodeP> body;
                if (as<ir::Block>(then)) {
                    const ir::Nodes& gs = as<ir::Block>(then)->stmts;
                    if (!gs) none_iter();
                    body = *gs;
                } else {
                    body.append(then);
                }
                if (py::len(body) != 1 || !as<ir::ExprStmt>(body[0])) {
                    continue;
                }
                ir::NodeP a = rw::strip(as<ir::ExprStmt>(body[0])->expr);
                auto asg = as<ir::Assign>(a);
                if (!(asg && !asg->op)) {
                    continue;
                }
                auto tgt = as<ir::Select>(asg->target);
                if (!(tgt && as<ir::This>(tgt->obj) && as<ir::Name>(rw::strip(asg->value)))) {
                    continue;
                }
                (*ss)[i] = body[0];                        // set the level directly: no applet on console
                per[cls->name] += 1;
            }
        }
    }
    for (const auto& kv : per) {
        if (!kv.first) {
            throw py::TypeError("unsupported operand type(s) for +: 'NoneType' and 'str'");   // sorted / c + '.cpp'
        }
    }
    Result res;
    for (const auto& kv : per) {
        res.files.append(Touched{*kv.first + py::str(".cpp"), kv.second});
        res.sites += kv.second;
    }
    return res;
}

}  // namespace codebreaker::java::projects::minecraft::applet_ping
