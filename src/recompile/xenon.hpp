// recompile/xenon.hpp -- XenonRecomp, the Xbox 360 recompiler CodeBreaker's projects are generated with.
//
//     Xenon xenon(config);
//     xenon.recompile(config_toml);          // XenonRecomp on a project's configuration (config.hpp writes one)
//     xenon.recompile_synced(config_toml);   // the same into a clean folder, then only the changed files into the
//                                            // configuration's output folder (and the stale ones out)
//     xenon.instruction_tests();             // Xenia's PowerPC tests through it
//
// src/recompile/xenonrecomp is a fork of hedge-dev's XenonRecomp (MIT) with the changes the Xbox 360 system-software ports
// needed (VMX128 compares that set CR6, the import library index, symbol suffixes for several modules in one
// program, MMIO stores, the shared indirect-call table; every change from upstream is listed in
// src/recompile/xenonrecomp/CHANGES.md). It is the one XenonRecomp: the Xbox 360 Dashboard's line was merged into it. It
// turns each guest instruction into one C++ statement on a PPCContext; the runtime (src/recompile/x360) supplies the
// kernel, XAM, GPU, audio and input.
//
// It is linked into the program (LibXenonRecomp, LibXenonAnalyseMain) and runs in-process (xenon_engine.hpp). The
// instruction tests assemble Xenia's tests
// (src/recompile/xenonrecomp/ppc_tests: inputs and the outputs measured on the console) with PowerPC binutils that know
// VMX128, recompile them, build them and run them: every printed line is a mismatch.
//
// XenonRecomp failing is a ToolError carrying what it printed. The other programs (the binutils, clang-cl for the instruction
// tests) run through cb::process (a Job Object, a timeout).
#pragma once

#include <chrono>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "core/process.hpp"
#include "recompile/xenon_engine.hpp"

namespace cb::recompile::xenon {

namespace stdfs = std::filesystem;

// How a program is run: cb::process::run in production. The tests hand in an in-process stand-in that plays
// the binutils and clang-cl ... so that nothing has to be built or installed (and no fake .exe is dropped on
// disk for a virus scanner to judge).
using ToolRunner = std::function<process::ProcessResult(const process::ProcessSpec&)>;

// Where things are and which tools to use. The settings the original read are plain values here.
struct Config {
    stdfs::path checkout_root;                       // the CodeBreaker checkout: src/recompile/xenonrecomp is below it
    stdfs::path state_dir;                           // where the instruction tests are built (xenonrecomp/ppc_tests)
    std::optional<stdfs::path> visual_studio;        // the Visual Studio install with clang-cl; found by vswhere if not set
    std::optional<stdfs::path> ppc_binutils;         // the folder with powerpc-none-elf-as / -objdump (instruction tests only)
    std::chrono::minutes tool_timeout{30};           // every program it starts (the binutils, clang-cl)
    std::chrono::minutes compile_timeout{120};       // clang-cl building the instruction tests
    ToolRunner runner;                               // empty: cb::process::run
    xenon_engine::Tool recomp = xenon_engine::recomp;   // XenonRecomp (a test hands its own)
    // The p-code translator (recompile/pcode_emit, on this description: ppc_64_xenon_be.sla) writes every instruction it can, and
    // XenonRecomp the rest (docs/RECOMPILER.md, step 2). Unset: XenonRecomp writes them all.
    std::optional<stdfs::path> pcode_sla;
};

// What the p-code translator did in the last XenonRecomp run (all zero without one).
struct Translation {
    std::size_t translated = 0;                      // instructions it wrote
    std::size_t declined = 0;                        // instructions XenonRecomp wrote
    std::map<std::string, std::size_t> why;          // the reasons it declined, counted
};

// `inner` (XenonRecomp) with the p-code translator on the description `sla` asked first for every instruction; what it did goes
// into `counted` (reset at each run).
xenon_engine::Tool with_pcode(xenon_engine::Tool inner, stdfs::path sla, std::shared_ptr<Translation> counted);

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
    Translation translation;                         // what the p-code translator wrote (Config::pcode_sla)
};

using ReportFn = std::function<void(std::string_view line)>;

class Xenon {
public:
    explicit Xenon(Config config);

    const Config& config() const { return config_; }
    stdfs::path source_dir() const { return config_.checkout_root / "src" / "recompile" / "xenonrecomp"; }
    stdfs::path tests_dir() const { return source_dir() / "ppc_tests"; }
    // XenonUtils/ppc_context.h, the header every recompile is given
    stdfs::path context_header() const { return source_dir() / "XenonUtils" / "ppc_context.h"; }

    // The Visual Studio install that has clang-cl (the setting, else the newest vswhere finds). ToolError if none.
    stdfs::path visual_studio() const;
    // clang-cl.exe of that install. ToolError if it is not installed.
    stdfs::path clang_cl() const;
    // vcvars64's environment, read the first time it is asked for and kept.
    const Environment& msvc_env();

    // Run XenonRecomp on a configuration file. -> its output (warnings, the unimplemented).
    std::string recompile(const stdfs::path& config_toml);

    // XenonRecomp on `config_toml` into a clean folder, then into its out_directory_path only the files that
    // changed (the others keep their times, so builds stay incremental) and out the files this run did not make:
    // XenonRecomp never deletes, and a recompile that splits the code into fewer files would leave old ones
    // defining functions twice. `report` takes each line to show.
    SyncResult recompile_synced(const stdfs::path& config_toml, const ReportFn& report = {});

    // Xenia's PowerPC instruction tests (and ours) through XenonRecomp.
    InstructionTestResult instruction_tests(const InstructionTestOptions& options = {});

    // What the p-code translator did in the last XenonRecomp run.
    const Translation& last_translation() const { return translation_; }

private:
    // XenonRecomp in-process over `args`, with the p-code translator asked first when the configuration names one.
    xenon_engine::Run run_recomp(const std::vector<std::string>& args);

    Config config_;
    std::optional<Environment> msvc_env_;
    Translation translation_;
};

// Tests that are not run: a host-side helper, and one that needs a switch-table configuration test mode has not.
std::span<const std::string_view> skipped_instruction_tests();

// `codebreaker xenon-recompile <config.toml>`: the synced recompile for a build to run (the Xbox 360 Dashboard's
// modules), printing each line. UsageError unless `args` is exactly the configuration file.
void xenon_recompile_command(Xenon& xenon, std::span<const std::string> args);

// The out_directory_path of a configuration file's text, as the file gives it. nullopt if it has none.
std::optional<std::string> out_directory_path(std::string_view toml_text);

}  // namespace cb::recompile::xenon
