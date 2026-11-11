// cb/core/process.hpp -- running external programs (OpenWatcom, MASM, DOSBox, cmd) the one way.
//
// Everything goes through ProcessSpec: an argv (never a shell string), a working directory, environment
// overrides, what to do with stdin/stdout/stderr, a timeout and a priority. Output is *bytes*; the caller
// decodes it with the code page the tool speaks (cb::text::from_cp437 ...), once.
//
// Every child is started inside a Job Object that CodeBreaker creates with kill-on-close. A timeout, a
// kill(), the Process object's destruction and our own exit terminate exactly the processes in that job. No
// code in CodeBreaker walks process trees or kills by name or PID: the job is how children-of-children are
// cleaned up without it (the standing safety rule).
//
//     ProcessResult r = process::run({.argv = {"wasm", "-zq", src.string()}, .cwd = work,
//                                     .err = Capture::Merge, .timeout = std::chrono::minutes(5)});
//     std::string log = text::from_latin1(r.out);
//
// See docs/CPP_DESIGN.md 3.10.
#pragma once

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"

namespace cb::process {

// What to do with an output stream of the child.
struct Capture {
    enum Mode {
        Collect,   // gathered into ProcessResult::out / ::err (or read through Process::read)
        Inherit,   // the child writes to our own stdout/stderr
        Discard,   // thrown away
        Merge,     // stderr only: goes where stdout goes (so the two interleave in order)
        File,      // written to `file` (truncated)
    };
    Mode mode = Collect;
    std::filesystem::path file;

    Capture(Mode m = Collect) : mode(m) {}   // NOLINT: lets `.err = Capture::Merge` read naturally
    static Capture ToFile(std::filesystem::path p) {
        Capture c(File);
        c.file = std::move(p);
        return c;
    }
};

enum class Priority { Normal, BelowNormal };   // BelowNormal for background sweeps

struct ProcessSpec {
    std::vector<std::string> argv;                    // argv[0] is found through PATH (and PATHEXT) as given below
    std::optional<std::filesystem::path> cwd;         // default: ours
    std::map<std::string, std::string> env;           // overrides on top of the inherited environment
    std::vector<std::string> env_remove;              // variables to drop from the inherited environment
    Bytes stdin_data;                                 // empty: stdin is NUL
    Capture out = Capture::Collect;
    Capture err = Capture::Collect;                   // or Capture::Merge
    std::chrono::milliseconds timeout{0};             // 0 = none; on expiry the whole job is terminated
    Priority priority = Priority::Normal;
    std::uint64_t memory_limit = 0;                   // bytes for the whole job; 0 = none
};

struct ProcessResult {
    std::uint32_t exit_code = 0;                      // as Windows gives it (unsigned 32-bit)
    Bytes out;
    Bytes err;
    bool timed_out = false;
    std::chrono::milliseconds elapsed{0};
    bool ok() const { return exit_code == 0 && !timed_out; }
};

// Run to completion. Throws ToolError only if the program cannot be started (not found, access denied).
// A program that runs and fails is a ProcessResult with a non-zero exit_code, not an exception: the caller
// decides what a failure means. Descendants still running when the program exits are terminated.
ProcessResult run(const ProcessSpec& spec);

// run(), but a non-zero exit code or a timeout is a ToolError (TimeoutError) carrying the captured output.
ProcessResult run_checked(const ProcessSpec& spec);

// A running child, for streaming (a worker process, an interactive tool). Output capture is by pipe: use
// Collect for the streams you want to read. The destructor terminates the job.
class Process {
public:
    enum class Stream { Out, Err };

    Process(Process&& other) noexcept;
    Process& operator=(Process&& other) noexcept;
    Process(const Process&) = delete;
    Process& operator=(const Process&) = delete;
    ~Process();

    // Read up to buf.size() bytes of the stream; blocks until some arrive. 0 means end of stream.
    std::size_t read(Stream which, ByteSpan buf);
    // Everything until end of stream.
    Bytes read_all(Stream which);
    // Write to the child's stdin (blocks until all is written); close it to signal end of input.
    void write(ByteView data);
    void close_stdin();
    // Wait for the child to exit; nullopt if `timeout` passes first. Returns the exit code.
    std::optional<std::uint32_t> wait(std::chrono::milliseconds timeout = std::chrono::milliseconds::max());
    bool running() const;
    // Terminate the job (the child and everything it started).
    void kill();

    struct Impl;   // (implementation detail; public only so the launch code in process.cpp can name it)

private:
    friend Process start(const ProcessSpec&);
    explicit Process(std::unique_ptr<Impl> impl);
    std::unique_ptr<Impl> impl_;
};

// Start a child with a pipe on stdin and on each stream set to Collect. Throws ToolError if it cannot start.
Process start(const ProcessSpec& spec);

// One argument quoted by the Windows argv rules (CommandLineToArgvW / the MSVC CRT): backslashes before a
// quote are doubled, embedded quotes escaped, spaces and empty arguments wrapped in quotes.
std::string quote_arg(std::string_view arg);
// The whole command line for an argv.
std::string command_line(const std::vector<std::string>& argv);

}  // namespace cb::process
