// cb/decompile/l2tree.hpp -- reading and rewriting an L2 document held as a Json tree (asm_blocks, frame16, shape16).
//
// The decompiler's L2 document is a tree of objects whose keys are the node kinds' own ("node", "body", "then", "origin",
// "sym", "type" ...). A document made from a binary carries more than the translator's node classes name, and these
// passes read and edit it by the keys they know, so they work on the Json itself, in place: the caller copies the
// document first when it wants to keep the original (a Json copy is a deep copy).
//
// A lookup that finds something that is not there, or is of another kind, answers "nothing" (a null pointer, an empty
// text, nullopt): the passes treat a node without a member as a node whose member is empty, which is how the decompiler
// writes them.
//
//     if (l2tree::is_kind(stmt, "ExprStmt")) { ... l2tree::text(l2tree::member(stmt, "sym"), "name") ... }
//     l2tree::walk(body, [&](Json& n) { if (l2tree::is_kind(n, "Name")) rename(n); });
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <string_view>
#include <vector>

#include "cb/core/json_fwd.hpp"

namespace cb::decompile::l2tree {

// The member `key` of `node` when `node` is an object that has it (and it is not null); otherwise a null pointer.
const Json* member(const Json* node, std::string_view key);
Json* member(Json* node, std::string_view key);
inline const Json* member(const Json& node, std::string_view key) { return member(&node, key); }
inline Json* member(Json& node, std::string_view key) { return member(&node, key); }

// The text of member `key` when it is a string, else an empty view (valid while the document is not changed).
std::string_view text(const Json* node, std::string_view key);
// The integer in member `key`, when it is one.
std::optional<std::int64_t> integer(const Json* node, std::string_view key);

// An object whose "node" member says it is of this kind.
bool is_kind(const Json* node, std::string_view kind);
inline bool is_kind(const Json& node, std::string_view kind) { return is_kind(&node, kind); }

// Is the value a non-empty array or object, a non-empty string, a number other than zero, or true? (what "x or default"
// tests in the decompiler's own notes)
bool present(const Json* node);

// Every object of the tree, depth first, a node before its members. `f` runs on a node before its members are read, so a
// node it rewrites is walked as rewritten; arrays are read by index over their live size.
void walk(Json& root, const std::function<void(Json&)>& f);
void walk(const Json& root, const std::function<void(const Json&)>& f);
// The nodes alone, collected first (the tree must not change while they are used).
std::vector<const Json*> nodes(const Json& root);

// Every statement list of the tree: the arrays held under "body", "then", "orelse" and "stmts", a list before the lists
// inside it. `f` may rewrite the list it is given (the lists inside are read afterwards, as rewritten).
void for_each_statement_list(Json& root, const std::function<void(Json&)>& f);

// The integers of origin.at of a statement (the addresses of the instructions it came from, as the decompiler writes
// them), each reduced to its 16-bit offset, sorted; without repeats unless `keep_repeats`.
std::vector<std::uint32_t> origin_offsets(const Json& stmt, bool keep_repeats = false);

}  // namespace cb::decompile::l2tree
