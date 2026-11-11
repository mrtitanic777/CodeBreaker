// codebreaker/source/ne_asm/reachability.hpp -- typed C++ of codebreaker.source.ne_asm.reachability.
//
// reachability.py -- which bytes in a segment are actually reachable CODE.
//
// This is the foundation the safe form of the readability track needs. Instead of guessing code-vs-data from
// "looks printable" (which misclassifies printable code and broke byte-exact), it traces control flow: from every
// entry point, follow fall-through and direct branch/call targets. Whatever is NEVER reached is not executed -- a
// safe, conservative signal for DATA.
//
// Entry points (seeds): carved function starts, exported entries, and intra-segment relocation targets (code
// addresses the binary itself references).
//
// Reachable  -> real code (emit live).
// Unreachable -> safe to treat as data (nothing branches into it, so labeling it data can't drop a needed label).
// Worst case a jump-table-only-reached function is conservatively called data -- byte-exact still holds (db), just
// less live.
//
//     python reachability.py MOUSE --seg 1
//
// Typing notes:
//   * The Python's four lazy-import helpers (_masm, _segdiff, _segment, _msvc152: they only return modules) have no
//     C++ counterpart; _Corpus / _target() are the struct Corpus (the old target registry's `resolve`).
//   * The functions take the typed port of source.ne_asm.segment's Seg (its funcs, export_names, fixups, rows, si:
//     the Python reads them with getattr defaults, all of which a Seg has).
//   * main(argv): argv is sys.argv[1:]; the return is the exit status. The command line is read by hand (the module,
//     --seg N / --seg=N, -h/--help): a bad one prints a one-line usage and error to `err` and returns 2; argparse's
//     abbreviations and its exact wording are not reproduced. What the tool prints goes to `out`.
#pragma once

#include <cstdint>
#include <iosfwd>
#include <tuple>

#include "codebreaker/source/ne_asm/segment.hpp"
#include "py.hpp"

namespace codebreaker::source::ne_asm::reachability {

// instructions with a direct target / no fall-through (control does not continue to the next byte)
extern const py::set<py::str> JMPCC;
extern const py::set<py::str> NOFALL;

// What the old target registry answered: where a module of the corpus is.
struct Corpus {
    static py::str resolve(const py::str& module);
};
Corpus _target();

// Set of instruction offsets reachable from the segment's entry points.
py::set<int64_t> reachable(const segment::Seg& sg);

// Every offset that something ENTERS by a jump/call/entry (needs a label).
py::set<int64_t> branch_targets(const segment::Seg& sg);

// [(source, target)] for every direct intra-segment branch/call.
py::list<std::tuple<int64_t, int64_t>> _branches(const segment::Seg& sg);

// datacode regions with no EXTERNAL entry -- nothing OUTSIDE the region jumps or falls into it, and no
// function-start/export/reloc-target lands in it. A region's own internal (misdecoded) jumps don't count, so a data
// blob that objdump littered with fake branches is still safely emitted as data. This is stricter than 'no target
// inside' where it matters (real entries) and looser where it doesn't (self-references) -> more data recovered, still
// label-safe.
py::list<std::tuple<int64_t, int64_t, py::str>> safe_data_regions(const segment::Seg& sg);

int64_t main(const py::list<py::str>& argv, std::ostream& out, std::ostream& err);

}  // namespace codebreaker::source::ne_asm::reachability
