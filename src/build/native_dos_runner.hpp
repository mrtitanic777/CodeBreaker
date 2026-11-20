// build/native_dos_runner.hpp -- DosRunner (build/dos_run.hpp) on CodeBreaker's own DOS executive: what the toolchain drivers
// asked DOSBox-X for -- folders as drives, BLD.BAT's lines, the files read back -- run in-process, no emulator started.
//
//     NativeDosRunner runner(executive, &dosbox_runner);   // (the fallback: a run the executive cannot complete)
//     DosRunResult r = runner.run(spec);                    // as DosboxRunner::run
//
// The session is the batch file's lines as COMMAND.COM would run them: `X:` selects a drive, `SET NAME=value` sets a
// variable, `DEL file` deletes one, `ECHO`, `REM`, `CD` and `@ECHO OFF` do nothing a build needs; every other line is a
// program, found as COMMAND.COM finds it (the name as given, else .COM then .EXE in the current folder, then each PATH
// entry), with its output redirected as the line says (`> FILE`, `>> FILE`, `2>&1`; `> NUL` is nothing). A program the
// executive cannot complete -- a service it lacks, a CPU fault, a DOS extender it does not host -- makes the whole run go to
// the fallback (DOSBox-X while it is there); without one, the run's `ok` is false and nothing is pretended.
#pragma once

#include <mutex>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include "build/dos_run.hpp"
#include "build/native_dos.hpp"

namespace cb::build {

// One BLD.BAT line as COMMAND.COM reads it.
struct DosCommandLine {
    enum class Kind { Nothing, Drive, Set, Delete, Program };
    Kind kind = Kind::Nothing;
    std::string program;                    // Program: the name as typed ("cl", "c:\\ml.exe")
    std::string tail;                       // Program: the command tail, with its leading blank (" /c t.c")
    std::string name, value;                // Set: NAME, value; Delete: name (the file); Drive: name ("D")
    std::optional<std::string> out;         // `> FILE` / `>> FILE`: the file (as typed)
    bool append = false;                    // `>>`
    bool with_errors = false;               // `2>&1`: the standard error goes with it
};
DosCommandLine parse_dos_command(std::string_view line);

class NativeDosRunner final : public DosRunner {
public:
    explicit NativeDosRunner(const native_dos::DosExecutive& dos, const DosRunner* fallback = nullptr) : dos_(dos), fallback_(fallback) {}
    DosRunResult run(const DosRunSpec& spec) const override;

private:
    DosRunResult run_here(const DosRunSpec& spec) const;   // (NativeDosError when a program cannot complete)
    const native_dos::DosExecutive& dos_;
    const DosRunner* fallback_;
    // the sessions (their drives and programs) the executive could not do once: straight to the fallback from then on
    mutable std::mutex lock_;
    mutable std::set<std::string> fallen_back_;
};

}  // namespace cb::build
