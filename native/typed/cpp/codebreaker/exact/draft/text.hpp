// codebreaker/exact/draft/text.hpp -- compat shim for cb/exact/draft/text.hpp (docs/CPP_DESIGN.md 6.3). The functions
// convert and forward. What the old passes (shapes, forms, candidates, prove) keep in module-level tables - the
// handles UCHAR, BYTEG, PTRSIZE, STRUCTARR, ARRAYS, FILEPTR, RANGES, LONGINFO, BYTEPTR, _LIBDATA, _HEADERS, _TABLES -
// stay here, in the old layer's shape, until those passes are refactored to hold a cb::exact::draft::text::FunctionFacts
// instead. Deleted with its last old-layer consumer.
//
// text.py -- what every rewrite of a draft reads its text with: the parts of an if, a block's ends, a test's
// parentheses and negation, whether a register is read later, and the names of registers and the library's data.
//
// C++ spelling notes:
//  * the module-level tables the other draft modules fill are py::set / py::dict handles (shared, like Python's):
//      UCHAR     {(pointer, offset)} or {"g_xxxx"}           -> std::variant<tuple<str, int>, str>
//      BYTEG     {"g_xxxx"}
//      PTRSIZE   name -> 2
//      STRUCTARR name -> (size, type name | [(offset, type name)])
//      ARRAYS    name -> element type;  FILEPTR name -> type (None for a missing include)
//      RANGES    {(lo, hi, flag)};  BYTEPTR {(pointer, offset)}
//      LONGINFO  the five keys "unsigned" (set of names), "ret" (bool), "fields" (set of offsets), "globals" and
//                "funcs" (sets of names): the struct LongInfo, each key std::optional (absent until set)
//      _LIBDATA  project dir -> {name -> address};  _HEADERS (toolchain, header) -> names;
//      _TABLES   (project dir, header) -> [(macro, table, offset, mask)]
//  * `lines` that may hold None (a dropped line) are py::list<std::optional<py::str>> (_live_after only).
//  * GOTO_LIVE is Python's one-element list [True]: py::list<bool>.
#pragma once

#include <functional>
#include <optional>
#include <tuple>
#include <variant>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/text.hpp"
#include "cb/exact/draft/text.hpp"
#include "py.hpp"

namespace codebreaker::exact::draft::text {

inline const py::str REGS = py::str("\\b(ax|bx|cx|dx|si|di|al|ah|bl|bh|cl|ch|dl|dh|es)\\b");
inline const py::str WREG = py::str("(ax|bx|cx|dx|si|di)");

namespace nt = cb::exact::draft::text;

namespace detail {
using cb::compat::from_py;
using cb::compat::to_py;

// The old layer's lines as the new tree takes them. A line index is converted by the caller.
inline std::vector<std::string> lines_of(const py::list<py::str>& l) { return from_py(l); }
inline py::list<py::str> list_of(const std::vector<std::string>& v) { return to_py(v); }
inline std::size_t index_of(int64_t k) { return cb::compat::int_from_py<std::size_t>(k); }

// The words a read pattern of the old layer names: \bX\b or \b(?:X|Y|Z)\b.
inline std::vector<std::string> words_of_pattern(const std::string& pattern) {
    std::string p = cb::text::replace_all(pattern, "\\b", "");
    p = cb::text::replace_all(cb::text::replace_all(cb::text::replace_all(p, "(?:", ""), "(", ""), ")", "");
    std::vector<std::string> words;
    for (std::string_view w : cb::text::split(p, '|')) words.emplace_back(w);
    return words;
}
}  // namespace detail

// ((X)) -> (X): the parentheses a test is printed in, once
inline py::str _strip(const py::str& c) { return py::str(nt::strip_parens(c.utf8())); }

// a test as the value it tests: (X == 0) -> !X, (X != 0) -> X
inline py::str _truth(const py::str& c) { return py::str(nt::truth(c.utf8())); }

inline bool _balanced(const py::str& s) { return nt::balanced(s.utf8()); }

// c split at op (&& or ||) outside parentheses, or [c]
inline py::list<py::str> _top_split(const py::str& c, const py::str& op) { return detail::list_of(nt::top_split(c.utf8(), op.utf8())); }

inline py::str _not(const py::str& c) { return py::str(nt::negate(c.utf8())); }

// (bytes the code widens with zeros: unsigned char)
inline const py::set<std::variant<std::tuple<py::str, int64_t>, py::str>> UCHAR{};
// (globals the code reads or writes a byte of: char)
inline const py::set<py::str> BYTEG{};

// the line opening the block line k (`}` or `} else {`) closes: the nearest before it at its indentation
inline std::optional<int64_t> _opener(const py::list<py::str>& lines, int64_t k) {
    const auto r = nt::opener(detail::lines_of(lines), detail::index_of(k));
    return r ? std::optional<int64_t>(static_cast<int64_t>(*r)) : std::nullopt;
}

// is line j inside a loop or a switch opened after line lo, deeper than the indentation ind
inline bool _in_loop(const py::list<py::str>& lines, int64_t lo, int64_t j, int64_t ind) {
    return nt::in_loop(detail::lines_of(lines), lo, detail::index_of(j), detail::index_of(ind));
}

// f(text), or None when it changes nothing (a pass as a variant)
inline std::optional<py::str> _changed(const std::function<py::str(const py::str&)>& f, const py::str& text) {
    py::str t = f(text);
    if (t == text) return std::nullopt;
    return t;
}

// [the line opening each block around line k, the innermost first]
inline py::list<int64_t> _openers(const py::list<py::str>& lines, int64_t k) {
    return detail::to_py(nt::openers(detail::lines_of(lines), detail::index_of(k)));
}

inline const py::list<bool> GOTO_LIVE = py::list<bool>{true};

// is reg (or what `read` matches: its byte too) read after line k before it is given a new value - on any
// path: a value given it inside a nested block (an arm, a loop) is not on every path, so it ends nothing; one
// given at k's own depth, in a bare block, or out of k's block does (lines None: dropped)
inline bool _live_after(const py::list<std::optional<py::str>>& lines, int64_t k, const py::str& reg,
                        const std::optional<py::str>& read = std::nullopt) {
    std::vector<std::optional<std::string>> converted;
    for (const auto& l : lines) converted.push_back(l ? std::optional<std::string>(l->utf8()) : std::nullopt);
    int64_t first = k + 1;   // (lines[k + 1:], a negative start counting from the end)
    if (first < 0) first = std::max<int64_t>(0, first + static_cast<int64_t>(converted.size()));
    std::vector<std::string> words;
    if (read && !read->empty()) words = detail::words_of_pattern(read->utf8());   // (an empty pattern is no pattern: `read or ...`)
    const std::vector<std::string_view> views(words.begin(), words.end());
    return nt::live_after(converted, static_cast<std::size_t>(first), reg.utf8(), views, nt::LiveOptions{GOTO_LIVE[0]});
}

inline const py::dict<py::str, int64_t> SIZES = py::dict<py::str, int64_t>{
    {py::str("char"), 1}, {py::str("int"), 2}, {py::str("unsigned"), 2}, {py::str("short"), 2}, {py::str("long"), 4}};
// (data a draft steps through by 2: a pointer to words)
inline const py::dict<py::str, int64_t> PTRSIZE{};
// (data an array of structs: name -> (size, first field's type); the struct pass later makes the second a
// list of (offset, type))
inline const py::dict<py::str, std::tuple<int64_t, std::variant<py::str, py::list<std::tuple<int64_t, py::str>>>>> STRUCTARR{};
// (data a draft indexes: an array - name: element type)
inline const py::dict<py::str, py::str> ARRAYS{};
// (a stream a library macro reads: name -> its type; "#include" names a header, None when the macro has none)
inline const py::dict<py::str, std::optional<py::str>> FILEPTR{};
inline const py::dict<py::str, py::dict<py::str, int64_t>> _LIBDATA{};
inline const py::dict<std::tuple<py::str, py::str>, py::set<py::str>> _HEADERS{};
inline const py::dict<std::tuple<py::str, py::str>, py::list<std::tuple<py::str, py::str, int64_t, int64_t>>> _TABLES{};

// s split at its commas outside parentheses and brackets
inline py::list<py::str> _top_commas(const py::str& s) { return detail::list_of(nt::top_commas(s.utf8())); }

// the index of the line closing the block line k opens (by indentation: the first line back at its indent)
inline std::optional<int64_t> _block_end(const py::list<py::str>& lines, int64_t k) {
    const auto r = nt::block_end(detail::lines_of(lines), detail::index_of(k));
    return r ? std::optional<int64_t>(static_cast<int64_t>(*r)) : std::nullopt;
}

// `} else if (C) {` chains written out as nested ifs, each inside the else before it (rewrites `lines` in place
// and returns it)
inline py::list<py::str> _else_ifs(py::list<py::str> lines) {
    std::vector<std::string> v = detail::lines_of(lines);
    nt::else_ifs(v);
    lines.vec() = detail::list_of(v).vec();
    return lines;
}

// (cond, then, else, end) of the if at line k - `if (C) {`, its arms' lines, the line after it - or None
using IfParts = std::tuple<py::str, py::list<py::str>, std::optional<py::list<py::str>>, int64_t>;
inline std::optional<IfParts> _if_parts(const py::list<py::str>& lines, int64_t k) {
    const auto r = nt::if_parts(detail::lines_of(lines), detail::index_of(k));
    if (!r) return std::nullopt;
    std::optional<py::list<py::str>> other;
    if (r->else_arm) other = detail::list_of(*r->else_arm);
    return IfParts(py::str(r->cond), detail::list_of(r->then_arm), other, static_cast<int64_t>(r->end));
}

// arm without the label the other arm's goto went to (the only label it loses: others are wanted)
inline py::list<py::str> _unlabelled(const py::list<py::str>& arm, const std::optional<py::list<py::str>>& other = std::nullopt) {
    std::optional<std::vector<std::string>> o;
    if (other) o = detail::lines_of(*other);
    return detail::list_of(nt::unlabelled(detail::lines_of(arm), o));
}

// a op b, an operand that is the other kind of join parenthesized (|| inside &&)
inline py::str _conj(const py::str& op, const py::str& a, const py::str& b) { return py::str(nt::join_tests(op.utf8(), a.utf8(), b.utf8())); }

// do the arms x and y run the same: equal, or x the statements y begins with and then a goto to y's label
// there (the compiler's one copy of a tail both reach) - and the label then wanted by none else
inline bool _same_arm(const py::list<py::str>& x, const py::list<py::str>& y) {
    return nt::same_arm(detail::lines_of(x), detail::lines_of(y));
}

// `X = E;` with reg read in E alone and E giving nothing a value: reg's value read before the store
inline bool _plain_store(const py::str& l, const py::str& reg) { return nt::plain_store(l.utf8(), reg.utf8()); }

// (the ranges a loaded register is tested against: (lo, hi, flag))
inline const py::set<std::tuple<int64_t, int64_t, bool>> RANGES{};

// (the function's longs the code shows: unsigned ones, its value, its fields, globals and routines)
struct LongInfo {
    std::optional<py::set<py::str>> unsigned_;   // "unsigned"
    std::optional<bool> ret;
    std::optional<py::set<int64_t>> fields;
    std::optional<py::set<py::str>> globals;
    std::optional<py::set<py::str>> funcs;
    void clear() { *this = LongInfo(); }
};
inline LongInfo LONGINFO;

// the source with x (an int parameter or local) declared a pointer to ty, or None
inline std::optional<py::str> _declare_pointer(const py::str& src, const py::str& x, const py::str& ty) {
    const auto r = nt::declare_pointer(src.utf8(), x.utf8(), ty.utf8());
    return r ? std::optional<py::str>(py::str(*r)) : std::nullopt;
}

// (the byte-wide places the code reads: (pointer, offset))
inline const py::set<std::tuple<py::str, int64_t>> BYTEPTR{};

// (head, "{", body) at the function body's own brace - a line of "{" alone, after its signature - not a
// struct's that a pass declared before it
inline std::tuple<py::str, py::str, py::str> split_body(const py::str& text) {
    const nt::BodySplit s = nt::split_body(text.utf8());
    return {py::str(s.head), py::str(s.brace), py::str(s.body)};
}

}  // namespace codebreaker::exact::draft::text
