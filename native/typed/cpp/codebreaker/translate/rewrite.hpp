// codebreaker/translate/rewrite.hpp -- typed C++ port of codebreaker.translate.rewrite.
//
// NOT YET A SHIM (W1-B26): the pass tools are now native/cb/translate/rewrite.{hpp,cpp} over the typed tree. This port stays, as it was,
// because the callbacks of `replace` and every other tool here work on the OLD IR's nodes (py::ref); the legacy IR form of the
// Minecraft fix passes (java/projects/minecraft/applet_ping, render_fix, sound_fix) is its last consumer. It goes with them, when
// translate/ir's old tree does.
//
// rewrite.py -- small tools for passes that change the IR in place (docs/TRANSLATOR_DESIGN.md, passes).
//
// A pass matches a shape in the tree -- by node kinds, symbols and types, never by the text a back end
// would print -- and rebuilds it from the nodes it matched plus a few new ones. Language-neutral: what a
// pass means belongs to the pass (a project's fix, a platform's), not here.
//
// Typing notes:
//   * methods() and stmt_lists() are generators in the Python; here they return lists, built up front.
//     (A caller that changes the classes' members while it iterates the Python generator would see the
//     change; the list is a snapshot.) The lists stmt_lists() returns are the trees' own lists (py::list
//     shares its storage), so appending to or assigning into one changes the tree, as in the Python. A
//     Block whose stmts is None yields None: Nodes (an optional list).
//   * replace() sets a field of a node by name in the Python (setattr); here it walks the node's typed
//     slots, in the order of its `fields`, and assigns the replacement to the slot itself.
//   * `true` is true_ (a C++ keyword); every other name is the Python's.
//   * name(sym, type_=None) takes a null TypeP for None; the Python's `None` for an optional string
//     (on_field) is std::nullopt.
#pragma once

#include <functional>
#include <optional>
#include <tuple>

#include "codebreaker/translate/ir.hpp"

namespace codebreaker::translate::rewrite {

namespace ir = codebreaker::translate::ir;

// The expression under any grouping parentheses.
ir::NodeP strip(const ir::NodeP& e);

// (class, method) for every method, constructor and initialiser block of the units, nested classes
// included. `units` holds Unit nodes.
py::list<std::tuple<py::ref<ir::ClassDecl>, py::ref<ir::MethodDecl>>> methods(const py::list<ir::NodeP>& units);

// Every statement list under `node` that statements sit in, in order: a block's, a case's.
py::list<ir::Nodes> stmt_lists(const ir::NodeP& node);

// Rebuild `node`'s subtree bottom-up: fn(child) returns a replacement or None (keep it). The node
// itself is not replaced (its parent holds it).
ir::NodeP replace(const ir::NodeP& node, const std::function<ir::NodeP(const ir::NodeP&)>& fn);

// Does `node` refer to the local or parameter `sym` (by name: each use carries its own Sym)?
bool mentions(const ir::NodeP& node, const ir::SymP& sym);

bool is_name(const ir::NodeP& e, const ir::SymP& sym);

// A call of method `name`; with on_field, on `this.<on_field>`.
bool is_call(const ir::NodeP& e, const py::str& name, const std::optional<py::str>& on_field = std::nullopt);

// `sym != null` (or ==).
bool is_null_test(const ir::NodeP& e, const ir::SymP& sym, const py::str& op = py::str("!="));

// -- building
ir::NodeP lit(const ir::Val& value, const ir::TypeP& type_);
ir::NodeP null();
ir::NodeP true_();
ir::NodeP name(const ir::SymP& sym, const ir::TypeP& type_ = nullptr);
ir::NodeP binary(const py::str& op, const ir::NodeP& left, const ir::NodeP& right, const ir::TypeP& type_);
ir::NodeP if_(const ir::NodeP& cond, const ir::NodeP& then, const ir::NodeP& orelse = nullptr);

}  // namespace codebreaker::translate::rewrite
