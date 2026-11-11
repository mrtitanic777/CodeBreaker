// codebreaker/formats/xbox/crypto/package.hpp -- typed C++ port of codebreaker/formats/xbox/crypto/__init__.py.
//
// crypto -- Xbox 360 console crypto: XeCrypt, keyvaults, the boot chain, NAND images, shadowboots.
//
// Most of this package is Xbox-360-Crypto by GoobyCorp (BSD 3-Clause; see LICENSE and ATTRIBUTION.md), kept as its
// authors wrote it. Each tool runs as
//
//     python -m codebreaker crypto <tool> [arguments]      (python -m codebreaker crypto: the list)
//
// and takes its own --help. What the tools read that is not part of the package comes from settings:
// `xbox360_keys` (the key store folder: 1BL_pub.bin, SB_prv.bin, ...; default Keys/ in the current folder) and
// `xbox360_crypto_bin` (upstream's bin/ folder: the LZX libraries, the Free60 xenon-as and xenon-objcopy, hypervisor
// images). Neither is in the repository.
//
// Typing notes:
//   * `tools()` lists the Python package's own folder (the modules that define `main`): `_HERE()` is that folder of the
//     checkout (codebreaker/formats/xbox/crypto: `set_here` to change it), which a C++ port cannot take from its own
//     file name. The names are the Python modules', so the list is the same whatever has been ported.
//   * The Python imports the tool module by name and calls its main(), which reads sys.argv. A typed port cannot import by
//     name, and each ported tool's main has its own signature (some take the argument list, some none, some return an
//     exit code): a tool is a `Tool`, a function of sys.argv (argv[0] the tool's name, as main() sets it) answering
//     what main() returned as an int (nullopt for None / anything that is not an int). `register_tool(name, tool)` adds
//     one; a tool that is not registered raises ImportError "No module named ..." as an absent module does. The
//     registry starts empty: wiring each ported tool's main to its name is the caller's (see the report).
//   * SystemExit raised by a tool is caught and its code is the result (`result = e.code`); a code that is not an int
//     (None, a message) gives 0, as `isinstance(result, int)` does.
//   * main(argv) prints with py::print (stdout), as the Python's print does, and returns the exit code.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>

#include "py.hpp"

namespace codebreaker::formats::xbox::crypto {

// the package docstring, as `codebreaker.formats.xbox.crypto.__doc__` has it
extern const py::str DOC;

// the folder of the Python package (where the tool modules are listed from)
py::str _HERE();
void set_here(const py::str& here);

// Names of the modules here that run as tools (they define main).
py::list<py::str> tools();

// A tool's main(): sys.argv in, what it returned out (an int, else nullopt).
using Tool = std::function<std::optional<int64_t>(const py::list<py::str>& argv)>;
void register_tool(const py::str& name, const Tool& tool);

int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::formats::xbox::crypto
