// codebreaker/build/definitions.hpp -- C++17 port of codebreaker.build.definitions.
//
// definitions.py -- what a module offers and what it needs: exports, imports, and their source forms.
//
// A module's interface is read from the original and written three ways: a module-definition
// file (`.def`, the form a person reads and edits), linker directives (what the build passes to
// the linker), and a C header declaring the exported functions.
//
//     definitions::exports(path)      // [Export]: ordinal, name, where it points, what kind of thing
//     definitions::imports(path)      // {(module, name or ordinal)}: every function it takes from others
//     definitions::definition_file(path)                   // the text of a .def
//     definitions::export_directives(path, symbol_at)      // (linker lines, [(export, why it is missing)])
//     definitions::public_header(path, declaration_at)     // the text of a C header
//
// How the linker is told (facts found the hard way):
//
//   * An export is tied to the function AT ITS ADDRESS, not to a function of the same name: the
//     reconstruction's name for a function need not be the exported name, and an export by
//     ordinal has no name at all. `symbol_at(address)` gives the object-file symbol there.
//   * A symbol holding `@`, `?` or `$` (a decorated __stdcall name, a C++ name) must be quoted:
//     unquoted, the linker reads `@` as "take directives from this file". Quoting is harmless
//     for a plain symbol in an export, so every symbol is quoted.
//   * The linker cannot export by ordinal alone: such an export gets a made-up name
//     (`ordinal_N`), which the build's check reports as an extra name.
//   * A 16-bit module is a library when its header says so (flag 0x8000), whatever its file is
//     called -- KRNL386, USER and GDI are libraries named .EXE.
//
// Typing notes:
//   * Export.name is None for an export by ordinal only: std::optional<py::str>.
//   * Export.address is a virtual address (32-bit; the PE's, so uint64_t) or (segment, offset) (16-bit):
//     a std::variant of the two.
//   * imports() is {(module, name or ordinal)}: the second item is a str or an int (std::variant).
//   * symbol_at / declaration_at are callables of an Export giving a text or None
//     (std::optional<py::str>; an empty text counts as None, as the Python's truthiness test does).
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>
#include <variant>

#include "py.hpp"

namespace codebreaker::build::definitions {

// One export. `address` is a virtual address (32-bit) or (segment, offset) (16-bit);
// `kind` is "code", "data" or "forward" (`target` then names the function elsewhere);
// `resident` says a 16-bit name is in the resident table.
struct Export {
    int64_t ordinal;
    std::optional<py::str> name;
    std::variant<uint64_t, std::tuple<int64_t, int64_t>> address;
    py::str kind;
    bool resident;
    py::str target;
    bool shared_data;   // 16-bit: the loader gives it the module's data segment

    Export(int64_t ordinal_, const std::optional<py::str>& name_, const std::variant<uint64_t, std::tuple<int64_t, int64_t>>& address_,
           const py::str& kind_, bool resident_ = false, const py::str& target_ = py::str(""), bool shared_data_ = true)
        : ordinal(ordinal_), name(name_), address(address_), kind(kind_), resident(resident_), target(target_), shared_data(shared_data_) {}

    // <export NAME @ORDINAL KIND>  (the Python's __repr__)
    py::str repr() const;
};

using Import = std::tuple<py::str, std::variant<py::str, int64_t>>;   // (MODULE, name or ordinal)

// Whether a module is a DLL: the header's flag, not the file's name.
bool is_library(const py::str& path);

// The name the module calls itself (falls back to the file's name).
py::str module_name(const py::str& path);

// A 16-bit module's description line (the first entry of its non-resident names).
py::str description(const py::str& path);

// Every export of a module, by ordinal.
py::list<Export> exports(const py::str& path);

// {(MODULE, name or ordinal)} -- everything a module takes from other modules. A 16-bit
// module's imports are read from its relocations (it has no import list as such).
py::set<Import> imports(const py::str& path);

// The module's interface as a module-definition file.
py::str definition_file(const py::str& path);

// Linker `export` lines for every export whose address has a symbol in the build.
// `symbol_at(export)` -> the object-file symbol for the thing the export points at, or None.
// -> ([line], [(Export, why it could not be exported)])
std::tuple<py::list<py::str>, py::list<std::tuple<Export, py::str>>> export_directives(
    const py::str& path, const std::function<std::optional<py::str>(const Export&)>& symbol_at);

// A C header declaring the module's exported functions. `declaration_at(export)` gives
// the reconstruction's declaration of the function an export points at, or None.
py::str public_header(const py::str& path, const std::function<std::optional<py::str>(const Export&)>& declaration_at);

}  // namespace codebreaker::build::definitions
