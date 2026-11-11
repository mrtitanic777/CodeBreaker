// cb/port/mend.hpp -- mending the port from the complaints of a compiler (the Python's pre_ansi `mend`).
//
// The port's rewriting passes (old_c_passes.hpp and its siblings) make the old C as a compiler of today takes it
// as far as reading it can tell. What is left, a compiler says: this argument is a pointer where an int is wanted,
// this member is not in that structure, this function gives an int where a pointer is used. `mend` reads those
// complaints (gcc's words, clang's, MSVC's) and answers each with the smallest change that keeps the meaning old C
// gave it: a cast on the line, a parameter declared the pointer it always held (intptr_t where either was kept), a
// function declared to give a pointer, a structure's header included. A change to one line is kept as a rule (the
// next round's text has the line again: `line_fixes`); a change to a declaration is a rule on the text of every file
// (`decl_rules`) or a retyping of one local (`local_retypes`). The loop that runs a compiler and calls `mend` until
// nothing is left to mend belongs to the caller.
//
// MendState is the part of the port that this reads and writes: the functions the program defines, the files'
// own texts, and the rules and notes the mending leaves. The Port owns one.
#pragma once

#include <cstddef>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include "cb/port/old_c_passes.hpp"

namespace cb::port {

// A parameter of a function: its name, and its declaration whole ("register char *p", "intptr_t n").
struct Param {
    std::string name;
    std::string type;
};

// A function of the program as the passes know it: what its definition said, and what the passes have since made of it.
struct Function {
    std::string name;
    std::string ret; // empty: old C's int, unsaid
    std::vector<Param> params;
    bool is_static = false;
    bool returns_void = false;
    bool loose = false; // called with other numbers of arguments than it has: old style kept
    bool varargs = false;
    bool ret_set = false; // its return type was settled by a pass
    bool int_ret = false; // its callers take it for a pointer but it gives an integer
    bool keep_int = false;
    std::set<std::string> implicit; // parameters its definition never declared (ints by default)
    std::string body;
    std::optional<std::string> file;
};

// A variable the mending retyped. `function` empty: a global (declared the same in every file).
struct VariableKey {
    std::string file;
    std::string function;
    std::string variable;
    friend auto operator<=>(const VariableKey&, const VariableKey&) = default;
};

// An integer declaration that held a pointer, rewritten in every file: `pattern` is a regular expression over a
// file's text, `replacement` its template ($1..$9 the groups, $$ a dollar); or, when `split` is set, a declaration
// of several names (int a, tag, b;) of which only `split->name` is retyped: int a, b; struct tag *tag;
struct SplitDecl {
    std::string name;
    std::string type;
    friend bool operator==(const SplitDecl&, const SplitDecl&) = default;
};
struct DeclRule {
    std::string pattern;
    std::string replacement;
    std::optional<SplitDecl> split;
    friend bool operator==(const DeclRule&, const DeclRule&) = default;
};

// A line of a file as the passes found it before the rules changed it, for keeping a mended line across rounds.
struct LineOrigin {
    std::string text;
    long n = 0;
};
struct LineFixKey {
    std::string file;
    std::string text;
    long n = -1; // -1: the line is identified by its text alone
    friend auto operator<=>(const LineFixKey&, const LineFixKey&) = default;
};

struct MendState {
    // what the program defines and the port's own text of it
    std::map<std::string, Function> functions;
    OrderedMap<std::string, std::string> files;      // the port's text of each file, in the program's order
    OrderedMap<std::string, std::string> asm_protos; // the assembly routines: their C declarations
    std::set<std::string> libc;                      // the names of the old C library's functions
    bool arg_order_left = false;                     // the old compiler evaluated arguments left to right
    bool int16 = false;                              // every int is 16 bits, as the 8086's were

    // what the mending leaves
    Notes mended;                                     // why, and where
    std::map<std::string, std::string> renames;       // functions renamed (proto.h agrees with the definitions)
    std::map<VariableKey, std::string> local_retypes; // a local retyped, by function (function never empty)
    std::map<VariableKey, std::string> retyped;       // everything retyped, local or global
    std::vector<DeclRule> decl_rules;
    std::map<LineFixKey, std::string> line_fixes;
    std::map<std::string, std::map<std::size_t, LineOrigin>> line_origin;   // file -> line (1-based) -> its origin
    std::map<std::string, std::set<std::string>> need_headers;              // file -> headers it must include
    std::map<std::pair<std::string, std::size_t>, std::string> param_types; // (function, parameter): its type
    std::set<std::pair<std::string, std::size_t>> given_params;             // given pointers by their callers
    std::map<std::string, std::set<std::string>> fptr; // a function pointer: the types of what it is given
    std::set<std::string> fptr_done, fptr_mixed;       // given its prototype / given functions of several
};

// One complaint of a compiler.
struct Diag {
    std::string file;
    std::size_t line = 0;           // 1-based
    std::optional<std::size_t> col; // 1-based
    std::string text;               // the message
    std::vector<std::string> notes; // the notes after it ("expected 'char *' but argument is of type 'int'")
};

// An integer parameter a function gives on, as it is, to a parameter that holds a pointer whole (intptr_t): it
// carried the pointer too (16 bits both), so it is intptr_t. True if any was made so.
bool passed_on(MendState& state, Sources& out);

// push(n) intptr_t n; { s->v = n; } and pop() { int v; v = s->v; return v; }: an intptr_t parameter (a pointer it
// carries) kept in a member, and taken out again by a function giving it back. The member, the local and the
// function's value are intptr_t too (16 bits all then). True if any was made so.
bool stored_wide(MendState& state, Sources& out, const Rules& rules);

// The member of a union (its definition in the port's text) that a value of the type is: of the type itself, or the
// like kind (a double the floating one, a pointer the first pointer, a number the integer of its size).
std::optional<std::string> union_member_for(const Sources& out, std::string_view union_type, std::string_view type);

// The struct and union tags a file sees: its own and its headers' (theirs, in turn).
std::set<std::string, std::less<>> visible_tags(const Sources& out, std::string_view file);

// One round of mending from the compiler's complaints; true if anything was changed.
bool mend(MendState& state, Sources& out, const std::vector<Diag>& diags, const Rules& rules);

} // namespace cb::port
