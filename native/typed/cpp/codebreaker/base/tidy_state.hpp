// codebreaker/base/tidy_state.hpp -- typed C++ port of codebreaker.base.tidy_state.
//
// tidy_state.py -- how big state/ is, part by part, and what of it is stale, gone.
//
//     python -m codebreaker tidy-state              the sizes, and the stale removed
//     python -m codebreaker tidy-state --dry        the sizes, and what would be removed
//     python -m codebreaker tidy-state --stray      also remove what no part of CodeBreaker writes
//     python -m codebreaker tidy-state --keep-builds    leave Converted/*/build/ alone
//
// (The module's whole docstring, with the rules of what goes, is kept in tidy_state.cpp as DOC;
// `main` prints it for -h / --help.)
//
// Notes on the port:
//   * glob.glob has no counterpart in the runtime: a small glob() (the pattern rules of glob.glob on
//     Windows: * ? [seq], hidden names not matched by a wildcard that does not start with a dot) is
//     local to tidy_state.cpp;
//   * settings.ROOT is py::settings::checkout_root() (CB_CHECKOUT_ROOT);
//   * the Python tuples of names (RUN_TOOLS, NAMED_ELSEWHERE, OLD_CACHES) are py::list<py::str>.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::base::tidy_state {

extern const int64_t DAY;
extern const py::list<py::str> RUN_TOOLS;
// what the code names by a value, not by a literal: the period compilers' work folders, the
// setup ISO's folder (the setting win95_setup_iso).
extern const py::list<py::str> NAMED_ELSEWHERE;
// (the fingerprints' own file, before they were in the cache)
extern const py::list<py::str> OLD_CACHES;

// The module's docstring (what `main` prints for -h).
extern const char* const DOC;

int64_t _size(const py::str& path);
py::str _gb(int64_t n);

// The names at the top of state/ that CodeBreaker's code writes: read from the code itself
// (settings.state("<name>" ..., runs.<call>("<name>" ..., "state", "<name>"), so a new tool's
// folder is known the day it is written.
py::set<py::str> known();

bool _is_known(const py::str& name, const py::set<py::str>& names);

// [(path, bytes, rule)] of what the rules say can go (the cache apart: it prunes itself).
// `now` is time.time() when not given (or 0).
py::list<std::tuple<py::str, int64_t, py::str>> stale(const std::optional<double>& now = std::nullopt);

// Every Converted/<name>/build/ folder.
py::list<py::str> _builds();

// [(path, bytes)] at the top of state/ that no part of CodeBreaker writes.
py::list<std::tuple<py::str, int64_t>> stray();

// [(name, bytes)] of state/'s top-level parts, the biggest first.
py::list<std::tuple<py::str, int64_t>> parts();

bool _delete(const py::str& p);

// python -m codebreaker tidy-state [--dry] [--stray] [--keep-builds]
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::base::tidy_state
