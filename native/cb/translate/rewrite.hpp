// cb/translate/rewrite.hpp -- small tools for the passes that change the translator's IR in place (docs/TRANSLATOR_DESIGN.md,
// passes).
//
// A pass matches a shape in the tree -- by node kinds, symbols and types, never by the text a back end would print -- and
// rebuilds it from the nodes it matched plus a few new ones, made in the arena the tree lives in. Language-neutral: what a pass
// means belongs to the pass (a project's fix, a platform's), not here.
//
//     for (const rewrite::MethodInClass& m : rewrite::methods(units))            // every method with a body
//         for (std::vector<ir::Node*>* list : rewrite::stmt_lists(m.method_node)) // every block's and case's statements
//             ...                                                                // splice, replace, insert
//     rewrite::replace(root, [&](ir::Node* n) -> ir::Node* { return is_old(n) ? make_new(arena, n) : nullptr; });
#pragma once

#include <cstdint>
#include <functional>
#include <string_view>
#include <vector>

#include "cb/core/arena.hpp"
#include "cb/translate/ir.hpp"

namespace cb::translate::rewrite {

namespace ir = cb::translate::ir;

// The expression under any grouping parentheses (null stays null).
ir::Node* strip(ir::Node* expr);

// A method, constructor or initialiser block that has a body, with the class that declares it.
struct MethodInClass {
    ir::ClassDecl* cls;
    ir::MethodDecl* method;
    ir::Node* method_node;   // the node that holds `method`
};

// Every method, constructor and initialiser block of the units that has a body, nested classes included, in source order (a
// class's own members before those of the classes nested in it). `units` are Unit nodes; anything else is skipped.
std::vector<MethodInClass> methods(const std::vector<ir::Node*>& units);

// Every statement list under `node` (and including it) that statements sit in, in order: a block's, a case's. The pointers stay
// valid while those lists are not reallocated; a pass that adds statements to a list it holds changes the tree.
std::vector<std::vector<ir::Node*>*> stmt_lists(ir::Node* node);

// Rebuild `node`'s subtree bottom-up: `fn(child)` returns a replacement, or null to keep the child. The node itself is not
// replaced (its parent holds it). A replacement is not visited again.
void replace(ir::Node* node, const std::function<ir::Node*(ir::Node*)>& fn);

// Does `node` (or anything under it) name the local or parameter `sym`? Symbols are compared by name: each use carries its own.
bool mentions(ir::Node* node, const ir::Sym& sym);
// Is `expr`, parentheses aside, a use of `sym`?
bool is_name(ir::Node* expr, const ir::Sym& sym);

// A call of method `name`; with `on_field` non-empty, on `this.<on_field>`.
bool is_call(ir::Node* expr, std::string_view name, std::string_view on_field = {});

// `sym != null` (or `==`, as `op` says).
bool is_null_test(ir::Node* expr, const ir::Sym& sym, std::string_view op = "!=");

// A statement list or a single statement as a list: a Block's statements, else the statement alone.
std::vector<ir::Node*> block_or_single(const ir::Suite& suite);

// ---- building (in the arena the tree lives in) -----------------------------------------------------------------------------

ir::Node* lit_int(Arena& arena, std::int64_t value, ir::TypeP type);
ir::Node* null_lit(Arena& arena);
ir::Node* true_lit(Arena& arena);
// A use of `sym`, of its type unless one is given.
ir::Node* name_of(Arena& arena, ir::SymP sym, ir::TypeP type = {});
ir::Node* binary(Arena& arena, std::string_view op, ir::Node* left, ir::Node* right, ir::TypeP type);
// `if (cond) then`, where `then` is wrapped in a Block unless it already is one.
ir::Node* if_then(Arena& arena, ir::Node* cond, ir::Node* then);

}  // namespace cb::translate::rewrite
