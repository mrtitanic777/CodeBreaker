// cb/translate/judge_cs_cpp_command.hpp -- `codebreaker judge-cs-cpp`: C# -> C++17 judged by running both.
//
//     codebreaker judge-cs-cpp [--cxx gcc|clang|msvc|PATH] [--keep] [--work DIR] [--l2] [FILE.cs | DIR ...]
//
// Each program (a file, or a folder of files that make one program; the default is the repo's C# corpus, tests/translate/corpus/csharp,
// one program per file) goes through the whole path and is scored at each step:
//
//     lowers     the Roslyn front end lowered all of it to IR (no Unsupported node)
//     writes     the C++17 back end (backends/csharp_cpp17) wrote it (nothing declined)
//     compiles   the host C++ compiler (g++, clang++ or cl) built the C++ with the runtime, data/translate/runtime/cpp17/csrt.hpp
//     agrees     the C# program (csc and the installed runtime) and the C++ printed the same output and exited with the same code
//
// The result is a table, one row per program (the scoreboard of docs/TRANSLATOR_DESIGN.md), and the exit code is 0 when every program
// agrees. --keep leaves the working folder (the Roslyn dumps, the C++, both runs' output); --l2 writes each program's IR beside it.
#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/base/config.hpp"
#include "cb/core/cli.hpp"
#include "cb/translate/dotnet/package.hpp"

namespace cb::translate::judge_cs_cpp {

// What a judged run tells whoever watches it (the window's progress, translate/translate_file.hpp). Every member may be empty. A member may throw to
// stop the run (a cancelled conversion): nothing in the judges catches what is not an Error.
struct Observer {
    // Translating: `done` of `total` functions written; total 0 while the front end has not yet said how many there are.
    std::function<void(std::uint64_t done, std::uint64_t total, std::string_view what)> translating;
    // Checking: step `done` of `total` ("Building the C++", "Running the original" ...); `what` names the step that begins.
    std::function<void(std::uint64_t done, std::uint64_t total, std::string_view what)> checking;
};

struct Environment {
    cb::config::Config config;
    Observer observer;
};

// The host C++ compiler the generated program is built with.
enum class Compiler { Gcc, Clang, Msvc };
struct Cxx {
    Compiler kind = Compiler::Gcc;
    std::filesystem::path exe;   // empty for Msvc: found through Visual Studio's vcvars64.bat
};
// `want`: "gcc", "clang", "msvc", a path to a compiler, or empty (the first the machine has, in that order). nullopt: none.
std::optional<Cxx> find_cxx(const std::string& want);

// One program's scores. A step that was not reached is nullopt.
struct Score {
    std::string program;
    std::optional<bool> lowers, writes, compiles, agrees;
    std::string detail;   // what stopped it, or the first difference
};

// Judge one program (its files) in `work` (made new).
Score judge_program(const dotnet::Sdk& sdk, const Environment& environment, const Cxx& cxx, const std::string& name,
                    const std::vector<std::filesystem::path>& files, const std::filesystem::path& work, bool write_l2 = false);

// Build one C++ source (with the translator's runtime folder on the include path) into `exe`; `log` is the compiler's output.
// false: it did not compile.
bool build_program(const Cxx& cxx, const std::filesystem::path& runtime, const std::filesystem::path& source, const std::filesystem::path& exe, std::string& log);

// The score table: a markdown table, one row per program.
std::string table(const std::vector<Score>& scores);

cli::Command command(const Environment& environment);
int main(const std::vector<std::string>& args, const Environment& environment);

}  // namespace cb::translate::judge_cs_cpp
