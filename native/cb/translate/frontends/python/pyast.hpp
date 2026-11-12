// cb/translate/frontends/python/pyast.hpp -- the Python syntax tree the front end walks.
//
// The tree itself is native/pyfront's: CPython's `ast` shape (Python 3.14), one struct per node class, owned by the
// arena of its `pyfront::Module`. This header adds what the front end needs on top of it and pyfront leaves to its
// first consumer: one handle for "any node" (the helper nodes -- arguments, keywords, handlers -- carry no kind field),
// the children of a node in CPython's field order, breadth-first and statement-list walks, the names a function binds,
// and the few expression readers (a constant string, None, a short source text for a base class in a message).
#pragma once

#include <cstdint>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include <pyfront/pyfront.hpp>

namespace cb::translate::frontends::python::pyast {

namespace syntax = ::pyfront;

// What a node is, for the nodes that have no kind field of their own.
enum class Category : std::uint8_t {
    None, Expr, Stmt, Arguments, Arg, Keyword, Alias, WithItem, Comprehension, Handler, MatchCase, Pattern, TypeParam
};

// A node of a tree and what it is. Cheap to copy; does not own.
struct AnyNode {
    const syntax::Node* node = nullptr;
    Category category = Category::None;

    AnyNode() = default;
    AnyNode(const syntax::Expr* e) : node(e), category(e ? Category::Expr : Category::None) {}   // NOLINT: implicit by design
    AnyNode(const syntax::Stmt* s) : node(s), category(s ? Category::Stmt : Category::None) {}   // NOLINT
    AnyNode(const syntax::Node* n, Category c) : node(n), category(n ? c : Category::None) {}

    explicit operator bool() const { return node != nullptr; }
    const syntax::Expr* expr() const { return category == Category::Expr ? static_cast<const syntax::Expr*>(node) : nullptr; }
    const syntax::Stmt* stmt() const { return category == Category::Stmt ? static_cast<const syntax::Stmt*>(node) : nullptr; }
    bool operator==(const AnyNode& other) const { return node == other.node; }
};

// ast.iter_child_nodes: the children appended to `out` in CPython's field order (operators and contexts, which CPython
// also yields, are leaves and left out).
void children(AnyNode parent, std::vector<AnyNode>& out);

// ast.walk: breadth first, the node itself first.
std::vector<AnyNode> walk(AnyNode root);

// Every statement list a node holds (body, orelse, finalbody, a handler's or a case's body).
std::vector<const std::vector<syntax::Stmt*>*> statement_lists(AnyNode node);

// Every name a function binds: a name stored or deleted anywhere in its body, an import's local name, an `except ... as`
// name, a nested def or class (Python's rule: a name assigned anywhere in a function is local to it). A nested def's or
// class's own body, a lambda, and a comprehension (their own scopes) are not entered.
std::set<std::string> bound_names(const syntax::FunctionDef& fn);

// The text of a string constant, nullopt for any other expression.
std::optional<std::string> string_constant(const syntax::Expr* e);
bool is_none_constant(const syntax::Expr* e);
// True for an Ellipsis constant.
bool is_ellipsis(const syntax::Expr* e);

// The source text of an expression the way a message names it: names, dotted names, subscripts, calls, tuples,
// slices, simple constants; any other expression as "<Kind>".
std::string describe_expr(const syntax::Expr* e);

}  // namespace cb::translate::frontends::python::pyast
