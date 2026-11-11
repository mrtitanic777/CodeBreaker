// codebreaker/port/__main__.hpp -- typed C++ port of codebreaker.port.__main__.
//
// python -m codebreaker.port SOURCE_DIR OUT_DIR [--compiler gcc|clang|msvc] [--runtime NAME] [--cc PATH]
//
// SOURCE_DIR is read, never written: its C (*.c, *.h, in it or its src/) and its assembler modules (*.asm, in it or
// its asm/). OUT_DIR gets the port: the modern sources, the platform layer, a build script, the program built, and
// PORT.md - every change made and why, what the compiler still says, what could not be ported.
//
//   --cases CASES.json   the behaviour cases (port/verify): the sanitizer pass runs them
//   --asm16 ASSEMBLER    the period assembler (run under the emulator) whose listing places the assembly's tables
//   --types TYPES.json   {"members": {"struct.member": "type"}}: members the source declared otherwise than the
//                        program uses them (what the assembly shows) - the port's corrections, said in PORT.md
//
// Typing notes (what the Python leaves open):
//   * The Python's dicts with fixed keys are records: a compiler of compilers.json is `Compiler` (find / check / build /
//     setup lists, diag text - each may be absent: the Python's spec.get / spec[...]; a key that is used and absent is
//     KeyError), a runtimes/*.json profile is `RuntimeProfile` (the keys this module reads; pre_ansi::Profile has the
//     ones Port reads), a compiler's complaint is `Diagnostic` (file, line, col, kind, text, and the "notes" /
//     "macro_at" keys it may get).
//   * HERE is the folder codebreaker/port of the checkout (CB_CHECKOUT_ROOT/codebreaker/port; settings.json is in the
//     checkout's root): the C++ has no __file__.
//   * subprocess.run(..., env=env_for(cc)) has no equivalent in py::subprocess (no env parameter): `run` puts the
//     compiler's folder in front of this process' PATH while any run of it is going (the children inherit it), as
//     env_for says; two compilers' runs at once are not supported (the Python never does that either).
//   * cache::value / keep are typed (no dict of mixed values): the compiler's complaints of one file are kept as a
//     list of tuples under the cache kind "port-checks-cpp" (Python's "port-checks" holds dicts, which a typed
//     cache cannot read), so the two programs do not share these entries (the objects, "port-objects", are shared).
//   * The Python's function attributes (asm_data_defs.one_place / .aliases / .sizes) are the fields of DataDefs.
//   * oldlayout_pass leaves the lines it mended (Python: port._ol_wanted) in `wanted`.
//   * memory_guard: the Python dumps all threads' tracebacks (faulthandler) into port-memory.txt; C++ has no such
//     dump: the file holds the message alone.
//   * main(argv, out): what the Python prints goes to `out`; stderr is the process' stderr.
#pragma once

#include <cstdint>
#include <functional>
#include <iosfwd>
#include <optional>
#include <tuple>

#include "py.hpp"
#include "py_os.hpp"

#include "codebreaker/port/asm16.hpp"
#include "codebreaker/port/pre_ansi.hpp"

namespace codebreaker::port::__main__ {

// the folder of codebreaker/port
extern const py::str HERE;

// ---------------------------------------------------------------- compilers.json
struct Compiler {
    std::optional<py::list<py::str>> find;      // names or paths (globs allowed) tried in turn
    std::optional<py::list<py::str>> check;     // the command that checks a file
    std::optional<py::list<py::str>> build;     // the command that builds the program
    std::optional<py::list<py::str>> setup;     // a script run first in the same shell (MSVC's vcvars)
    std::optional<py::str> diag;                // the pattern of a complaint (file, line, column, kind, text)
};
using Compilers = py::dict<py::str, Compiler>;

// compilers.json, over it the user's own (settings.json: port.compilers)
Compilers load_compilers();

// the first of `given` and `names` that is a program (on the PATH, or a path/glob that exists)
std::optional<py::str> find(const py::list<py::str>& names, const std::optional<py::str>& given = std::nullopt);

// the environment of a compiler's run: this process' own, its folder first on the PATH
py::dict<py::str, py::str> env_for(const py::str& cc);

// subprocess.CompletedProcess of a run (text of both streams)
struct Process {
    int64_t returncode = 0;
    py::str stdout_;
    py::str stderr_;
};

// a compiler's command, after its setup script (MSVC's vcvars) in the same shell
Process run(const Compiler& spec, const py::list<py::str>& argv, const py::str& cwd, const py::str& cc);

// a command template: {srcs} the list, the others str.format'ed with kw
py::list<py::str> args(const py::list<py::str>& tpl, const py::dict<py::str, py::str>& kw,
                       const std::optional<py::list<py::str>>& srcs = std::nullopt);

// a complaint of the compiler
struct Diagnostic {
    py::str file;
    int64_t line = 0;
    int64_t col = 0;
    py::str kind;
    py::str text;
    py::list<py::str> notes;                                          // ("notes": what the complaint expected, after it)
    std::optional<std::tuple<py::str, int64_t, int64_t>> macro_at;    // ("macro_at": where it is in the source)
};
py::list<Diagnostic> diagnostics(const Compiler& spec, const py::str& text, const py::str& folder);

// (the C files, the assembler modules, the assembler modules' paths) of a source tree
std::tuple<py::dict<py::str, py::str>, py::dict<py::str, py::str>, py::dict<py::str, py::str>> read_tree(const py::str& src);

// the bytes this process has committed (Windows: its private bytes; elsewhere: its resident set) - or None
std::optional<int64_t> memory_used();

// a watch on the port's own memory: above the ceiling (settings.json port.max_memory_gb, 4 by default) it
// stops - where it was written to OUT/port-memory.txt - before it can starve the machine (a rule that runs away)
void memory_guard(const py::str& out);

// what one file's check depends on: the compiler and its command, the file, every header it reaches
py::str check_key(const Compiler& spec, const py::str& cc, const py::str& n, const py::dict<py::str, py::str>& res);

// the port built a C file at a time, side by side (the workbench's workers, at most four), each file's object
// kept by all it depends on (the compiler and its command, the file, every header it reaches): a file as it was
// the last time not compiled again; then the objects linked. A compiler of another form (MSVC's): all at once
Process build(const Compiler& spec, const py::str& cc, const py::list<py::str>& tpl, const py::str& out,
              const py::list<py::str>& srcs, const py::str& exe, const py::str& objdir);

// an object file whole: not empty, its first bytes not nothing (a build cut short leaves either)
bool sound(const py::str& path);

// the C files to check again: those whose text is not what it was, and those that include (themselves or
// through another header) a header whose text is not
py::list<py::str> changed_files(const py::dict<py::str, py::str>& res, const py::dict<py::str, py::str>& before,
                                const py::list<py::str>& cfiles);

// {C name: its place in the data segment} of a SYMBOLS list (the recon's: name, C or D, address)
py::dict<py::str, int64_t> read_symbols(const py::str& src);

// {C name: its bytes} of the variables a BSS list names (the recon's: what the old build defined beginning
// without a value, by DS offset - 'range from to', 'hole at length', 'name offset')
py::dict<py::str, int64_t> read_bss(const py::str& src);

// ---------------------------------------------------------------- the old runtime (runtimes/NAME.json)
struct RuntimeProfile {
    py::str name;
    py::list<py::str> libc;
    py::list<py::str> runtime_defines;
    py::list<py::str> platform_calls;                // platform["calls"]
    std::optional<bool> text_mode;                   // profile.get("text_mode")
    std::optional<bool> console_text;                // profile.get("console_text")
    std::optional<bool> exit_status;                 // profile.get("exit_status")
    py::dict<py::str, py::str> notes;                // "notes" (in the file's order)
    int64_t width_int = 16;                          // widths["int"]
    bool char_signed = false;                        // profile.get("char_signed")
    bool c_underscore = true;                        // profile.get("c_underscore", True)
    static RuntimeProfile load(const py::str& path);
};

// what an assembler module of the program is: the old runtime's (it makes public what the runtime defines,
// or every name it makes public is the C library's or the platform layer's), data alone (no instruction in a
// code segment), or code of the program - to be ported as assembly language
// -> (role "runtime" | "data" | "code", its public names, sorted)
std::tuple<py::str, py::list<py::str>> asm_role(const py::str& text, const RuntimeProfile& profile);

py::str strip_asm_comment(const py::str& l);

extern const py::dict<py::str, py::str> NEW_TYPES;

// {name: the type it returns, today's spelling} from the C program's declarations of functions it does not
// define (extern T NEAR f();) - what it expects of the assembly's
py::dict<py::str, py::str> c_declared(const py::list<py::str>& texts);

extern const py::dict<py::str, int64_t> NEW_SIZES;

// [(old address in the data segment, name)] of the program's variables, where the recovered source has them
// (a SYMBOLS file beside it: name, D, address - as the original image placed them)
py::list<std::tuple<int64_t, py::str>> data_places(const py::str& asm_path);

// {alias: (owner, byte offset now)} of the names the old linker gave places inside other variables;
// `aliases` is {alias: (where, the owner's definition)} (an owner's definition the program does not have: "")
py::dict<py::str, std::tuple<py::str, int64_t>> alias_places(const py::dict<py::str, std::tuple<py::str, py::str>>& aliases,
                                                             const asm16::CTypes& ct);

// [(old size, what it points to or None)] of the arguments the C program calls an assembly routine with: a
// pointer to a variable it declares (&g), a pointer variable (p) - far ones 4 bytes, as they were pushed
py::list<std::tuple<int64_t, std::optional<py::str>>> call_arg_types(const py::str& fn, const py::list<py::str>& texts,
                                                                     const py::list<py::str>& origs, const asm16::CTypes& ct);

// {routine: its lines} - the old runtime's routines that are straight lines to a RET (cret: the shared end of
// the routines of Lattice-era C - LEA SP,[BP-4], the POPs, RET): what a jump into one does, there
py::dict<py::str, py::list<py::str>> runtime_tails(const py::dict<py::str, py::str>& mods);

// JMP to one of the old runtime's straight routines: its instructions in place of the jump
py::str inline_tails(const py::str& text, const py::dict<py::str, py::list<py::str>>& tails, py::list<py::str>& notes,
                     const py::str& name);

// roles: {module: (role, public names)}; opt: the command line's option (None when absent)
using Roles = py::dict<py::str, std::tuple<py::str, py::list<py::str>>>;
using Opt = std::function<std::optional<py::str>(const py::str&)>;

// the program's own assembly language, ported as assembly language (port/asm16) and assembled; its data
// alone as C definitions. -> ([objects to link], {name: its C declaration}, [notes])
std::tuple<py::list<py::str>, py::dict<py::str, py::str>, py::list<py::str>>
asm_modules(pre_ansi::Port& port, const py::str& out, const py::dict<py::str, py::str>& res, const py::dict<py::str, py::str>& asm_,
            const py::dict<py::str, py::str>& paths, const Roles& roles, const RuntimeProfile& profile, const Opt& opt,
            const Compilers& compilers);

// the variables of an assembler module of data alone (placed by the old linker): C definitions, with the
// C program's types where it declares them. The Python leaves three more results on the function (one_place,
// aliases, sizes): they are the other fields.
struct DataDefs {
    py::list<py::str> defs;
    py::dict<py::str, py::str> one_place;           // (scalars of one place under two names: one variable)
    py::dict<py::str, py::str> aliases;             // (names of one place: {alias: where})
    py::dict<py::str, int64_t> sizes;               // ({name: bytes of its own})
};
DataDefs asm_data_defs(const py::str& text, const asm16::CTypes& ct);

// the records the program takes as bytes kept in their old layout, their pointers two-byte handles
// (port/oldlayout.py); -> how many lines it changed. `wanted`: the Python's port._ol_wanted, {(file, line): line}
int64_t oldlayout_pass(pre_ansi::Port& port, const py::str& out, const py::dict<py::str, py::str>& res, const Compilers& compilers,
                       py::dict<std::tuple<py::str, py::str>, py::str>* wanted = nullptr);

// arith16 over the port's C (clang reads it): each line it changed kept as a rule; -> how many casts
int64_t arith16_pass(pre_ansi::Port& port, const py::str& out, const py::dict<py::str, py::str>& res, const Compilers& compilers);

// the port built with AddressSanitizer and run on the cases until it reports nothing; -> the rounds' notes
py::list<py::str> sanitize_loop(pre_ansi::Port& port, const py::str& out, const std::function<py::dict<py::str, py::str>()>& write,
                                const py::list<py::str>& srcs, const Compilers& compilers, const py::str& comp, const py::str& cases,
                                int64_t rounds = 6);

// everything a sanitized build and its runs depend on: the port's files, the cases and their inputs, the
// other passes' ports the cases run, the compiler
py::str sanitize_key(const py::str& out, const py::list<py::str>& srcs, const py::str& cases, const py::str& cc);

void report(const py::str& out, const py::str& src, const py::str& comp, const std::optional<py::str>& cc, const pre_ansi::Port& port,
            const Roles& roles, const py::list<Diagnostic>& diags, int64_t rounds, bool built, const py::str& blog,
            const RuntimeProfile& profile, const py::list<py::str>& sanitized = py::list<py::str>(),
            const py::list<py::str>& asm_notes = py::list<py::str>());

// the help text (the module's docstring)
extern const py::str DOC;

// the program's main: -> the exit code, what it prints goes to `out`
int64_t main(const py::list<py::str>& argv, std::ostream& out);

}  // namespace codebreaker::port::__main__
