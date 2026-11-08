// codebreaker/decompile/ctree.hpp -- typed C++ port of codebreaker.decompile.ctree.
//
// ctree.py -- the decompiler's C as a syntax tree, for the passes of c_text.py that rewrite it.
//
// A pass that finds what to change with a regular expression over the text also finds it inside a
// string or a comment, takes `a.b._1_2_` for a piece of `b`, and cannot tell a declaration from an
// expression that looks like one. Here the text is parsed (tree-sitter's C grammar, which goes on
// past what it cannot parse -- a calling convention it does not know, a type never declared -- and
// keeps every byte's place), a pass finds the nodes it means, and replaces exactly their text:
// everything else stays as it was, byte for byte.
//
//     Source src(text);
//     for (node : src.find({"field_expression"})) {
//         ...
//         src.replace(node, "new text");
//     }
//     text = src.text();                   // the edits applied (they must not overlap)
//
// Nodes are tree-sitter nodes: `type`, `children`, `named_children()`, `child_by_field_name()`,
// `start_byte`, `end_byte`, `parent()`. `src.at(node)` is a node's text as written (with its
// `__far` -- see Source::Source).
//
// Notes on the port:
//   * THE PARSER IS AN INTERFACE. tree-sitter (its runtime and the C grammar) has no C++ port and its
//     sources are not part of this tree, so `Node` is a plain tree record carrying exactly what the Python
//     reads from a tree-sitter node (type, byte range, named, children, field children, parent) and
//     `_parser()` is a hook: call set_parser() with a function that parses the (masked) bytes into a Node
//     tree (a binding to the tree-sitter C library, or a replayed tree in a test). Without one, Source's
//     constructor raises RuntimeError. Everything else in the module -- the masking of the 16-bit
//     qualifiers and end-of-block labels, the cast patterns, every query and the edit machinery -- is the
//     Python's, ported in full;
//   * the bytes patterns (_QUALIFIERS, _LAST_LABEL) run over the bytes read as Latin-1 text with
//     re.ASCII: byte for byte the same matches (a bytes pattern's \w \s \b are ASCII-only);
//   * Node equality is identity (tree-sitter's is the node id): compare the py::ref;
//   * `replace` takes a node or a (start, end) span, as the Python's hasattr test does: two overloads;
//   * `override` of render() is a function from a node to the text to put in its place, or nullopt.
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::decompile::ctree {

// A tree-sitter node, as far as ctree.py reads one.
struct Node {
    py::str type;
    int64_t start_byte = 0;
    int64_t end_byte = 0;
    bool is_named = false;
    std::weak_ptr<Node> parent_;          // empty for the root
    py::list<py::ref<Node>> children;     // every child, anonymous tokens included
    // What child_by_field_name() answers, field name -> child, as the tree-sitter library gives it (its answer
    // for an ERROR node is not the field names its children carry, so it is recorded as the library gives it).
    py::dict<py::str, py::ref<Node>> fields;

    py::ref<Node> parent() const;                          // null for the root
    int64_t child_count() const;
    py::list<py::ref<Node>> named_children() const;
    // The child in that field, or null.
    py::ref<Node> child_by_field_name(const py::str& name) const;
};
using NodeP = py::ref<Node>;

// Builds the parent links of a tree whose `children` and `child_fields` are filled in.
void link_parents(const NodeP& root);

// Parses (masked) C text into a tree.
using Parser = std::function<NodeP(const py::bytes&)>;

// The decompiler's own type names (decompile/c_text.py HEADER) and C's. The grammar does not know
// which names are types, so `(uint4)*p` reads as `(uint4) * p`, a product, `(uint2)&x` as a bitwise
// and, and `(uint2)(uint1)*p` as a call: misread() tells such a node from the cast it is.
extern const py::str _TYPE;
extern const py::str _QUALIFIERS_PATTERN;
extern const py::str _LAST_LABEL_PATTERN;
extern const py::str _CASTS_PATTERN;

// Installs the parser _parser() hands out.
void set_parser(const Parser& parser);
// The parser (RuntimeError when none was installed).
const Parser& _parser();

class Source {
  public:
    py::str original;
    py::bytes data;
    NodeP tree;
    NodeP root;
    py::list<std::tuple<int64_t, int64_t, py::bytes>> edits;

    explicit Source(const py::str& text);

    // -- reading ----------------------------------------------------------------------------------
    py::str at(const NodeP& node) const;
    // Every node of these types, in the order they appear (outermost first); under = null for the whole tree.
    py::list<NodeP> find(const py::list<py::str>& types, const NodeP& under = nullptr) const;
    // The function's compound statement (its `{ ... }`), or null.
    NodeP body() const;
    // The identifier naming the function being defined, or null.
    NodeP function_name() const;
    // The function's own `( ... )` (of the function being defined, not of a pointer it returns).
    NodeP parameter_list() const;
    // Each parameter as written (stripped), or nullopt when there is no parameter list.
    std::optional<py::list<py::str>> parameters() const;
    // The outermost node of `kind` that starts where `node` starts (`node` itself, or the
    // first thing in it: the cast in `(T *)x + 2`), else null.
    NodeP leading(const NodeP& node, const py::str& kind) const;
    // A call_expression that is a call: not a cast the grammar read as one (`(uint4)(x)`,
    // `(uint2)(uint1)x`), nor a declaration with a type name in front (`uint2 (*p)`).
    bool is_call(const NodeP& node) const;
    // For a binary `*`, `&`, `-` or `+` whose left side is nothing but casts -- `(uint4)`,
    // `(uint2)(uint1)`, `(xunknown1 [4])` -- the operator of the unary expression the grammar
    // took for an operation on two values; otherwise nullopt.
    std::optional<py::str> misread(const NodeP& node) const;
    // (operator, where it starts) when `node` is the operand of a unary `*`, `&`, `-` or `+`
    // -- also one the grammar read as a binary operation after a cast (see misread) -- else nullopt.
    std::optional<std::tuple<py::str, int64_t>> unary(const NodeP& node) const;
    // The text of the line `node` starts on (without its line end).
    py::str line(const NodeP& node) const;
    // (start, end) of the line `node` starts on, its line end included.
    std::tuple<int64_t, int64_t> line_span(const NodeP& node) const;
    // The nodes `node` is inside, nearest first (only those of `types`, if given).
    py::list<NodeP> ancestors(const NodeP& node, const py::list<py::str>& types = py::list<py::str>()) const;
    // A call's arguments as written, each with the spaces and line breaks before it.
    py::list<py::str> raw_arguments(const NodeP& call) const;
    // The name a declarator declares (through pointers, arrays, an initialiser); nullopt when none.
    std::optional<py::str> declared_name(const NodeP& declarator) const;
    // `node`'s text with each node inside it for which override(n) gives text replaced by that
    // (override(node) itself is not asked).
    py::str render(const NodeP& node, const std::function<std::optional<py::str>(const NodeP&)>& override_) const;
    // Inside a string, a character constant or a comment: not code.
    static bool in_text(NodeP node);

    // -- writing ----------------------------------------------------------------------------------
    void replace(const NodeP& node, const py::str& text);
    void replace(const std::tuple<int64_t, int64_t>& span, const py::str& text);
    void insert(int64_t at, const py::str& text);
    // The text with the edits applied (ValueError when two overlap).
    py::str text() const;
};

// `fn(src)` records edits on the parsed text; the text with them applied.
py::str rewrite(const py::str& text, const std::function<void(Source&)>& fn);

}  // namespace codebreaker::decompile::ctree
