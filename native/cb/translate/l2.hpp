// cb/translate/l2.hpp -- L2, the document form of the translator's tree, and the one module model every lane shares.
//
// L2 is the JSON every producer and consumer of the IR exchanges (the schema is codebreaker/translate/l2.schema.json,
// docs/IR_UNIFICATION.md): one node is an object with a "node" member naming its kind, its line and column, and one
// member per slot. This file reads and writes that form over the typed tree of cb/translate/ir.hpp -- the edge of the
// program; between the edges the tree is typed nodes.
//
//     Arena arena;
//     ir::Node* unit = l2::from_json(arena, json::parse(text));     // FormatError for a document that is not L2
//     Json doc = l2::to_json(*unit);
//
// Reading is lenient where the documents are: a slot that is absent is empty; a key the node does not have is ignored
// (a newer producer may write more); a body slot may hold a statement or a list of them. Writing keeps what the source
// said -- a list slot the walk follows is written even when empty (`"stmts": []`), so a consumer that iterates it never
// meets null. A symbol is written whole at every use and read as one new symbol per use: sharing is the tree's, not the
// document's. A float literal that is not finite (NaN, infinity) is written as its name ("nan", "inf", "-inf"), as JSON
// has no form for it, and reads back as that text.
//
// The module model (below) is the one tree a module of any lane is made of: its units in order -- a function rebuilt as
// a tree (a MethodDecl with a body), one whose source is still text (a MethodDecl with `text`), what stays machine code
// (a MachineFragment: its assembly, or only its name and place), a comment at its place. The decompiler's modules, the
// readable pass's, the 16-bit port's, a library's members and every assembly writer's are this one model.
#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/arena.hpp"
#include "cb/core/error.hpp"
#include "cb/core/json_fwd.hpp"
#include "cb/core/ordered_map.hpp"
#include "cb/translate/ir.hpp"

namespace cb::translate::l2 {

// ---- the document edge ----------------------------------------------------------------------------------------------

// A type as its L2 object (null for an absent type). FormatError never: every type has a form.
Json type_to_json(const ir::TypeP& type);
// The type an L2 object describes (an absent type for null). FormatError for an unknown kind or a missing member.
ir::TypeP type_from_json(const Json& doc);

Json sym_to_json(const ir::SymP& sym);
ir::SymP sym_from_json(const Json& doc);

// A node and everything under it as its L2 document.
Json to_json(const ir::Node& node);
// The tree an L2 document describes, its nodes made in `arena`. FormatError for a document that is not an object with a
// known "node" kind, or a slot that holds the wrong kind of value.
ir::Node* from_json(Arena& arena, const Json& doc);

// ---- the module model -----------------------------------------------------------------------------------------------

// A Module of `units` (and the declarations they share as text, where those are not a tree yet).
ir::Node* module_node(Arena& arena, std::span<ir::Node* const> units = {}, std::optional<std::string> header = std::nullopt);

// A function of a module: a MethodDecl of kind "function", its source as `text` while it is not a tree.
struct FunctionSpec {
    std::string name;
    std::optional<std::string> text;
    std::optional<ir::Origin> origin;   // where in the binary it came from
    std::vector<std::string> comments;  // the text of each comment written above it
};
ir::Node* function_node(Arena& arena, const FunctionSpec& spec);

// What stays machine code: a MachineFragment (docs/L2_BINARY_TYPES.md section 7).
ir::Node* fragment_node(Arena& arena, ir::MachineFragment fragment, std::optional<ir::Origin> origin = std::nullopt);

ir::Node* comment_node(Arena& arena, std::string text);

// The units of a Module, all of them or those of one kind (`units_of<ir::MethodDecl>(m)`). A node that is not a Module has none.
std::vector<ir::Node*> units(const ir::Node* module);
template <class Kind>
std::vector<ir::Node*> units_of(const ir::Node* module) {
    std::vector<ir::Node*> out;
    for (ir::Node* u : units(module)) {
        if (ir::is<Kind>(u)) out.push_back(u);
    }
    return out;
}

// {name: text} of a module's functions whose source is text, in their order. (A name met twice keeps its first place and
// takes the last text.)
OrderedMap<std::string, std::string> texts(const ir::Node* module);

// A module made compilable for a lane's build, as the one module model: each function a MethodDecl with its C as `text`
// and its place as `origin` ({module, va}), in the module's order, the header the Module's. The lane keeps what else it
// knows of the module (near, wide, imports, ...) in its own type beside this one.
//
//     Made m = Made::of("lzexpand.dll", header, texts, order, by_va);
//     m.texts() / m.order() / m.by_va() / m.header() / m.functions()      views of the module
//     m.set_text(name, text), m.update_texts(texts), m.set_header(text), m.add(name, text, va)
//     ir::Node* doc = m.document(arena);                                  the Module tree
//
// It is a value: a copy is another module.
struct MadeFunction {
    std::string name;
    std::optional<std::string> text;
    std::optional<std::uint64_t> va;   // its place in the module's binary
};

class Made {
public:
    Made() = default;
    Made(std::string name, std::string header) : name_(std::move(name)), header_(std::move(header)) {}

    // The functions named in `order` first (a name with no text has none), then those of `texts` that `order` does not
    // name; `by_va` (address to name) gives each its place. `order` empty: the order of `texts`.
    static Made of(std::string name, std::string header, const OrderedMap<std::string, std::string>& texts,
                   std::span<const std::string> order = {}, const OrderedMap<std::uint64_t, std::string>& by_va = {});

    // One more function, last.
    void add(std::string name, std::optional<std::string> text, std::optional<std::uint64_t> va = std::nullopt);

    const std::string& name() const { return name_; }
    const std::string& header() const { return header_; }
    void set_header(std::string text) { header_ = std::move(text); }
    const std::vector<MadeFunction>& all() const { return functions_; }

    // {name: C text} of the functions that have text, in the module's order.
    OrderedMap<std::string, std::string> texts() const;
    // Set these functions' texts (a name the module has keeps its place; one it has not is added last).
    void update_texts(const OrderedMap<std::string, std::string>& new_texts);
    void set_text(const std::string& name, std::string text);
    // The names of the functions, in order.
    std::vector<std::string> order() const;
    // {address: name} of the functions that have a place, in the module's order.
    OrderedMap<std::uint64_t, std::string> by_va() const;
    // {address: (name, text)} of the functions that have a place.
    OrderedMap<std::uint64_t, std::pair<std::string, std::optional<std::string>>> functions() const;

    // The Module tree.
    ir::Node* document(Arena& arena) const;

private:
    std::string name_;
    std::string header_;
    std::vector<MadeFunction> functions_;
};

// A module being written as assembly language, built as its Module as it goes:
//
//     Assembly out({"; head ..."}, "nasm", "lzexpand.dll");
//     out.append(line); out.part("seg1", "section", origin);     // the next fragment begins here
//     ir::Node* doc = out.document(arena);                      // a Module of MachineFragments
//
// Each part() begins a MachineFragment at the next line; the lines before the first part are the head's. A part with no
// lines is left out, so the fragments joined by a line break are the lines joined, as they were written.
class Assembly {
public:
    explicit Assembly(std::vector<std::string> lines = {}, std::optional<std::string> syntax = std::nullopt,
                      std::optional<std::string> module = std::nullopt, std::string name = "head", std::string reason = "head");

    void append(std::string line) { lines_.push_back(std::move(line)); }
    void extend(std::span<const std::string> lines) { lines_.insert(lines_.end(), lines.begin(), lines.end()); }
    const std::vector<std::string>& lines() const { return lines_; }
    const std::optional<std::string>& syntax() const { return syntax_; }
    const std::optional<std::string>& module() const { return module_; }

    void part(std::string name, std::string reason = "section", std::optional<ir::Origin> origin = std::nullopt,
              std::optional<std::uint64_t> length = std::nullopt);

    ir::Node* document(Arena& arena) const;

private:
    struct Part {
        std::size_t start = 0;   // the first line of the fragment
        std::string name;
        std::string reason;
        std::optional<ir::Origin> origin;
        std::optional<std::uint64_t> length;
    };
    std::vector<std::string> lines_;
    std::optional<std::string> syntax_;
    std::optional<std::string> module_;
    std::vector<Part> parts_;
};

}  // namespace cb::translate::l2
