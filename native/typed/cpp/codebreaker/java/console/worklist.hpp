// codebreaker/java/console/worklist.hpp -- C++17 port of codebreaker.java.console.worklist.
//
// worklist.py -- compile the converted output and write a per-file failure worklist.
//
//     python -m codebreaker java worklist <out_dir> [--cc g++] [--std c++03] [--out WORKLIST.md]
//
// For each <out_dir>/*.cpp it runs `<cc> -std=<std> -fsyntax-only -I<out_dir> <file>` and records
// the files that fail to compile, sorted by error count (fewest first = closest to clean), with the
// dominant error category.
//
// Standard-library stand-ins (the runtime has none): glob.glob("<out_dir>/*.cpp") and
// subprocess.run([...], stderr=PIPE, stdout=DEVNULL) are small helpers in worklist.cpp.
// subprocess is the Windows CreateProcess one (as Python's is on this platform).
#pragma once

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::java::console::worklist {

// re.compile(r': error:') and re.compile(r'error: (.*)'), compiled once
const py::re::Pattern& _ERR();
const py::re::Pattern& _CAT();

// The most common error message in a compiler's stderr, with quoted names, numbers and long tails
// made alike ("(unknown)" when no line has an `error: `).
py::str _category(const py::str& text);

// Compile every <out_dir>/*.cpp with `cc` (default: settings host_cxx, else g++), write the worklist
// to `worklist_path` (default WORKLIST.md) and print a one-line summary. `std_` is the Python's `std`
// (renamed: `std` is C++'s namespace). -> 0
int64_t run(const py::str& out_dir, const std::optional<py::str>& cc = std::nullopt,
            const py::str& std_ = py::str("c++03"), const std::optional<py::str>& worklist_path = std::nullopt);

}  // namespace codebreaker::java::console::worklist
