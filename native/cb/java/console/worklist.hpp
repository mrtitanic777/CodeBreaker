// cb/java/console/worklist.hpp -- compile the converted output and write a per-file failure worklist.
//
//     codebreaker java worklist <out_dir> [--cc g++] [--std c++03] [--out WORKLIST.md]
//
// For each <out_dir>/*.cpp it runs `<cc> -std=<std> -fsyntax-only -I<out_dir> <file>` and records the files that
// fail to compile, sorted by error count (fewest first = closest to clean), with the dominant error category.
#pragma once

#include <chrono>
#include <cstddef>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

namespace cb::java::console::worklist {

struct Options {
    std::string compiler = "g++";                        // a GCC-compatible front end
    std::string standard = "c++03";                      // the -std= value
    std::size_t jobs = 1;                                // files compiled at the same time
    std::chrono::milliseconds timeout{std::chrono::minutes(10)};   // per file; a file that times out fails
};

// A file that did not compile.
struct Failure {
    std::size_t errors = 0;       // lines of the compiler's output that report an error
    std::string file;             // the file's name inside out_dir
    std::string category;         // the most common error, see dominant_category()
};

struct Worklist {
    std::size_t total = 0;                // .cpp files compiled
    std::size_t clean = 0;                // those without errors
    std::vector<Failure> failing;         // fewest errors first, ties by file name
};

// The most common error message in a compiler's output, with quoted names and numbers made alike so that
// "unknown type 'Foo'" and "unknown type 'Bar'" count as one ("(unknown)" when no line says "error: "). The
// message is cut at 60 columns; a tie goes to the message that came first.
std::string dominant_category(std::string_view compiler_output);

// How many lines of the compiler's output report an error.
std::size_t count_errors(std::string_view compiler_output);

// Compile every <out_dir>/*.cpp (files directly in the folder, in name order) and sort out which fail. Throws
// ToolError if the compiler cannot be started.
Worklist check(const std::filesystem::path& out_dir, const Options& options = {});

// The worklist as Markdown, for `out_dir`.
std::string render(const std::filesystem::path& out_dir, const Worklist& worklist);

// check() and write the Markdown to `worklist_path`. Returns the one-line summary the command prints:
// "clean=3/10  failing=7  -> WORKLIST.md".
std::string run(const std::filesystem::path& out_dir, const std::filesystem::path& worklist_path,
                const Options& options = {});

}  // namespace cb::java::console::worklist
