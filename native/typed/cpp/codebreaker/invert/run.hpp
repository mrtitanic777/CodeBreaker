// codebreaker/invert/run.hpp -- C++17 port of codebreaker.invert.run.
//
// run.py -- read a module's functions back into the source its compiler was given.
//
//     python -m codebreaker invert <16-bit module> [--first N] [--flags "/AM /G2 /Gsw /Ox"] [--show NAME]
//     python -m codebreaker invert --early FILE OFFSET LENGTH     one function of a program built with the
//                                                                 C compiler of 1983-84 (offset in hex)
//     python -m codebreaker invert --check [MODEL]                each model against its own compiler
//     python -m codebreaker invert --tool NAME [arguments]        a model's own command line
//
// The decompiler gives source that BEHAVES like a function. A compiler model gives the source
// that was WRITTEN, for the functions it covers: it knows which instructions the compiler emits
// for which statement, so it reads the statements off the instructions instead of searching for
// them (verify/authored/repair.py searches). What it says is then put to the compiler itself: the text is
// compiled by the real Microsoft C 6 and its bytes compared with the original's, outside the
// places a linker fills in.
//
//     recognised    the model read the whole function
//     identical     ... and the real compiler turns that text back into the function's bytes
//     declined      the function has something the model does not cover; it says nothing
//
//     assembly      a segment-switch thunk: generated code, written as the assembly that makes it
//                   (invert/segment_thunk.py rebuilds its bytes exactly)
//
// A model never guesses: a function it does not cover is declined, with the reason. Each function
// is compiled by the compiler whose frame it has (compiler_of): Microsoft C 6, or Microsoft C/C++ 8
// (settings: msvc152) for the modules Visual C++ 1.52c built.
//
//     auto r = run::module(path);      {name: (verdict, text or reason, the flags that gave the bytes)}
//
// Typing notes:
//   * module() answers {function name: (verdict, C text or the reason it was declined, flags that gave the bytes or None)}.
//     `log` is the std::function the Python passes `print` as (an empty one is None).
//   * signature() answers (returns, [(name, kind)]) or None.
//   * tool() runs a model's own command line through base::commands::run_module: the modules a program can run are
//     registered with commands::set_module_runner (C++ has no import by name).
//   * msc6_calls (the parameters of a module's internal functions, from how they are called) has no typed port: _segment_signatures
//     calls its model's entry point (cbinv::msc6_segment_sigs) directly, as the module's Python reached it through native_invert.
//   * main takes sys.argv[1:] (the arguments after the program name) and returns the exit status.
#pragma once

#include <functional>
#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::invert::run {

// the module's docstring (printed by main without arguments, with the list of tools filled in)
extern const py::str DOC;

extern const py::list<py::str> FLAGS;
// Microsoft C/C++ 8 (Visual C++ 1.52c) built some of the 16-bit modules; it ships much unoptimised code
extern const py::list<py::str> FLAGS_MSVC152;

// Which compiler made a function, by its frame: Microsoft C/C++ 8 enters with mov ax,ds
// (8c d8) or the near-loadds frame and leaves through lea sp,[bp-2] (8d 66 fe 1f 5d); Microsoft
// C 6 does neither.
py::str compiler_of(const py::bytes& code);

// the models, and what each one's own command line does
extern const py::dict<py::str, py::str> TOOLS;
extern const py::list<py::str> CHECKS;

// Run one model's own command line.
int64_t tool(const py::str& name, const py::list<py::str>& arguments);

extern const py::set<py::str> _LONG;

// The model's word for an SDK type: a far pointer, a long, or an int.
py::str _kind(const py::str& c_type);

// (returns, [(name, kind)]) from the SDK headers for an exported function, or None.
std::optional<std::tuple<py::str, py::list<std::tuple<py::str, py::str>>>> signature(const py::str& name);

// {(segment, offset): [(name, kind)]} for the framed functions of each code segment, from how they are called (msc6_calls.py).
py::dict<std::tuple<int64_t, int64_t>, py::list<std::tuple<py::str, py::str>>> _segment_signatures(const py::str& module_path);

// {offset: (name, "long" | "word")} for the variables a function names by address: a long
// shows as two words two apart used as one, anything else it stores to or pushes is a word.
// The names say what they are -- an address -- and nothing more.
py::dict<int64_t, std::tuple<py::str, py::str>> _globals(const py::bytes& code);

using Result = std::tuple<py::str, py::str, std::optional<py::str>>;
using Log = std::function<void(const py::str&)>;

// -> {function name: (verdict, C text or the reason it was declined, flags that gave the bytes)}
py::dict<py::str, Result> module(const py::str& module_path, int64_t limit = 0, const std::optional<py::str>& flags = std::nullopt,
                                 const Log& log = nullptr);

int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::invert::run
