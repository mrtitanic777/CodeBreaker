// codebreaker/analyze/origin.hpp -- interface of codebreaker.analyze.origin (typed C++; implementation pending).
//
// origin.py -- was a 16-bit function written in assembly, or made by a C compiler?
//
//     auto r = origin::module16(path);   r.functions {va: Origin(.name .language .evidence)}, r.summary()
//     python -m codebreaker origin <module>
//
// A function is reconstructed in the language it was written in: what a C compiler made, as C; what a person wrote in
// assembly, as assembly (Windows' 16-bit core and its drivers are largely the second). Microsoft's C compilers of the
// period (C 5.1 to 8.0) have a small, regular repertoire (no LOOP/JCXZ, string instructions only with REP, no
// arguments in registers, no INT n, no 32-bit registers, BP never a plain register ...); one piece of assembly
// evidence makes a function assembly; the evidence is kept with the verdict. A C compiler leaves fingerprints of its
// own (__chkstk, its arithmetic helpers, the frame it builds and takes down).
//
// Typing notes:
//   * Origin / Result are the Python's classes; Result.summary() answers the two collections.Counter objects as
//     py::dict<str, int> in the Counter's insertion order, plus most_common() below for the Counter method main()
//     uses.
//   * module16() is kept in the cache (base.cache "origin16", pickled in the Python): the key is the module's bytes,
//     `more`, and this file's and describe's source text; the C++ keeps its own serialised form (the pickle is not
//     read by anything else).
//   * _judge / _c8_forms (private, taking the capstone decoder and a segment's bytes) are not declared: nothing else
//     reaches them. _linker16 is declared (cheap, pure).
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::analyze::origin {

// collections.Counter as an ordered {key: count}
using Counter = py::dict<py::str, int64_t>;
// Counter.most_common(n): the n commonest keys with their counts (all when n is absent), ties in insertion order
py::list<std::tuple<py::str, int64_t>> most_common(const Counter& c, const std::optional<int64_t>& n = std::nullopt);

// The verdict on one function.
struct Origin {
    int64_t va = 0;
    py::str name;
    py::str language = py::str("c");     // "c" | "assembly"
    py::list<py::str> evidence;          // why assembly (one piece makes it so)
    py::list<py::str> compiler;          // the C compiler's fingerprints

    Origin() = default;
    Origin(int64_t va_, const py::str& name_) : va(va_), name(name_) {}
    // "<name language: evidence; ...>"
    py::str repr() const;
};

struct Result {
    py::dict<int64_t, Origin> functions;
    // (the languages counted, the first evidence's kind counted)
    std::tuple<Counter, Counter> summary() const;
};

// Every function of a 16-bit module, judged (see the cache note). `more`: {va: name} of functions found besides the
// ones the module's own tables give (the decompiler finds the ones only called).
Result module16(const py::str& path, const std::optional<py::dict<int64_t, py::str>>& more = std::nullopt);
Result _module16(const py::str& path, const std::optional<py::dict<int64_t, py::str>>& more = std::nullopt);

// The (major, minor) version of the linker that made a 16-bit module, from its NE header ((0, 0) when not NE).
std::tuple<int64_t, int64_t> _linker16(const py::str& path);

// python -m codebreaker origin <module>   which functions were written in assembly
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::analyze::origin
