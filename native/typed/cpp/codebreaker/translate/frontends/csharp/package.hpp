// codebreaker/translate/frontends/csharp/package.hpp -- typed C++ port of codebreaker/translate/frontends/csharp/__init__.py.
//
// The C# front end, step one: Roslyn's bound tree as JSON.
//
// CsTreeDump.cs (beside the Python module) runs the C# compiler of the installed .NET SDK over a set of files -- parse and bind,
// no assembly written -- and writes each file's declarations, and every body and initialiser as Roslyn's IOperation tree, with
// what the compiler resolved: each expression's type, the symbol each name, call and `new` refers to (the overload chosen, its
// arguments in parameter order), each implicit conversion. `tool` compiles CsTreeDump once with the SDK's own csc.dll against
// the SDK's Microsoft.CodeAnalysis assemblies, by path -- no project, no NuGet restore, nothing downloaded -- cached under the
// state folder, keyed by its source and the SDK.
//
//     auto units = csharp::dump({"Prog.cs"}, out_dir);     // {file name without .cs: tree}
//     csharp::diagnostics(out_dir);                        // the compiler's errors and warnings
//     csharp::units({"Prog.cs"});                          // dump + lower: [ir.Unit]
//
// Typing notes:
//   * A tree is a node Dict (ir::Dict), read with frontends::csharp::lower::load_tree; `diagnostics` is the JSON list of
//     `_diagnostics.json` (a Val list of dicts: kind, file, line, message ...); errors() those of kind ERROR.
//   * The C# source CsTreeDump.cs is read from the Python package's folder of the checkout (`_HERE()`; set_here to change it);
//     the cache key of the compiled tool is the Python's (sha1 of the source, repr of the Roslyn files' cache.tool and of
//     dotnet.runtime()), so the two share the tool folders.
//   * `langversion` (any value `str()` takes in the Python) is optional text.
//   * The dotnet module is translate/dotnet/package.hpp (it also holds the process runner used here).
#pragma once

#include <optional>

#include "codebreaker/translate/ir.hpp"
#include "py.hpp"

namespace codebreaker::translate::frontends::csharp {

namespace ir = codebreaker::translate::ir;

extern const py::list<py::str> _ROSLYN;
extern const int64_t TIMEOUT;

py::str _HERE();
void set_here(const py::str& here);
py::str _SOURCE();   // _HERE/CsTreeDump.cs

// RuntimeError when the .NET SDK (dotnet.exe with Roslyn and the targeting pack) is missing.
void _dotnet();
py::str _tmp();   // settings.state('translate', 'tmp')

// CsTreeDump.dll, compiled when its source or the SDK changed.
py::str tool();

// Dump the C# files into out_dir. -> how many were dumped.
int64_t run(const py::list<py::str>& files, const py::str& out_dir, const std::optional<py::str>& langversion = std::nullopt);

// {file name without .cs: tree} for every dumped file.
py::dict<py::str, ir::Dict> load(const py::str& out_dir);

ir::Val::List diagnostics(const py::str& out_dir);
ir::Val::List errors(const py::str& out_dir);

// run + load.
py::dict<py::str, ir::Dict> dump(const py::list<py::str>& files, const py::str& out_dir, const std::optional<py::str>& langversion = std::nullopt);

// Every .cs file under a folder (or the one file), sorted.
py::list<py::str> sources(const py::str& src);

// Dump and lower C# files. -> [ir.Unit] (raises on a compile error: the front end needs bound code). `out_dir` None: a
// temporary folder, removed after.
py::list<py::ref<ir::Unit>> units(const py::list<py::str>& files, const std::optional<py::str>& out_dir = std::nullopt);

}  // namespace codebreaker::translate::frontends::csharp
