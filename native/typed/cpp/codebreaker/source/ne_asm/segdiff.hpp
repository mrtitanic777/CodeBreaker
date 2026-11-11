// codebreaker/source/ne_asm/segdiff.hpp -- typed C++ port of codebreaker.source.ne_asm.segdiff.
//
// segdiff.py -- the whole-segment byte-match harness (byte-exact stage F).
//
// Turns "is this reconstructed C byte-exact?" into an automated verdict. The hard
// part (learned in the 3.1 project): you cannot match a function in isolation,
// because a near call/jmp to a *neighbouring* function is a self-relative
// displacement that only becomes concrete when the neighbour is in the SAME
// compiland. So this harness compiles a GROUP of functions together (their mutual
// calls resolve to real displacements instead of masked relocations), then diffs
// against the shipped bytes.
//
// Comparison is mask-aware on BOTH sides: our side masks OMF FIXUPP sites
// (formats/omf); the shipped side masks NE relocation sites (ne.relocations,
// following the fixup chains). Absolute addresses legitimately differ between
// builds, so masking them is what makes a true opcode-stream match possible.
//
//     segdiff --selftest                                 # prove the mechanism
//     segdiff recon.c --module KRNL386 --seg 1 --off 0x1234 --func _Foo
//                                                        # verdict vs real bytes
//
// Typing notes:
//   * The Python's four lazy-import helpers (_masm, _segdiff, _segment, _msvc152: they only return modules) have no C++
//     counterpart; _Corpus / _target() are the struct Corpus (the old target registry's `resolve`).
//   * diff()'s dict is the struct DiffResult; "first_diff" is std::optional.
//   * main() is the command line: it prints to `out`, the messages of sys.exit(text) and of argparse's errors to `err`, and returns the
//     exit status (argparse's own usage text is not reproduced: its errors give status 2 and a one-line message).
//   * compile_group keeps the work folder the compiler left, as the Python does (nothing cleans it).
#pragma once

#include <cstdint>
#include <iosfwd>
#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::source::ne_asm::segdiff {

// NE relocation source-type -> width in bytes of the relocated field
extern const py::dict<int64_t, int64_t> NE_SRC_WIDTH;

// What the old target registry answered: where a module of the corpus is.
struct Corpus {
    static py::str resolve(const py::str& module);
};
Corpus _target();

// ---------------------------------------------------------------------------
// our side: compile a group, get each function's bytes + fixup mask
// ---------------------------------------------------------------------------
// {function: (its bytes, the offsets its fix-ups cover)}; RuntimeError when the compiler made no object
py::dict<py::str, std::tuple<py::bytes, py::set<int64_t>>> compile_group(const py::str& csrc, const py::str& flags = py::str("/AM /Ox /c"));

// ---------------------------------------------------------------------------
// shipped side: pull a segment's bytes + a relocation mask from an NE module
// ---------------------------------------------------------------------------
std::tuple<py::bytes, py::set<int64_t>> shipped_ne_segment(const py::str& module_path, int64_t seg_index);

// ---------------------------------------------------------------------------
// the differ
// ---------------------------------------------------------------------------
struct DiffResult {
    bool exact = false;
    bool length_match = false;
    int64_t our_len = 0, shipped_len = 0;
    int64_t n_diff = 0;
    std::optional<int64_t> first_diff;
    int64_t masked = 0;
};

// Compare two byte strings ignoring masked positions on either side (sets of offsets).
DiffResult diff(const py::bytes& our, const py::set<int64_t>& omask, const py::bytes& shipped, const py::set<int64_t>& smask);

// the verdict as text; `our` and `shipped` (given) show the bytes around the first difference
py::str render(const DiffResult& res, const std::optional<py::bytes>& our = std::nullopt, const std::optional<py::bytes>& shipped = std::nullopt,
               const py::set<int64_t>& omask = py::set<int64_t>(), const py::set<int64_t>& smask = py::set<int64_t>());

// ---------------------------------------------------------------------------
// selftest: prove the mechanism without needing a retail anchor
// ---------------------------------------------------------------------------
extern const py::str GROUP;
bool _selftest(std::ostream& out);

int64_t main(const py::list<py::str>& argv, std::ostream& out, std::ostream& err);

}  // namespace codebreaker::source::ne_asm::segdiff
