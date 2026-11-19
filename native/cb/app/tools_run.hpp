// cb/app/tools_run.hpp -- running a tool: the command as a child process of this same program (`codebreaker <command> <args>`), its output streamed into a
// buffer a window reads from its own thread, the percentage it prints followed, and Stop that ends exactly that child.
//
//     tools::Run run(tools::build_argv(tool, values));
//     ... each frame:  tools::RunState s = run.state();   // s.output so far, s.running, s.percent (-1: the command prints none), s.exit_code
//     run.stop();                                          // the child (and what it started) end; nothing else is touched
//
// The child is started through core/process.hpp: inside a job object it alone owns, so Stop and the destructor end that job and nothing else (no process is
// found by name or walked as a tree). No operating-system API here.
#pragma once

#include <atomic>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

namespace cb::app::tools {

struct RunState {
    std::string output;            // everything the command wrote (stdout and stderr in order), the most recent 256 KB
    bool running = true;
    bool stopped = false;          // ended by Stop
    int exit_code = 0;             // when not running
    int percent = -1;              // the last "NN%" a line began with; -1 while the command has printed none (an indeterminate bar)
    std::string failure;           // the program could not be started: why
    std::uint64_t version = 0;     // grows with every change: a window repaints when it differs from the one it drew
};

// The percentage a line begins with ("  35%  Translating"): -1 when it does not.
int percent_of_line(std::string_view line);

class Run {
public:
    // Starts at once. `exe` empty: this program (fs::executable_path()). `cwd` empty: ours.
    explicit Run(std::vector<std::string> argv, std::filesystem::path exe = {}, std::filesystem::path cwd = {});
    ~Run();
    Run(const Run&) = delete;
    Run& operator=(const Run&) = delete;

    RunState state() const;
    void stop();

private:
    struct Shared;
    std::shared_ptr<Shared> shared_;
    std::thread reader_;
};

}  // namespace cb::app::tools
