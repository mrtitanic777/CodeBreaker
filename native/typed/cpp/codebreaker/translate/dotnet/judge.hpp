// codebreaker/translate/dotnet/judge.hpp -- typed C++ port of codebreaker.translate.dotnet.judge.
//
// judge.py -- the judge for whole programs, with the .NET adapter: run a program in its own runtime and its
// translation in the target's, and compare what they do (docs/TRANSLATOR_DESIGN.md, "The judge, for every
// pair").
//
// What is compared is what a program says to the outside: its standard output, byte for byte after
// line endings are made the same, and its exit code. A verdict is one of
//
//     equivalent   both ran; the same output and exit code
//     divergent    both ran; the output or the exit code differs (the first differing line is kept)
//     refused      the translator declined something in the program (its ledger says what and why), so
//                  the translation is not complete and is not run
//     no-compile   the translation did not compile (the compiler's first errors are kept)
//     no-original  the original did not build or run (nothing to judge against)
//
// Adapters (each -> Run): `csharp` (the installed .NET SDK's csc and runtime, no restore), `java` (javac
// and the JVM of the Java lane), `cpp` (the host C++ compiler: a transpiled program and its runtime).
//
//     Run r = judge::csharp(files, work);                  // Run(ok, exit, out, err, log)
//     auto [verdict, detail] = judge::compare(original_run, translated_run);
//
// Typing notes (how the Python's values are typed here):
//   * Run is a struct (the Python's __slots__ class): exit is std::optional<int64_t> (None: the program did not
//     finish), out / err / log are py::str ('' where the Python's default is '').
//   * compare()'s `refused` (a list of str or None) is a py::list<str>: None and [] are alike in the Python's
//     `if refused:`.
//   * csharp() calls the typed package codebreaker::translate::dotnet (the installed .NET SDK as a toolchain:
//     dotnet::compile and dotnet::run).
//   * java() and cpp() start javac, the JVM and the program with a timeout. subprocess.run(timeout=) is not in the
//     runtime, so a local helper waits for the timeout and ends the child it started (as java/probe.cpp does); a
//     javac that outlasts its 600 seconds raises subprocess.TimeoutExpired in the Python and py::TimeoutError here.
//   * java() and cpp() read the JDK and the host compiler from verify::lanes::java (jdk() and compiler()) and
//     build through java::probe (library(), build_native()), the typed ports of what the Python imports.
//   * Python ints are int64_t.
#pragma once

#include <functional>
#include <optional>
#include <tuple>

#include "codebreaker/translate/dotnet/package.hpp"
#include "py.hpp"

namespace codebreaker::translate::dotnet::judge {

inline constexpr int64_t TIMEOUT = 120;

// One program run: built (ok), its exit code, standard output and error; log: the build's output.
struct Run {
    bool ok = false;
    std::optional<int64_t> exit;
    py::str out;
    py::str err;
    py::str log;

    Run() {}
    explicit Run(bool ok, const std::optional<int64_t>& exit = std::nullopt, const py::str& out = py::str(""),
                 const py::str& err = py::str(""), const py::str& log = py::str(""))
        : ok(ok), exit(exit), out(out), err(err), log(log) {}

    // Run.__repr__
    py::str repr() const;
};

py::str _norm(const py::str& s);

// -> (verdict, detail).
std::tuple<py::str, py::str> compare(const Run& orig, const Run& trans, const py::list<py::str>& refused = py::list<py::str>());

// A folder made new: removed with what it holds, made again.
py::str _fresh(const py::str& work);

// ---------------------------------------------------------------------------------------------- .NET
// Compile C# files with the SDK's csc and run the program on the installed runtime (main: the class
// whose Main is the entry point, where several have one).
Run csharp(const py::list<py::str>& files, const py::str& work, const py::list<py::str>& args = py::list<py::str>(),
           int64_t timeout = TIMEOUT, const py::str& name = py::str("Program"), const std::optional<py::str>& main = std::nullopt);

py::str _errors(const py::str& log);

// ---------------------------------------------------------------------------------------------- the JVM
// Compile a Java source tree with javac and run class `main` (qualified) on the JVM.
Run java(const py::str& src_dir, const py::str& main, const py::str& work, const py::list<py::str>& args = py::list<py::str>(),
         int64_t timeout = TIMEOUT, const py::list<py::str>& classpath = py::list<py::str>());

// ---------------------------------------------------------------------------------------------- C++
// Build a program the Java transpiler's C++03 back end wrote (every .cpp of cpp_dir, with its runtime
// headers) with the host C++ compiler, and run `main_class::main` after the static initialisers.
Run cpp(const py::str& cpp_dir, const py::str& main_class, const py::str& work, const py::list<py::str>& args = py::list<py::str>(),
        int64_t timeout = TIMEOUT, int64_t jobs = 2);

}  // namespace codebreaker::translate::dotnet::judge
