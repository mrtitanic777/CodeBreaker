// codebreaker/translate/l2.hpp -- typed C++ port of codebreaker.translate.l2.
//
// l2.py -- the IR as L2 JSON, the document format every producer and consumer shares.
//
// L2 is defined by codebreaker/translate/l2.schema.json (docs/IR_UNIFICATION.md: one IR family, three
// levels; L2 is the typed hub tree). ir.py is the Python implementation; the decompiler's PrintIR and the
// Rust core produce and read the same JSON. A document is one Node, usually a Module or a Unit:
//
//     {"node": "Binary", "line": 7, "col": 16, "op": ">>>", "type": {"kind": "i32"},
//      "left": {...}, "right": {...}}
//
//     to_json(node) -> dict          from_json(dict) -> node          validate(dict) (needs jsonschema)
//
// Typing notes (how the Python's values are typed here):
//   * A JSON document is a Dict (py::dict<py::str, Val>) of Vals: the ir.hpp Val is exactly what a document
//     holds -- None, bool, int, float, str, a list, a dict. (A Val may also hold a Type, which only an IR
//     tree's Lit.value or Type.info can: to_json turns the ones in lists of a slot into their L2 form, as the
//     Python's _value does, and leaves the ones in dicts as they are.)
//   * type_json / sym_json return a Val (a Dict, or None for None); the others return a Dict.
//   * to_json reads the typed slots of each node class through the class's each_slot(); from_json builds the
//     class named by d['node'] and gives each slot the kind of value the class types it with. Where the
//     Python stores a document value of another kind than its slot's (a dict without 'node' in a node slot,
//     a list of raw dicts where a node list has a None in it), the slot's type cannot hold it: that is a
//     TypeError here, and a list of nodes keeps None as None.
//   * Python's `**fields` / `**facts` keyword dicts are a Dict argument. A dict subclass with a write-through
//     (Made.texts' _Texts) is a copy here, with set_text() / update_texts() for the writes. Made's cached
//     views (_view) are not kept: each view is made from the document when asked for (the values are the same).
//     Made's pickling and copy hooks go with them (copying a Made shares its doc, which is never changed in
//     place, as the Python's __copy__ does).
//   * Identifier spellings that are C++ keywords: the `asm` argument of fragment() is asm_.
//   * Python ints are int64_t here.
#pragma once

#include <memory>
#include <optional>
#include <tuple>

#include "codebreaker/translate/ir.hpp"
#include "py.hpp"

namespace codebreaker::translate::l2 {

namespace ir = codebreaker::translate::ir;
using ir::Dict;
using ir::NodeP;
using ir::SymP;
using ir::TypeP;
using ir::Val;

// jsonschema.ValidationError, as validate() raises it
struct ValidationError : py::Exception {
    ValidationError() : py::Exception() {}
    explicit ValidationError(const std::string& m) : py::Exception(m) {}
    const char* type() const override { return "ValidationError"; }
};

// The path of l2.schema.json (the Python's: beside l2.py -- here, from this source file's place in the
// tree: <repo>/codebreaker/translate/l2.schema.json). A variable: a caller with the schema elsewhere sets it.
extern py::str SCHEMA;

// ---------------------------------------------------------------------------------------------- types
// what the Python front end says of a class type (frontends/python/lower.py): a record (a value struct,
// or shared when `ref`), held by reference or not, an exception (made and thrown as a value)
extern const py::list<py::str> CLASS_FACTS_;

// a Type as its L2 dict (None for None). ValueError for a kind with no L2 form.
Val type_json(const TypeP& t);
// the Type of an L2 type dict (None for None). KeyError for a missing key, ValueError for an unknown kind.
TypeP type_from(const Val& d);

// ---------------------------------------------------------------------------------------------- symbols
Val sym_json(const SymP& s);
SymP sym_from(const Val& d);

// ---------------------------------------------------------------------------------------------- nodes
// a node as its L2 dict (AttributeError for None, as the Python's node.line is)
Dict to_json(const NodeP& node);
// the node an L2 dict describes; KeyError for a dict with no 'node' or an unknown one
NodeP from_json(const Dict& d);
NodeP from_json(const Val& d);       // (TypeError where the Python would fail on a value that is not a dict)

// the parsed l2.schema.json
Val schema();
// Raise ValidationError if `doc` is not L2. (The Python hands the schema to jsonschema; here the native L2
// core's validator, native/l2/src/l2.cc, which is written to decide as l2.validate does.)
void validate(const Dict& doc);

// ------------------------------------------------------------------------------------ the module model
// A module, of any lane and whatever each part of it came back as, is one L2 Module document: its units
// in order -- a function rebuilt as a tree (a MethodDecl with a body), one whose source is still text
// (a MethodDecl with `text`), what stays machine code (a MachineFragment: its assembly, or only its name
// and place), a comment at its place. Every lane's module is this one model (docs/ARCHITECTURE.md, "one
// Module model"): the decompiler's (build/module.module_document), the readable pass's
// (source/readable.Readable), the 16-bit port's (port/asm16.Port), a library's members
// (exact/library.Member), and every assembly writer's (l2.Assembly, written by backends/asm.py).

// A Module document. -> {"node": "Module", "units": [...]}
Dict module(const py::list<Val>& units = py::list<Val>(), const std::optional<py::str>& header = std::nullopt,
            const std::optional<Dict>& origin = std::nullopt);

// A function of a module: a MethodDecl (kind "function"), its source as `text` while it is not a tree.
// `fields` are the Python's **fields (they override what is set before them).
Dict function(const py::str& name, const std::optional<py::str>& text = std::nullopt,
              const std::optional<Dict>& origin = std::nullopt,
              const std::optional<py::list<py::str>>& comments = std::nullopt, const Dict& fields = Dict());

// What stays machine code: a MachineFragment (L2_BINARY_TYPES.md section 7).
Dict fragment(const std::optional<py::str>& asm_ = std::nullopt, const std::optional<py::str>& name = std::nullopt,
              const std::optional<py::str>& syntax = std::nullopt, const std::optional<Dict>& origin = std::nullopt,
              const std::optional<py::str>& reason = std::nullopt, const std::optional<py::str>& why = std::nullopt,
              const std::optional<int64_t>& length = std::nullopt, const std::optional<py::str>& within = std::nullopt);

Dict comment(const py::str& text);

// The units of a Module document (those of one kind, given `node`). AttributeError for a unit that is not
// a dict, as the Python's u.get is.
py::list<Dict> units(const Dict& doc, const std::optional<py::str>& node = std::nullopt);

// {name: text} of a module's functions whose source is text, in their order.
py::dict<py::str, py::str> texts(const Dict& doc);

// A module made compilable for a lane's build (decompile/c_text: compilable16, compilable32,
// compilable_powerpc), as the one module model: an L2 Module document, `doc` -- each function a
// MethodDecl with its C as `text` and its place as `origin` ({module, va}), in the module's
// order, the header the Module's -- and, beside it, what the lane keeps
// of the module as attributes (16-bit: near, wide, novalue, interrupts, selector_imports, ...;
// 32-bit: imports, in_ecx, in_edx; PowerPC: externs, argc, image) -- `facts` here.
//
//     Made m("lzexpand.dll", header, texts, order, by_va, facts);
//     m.texts() / m.order() / m.by_va() / m.header() / m.functions()      views of m.doc
//     m.set_text(name, text), m.update_texts({...}), m.set_texts({...}),
//     m.set_header(...)                                                    rewrite m.doc
//     m.add(name, text, va)                                               one more function, last
//
// (A change makes a new document rather than changing the one there: a copy of m and m share
// it until one of them changes.)
struct Made {
    py::str name;
    Dict doc;
    Dict facts;         // what the Python keeps as attributes of the object beside name and doc

    Made(const py::str& name = py::str(""), const py::str& header = py::str(""),
         const std::optional<py::dict<py::str, py::str>>& texts = std::nullopt,
         const std::optional<py::list<py::str>>& order = std::nullopt,
         const std::optional<py::dict<int64_t, py::str>>& by_va = std::nullopt, const Dict& facts = Dict());

    void add(const py::str& name, const std::optional<py::str>& text, const std::optional<int64_t>& va = std::nullopt);

    py::list<Dict> _functions() const;

    // `made.texts`: {name: C text}, in the module's order (a copy; write with set_text / update_texts)
    py::dict<py::str, py::str> texts() const;
    // `made.texts = new`
    void set_texts(const py::dict<py::str, py::str>& new_texts);
    // `made.texts[name] = text`
    void set_text(const py::str& name, const py::str& text);
    // `made.texts.update(new)`
    void update_texts(const py::dict<py::str, py::str>& new_texts);

    py::str header() const;
    void set_header(const py::str& text);

    py::list<py::str> order() const;
    py::dict<int64_t, py::str> by_va() const;
    // {address: (name, text)}
    py::dict<int64_t, std::tuple<py::str, std::optional<py::str>>> functions() const;
};

// A module being written as assembly language, built as its L2 Module as it goes.
//
//     Assembly L(lines, "nasm", "lzexpand.dll", ...);
//     L.append(...); L.extend(...)                 // lines, as to a list (every writer's helpers take one)
//     L.part("seg1", "section", origin);           // the next fragment begins here
//     Dict doc = L.document();                     // a Module of MachineFragments: backends/asm.py writes it
//
// Each part() begins a MachineFragment at the next line; the lines before the first part are the
// head's. A part with no lines is left out, so the fragments joined by a line break are the lines
// joined, as they were written.
struct Assembly : py::list<py::str> {
    std::optional<py::str> syntax;
    std::optional<py::str> module;
    // (start line, name, reason, origin, length) of each part
    std::vector<std::tuple<int64_t, py::str, py::str, std::optional<Dict>, std::optional<int64_t>>> _parts;

    Assembly(const py::list<py::str>& lines = py::list<py::str>(), const std::optional<py::str>& syntax = std::nullopt,
             const std::optional<py::str>& module = std::nullopt, const py::str& name = py::str("head"),
             const py::str& reason = py::str("head"));

    void part(const py::str& name, const py::str& reason = py::str("section"), const std::optional<Dict>& origin = std::nullopt,
              const std::optional<int64_t>& length = std::nullopt);
    Dict document() const;
};

}  // namespace codebreaker::translate::l2
