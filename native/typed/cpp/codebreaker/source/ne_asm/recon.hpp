// codebreaker/source/ne_asm/recon.hpp -- typed C++ of codebreaker.source.ne_asm.recon.
//
// asmrecon.py -- deterministic byte-exact reconstruction of hand-asm (stages C+F).
//
// The 16-bit core (KERNEL/USER/GDI, the .drv/.386 drivers) was hand-written assembly. The faithful reconstruction is
// assembly that RE-ASSEMBLES to the shipped bytes -- and that can be produced mechanically, with NO LLM and NO permuter:
//
//   1. disassemble the carved function (trace.Seg, objdump-verified)
//   2. transcribe each instruction to MASM syntax; intra-function branches become labels (their self-relative
//      displacement is position-independent, so it reproduces exactly); data / external calls become `db`
//   3. assemble (masmlift) + diff (segdiff); any instruction MASM does not encode identically is rewritten
//      `db <original bytes>` and we retry
//
// The db-fallback GUARANTEES convergence to byte-exact (worst case: all db). The score is "% live" -- how much
// reassembles as real instructions vs db. This is the sibling projects' proven technique, here composed from the
// CodeBreaker tools.
//
//     python asmrecon.py --selftest            # reconstruct a real function, prove exact
//     python asmrecon.py KEYBOARD --seg 1 --func 0x0100
//
// Typing notes:
//   * The Python's lazy-import helpers (_masm, _segdiff, _segment, _msvc152: they only return modules) have no C++
//     counterpart; _Corpus / _target() / `target` are the struct Corpus (the old target registry's `resolve`).
//   * A callable `is_target(off) -> label | None` is std::function<std::optional<py::str>(int64_t)>.
//   * reconcile()'s tuple (asm_text, result, force_db, n_code_instr) is the struct Reconciled; its `res` is the segdiff
//     DiffResult, or none when nothing assembled. `asm` is set before the loop (the Python leaves it unbound, so
//     max_passes <= 0 raised UnboundLocalError; it is "" here -- the Python was changed alike).
//   * _pick()'s (name, seg, func) is std::optional of the tuple (the Python's (None, None, None) is nullopt).
//   * _run() returns (asm, DiffResult-or-none); sys.exit(text) in it is py::SystemExit(text).
//   * main(argv, out, err): argv is sys.argv[1:]; it returns the exit status instead of calling sys.exit; the messages of
//     sys.exit(text) go to `err`. The command line is read by hand (module, --seg N, --func N (int(x, 0)), --selftest,
//     --out FILE, -h/--help; `--opt=value` too): a bad one prints a one-line usage and error to `err` and gives 2;
//     argparse's abbreviations and exact wording are not reproduced.
#pragma once

#include <cstdint>
#include <functional>
#include <iosfwd>
#include <optional>
#include <tuple>

#include "codebreaker/formats/omf.hpp"
#include "codebreaker/source/ne_asm/segdiff.hpp"
#include "codebreaker/source/ne_asm/segment.hpp"
#include "py.hpp"

namespace codebreaker::source::ne_asm::recon {

// What the old target registry answered: where a module of the corpus is.
struct Corpus {
    static py::str resolve(const py::str& module);
};
Corpus _target();

extern const py::set<py::str> JMPCC;

// 0x1f -> "1fh", 0xff -> "0ffh" (a MASM number does not start with a letter)
py::str masm_hex(int64_t v);

using IsTarget = std::function<std::optional<py::str>(int64_t)>;

// One objdump (Intel) instruction -> MASM syntax. is_target(off)->label|None.
py::str to_masm(const py::str& asm_, const IsTarget& is_target);

extern const py::list<py::str> RETF_MACRO;

bool _cancelled();

py::str _dbline(const py::bytes& raw, int64_t off, const py::str& note);

// Append db line(s) for `raw`, chunked at 16 bytes/line so no line exceeds MASM's ~128-char limit (objdump collapses
// zero runs, so a single 'instruction' row can span 100+ data bytes).
void _emit_db(py::list<py::str>& lines, const py::bytes& raw, int64_t off, const py::str& note);

// Emit a DATA region as READABLE data: printable text as `db 'text'`, else db hex. Byte-exact either way (same bytes).
void _emit_data(py::list<py::str>& lines, const py::bytes& code, int64_t s, int64_t e, const py::str& kind);

// MASM body lines for the function [start,end). force_db rows, external branches and far calls become db.
// `data_spans` [(s,e,kind)] (safe, row-aligned, no branch target inside) are emitted as readable DATA.
py::list<py::str> emit_function(const segment::Seg& sg, int64_t start, int64_t end, const py::set<int64_t>& force_db,
                                const py::list<std::tuple<int64_t, int64_t, py::str>>& data_spans = py::list<std::tuple<int64_t, int64_t, py::str>>());

py::str wrap(const py::list<py::str>& body);

// Per-instruction comparison via the MASM listing: (length-differers, same-length re-encodes), as sets of original
// offsets (see the Python's docstring for the two-phase use).
std::tuple<py::set<int64_t>, py::set<int64_t>> _divergers(const py::str& lst_text, const py::bytes& our, const py::set<int64_t>& omask,
                                                          const py::bytes& shipped, const py::set<int64_t>& smask,
                                                          const py::dict<int64_t, int64_t>& spans, int64_t start);

// Safe data regions within [start,end): from reachability (no branch target inside), snapped to row boundaries, then
// RE-checked so snapping never absorbs a branch target. Guarantees no needed label is dropped -> byte-exact safe.
py::list<std::tuple<int64_t, int64_t, py::str>> _safe_data_spans(const segment::Seg& sg, int64_t start, int64_t end);

struct Reconciled {
    py::str asm_;
    std::optional<segdiff::DiffResult> res;
    py::set<int64_t> force_db;
    int64_t n_code = 0;
};

// Reconstruct [start,end) to byte-exact via db-fallback. Safe data regions are emitted as readable data and excluded
// from the code live count.
Reconciled reconcile(const segment::Seg& sg, int64_t start, int64_t end, const py::set<int64_t>& seg_mask, int64_t max_passes = 200,
                     bool verbose = true);

// Find a real, substantive (16..120 byte) carved function to reconstruct: (module, segment, function start).
std::optional<std::tuple<py::str, int64_t, int64_t>> _pick();

std::tuple<py::str, std::optional<segdiff::DiffResult>> _run(const py::str& module, int64_t seg_index, std::optional<int64_t> func_off,
                                                             std::ostream& out);

extern const py::str USAGE;

int64_t main(const py::list<py::str>& argv, std::ostream& out, std::ostream& err);

}  // namespace codebreaker::source::ne_asm::recon
