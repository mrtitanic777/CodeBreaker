// cb/invert/command.hpp -- `codebreaker invert`, the tool table: the compiler models and what reads their files.
//
//     codebreaker invert --tool NAME [arguments]        a model's own command line
//     codebreaker invert --early FILE OFFSET LENGTH     one function of a program built with the C compiler of 1983-84 (offset in hex)
//     codebreaker invert --check [MODEL]                each model against its own compiler
//     codebreaker invert <16-bit module> [--first N] [--flags "..."] [--show NAME]
//
// The tools that are native: early-c (the bytes of MS C 2.03 / 3.00 back to C), early-c-forward (the forward model against a quad file and its
// object), quads, quad-align, omf, known-differences. The rest of the Python's table -- the models of Microsoft C 6 and C/C++ 8 (msc6*, msvc2*)
// and the checks that put a model's text to the period compiler (early-c-check, early-c-round-trip, the frame and order probes) -- and the
// module pass that runs them over a 16-bit module are not in native/cb yet: a command that names one says so and exits 2. (Their models are
// the C++ of native/invert, on the Python-imitation layer, reached through cbinvert.dll; they move into native/cb as typed modules first.)
//
// A raw runner, not a declared cli::Command: every tool reads its own words.
#pragma once

#include <filesystem>
#include <functional>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace cb::invert::command {

struct Environment {
    // A corpus name -> the module's file (corpus.retail); nullopt when there is none. Used by the module pass.
    std::function<std::optional<std::filesystem::path>(std::string_view)> retail;
};

// The tools of the table, by name, with one line each, native or not.
struct Tool {
    std::string_view name, summary;
    bool native;
};
std::span<const Tool> tools();

// The text the command prints for no arguments (and for -h).
std::string usage();

// The command: `args` is what follows `invert`. The exit code (2 for a command line that names no tool, a tool that is not native, or no module).
int run(std::span<const std::string> args, const Environment& environment);

// As the command table's raw runner takes it.
std::function<int(std::span<const std::string>)> runner(const Environment& environment);

}  // namespace cb::invert::command
