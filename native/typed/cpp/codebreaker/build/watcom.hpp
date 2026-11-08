// codebreaker/build/watcom.hpp -- typed C++ of codebreaker.build.watcom
//
// watcom.py -- compiling and linking reconstructed C with OpenWatcom.
//
// The pieces a whole-module build needs: where the tools are and the environment they run in,
// the flags a reconstruction is compiled with, how a module's imports are declared and linked
// so calls reach the same DLL functions the original calls, and a check that catches calls
// whose argument count disagrees with the callee's definition before the compiler does.
//
// Typing notes:
//   * The Python's `from codebreaker.build import watcom as toolchain` (a self-import) is a call within this
//     module here.  formats.omf and formats.pe are the typed ports (omf.hpp, pe.hpp).
//   * `_ow()`'s dict is the struct `OW`.  `_scratch(*parts)` takes the parts as a list.  `_first(*cands)` takes
//     the candidates as a list (an empty str is a candidate that is not there, as Python's None or "" is).
//   * The module's mutable tables (`_IMPORT_MAPS`, `_EXPORT_NAMES`) are functions returning the one dict each,
//     which the caller may change as the Python caller may.
//   * `_import_lines`' `names` is any iterable of str (callers pass a set): a py::set<py::str>.  `numbered` is
//     {name: (DLL, number, bytes of arguments removed or None)}, as describe.BY_NUMBER holds it.
//   * ow_env's environment dict is a py::dict<py::str, py::str> built from the process environment
//     (`dict(os.environ)`: on Windows the names upper-cased, in the order the system gives them).
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::build::watcom {

// {"root", "wcc", "wlink", "inc"} of _ow()
struct OW {
    py::str root, wcc, wlink;
    py::list<py::str> inc;
};

// {name: (DLL, number, bytes of arguments removed or None)}
using Numbered = py::dict<py::str, std::tuple<py::str, int64_t, std::optional<int64_t>>>;

OW _ow();

// Build scratch lives in the temp dir, not beside the source (it grew to many thousands of
// files under src/targets/win95 and slowed every search of the repo).
py::str _scratch(const py::list<py::str>& parts);

// The compiler emits calls to its own runtime helpers for some constructs: __TryInit/__TryUnwind/
// __Except for __try/__except, and 64-bit arithmetic and floating-point helpers. They live in the
// C runtime library; without it every body using such a construct failed to link. Local stubs
// for a body's callees are defined in the object and still take precedence.
inline const py::str _RTL = py::str("library clib3r.lib\n");

// Where a rebuilt DLL is based. The linker's default (0x400000) is exactly where ordinary computed
// values fall -- the sum of two of the oracle's scratch pointers, say -- and a value inside the
// rebuilt image is taken for "an address of this build's own data", which the original cannot
// have: a correct body was then reported as passing a different argument. Up here nothing collides.
inline constexpr int64_t IMAGE_BASE = 0x6E000000;

extern const py::str _BASE;       // "option offset=0x6E000000\n"

py::dict<py::str, py::str> _ow_env(const OW& ow);

// ---------------------------------------------------------------------------------------------
// Real imports. A rebuilt body must call the retail module's imported functions THROUGH AN
// IMPORT, not through a local stand-in. The oracle records an external call when execution
// lands on an import; a local `int HeapAlloc(){return 0;}` is invisible to it. With stand-ins
// the rebuilt side never called anything, so any body whose retail path reached an API could
// not match -- most of the code that matters. (The 16-bit lane had the same flaw.)
// ---------------------------------------------------------------------------------------------
py::dict<py::str, py::dict<py::str, py::str>>& _IMPORT_MAPS();

// C name of an import -> the name the DLL exports it by, where they differ
py::dict<py::str, py::str>& _EXPORT_NAMES();

// {name: dll} for the functions a retail PE imports by name (the first DLL wins). An export
// with a stdcall decoration (`_AtqReadFile@16`, INFOCOMM's) or a C++ one (`?QueryStrA@STR@@QBEPADXZ`)
// is under its C spelling, the one the decompiler writes (`_AtqReadFile_16`,
// `_QueryStrA_STR__QBEPADXZ`), and links to the decorated name (_EXPORT_NAMES): left out, the
// call was linked to nothing and went to the next instruction.
py::dict<py::str, py::str> import_map(const py::str& module);

// wlink `import` directives binding each imported name's decorated symbol to DLL.name --
// or to DLL.number for one the original imports by number (`numbered`).
py::str _import_lines(const py::bytes& obj, const py::set<py::str>& names, const py::dict<py::str, py::str>& imports,
                      const std::optional<Numbered>& numbered = std::nullopt);

// The first candidate that is a path that exists, else "".
py::str _first(const py::list<py::str>& cands);

// ===============================================================================
// The RECONSTRUCTION pipeline's own tools: the native decompilers and OpenWatcom.
// (The registry above is the period Microsoft toolchain used for byte-exact work.)
// Every consumer used to rebuild these paths by hand -- the decompiler bin dir in six
// files, the OpenWatcom root in thirteen -- so pinning a different build meant editing
// each one. They resolve here now; nothing else should spell these paths.
// ===============================================================================

// Root of the from-source OpenWatcom release tree (CB_OPENWATCOM overrides).
py::str openwatcom_root();

// Absolute path of an OpenWatcom tool ('wcc', 'wcc386', 'wlink', 'wasm', 'wrc').
// Absolute because Windows CreateProcess searches the CALLER's PATH, not the env passed in.
// Returns "" when OpenWatcom is not built.
py::str ow_tool(const py::str& name);

// The C compiler for a module class: 'ne' (16-bit Windows) -> wcc, else wcc386.
py::str ow_cc(const py::str& cls);

// Header search dirs for a module class: Win16 uses h/win, Win32 uses h/nt.
py::list<py::str> ow_include(const py::str& cls);

// Runtime library dir for a module class (the Win16 one holds libentry.obj + clib*.lib).
py::str ow_lib(const py::str& cls);

// Process environment for running OpenWatcom tools on a module class.
py::dict<py::str, py::str> ow_env(const py::str& cls);

}  // namespace codebreaker::build::watcom
