// codebreaker/verify/lanes/java.hpp -- typed C++ port of codebreaker.verify.lanes.java.
//
// java.py -- what is particular to reconstructing a Java program as C++03.
//
// One of the kinds reconstruct.py works with (see there for what a kind provides). The module is a folder of
// Java source (a decompiled game's src/, say); its "functions" are its classes. The source is transpiled by
// codebreaker/java (transpile.convert, for no particular platform) and each class's C++ is compiled on its own
// with the host compiler (settings.json `host_cxx`, else g++ on PATH).
//
// A class that does not compile is DOES NOT COMPILE, with the compiler's first complaint. One that compiles
// is then run beside the original where it can be (codebreaker/java/probe.py): its methods with primitive
// parameters and results (static ones directly, instance ones on an object made with a constructor that takes
// primitives) are called on the same inputs in the JVM -- the Java source compiled by javac (settings.json
// `java`; javac beside it, else on PATH) -- and natively, and the results compared exactly. All alike:
// VERIFIED, or VERIFIED (weak) "methods-partial" when only some of the class's methods could be run. Any
// different: UNVERIFIED, with the method, the input and both results. Nothing it could run (no such method,
// no JDK): NOT JUDGED, saying why. A jar is not a module of this kind: decompile it first (`codebreaker java
// decompile`).
//
// Notes on the port:
//   * the kind's four functions are profile / prepare / build / judge; prepare's object (`Prepared` in the
//     Python: a bare class holding attributes) is the Prepared record, build's context a Context record and an
//     item an Item record (name, unit, java source or '').
//   * `prefix` (the precompiled header) is std::optional<str>; the Python's "" for "no JDK" is "".
//   * PLATFORM is None: std::optional<str>.
//   * a jar given to prepare raises py::SystemExit with the Python's message.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "codebreaker/java/probe.hpp"
#include "codebreaker/java/transpile.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::verify::lanes::java {

extern const py::str KIND;                 // "java"
inline constexpr int64_t REV = 3;          // bump when what a verdict means changes (flags, the judge, the transpile call)
extern const py::str SOURCE_REV;           // who made the candidates: the transpiler, not the native decompiler
inline constexpr bool LIBRARY = false;     // no runtime-library functions to identify
inline constexpr bool PER_FUNCTION = true; // a class's verdict depends on its text, the support headers and the classes it names
extern const py::str STD;                  // "c++03"
extern const py::str CPP_STD;              // "C++03": the back end transpile.convert's cpp_std (C++17: backends/cpp17.Bodies)
extern const std::optional<py::str> PLATFORM;   // no platform: the generic stdafx.h, nothing a host compiler lacks
extern const py::list<py::str> FLAGS;
extern const py::str COMPILES;             // "compiles (%s %s); not run: %s"

// A folder holding .java files (at any depth), or a single .java file's folder.
bool is_module(const py::str& path);
py::str source_dir(const py::str& path);

// What results are filed under: the .java files' names and contents, in a fixed order.
py::str sha(const py::str& path);

// (javac, java) -- the JDK the behavioural judge runs the original with -- or ("", "").
std::tuple<py::str, py::str> jdk();

// The host C++ compiler, or "" when there is none.
py::str compiler();

// Compile one translation unit for syntax and types. -> (ok, [error lines]). `prefix` is a header precompiled
// by `precompile` -- the support headers every unit starts with -- included first.
std::tuple<bool, py::list<py::str>> check(const py::str& cxx, const py::str& include_dir, const py::str& unit,
                                          const std::optional<py::str>& prefix = std::nullopt);

// (a digest of the settings a verdict depends on, the text it is made from). RuntimeError: no C++ compiler.
std::tuple<py::str, py::str> profile(const std::optional<py::str>& module_path = std::nullopt);

// stdafx.h and the Java runtime (JavaRT.h and what it includes) precompiled once for the module: every class
// unit starts with them, and parsing them was four fifths of each class's compile. -> the header to pass as
// `prefix`, or None (a compiler that is not GCC's, or the header did not precompile).
std::optional<py::str> precompile(const py::str& cxx, const py::str& include_dir, const py::str& work);

struct Prepared {
    py::str out;
    codebreaker::java::transpile::Summary summary;
    py::str src;
    py::dict<py::str, py::str> java;            // class name -> its .java path
    py::dict<py::str, py::str> texts;
    py::str header;
    py::list<py::str> order;
    py::dict<py::str, py::str> key;
    py::list<py::str> all_keys;
    py::str sig;
};

// Transpile the module into work/cpp. Classes are named by their .java file.
Prepared prepare(const py::str& module_path, const py::str& work);

// (name, unit, java source or '')
struct Item {
    py::str name;
    py::str unit;
    py::str source;
};

// what judge() is given for the whole program
struct Context {
    py::str cxx;
    py::str include_dir;
    std::optional<py::str> prefix;
    py::str staged;
    py::str javac;
    py::str java;
    py::str work;
    codebreaker::java::probe::Compiled compiled;
};

struct Built {
    Context context;
    py::dict<py::str, Item> items;
    py::dict<py::str, py::str> missing;   // {name: why it is not in the build}
};

// One translation unit per class: its .cpp, or for a header-only class a unit that includes it.
Built build(const Prepared& m, const py::str& module_path, const py::str& work);

// (verdict, what differs, [limits], text)
using Verdict = std::tuple<py::str, py::str, py::list<py::str>, py::str>;

Verdict judge(const Context& context, const Item& item);

}  // namespace codebreaker::verify::lanes::java
