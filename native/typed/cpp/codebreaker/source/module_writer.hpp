// codebreaker/source/module_writer.hpp -- interface of codebreaker.source.module_writer (typed C++; implementation pending).
//
// module_writer.py -- one source file per module, every function labelled with what is known.
//
//     module_writer::write_module("comctl32.dll")    reconstruct, judge, then write the file
//     module_writer::scoreboard()                    rewrite the scoreboard only
//
// Each function carries one status line (VERIFIED, VERIFIED (weak), UNVERIFIED, NOT JUDGED, DOES NOT COMPILE,
// NO SOURCE, IDENTIFIED (runtime library)).
//
// Typing notes:
//   * `fs` is the function store; `row` of describe() is a store row as best() gives it (native_engine::Candidate:
//     class in `klass`, `weak`, `limits`, `reason`, `detail` ...).
//   * write_module() returns (file written, {class: count}, number of functions) for a module, a driver or a real-mode
//     program; for a Java program (a folder, .java or .jar) it returns write_java()'s folder instead: a variant.
//   * `log` defaults to print (nullptr here); `used` is a set of names (the Python passes a set or a tuple).
//   * write_module raises py::SystemExit("no module ... in the corpus") for an unknown module.
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>
#include <variant>

#include "codebreaker/engine/native_engine.hpp"
#include "py.hpp"

namespace codebreaker::source::module_writer {

using FunctionStorePtr = std::shared_ptr<engine::native_engine::FunctionStore>;
using Counts = py::dict<py::str, int64_t>;
// (file written, counts by class, number of functions)
using ModuleWritten = std::tuple<py::str, Counts, int64_t>;
using Log = std::function<void(const py::str&)>;

// verdict reason -> the words that say it
extern const py::dict<py::str, py::str> WHY;
extern const py::list<py::str> ORDER;               // = verify::verdicts::SCORED
extern const py::dict<py::str, py::str> TITLE;      // = verify::verdicts::TITLE

// The words after the status for one function's best candidate.
py::str describe(const engine::native_engine::Candidate& row);

// {symbol: name} for a 16-bit module's imports by number that can be written by their real name (a PowerPC image's
// are console_import_names).
py::dict<py::str, py::str> real_import_names(const py::str& path, const py::set<py::str>& used = py::set<py::str>());
// {symbol: name} for a PowerPC image's imports (`import0_ord_16` -> the console's name).
py::dict<py::str, py::str> console_import_names(const py::str& path, const py::set<py::str>& used = py::set<py::str>());

// A driver's or a real-mode program's source: its one assembly file, the status at its head.
ModuleWritten write_assembly(const py::str& path, const FunctionStorePtr& fs, const py::str& sha, const py::str& digest,
                             const py::str& out_dir, const Log& log = nullptr);

// Is the file at `path` one CodeBreaker wrote -- and so may be written again?  (a file not there is ours to write)
bool made_by_us(const py::str& path);
// Is the file one of the selected product's originals?
bool in_corpus(const py::str& path);

// What write_module answers: a Java program's folder, or the module's ModuleWritten.
using Written = std::variant<py::str, ModuleWritten>;
Written write_module(const py::str& module, bool verify = true, const std::optional<py::str>& out_dir = std::nullopt,
                     const Log& log = nullptr, int64_t split = 0, bool build_whole = false);

// Rewrite SCOREBOARD.md from the store. -> the path written
py::str scoreboard(const std::optional<py::str>& out_dir = std::nullopt, const Log& log = nullptr);

// python -m codebreaker module ...; returns the exit status
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::source::module_writer
