// cb/recompile/xenon.hpp -- XenonRecomp, the Xbox 360 recompiler CodeBreaker's projects are generated with.
//
//     Xenon xenon(config);
//     xenon.build();                         // native/xenonrecomp -> build/XenonRecomp/XenonRecomp.exe
//     xenon.recompile(config_toml);          // runs it on a project's configuration (config.hpp writes one)
//     xenon.recompile_synced(config_toml);   // the same into a clean folder, then only the changed files into the
//                                            // configuration's output folder (and the stale ones out)
//     xenon.instruction_tests();             // Xenia's PowerPC tests through it
//
// native/xenonrecomp is a fork of hedge-dev's XenonRecomp (MIT) with the changes the Xbox 360 system-software ports
// needed (VMX128 compares that set CR6, the import library index, symbol suffixes for several modules in one
// program, MMIO stores, the shared indirect-call table; every change from upstream is listed in
// native/xenonrecomp/CHANGES.md). It is the one XenonRecomp: the Xbox 360 Dashboard's line was merged into it. It
// turns each guest instruction into one C++ statement on a PPCContext; the runtime (native/x360) supplies the
// kernel, XAM, GPU, audio and input.
//
// It is built with clang-cl from Visual Studio and Ninja. The instruction tests assemble Xenia's tests
// (native/xenonrecomp/ppc_tests: inputs and the outputs measured on the console) with PowerPC binutils that know
// VMX128, recompile them, build them and run them: every printed line is a mismatch.
//
// Every program runs through cb::process (a Job Object, a timeout); a program that cannot be started or that fails
// is a ToolError carrying its output.
#pragma once

#include <chrono>
#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/process.hpp"

namespace cb::recompile::xenon {

namespace stdfs = std::filesystem;

// How a program is run: cb::process::run in production. The tests hand in an in-process stand-in that plays
// cmake, XenonRecomp, clang-cl ... so that nothing has to be built or installed (and no fake .exe is dropped on
// disk for a virus scanner to judge).
using ToolRunner = std::function<process::ProcessResult(const process::ProcessSpec&)>;

// Where things are and which tools to use. The settings the original read are plain values here.
struct Config {
    stdfs::path checkout_root;                       // the CodeBreaker checkout: native/xenonrecomp is below it
    stdfs::path state_dir;                           // where the instruction tests are built (xenonrecomp/ppc_tests)
    std::optional<stdfs::path> visual_studio;        // the Visual Studio install with clang-cl; found by vswhere if not set
    std::optional<stdfs::path> ppc_binutils;         // the folder with powerpc-none-elf-as / -objdump (instruction tests only)
    std::optional<unsigned> build_jobs;              // cmake --build -j; default: half the processors, at least 1
    std::chrono::minutes build_timeout{120};         // building XenonRecomp
    std::chrono::minutes recompile_timeout{180};     // one XenonRecomp run on a title
    std::chrono::minutes tool_timeout{30};           // every other program
    ToolRunner runner;                               // empty: cb::process::run
};

// The environment vcvars64.bat sets up (the linker, the SDK's headers and libraries).
using Environment = std::map<std::string, std::string>;

// What recompile_synced did to the output folder.
struct SyncResult {
    std::size_t generated = 0;                       // files this run made
    std::size_t changed = 0;                         // of those, the ones whose content differs from what was there
    std::vector<std::string> stale;                  // names this run did not make, removed from the folder
    std::vector<std::string> log;                    // XenonRecomp's output lines, without its progress lines
};

struct InstructionTestOptions {
    std::vector<std::string> names;                  // only these tests; empty: all
    std::optional<stdfs::path> work;                 // where they are built; default <state>/xenonrecomp/ppc_tests
    std::string march = "haswell";                   // clang's -march for the tests
};

struct InstructionTestResult {
    std::size_t tested = 0;
    std::vector<std::string> skipped;                // tests of instructions XenonRecomp does not implement
    std::vector<std::string> mismatches;             // the lines the tests printed
    std::vector<std::string> failed;                 // the tests those lines belong to
};

using ReportFn = std::function<void(std::string_view line)>;

class Xenon {
public:
    explicit Xenon(Config config);

    const Config& config() const { return config_; }
    stdfs::path source_dir() const { return config_.checkout_root / "native" / "xenonrecomp"; }
    stdfs::path build_dir() const { return source_dir() / "build"; }
    stdfs::path recomp_exe() const { return build_dir() / "XenonRecomp" / "XenonRecomp.exe"; }
    stdfs::path analyse_exe() const { return build_dir() / "XenonAnalyse" / "XenonAnalyse.exe"; }
    stdfs::path tests_dir() const { return source_dir() / "ppc_tests"; }
    // XenonUtils/ppc_context.h, the header every recompile is given
    stdfs::path context_header() const { return source_dir() / "XenonUtils" / "ppc_context.h"; }

    // The Visual Studio install that has clang-cl (the setting, else the newest vswhere finds). ToolError if none.
    stdfs::path visual_studio() const;
    // clang-cl.exe of that install. ToolError if it is not installed.
    stdfs::path clang_cl() const;
    // vcvars64's environment, read the first time it is asked for and kept.
    const Environment& msvc_env();

    // Build XenonRecomp and XenonAnalyse, only when a source is newer than the executables (or `force`).
    // -> the path of XenonRecomp.exe.
    stdfs::path build(bool force = false);

    // Run XenonRecomp on a configuration file. -> its output (warnings, the unimplemented).
    std::string recompile(const stdfs::path& config_toml);

    // XenonRecomp on `config_toml` into a clean folder, then into its out_directory_path only the files that
    // changed (the others keep their times, so builds stay incremental) and out the files this run did not make:
    // XenonRecomp never deletes, and a recompile that splits the code into fewer files would leave old ones
    // defining functions twice. `report` takes each line to show.
    SyncResult recompile_synced(const stdfs::path& config_toml, const ReportFn& report = {});

    // Xenia's PowerPC instruction tests (and ours) through XenonRecomp.
    InstructionTestResult instruction_tests(const InstructionTestOptions& options = {});

private:
    Config config_;
    std::optional<Environment> msvc_env_;
};

// Tests that are not run: a host-side helper, and one that needs a switch-table configuration test mode has not.
std::span<const std::string_view> skipped_instruction_tests();

// `codebreaker xenon-recompile <config.toml>`: the synced recompile for a build to run (the Xbox 360 Dashboard's
// modules), printing each line. UsageError unless `args` is exactly the configuration file.
void xenon_recompile_command(Xenon& xenon, std::span<const std::string> args);

// The out_directory_path of a configuration file's text, as the file gives it. nullopt if it has none.
std::optional<std::string> out_directory_path(std::string_view toml_text);

}  // namespace cb::recompile::xenon
