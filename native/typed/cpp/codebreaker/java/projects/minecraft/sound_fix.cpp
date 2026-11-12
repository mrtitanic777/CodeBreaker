// codebreaker/java/projects/minecraft/sound_fix.cpp -- the legacy IR form (patch) of the sound-fix pass, over the OLD
// translate IR (py::ref nodes). COMPAT(py): remove when translate/ir and translate/rewrite are refactored and
// their shims land; the tree-shape pass then lives only in cb/java/projects/minecraft/sound_fix.cpp (over the new IR).
// The text form (process / run / stranded) forwards to the new module (see the header).

#include "codebreaker/java/projects/minecraft/sound_fix.hpp"

#include <array>
#include <map>
#include <memory>

#include "codebreaker/translate/ir.hpp"
#include "codebreaker/translate/rewrite.hpp"

namespace codebreaker::java::projects::minecraft::sound_fix {

namespace ir = codebreaker::translate::ir;
namespace rw = codebreaker::translate::rewrite;

namespace {

[[noreturn]] void none_iter() { throw py::TypeError("'NoneType' object is not iterable"); }

template <class T> py::ref<T> as(const ir::NodeP& n) { return std::dynamic_pointer_cast<T>(n); }
template <class T> py::ref<T> as(const ir::NodeOrList& v) { return std::dynamic_pointer_cast<T>(ir::node_of(v)); }   // a list is no node

// A body slot (NodeOrList) as the node ir.walk() takes; a bare list of statements has no .children().
ir::NodeP walkable(const ir::NodeOrList& v) {
    if (std::holds_alternative<py::list<ir::NodeP>>(v)) throw py::AttributeError("'list' object has no attribute 'children'");
    return std::get<ir::NodeP>(v);
}

}  // namespace

Result patch(const py::list<ir::NodeP>& units) {
    // class name (None possible) -> [sfx, music]
    std::map<std::optional<py::str>, std::array<int64_t, 2>> per;
    for (const auto& cm : rw::methods(units)) {
        const py::ref<ir::ClassDecl>& cls = std::get<0>(cm);
        const py::ref<ir::MethodDecl>& m = std::get<1>(cm);
        for (ir::Nodes ss : rw::stmt_lists(walkable(m->body))) {
            if (!ss) {
                throw py::TypeError("object of type 'NoneType' has no len()");
            }
            for (int64_t i = 0; i < py::len(*ss) - 1; ++i) {
                ir::NodeP loc_n = (*ss)[i];
                ir::NodeP nxt_n = (*ss)[i + 1];
                auto loc = as<ir::Local>(loc_n);
                auto nxt = as<ir::If>(nxt_n);
                if (!(loc && loc->init && nxt)) {
                    continue;
                }
                ir::SymP v = loc->sym;
                if (rw::is_call(loc->init, py::str("getRandomSoundFromSoundPool"), py::str("soundPoolSounds"))) {
                    ir::NodeP cond = rw::strip(nxt->cond);
                    ir::NodeP rest;
                    if (rw::is_null_test(cond, v)) {
                        rest = nullptr;
                    } else if (as<ir::Binary>(cond) && as<ir::Binary>(cond)->op && *as<ir::Binary>(cond)->op == py::str("&&") &&
                               rw::is_null_test(as<ir::Binary>(cond)->left, v)) {
                        rest = as<ir::Binary>(cond)->right;     // keep the volume gate
                    } else {
                        continue;
                    }
                    // the sound name: the XACT cue
                    auto call = as<ir::Call>(rw::strip(loc->init));
                    if (!call->args) {
                        throw py::TypeError("'NoneType' object is not subscriptable");
                    }
                    if (py::len(*call->args) == 0) {
                        throw py::IndexError("list index out of range");
                    }
                    ir::NodeP sound = (*call->args)[0];
                    loc->init = rw::null();                    // no query: the empty pool's lookup hangs on console
                    nxt->cond = rest ? rest : rw::true_();

                    auto route = [&](const ir::NodeP& n) -> ir::NodeP {
                        auto sel = as<ir::Select>(n);
                        if (sel && rw::is_name(sel->obj, v) && sel->sym) {
                            if (sel->sym->name == py::str("soundUrl")) {
                                return rw::null();
                            }
                            if (sel->sym->name == py::str("soundName")) {
                                return sound;
                            }
                        }
                        return nullptr;
                    };
                    rw::replace(nxt, route);
                    per[cls->name][0] += 1;
                } else if (rw::is_call(loc->init, py::str("getRandomSound"), py::str("soundPoolMusic")) &&
                           rw::is_null_test(nxt->cond, v) && !ir::truthy(nxt->orelse) && as<ir::Block>(nxt->then)) {
                    nxt->cond = rw::true_();                   // play("BgMusic") starts the XACT playlist: always
                    auto then = as<ir::Block>(nxt->then);
                    if (!then->stmts) {
                        none_iter();
                    }
                    py::list<ir::NodeP> rebuilt;
                    for (const ir::NodeP& s : *then->stmts) {
                        if (rw::mentions(s, v)) {
                            rebuilt.append(rw::if_(rw::binary(py::str("!="), rw::name(v, loc->type), rw::null(), ir::BOOL), s));
                        } else {
                            rebuilt.append(s);
                        }
                    }
                    then->stmts = rebuilt;
                    per[cls->name][1] += 1;
                }
            }
        }
    }
    Result res;
    for (const auto& kv : per) {
        if (!kv.first) {
            throw py::TypeError("'<' not supported between instances of 'NoneType' and 'str'");   // sorted / c + '.cpp'
        }
    }
    for (const auto& kv : per) {
        res.files.append(Touched{*kv.first + py::str(".cpp"), kv.second[0], kv.second[1]});
        res.sites += kv.second[0] + kv.second[1];
    }
    return res;
}

}  // namespace codebreaker::java::projects::minecraft::sound_fix
