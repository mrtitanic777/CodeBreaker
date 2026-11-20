// port/tool_run.hpp -- `codebreaker port SOURCE_DIR OUT_DIR [options]`: the pieces of the command that are not the port itself.
//
// SOURCE_DIR is read, never written: its C (*.c, *.h, in it or its src/) and its assembler modules (*.asm, in it or its asm/). OUT_DIR
// gets the port: the modern sources, the platform layer, a build script, the program built, and PORT.md -- every change made and why,
// what the compiler still says, what could not be ported.
//
//   --compiler gcc|clang|msvc   the compiler to build for (gcc)
//   --runtime NAME              the old runtime's profile (dos_c_early)
//   --cc PATH                   the compiler's program
//   --cases CASES.json          the behaviour cases (port/verify): the sanitizer pass runs them
//   --asm16 ASSEMBLER           the period assembler (run under the emulator) whose listing places the assembly's tables
//   --types TYPES.json          {"members": {"struct.member": "type"}}: members the source declared otherwise than the program uses
//                               them (what the assembly shows) -- the port's corrections, said in PORT.md
//
// The command drives a port (`PortProgram`: the files as rules have made them, the mending) through rounds of a compiler's complaints,
// builds it, and reports. This file holds what is the same for every program: reading the options, the platform layer's files, the
// loop of check rounds (check every file a compiler has not seen in this form, mend what it says, go round again), the build script,
// and the watch on the port's own memory.
#pragma once

#include <atomic>
#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <string>
#include <thread>
#include <vector>

#include "base/cache.hpp"
#include "core/ordered_map.hpp"
#include "port/c_source.hpp"
#include "port/tool_build.hpp"
#include "port/tool_compilers.hpp"

namespace cb::port::tool {

using Files = pre_ansi::Files;

struct Options {
    std::filesystem::path source;
    std::filesystem::path out;
    std::string compiler = "gcc";
    std::string runtime = "dos_c_early";
    std::optional<std::string> cc;
    std::optional<std::filesystem::path> cases;
    std::optional<std::filesystem::path> asm16;
    std::optional<std::filesystem::path> types;
};

// The command line after `port`: two folders, then options. UsageError for fewer than two arguments, an option with no value, an
// option that is not one, or an OUT_DIR inside SOURCE_DIR (the port is written beside the source, never into it).
Options parse_options(const std::vector<std::string>& args);

// The usage text.
std::string usage();

// The platform layer the port is built with: dosrt.h and dosrt.c copied from `platform_dir`, and dosrt_config.h (the old runtime's
// ways: text mode, console text, exit status) from the runtime's profile.
struct RuntimeWays {
    std::string profile_name;
    bool text_mode = false;
    std::optional<bool> console_text;   // absent: as text_mode
    bool exit_status = true;
};
void write_platform_layer(const std::filesystem::path& out, const std::filesystem::path& platform_dir, const RuntimeWays& ways);

// What the loop of check rounds asks of the port.
struct CheckHooks {
    // The port's files as they are now (every rule applied); also written into the port's folder.
    std::function<Files()> write;
    // Mend what the compiler said; whether anything was changed.
    std::function<bool(const Files& texts, const std::vector<Diagnostic>& diags)> mend;
    // A round's one-line account (the command writes it to stderr).
    std::function<void(const std::string& line)> say;
};

struct CheckedCompiler {
    const Compiler* spec = nullptr;
    std::filesystem::path cc;
    std::string name;
};

struct CheckResult {
    int rounds = 0;
    std::vector<Diagnostic> diagnostics;   // the last round's
    Files texts;                           // the port's files as the last round wrote them
};

// The check loop over each compiler in turn (the port's own, then clang's eyes too: gcc does not check a call of an old-style
// definition, clang does). A round checks the C files whose text, or a header's they include, is not what it was and that the
// cache has no answer for; mends; and goes round again. It stops when the compiler says nothing to mend, when it says the same twice
// in a row (the rules have nothing more), when mending changes nothing, or after eight rounds. The later compilers' errors only
// count. `jobs`: files checked at once.
CheckResult run_check_rounds(cache::Cache& cache, const std::vector<CheckedCompiler>& compilers, const std::filesystem::path& out,
                             const CheckHooks& hooks, int jobs);

// The shell script that rebuilds the port with the compiler it was built with.
std::string build_script(const std::string& compiler_name, const Compiler& spec, const std::string& cc_name, const std::string& exe_name,
                         const std::vector<std::string>& sources);

// A watch on the port's own memory: above the ceiling (settings `port.max_memory_gb`, 4 by default) it writes where it was to
// <out>/port-memory.txt and stops the process before a rule that has run away can starve the machine.
class MemoryGuard {
public:
    MemoryGuard(std::filesystem::path out, double limit_gb, std::function<std::optional<std::uint64_t>()> used = {});
    ~MemoryGuard();
    MemoryGuard(const MemoryGuard&) = delete;
    MemoryGuard& operator=(const MemoryGuard&) = delete;
    // The bytes this process has committed, or nullopt when the system does not say.
    static std::optional<std::uint64_t> committed_bytes();
    // Whether the guard would stop now (for a test: the watch is a thread; this is its decision).
    bool over_limit() const;

private:
    void watch();
    std::filesystem::path out_;
    std::uint64_t limit_bytes_;
    std::function<std::optional<std::uint64_t>()> used_;
    std::atomic<bool> stop_{false};
    std::thread thread_;
};

}  // namespace cb::port::tool
