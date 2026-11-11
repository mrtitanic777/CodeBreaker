// codebreaker/java/run.hpp -- typed C++ from codebreaker.java.run.
//
// run.py -- the Java transpiler's tools: a Java program as C++03, and what its console port needs.
//
//     python -m codebreaker java <tool> [arguments]      (`java <tool> --help` for a tool's options)
//
// (the tools are listed in the help text, MODULE_DOC of run_parser.inc, printed when no tool is named)
//
// Typing and runtime notes:
//   * main(argv) takes sys.argv[1:] and returns the exit code; what the Python prints goes through py::print (stdout), what
//     argparse prints (usage, errors) to stderr, and argparse's SystemExit (a usage error: 2, -h: 0) is py::SystemExit.
//   * The runtime's py_argparse does not do subparsers, nargs='?', choices, float values or append, so the command line is
//     read by the argparse algorithm itself (the Python's _parse_known_args: the O / A pattern of the arguments,
//     consume_optional / consume_positionals with the nargs regular expressions, abbreviations, `--opt=value`, `--`,
//     the "required" and "unrecognized" errors, argparse's error texts) written here for the parsers run.py builds; those
//     are data in run_parser.inc (every subcommand's options and positionals, and the usage and help texts argparse
//     formats for them), generated from the Python with COLUMNS=80 and no colour.
//   * `args.xxx` is a Namespace of str / float / bool / list values read by name; `set_defaults(func=...)` is the table
//     in _dispatch.
//   * The commands are the typed ports of what each calls (java.analyze, report, ledger, signatures, scaffold, apimap,
//     debug, bugfix_ledger, reflection_factory, transpile, console.*, projects.minecraft.*, decompile.jar); the active
//     target profile (java.profiles) is the registry that transpile.hpp carries.
//   * `settings.state(*parts)` is the state folder joined (and its folder made), written here.
//   * `build`'s `subprocess.call` is py::subprocess::run with the child's streams inherited.
#pragma once

#include <cstdint>

#include "py.hpp"

namespace codebreaker::java::run {

// The command line without the program name (sys.argv[1:]). -> the exit code
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::java::run
