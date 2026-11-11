// codebreaker/symbols/xdk_libraries.hpp -- typed C++ port of codebreaker.symbols.xdk_libraries.
//
// xdk_libraries.py -- naming the XDK library functions an Xbox 360 program was statically linked with.
//
//     codebreaker xdk-match <program.xex or image> [<XDK lib folder>] [-o names.json]
//     auto found = xdk_libraries::match(image_path, lib_folder);    // {address: {"names", "libs", "size"}}
//
// Every COFF archive (.lib) in the XDK's library folder is read (symbols/libraries), each function's machine
// code taken from its object with the bits the linker patches (the fields its relocations name) masked out,
// and the program's code searched for the same words. A program linked against the same XDK and compiler
// holds the release libraries' functions byte for byte apart from those fields, so a match names the
// function for certain -- once a signature is long enough to place (at least three unmasked words) and lands
// in exactly one place.
//
// A match counts only where a function can start: a .pdata entry, or after an instruction control does not
// fall through (a branch, a return, a trap, padding). A short library function can equal the tail of a
// program function word for word; such a hit is refused unless the library symbol is itself a secondary
// entry of its section (the register save / restore runs and other hand-written routines are entered by
// falling into them).
//
// The library folder defaults to the setting `xdk_libs`, else the XDK's usual install
// (Program Files (x86)/Microsoft Xbox 360 SDK/lib/xbox). The names are the XDK's: the output stays with the
// program's project, never in the repository.
//
// Notes on the port:
//   * The signature table is {(words, bits left out of each): [names, libraries, secondary entry?]}; the
//     knowledge base keeps it as the Python does (platform xbox360, kind "library_signatures", one piece named
//     by the stamp) but as JSON (a list of [words, ignore, names, libs, secondary]), because the Python's
//     value -- a dict with tuple keys -- can only be pickled. The stamp is prefixed "cpp-" so a base made by
//     one implementation is made again by the other, never misread. Signatures are a list of records here.
//   * functions(path) and Index are codebreaker.symbols.libraries's (symbols/libraries/package).
//   * main(argv) is the `python -m codebreaker xdk-match` entry; sys.exit(main(...)) is the caller's.
#pragma once

#include <cstdint>
#include <map>
#include <optional>
#include <vector>

#include "py.hpp"

namespace codebreaker::symbols::xdk_libraries {

extern const py::str DEFAULT_LIBS;
constexpr int64_t REV = 2;      // how the libraries are read (a mask: the bits left out): part of the stamp

py::str lib_folder();

// Without the zero words that pad a function to its alignment.
std::tuple<py::bytes, py::list<int64_t>> _trim(const py::bytes& code, const py::list<int64_t>& mask);

bool _falls_through(int64_t w);

// One library function's signature: its words, the bits left out of each, and where it was found.
struct Signature {
    std::vector<int64_t> words, ignore;
    py::set<py::str> names, libs;
    bool secondary = false;     // a secondary entry of its section
};

// {(words, bits left out of each): [names, libraries, secondary entry?]} from every .lib in `folder`, kept in
// the knowledge base (platform xbox360, kind "library_signatures", one piece named by every library's name,
// size and time and REV: a changed XDK is read again).
py::list<Signature> signatures(const py::str& folder);

py::list<Signature> _read_signatures(const py::str& folder);

struct Found {
    py::list<py::str> names, libs;
    int64_t size = 0;
};

// {address: {"names": [...], "libs": [...], "size": bytes}} for the program's library functions, by address.
py::dict<int64_t, Found> match(const py::str& image_path, const std::optional<py::str>& folder = std::nullopt);

// The module's text a caller prints for no arguments.
extern const py::str USAGE;

// `codebreaker xdk-match`: prints; the exit code.
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::symbols::xdk_libraries
