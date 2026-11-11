// cb/jobs/workers.hpp -- worker PROCESSES of the program itself, for work that runs an emulator.
//
// Threads are CodeBreaker's default way to run things side by side (cb/core/parallel.hpp). A worker process is for
// the work where isolation is the point: Unicorn and the tracer keep state per process (the TCG code buffer, the
// callback slots), so on threads two emulations corrupt each other, and a guest that crashes the host must take only
// its own process with it. That work (the gate's shares, the judge's items, one module of a build) is handed to child
// processes of this same program, started in a worker mode:
//
//     codebreaker.exe --cb-worker <kind> <args...>
//
// The host program opts in at the top of main():
//
//     jobs::WorkerKinds kinds;                       // what this program's workers can do
//     kinds.add("gate", [](const jobs::Args& a) { return gate_share(a); });
//     if (const std::optional<int> rc = jobs::run_as_worker(kinds, argc, argv)) return *rc;   // this IS a worker
//     const jobs::WorkerLauncher launcher = jobs::WorkerLauncher::for_this_program();          // ... and may start some
//
// What a share needs travels in files: the parent writes them (a `Handover` folder, deleted when it goes out of scope),
// the worker reads them and writes its answer to one. Their text is a line of tokens (`TokenWriter`/`TokenReader`):
// integers as they are, text and bytes as hex, so no encoding can change a result.
//
// The launcher puts each child in a Job Object (cb/core/process.hpp): a timeout, or this program's exit, ends exactly
// the worker and what it started. A worker that crashes is a non-zero exit code here, not an exception, and its
// siblings are unaffected.
#pragma once

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/process.hpp"

namespace cb::jobs {

using Args = std::vector<std::string>;

// argv[1] of a worker process.
inline constexpr std::string_view kWorkerFlag = "--cb-worker";
// What a worker process exits with when it was asked for a kind this program does not know, and when its share threw.
inline constexpr int kWorkerUnknownKind = 2;
inline constexpr int kWorkerFailed = 3;

// ---- what a program's workers can do --------------------------------------------------------------------------
// A worker's share: its arguments in, its exit code out (0 = it did its share; the answer is in a handover file).
using WorkerMain = std::function<int(const Args&)>;

class WorkerKinds {
public:
    // Throws UsageError for a kind added twice.
    void add(std::string kind, WorkerMain run);
    const WorkerMain* find(std::string_view kind) const;

private:
    std::vector<std::pair<std::string, WorkerMain>> kinds_;   // a handful, searched by name
};

// Is this process a worker (argv[1] is kWorkerFlag)? Then it runs its share and the exit code is returned; the host
// returns it from main(). Otherwise nullopt. A share that throws is reported on stderr and is exit code
// kWorkerFailed (the isolation boundary of a worker's job loop).
std::optional<int> run_as_worker(const WorkerKinds& kinds, int argc, char** argv);
std::optional<int> run_as_worker(const WorkerKinds& kinds, const Args& argv);   // argv[0] is the program

// ---- starting workers ------------------------------------------------------------------------------------------
struct WorkerResult {
    std::uint32_t exit_code = 0;   // as Windows gives it: a crash is 0xC0000005 and the like
    bool timed_out = false;        // the job was terminated at the deadline
    bool ok() const { return exit_code == 0 && !timed_out; }
};

struct LaunchOptions {
    std::chrono::milliseconds timeout{0};                    // 0 = none
    process::Priority priority = process::Priority::Normal;
};

class WorkerLauncher {
public:
    // `exe`: this program's own path. An empty path, or CB_NO_WORKERS in the environment, disables workers: callers
    // then run the share in their own process, one at a time (enabled() says which).
    explicit WorkerLauncher(std::filesystem::path exe);
    // The running program's own path (what GetModuleFileName says: argv[0] may be relative or lack its .exe).
    static WorkerLauncher for_this_program();

    bool enabled() const { return !exe_.empty(); }
    const std::filesystem::path& exe() const { return exe_; }

    // Run one share in a child and wait for it; stdout is discarded, stderr is ours. Throws UsageError when workers
    // are not enabled, ToolError when the child cannot be started.
    WorkerResult run(std::string_view kind, const Args& args, const LaunchOptions& options = {}) const;

    // The argv the child is started with (exposed for the tests and for callers that start it themselves).
    std::vector<std::string> command(std::string_view kind, const Args& args) const;

private:
    std::filesystem::path exe_;
};

// ---- the files' text ----------------------------------------------------------------------------------------------
// Tokens separated by single spaces: integers in decimal, text and bytes in lowercase hex ("-" for empty).
class TokenWriter {
public:
    TokenWriter& integer(std::int64_t v);
    TokenWriter& text(std::string_view v);
    TokenWriter& bytes(ByteView v);
    const std::string& str() const { return out_; }

private:
    void token(std::string_view t);
    std::string out_;
};

class TokenReader {
public:
    // Any run of spaces separates tokens; other whitespace (a trailing newline) is ignored.
    explicit TokenReader(std::string_view text);
    // Each throws FormatError (reader "worker tokens") when the tokens ran out or one is malformed.
    std::int64_t integer();
    std::string text();
    Bytes bytes();
    std::size_t remaining() const { return tokens_.size() - at_; }

private:
    std::string_view next(const char* what);
    std::vector<std::string> tokens_;
    std::size_t at_ = 0;
};

// A folder for the files of one hand-over, deleted (with everything in it) when the object goes out of scope, also
// by an exception: a crashed run no longer leaves multi-gigabyte folders behind.
class Handover {
public:
    // Created under <system temp>/codebreaker_workers.
    explicit Handover(std::string_view purpose = "w");
    const std::filesystem::path& path() const { return dir_.path(); }
    std::filesystem::path file(std::string_view name) const { return dir_.path() / std::string(name); }
    void write(std::string_view name, std::string_view text) const;
    std::string read(std::string_view name) const;

private:
    fs::ScopedTempDir dir_;
};

}  // namespace cb::jobs
