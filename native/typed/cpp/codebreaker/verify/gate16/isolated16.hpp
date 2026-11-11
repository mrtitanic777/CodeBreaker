// codebreaker/verify/gate16/isolated16.hpp -- interface of codebreaker.verify.gate16.isolated16 (typed C++; implementation pending).
//
// isolated16.py -- each function of a rebuilt 16-bit module run beside the original's, on its own.
//
//     for (auto& r : isolated16::check(original_path, build_folder, names)) { r.name, r.verdict, r.why }
//
// Both are entered the way the function is called (far or near), on the same arguments, with the original's data segment
// on both sides.  What one function calls -- another of the module's functions, a Windows function, an interrupt -- is not
// run: it is recorded (who, with which argument words) and answered at once with the same made-up result on both sides
// (emulate/windows16, `callees`).  Compared, for each set of inputs: how the run ends; the calls it makes, in order, with
// their arguments; the result (AX, and DX for a function that returns four bytes); what is left in the data segment (and
// through far pointers); how many bytes of arguments were removed; whether SI, DI and DS were kept.  The inputs are the
// 16-bit judge's (verify/lanes/win16): fixed, directed, branch-guided / path by path, and the scattered ones again over a
// patterned data segment.  CB_NO_GUIDED=1 leaves the branch-guided ones out.
//
// Typing notes:
//   * check() is a generator in Python; here it returns the list of what it would have yielded, in order.  A verdict is
//     the judge's word (verify/verdicts.py: "equivalent", "divergent", "inconclusive").
//   * "tables" (the Python dict {funcs, wide, place}, kept as JSON in the cache and handed to worker processes) is `Tables`.
//   * The private fingerprint helpers (_word, _input, _fingerprint, _differ, _built_places, _part, _original_key) are
//     internal to the .cpp; only what other modules or tests use is declared.
//   * `explored` (guided_inputs): the dict the caller gives to be filled is `ExploredSummary`; its "runs" are keyed by the
//     index in the returned list (the Python: id() of the returned input).
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>
#include <vector>

#include "codebreaker/core16_types.hpp"
#include "codebreaker/decompile/c_text.hpp"
#include "codebreaker/emulate/windows16.hpp"
#include "codebreaker/verify/concolic.hpp"
#include "codebreaker/verify/lanes/win16.hpp"
#include "py.hpp"

namespace codebreaker::verify::gate16::isolated16 {

// What check() needs of the decompiled module (made once, and kept)
struct Tables {
    py::dict<py::str, std::tuple<int64_t, bool>> funcs;   // {name: (bytes of arguments it removes (-1: not known), far)}
    py::list<py::str> wide;                               // functions with a four-byte result (sorted)
    py::dict<py::str, core16::SegOff> place;              // {name: (segment, offset)} in the original
};

// {name: (bytes of arguments it removes, far)} from the description's F lines.   used by: engine.win16
py::dict<py::str, std::tuple<int64_t, bool>> _functions(const decompile::c_text::Module16& m);

// What check() needs of the decompiled module, kept by the original and all that makes the decompiled text.
Tables _tables(const py::str& original);

// What guided_inputs hands back besides the inputs: the exploration's summary ("paths", "all": complete, "spent": runs;
// empty when there was no exploration) and, when it was made now, the records of the runs that made the returned inputs.
struct ExploredSummary {
    std::optional<int64_t> paths;
    bool all = false;
    int64_t spent = 0;
    py::dict<int64_t, emulate::windows16::Trace16> runs;   // index in the returned list -> the record of its run
};

// Branch-guided inputs: arguments, words of the data segment and what an unmodelled Windows call returns, chosen to take
// paths of the original no other input took -- the original run as this checker runs it, its callees answered.  Kept,
// keyed on the original's side; none when CB_NO_GUIDED is set.
//   `known`: {seed index: trail} of seeds already run; `explored`: filled with the exploration's summary.
py::list<lanes::win16::Vector> guided_inputs(const py::str& original, const py::str& original_key, const py::str& name,
                                             int64_t oseg, int64_t ooff, bool far_, const std::tuple<int64_t, int64_t>& span,
                                             const py::list<lanes::win16::Vector>& seeds,
                                             const std::optional<emulate::windows16::Callees>& callees,
                                             const std::optional<std::tuple<int64_t, int64_t>>& data_end,
                                             int64_t insn_cap = 20000,
                                             const std::optional<py::dict<int64_t, verify::concolic::Trail>>& known = std::nullopt,
                                             ExploredSummary* explored = nullptr);

// One result of check(): (name, verdict, why).
struct Checked {
    py::str name;
    py::str verdict;
    py::str why;
};

// Each named function (one the build has as C) beside the original's.  Many at once: shared out over processes.
//   image: the built module (default: the file in `folder` named as the original); inputs: replaces the usual inputs;
//   workers: processes (default CB_GATE_WORKERS or base.parallel.workers()); tables: from _tables(original) when absent;
//   chain: the module's other functions run for real on both sides.
py::list<Checked> check(const py::str& original, const py::str& folder, const py::list<py::str>& names,
                        const std::optional<py::str>& image = std::nullopt,
                        const std::optional<py::list<lanes::win16::Vector>>& inputs = std::nullopt,
                        const std::optional<int64_t>& workers = std::nullopt, const std::optional<Tables>& tables = std::nullopt,
                        bool chain = false);

// A worker process's share (jobs/workers): args = original, folder, image or "-", tables file, names file, answer file.
int worker_main(const std::vector<std::string>& args);

}  // namespace codebreaker::verify::gate16::isolated16
