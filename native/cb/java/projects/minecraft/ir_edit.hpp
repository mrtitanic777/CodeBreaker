// cb/java/projects/minecraft/ir_edit.hpp -- small tools for the Minecraft fix passes that change the translator's IR
// in place (applet_ping, sound_fix, render_fix).
//
// A pass matches a shape in the tree -- by node kinds, symbols and types, never by the text a back end would print --
// and rebuilds it from the nodes it matched plus a few new ones, made in the arena the tree lives in. Language-neutral:
// what a pass means belongs to the pass, not here. (The translator's own pass tools, translate/rewrite, take the same
// shape; they join this file when that module is refactored.)
#pragma once

#include <functional>
#include <string_view>
#include <vector>

#include "cb/translate/ir.hpp"

namespace cb::java::projects::minecraft::ir_edit {

namespace ir = cb::translate::ir;

// The expression under any grouping parentheses.
ir::Node* strip(ir::Node* e);

// A method, constructor or initialiser block that has a body, with the class that declares it.
struct MethodInClass {
    ir::ClassDecl* cls;
    ir::MethodDecl* method;
};

// Every method, constructor and initialiser block of the units that has a body, nested classes included, in source
// order. `units` are Unit nodes.
std::vector<MethodInClass> methods(const std::vector<ir::Node*>& units);

// Every statement list under `node` that statements sit in, in order: a block's, a case's. Pointers stay valid while
// the tree's slots are not reallocated.
std::vector<std::vector<ir::Node*>*> stmt_lists(ir::Node* node);

// Rebuild `node`'s subtree bottom-up: fn(child) returns a replacement or nullptr (keep it). The node itself is not
// replaced (its parent holds it).
void replace(ir::Node* node, const std::function<ir::Node*(ir::Node*)>& fn);

// Does `node` refer to the local or parameter `sym` (by name: each use carries its own symbol)?
bool mentions(ir::Node* node, const ir::Sym& sym);
bool is_name(ir::Node* e, const ir::Sym& sym);

// A call of method `name`; with `on_field` non-empty, on `this.<on_field>`.
bool is_call(ir::Node* e, std::string_view name, std::string_view on_field = {});

// `sym != null` (or `==`).
bool is_null_test(ir::Node* e, const ir::Sym& sym, std::string_view op = "!=");

// A statement list or a single statement as a list: a Block's statements, else the statement alone.
std::vector<ir::Node*> block_or_single(const ir::Suite& suite);

// ---- building (in the arena the tree lives in) ----------------------------------------------------------------------

ir::Node* lit_int(Arena& arena, std::int64_t value, ir::TypeP type);
ir::Node* null_lit(Arena& arena);
ir::Node* true_lit(Arena& arena);
ir::Node* name_of(Arena& arena, ir::SymP sym, ir::TypeP type = {});
ir::Node* binary(Arena& arena, std::string_view op, ir::Node* left, ir::Node* right, ir::TypeP type);
// `if (cond) then` where `then` is wrapped in a Block unless it already is one.
ir::Node* if_then(Arena& arena, ir::Node* cond, ir::Node* then);

}  // namespace cb::java::projects::minecraft::ir_edit
