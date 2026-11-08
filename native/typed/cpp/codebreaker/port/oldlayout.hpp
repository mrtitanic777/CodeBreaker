// codebreaker/port/oldlayout.hpp -- typed C++ port of codebreaker.port.oldlayout.
//
// oldlayout.py -- the records a program keeps as bytes, kept in their old layout.
//
// A compiler of the 8086 writes its records to its own files as they lie in memory: `p = (char *)&e->e_v`, then the
// bytes, so many of them; it reads them back the same way and goes on. On the 8086 a pointer in such a record was two
// bytes, and the bytes of one member were the bytes of another (a union: a long here, two pointers there). Laid out
// anew - pointers of eight bytes - the counts no longer fit and the overlaps are gone.
//
// So those records keep the layout they had: the program's own ints 16 bits, its longs 32, its pointers two bytes - a
// handle (dos_ph) that stands for the pointer, turned back into it (dos_hp) wherever the program reads the member. A
// record is one of them when the program takes it as bytes - `(char *)&x`, `(char *)&x.member`, `(char *)p` of a pointer
// to it. Every read and write of a pointer member is found in clang's tree of the port (-ast-dump=json, as arith16
// does) and given the conversion:
//
//   r = e->e_sym          r = ((struct sym *)dos_hp(e->e_sym))
//   e->e_sym = s          e->e_sym = dos_ph(s)
//
// and the members are declared uint16_t in the record. A value of the program's that is no address - a symbol's
// number put where its pointer was - stays the number (dos_ph keeps what is below 8000h as it is).
//
// Typing notes (what the Python leaves open):
//   * clang's tree is a JSON document of dicts. arith16's typed port reads the keys arith16 needs into
//     arith16::Node; this pass reads a few more ("id", "name", "completeDefinition", "tagUsed", "referencedDecl"
//     {"name"}, "referencedMemberDecl"), which arith16::Node does not hold and whose parser (node_of) is not
//     exported. So a tree here is a Tree: the arith16::Node tree itself (so arith16::bare, mark_files, _files and
//     the Loc records are used as they are) plus an Extra record per node, found by the node's address
//     (Tree::ex). _dump() runs clang as arith16::dump does and builds both from the one JSON document; the node
//     and location parsers are small copies of arith16's (the only part not shared).
//   * a dict with fixed keys is a record: Extra; the Python's `{"new": ..., "found": ..., "refused": ...}` /
//     `{"olds": ...}` cache values are a tuple / a list (cache::value / keep are typed), and their cache keys
//     carry this port's own version (the entries are not the Python's: see CODE_VERSION).
//   * _walk() hands the callback the chain of ancestors as a std::vector<const Node*> (the Python's tuple).
//   * An absent child ("(n.get('inner') or [{}])[0]") is arith16's empty node; "x is n" is the address of a node.
//   * PTR_PARAMS is {function: [bool per parameter]}.
//
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "py.hpp"
#include "py_os.hpp"

#include "codebreaker/port/arith16.hpp"

namespace codebreaker::port::oldlayout {

using Node = arith16::Node;

// The keys of a node that arith16::Node does not hold.
struct Extra {
    std::optional<py::str> id;
    py::str name;                       // ("name", or "" when absent)
    py::str tagUsed = py::str("struct");  // ("tagUsed", default "struct")
    bool completeDefinition = false;
    py::str referencedDeclName;         // (node["referencedDecl"]["name"], or "")
    py::str referencedMemberDecl;       // ("referencedMemberDecl", or "")
};

// clang's tree: the root and what is known of each node of it
struct Tree {
    Node root;
    std::unordered_map<const Node*, Extra> extra;
    const Extra& ex(const Node& n) const;   // (the Extra of a node; of an unknown node, the empty one)
};

// clang's tree of a file of the port (the JSON of -ast-dump), or None  (arith16.dump, with the extra keys)
std::shared_ptr<Tree> _dump(const py::str& cc, const py::str& src, const py::str& folder);

// the chain of a node's ancestors, outermost first
using Parents = std::vector<const Node*>;
using WalkFn = std::function<void(const Node&, const Parents&)>;

void _walk(const Node& node, const WalkFn& fn, Parents& parents);
void _walk(const Node& node, const WalkFn& fn);

// {RecordDecl id: (name, [FieldDecl ids], parent record id)}
using RecInfo = std::tuple<py::str, py::list<py::str>, std::optional<py::str>>;
// {FieldDecl id: (record id, name, type)}
using FieldInfo = std::tuple<py::str, py::str, py::str>;
std::tuple<py::dict<py::str, RecInfo>, py::dict<py::str, FieldInfo>> _records(const Tree& tree);

// the named record a record is inside (itself when named)
py::str _top(const py::dict<py::str, RecInfo>& recs, const py::str& rid);

// 'struct expr *' -> 'struct expr'; 'union (unnamed at ...)' -> None
std::optional<py::str> _rtype(const std::optional<py::str>& t);

// the name of the function a CallExpr calls (through its decay to a pointer, parentheses) - or ''
// (call: the node, or nullptr for None)
py::str callee_name(const Tree& tree, const Node* call);

// the named records the program takes as bytes: (char *)&x, (char *)&x.m, (char *)p
py::set<py::str> byte_records(const py::list<std::shared_ptr<Tree>>& trees);

// ({old-style function the port keeps: [whether each parameter is a pointer]} - set by the port's pass)
extern py::dict<py::str, py::list<bool>> PTR_PARAMS;

// every (record, member) found -> its pointer type
using Found = py::dict<std::tuple<py::str, py::str>, py::str>;
// [(file offset of a refused use, the operator)]
using Refused = py::list<std::tuple<int64_t, std::optional<py::str>>>;
// [(offset, depth, text)]
using Edits = py::list<std::tuple<int64_t, int64_t, py::str>>;

// [(offset, depth, text)] - every read and write of a pointer member of the records kept old; and the
// {(record, member): its pointer type} found; and the uses refused
std::tuple<Edits, Found, Refused> edits_of(const Tree& tree, const py::str& text, const py::str& path, const py::set<py::str>& olds);

// (start, end) of an expression in the file path - its first token where it is written, its last where the
// macro that gave it is used (e->e_sym, e_sym a macro) - None when it is not all in the file
std::optional<std::tuple<int64_t, int64_t>> place(const Node& n, const py::str& path);

// the insertions made: at one place the outer ones' openings before the inner, closings after
py::str apply(const py::str& text, const Edits& edits);

// ((struct n *)dos_hp(h))->op, and l = ((struct n *)dos_hp(h)); ... l->op with l never tested: a read
// through what may be NULL - on DOS the start of the data segment (no fault) - dos_hpr, the null segment for
// handle 0; a pointer the function tests (if (!l), l == 0, l && ...) keeps dos_hp, its NULL a NULL
py::str null_reads(const py::str& text);

// the pointer members of the records kept old declared uint16_t (a handle), in their definitions
py::str declare(const py::str& text, const py::set<py::str>& olds, const Found& found);

// whether a file, or a header of the program's it includes, names one of the tags
bool mentions(const py::str& folder, const py::str& f, const py::list<py::str>& tags, const std::optional<py::set<py::str>>& seen = std::nullopt);

// The version of the code that makes this pass's cached results -- for THIS C++ port (see arith16::CODE_VERSION:
// the Python hashes the source of its own functions). BUMP IT WHENEVER THE LOGIC OF byte_records, edits_of or
// apply CHANGES.
extern const py::str CODE_VERSION;

// -> ({file: its text with the conversions}, set of records kept old, {(record, member): type}, refusals)
//    refusals: [(file, offset, operator)]
std::tuple<py::dict<py::str, py::str>, py::set<py::str>, Found, py::list<std::tuple<py::str, int64_t, std::optional<py::str>>>>
run(const py::str& cc, const py::str& folder, const py::list<py::str>& files);

}  // namespace codebreaker::port::oldlayout
