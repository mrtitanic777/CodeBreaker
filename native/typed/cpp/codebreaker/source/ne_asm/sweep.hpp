// codebreaker/source/ne_asm/sweep.hpp -- typed C++ of codebreaker.source.ne_asm.sweep.
//
// ne_sweep.py -- run the idiomatic NE re-emitter (ne_idiom) across the whole 16-bit NE corpus, unattended.  The KERNEL/USER/GDI
// analog of the PE `engine/targets/win95/l2sweep.py`: for every CODE segment of every NE module it reconciles the segment to
// BYTE-EXACT idiomatic Microsoft SDK cMacros source (via the DOSBox-free nasm-masm backend, db-pinning only what nasm re-encodes),
// and banks a corpus coverage ledger.
//
// This is the structural piece the NE track lacked -- ne_idiom.py was CLI-only, one <module> <seg> <lo> <hi> at a time.  With this,
// the ~309 not-yet-idiomatic NE modules reconstruct in one pass.
//
//     python -m engine.inverse_engine.ne_sweep [--limit N] [--start K] [--emit DIR]
//                                              [--list] [--backend nasm|ml]
//
// Typing notes:
//   * The Python's lazy-import helpers (_masm, _segdiff, _segment, _msvc152) have no C++ counterpart; _Corpus / _target() are the
//     struct Corpus (the old target registry's `resolve`).
//   * ROOT is the checkout root: a function (settings.ROOT); the Python's was the `codebreaker` package folder (one dirname short
//     since the module moved out of engine/) and was changed to settings.ROOT. _setup() only changes the working folder: the
//     `sys.path.insert(0, ".")` of the Python has no counterpart.
//   * ne_modules() looked in <corpus>/SYSTEM, /SYSTEM32, the corpus folder and /COMMAND; the corpus holds WINDOWS/SYSTEM (and WINDOWS,
//     WINDOWS/COMMAND), so it found no module at all. Those folders are looked in first now (the Python was changed alike).
//   * The per-module record dict is the struct ModuleRecord (its per-segment dicts SegRecord); sweep()'s summary list holds a record
//     or, for a module that raised, {module, error}: the struct Summary.
//   * Everything the Python prints goes to `out` (the caller's std::ostream; print()); the ledger file is written exactly as
//     json.dump(..., indent=1) writes it.
//   * main(argv, out, err): argv is sys.argv[1:], the return is the exit status. The command line is read by hand (--limit N,
//     --start K, --emit DIR, --backend nasm|ml, --list, -h/--help; `--opt=value` too): a bad one prints a one-line usage and error
//     to `err` and gives 2; argparse's abbreviations and exact wording are not reproduced.
#pragma once

#include <cstdint>
#include <iosfwd>
#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::source::ne_asm::sweep {

// What the old target registry answered: where a module of the corpus is.
struct Corpus {
    static py::str resolve(const py::str& module);
};
Corpus _target();

// the checkout root (settings.ROOT)
py::str ROOT();

// the sweep works from the checkout root
void _setup();

// Every NE module under the system tree, de-duplicated by basename: [(name lower-cased, path)].
py::list<std::tuple<py::str, py::str>> ne_modules();

py::str _resolve(const py::str& module);

struct SegRecord {
    int64_t seg = 0, n = 0;
    bool ok = false;
    int64_t n_code = 0, db = 0, live = 0;
};

struct ModuleRecord {
    py::str module;
    py::list<SegRecord> segs;
    bool ok = false;
    int64_t nseg = 0, nok = 0, code = 0, live = 0;
};

// Reconcile every CODE segment of one NE module to idiomatic cMacros. Returns a per-module record with per-segment coverage.
// `emit`: a folder; each byte-exact segment's source is written to <emit>/<module tag>/<tag>_segNN.asm.
ModuleRecord sweep_module(const py::str& module, const py::str& path, const py::str& backend = py::str("nasm"),
                          const std::optional<py::str>& emit = std::nullopt);

struct Summary {
    py::str module;
    std::optional<ModuleRecord> record;   // the module's record, or
    std::optional<py::str> error;         // the (first 200 characters of the) message of what it raised
};

py::list<Summary> sweep(std::ostream& out, std::optional<int64_t> limit = std::nullopt, int64_t start = 0,
                        const std::optional<py::str>& emit = std::nullopt, const py::str& backend = py::str("nasm"));

int64_t main(const py::list<py::str>& argv, std::ostream& out, std::ostream& err);

}  // namespace codebreaker::source::ne_asm::sweep
