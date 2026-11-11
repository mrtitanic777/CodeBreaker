// codebreaker/port/arith16.hpp -- COMPAT SHIM over cb/port/arith16.hpp (docs/CPP_DESIGN.md 6.3). Deleted with its last
// py:: consumer.
//
// arith16 is refactored: the pass lives in native/cb/port/arith16.hpp (clang's tree read into typed nodes by a streaming
// parser, the casts as insertions, the results kept in a cb::base::cache::Cache). This header keeps what the ports that
// have not been refactored yet use:
//
//   * port/__main__ calls run(cc, folder, files);
//   * port/oldlayout reads clang's tree as the records below (Loc, Range, TypeRef, Number, Node) with bare(), mark_files(),
//     _files(), MAIN, TEXT, _headers_key() and pass_key(). Those records and the five small helpers on them stay here
//     as they were -- COMPAT(port/oldlayout): they go when oldlayout is refactored onto cb::port::arith16::Tree.
//
// What is different, on purpose: mark_files() no longer lets a location's `includedFrom` (the file that included this one)
// move the file the following locations are taken to be in (the Python had the same fault; every location of a header after
// its first was taken for the includer's, so the header's casts were put into the C file at the header's offsets).
#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <variant>

#include "cb/compat/py_bridge.hpp"
#include "cb/port/arith16.hpp"
#include "codebreaker/base/cache.hpp"

namespace codebreaker::port::arith16 {

// ---- COMPAT(port/oldlayout): clang's tree as records ------------------------------------------------------------------

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

// The text of the file being read and the file of the main source (the Python's module state, set by oldlayout).
inline std::optional<py::str> MAIN;
inline std::optional<py::str> TEXT;

// Without an inner node: what `(n.get("inner") or [{}])[0]` is.
inline const Node& empty_node() {
    static const Node e;
    return e;
}

// An expression with its parentheses and conversions passed.
inline const Node& bare(const Node& n0) {
    const Node* n = &n0;
    const auto is = [](const std::optional<py::str>& a, const char* b) { return a.has_value() && *a == py::str(b); };
    while (is(n->kind, "ParenExpr") ||
           (is(n->kind, "ImplicitCastExpr") && (is(n->castKind, "LValueToRValue") || is(n->castKind, "NoOp") || is(n->castKind, "IntegralCast"))))
        n = n->inner.size() != 0 ? &n->inner[0] : &empty_node();
    return *n;
}

// clang names a location's file only where it changes: each location given its own (_file). A location's `includedFrom`
// is the includer's name and does not move the cursor.
inline void mark_loc(Loc& v, std::optional<py::str>& cur, bool included_from = false) {
    if (v.offset.has_value() || v.file.has_value() || v.spellingLoc) {
        if (v.file.has_value() && !included_from) cur = v.file;
        v._file = cur;
    }
    if (v.spellingLoc) mark_loc(*v.spellingLoc, cur);
    if (v.expansionLoc) mark_loc(*v.expansionLoc, cur);
    if (v.includedFrom) mark_loc(*v.includedFrom, cur, true);
}

inline void mark_files(Node& node, std::optional<py::str>& cur) {
    mark_loc(node.loc, cur);
    mark_loc(node.range.begin, cur);
    mark_loc(node.range.end, cur);
    for (Node& x : node.inner.vec()) mark_files(x, cur);
}

inline void files_loc(const Loc& l, py::list<py::str>& seen) {
    if (l.file.has_value() && !seen.contains(*l.file)) seen.append(*l.file);
    if (l.spellingLoc) files_loc(*l.spellingLoc, seen);
    if (l.expansionLoc) files_loc(*l.expansionLoc, seen);
    if (l.includedFrom) files_loc(*l.includedFrom, seen);
}

// the files a tree names, in the order they are met
inline py::list<py::str> _files(const Node& node, const std::optional<py::list<py::str>>& seen0 = std::nullopt) {
    py::list<py::str> seen = seen0.has_value() ? *seen0 : py::list<py::str>();
    files_loc(node.loc, seen);
    files_loc(node.range.begin, seen);
    files_loc(node.range.end, seen);
    for (const Node& x : node.inner.vec()) _files(x, seen);
    return seen;
}

// What every header of the port holds, as a key the Python kept in `headers=`: no pass reads it any more (the key of a
// file's result is the headers it reaches), so it is nothing.
inline py::str _headers_key(const py::str&) { return py::str(""); }

// ---- forwards ---------------------------------------------------------------------------------------------------------

// What a pass's result for one file depends on (see cb::port::arith16::pass_key). `headers` is no longer used.
inline py::str pass_key(const py::str& cc, const py::str& folder, const py::str& f, const py::str& kind,
                        const py::list<py::str>& extra = py::list<py::str>(), const std::optional<py::str>& /*headers*/ = std::nullopt) {
    namespace compat = ::cb::compat;
    return compat::to_py(::cb::port::arith16::pass_key(::cb::fs::from_text(compat::from_py(cc)), ::cb::fs::from_text(compat::from_py(folder)),
                                                       compat::from_py(f), compat::from_py(kind), ::cb::port::arith16::kCodeVersion,
                                                       compat::from_py(extra)));
}

// {file: its text with the casts} for the port's C files (those clang could read) - a file's result kept in the cache.
inline py::dict<py::str, py::str> run(const py::str& cc, const py::str& folder, const py::list<py::str>& files) {
    namespace compat = ::cb::compat;
    namespace newp = ::cb::port::arith16;
    ::cb::base::cache::Cache& cache = base::cache::shim::any_cache();
    static std::once_flag declared;
    std::call_once(declared, [&] { newp::declare_cache(cache); });
    const std::vector<std::string> names = compat::from_py(files);
    const std::map<std::string, std::string> made =
        compat::guard([&] { return newp::run(cache, ::cb::fs::from_text(compat::from_py(cc)), ::cb::fs::from_text(compat::from_py(folder)), names); });
    py::dict<py::str, py::str> out;
    for (const std::string& f : names) {   // (in the order of the files)
        const auto it = made.find(f);
        if (it != made.end()) out.set(compat::to_py(f), compat::to_py(::cb::text::from_latin1(::cb::as_bytes(it->second))));
    }
    return out;
}

}  // namespace codebreaker::port::arith16
