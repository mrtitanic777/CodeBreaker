// recompile/project.hpp -- a recompiled Xbox 360 title as a PC project that builds.
//
//     codebreaker recompile-project <image or .xex> -o <folder>                  (the rexglue layout, below)
//     Scaffolded made = scaffold(env, xex_path, folder);
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
//       graphics/   the native GPU subsystem (recompile/gpu), with the rexglue DXBC shader translator
//       kernel/     the rexglue kernel core (src/recompile/rexglue/kernel), which the recompiled code
//                   bridges onto: its PPCContext, and its real implementations of kernel exports
//       assets/     the image (loaded at its base) and the .xex
//
// What it is today: the project builds and links. To run a whole title, the host side has to be filled in where it is not yet:
// the imports the kernel core does not implement are stubs that log once, and the GPU bridge is a scaffold (GPU_DESIGN.md).
//
// `run_command` is the `recompile-project` command: by default (the x360 runtime) it hands a title to recompile/x360project
// (XenonRecomp's C++ on CodeBreaker's Xbox 360 runtime), unpacking a package, disc image or folder first; `--runtime rexglue`
// is `scaffold`. What lies outside this module -- reading an image and its .xex header, finding a title in a package or disc,
// the names of the imports -- is given as `Environment` and `CommandHooks`.
#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "store/converted.hpp"
#include "core/bytes.hpp"
#include "recompile/disassemble.hpp"
#include "recompile/powerpc.hpp"
#include "recompile/x360project.hpp"

namespace cb::recompile::project {

inline constexpr std::size_t kShardBytes = 1'500'000;   // target size of each src/*.cpp translation unit

// ---- the text of the files --------------------------------------------------------------------------------------------------

// The project's name from a module name or file name: without its extension and a "Xenon" mark, only letters, digits and
// underscores (ASCII: it becomes a C++ identifier and a CMake target), else "title".
std::string clean_name(std::string_view module);

// Group the per-function C++ into balanced translation units: the [begin, end) ranges of `parts`, each as soon as it
// reaches `target` bytes (the last may be shorter).
std::vector<std::pair<std::size_t, std::size_t>> shard_ranges(std::span<const std::string> parts, std::size_t target = kShardBytes);

std::string config_h(std::string_view name, std::uint32_t image_base, std::uint32_t entry, std::uint64_t image_size);

// The one shared header every src/*.cpp includes: the runtime, every function's declaration and the mapping-table type, so
// the recompiled code can be split across many translation units and still call across them.
std::string recomp_header(std::string_view name, std::span<const std::uint32_t> declared);

// The function-pointer table (guest address -> function); `undeclared` are import shims no recompiled code calls directly,
// declared here.
std::string init_cpp(std::string_view name, std::span<const std::uint32_t> mapped, std::span<const std::uint32_t> undeclared = {});

std::string host_h(std::string_view name);
std::string runtime_cpp(std::string_view name);
std::string cmakelists(std::string_view name);
std::string launcher_bat(std::string_view name);
std::string readme(std::string_view name, const powerpc::Aggregate& agg, std::uint32_t image_base, std::uint32_t entry, std::size_t shards);

// ---- the imports -----------------------------------------------------------------------------------------------------------

// An import stub of the image: where it is, which library (by the number the .xex header gives) and which ordinal.
struct Thunk {
    std::string library;
    std::uint32_t ordinal = 0;
};
using Thunks = std::map<std::uint32_t, Thunk>;   // guest address -> the import

// The names of the rexglue kernel core's exports: (module, ordinal) -> name from its export tables, and the names that have a
// real `__imp__<name>` function (the code the kernel core was built from).
struct RexExports {
    std::map<std::pair<std::string, std::uint32_t>, std::string> names;
    std::set<std::string, std::less<>> implemented;
};
// Read from src/recompile/rexglue/kernel (`rexglue` is src/recompile/rexglue); empty when the kernel is not in the tree.
RexExports read_rex_exports(const std::filesystem::path& rexglue);

// The host-implementation table: the statements that implement a tractable named import.
const std::map<std::string, std::vector<std::string>, std::less<>>& host_impl();

// The name an import has (its ordinal in the library, lower-case name like "xboxkrnl.exe"), "" for an unnamed one.
using ImportNames = std::function<std::string(std::string_view library_lower, std::uint32_t ordinal)>;

// One typed host-shim per imported kernel/XAM function: the recompiled-ABI sub_<addr> the game calls. In order of
// preference: a Vd* import goes to the native GPU subsystem; an export the kernel core implements goes to its
// `__imp__<name>`; a host implementation of this module's; else a stub that logs once.
std::string imports_cpp(std::string_view name, const Thunks& thunks, const ImportNames& names, const RexExports& rex);

// ---- writing the tree ----------------------------------------------------------------------------------------------------

// Write `data` to `path` unless it already holds exactly that: a file left alone keeps its time, so rebuilding a regenerated
// project recompiles only what changed. -> whether it was written.
bool write_if_changed(const std::filesystem::path& path, ByteView data);

// Make `dst` a copy of `src` (less the files and folders `drop` names), touching only files that differ and removing the files
// `src` no longer has.
void sync_tree(const std::filesystem::path& src, const std::filesystem::path& dst, const std::function<bool(const std::string&)>& drop);

// Copy the rexglue DXBC shader translator (src/recompile/rexglue/dxbc) or the kernel core (src/recompile/rexglue/kernel) into a project,
// without build products or standalone tests. -> whether there was one to copy.
bool copy_rex_dxbc(const std::filesystem::path& rexglue, const std::filesystem::path& dst);
bool copy_rex_kernel(const std::filesystem::path& rexglue, const std::filesystem::path& dst);

// What `scaffold` needs of a title, read from its image and, for a .xex, its header.
struct TitleImage {
    bool is_xex = false;
    std::optional<std::string> image_name;   // the .xex header's name for the image
    std::vector<std::string> libraries;      // its import libraries, in the order the import stubs number them (none: not a .xex)
    struct Stub {
        std::uint32_t address = 0;
        std::size_t library = 0;
        std::uint32_t ordinal = 0;
    };
    std::vector<Stub> imports;               // the image's import stubs
    powerpc::LoadedImage loaded;             // the image as the recompiler reads it
};

struct Environment {
    std::function<TitleImage(const std::filesystem::path&)> read_title;   // FormatError for what is not an image
    disassemble::VectorDecoder vector;
    ImportNames import_names;
    std::filesystem::path rexglue;                                        // src/recompile/rexglue
};

struct Scaffolded {
    std::string name;
    std::filesystem::path dir;
    powerpc::Aggregate agg;
    std::size_t shards = 0;     // the src/<name>_NNNN.cpp files
    std::size_t imports = 0;    // the host functions
    bool dxbc_copied = false;
    bool kernel_copied = false;
    bool xex_copied = false;    // the .xex into assets/ (best effort: a copy that fails leaves the project without it)
};

// Write the project (see the top of this file). `jobs`: threads for recompiling (none: the usual number).
Scaffolded scaffold(const Environment& env, const std::filesystem::path& xex_path, const std::filesystem::path& out_dir,
                    std::optional<unsigned> jobs = std::nullopt);

// ---- the command -----------------------------------------------------------------------------------------------------------

// A title found in a package, disc image or folder: its default.xex and its files.
struct TitleFiles {
    std::string name;
    std::string source;                                                  // what it was found in
    Bytes xex;                                                           // default.xex
    std::function<std::size_t(const std::filesystem::path&)> unpack;     // every file under the folder (once); how many it wrote
};

struct CommandHooks {
    Environment env;                                                     // for `--runtime rexglue`
    x360project::Recompiler recompiler;                                  // for the x360 runtime
    std::function<std::optional<TitleFiles>(const std::filesystem::path&)> locate_title;
    store::converted::Store store;                                        // the projects' folders
    std::filesystem::path pcode_sla;                                      // the Xenon's description, for --translator pcode
};

// `codebreaker recompile-project <title> [-o <folder>] [--name N] [--title-files <dir>] [--build] [--check [seconds]]
// [--runtime x360|rexglue] [--translator xenonrecomp|pcode] [-j N]`. `say` gets each line of output. -> the exit code (2: usage, 1: no title found or the
// build failed). UsageError for a flag without its value.
int run_command(const CommandHooks& hooks, std::span<const std::string> args, const std::function<void(std::string_view)>& say);

}  // namespace cb::recompile::project
