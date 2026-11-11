// codebreaker/verify/authored/match16.hpp -- typed C++ of codebreaker.verify.authored.match16.
//
// match16.py -- does Microsoft C 6 turn a 16-bit module's reconstruction back into its bytes?
//
//     python -m codebreaker match <16-bit module> [--flags "/AM /Gcsw /Os"] [--first N]
//
// The 16-bit counterpart of match.py (see there for what the comparison is and is not). Each reconstructed function is
// compiled by Microsoft C (build/msc6.py) and its bytes are compared with the original function's, outside the places a
// loader or linker fills in: in the original, every word a relocation record of its segment names and the distance of
// every near call and jump to another function; in the compiled function, every place the object file has a fix-up for.
//
// Without --flags the switches are found first: the memory model and options under which most of a sample of the
// functions come out the original's length say how the module was built.
//
// Typing notes:
//   * The lane module `verify/lanes/win16.py` (prepare, _spans) has no typed port: what run() reads of it is `Prepare16`
//     (-> Prepared16: header, texts, order, by_va, where) and `SpansFn` (win16._spans(nef, {va: (segment, offset)})),
//     both passed in.
//   * `run` leaves its working state as attributes of the function in the Python (run.header, .library, .texts, .settings,
//     .setting, .files, .segment, .winner, .code: scoreboard16 reads them): here `last()`.
//   * `log` is the Python's `log=print`: an empty std::function is None (no log); `log_print` is the default.
//   * A verdict is the tuple (word, differing bytes, total bytes) of match.distance(...)[:3]; a setting is (compiler, switches).
//   * `wine16.exports(module)` (settings: wine16) is called through symbols::wine16.
//   * The set {file_of[s] for s in sources} that search's compile_group walks is visited in ascending order (CPython's
//     iteration order of a set of small ints is not defined by the language).
//   * The remembered choices (choices.json) are read and written as JSON (json.dump indent=1).
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>

#include "codebreaker/build/msc6.hpp"
#include "codebreaker/formats/ne.hpp"
#include "codebreaker/verify/authored/match.hpp"
#include "py.hpp"

namespace codebreaker::verify::authored::match16 {

using match::Spans16;
using match::Where16;
using Verdict = std::tuple<py::str, int64_t, int64_t>;                 // (word, differing bytes, total bytes)
using Setting = std::tuple<py::str, py::str>;                          // (compiler, switches)
using Log = std::function<void(const py::str&)>;
void log_print(const py::str& line);                                   // print

// What run() reads of the lane's prepared 16-bit module.
struct Prepared16 {
    py::str header;
    py::dict<py::str, py::str> texts;
    py::list<py::str> order;
    py::dict<int64_t, py::str> by_va;
    Where16 where;                                                     // {address: (segment, offset)}
};
using Prepare16 = std::function<Prepared16(const py::str& module_path, const py::str& work)>;
using SpansFn = std::function<Spans16(const formats::ne::NEFile& nef, const Where16& where)>;

// [[function, ...], ...] -- the original's source files, as far as its calls tell: a far call to a function as
// `push cs; call near` (the segment known: defined in the same file) or a near call, puts caller and callee in one file,
// and a file is contiguous, so everything between them too. A function no such call reaches is a file of its own.
py::list<py::list<py::str>> source_files(const py::list<py::str>& names, const Where16& where,
                                         const py::dict<int64_t, match::ByteMasked>& originals, const py::dict<py::str, int64_t>& address);

// how near a set of functions came: each counts the share of its bytes that are the original's (a function that does not
// compile, nothing), an identical one counting double
double _score(const py::dict<int64_t, Verdict>& results);
double _score1(const Verdict& v);

struct Options {
    std::optional<py::str> flags;
    int64_t limit = 0;
    Log log = log_print;
    std::optional<py::str> compiler;
    bool choose = false;
    bool sdk = false;
    bool fold = true;
    int64_t search = 0;
    bool wine = false;
};

// What the last run() left (the Python's attributes of the function).
struct Last {
    std::optional<py::str> header;
    py::dict<py::str, py::str> library;
    py::dict<py::str, py::str> texts;
    std::optional<py::set<Setting>> settings;
    py::dict<py::str, Setting> setting;
    py::list<py::list<py::str>> files;
    py::dict<py::str, int64_t> segment;
    py::dict<py::str, py::str> winner;
    py::dict<py::str, std::tuple<std::optional<match::ByteMasked>, match::ByteMasked>> code;
    bool ran = false;
};
Last& last();

// -> ({segment: (compiler, flags)}, {name: (verdict, differing bytes, total)})
//
// Without `flags` the compiler and switches are chosen segment by segment (a segment is a source file's code): those under
// which most of a sample of its functions come out the original's length, or the original's bytes.
std::tuple<py::dict<int64_t, Setting>, py::dict<py::str, Verdict>> run(const py::str& module_path, const Options& options, const Prepare16& win16_prepare,
                                                                        const SpansFn& win16_spans);

extern const py::str DOC;
// python -m codebreaker match <16-bit module> [--flags "..."] [--compiler msc6|msvc152] [--first N] [--choose] [--sdk] [--no-fold] [--search ROUNDS] [--wine]
int64_t main(const py::list<py::str>& argv, const Prepare16& win16_prepare, const SpansFn& win16_spans);

}  // namespace codebreaker::verify::authored::match16
