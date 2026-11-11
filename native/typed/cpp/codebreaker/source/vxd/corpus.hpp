// codebreaker/source/vxd/corpus.hpp -- typed C++ of codebreaker.source.vxd.corpus.
//
// corpus.py -- drive the whole VxD corpus through object-level assemble-and-compare.
//
// Runs vxd_masm.compare_module on every VxD (and every module of the vmm32 W3 bundle), proving -- or measuring -- byte-exact CODE
// recovery across all ~215 drivers. It is the corpus-scale version of the single-driver test: emit each object as MASM, assemble with
// the period DDK ML.EXE, and compare the assembled image to the oracle object byte-for-byte.
//
// Results are checkpointed to a JSON after every module, so the run is RESUMABLE -- re-invoking skips modules already recorded. This
// matters because a full pass is many hours of DOSBox assembly.
//
//     python -m engine.inverse_engine.vxd_corpus <corpus>/oracle results.json
//     python -m engine.inverse_engine.vxd_corpus ... --redo vkd    # force-refresh one module
//
// Typing notes:
//   * The results dict {key: record} is Results = py::dict<py::str, Entry>; an Entry is a record of a module (ModuleRecord, its objects
//     ObjectRow) or {error, trace} (the Python's `{"error": ...}` of a file that would not open has no trace: `trace` is then none).
//     A resumed file's entries are read into these (a key they do not know is dropped when the file is written again).
//   * The `trace` of a module that raised is traceback.format_exc()[-300:] in the Python; a C++ exception has no traceback, so it is
//     the exception's type and message (also last 300 characters) here.
//   * ROOT is a function (settings.ROOT: the Python's `dirname(dirname(HERE))` was the `codebreaker` package folder, one dirname short
//     since the module moved out of engine/; the Python was changed alike). The Python's sys.path insert has no counterpart.
//   * Everything the Python prints goes to `out` (print()); `workers` run as threads (jobs/pool as_done); with one worker the order is
//     the items'.
//   * main(argv, out, err): the Python's `__main__` block; argv is sys.argv[1:]; the return is the exit status. The values of `--redo`
//     and `--workers` are no longer taken for the positional arguments (the Python's `[a for a in sys.argv[1:] if not a.startswith("--")]`
//     did, so `--workers 4 DIR` ran the folder "4"; the Python was changed alike).
#pragma once

#include <cstdint>
#include <iosfwd>
#include <optional>
#include <tuple>

#include "codebreaker/formats/le.hpp"
#include "py.hpp"

namespace codebreaker::source::vxd::corpus {

namespace le = codebreaker::formats::le;

py::str ROOT();

struct ObjectRow {
    std::optional<int64_t> obj;
    bool match = false, ok = false;
    std::optional<int64_t> len, symbolic, forced_raw;
    bool raw_fallback = false;
    std::optional<int64_t> diffs;
    std::optional<py::str> err;
};

struct ModuleRecord {
    bool byte_exact = false;
    py::list<ObjectRow> objects;
    int64_t n_obj = 0, n_exact = 0, symbolic = 0, forced_raw = 0, raw_fallback_objs = 0;
    double seconds = 0;
};

struct Entry {
    std::optional<ModuleRecord> record;   // a module's record, or
    std::optional<py::str> error;         // {"error": ..., "trace": ...}
    std::optional<py::str> trace;
};

using Results = py::dict<py::str, Entry>;

// compare_module of one module: its record, or {error, trace}
Entry _one_module(const py::str& name, const le::LEFile& v);

// VxDs also ship as IOS port/mini-port drivers (.pdr/.mpd) and a few under misc extensions -- all LE Ring-0 modules the engine
// reconstructs identically. Include them so the corpus is the COMPLETE set.
extern const py::list<py::str> _VXD_EXTS;
extern const py::list<py::str> _EXTRA_FILES;

// True if `path` is an LE/LX/W3/W4 Ring-0 module (so a stray real-mode .sys/.drv is skipped).
bool _is_le_vxd(const py::str& path);

// Assemble-and-compare every module of the corpus, `workers` in parallel (each an independent DOSBox), checkpointing to out_json after
// every module so the run resumes where it stopped.
Results run(std::ostream& out, const py::str& srcdir, const py::str& out_json, const std::optional<py::list<py::str>>& redo = std::nullopt,
            int64_t workers = 4);

// results as json.dump(results, f, indent=1) writes them (to a temp file, then os.replace)
void _save(const Results& results, const py::str& path);

// the text json.dump(results, indent=1) writes
py::str dump_results(const Results& results);

void _summary(std::ostream& out, const Results& results, double secs);

int64_t main(const py::list<py::str>& argv, std::ostream& out, std::ostream& err);

}  // namespace codebreaker::source::vxd::corpus
