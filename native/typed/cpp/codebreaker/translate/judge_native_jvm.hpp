// codebreaker/translate/judge_native_jvm.hpp -- typed C++ from codebreaker.translate.judge_native_jvm.
//
// judge_native_jvm.py -- the judge for C/C++ -> Java (docs/TRANSLATOR_DESIGN.md, "The judge, for every pair").
//
// The original runs natively: clang compiles the C or C++ program (-O0, the front end's own target) and
// it runs. The translation runs on the JVM: the clang front end lowers each file to L2, the Java back
// end writes the program as Java, javac compiles it beside the C runtime (crt), and java runs it. Both
// get the same arguments and the same stdin; their stdout, stderr and exit code are compared. Windows
// text mode writes "\r\n" for "\n" on the native side, so line ends are compared as "\n".
//
// Every program gets a verdict:
//   equivalent  -- same stdout, stderr and exit code;
//   divergent   -- both ran, something differs (the first difference is the reason);
//   refused     -- the translator declined, and says why (front end Unsupported, back end Refused);
//   no-compile  -- the Java it wrote does not compile (a back end bug: javac's first error is the reason);
//   no-original -- the original itself does not build or run here.
//
//     Verdict v = judge({"prog.c"}, work_dir);
//     v.verdict, v.reason, v.tier
//
// Notes on the port:
//   * the clang front end is codebreaker::translate::frontends::clang (clang(), language_of(), flags(), l2_file()).
//   * a run's result `(stdout, stderr, returncode)` is a Run: the exit code is an int, or the word 'timeout'
//     (the Python's mixed tuple item: a std::variant);
//   * subprocess.run(timeout=) raising TimeoutExpired (the text-mode runs of clang, javac) is TimeoutExpired here;
//     the runtime's py::subprocess::run has no timeout, environment or stdin bytes, so this file runs its
//     children itself (Spawn), as java/probe.cpp does;
//   * a Verdict's native / jvm are optional Runs (None: not run), `java` the optional Java text, `notes` a list
//     of str; row() is the dict of the Python, with the one None-able value (tier) an optional;
//   * Python ints are int64_t; text of a run is decoded as the runtime of the Python on Windows does (cp1252,
//     errors='replace', universal newlines) where the Python asks for text=True.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <variant>

#include "codebreaker/translate/backends/java.hpp"
#include "codebreaker/translate/ir.hpp"
#include "py.hpp"

namespace codebreaker::translate::judge_native_jvm {

namespace ir = codebreaker::translate::ir;
namespace jback = codebreaker::translate::backends::java;

inline constexpr int64_t RUN_TIMEOUT = 120;

// subprocess.TimeoutExpired
struct TimeoutExpired : py::Exception {
    TimeoutExpired() : py::Exception("timed out") {}
    explicit TimeoutExpired(const std::string& m) : py::Exception(m) {}
    const char* type() const override { return "TimeoutExpired"; }
};

// what a run gave: (stdout, stderr, returncode); the code is an int, or the word 'timeout'
struct Run {
    py::bytes out;
    py::bytes err;
    std::variant<int64_t, py::str> code;
};

struct Verdict {
    py::str name;
    py::str verdict;
    py::str reason;
    std::optional<py::str> tier;
    std::optional<py::str> java;
    std::optional<Run> native;
    std::optional<Run> jvm;
    py::list<py::str> notes;

    Verdict(const py::str& name, const py::str& verdict, const py::str& reason = py::str(""),
            const std::optional<py::str>& tier = std::nullopt, const std::optional<py::str>& java = std::nullopt,
            const std::optional<Run>& native = std::nullopt, const std::optional<Run>& jvm = std::nullopt,
            const py::list<py::str>& notes = py::list<py::str>())
        : name(name), verdict(verdict), reason(reason), tier(tier), java(java), native(native), jvm(jvm), notes(notes) {}

    // Verdict(name: verdict -- reason)
    py::str repr() const;
    // {'program': name, 'verdict': ..., 'reason': ..., 'tier': tier}
    py::dict<py::str, std::optional<py::str>> row() const;
};

// the folder of crt compiled once (cached by its sources and the JDK)
py::str runtime_classes();

// the program's original, as the original of the judge builds it: clang -O0 -> (ok, the end of clang's message)
std::tuple<bool, py::str> native(const py::list<py::str>& files, const py::str& out_exe, const py::str& language,
                                 const std::optional<py::str>& std_ = std::nullopt,
                                 const py::list<py::str>& defines = py::list<py::str>(),
                                 const py::list<py::str>& includes = py::list<py::str>());

// The program's files -> jback::Program (raises jback::Refused / py::RuntimeError).
jback::Program translate(const py::list<py::str>& files, const py::str& name, const std::optional<py::str>& language = std::nullopt,
                         const std::optional<py::str>& std_ = std::nullopt, const py::list<py::str>& defines = py::list<py::str>(),
                         const py::list<py::str>& includes = py::list<py::str>());

// A source file's IR, through the front end's cached L2 document (keyed by the source and the lowering).
ir::NodeP _unit(const py::str& path, const py::str& language, const std::optional<py::str>& std_, const py::list<py::str>& includes,
                const py::list<py::str>& quote_includes, const py::list<py::str>& defines,
                const std::optional<py::list<py::str>>& own = std::nullopt);

Verdict judge(const py::list<py::str>& files, const py::str& work, const std::optional<py::str>& name = std::nullopt,
              const std::optional<py::str>& language = std::nullopt, const std::optional<py::str>& std_ = std::nullopt,
              const py::list<py::str>& defines = py::list<py::str>(), const py::list<py::str>& includes = py::list<py::str>(),
              const py::bytes& stdin_ = py::bytes(), const py::list<py::str>& args = py::list<py::str>());

// Java -> C++ -> Java, judged: the Java lane (javac front end -> IR -> the C++03 back end,
// codebreaker/java/transpile.convert) writes the classes as C++; the clang front end reads that C++ back
// (j2x.lift maps the Java runtime in C++ onto Java's library); the Java back end writes Java again. The
// original classes and the round-tripped ones run on the JVM; their output is compared.
// main_class: the qualified class whose main runs ('sample.RoundTrip'). -> Verdict (java: the Java
// written; notes: what was translated).
Verdict roundtrip(const py::list<py::str>& java_files, const py::str& main_class, const py::str& work,
                  const py::str& name = py::str("RoundTripped"));

// run(cmd, stdin, cwd, timeout) -> (stdout, stderr, returncode); a timeout is (b'', b'(timed out)', 'timeout')
Run _run(const py::list<py::str>& cmd, const py::bytes& stdin_, const py::str& cwd, int64_t timeout = RUN_TIMEOUT);

// replace "\r\n" by "\n"
py::bytes _norm(const py::bytes& b);

// None when the two runs agree, else what differs first.
std::optional<py::str> _compare(const Run& a, const Run& b);

py::str _first_error(const py::str& text);

py::str _class_name(const py::str& stem);

}  // namespace codebreaker::translate::judge_native_jvm
