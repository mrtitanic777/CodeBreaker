// codebreaker/translate/dotnet/package.hpp -- COMPAT SHIM over cb/translate/dotnet/package.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module is native/cb/translate/dotnet/package.{hpp,cpp}: the installed .NET SDK as a toolchain (`cb::translate::
// dotnet::Sdk`). The only part of the old API that old code still calls is `run_process` and its types (the C#, Java and
// clang front ends start their tools through it); that is kept, over cb::process::run. The SDK functions (exe, home, sdk,
// roslyn, runtime, refs, available, env, runtimeconfig, compile, run, _ver, _newest) had no old-typed caller but the judge
// (which is handed them as arguments) and are not kept here: new code uses `Sdk`.
//
// `env`, as an argument of run_process, is applied on top of this process's environment (a variable it does not name is
// still inherited); the two callers that exist pass none.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/process.hpp"
#include "cb/translate/dotnet/package.hpp"
#include "py.hpp"

namespace codebreaker::translate::dotnet {

inline const int64_t TIMEOUT = 300;
inline const py::list<py::str> STD_INSTALLS{py::str("C:\\Program Files\\dotnet\\dotnet.exe"), py::str("/usr/share/dotnet/dotnet"),
                                            py::str("/usr/local/share/dotnet/dotnet")};

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

// `timeout`: seconds (nullopt: none). The child is killed and TimeoutExpired raised when it runs longer. OSError
// (FileNotFoundError) when the program cannot be started.
inline Process run_process(const py::list<py::str>& args, const std::optional<py::dict<py::str, py::str>>& env = std::nullopt,
                           const std::optional<py::bytes>& input = std::nullopt, std::optional<double> timeout = std::nullopt,
                           bool stderr_to_stdout = false) {
    cb::process::ProcessSpec spec;
    spec.argv = cb::compat::from_py(args);
    if (env) {
        for (const auto& kv : cb::compat::from_py(*env)) spec.env[kv.first] = kv.second;
    }
    if (input) spec.stdin_data = cb::compat::from_py(*input);
    if (timeout) spec.timeout = std::chrono::milliseconds(static_cast<std::int64_t>(*timeout * 1000.0));
    if (stderr_to_stdout) spec.err = cb::process::Capture::Merge;
    cb::process::ProcessResult r;
    try {
        r = cb::process::run(spec);
    } catch (const cb::ToolError& e) {
        throw py::FileNotFoundError(e.what());   // the program could not be started
    }
    if (r.timed_out) throw TimeoutExpired("Command timed out after " + std::to_string(timeout.value_or(0.0)) + " seconds");
    Process p;
    p.returncode = static_cast<int64_t>(r.exit_code);
    p.stdout_ = cb::compat::to_py(cb::ByteView(r.out));
    p.stderr_ = cb::compat::to_py(cb::ByteView(r.err));
    return p;
}

}  // namespace codebreaker::translate::dotnet
