// codebreaker/recompile/x360project.hpp -- typed C++ from codebreaker.recompile.x360project.
//
// x360project.py -- an Xbox 360 title as a PC project on CodeBreaker's Xbox 360 runtime.
//
//     python -m codebreaker recompile-project <default.xex> [-o <folder>] [--name N] [--title-files <dir>]
//     auto info = x360project::scaffold(xex_path, folder, name, title_files);
//
// The folder is the binary's project folder (codebreaker/base/converted.py: Converted/<name>/)
// unless -o names another:
//
//     <folder>/
//       CMakeLists.txt        x360_title(<name> ...) from native/x360/x360.cmake
//       src/                  the title's XenonRecomp configuration (config.py), its switch tables
//                             (switches.py, with XenonAnalyse's), and default.xex in basic form
//       src/ppc/              XenonRecomp's C++ (one statement per guest instruction), regenerated:
//                             only changed files are rewritten, so a rebuild recompiles only those
//       src/module_info.cpp   the title described to the runtime (modules.h: g_titleModule)
//       assets/               the title folder the program runs: its files (--title-files) and
//                             default.xex in basic form; it is the title's game: device
//       build/                the CMake build: build/<name>.exe
//       reports/recompile.log what XenonRecomp said
//       Build-<name>.bat      configures and builds with clang-cl and Ninja
//       Play-<name>.bat       runs it
//
// The program is native/x360's runtime with the title linked in; run it as
// `build/<name>.exe assets` (or from assets/).
//
// Typing and runtime notes:
//   * `ROOT` is three folders above the Python module; here `set_root` derives it from the checkout root (as
//     recompile/xenon.hpp does for its own), a plain variable a test may point elsewhere.
//   * scaffold's result, check's result and build's (ok, lines) are records / a tuple.
//   * every subprocess runs through py::subprocess::run (timeout, env); check()'s Popen + wait(timeout) + kill is
//     run(timeout=...) with TimeoutExpired caught: the child is ended with exit code 1, as Process.kill() leaves it.
//   * shutil.copy2 / copytree(dirs_exist_ok=True) and filecmp.cmp(shallow=False) are not in the runtime: small local
//     helpers (copy2 keeps the modification time).
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::recompile::x360project {

// The checkout root (three folders above x360project.py).
extern py::str ROOT;
void set_root(const py::str& root);

// the program's, target's and scripts' name from a .xex path: its stem, lower case, letters and digits kept
py::str _name(const py::str& xex_path);

// Make dst hold src's files, rewriting only those that changed and removing the rest.
void _sync(const py::str& src, const py::str& dst);

// scaffold's result: {"name", "dir", "functions", "switches", "unimplemented"}
struct ScaffoldInfo {
    py::str name;
    py::str dir;
    int64_t functions = 0;
    int64_t switches = 0;
    int64_t unimplemented = 0;
};
template <class F> void for_each_field_mut(ScaffoldInfo& r, F&& f) {
    f("name", r.name, true);
    f("dir", r.dir, true);
    f("functions", r.functions, true);
    f("switches", r.switches, true);
    f("unimplemented", r.unimplemented, true);
}

// `name` and `title_files` are None by default (the name is taken from the .xex; no title folder is copied).
ScaffoldInfo scaffold(const py::str& xex_path, const py::str& out_dir, const std::optional<py::str>& name = std::nullopt,
                      const std::optional<py::str>& title_files = std::nullopt);

// Runs Build-<name>.bat with at most `jobs` compiles at once. -> (ok, the last lines of its output)
std::tuple<bool, py::list<py::str>> build(const py::str& out_dir, const py::str& name, int64_t jobs = 4);

// check's result: {"frames", "unimplemented": {import: calls}, "fatal": message or "", "exit", "log"}
struct CheckResult {
    int64_t frames = 0;
    py::dict<py::str, int64_t> unimplemented;
    py::str fatal;
    std::optional<int64_t> exit;   // the program's exit code (1 when it had to be ended)
    py::str log;
};
template <class F> void for_each_field_mut(CheckResult& r, F&& f) {
    f("frames", r.frames, true);
    f("unimplemented", r.unimplemented, true);
    f("fatal", r.fatal, true);
    f("exit", r.exit, true);
    f("log", r.log, true);
}

// Runs the built title headless for `seconds` and reads its log.
CheckResult check(const py::str& out_dir, const py::str& name, int64_t seconds = 60);

}  // namespace codebreaker::recompile::x360project
