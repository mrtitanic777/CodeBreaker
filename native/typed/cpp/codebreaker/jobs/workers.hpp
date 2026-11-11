// codebreaker/jobs/workers.hpp -- worker PROCESSES of the program itself, for work that runs an emulator.
//
// The Python ran the gate's shares, the judge's items and each module of build_many in worker processes. Unicorn and the tracer DLL keep
// state per process (the TCG code buffer, the callback slots), so on threads they corrupt each other (found by the core16 integration):
// the emulator-bound work is handed to child processes of this same program, started in a worker mode:
//
//     core16 --cb-worker <gate|judge|build> <files...>
//
// A host program (the `codebreaker` exe, a harness) opts in with two lines at the top of its main():
//
//     int rc = 0;
//     if (codebreaker::jobs::workers::dispatch(argc, argv, rc)) return rc;     // this process is a worker: it did its share
//     codebreaker::jobs::workers::enable(argv[0]);                             // ... and may start workers of its own
//
// Without enable() (a program that never calls dispatch), nothing is started and the work runs in the caller's process, one share at a time.
// What a share needs travels in files (the parent writes them, the worker reads them and writes its answer to one): text, hex for anything that
// is not plain, so no encoding can change a result.
#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

namespace codebreaker::jobs::workers {

// This program's own path is `exe`: from now on workers may be started.
void enable(const std::string& exe);
bool enabled();

// Is this process a worker (argv[1] == "--cb-worker")? Then it ran its share: `rc` is its exit code.
bool dispatch(int argc, char** argv, int& rc);

// Run the program as a worker with these arguments (after "--cb-worker"); its exit code. Blocks.
int64_t run(const std::vector<std::string>& args);

// How many at once (base/parallel workers()).
int64_t count();

// ---- the files' text ------------------------------------------------------------------------------------------------------------
std::string hex(const std::string& raw);
std::string unhex(const std::string& text);

// Tokens separated by spaces: integers as they are, text and bytes as hex ("-" for empty).
class Enc {
  public:
    Enc& i(int64_t v);
    Enc& s(const std::string& v);
    const std::string& text() const { return out_; }

  private:
    std::string out_;
};
class Dec {
  public:
    explicit Dec(const std::string& text);
    int64_t i();
    std::string s();

  private:
    std::vector<std::string> tokens_;
    size_t at_ = 0;
};

std::string read_file(const std::string& path);
void write_file(const std::string& path, const std::string& text);

// A folder for one hand-over (removed by the caller).
std::string scratch_folder();

}  // namespace codebreaker::jobs::workers
