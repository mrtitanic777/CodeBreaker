// codebreaker/source/vxd/run.hpp -- C++ from codebreaker.source.vxd.run.
//
// The module's own description (its docstring, DOC below) is the command's help text.
#pragma once

#include <functional>

#include "py.hpp"

namespace codebreaker::source::vxd::run {

// The module's docstring: shown when the tool name is missing or unknown.
extern const py::str DOC;

// The tool table: tool name -> the module that runs it.
extern const py::dict<py::str, py::str> TOOLS;

// What codebreaker.base.commands.run_module does -- "run `module` as `python -m module arguments...`" -- is a
// Python facility (runpy), and codebreaker.base.commands has no typed port yet; so main takes it as a parameter.
// Returns the module's exit code.
using RunModule = std::function<int64_t(const py::str& module, const py::list<py::str>& arguments)>;

// commands.dispatch(TOOLS, argv, DOC): argv[0] names one of TOOLS, the rest are its arguments.
// With no tool or an unknown one: prints DOC and the sorted tool names to stdout, returns 2.
int64_t main(const py::list<py::str>& argv, const RunModule& run_module);

}  // namespace codebreaker::source::vxd::run
