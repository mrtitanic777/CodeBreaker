// cb/decompile/ctree.hpp -- the decompiler's C as a syntax tree, for the passes that rewrite it.
//
// A pass that finds what to change with a regular expression over the text also finds it inside a string or a
// comment, takes `a.b._1_2_` for a piece of `b`, and cannot tell a declaration from an expression that looks like
// one. Here the text is parsed (tree-sitter's C grammar, which goes on past what it cannot parse -- a calling
// convention it does not know, a type never declared -- and keeps every byte's place), a pass finds the nodes it
// means, and replaces exactly their text: everything else stays as it was, byte for byte.
//
//     Source src(text, parser);
//     for (const Node* node : src.find({"field_expression"})) {
//         ...
//         src.replace(*node, "new text");
//     }
//     std::string out = src.text();      // the edits applied (they must not overlap)
//
// THE PARSER IS AN INTERFACE. tree-sitter (its runtime and the C grammar) is not part of this tree, so a `Parser`
// is handed to Source: a function from the bytes to a `Tree` of `Node`s carrying what tree-sitter's nodes carry
// (type, byte range, named, parent, children, the child each field name gives). A binding to the tree-sitter C
// library -- or a replayed recording, which is what the tests use -- supplies it. Everything else here is the
// module's own: the masking of the 16-bit qualifiers and end-of-block labels before the parse, the cast patterns,
// every query and the edit machinery.
//
// Nodes are tree-sitter's: `type`, `children`, `named_children()`, `child_by_field_name()`, `start_byte`,
// `end_byte`, `parent`. `src.at(node)` is a node's text as written (with its `__far`: see mask_for_parser).
#pragma once

#include <cstdint>
#include <deque>
#include <functional>
#include <initializer_list>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "cb/core/bytes.hpp"

namespace cb::decompile::ctree {

// A syntax-tree node. Owned by the Tree it was added to; pointers to it stay valid as long as that Tree lives.
struct Node {
    std::string type;                         // tree-sitter's node-type name ("call_expression", "(", "ERROR")
    std::uint32_t start_byte = 0;
    std::uint32_t end_byte = 0;
    bool is_named = false;
    const Node* parent = nullptr;             // null for the root
    std::vector<const Node*> children;        // every child, anonymous tokens included
    // What child_by_field_name() answers, field name -> child, as the tree-sitter library gives it. (Its answer for
    // an ERROR node is not the field names its children carry, so it is recorded as the library gives it.)
    std::vector<std::pair<std::string, const Node*>> fields;

    std::size_t child_count() const { return children.size(); }
    std::vector<const Node*> named_children() const;
    // The child in that field, or null.
    const Node* child_by_field_name(std::string_view name) const;
};

// The nodes of one parse. Nodes are added parents-first (or linked afterwards) and never move.
class Tree {
public:
    Tree() = default;
    Tree(Tree&&) noexcept = default;              // (the nodes keep their addresses: a deque moves its blocks)
    Tree& operator=(Tree&&) noexcept = default;
    Tree(const Tree&) = delete;                   // (a copy's nodes would point into the original)
    Tree& operator=(const Tree&) = delete;
    // Adds a node; with a parent it becomes that parent's next child. The first parentless node is the root.
    Node& add(std::string type, std::uint32_t start_byte, std::uint32_t end_byte, bool is_named, Node* parent = nullptr);
    // Records that `parent.child_by_field_name(field)` is `child`.
    void set_field(Node& parent, std::string field, const Node& child);
    // The root node; the tree must not be empty.
    const Node& root() const;
    bool empty() const { return nodes_.empty(); }

private:
    std::deque<Node> nodes_;
};

// Parses the (masked) C text into a tree.
using Parser = std::function<Tree(ByteView masked)>;

// The text as the parser sees it. The grammar has no word for `__far` (it took `(char __far *)` for a cast to a
// type named __far), so the 16-bit qualifiers and the calling conventions are blanked, byte for byte (`__far`,
// `__near`, `__huge`, `__export`, `__loadds`, `__saveregs`, `__interrupt`, `__cdecl*`, `__stdcall*`, `__pascal*`,
// `__thiscall`, `__fastcall`, `_Packed` as whole words); and a label at the end of a block (`L:` then `}`) is not C
// -- a label needs a statement -- and the grammar's recovery took what came after it for something else, so it is
// read as `L;`. Every place in the tree is still a place in the text as written.
std::string mask_for_parser(std::string_view text);

class Source {
public:
    // Parses `text` with `parser` (the text is kept as given; the parser sees mask_for_parser(text)).
    Source(std::string_view text, const Parser& parser);
    Source(const Source&) = delete;
    Source& operator=(const Source&) = delete;

    const std::string& original() const { return data_; }   // (the text as given; edits are applied by text())
    const Node& root() const { return tree_.root(); }

    // -- reading ----------------------------------------------------------------------------------
    // A node's text as written.
    std::string_view at(const Node& node) const;
    // Every node of these types, in the order they appear (outermost first); under = null for the whole tree.
    std::vector<const Node*> find(std::span<const std::string_view> types, const Node* under = nullptr) const;
    std::vector<const Node*> find(std::initializer_list<std::string_view> types, const Node* under = nullptr) const;
    // The function's compound statement (its `{ ... }`), or null.
    const Node* body() const;
    // The identifier naming the function being defined, or null.
    const Node* function_name() const;
    // The function's own `( ... )` (of the function being defined, not of a pointer it returns), or null.
    const Node* parameter_list() const;
    // Each parameter as written (stripped), or nullopt when there is no parameter list.
    std::optional<std::vector<std::string>> parameters() const;
    // The outermost node of `kind` that starts where `node` starts (`node` itself, or the first thing in it: the
    // cast in `(T *)x + 2`), else null.
    const Node* leading(const Node& node, std::string_view kind) const;
    // A call_expression that is a call: not a cast the grammar read as one (`(uint4)(x)`, `(uint2)(uint1)x`), nor a
    // declaration with a type name in front (`uint2 (*p)`).
    bool is_call(const Node& call) const;
    // For a binary `*`, `&`, `-` or `+` whose left side is nothing but casts -- `(uint4)`, `(uint2)(uint1)`,
    // `(xunknown1 [4])` -- the operator of the unary expression the grammar took for an operation on two values;
    // otherwise nullopt.
    std::optional<std::string_view> misread(const Node& node) const;
    struct UnaryOp {
        std::string_view op;           // "*", "&", "-" or "+"
        std::uint32_t start = 0;       // where the operator starts
        friend bool operator==(const UnaryOp&, const UnaryOp&) = default;
    };
    // The operator, and where it starts, when `node` is the operand of a unary `*`, `&`, `-` or `+` -- also one the
    // grammar read as a binary operation after a cast (see misread) -- else nullopt.
    std::optional<UnaryOp> unary(const Node& node) const;
    // The text of the line `node` starts on (without its line end).
    std::string_view line(const Node& node) const;
    struct Span {
        std::uint32_t start = 0;
        std::uint32_t end = 0;
    };
    // The line `node` starts on, its line end included.
    Span line_span(const Node& node) const;
    // The nodes `node` is inside, nearest first (only those of `types`, if given).
    std::vector<const Node*> ancestors(const Node& node, std::span<const std::string_view> types = {}) const;
    std::vector<const Node*> ancestors(const Node& node, std::initializer_list<std::string_view> types) const;
    // A call's arguments as written, each with the spaces and line breaks before it.
    std::vector<std::string_view> raw_arguments(const Node& call) const;
    // The name a declarator declares (through pointers, arrays, an initialiser); nullopt when none.
    std::optional<std::string_view> declared_name(const Node& declarator) const;
    // `node`'s text with each node inside it for which `override_` gives text replaced by that (it is not asked
    // about `node` itself).
    std::string render(const Node& node, const std::function<std::optional<std::string>(const Node&)>& override_) const;
    // Inside a string, a character constant or a comment: not code.
    static bool in_text(const Node& node);

    // -- writing ----------------------------------------------------------------------------------
    // Replace a node's text, or a byte span's. A span past the end of the text is a UsageError.
    void replace(const Node& node, std::string text);
    void replace(Span span, std::string text);
    void insert(std::uint32_t at, std::string text);
    std::size_t edit_count() const { return edits_.size(); }
    // The text with the edits applied; edits that overlap are a UsageError (the pass is wrong, not the input).
    std::string text() const;

private:
    struct Edit {
        std::uint32_t start;
        std::uint32_t end;
        std::string text;
    };
    std::string_view slice(std::uint32_t start, std::uint32_t end) const;

    std::string data_;
    Tree tree_;
    std::vector<Edit> edits_;
};

// `fn(src)` records edits on the parsed text; the text with them applied.
std::string rewrite(std::string_view text, const Parser& parser, const std::function<void(Source&)>& fn);

}  // namespace cb::decompile::ctree
