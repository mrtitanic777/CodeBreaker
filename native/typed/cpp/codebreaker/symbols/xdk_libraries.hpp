// codebreaker/symbols/xdk_libraries.hpp -- compat shim for cb/symbols/xdk_libraries.hpp (docs/CPP_DESIGN.md 6.3). The old
// types in, the new signatures and matcher called, the old types out; the old format readers (symbols/libraries, formats/xex,
// formats/powerpc_image) and the knowledge base (platform xbox360, kind "library_signatures") stay behind
// xdk_libraries.cpp, the shim's translation unit. Deleted with its last old-layer consumer.
//
//     codebreaker xdk-match <program.xex or image> [<XDK lib folder>] [-o names.json]
//     auto found = xdk_libraries::match(image_path, lib_folder);    // {address: {"names", "libs", "size"}}
//
// The knowledge base keeps the signature table as JSON (a list of [words, ignore, names, libs, secondary]) under a
// stamp of the library files and the reader's revision (cb::symbols::xdk_libraries::kReaderRevision, prefixed "cpp-"),
// so a base made by another reader is made again, never misread.
#pragma once

#include <cstdint>
#include <map>
#include <optional>
#include <vector>

#include "py.hpp"

namespace codebreaker::symbols::xdk_libraries {

extern const py::str DEFAULT_LIBS;
constexpr int64_t REV = 2;      // how the libraries were read before the refactor; the stamp now carries the reader's revision

py::str lib_folder();

bool _falls_through(int64_t w);

// One library function's signature: its words, the bits left out of each, and where it was found.
struct Signature {
    std::vector<int64_t> words, ignore;
    py::set<py::str> names, libs;
    bool secondary = false;     // a secondary entry of its section
};

// {(words, bits left out of each): [names, libraries, secondary entry?]} from every .lib in `folder`, kept in the
// knowledge base (platform xbox360, kind "library_signatures", one piece named by the stamp of the files).
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
