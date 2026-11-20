// port/tool_compilers.hpp -- the compilers of today the port runs: where they are, how they are called, what they say.
//
// The port writes C a compiler of today takes; it asks gcc, clang or MSVC to check it and to build it, and reads their
// complaints back (the loop that mends them is port/mend.hpp). How each compiler is run is data, `compilers.json`:
//
//     {"gcc": {"find": ["gcc", "C:/msys64/mingw64/bin/gcc.exe"],           // names or paths (globs allowed) tried in turn
//              "check": ["{cc}", "-fsyntax-only", "-I{inc}", "{src}"],      // the command that checks one file
//              "build": ["{cc}", "-O1", "-I{inc}", "-o", "{out}", "{srcs}"],// the command that builds the program
//              "diag": "^(?P<file>...):(?P<line>\\d+):..."                  // a complaint: file, line, col, kind, text
//              "setup": ["C:/.../vcvars64.bat"]}}                           // a script run first in the same shell (MSVC)
//
// A user's own compilers, or changes to these, come from `settings.json` under port.compilers. `{cc}` is the compiler, `{inc}`
// the folder of the headers, `{out}` the program, `{src}` the file and `{srcs}` the C files (one argument each).
#pragma once

#include <filesystem>
#include <map>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "core/regex.hpp"

namespace cb::port::tool {

struct Compiler {
    std::vector<std::string> find;
    std::vector<std::string> setup;
    std::vector<std::string> check;
    std::vector<std::string> build;
    std::string diag;                               // the pattern of a complaint
    std::shared_ptr<const Regex> diag_regex;        // compiled once, when the compiler is loaded
};

// By name (gcc, clang, msvc, ml64, and the user's own).
using Compilers = std::map<std::string, Compiler, std::less<>>;

// The compilers of `compilers_json`, changed and added to by the user's settings file (`settings_json`, whose
// "port" -> "compilers" object holds the same keys; may be empty or absent). IoError / ParseError for a file that cannot be
// read or is not JSON; FormatError for a compiler whose `diag` is not a pattern.
Compilers load_compilers(const std::filesystem::path& compilers_json, const std::filesystem::path& settings_json = {});

// The first of `given` (if any) and `names` that is a program found on the PATH or a file matching a glob pattern (the
// newest by name when several match); nullopt when none.
std::optional<std::filesystem::path> find_compiler(std::span<const std::string> names, const std::optional<std::string>& given = std::nullopt);

// Files matching a path pattern whose segments may hold `*` and `?`, in reverse name order.
std::vector<std::filesystem::path> glob_reverse(std::string_view pattern);

// What a command template is filled with.
struct Values {
    std::string cc;
    std::string inc;
    std::string out;
    std::string src;
    std::vector<std::string> srcs;
};
// The template's words with {cc} {inc} {out} {src} replaced; a word that is exactly {srcs} becomes one word per file.
std::vector<std::string> expand(std::span<const std::string> tpl, const Values& values);

// A command's result.
struct Run {
    std::uint32_t exit_code = 0;
    std::string out;
    std::string err;
    std::string output() const { return out + err; }
    bool ok() const { return exit_code == 0; }
};

// A compiler's command, after its setup script (MSVC's vcvars) in the same shell. The compiler's own folder is first on
// the PATH (its DLLs beside it). ToolError if nothing can be started.
Run run(const Compiler& spec, std::vector<std::string> argv, const std::filesystem::path& cwd, const std::filesystem::path& cc);

// One complaint of a compiler.
struct Diagnostic {
    std::string file;       // the file's own name, no folder
    std::size_t line = 0;
    std::size_t col = 0;
    std::string kind;       // "error" or "warning" (what the compiler's pattern says)
    std::string text;
    std::vector<std::string> notes;   // the notes after it ("expected 'char *' but argument is of type 'int'")
    // A complaint inside a macro's expansion: where it was in the source (file as written, line, column) -- the file, line and
    // column above are then where the outermost use of the macro is.
    struct Place {
        std::string file;
        std::size_t line = 0;
        std::size_t col = 0;
    };
    std::optional<Place> macro_at;
};

// The complaints in a compiler's output, in the order it said them; a note joins the complaint before it.
std::vector<Diagnostic> diagnostics(const Compiler& spec, std::string_view output);

}  // namespace cb::port::tool
