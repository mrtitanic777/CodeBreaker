// codebreaker/recompile/xenon.hpp -- typed C++ from codebreaker.recompile.xenon.
//
// xenon.py -- XenonRecomp, the Xbox 360 recompiler CodeBreaker's projects are generated with.
//
//     xenon::build();                       // native/xenonrecomp -> build/XenonRecomp/XenonRecomp.exe
//     xenon::recompile(config_toml);        // runs it on a project's configuration (config.py writes one)
//     xenon::recompile_synced(config_toml); // the same into a clean folder, then only the changed files into the
//                                           // configuration's output folder (and the stale ones out)
//
//     python -m codebreaker xenon-recompile <config.toml>   the synced form, for a build to run (the Xbox 360
//                                       Dashboard's modules: PYTHONPATH=<CodeBreaker>)
//     xenon::instruction_tests();           // Xenia's PowerPC tests through it: {tested, skipped, mismatches}
//
// native/xenonrecomp is a fork of hedge-dev's XenonRecomp (MIT) with the changes the Xbox 360
// system-software ports needed (VMX128 compares that set CR6, the import library index, symbol
// suffixes for several modules in one program, MMIO stores, the shared indirect-call table; every
// change from upstream is listed in native/xenonrecomp/CHANGES.md). It is the one XenonRecomp: the
// Xbox 360 Dashboard's line was merged into it. It
// turns each guest instruction into one C++ statement on a PPCContext; the runtime
// (native/x360) supplies the kernel, XAM, GPU, audio and input.
//
// Built with clang-cl from Visual Studio (setting `visual_studio`) and Ninja. The instruction tests
// assemble Xenia's tests (native/xenonrecomp/ppc_tests: inputs and the outputs measured on the
// console) with PowerPC binutils that know VMX128 (setting `ppc_binutils`), recompile them, build
// them and run them: every printed line is a mismatch.
//
// Python's subprocess.run(capture_output=True, text=True) has no runtime counterpart yet, so _run is
// written here (CreateProcess, stdout and stderr in separate pipes, decoded as cp1252 with universal
// newlines, as Python's text mode does on Windows). It is implemented for Windows only.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::recompile::xenon {

// The module's locations. In Python they are fixed at import from the module's own place in the checkout
// (ROOT is three folders above xenon.py); here set_root derives them from the checkout root, and each is
// a plain variable so a caller (or a test) can point any one of them elsewhere, as a test patches the
// module's attributes.
extern py::str ROOT;
extern py::str SOURCE;
extern py::str BUILD;
extern py::str RECOMP;
extern py::str ANALYSE;
extern py::str TESTS;
// a host-side helper, and a test that needs a switch-table configuration test mode has not
extern const std::tuple<py::str, py::str> SKIP_TESTS;

// Derive ROOT, SOURCE, BUILD, RECOMP, ANALYSE and TESTS from a checkout root (what the import does).
void set_root(const py::str& root);

// What subprocess.run returned: the exit code and the text of stdout and of stderr.
struct CompletedProcess {
    int64_t returncode = 0;
    py::str stdout_text;
    py::str stderr_text;
};

// subprocess.run(cmd, capture_output=True, text=True, cwd=, env=): cmd is the program and its arguments.
CompletedProcess _run(const py::list<py::str>& cmd, const std::optional<py::str>& cwd = std::nullopt,
                      const std::optional<py::dict<py::str, py::str>>& env = std::nullopt);

// subprocess.run(command, capture_output=True, text=True, shell=True): the command line through COMSPEC /c.
CompletedProcess _run_shell(const py::str& command);

// The Visual Studio install that has clang-cl.
py::str visual_studio();

py::str clang_cl();

// The environment vcvars64.bat sets up (the linker, the SDK's headers and libraries), cached after the first
// use. (the module's `_env`; nullopt until msvc_env first runs)
extern std::optional<py::dict<py::str, py::str>> _env;

// The environment vcvars64.bat sets up (the linker, the SDK's headers and libraries).
py::dict<py::str, py::str> msvc_env();

double _newest_source();

// Build XenonRecomp and XenonAnalyse (only when a source is newer than the executables).
py::str build(bool force = false);

// Run XenonRecomp on a configuration file. -> its output (warnings, the unimplemented).
py::str recompile(const py::str& config);

// recompile_synced's result: {"generated", "changed", "stale": [names], "log": [its warning lines]}
struct SyncResult {
    int64_t generated = 0;
    int64_t changed = 0;
    py::list<py::str> stale;
    py::list<py::str> log;
};
template <class F> void for_each_field_mut(SyncResult& r, F&& f) {
    f("generated", r.generated, true);
    f("changed", r.changed, true);
    f("stale", r.stale, true);
    f("log", r.log, true);
}

// XenonRecomp on `config` into a clean folder, then into its out_directory_path only the files that changed
// (the others keep their times, so builds stay incremental) and out the files this run did not make: XenonRecomp
// never deletes, and a recompile that splits the code into fewer files would leave old ones defining functions
// twice. `report` takes each line to show (default: print it).
SyncResult recompile_synced(const py::str& config, const std::function<void(const py::str&)>& report = nullptr);

// The command line without the program name (sys.argv[1:]); prints to stdout. -> the exit code
int64_t main(const py::list<py::str>& argv);

py::str _unix(const py::str& p);

// instruction_tests' result: {"tested": n, "skipped": [...], "mismatches": [...], "failed": [...]}
struct InstructionTestsResult {
    int64_t tested = 0;
    py::list<py::str> skipped;
    py::list<py::str> mismatches;
    py::list<py::str> failed;
};
template <class F> void for_each_field_mut(InstructionTestsResult& r, F&& f) {
    f("tested", r.tested, true);
    f("skipped", r.skipped, true);
    f("mismatches", r.mismatches, true);
    f("failed", r.failed, true);
}

// Xenia's PowerPC instruction tests (and ours) through XenonRecomp.
// -> {"tested": n, "skipped": [tests with instructions it does not implement],
//     "mismatches": [lines the tests printed], "failed": [test names]}
// `names` limits the tests (empty or none: all); `work` is the folder they are built in (empty or none: the
// state folder's xenonrecomp/ppc_tests).
InstructionTestsResult instruction_tests(const std::optional<py::list<py::str>>& names = std::nullopt,
                                          const std::optional<py::str>& work = std::nullopt,
                                          const py::str& march = py::str("haswell"));

}  // namespace codebreaker::recompile::xenon
