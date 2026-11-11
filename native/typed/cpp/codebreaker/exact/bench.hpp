// codebreaker/exact/bench.hpp -- typed C++ of codebreaker.exact.bench
//
// bench.py -- the inverse model held to a project whose sources are known: what it drafts from no source, beside
// the real source, with the compiler's verdict. The measure of the model, and where it fails.
//
//     python -m codebreaker exact PROJECT bench [ADDR..] [--show] [--out FILE]
//
// Every function the project has a one-function source for (src/fADDR.c) is drafted from the program's bytes
// alone and proven as the draft stage proves it. The verdicts are counted; --show prints each draft beside its
// source; --out writes one line a function (address, verdict) for comparing two runs.
//
// Typing notes:
//   * The inverse model is draft::InverseModel, loaded by the project's draft view (project::as_draft: invert::early_c,
//     the model a toolchain profile names); the draft stage's model_texts and _names are the typed port
//     (exact/draft/package.hpp), and draft.prove.prove_texts is exact/draft/prove (the project's draft view calls it).
//   * A row is (address, verdict, source).  Where proving raises, the Python's source is traceback.format_exc(); here it is
//     "Traceback (most recent call last):\n" + "<Type>: <message>\n" (the frames are the Python's own).
//   * main() prints to `out` (std::cout by default).
#pragma once

#include <cstdint>
#include <iostream>
#include <optional>
#include <tuple>

#include "codebreaker/exact/project.hpp"
#include "py.hpp"

namespace codebreaker::exact::bench {

// [(address, verdict, source)]
py::list<std::tuple<int64_t, py::str, py::str>> run(project::Project& prj, const std::optional<py::list<int64_t>>& want = std::nullopt, bool show = false,
                                                    std::ostream& out = std::cout);

int64_t main(project::Project& prj, const py::list<py::str>& argv, std::ostream& out = std::cout);

}  // namespace codebreaker::exact::bench
