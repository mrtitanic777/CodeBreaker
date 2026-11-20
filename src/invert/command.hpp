// invert/command.hpp -- `codebreaker invert`, the tool table: the compiler models and what reads their files.
//
//     codebreaker invert --tool NAME [arguments]        a model's own command line
//     codebreaker invert --early FILE OFFSET LENGTH     one function of a program built with the C compiler of 1983-84 (offset in hex)
//     codebreaker invert --check [MODEL]                each model against its own compiler
//     codebreaker invert <16-bit module> [--first N] [--flags "..."] [--show NAME]
//
// Every tool is native: early-c (the bytes of MS C 2.03 / 3.00 back to C), early-c-check / early-c-round-trip / the frame and order probes (the early model put
// to the compiler of its time), early-c-forward (the forward model against a quad file and its object), quads, quad-align, omf, known-differences,
// early-c-frame (the frame model against the compiler's own frames), the small readers of Microsoft C 6.0a (msc6-templates, msc6-walk, msc6-function,
// msc6-registers: cb/invert/msc6_small), the larger one (msc6, msc6-idioms: cb/invert/msc6) and the Microsoft C/C++ 2.0 model (msvc2, msvc2-forward:
// invert/msvc2). The module pass (`invert <16-bit module>`) runs the models over a module's functions and puts each reading to the compiler
// (cb/invert/module_pass).
//
// What a tool puts a model's text to is a compiler, and a compiler is the environment's: where one is not set up the tool says so and goes without.
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

#include "core/bytes.hpp"
#include "invert/module_pass.hpp"

namespace cb::invert::command {

struct Environment {
    // A corpus name -> the module's file (corpus.retail); nullopt when there is none. Used by the module pass.
    std::function<std::optional<std::filesystem::path>(std::string_view)> retail;
    // The code bytes Microsoft C 6.0a gives for one source text under these switches (build/msc6); none: the compiler refused the text. Absent
    // where the compiler is not set up: a tool that puts a model's text to it says so and goes without.
    std::function<std::optional<Bytes>(std::string_view text, std::string_view flags)> compile_msc6;
    // The same for Microsoft C/C++ 8 (Visual C++ 1.52c), which built some of the 16-bit modules.
    std::function<std::optional<Bytes>(std::string_view text, std::string_view flags)> compile_msvc152;
    // The same for many texts at once, one answer per text: each compile is a DOS program, and starting it is most of its cost, so the module pass
    // gives a compiler every text of a switch set in one call where this is set (else it asks one text at a time).
    module_pass::CompileMany compile_msc6_many, compile_msvc152_many;
    // The object file (OMF) Microsoft C 2.03 gives for one source text (MC1 then MC2); none: it refused the text. `stem` names the work files.
    std::function<std::optional<Bytes>(std::string_view text, std::string_view stem)> compile_mc203;
    // The 32-bit compiler of 1994: an object file, or what it said.
    struct Msvc2Object {
        std::optional<Bytes> object;
        std::string error;
    };
    std::function<Msvc2Object(std::string_view text, std::string_view flags)> compile_msvc2;
    // The functions of a 16-bit module as its describer finds them (names, places, bytes, relocations, SDK signatures, how they are called); absent: the
    // module's framed functions are found by their frames (module_pass::framed_functions).
    std::function<std::vector<module_pass::Function>(const std::filesystem::path&)> functions;
};

// The tools of the table, by name, with one line each, native or not.
struct Tool {
    std::string_view name, summary;
};
std::span<const Tool> tools();

// The text the command prints for no arguments (and for -h).
std::string usage();

// The command: `args` is what follows `invert`. The exit code (2 for a command line that names no tool, a tool that is not native, or no module).
int run(std::span<const std::string> args, const Environment& environment);

// As the command table's raw runner takes it.
std::function<int(std::span<const std::string>)> runner(const Environment& environment);

}  // namespace cb::invert::command
