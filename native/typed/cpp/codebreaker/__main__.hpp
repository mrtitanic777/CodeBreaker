// codebreaker/__main__.hpp -- typed C++ port of codebreaker.__main__.
//
// __main__.py -- `python -m codebreaker <command> [arguments]`: the command table and the dispatch.
//
//     main(argv)       the command line: `--corpus NAME` taken off, the command found in COMMANDS, its module's `main`
//                      run with the command's fixed arguments and the rest; no command (or a help word, or a name
//                      COMMANDS does not list) prints the docstring.
//
// Every command is listed here, by name. Nothing is discovered by scanning folders.
//
// Notes on the port:
//   * The docstring (the usage text) and COMMANDS are the Python's, word for word (generated from it into
//     __main__.cpp): `DOC()` and `COMMAND_TABLE()` (Python's dict order). A COMMANDS value (module, fixed arguments) is
//     `Command`.
//   * importlib.import_module(module).main(...) is a registry: the module's typed `main` is registered under its dotted
//     name (`register_command("codebreaker.ui.mcp_server", fn)`) where the program that runs the commands links it.
//     This file registers ui.mcp_server's only (the one it shares a cycle with). A command whose module has no
//     handler registered raises ModuleNotFoundError, as an import that fails does; the commands, their modules and
//     whether a typed `main` exists for them are listed in the port's report. The commands run by
//     "codebreaker_native" (C++ already: native/cbexe/commands.cc) are registered under that name, the fixed
//     arguments (the command's name) first.
//   * `sys.stdin.isatty() and sys.stdout.isatty() and importlib.util.find_spec("textual")` is `terminal_available()`:
//     the two isatty tests are the C runtime's, and "is the terminal interface installed" is the hook
//     `has_terminal_interface` (a Python package; unset: false).
#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "py.hpp"

namespace codebreaker::main_module {

// A COMMANDS value: the module that runs it and the arguments put before the command's own.
struct Command {
    py::str module;
    py::list<py::str> fixed;
};

// The module's docstring (the usage text).
const std::string& DOC_TEXT();
inline py::str DOC() { return py::str(DOC_TEXT()); }

// COMMANDS, in the Python dict's order.
const std::vector<std::pair<py::str, Command>>& COMMAND_TABLE();
// COMMANDS[name], or nothing.
const Command* command(const py::str& name);
// the command names, in order
py::list<py::str> command_names();

// importlib.import_module(module).main: a handler registered under the module's dotted name.
using Handler = std::function<int64_t(const py::list<py::str>& argv)>;
void register_command(const py::str& module, Handler handler);
// The module's handler, or null.
const Handler* handler(const py::str& module);

// find_spec("textual") (the terminal interface's library): set by the program that has it.
std::function<bool()>& has_terminal_interface();
// stdin and stdout are both terminals
bool terminal_available();

// The command line: -> the exit status.
int64_t main(const py::list<py::str>& argv);

// The `codebreaker` command an install provides (sys.exit(main(sys.argv[1:])) in the Python): the status.
int64_t _entry(const py::list<py::str>& argv);

}  // namespace codebreaker::main_module
