// codebreaker/port/arith16.hpp -- typed C++ of codebreaker.port.arith16.
//
// arith16.py -- the arithmetic of a 16-bit int kept, where the C of today would do it otherwise.
//
// On the 8086 an int was 16 bits, so an unsigned (16 bits) met by an int made the int unsigned: `u > -1` was false,
// `u - 1` with u 0 was 65535. The port keeps every int 16 bits wide (int16_t, uint16_t), which keeps what is stored -
// but the C of today makes both of them a 32-bit int before it compares, divides or widens them: `u > -1` is true,
// `(long)(u - 1)` is -1. Wherever that changes the answer this pass says, by a cast, what the 8086 did:
//
//   a comparison, / % or >> whose operands the 8086 made unsigned   each operand that is not so already: (uint16_t)
//   ... of 16-bit operands of which one is arithmetic (it may have      that operand: (int16_t)(...) or (uint16_t)(...)
//       passed 16 bits)
//   arithmetic of 16 bits made 32 (a long assigned, given, cast)    the arithmetic, cut to its 16 bits first
//
// Each expression's type is worked out by the 8086's rules from the tree clang makes of the port (clang -Xclang
// -ast-dump=json: every expression with its exact place, and the types as the source names them); the casts go in
// at those places, the rest of the text as it was.
//
// Typing notes (what the Python leaves open):
//   * clang's tree is a JSON document of dicts; this pass reads a fixed set of its keys, so a node is a record
//     (Node) with those keys, a source location a record (Loc), and dump() makes the records from the JSON. The
//     "_file" key mark_files() adds to each location is the Loc's `_file`.
//     The order mark_files() and _files() walk a node's values in is the order of clang's keys: loc, range
//     (begin, end), inner; and in a location: spellingLoc, expansionLoc, (its own keys), includedFrom.
//   * A node's "value" is a string (IntegerLiteral) or a number (CharacterLiteral): std::variant<int64_t, str>.
//   * MAIN and TEXT are one-element lists in the Python (module state edits_of() sets); here the element.
//
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>
#include <variant>

#include "py.hpp"
#include "value.hpp"   // (py::V: clang's JSON document)

namespace codebreaker::port::arith16 {

// A source location of clang's tree: {"offset", "tokLen", "file", "spellingLoc", "expansionLoc", "includedFrom"},
// and "_file" (the file it is in, which mark_files() puts to it).
struct Loc {
    std::optional<int64_t> offset;
    std::optional<int64_t> tokLen;
    std::optional<py::str> file;
    std::shared_ptr<Loc> spellingLoc, expansionLoc, includedFrom;
    std::optional<py::str> _file;
};

// {"begin": Loc, "end": Loc}  (a missing one is {})
struct Range {
    Loc begin, end;
};

// {"qualType": str}
struct TypeRef {
    std::optional<py::str> qualType;
};

// "value": a number as a string (IntegerLiteral) or as a number (CharacterLiteral)
using Number = std::variant<int64_t, py::str>;

// A node of the tree: {"kind", "opcode", "castKind", "type", "value", "loc", "range", "inner"}.
struct Node {
    std::optional<py::str> kind, opcode, castKind;
    std::optional<TypeRef> type;      // absent: the Python's n["type"] is a KeyError
    std::optional<Number> value;
    Loc loc;
    Range range;
    py::list<Node> inner;
};

// (the type name -> the 8086's i16 u16 i32 u32)
extern py::dict<py::str, py::str> W16;
extern py::set<py::str> ARITH;
extern py::set<py::str> TESTS;
extern py::dict<py::str, py::str> CAST;

// clang's JSON tree of a file of the port (-ast-dump=json), or None (too for a document that is not an object);
// node_of() reads it into records. (dump() is the two together; a pass that reads more keys of the document than
// Node holds -- oldlayout -- takes the JSON and makes its own records beside.)
std::optional<py::V> dump_json(const py::str& cc, const py::str& src, const py::str& folder, const py::list<py::str>& extra = py::list<py::str>());
Node node_of(const py::V& doc);

// clang's tree of a file of the port (the JSON of -ast-dump), or None
// (None too for a JSON document that is not an object.)
std::optional<Node> dump(const py::str& cc, const py::str& src, const py::str& folder, const py::list<py::str>& extra = py::list<py::str>());

// the type the 8086's compiler gave an expression: i16 u16 i32 u32 - or None (a pointer, a double, unknown)
std::optional<py::str> old_type(const Node& n, const py::str& text);

// the 8086's usual arithmetic conversions (int 16 bits, long 32)
std::optional<py::str> common(const std::optional<py::str>& a, const std::optional<py::str>& b);

// (start, end) offsets of an expression in the file - None when in a macro or another file
// -> (start, end, the file it is in)
std::optional<std::tuple<int64_t, int64_t, std::optional<py::str>>> place(const Node& n);

std::optional<py::str> spelled(const Node& n, const py::str& text);

extern std::optional<py::str> MAIN;
extern std::optional<py::str> TEXT;   // (the text of the file being read)

// clang names a location's file only where it changes: each location given its own (_file)
void mark_files(Node& node, std::optional<py::str>& cur);

const Node& bare(const Node& n);

// a constant of no sign: the same number whichever way it is converted
bool nonneg(const Node& n);

// an operand that is arithmetic of its own that may have gone past 16 bits (or, with & | ^, mixed signs),
// its parentheses passed. -> the node, or nullptr (None)
const Node* compound(const Node& n, const std::optional<py::str>& text = std::nullopt);

// [(offset, text to put there)] - the casts that keep the 8086's arithmetic
py::list<std::tuple<int64_t, py::str>> edits_of(const Node& tree, const py::str& text, const py::str& path);

bool simple(const Node& n, const py::str& text);

py::str apply(const py::str& text, const py::list<std::tuple<int64_t, py::str>>& edits);

// what every file of the port includes: its headers' texts (one changed: every file's result again)
py::str _headers_key(const py::str& folder);

// the file and every header of the port's it reaches (an unchanged header's file not read again)
py::str reach_key(const py::str& folder, const py::str& f);

// the files a tree names, in the order they are met
py::list<py::str> _files(const Node& node, const std::optional<py::list<py::str>>& seen = std::nullopt);

// The version of the code that makes each kind of result -- for THIS C++ port. The Python hashes the source text
// of its own functions (ast.parse of arith16.py and oldlayout.py), which a C++ program cannot do, so the key is a
// fixed, documented version string: BUMP CODE_VERSION WHENEVER THE LOGIC OF A PASS CHANGES (dump, old_type,
// edits_of, apply and what they use), as editing the Python would change the Python's key. The entries this port
// keeps in the cache therefore live under different keys than the Python's (the values have the same form:
// {"new": the text with the casts, or None}), and neither reads the other's.
extern const py::str CODE_VERSION;

// the code a kind of result comes from, by CODE_VERSION (a kind not known: the same, as the Python hashes both passes)
py::str code_key(const py::str& kind);

// a pass's result for one file, by all it depends on: the file and the headers it reaches, the compiler, the
// code of the pass that result comes from. `extra` are the Python's *extra (strs here); `headers` is no longer
// used there either (every header of the port).
py::str pass_key(const py::str& cc, const py::str& folder, const py::str& f, const py::str& kind,
                 const py::list<py::str>& extra = py::list<py::str>(), const std::optional<py::str>& headers = std::nullopt);

// {file: its text with the casts} for the port's C files (those clang could read) - a file's result kept,
// by its text and its headers': the same file again, not read again (cache kind "port-passes")
py::dict<py::str, py::str> run(const py::str& cc, const py::str& folder, const py::list<py::str>& files);

// (KINDS, _CODE and the Python's per-function key are not kept: see CODE_VERSION.)
}  // namespace codebreaker::port::arith16
