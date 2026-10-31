// astutil.hpp -- what pyfront deliberately leaves to its first consumer: a generic child walker in
// CPython's field order (ast.iter_child_nodes), ast.walk's breadth-first order, a parent map, and
// the small helpers of the `ast` module the engine uses (get_docstring, get_source_segment, unparse
// of names, structural equality).
#pragma once
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "big.hpp"
#include "pyfront/ast.hpp"

namespace pyengine {
namespace A = pyfront;

// Any node of a tree, with what it is (pyfront's helper nodes have no kind field of their own).
struct AnyNode {
    enum Cat : uint8_t { None, Expr, Stmt, Arguments, Arg, Keyword, Alias, WithItem, Comprehension, Handler, MatchCase, Pattern, TypeParam };
    const A::Node* p = nullptr;
    Cat cat = None;
    AnyNode() = default;
    AnyNode(const A::Expr* e) : p(e), cat(e ? Expr : None) {}                 // NOLINT
    AnyNode(const A::Stmt* s) : p(s), cat(s ? Stmt : None) {}                 // NOLINT
    AnyNode(const A::Node* n, Cat c) : p(n), cat(n ? c : None) {}
    explicit operator bool() const { return p != nullptr; }
    const A::Expr* expr() const { return cat == Expr ? static_cast<const A::Expr*>(p) : nullptr; }
    const A::Stmt* stmt() const { return cat == Stmt ? static_cast<const A::Stmt*>(p) : nullptr; }
    bool operator==(const AnyNode& o) const { return p == o.p; }
};

// ast.iter_child_nodes (operators and contexts, which CPython also yields, are leaves and left out)
void child_nodes(AnyNode n, std::vector<AnyNode>& out);
// ast.walk: breadth first, the node itself first
std::vector<AnyNode> walk(AnyNode n);
template <class F> void walk_each(AnyNode n, F&& f) {
    for (const AnyNode& x : walk(n)) f(x);
}

// the parent of every node under a root (the scope pass and the lowering's anchors use it)
class ParentMap {
public:
    void build(AnyNode root);
    AnyNode parent(const A::Node* n) const;
private:
    std::unordered_map<const A::Node*, AnyNode> up_;
};

// the statement lists of a statement (body / orelse / finalbody / handlers' bodies, cases' bodies)
std::vector<const std::vector<A::Stmt*>*> stmt_lists(const A::Stmt* s, bool with_handlers = true);

const A::Pos& pos_of(AnyNode n);
int lineno(AnyNode n);
std::string type_name(AnyNode n);          // the CPython class name (Name, Call, FunctionDef ...)

// ast.get_docstring (clean=True: inspect.cleandoc)
bool docstring(const std::vector<A::Stmt*>& body, std::string& out);
std::string cleandoc(const std::string& s);
// ast.get_source_segment (positions in UTF-8 bytes); false when the node has no position
bool source_segment(const std::string& src, const std::vector<size_t>& line_starts, const A::Pos& p, std::string& out);
std::vector<size_t> line_starts(const std::string& src);
// ast.unparse for the expressions messages name (names, attributes, calls, subscripts, constants)
std::string unparse(const A::Expr* e);
// ast.dump(a) == ast.dump(b)
bool same_tree(const A::Expr* a, const A::Expr* b);

// constants
bool is_str_const(const A::Expr* e, std::string* out = nullptr);
bool is_none_const(const A::Expr* e);
bool int_const(const A::Expr* e, Big* out);   // an int literal, or -literal (not a bool)
bool is_const(const A::Expr* e);

}  // namespace pyengine
