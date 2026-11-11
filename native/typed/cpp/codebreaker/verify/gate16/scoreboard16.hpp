// codebreaker/verify/gate16/scoreboard16.hpp -- typed C++ of codebreaker.verify.gate16.scoreboard16.
//
// scoreboard16.py -- where each of a 16-bit module's functions differs from the original, and whose
// work that difference is.
//
//     python -m codebreaker score16 <16-bit module> [--segment N] [--json FILE]
//
// Every reconstructed function is compiled twice from the same C, in the same source files and with
// the same switches: by Microsoft C 8.00c (the compiler that built the module) and by OpenWatcom with
// -zmsc (the fork that is to make Microsoft C 8's choices). Three comparisons follow:
//
//     C          Microsoft C 8's bytes against the original's: where they differ, the C is not what was
//                written -- the decompiler's work
//     compiler   OpenWatcom's bytes against Microsoft C 8's, from the same C: where they differ, the
//                fork does not yet do what Microsoft C 8 does -- the compiler's work
//     fits       OpenWatcom's length against the original's: a function no longer than the original
//                goes back into its own place, and nothing in the segment moves
//
// The decompiler's patterns are counted too (far pointers split into int pairs, CONCAT22, MK_FP,
// segment registers read through helpers, single-use temporaries), with the bytes the functions that
// show each one are off by, so the largest is the next to remove.
//
// Typing notes:
//   * A row (the Python's dict, one per function) is `Row`; `patterns` keeps the order of PATTERNS; the board is
//     py::dict<name, Row> in the order of match16's results. `msc8`, `watcom` and `segment` are optional (None).
//   * match16.run leaves its state as attributes of the function: here match16::last(); match16.run itself needs the lane's
//     prepare and spans (verify/lanes/win16), passed in as match16::main takes them.
//   * `_watcom`'s `_watcom.refused` attribute (why a function was refused) is the second member of the result.
//   * `log` is the Python's `log=print`: an empty std::function is None (nothing logged).
//   * json.dump(rows, indent=1) is rows_json(); json.load of a saved board is read with knowledge::sources::_json.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>

#include "codebreaker/build/msc6.hpp"
#include "codebreaker/verify/authored/match16.hpp"
#include "py.hpp"

namespace codebreaker::verify::gate16::scoreboard16 {

using Log = std::function<void(const py::str&)>;
void log_print(const py::str& line);   // print

// what the decompiler writes that a programmer does not, and that a compiler turns into more code: (name, pattern), in order
const py::list<std::tuple<py::str, py::re::Pattern>>& PATTERNS();

// OpenWatcom's switches for Microsoft's (tests/msc8/suite.py keeps the same mapping)
py::list<py::str> watcom_flags(const py::str& ms);

// the code alike, a fix-up's bytes (either side's) aside
bool _same(const build::msc6::Code& a, const build::msc6::Code& b);

// {function: (bytes, mask) or None}: each source file compiled by OpenWatcom -zmsc as Microsoft C compiled it (the same functions
// together, the same switches); the second: {function: why it was refused}.
std::tuple<py::dict<py::str, std::optional<build::msc6::Code>>, py::dict<py::str, py::str>> _watcom(
    const py::list<py::list<py::str>>& files, const py::dict<py::str, py::str>& texts, const py::str& header,
    const py::dict<py::str, authored::match16::Setting>& settings_of, const Log& log);

struct HexBytes {
    py::str original, msc8, watcom;
};

struct Row {
    std::optional<int64_t> segment;
    int64_t original = 0;
    std::optional<int64_t> msc8;
    py::str msc8_vs_original;
    int64_t msc8_bytes_off = 0;
    std::optional<int64_t> watcom;
    py::str watcom_vs_msc8;               // "same", "differs" or "n/a"
    bool fits = false;
    py::list<std::tuple<py::str, int64_t>> patterns;
    bool library = false;
    py::str c;
    HexBytes hex;
};

using Rows = py::dict<py::str, Row>;

Rows run(const py::str& module_path, const std::optional<int64_t>& segment, const Log& log, const authored::match16::Prepare16& win16_prepare,
         const authored::match16::SpansFn& win16_spans);

void report(const Rows& rows, const Log& out = log_print);

// a function's C, and its original's, Microsoft C 8's and OpenWatcom's instructions side by side
void show(const Row& row, const Log& out = log_print);

// json.dump(rows, f, indent=1)
py::str rows_json(const Rows& rows);

extern const py::str DOC;
int64_t main(const py::list<py::str>& argv, const authored::match16::Prepare16& win16_prepare, const authored::match16::SpansFn& win16_spans);

}  // namespace codebreaker::verify::gate16::scoreboard16
