// codebreaker/verify/sweep.hpp -- typed C++ of codebreaker.verify.sweep.
//
// sweep.py -- reconstruct and judge every module of the corpus, smallest first, resumably.
//
//     python -m codebreaker sweep                    every 32-bit module not yet measured
//     python -m codebreaker sweep --only a.dll,b.dll these
//     python -m codebreaker sweep --only @list.txt   one module name per line
//     python -m codebreaker sweep --16               the 16-bit modules; --drivers the drivers;
//                                                    --dos real-mode programs; --all every kind
//     python -m codebreaker sweep --status           what has been measured; no work
//     python -m codebreaker sweep --again            measure modules again even if already done
//
// A module counts as measured when every one of its functions has a result under the current
// verifier settings, so an interrupted sweep picks up at the module it stopped in, and changing
// the verifier makes the whole corpus due again without anyone having to say so. One module that
// fails outright is reported and the sweep moves on.
//
// Typing notes:
//   * `log` is the Python's `log=print`: an empty std::function is print.
//   * status() returns {path: module hash} in the order of `paths` (an insertion-ordered dict).
//   * `_wanted` is nullopt for "no --only" (the Python's None), else the lower-cased names.
//   * An exception of the module's run other than py::SystemExit (a BaseException in Python) is reported and the sweep
//     goes on; SystemExit ends it as it does there.
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>

#include "codebreaker/engine/native_engine.hpp"
#include "py.hpp"

namespace codebreaker::verify::sweep {

extern const py::str DOC;

std::optional<py::set<py::str>> _wanted(const py::list<py::str>& argv);

// Report what has been measured of `paths` under the settings in force for each one's kind. -> {path: module hash} of those with results.
py::dict<py::str, py::str> status(const std::shared_ptr<engine::native_engine::FunctionStore>& fs, const py::list<py::str>& paths,
                                  const std::function<void(const py::str&)>& log = nullptr);

int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::verify::sweep
