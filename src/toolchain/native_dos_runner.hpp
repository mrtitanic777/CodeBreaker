// toolchain/native_dos_runner.hpp -- DosRunner (build/dos_run.hpp) on CodeBreaker's own DOS executive: what the toolchain drivers
// ask for -- folders as drives, BLD.BAT's lines, the files read back -- run in-process.
//
//     NativeDosRunner runner(executive);
//     DosRunResult r = runner.run(spec);
//
// The session is the batch file's lines as COMMAND.COM would run them: `X:` selects a drive, `SET NAME=value` sets a
// variable, `DEL file` deletes one, `ECHO`, `REM`, `CD` and `@ECHO OFF` do nothing a build needs; every other line is a
// program, found as COMMAND.COM finds it (the name as given, else .COM then .EXE in the current folder, then each PATH
// entry), with its output redirected as the line says (`> FILE`, `>> FILE`, `2>&1`; `> NUL` is nothing). A program the
// executive cannot complete -- a service it lacks, a CPU fault, a DOS extender it does not host -- ends the run with `ok`
// false and nothing pretended; `why` says what stopped it.
#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "toolchain/dos_run.hpp"
#include "toolchain/native_dos.hpp"

namespace cb::toolchain {

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
    explicit NativeDosRunner(const native_dos::DosExecutive& dos) : dos_(dos) {}
    DosRunResult run(const DosRunSpec& spec) const override;

private:
    DosRunResult run_here(const DosRunSpec& spec) const;   // (NativeDosError when a program cannot complete)
    const native_dos::DosExecutive& dos_;
};

}  // namespace cb::toolchain