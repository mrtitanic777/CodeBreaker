// codebreaker/verify/authored/shape.hpp -- C++ port of codebreaker.verify.authored.shape.
//
// shape.py -- the same function written in the other shapes a person might have written it in.
//
//     for start, end, text in shape.switches(body, shadow):   an if-chain on one value as a switch
//     new_body = shape.returns(head, decls, body, shadow)       `r = x; ... return r;` as `return x;`
//
// A decompiler gives every function one shape: tests in chains of if and else, one variable that
// collects the result, one return at the end. A compiler that optimises sees through small
// differences of spelling (the order of a comparison's sides, the order of declarations) and gives
// the same bytes for all of them -- but not through these: a `switch` is compiled as a switch, and
// a function that returns from the middle is laid out differently from one that does not. Each
// rewrite here changes no meaning; verify/authored/repair.py tries them and keeps what brings the bytes
// nearer the original's.
//
// `shadow` is the body with its strings and comments blanked (repair._blank), the same length:
// where things are is found in it, what is written comes from the body.
//
// The Python's two generators (switches, folds) are returned as lists of (start, end, text): they are pure, so
// running one to the end gives what iterating it would.
#pragma once

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::verify::authored::shape {

extern const py::str _NUM;
extern const py::str _NAME;

// (start, end) of a statement; (start, end, text) of a rewrite
using Span = std::tuple<int64_t, int64_t>;
using Edit = std::tuple<int64_t, int64_t, py::str>;

// The index of the bracket that closes the one at `i`, or -1.
int64_t _close(const py::str& shadow, int64_t i, const py::str& pair = py::str("{}"));
// [(start, end)] of the statements between `lo` and `hi`, an if with all its elses as one.
py::list<Span> statements(const py::str& shadow, int64_t lo, int64_t hi);
// (start, end, text) for each `if (v == A) {..} else if (v == B) {..} [else {..}]` chain: the same as a switch.
py::list<Edit> switches(const py::str& body, const py::str& shadow);
// Collect, for the statement at [start, end), every `r = x;` that is the last thing done before control reaches
// the end of the function (into `edits`).
void _tail(const py::str& body, const py::str& shadow, int64_t start, int64_t end, const py::str& r, const py::list<Edit>& edits);
// The body with every `r = x;` that only the final `return r;` follows written `return x;`, or None.
std::optional<py::str> returns(const py::str& head, const py::list<py::str>& decls, const py::str& body, const py::str& shadow);
// The type the function returns, from its heading (the word before its convention and name).
std::optional<py::str> _returned(const py::str& head);
// (start, end, text) for each `t = f(...);` followed at once by `if (t <compare> N) {`.
py::list<Edit> folds(const py::list<py::str>& decls, const py::str& body, const py::str& shadow);

}  // namespace codebreaker::verify::authored::shape