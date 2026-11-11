// codebreaker/translate/frontends/java/package.hpp -- typed C++ port of codebreaker/translate/frontends/java/__init__.py.
//
// The Java front end, step one: javac's typed tree as JSON.
//
// TreeDump.java (beside the Python module) runs javac over a source tree -- parse and attribute, no class files -- and writes
// every node of every compilation unit with what javac resolved: each expression's type, the declaration each name, call and
// `new` refers to (with the overload chosen). `dump` compiles TreeDump once (cached under the state folder, keyed by its source
// and the JDK) and runs it.
//
//     auto units = java::dump(src_dir, out_dir, classpath);       // {source path relative to src: tree}
//     java::diagnostics(out_dir);                                 // javac's errors and warnings
//
// A node is a dict: "k" its kind (javac's Tree.Kind name: METHOD_INVOCATION, IF, PLUS, ...), "l"/"c" its line and column,
// "$type" the resolved type of an expression or declaration, "$sym" the element a name refers to ({"kind", "name", "owner",
// "static", "ret", "params", ...}), and its children under javac's property names ("methodSelect", "arguments", "condition",
// "thenStatement", ...).
//
// Typing notes:
//   * A tree is a node Dict (ir::Dict), read with frontends::java::lower::load_tree; `diagnostics` is the JSON list in
//     `_diagnostics.json` (a Val list of dicts: kind, file, line, message ...).
//   * The Java source TreeDump.java is read from the Python package's folder of the checkout (`_HERE()`; set_here to change):
//     it is both what is compiled and part of the cache key, which is the Python's (sha1 of the source and repr(cache.tool(javac)),
//     first 12 digits), so the two share the compiled tool folders.
//   * `_jdk()` is verify::lanes::java::jdk(); a JDK that is not found is RuntimeError, as the Python's.
//   * javac and java run with run_process (translate/dotnet/package.hpp: subprocess.run with a timeout and bytes); a timeout is
//     TimeoutExpired.
//   * `files=None` is std::optional<list<str>>; `release=None` std::optional<int64_t> (any value str() takes: an int here).
#pragma once

#include <optional>
#include <tuple>

#include "codebreaker/translate/ir.hpp"
#include "py.hpp"

namespace codebreaker::translate::frontends::java {

namespace ir = codebreaker::translate::ir;

extern const int64_t TIMEOUT;

// the folder of the Python package (where TreeDump.java is)
py::str _HERE();
void set_here(const py::str& here);
py::str _SOURCE();   // _HERE/TreeDump.java

// (javac, java) of the JDK; RuntimeError when there is none.
std::tuple<py::str, py::str> _jdk();

// The folder holding the compiled TreeDump, compiling it when its source or the JDK changed. `javac` None: the JDK's.
py::str tool(const std::optional<py::str>& javac = std::nullopt);

// Every .java file under src_dir, sorted.
py::list<py::str> sources(const py::str& src_dir);

// Dump src_dir's compilation units (or just `files`) into out_dir. -> how many were dumped.
int64_t run(const py::str& src_dir, const py::str& out_dir, const py::list<py::str>& classpath = py::list<py::str>(),
            const std::optional<py::list<py::str>>& files = std::nullopt, const std::optional<int64_t>& release = std::nullopt);

// {path relative to out_dir without .json: tree} for every dumped unit.
py::dict<py::str, ir::Dict> load(const py::str& out_dir);

ir::Val::List diagnostics(const py::str& out_dir);

// run + load.
py::dict<py::str, ir::Dict> dump(const py::str& src_dir, const py::str& out_dir, const py::list<py::str>& classpath = py::list<py::str>(),
                                 const std::optional<py::list<py::str>>& files = std::nullopt, const std::optional<int64_t>& release = std::nullopt);

}  // namespace codebreaker::translate::frontends::java
