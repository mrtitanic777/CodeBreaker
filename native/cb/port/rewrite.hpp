// cb/port/rewrite.hpp -- one file's rewrite: the steps that take C of before the standard to C a compiler of today takes.
//
// `preprocess` points the old library's headers at port.h; `rewrite_definitions` turns the old-style definitions
// (`f(a, b) int a; char *b; { ... }`) into prototypes, keeping what the old calling convention meant (a function of
// any number of arguments, a value left in the register at the end); `finish` applies the rules the passes collected
// to every file last: renamed names, retyped declarations, lines the compiler loop mended.
#pragma once

#include <map>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include "cb/port/c_source.hpp"

namespace cb::port::pre_ansi {

// `#include` of the old library's headers -> port.h; the program's own macros for what the C library has (putc ...)
// dropped. `left_out` are the headers the port left out of its tree (their names are the old library's too).
std::string preprocess(std::string_view text, std::string_view fname, const Profile& profile,
                       const std::set<std::string, std::less<>>& left_out, Notes& notes);

// What the loop found the parameters of a function to be: (function, parameter index) -> its type.
using ParamTypes = std::map<std::pair<std::string, std::size_t>, std::string>;

struct RewrittenFile {
    std::string text;
    std::vector<std::string> functions;   // the functions of the file, as `fns` now has them (the ones it updated)
};

// The old-style definitions of a file as prototypes. `fns` holds the program's functions (those this file defines are
// updated in place: their parameters, `varargs`, `loose`, `bare`, `returns_void`, `body`); a function not in it is added.
RewrittenFile rewrite_definitions(std::string_view text, std::string_view fname, OrderedMap<std::string, Fn>& fns,
                                const ParamTypes& param_types, Notes& notes);

// A variable of one function that holds a pointer: declared a pointer there alone.
struct LocalRetype {
    std::string file, function, variable, type;
};

// The variables of one function that hold pointers, declared pointers there alone.
std::string apply_local_retypes(std::string_view text, std::string_view fname, const std::vector<LocalRetype>& retypes);

// Everything `finish` applies to a file, in the order of the fields.
struct FinishRules {
    std::map<std::string, std::string> renames;     // the C library's names the program meets: their new names
    std::vector<DeclRule> decl_rules;               // declarations given their real types
    std::vector<LocalRetype> local_retypes;
    std::map<std::string, std::uint64_t> array_sizes;   // `extern T a[];` -> its size, where it is defined
    LineFixes line_fixes;                           // lines the loop mended
    std::vector<NullRead> null_reads;               // reads through the null segment, as on DOS
};

struct Finished {
    std::string text;
    std::vector<LineOrigin> origin;   // each line of `text` before the line fixes: its text and which of the alike it was
};

Finished finish(const FinishRules& rules, std::string_view fname, std::string_view text);

}  // namespace cb::port::pre_ansi
