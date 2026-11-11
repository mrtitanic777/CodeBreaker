// codebreaker/source/ne_asm/idiom.hpp -- typed C++ of codebreaker.source.ne_asm.idiom.
//
// ne_idiom.py -- re-emit a 16-bit NE code segment as idiomatic Microsoft SDK cMacros source (cProc / cBegin / cEnd), the
// KERNEL/USER/GDI analog of the VxD `vxd_idiom.py`.  The mechanical Track-A form (flat `_TEXT segment`, numeric labels,
// cross-function branches pinned as `db`) becomes readable, structured source that still assembles BYTE-EXACT through the
// period DDK ML.EXE + the Windows SDK CMACROS.INC.
//
// Assembly recipe (proven on the KRNL386 RIP validation cluster):
//     OPTION M510        ; MASM 5.10 compat -- cMacros v5.20 uses the old `&' paste
//     .286p              ; lar/lsl and friends
//     ?PLM = 1 / ?WIN = 1
//     include cmacros.inc
//     sBegin CODE ... sEnd CODE
//     ml.exe /c /Ic:\inc /Fom.obj m.asm      (16-bit OMF, no -coff)
// _TEXT is extracted from the OMF (LEDATA) and compared with NE relocations masked -- identical after the linker resolves
// the fixups, exactly like the VxD path.
//
// Typing notes:
//   * The Python's lazy-import helpers (_masm, _segdiff, _segment, _msvc152) have no C++ counterpart; _Corpus /
//     _target() are the struct Corpus (the old target registry's `resolve`).
//   * _branch's (op, target) with (None, None) for "not a direct branch" is std::optional of the pair.
//   * _emit's (text, unresolved) is the struct Emitted; `funcs` (None or a list of function starts) is std::optional.
//     `seg_i` is kept though the Python never reads it. force_db and data_off are sets of offsets.
//   * assemble / assemble_nasm's {obj: bytes|None, lst: str, log: str} is the struct AsmResult (obj: nullopt for None).
//   * reconcile_range's (asm_text, result, force_db, n_code) is recon's struct Reconciled (its `res` is the segdiff
//     DiffResult or none); `backend` is a str ("ml" or "nasm": any other behaves as "ml", as in the Python).
//   * verify_range's dict is the struct Verified (`res` is present only when the range is not exact and a diff was made).
//   * The Python's `if __name__ == "__main__"` block (it reads sys.argv) is main(argv, out, err): argv is sys.argv[1:], the return
//     is the exit status (the 1 of its sys.exit(1)), what it prints goes to `out`. int() of a bad number argument is the
//     ValueError the Python raises.
//   * The Python's assemble() left the DOS tool's work folder behind (a temp folder per call); it is removed here and in the
//     Python (it was only assemble_nasm that cleaned up).
#pragma once

#include <cstdint>
#include <iosfwd>
#include <optional>
#include <tuple>

#include "codebreaker/source/ne_asm/recon.hpp"
#include "codebreaker/source/ne_asm/segment.hpp"
#include "py.hpp"

namespace codebreaker::source::ne_asm::idiom {

// What the old target registry answered: where a module of the corpus is.
struct Corpus {
    static py::str resolve(const py::str& module);
};
Corpus _target();

// Direct-branch mnemonics whose single operand is a code offset (objdump Intel).
extern const py::set<py::str> _JMP;
extern const py::set<py::str> _JCC;
extern const py::set<py::str> _CALL;

extern const py::list<py::str> _PREAMBLE;

// (op, target_off) for a direct jmp/jcc/call to a hex offset, else nothing.
std::optional<std::tuple<py::str, int64_t>> _branch(const py::str& objasm);

// A cProc-legal name for the function at `off`: the export name if known, else a synthesized `sub_<off>`.
// (Byte-exactness never depends on the name.)
py::str _procname(const segment::Seg& sg, int64_t off);

struct Emitted {
    py::str text;
    // the segment offsets whose bytes are cross-range (extern) branch displacements -- unresolved (0000) in an isolated OBJ but
    // correct once the whole segment is present, so range verification masks them exactly like a relocation
    py::set<int64_t> unresolved;
};

// Emit functions covering [start,end) as idiomatic cMacros source text.
//
// force_db: offsets to pin as `db` (divergers the reconcile loop found).
// data_off: offsets inside safe-data regions to pin as `db`.
//
// funcs: sorted list of function START offsets in the range (defaults to the carve's `sg.funcs`).  Every function is emitted
// `cProc name,<NEAR>` / `cBegin nogen` / body / `cEnd nogen` -- `nogen' adds no bytes, so the body is reproduced verbatim while
// gaining structure, names and symbolic branches.
Emitted _emit(const segment::Seg& sg, int64_t start, int64_t end, const std::optional<py::list<int64_t>>& funcs = std::nullopt, int64_t seg_i = 1,
              const py::set<int64_t>& force_db = py::set<int64_t>(), const py::set<int64_t>& data_off = py::set<int64_t>());

// verify: assemble via ML + CMACROS.INC (16-bit OMF) and diff _TEXT vs shipped
py::str _stage_cmacros(const py::str& root);

struct AsmResult {
    std::optional<py::bytes> obj;
    py::str lst, log;
};

// Assemble idiomatic cMacros source with ML + CMACROS.INC (16-bit OMF). The `lst' drives db-fallback.
AsmResult assemble(const py::str& asm_text, const py::str& root = py::str("."));

// DOSBox-free backend: the nasm-masm fork assembles cMacros natively (its cmacros_shim.inc replaces CMACROS.INC), ~100x faster than
// ML-under-DOSBox. nasm picks different-but-equivalent encodings than ML on ambiguous cases, so the reconcile db-pins nasm's OWN
// divergers (found by direct byte compare, since nasm's listing format differs from ML's).

// settings.json `nasm` (nasm.exe of the nasm-masm project), else the NASM_MASM env var.
py::str _nasm_bin();

// A space-free include dir holding cmacros_shim.inc (nasm redirects `include cmacros.inc' -> it).
py::str _nasm_inc();

// Assemble idiomatic cMacros source with the nasm-masm fork (16-bit OMF), no DOSBox.
AsmResult assemble_nasm(const py::str& asm_text);

// nasm listing byte line: "<src#> <offset8> <bytes...> <source ; NNNN>" (shim-expansion lines carry a `<N>' marker and no offset
// -- they don't match).
extern const py::str _NLST;   // the pattern

// asmrecon._divergers for the nasm-masm listing format (line#/src#/offset8): (lendiff, reencode) sets of original offsets to db,
// exactly like the ML path: fix length-differers first, then same-length re-encodes.
std::tuple<py::set<int64_t>, py::set<int64_t>> _nasm_divergers(const py::str& lst, const py::bytes& our, const py::set<int64_t>& omask,
                                                               const py::bytes& shipped, const py::set<int64_t>& smask,
                                                               const py::dict<int64_t, int64_t>& spans, int64_t start);

// Emit [start,end) idiomatic, assemble via ML+CMACROS, and db-fallback each diverging instruction until byte-exact -- asmrecon's
// reconcile technique with the cMacros emitter.  A whole range is one assembly unit, so one instruction ML re-encodes (length or
// same-length) shifts the rest: find those via the listing and pin them as `db` (byte-exact), keeping everything else live and
// idiomatic.
recon::Reconciled reconcile_range(const segment::Seg& sg, int64_t start, int64_t end, const std::optional<py::list<int64_t>>& funcs = std::nullopt,
                                  const py::str& root = py::str("."), int64_t max_passes = 60, const py::str& backend = py::str("ml"));

struct Verified {
    bool ok = false;
    int64_t n = 0, n_code = 0, db = 0, live = 0;
    py::str asm_;
    std::optional<segdiff::DiffResult> res;   // only when not exact and a diff was made
};

// db-fallback [start,end) to byte-exact idiomatic cMacros source: ok, n_code (instructions), db (pinned), live, and the asm.
// backend='nasm' uses the fast DOSBox-free nasm-masm fork (byte-exact via its own db-fallback); 'ml' uses period ML+CMACROS under
// DOSBox.
Verified verify_range(const segment::Seg& sg, int64_t start, int64_t end, const std::optional<py::list<int64_t>>& funcs = std::nullopt,
                      const py::str& root = py::str("."), const py::str& backend = py::str("ml"));

int64_t main(const py::list<py::str>& argv, std::ostream& out, std::ostream& err);

}  // namespace codebreaker::source::ne_asm::idiom
