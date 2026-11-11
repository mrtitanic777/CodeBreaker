// codebreaker/translate/dotnet/package.hpp -- typed C++ port of codebreaker/translate/dotnet/__init__.py.
//
// dotnet -- the installed .NET SDK as a toolchain: compile C# with the SDK's own Roslyn (csc.dll), run the result on the
// installed runtime. No project file, no NuGet restore, nothing downloaded: the compiler is given the reference
// assemblies of the SDK's targeting pack by path, and the program gets a runtimeconfig.json naming the installed
// Microsoft.NETCore.App.
//
//     auto [ok, log] = dotnet::compile({"Prog.cs"}, "out/Prog.dll");     // an exe (Main) by default
//     auto [code, out, err] = dotnet::run("out/Prog.dll");
//
// The SDK is found from CB_DOTNET (dotnet.exe), then PATH, then the standard install folder.
//
// Typing notes:
//   * "or None" results are std::optional<str>; runtime() is (major, full version), either None when no targeting pack is
//     installed. A version (`_ver`) is the tuple of up to three ints the name splits into: a py::list<int64_t>.
//   * env() is the process environment as a dict (Windows' names upper-cased, as os.environ has them) plus the dotnet
//     variables and TEMP / TMP. It is what the child processes are given.
//   * subprocess.run with `env=`, `timeout=` and bytes output is not in the runtime (py::subprocess::run has none of
//     them): run_process() below is that call, written here for the C# / Java / clang front ends too (they take it from
//     this header). It raises TimeoutExpired (a py::Exception of that name) after killing the child, as subprocess.run does;
//     the exit code is the unsigned one Windows gives.
//   * run() answers (exit code, stdout, stderr) with the exit code None on a timeout, stdout "" and stderr "timeout".
//   * compile()'s `references` and `tmp` are optional/empty as in the Python (references=(), tmp=None, main=None).
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::translate::dotnet {

extern const int64_t TIMEOUT;
extern const py::list<py::str> STD_INSTALLS;   // the Python's `_STD` (a macro of the MSVC standard library, so spelled otherwise)

// subprocess.TimeoutExpired
struct TimeoutExpired : py::Exception {
    explicit TimeoutExpired(const std::string& m) : py::Exception(m) {}
    const char* type() const override { return "TimeoutExpired"; }
};

// What subprocess.run(args, stdout=PIPE, stderr=PIPE | STDOUT, [input=], [env=], [timeout=]) answers (bytes, not decoded).
struct Process {
    int64_t returncode = 0;
    py::bytes stdout_;
    py::bytes stderr_;   // empty when stderr went to stdout
};
// `env`: the child's whole environment (nullopt: this process's); `timeout`: seconds (nullopt: none). The child is killed
// and TimeoutExpired raised when it runs longer. OSError (FileNotFoundError) when the program cannot be started.
Process run_process(const py::list<py::str>& args, const std::optional<py::dict<py::str, py::str>>& env = std::nullopt,
                    const std::optional<py::bytes>& input = std::nullopt, std::optional<double> timeout = std::nullopt,
                    bool stderr_to_stdout = false);

py::list<int64_t> _ver(const py::str& s);

// dotnet.exe, or None.
std::optional<py::str> exe();
std::optional<py::str> home();
std::optional<py::str> _newest(const py::str& folder, const py::str& prefix = py::str(""));
// The newest SDK folder (it holds Roslyn), or None.
std::optional<py::str> sdk();
// The folder of the SDK's compiler: csc.dll and Microsoft.CodeAnalysis*.dll.
std::optional<py::str> roslyn();
// (major, full version) of the installed Microsoft.NETCore.App the SDK's targeting pack matches.
std::tuple<std::optional<py::str>, std::optional<py::str>> runtime();
// The targeting pack's reference assemblies folder (System.Runtime.dll, System.Console.dll, ...).
std::optional<py::str> refs();
bool available();
py::dict<py::str, py::str> env(const std::optional<py::str>& tmp = std::nullopt);
// Write <dll>.runtimeconfig.json beside a compiled program: run on the installed runtime.
py::str runtimeconfig(const py::str& dll);
// Compile C# sources to out_dll with the SDK's csc. -> (ok, compiler output)
std::tuple<bool, py::str> compile(const py::list<py::str>& sources, const py::str& out_dll,
                                  const py::list<py::str>& references = py::list<py::str>(), const py::str& target = py::str("exe"),
                                  bool nullable = true, bool unsafe = false, bool warn = false,
                                  const std::optional<py::str>& tmp = std::nullopt, const std::optional<py::str>& main = std::nullopt);
// Run a compiled program on the installed runtime. -> (exit code, stdout, stderr); a timeout is exit code None.
std::tuple<std::optional<int64_t>, py::str, py::str> run(const py::str& dll, const py::list<py::str>& args = py::list<py::str>(),
                                                          double timeout = 60, const std::optional<py::bytes>& stdin_ = std::nullopt,
                                                          const std::optional<py::str>& tmp = std::nullopt);

}  // namespace codebreaker::translate::dotnet
