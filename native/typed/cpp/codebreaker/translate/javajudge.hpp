// codebreaker/translate/javajudge.hpp -- typed C++ from codebreaker.translate.javajudge.
//
// javajudge.py -- the Java corpus judged on each back end, beside the JVM (docs/TRANSLATOR_DESIGN.md, phases 4 and 6).
//
// Two ways a Java program reaches native code, and a judge for each:
//
//   * through the transpiler (codebreaker/java): declarations from its front end, every body from the IR
//     by a body back end -- cpp03 (the 360's) or C++17 (backends/cpp17.Bodies: cpp03.Bodies17). Judged class by class by
//     the java lane and probe.py: each class compiled on its own, its methods run on the same inputs in
//     the JVM and natively.        judge_tree(src, work, backend)
//
//   * whole programs from L2 (declarations and bodies), with a runtime of their own: backends/cpp17.program (native)
//     and backends/c_java.Program. Judged per program: every static method with primitive or String
//     parameters and a primitive or String result is run on probe.py's inputs in the JVM and natively,
//     and the program's main is run on both sides and its output compared.   judge_program(src, work, backend)
//
// Verdicts: 'equivalent' (every result alike), 'divergent' (one differs, with the case), 'refused' (the back
// end declined -- a J2X-GAP body, a Decline, or what it wrote does not compile), 'not-run' (nothing it could
// compare).
//
// Notes on the port:
//   * the Java front end is codebreaker::translate::frontends::java (javac's TreeDump -> JSON trees, dump / diagnostics).
//   * judge_tree sets the lane's module variables CPP_STD, STD, FLAGS and probe's CXX_STD for the run and puts
//     them back, as the Python does. verify/lanes/java.cpp declares STD and CPP_STD const; this file writes them
//     through a const_cast (FLAGS, a list handle, is changed in place). They need to be plain variables there.
//   * judge_tree's worker threads are std::threads over a mutex; `log` is a std::function of a str (empty: print);
//   * judge_program's result dict is the ProgramVerdict record; `res['main']` (None, then a dict of the programs'
//     mains' outcomes) is an optional dict;
//   * the Python's `V.NOT_RUN` (a tuple) assigned as a verdict in judge_tree's worker is a verdict word of no
//     match here (V::INCONCLUSIVE): `_word` answers 'not-run' for both;
//   * _run's (returncode, stdout, stderr) of text is a tuple; a timeout is (-999, '', 'timed out');
//     the runtime's py::subprocess::run has no timeout or environment, so this file runs its children itself.
#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <tuple>

#include "codebreaker/translate/ir.hpp"
#include "py.hpp"

namespace codebreaker::translate::javajudge {

namespace ir = codebreaker::translate::ir;

extern const py::str EQUIVALENT, DIVERGENT, REFUSED, NOT_RUN;

// {class: (verdict, gaps, detail)}
using TreeVerdicts = py::dict<py::str, std::tuple<py::str, int64_t, py::str>>;
using Log = std::function<void(const py::str&)>;

// Every class of the Java tree `src` converted by the transpiler with the body back end `backend` ('cpp03' or 'cpp17')
// and judged by the java lane (probe.py). `only`: just these classes; `workers`: threads judging at once.
TreeVerdicts judge_tree(const py::str& src, const py::str& work, const py::str& backend = py::str("cpp03"),
                        const std::optional<py::set<py::str>>& only = std::nullopt, int64_t workers = 2, const Log& log = nullptr);

// verdicts word of a lane verdict, by gaps
py::str _word(const py::str& verdict, int64_t gaps);

// (javac, java); RuntimeError without a JDK
std::tuple<py::str, py::str> java_tools();

// The Java sources under `src` through javac's tree into L2. -> [ir.Unit]
py::list<ir::NodeP> lower_program(const py::str& src, const py::str& work, const std::optional<py::str>& release = std::nullopt);

// subprocess.run -> (returncode, stdout, stderr); a timeout is (-999, '', 'timed out'). `env` is added to this process's.
std::tuple<int64_t, py::str, py::str> _run(const py::list<py::str>& cmd, const std::optional<py::str>& cwd = std::nullopt,
                                           int64_t timeout = 120, const std::map<std::string, std::string>& env = {});

// the probe harness: written in Java from L2, so the same cases run in the JVM and -- translated by the back
// end under judgement, as any other class of the program -- natively (TRANSLATOR_DESIGN.md, "the judge")
extern const py::str HARNESS;   // "J2XProbe"
extern const py::dict<py::str, py::str> _FORMAT;
extern const py::dict<py::str, py::str> _JAVA_PRIM;
extern const py::str _HELPERS;

// the Java name of a primitive or String type, else None
std::optional<py::str> _jname(const ir::TypeP& t);

// (class reference in Java, method, [param type names], result type name)
using ProbeMethod = std::tuple<py::str, py::str, py::list<py::str>, py::str>;

// every static method of a class of the default package, not private, whose parameters are primitives or Strings and whose
// result is one.
py::list<ProbeMethod> probe_methods(const py::list<ir::NodeP>& units);

// the one package of the program's units ('' for the default package)
py::str _package(const py::list<ir::NodeP>& units);

// key -> (method, values, result type)
using ProbeKeys = py::dict<py::str, std::tuple<py::str, py::list<py::str>, py::str>>;

// The Java source of J2XProbe for `methods` (probe_methods), on probe.py's inputs. -> (source, keys)
std::tuple<py::str, ProbeKeys> harness(const py::list<ProbeMethod>& methods, const py::str& package = py::str(""));

// copies every .java file under `src` into `dest` (flat)
py::str _stage(const py::str& src, const py::str& dest);

// what a whole program's judgement found
struct ProgramVerdict {
    py::str verdict = NOT_RUN;
    int64_t methods = 0;
    int64_t cases = 0;
    // (method, values, the JVM's result, the other's)
    py::list<std::tuple<py::str, py::list<py::str>, py::str, py::str>> differs;
    std::optional<py::dict<py::str, py::str>> main;
    py::str why;
    py::str backend;
};

// A whole Java program (a folder of sources, default package) beside the JVM, translated by a whole-program back end
// ('cpp17' or 'c'). `log` is unused (the Python's, too).
ProgramVerdict judge_program(const py::str& src, const py::str& work, const py::str& backend = py::str("cpp17"), const Log& log = nullptr);

// Write a whole program with `backend` and compile it. -> (exe or None, why)
std::tuple<std::optional<py::str>, py::str> build(const py::list<ir::NodeP>& units, const py::str& work, const py::str& backend);

// the host C++ compiler; RuntimeError when there is none
py::str _host_cxx();

}  // namespace codebreaker::translate::javajudge
