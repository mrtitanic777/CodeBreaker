// cb/recompile/x360project.hpp -- an Xbox 360 title as a PC project on CodeBreaker's Xbox 360 runtime.
//
//     codebreaker recompile-project <default.xex> [-o <folder>] [--name N] [--title-files <dir>]
//     ScaffoldInfo info = scaffold(recompiler, xex_path, folder, name, title_files);
//     BuildResult built = build(folder, info.name, 4);
//     CheckResult run = check(folder, info.name, 60);
//
// The folder is the binary's project folder (Converted/<name>/) unless -o names another:
//
//     <folder>/
//       CMakeLists.txt        x360_title(<name> ...) from native/x360/x360.cmake
//       src/                  the title's XenonRecomp configuration, its switch tables (the finder's, with XenonAnalyse's),
//                             and default.xex in basic form
//       src/ppc/              XenonRecomp's C++ (one statement per guest instruction), regenerated: only changed files are
//                             rewritten, so a rebuild recompiles only those
//       src/module_info.cpp   the title described to the runtime (modules.h: g_titleModule)
//       assets/               the title folder the program runs: its files (--title-files) and default.xex in basic form;
//                             it is the title's game: device
//       build/                the CMake build: build/<name>.exe
//       reports/recompile.log what XenonRecomp said
//       Build-<name>.bat      configures and builds with clang-cl and Ninja
//       Play-<name>.bat       runs it
//
// The program is native/x360's runtime with the title linked in; run it as `build/<name>.exe assets` (or from assets/).
//
// What XenonRecomp's tool chain does (finding it, building it, the configuration it reads, the switch tables) is given as a
// `Recompiler`: recompile/xenon, config and discover own that knowledge.
#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/ordered_map.hpp"

namespace cb::recompile::x360project {

struct Recompiler {
    std::filesystem::path analyse;          // XenonAnalyse.exe
    std::filesystem::path recomp;           // XenonRecomp.exe
    std::filesystem::path source;           // XenonRecomp's source folder (XenonUtils/ppc_context.h is read from it)
    std::filesystem::path visual_studio;    // the Visual Studio folder the Build script loads vcvars64 from
    std::filesystem::path runtime_cmake;    // native/x360/x360.cmake

    std::function<void()> ensure_built;     // builds XenonRecomp and XenonAnalyse when they are not
    // `xex` as an unencrypted XEX with basic compression, the form XenonRecomp reads.
    std::function<void(const std::filesystem::path& xex, const std::filesystem::path& out)> write_basic;
    struct SwitchTables {
        std::size_t found = 0;              // the tables the finder located by itself
        std::size_t added = 0;              // those XenonAnalyse's output added
    };
    // Writes the switch tables file; `analysed` is XenonAnalyse's output when it made one.
    std::function<SwitchTables(const std::filesystem::path& file, const std::filesystem::path& basic, const std::optional<std::string>& analysed)>
        write_switch_tables;
    // The XenonRecomp configuration (TOML) for the image.
    std::function<std::string(const std::filesystem::path& basic, std::string_view out_dir, std::string_view switch_tables, std::string_view file_path)>
        make_config;
    // The address of __savegprlr_14 in the image, 0 when it has none.
    std::function<std::uint32_t(const std::filesystem::path& basic)> savegprlr_14_address;
};

// The program's, target's and scripts' name from a .xex path: its stem in lower case with every character that is not a
// letter or digit an underscore; "title" if nothing is left.
std::string default_name(const std::filesystem::path& xex_path);

// A name made safe for a target and script names: letters, digits and single underscores ("Pac-Man C.E." -> Pac_Man_C_E);
// "title" for nothing, and for "default".
std::string target_name(std::string_view name);

// Make `dst` hold `src`'s files, rewriting only those whose content changed (modification times of the rest are kept) and
// removing the rest.
void sync_folder(const std::filesystem::path& src, const std::filesystem::path& dst);

struct ScaffoldInfo {
    std::string name;
    std::filesystem::path dir;
    std::size_t functions = 0;              // entries of ppc_func_mapping.cpp
    std::size_t switches = 0;               // switch tables found and added
    std::size_t unimplemented = 0;          // "unimplemented" in what XenonRecomp said
};

// Writes the project. `name` is taken from the .xex when empty; `title_files` is copied into assets/. ToolError when
// XenonRecomp fails or leaves no function mapping (its output is in reports/recompile.log).
ScaffoldInfo scaffold(const Recompiler& recompiler, const std::filesystem::path& xex_path, const std::filesystem::path& out_dir,
                      const std::string& name = {}, const std::optional<std::filesystem::path>& title_files = std::nullopt);

struct BuildResult {
    bool ok = false;
    std::vector<std::string> tail;          // the error lines (else the last lines) of the build's output, 12 at most
};
// Runs Build-<name>.bat with at most `jobs` compiles at once; the whole output goes to reports/build.log.
BuildResult build(const std::filesystem::path& out_dir, std::string_view name, unsigned jobs = 4);

struct CheckResult {
    std::size_t frames = 0;                 // the highest frame the GPU reported presenting
    OrderedMap<std::string, std::size_t> unimplemented;   // import name -> calls, in the order first met
    std::string fatal;                      // its FATAL message, else a stall report, else empty
    std::optional<std::uint32_t> exit;      // the program's exit code; empty when it had to be ended
    std::filesystem::path log;
};

// What a title's log says (the parts of CheckResult that come from it).
CheckResult read_log(std::string_view text);

// Runs the built title headless for `seconds` (ending it then) and reads its log, assets/<name>.log.
CheckResult check(const std::filesystem::path& out_dir, std::string_view name, unsigned seconds = 60);

}  // namespace cb::recompile::x360project
