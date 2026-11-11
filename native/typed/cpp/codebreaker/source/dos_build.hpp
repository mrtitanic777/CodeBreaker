// codebreaker/source/dos_build.hpp -- typed C++ from codebreaker.source.dos_build.
//
// dos_build.py -- `dos-build`: a real-mode program made from the folder dos-source wrote (dos_writer.rebuild).
//
// Typing notes:
//   * source/dos_writer.py has no typed port yet: its `rebuild` (the folder dos-source wrote -> the path of the program it makes) is
//     the hook `rebuild`, which the caller sets (the Python imports it by name: `from codebreaker.source.dos_writer import rebuild`).
//     main() raises py::RuntimeError when it is not set.
//   * main() returns the exit code (0 made / the same, 1 differs, 2 usage); DOC is main's docstring, which it prints for the usage.
#pragma once

#include <cstdint>
#include <functional>

#include "py.hpp"

namespace codebreaker::source::dos_build {

// source.dos_writer.rebuild(folder) -> the made program's path (set by the caller: no typed port of dos_writer yet)
extern std::function<py::str(const py::str& folder)> rebuild;

// main.__doc__ (the docstring with its common indentation removed, as CPython 3.13+ compiles it)
extern const py::str DOC;

// python -m codebreaker dos-build <source folder> [--against ORIGINAL]
//
// Make a real-mode program from the folder dos-source wrote, without the original.
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::source::dos_build
