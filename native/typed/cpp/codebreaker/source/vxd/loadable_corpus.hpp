// codebreaker/source/vxd/loadable_corpus.hpp -- typed C++ of codebreaker.source.vxd.loadable_corpus.
//
// loadable_corpus.py -- drive the whole VxD corpus through the from-source LOADABLE build.
//
// The object-level test (vxd_corpus) proves byte-exact CODE recovery. This proves the next thing: that the recovered source rebuilds, through the
// AUTHENTIC period toolchain (ML -> LINK 2.60 /VXD -> RC + ADRC2VXD), into a container the Win95 loader accepts -- device id stamped from the DDB,
// identity page map, version resource embedded. It runs vxd_from_source.build_from_source on every module and records the build stage reached and
// the container validation (device id, identity map, object count, resource length).
//
// Results checkpoint to JSON after every module (resumable). A boot is a separate, human-driven step; this measures BUILDABILITY and container
// validity at corpus scale, the static precondition for a boot.
//
//     python -m engine.inverse_engine.vxd_loadable_corpus <corpus>/oracle recovered/vxd_loadable.json
//     python -m engine.inverse_engine.vxd_loadable_corpus ... --redo vkd --workers 4
//
// Typing notes (see vxd/corpus.hpp: the same shapes):
//   * An entry of the results is the struct Entry: its `kind` says which of the Python's four dicts it is (the key order of each is kept when
//     the file is written): Built (ok, stage, seconds, objects, device_id, device_id_ok, identity, resource_len, vxd_len), Failed (ok, stage,
//     seconds, log), Exception (ok, stage, log, trace, seconds), Open (ok, stage, log: a file that would not open). A resumed file's entries
//     are read into these (a key they do not know is dropped when the file is written again).
//   * `trace` of an exception is traceback.format_exc()[-300:] in the Python; here the exception's type and message (the last 300 characters).
//   * ROOT is a function (settings.ROOT; the Python's was the `codebreaker` package folder, one dirname short since the move out of engine/).
//   * main(argv, out, err): the Python's `__main__` block (argv is sys.argv[1:]); the values of --redo / --workers are not taken for the
//     positional arguments (the Python was changed alike). `root` of run() is passed through os.path.abspath.
#pragma once

#include <cstdint>
#include <iosfwd>
#include <optional>
#include <tuple>

#include "codebreaker/formats/le.hpp"
#include "py.hpp"

namespace codebreaker::source::vxd::loadable_corpus {

namespace le = codebreaker::formats::le;

py::str ROOT();

struct Entry {
    enum class Kind { Built, Failed, Exception, Open };
    Kind kind = Kind::Failed;
    bool ok = false;
    std::optional<py::str> stage;
    double seconds = 0;
    int64_t objects = 0, device_id = 0, resource_len = 0, vxd_len = 0;
    bool device_id_ok = false, identity = false;
    py::str log, trace;
};

using Results = py::dict<py::str, Entry>;

// build_from_source of one module: the entry (an exception is an Exception entry)
Entry _one(const py::str& name, const le::LEFile& v, const py::str& root);

Results run(std::ostream& out, const py::str& srcdir, const py::str& out_json, const std::optional<py::list<py::str>>& redo = std::nullopt, int64_t workers = 4,
            const py::str& root = py::str("."));

void _save(const Results& results, const py::str& path);
py::str dump_results(const Results& results);
void _summary(std::ostream& out, const Results& results, double secs);

int64_t main(const py::list<py::str>& argv, std::ostream& out, std::ostream& err);

}  // namespace codebreaker::source::vxd::loadable_corpus
