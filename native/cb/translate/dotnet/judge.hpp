// cb/translate/dotnet/judge.hpp -- the judge for whole programs: run a program in its own runtime and its translation
// in the target's, and compare what they do (docs/TRANSLATOR_DESIGN.md, "The judge, for every pair").
//
// What is compared is what a program says to the outside: its standard output, byte for byte after line endings are
// made the same, and its exit code. A verdict is one of
//
//     Equivalent   both ran; the same output and exit code
//     Divergent    both ran; the output or the exit code differs (the first differing line is kept)
//     Refused      the translator declined something in the program (its ledger says what and why), so the translation
//                  is not complete and is not run
//     NoCompile    the translation did not compile (the compiler's first errors are kept)
//     NoOriginal   the original did not build or run (nothing to judge against)
//
// Adapters (each gives a Run): `csharp` (the installed .NET SDK's csc and runtime, no restore), `java` (javac and the
// JVM of the Java lane), `cpp` (the host C++ compiler: a transpiled program and its runtime). The Java and C++ adapters
// are told where the JDK and the host compiler are and how a transpiled program is built (those belong to the Java lane).
//
//     Run r = judge::csharp(sdk, {"Prog.cs"}, work);
//     Judgement j = judge::compare(original_run, translated_run);
#pragma once

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/translate/dotnet/package.hpp"

namespace cb::translate::dotnet::judge {

inline constexpr std::chrono::seconds kTimeout{120};   // how long a program may run

// One program run: built (ok), its exit code (nullopt: it did not finish), standard output and error; `log` is the
// build's output.
struct Run {
    bool ok = false;
    std::optional<std::uint32_t> exit;
    std::string out, err, log;
};

enum class Verdict { Equivalent, Divergent, Refused, NoCompile, NoOriginal };

// "equivalent", "divergent", "refused", "no-compile", "no-original": the words the ledgers and reports use.
std::string_view word(Verdict v);

struct Judgement {
    Verdict verdict = Verdict::Equivalent;
    std::string detail;   // what to look at: the first differing line, the compiler's errors, the refusals
};

// Judge a translation against its original. `refused`: what the translator declined, if it did.
Judgement compare(const Run& original, const Run& translation, std::span<const std::string> refused = {});

// Output as compared: line endings made \n, trailing line ends dropped.
std::string normalise(std::string_view output);

// The first compile errors of a compiler's log (twelve lines containing ": error "), else its last 800 bytes.
std::string first_errors(std::string_view log);

// A folder made new: removed with what it holds, made again.
void fresh_folder(const std::filesystem::path& work);

// Compile C# files with the SDK's csc and run the program on the installed runtime (`main`: the class whose Main is the
// entry point, where several have one).
struct CSharpOptions {
    std::vector<std::string> args;
    std::chrono::seconds timeout = kTimeout;
    std::string name = "Program";
    std::optional<std::string> main;
};
// What the adapter needs of a .NET toolchain: a compiler and a runner. `of` is the installed SDK (which must outlive
// the toolchain made from it).
struct CSharpToolchain {
    std::function<Compiled(const CompileOptions&)> compile;
    std::function<RunResult(const std::filesystem::path& dll, const RunOptions&)> run;
    static CSharpToolchain of(const Sdk& sdk);
};
Run csharp(const CSharpToolchain& tools, const std::vector<std::filesystem::path>& files, const std::filesystem::path& work,
           const CSharpOptions& options = {});

// The JDK of the Java lane.
struct Jdk {
    std::filesystem::path javac, jvm;
};

struct JavaOptions {
    std::vector<std::string> args;
    std::chrono::seconds timeout = kTimeout;
    std::vector<std::filesystem::path> classpath;
};
// Compile a Java source tree with javac and run class `main` (qualified) on the JVM. `jdk` is empty when there is none.
Run java(const std::optional<Jdk>& jdk, const std::filesystem::path& src_dir, std::string_view main,
         const std::filesystem::path& work, const JavaOptions& options = {});

// How the host C++ compiler builds a program the Java transpiler's C++03 back end wrote: the library of its sources,
// then the program from a harness and that library.
struct HostBuild {
    struct Native {
        bool ok = false;
        std::string why;   // what went wrong, when it did not
    };
    // The library of every .cpp of `cpp_dir`, built under `work`; nullopt if it did not build.
    std::function<std::optional<std::filesystem::path>(const std::filesystem::path& cpp_dir,
                                                       const std::filesystem::path& work, unsigned jobs)>
        library;
    // The program `exe` from `harness` and `library`.
    std::function<Native(const std::filesystem::path& cpp_dir, const std::filesystem::path& harness,
                         const std::filesystem::path& exe, const std::filesystem::path& library)>
        program;
    // Where `library` leaves an object for each source, to say which ones did not compile: <work>/probe/_objects.
    std::filesystem::path objects_folder(const std::filesystem::path& work) const { return work / "probe" / "_objects"; }
};

struct CppOptions {
    std::vector<std::string> args;
    std::chrono::seconds timeout = kTimeout;
    unsigned jobs = 2;
};
// Build a transpiled program (every .cpp of `cpp_dir` with its runtime headers) with the host C++ compiler, and run
// `main_class::main` after the static initialisers. `host` is empty when there is no host compiler.
Run cpp(const std::optional<HostBuild>& host, const std::filesystem::path& cpp_dir, std::string_view main_class,
        const std::filesystem::path& work, const CppOptions& options = {});

}  // namespace cb::translate::dotnet::judge
