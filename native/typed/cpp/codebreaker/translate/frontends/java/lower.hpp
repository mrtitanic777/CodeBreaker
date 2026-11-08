// codebreaker/translate/frontends/java/lower.hpp -- typed C++ port of codebreaker.translate.frontends.java.lower.
//
// lower.py -- javac's typed tree (TreeDump's JSON) into the translator's IR (codebreaker/translate/ir.py).
//
// Every name is lowered from the symbol javac resolved, not from its spelling: an IDENTIFIER that javac
// bound to a field becomes a Select on This (or on its class, when static), one bound to a local a Name;
// a MEMBER_SELECT bound to a class is a TypeName, `x.length` on an array a Length, `X.class` a ClassLit.
// Constructor calls this(...)/super(...) are CtorCall statements; the ones javac inserted (a constructor
// with no explicit call, a class with no constructor) are marked implicit.
//
// Anything not yet handled becomes ir.Unsupported(kind), never a guess.
//
// Typing notes (how the Python's values are typed here):
//   * A node of TreeDump's JSON is a Dict (py::dict<py::str, Val>) of Vals -- the ir.hpp Val is exactly what
//     JSON holds (None, bool, int, float, str, a list, a dict). A "$sym" record is a Val too (None, or the
//     dict). load_tree() reads a dump's text into a Dict with py::json_parse. Where the Python tests a child
//     node for truth (`if n.get('initializer')`), the C++ reads the child with opt_node(), an empty Dict
//     for None (an empty dict is false in Python too).
//   * Python's `dict(line=..., col=...)` records of self.pos(n) are the struct Pos; `**self.pos(n)` sets
//     a node's line and col.
//   * A Python private name keeps its meaning with a trailing underscore: _type -> type_, _PRIM, _LOCALS,
//     _TYPES, _KIND, _STRING, _OBJECT, _HASH_OWNER stay in the .cpp; the method `case` is case_ (a C++
//     keyword).
//   * parse_type(s, typevars): s is a str or None; typevars any iterable of str (a py::list<py::str>).
//   * A javac literal's value that Python reads with float() / int() is read as such: a number or a numeric
//     string (the JSON writes Double, Float, Char as strings, Integer, Long, Boolean as numbers).
//   * The Python stores whatever the JSON holds in a slot (a ClassDecl.name, a Break.label ...); the slots
//     are typed here (str or None); a JSON value of another kind raises TypeError instead of being stored.
//   * Python ints are int64_t here.
#pragma once

#include <optional>
#include <tuple>

#include "codebreaker/translate/ir.hpp"
#include "py.hpp"

namespace codebreaker::translate::frontends::java::lower {

namespace ir = codebreaker::translate::ir;
using ir::Dict;
using ir::NodeP;
using ir::SymP;
using ir::TypeP;
using ir::Val;

// -------------------------------------------------------------------------------------- type strings
// A javac type string ('int[][]', 'java.util.Map<K,java.util.List<V>>', '<nulltype>') -> ir.Type.
TypeP parse_type(const std::optional<py::str>& s, const py::list<py::str>& typevars = py::list<py::str>());

// (the type starting at s[i], and where it ends)
std::tuple<TypeP, int64_t> type_(const py::str& s, int64_t i, const py::set<py::str>& tv);

// -------------------------------------------------------------------------------------- symbols
// TreeDump's "$sym" record -> ir.Sym (None for None or an empty record).
SymP sym(const Val& d);

// -------------------------------------------------------------------------------------- the JSON
// A dump's text (one compilation unit's JSON) as a node Dict. ValueError for text that is not JSON.
Dict load_tree(const py::str& text);

// -------------------------------------------------------------------------------------- the lowering
// self.pos(n): dict(line=, col=)
struct Pos {
    int64_t line = 0;
    int64_t col = 0;
};

class Lowering {
  public:
    std::optional<py::str> path;

    explicit Lowering(const std::optional<py::str>& path = std::nullopt) : path(path) {}

    Pos pos(const Dict& n) const;
    TypeP ty(const Dict& n) const;

    // ---- declarations
    py::ref<ir::Unit> unit(const Dict& n);
    py::str qualified(const Dict& n);                       // (an empty Dict for None)
    py::list<py::str> mods(const Dict& n);                  // (the modifiers node, an empty Dict for None)
    py::ref<ir::ClassDecl> decl(const Dict& n, const std::optional<py::str>& outer);
    NodeP member(const Dict& m, const py::ref<ir::ClassDecl>& cls, const Dict& raw_cls);
    py::ref<ir::MethodDecl> method(const Dict& m, const Dict& raw_cls);

    // ---- statements
    py::ref<ir::Block> block(const Dict& n, bool ctor_body = false);
    NodeP stmt(const Dict& n);
    py::ref<ir::Case> case_(const Dict& c, bool in_expr = false);

    // ---- expressions
    NodeP expr(const Dict& n);
    py::ref<ir::Lit> literal(const Dict& n, const TypeP& t, const Pos& p);
    NodeP identifier(const Dict& n, const TypeP& t, const Pos& p);
    NodeP member_select(const Dict& n, const TypeP& t, const Pos& p);
    NodeP call(const Dict& n, const TypeP& t, const Pos& p);
};

// -------------------------------------------------------------------------------------- records
// Add a record's implicit members to ClassDecl `c` (those it does not declare itself).
void record_members(const py::ref<ir::ClassDecl>& c);

// One compilation unit's TreeDump JSON -> ir.Unit.
py::ref<ir::Unit> lower(const Dict& tree, const std::optional<py::str>& path = std::nullopt);

}  // namespace codebreaker::translate::frontends::java::lower
