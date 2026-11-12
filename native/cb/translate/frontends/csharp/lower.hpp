// cb/translate/frontends/csharp/lower.hpp -- the C# front end's lowering: Roslyn's bound tree into the translator's IR.
//
// CsTreeDump (codebreaker/translate/frontends/csharp/CsTreeDump.cs) runs the C# compiler over a file and writes what it
// bound as JSON: the declarations from the symbols the compiler gave them, every body as Roslyn's IOperation tree, in which
// each call already names its overload and lists its arguments in parameter order (defaults and `params` arrays filled in),
// every implicit conversion is a node, and `foreach`, `using`, `lock`, string interpolation, patterns and LINQ's query syntax
// are already what they mean. This module reads one file's dump and builds a cb::translate::ir::Unit from it:
//
//     Arena arena;
//     csharp::Lowered out = csharp::lower_text(arena, dump_text, "Shapes");   // ParseError for text that is not a dump
//     ir::Node* unit = out.unit;                                                // an ir::Unit, in `arena`
//     for (const csharp::Refusal& r : out.refused) ...                          // what became ir::Unsupported, and why
//
// What the lowering decides:
//
//   * a name is lowered from the symbol it binds to (a field read on this or on its class, a local, a parameter, a
//     property, an indexer), never from its spelling;
//   * implicit conversions that every target makes the same way (widening, boxing, identity) are left implicit, as javac's
//     tree leaves them; explicit ones are Casts (`checked`: overflow trap); user-defined ones, and user-defined operators,
//     are calls of their op_ methods (Call, flag "operator"), as are the BCL's operators that are methods (string ==,
//     decimal); `as` is a test and a cast;
//   * C#'s own kinds of member and type keep their shape: a property or indexer is a Property declaration and is read as a
//     Select (an indexer as an Index with its sym); a struct, a record, a delegate are ClassDecls of that kind; an enum
//     constant carries its number (EnumConst::value);
//   * what needs a statement where C# has an expression (an object or collection initialiser, a switch expression on a value
//     with effects, an `out var` declaration) is hoisted before the statement -- only where that cannot change what runs
//     first: never out of a branch that might not run, a loop condition, or past an earlier effect in the same statement;
//   * Roslyn's tree keeps no parentheses, so they are put back wherever an operand binds looser than its operator (Paren
//     nodes, as javac's tree has them), and a back end that prints the grouping the source wrote groups as C# does.
//
// What cannot be carried faithfully yet becomes ir::Unsupported (its kind and why), never a guess, and is listed in
// Lowered::refused: `await`, iterators (`yield return`), tuples and deconstruction, local functions, lifted nullable
// arithmetic, pattern-matching `switch` statements, `goto case`, exception filters, events, unsafe code, `dynamic`, decimal
// literals. A construct that refuses inside a statement makes that statement an ExprStmt of the Unsupported; a refusing
// field initialiser or lambda body becomes the Unsupported in its place.
//
// A tree that is not what CsTreeDump writes (a member missing, a value of the wrong kind) is a ParseError naming the member
// and the node's line.
#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/arena.hpp"
#include "cb/core/json_fwd.hpp"
#include "cb/translate/ir.hpp"

namespace cb::translate::frontends::csharp {

// The language the units carry in ir::Unit::lang.
inline constexpr std::string_view kLang = "csharp";

// A construct that became ir::Unsupported: where, which kind of construct ("await", "local-function" ...; absent for a
// member kind the dump did not name), and why.
struct Refusal {
    std::uint32_t line = 0;
    std::optional<std::string> kind;
    std::string why;
};

struct Lowered {
    ir::Node* unit = nullptr;       // an ir::Unit, its nodes made in the arena the caller gave
    std::vector<Refusal> refused;   // in the order they were met
};

// One file's CsTreeDump tree -> its Unit. `path` is the file's name, kept in Unit::path.
Lowered lower(Arena& arena, const Json& tree, std::optional<std::string> path = std::nullopt);

// The same from a dump's text. ParseError when the text is not JSON.
Lowered lower_text(Arena& arena, std::string_view dump_text, std::optional<std::string> path = std::nullopt);

// Put the parentheses back that Roslyn's tree does not keep (lower() does it already): every operand that binds looser than
// the operator it sits under is wrapped in a Paren, in place, in the tree under `root`.
void group(Arena& arena, ir::Node* root);

}  // namespace cb::translate::frontends::csharp
