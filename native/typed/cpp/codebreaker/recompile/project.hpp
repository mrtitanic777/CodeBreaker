// codebreaker/recompile/project.hpp -- typed C++ from codebreaker.recompile.project.
//
// project.py -- a recompiled Xbox 360 title as a PC project that builds.
//
//     python -m codebreaker recompile-project <image or .xex> -o <folder>
//     auto agg = project::scaffold(path, folder);
//
// Lays out the source tree a static recompilation needs, from the image alone:
//
//     <folder>/
//       CMakeLists.txt, Play-<name>.bat, README.md, GPU_DESIGN.md
//       include/    rex_ppc_min.h (the runtime header), <name>.h (every function declared),
//                   <name>_config.h (image base, entry, size), rex_host.h (guest heap, host helpers)
//       src/        <name>_NNNN.cpp (the recompiled functions, in translation units of ~1.5 MB),
//                   <name>_init.cpp (guest address -> function), rex_runtime.cpp (indirect calls,
//                   guest memory, boot), <name>_imports.cpp (one host function per import)
//       graphics/   the native GPU subsystem (gpu.py), with the rexglue DXBC shader translator
//       kernel/     the rexglue kernel core (native/rexglue/kernel), which the recompiled code
//                   bridges onto: its PPCContext, and its real implementations of kernel exports
//       assets/     the image (loaded at its base) and the .xex
//
// What it is today: the project builds and links. To run a whole title, the host side has to be
// filled in where it is not yet -- the imports the kernel core does not implement are stubs that
// log once, and the GPU bridge is a scaffold (GPU_DESIGN.md).
//
// Typing and runtime notes:
//   * The Python's text templates (the f-strings of _runtime_cpp, _cmakelists, _launcher_bat, _host_h, _readme, the fixed
//     lines of _imports_cpp, the main() help text) are in project_text.inc as C++ raw strings, generated from the
//     Python with the name given as @name@ and the numbers as markers: the C++ puts them in. HOST_IMPL is filled from
//     the same file.
//   * scaffold's result `agg` (powerpc.recompile_image's dict, plus "name", "dir", "shards", "imports") is the record
//     `Scaffolded` (its `agg` is powerpc::Agg).
//   * `ROOT` / `_REXGLUE`: three folders above the Python module; set_root derives them from the checkout root (as
//     recompile/xenon.hpp does) and each stays a plain variable a test may point elsewhere.
//   * the ordinal table of knowledge.table("xbox360", "ordinal_names") is formats::xex_imports::_names() (its authored
//     reader, with the numbers of the keys read); knowledge.table's `reads.record_kind` (the dependency recorder of a tracked step)
//     is not called: it would link the knowledge store and the whole engine for a recorder nothing listens to from here.
//   * `_sync_tree`'s `ignore` is a std::function (the folder, the names in it) -> the names to drop.
//   * main(argv) takes sys.argv[1:], prints through py::print and returns the exit code; the `__main__` block is the
//     caller's.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>

#include "codebreaker/recompile/powerpc.hpp"
#include "py.hpp"

namespace codebreaker::recompile::project {

extern py::str ROOT;
extern py::str _REXGLUE;       // <ROOT>/native/rexglue
void set_root(const py::str& root);

py::str _clean_name(const py::str& module);

inline constexpr int64_t SHARD_BYTES = 1500000;       // target size of each src/*.cpp translation unit

// Group per-function C++ into balanced translation units (~target bytes).
py::list<py::list<py::str>> _shards(const py::list<py::str>& parts, int64_t target = SHARD_BYTES);

// Write `data` to `path` unless it already holds exactly that: a file left alone keeps its time, so rebuilding a
// regenerated project recompiles only what changed. -> True when it was written.
bool _write_if_changed(const py::str& path, const py::bytes& data);

// Make `dst` a copy of `src` (less what `ignore` drops), touching only files that differ, and removing the ones `src`
// no longer has.
void _sync_tree(const py::str& src, const py::str& dst, const std::function<py::set<py::str>(const py::str&, const py::list<py::str>&)>& ignore);

// Copy the rexglue DXBC shader translator (native/rexglue/dxbc) into a project, without build products or standalone
// tests. -> True when it is there to copy.
bool _copy_rex_dxbc(const py::str& dst);

// Copy the rexglue kernel core (native/rexglue/kernel) into a project, without build products. -> True when it is there
// to copy.
bool _copy_rex_kernel(const py::str& dst);

// scaffold's result: powerpc.recompile_image's agg, with the project's name, folder, translation units and imports
struct Scaffolded {
    powerpc::Agg agg;
    py::str name;
    py::str dir;
    int64_t shards = 0;
    int64_t imports = 0;
};

// Write the project (see the module's notes).
Scaffolded scaffold(const py::str& xex_path, const py::str& out_dir, const std::optional<int64_t>& jobs = std::nullopt);

py::str _config_h(const py::str& name, int64_t image_base, int64_t entry, int64_t image_size);

// The one shared header every src/*.cpp includes: runtime + all the function declarations + the mapping-table type.
py::str _recomp_header(const py::str& name, const py::list<int64_t>& decls);

py::str _init_cpp(const py::str& name, const py::list<int64_t>& chosen, const py::list<int64_t>& undeclared = py::list<int64_t>());

// Real host implementations for the tractable named imports: {import name: [C++ statement]}.
const py::dict<py::str, py::list<py::str>>& HOST_IMPL();

py::str _host_h(const py::str& name);

// the rexglue kernel core's exports: ((module, ordinal) -> export name, the export names with a real __imp__<name>)
struct RexExports {
    py::dict<std::tuple<py::str, int64_t>, py::str> ord2name;
    py::set<py::str> implemented;
};
extern std::optional<RexExports> _REX_EXPORTS;        // read once
const RexExports& _rex_exports();

// A thunk: (library, ordinal) at a guest address.
using Thunks = py::dict<int64_t, std::tuple<py::str, int64_t>>;

// One typed host-shim per imported kernel/XAM function.
py::str _imports_cpp(const py::str& name, const Thunks& thunks);

py::str _runtime_cpp(const py::str& name, int64_t entry);
py::str _cmakelists(const py::str& name);
py::str _launcher_bat(const py::str& name);
py::str _readme(const py::str& name, const powerpc::Agg& agg, int64_t image_base, int64_t entry, int64_t shards);

// The command line without the program name (sys.argv[1:]); prints to stdout. -> the exit code
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::recompile::project
