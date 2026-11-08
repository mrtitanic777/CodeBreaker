// codebreaker/formats/xbox/crypto/xkelib_exports.hpp -- typed C++ of codebreaker.formats.xbox.crypto.xkelib_exports
//
// A script to create .def files from Xbox 360 libraries using dumpbin in the SDK.
#pragma once

#include <cstdint>
#include <optional>

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::formats::xbox::crypto::xkelib_exports {

// The Python's SystemExit (argparse's exit after an error or --help): the exit code.
struct SystemExit : py::Exception {
    int64_t code = 0;
    explicit SystemExit(int64_t c) : py::Exception(), code(c) {}
    const char* type() const override { return "SystemExit"; }
};

extern const py::str SDK_VER;

// SDK_DIR is computed from settings.get("xbox360_sdk") when first used (the Python computes it when the
// module is imported); it is a variable the callers may assign, as they may the Python module's.
py::str& SDK_DIR();

extern const py::re::Pattern HEADER_RE;
extern const py::re::Pattern EXPORT_RE;

// Runs the SDK's dumpbin.exe /<COMMAND> on the file, prints "Error!" when it fails, and returns what it
// wrote to stdout (decoded as UTF-8).
py::str dumpbin(const py::str& command, const py::str& filename);

// The library name the "DLL name :" line of dumpbin /headers gives, or None.
std::optional<py::str> fetch_version(const py::str& s);

// Writes the .def file of the exports of ifile to ofile.
void create_def(const py::str& ifile, const py::str& ofile, bool verbose = false);

// The command line: `args` are sys.argv[1:], `prog` is the basename of sys.argv[0]. Raises SystemExit as
// argparse does (usage errors: code 2, --help: code 0).
void main(const py::list<py::str>& args, const py::str& prog = py::str("xkelib_exports.py"));

}  // namespace codebreaker::formats::xbox::crypto::xkelib_exports
