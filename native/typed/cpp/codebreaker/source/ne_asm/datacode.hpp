// codebreaker/source/ne_asm/datacode.hpp -- typed C++ port of codebreaker.source.ne_asm.datacode.
//
// datacode.py -- separate DATA from CODE in a segment (readability track #3/#5).
//
// Code segments carry embedded data -- strings, jump tables, constant blobs -- which a linear disassembler
// (objdump) blindly decodes as bogus instructions (`outs dx,...`, `arpl`, `imul bp,[di+0x61],0x7972`...).
// Those show up as `db` byte-dumps and drag down the "live %" even though they were never instructions.
//
// This classifier finds the data regions so asmrecon can emit them as READABLE data (`db 'primary',0`,
// `dw L_case1,...`) and exclude them from the instruction/live count. Two detectors to start:
//
//   * strings  -- runs of >= MINSTR printable ASCII (optionally NUL-terminated)
//   * garbage  -- clusters of the tell-tale "this is not code" mnemonics objdump emits over data
//                 (outs/ins/arpl/segment-prefix soup)
//
//     python datacode.py MOUSE --seg 1        # list the data regions found
//
// Typing notes:
//   * a Region is (start, end, kind) with kind "str" or "data"; find_strings' text is the run decoded as latin-1.
//   * data_regions takes the segment as the typed port of source.ne_asm.segment's Seg (its `code` and `rows`).
//   * main(argv) takes sys.argv (argv[0] is the program name argparse puts in its usage line) (the Python returns None);
//     the command line is parsed as argparse (Python 3.14) does for this parser (RawDescriptionHelpFormatter,
//     the positional `module`, `--seg SEG` read with int()): -h prints the module's docstring as the help to the
//     output stream of set_streams and raises SystemExit(0); a bad command line prints the usage and the error to
//     the error stream and raises SystemExit(2). What the listing prints goes through py::print (stdout).
//   * the Python's `sys.path.append(...)` has no counterpart; the module is opened with
//     corpus::retail(name) or, when the corpus does not know it, the name as a path.
//   * not ported: the `__name__ == "__main__"` block (`main()`): the caller calls main.
#pragma once

#include <cstdint>
#include <iosfwd>
#include <tuple>

#include "codebreaker/source/ne_asm/segment.hpp"
#include "py.hpp"

namespace codebreaker::source::ne_asm::datacode {

inline constexpr int64_t MINSTR = 5;   // min printable run to call it a string

// objdump tends to emit ONLY when decoding data as code
extern const py::set<py::str> GARBAGE_MN;
extern const py::set<py::str> SEGPFX;

// (start, end, kind)
using Region = std::tuple<int64_t, int64_t, py::str>;

// A run 'looks like text' (not a printable-opcode sequence or a jump table) if it contains a lowercase
// letter. Real target strings almost always do; register push/pop runs (SQRVW) and arithmetic tables
// ($(,048<@) don't.
bool _texty(const py::bytes& run);

// [(start, end, text)] for printable-ASCII runs that look like text.
py::list<std::tuple<int64_t, int64_t, py::str>> find_strings(const py::bytes& code, int64_t minlen = MINSTR);

// [(start, end)] over runs of >=3 consecutive rows whose mnemonic is the kind objdump only produces over data
// -- a strong 'this is data' signal.
py::list<std::tuple<int64_t, int64_t>> garbage_spans(const py::dict<int64_t, py::str>& rows, const py::bytes& code);

// Merge string + garbage detectors into sorted, non-overlapping data spans.
py::list<Region> data_regions(const segment::Seg& sg);

// The output streams of argparse (-h, the usage and the errors): stdout and stderr unless set.
void set_streams(std::ostream& out, std::ostream& err);

void main(const py::list<py::str>& argv);

}  // namespace codebreaker::source::ne_asm::datacode
