// codebreaker/base/commands.hpp -- C++17 port of codebreaker.base.commands.
//
// commands.py -- run a module's own command line from inside a command.
//
// Some packages are a set of tools, each with its own command line (the compiler models, the
// driver-kit source chain). A command names them in a table and hands the rest of its arguments
// to the one asked for:
//
//     commands::dispatch(TOOLS, argv, doc);      // argv = [tool name, its arguments...]
//
// Python's run_module uses runpy to start a module by name as `python -m module`. C++ has no
// import-by-name, so the modules a program can run are registered with set_module_runner(): one
// function that, given a module name, runs it as that module's main, reading its command line
// from sys_argv (what sys.argv is in Python) and leaving through SystemExit like sys.exit().
#pragma once

#include <functional>
#include <variant>

#include "py.hpp"

namespace codebreaker::base::commands {

// ImportError: a module runner throws it ("No module named X") for a name it does not have,
// as runpy does
struct ImportError : py::Exception {
    ImportError() : py::Exception() {}
    explicit ImportError(const std::string& m) : py::Exception(m) {}
    const char* type() const override { return "ImportError"; }
};

// SystemExit(code): code is None, an int, or a str (the one place the Python mixes types: it is
// whatever sys.exit() was given, so a std::variant)
struct SystemExit : py::Exception {
    std::variant<std::monostate, int64_t, py::str> code;
    SystemExit() : py::Exception() {}
    explicit SystemExit(int64_t c) : py::Exception(), code(c) {}
    explicit SystemExit(const py::str& c) : py::Exception(c.utf8()), code(c) {}
    const char* type() const override { return "SystemExit"; }
};

// sys.argv, for the running module to read
extern py::list<py::str> sys_argv;

// Runs `module` as its own main (python -m module): reads sys_argv, throws SystemExit to exit.
using ModuleRunner = std::function<void(const py::str& module)>;
void set_module_runner(ModuleRunner runner);

// Run `module` as if started with `python -m module arguments...`. -> its exit code.
int64_t run_module(const py::str& module, const py::list<py::str>& arguments);

// argv[0] names one of `tools` {name: module}; the rest are its arguments.
int64_t dispatch(const py::dict<py::str, py::str>& tools, const py::list<py::str>& argv,
                 const py::str& doc = py::str(""));

}  // namespace codebreaker::base::commands
