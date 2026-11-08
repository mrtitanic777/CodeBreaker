// codebreaker/exact/draft/text.hpp -- typed C++ from codebreaker.exact.draft.text.
//
// text.py -- what every rewrite of a draft reads its text with: the parts of an if, a block's ends, a test's
// parentheses and negation, whether a register is read later, and the names of registers and the library's data.
//
// C++ spelling notes:
//  * the module-level tables the other draft modules fill (UCHAR, BYTEG, PTRSIZE, STRUCTARR, ARRAYS, FILEPTR, RANGES,
//    LONGINFO, BYTEPTR, _LIBDATA, _HEADERS, _TABLES) are py::set / py::dict handles (shared, like Python's), typed by
//    what the other modules of the package store in them:
//      UCHAR     {(pointer, offset)} or {"g_xxxx"}           -> std::variant<tuple<str, int>, str>
//      BYTEG     {"g_xxxx"}
//      PTRSIZE   name -> 2
//      STRUCTARR name -> (size, type name | [(offset, type name)])
//      ARRAYS    name -> element type;  FILEPTR name -> type (None for a missing include)
//      RANGES    {(lo, hi, flag)};  BYTEPTR {(pointer, offset)}
//      LONGINFO  a dict with the five keys "unsigned" (set of names), "ret" (bool), "fields" (set of offsets),
//                "globals" and "funcs" (sets of names): the struct LongInfo, each key std::optional (absent until set)
//      _LIBDATA  project dir -> {name -> address};  _HEADERS (toolchain, header) -> names;
//      _TABLES   (project dir, header) -> [(macro, table, offset, mask)]
//  * `lines` that may hold None (a dropped line) are py::list<std::optional<py::str>> (_live_after only).
//  * GOTO_LIVE is Python's one-element list [True]: py::list<bool>.
#pragma once

#include <functional>
#include <optional>
#include <tuple>
#include <variant>

#include "py.hpp"

namespace codebreaker::exact::draft::text {

extern const py::str REGS;
extern const py::str WREG;

// ((X)) -> (X): the parentheses a test is printed in, once
py::str _strip(const py::str& c);

// a test as the value it tests: (X == 0) -> !X, (X != 0) -> X
py::str _truth(const py::str& c);

bool _balanced(const py::str& s);

// c split at op (&& or ||) outside parentheses, or [c]
py::list<py::str> _top_split(const py::str& c, const py::str& op);

py::str _not(const py::str& c);

// (bytes the code widens with zeros: unsigned char)
extern const py::set<std::variant<std::tuple<py::str, int64_t>, py::str>> UCHAR;
// (globals the code reads or writes a byte of: char)
extern const py::set<py::str> BYTEG;

// the line opening the block line k (`}` or `} else {`) closes: the nearest before it at its indentation
std::optional<int64_t> _opener(const py::list<py::str>& lines, int64_t k);

// is line j inside a loop or a switch opened after line lo, deeper than the indentation ind
bool _in_loop(const py::list<py::str>& lines, int64_t lo, int64_t j, int64_t ind);

// f(text), or None when it changes nothing (a pass as a variant)
std::optional<py::str> _changed(const std::function<py::str(const py::str&)>& f, const py::str& text);

// [the line opening each block around line k, the innermost first]
py::list<int64_t> _openers(const py::list<py::str>& lines, int64_t k);

extern const py::list<bool> GOTO_LIVE;

// is reg (or what `read` matches: its byte too) read after line k before it is given a new value - on any
// path: a value given it inside a nested block (an arm, a loop) is not on every path, so it ends nothing; one
// given at k's own depth, in a bare block, or out of k's block does (lines None: dropped)
bool _live_after(const py::list<std::optional<py::str>>& lines, int64_t k, const py::str& reg,
                 const std::optional<py::str>& read = std::nullopt);

extern const py::dict<py::str, int64_t> SIZES;
// (data a draft steps through by 2: a pointer to words)
extern const py::dict<py::str, int64_t> PTRSIZE;
// (data an array of structs: name -> (size, first field's type); the struct pass later makes the second a
// list of (offset, type))
extern const py::dict<py::str, std::tuple<int64_t, std::variant<py::str, py::list<std::tuple<int64_t, py::str>>>>>
    STRUCTARR;
// (data a draft indexes: an array - name: element type)
extern const py::dict<py::str, py::str> ARRAYS;
// (a stream a library macro reads: name -> its type; "#include" names a header, None when the macro has none)
extern const py::dict<py::str, std::optional<py::str>> FILEPTR;
extern const py::dict<py::str, py::dict<py::str, int64_t>> _LIBDATA;
extern const py::dict<std::tuple<py::str, py::str>, py::set<py::str>> _HEADERS;
extern const py::dict<std::tuple<py::str, py::str>, py::list<std::tuple<py::str, py::str, int64_t, int64_t>>> _TABLES;

// s split at its commas outside parentheses and brackets
py::list<py::str> _top_commas(const py::str& s);

// the index of the line closing the block line k opens (by indentation: the first line back at its indent)
std::optional<int64_t> _block_end(const py::list<py::str>& lines, int64_t k);

// `} else if (C) {` chains written out as nested ifs, each inside the else before it (rewrites `lines` in place
// and returns it)
py::list<py::str> _else_ifs(py::list<py::str> lines);

// (cond, then, else, end) of the if at line k - `if (C) {`, its arms' lines, the line after it - or None
using IfParts = std::tuple<py::str, py::list<py::str>, std::optional<py::list<py::str>>, int64_t>;
std::optional<IfParts> _if_parts(const py::list<py::str>& lines, int64_t k);

// arm without the label the other arm's goto went to (the only label it loses: others are wanted)
py::list<py::str> _unlabelled(const py::list<py::str>& arm, const std::optional<py::list<py::str>>& other = std::nullopt);

// a op b, an operand that is the other kind of join parenthesized (|| inside &&)
py::str _conj(const py::str& op, const py::str& a, const py::str& b);

// do the arms x and y run the same: equal, or x the statements y begins with and then a goto to y's label
// there (the compiler's one copy of a tail both reach) - and the label then wanted by none else
bool _same_arm(const py::list<py::str>& x, const py::list<py::str>& y);

// `X = E;` with reg read in E alone and E giving nothing a value: reg's value read before the store
bool _plain_store(const py::str& l, const py::str& reg);

// (the ranges a loaded register is tested against: (lo, hi, flag))
extern const py::set<std::tuple<int64_t, int64_t, bool>> RANGES;

// (the function's longs the code shows: unsigned ones, its value, its fields, globals and routines)
struct LongInfo {
    std::optional<py::set<py::str>> unsigned_;   // "unsigned"
    std::optional<bool> ret;
    std::optional<py::set<int64_t>> fields;
    std::optional<py::set<py::str>> globals;
    std::optional<py::set<py::str>> funcs;
    void clear() { *this = LongInfo(); }
};
extern LongInfo LONGINFO;

// the source with x (an int parameter or local) declared a pointer to ty, or None
std::optional<py::str> _declare_pointer(const py::str& src, const py::str& x, const py::str& ty);

// (the byte-wide places the code reads: (pointer, offset))
extern const py::set<std::tuple<py::str, int64_t>> BYTEPTR;

// (head, "{", body) at the function body's own brace - a line of "{" alone, after its signature - not a
// struct's that a pass declared before it
std::tuple<py::str, py::str, py::str> split_body(const py::str& text);

}  // namespace codebreaker::exact::draft::text
