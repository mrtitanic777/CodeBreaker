// codebreaker/symbols/ordinals.hpp -- interface of codebreaker.symbols.ordinals (typed C++; implementation pending).
//
// ordinals.py -- the name of a function that is imported by number.
//
//     ordinals::name("USER", 91)            "GetDlgItem"
//     ordinals::name("MFC30", 800)          "??1CString@@QAE@XZ"
//     ordinals::name("KERNEL", 99999)       None
//     ordinals::source("USER", 91)          "user.exe"
//     ordinals::table("COMMDLG")            {ordinal: name}
//
// Typing notes:
//   * An ordinal is an int64_t (Python passes `int(ordinal)`; callers pass ints).
//   * (MODULE, ordinal) keys are tuples; a name that may be unknown is std::optional<py::str>.
//   * collect()'s `log` (default print) is a std::function; its result is the written table, a record per module
//     (`Shipped`: {from, checked:[yes,no], names:{str(ordinal): name}}).
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::symbols::ordinals {

// One module's entry in ordinal_names.json: where the names were read from, how many were checked against the
// code (agreeing, disagreeing), and the names by decimal ordinal.
struct Shipped {
    py::str from;
    std::tuple<int64_t, int64_t> checked{0, 0};
    py::dict<py::str, py::str> names;
};

// The export's name exactly as the tables hold it (decorated, capitals), or None.
std::optional<py::str> linker_name(const py::str& module, int64_t ordinal);
// The export's name: as the headers spell it when they know it, else as the tables hold it.
std::optional<py::str> name(const py::str& module, int64_t ordinal);
// (name, where) for an export only documentation names, or None.
std::optional<std::tuple<py::str, py::str>> documented(const py::str& module, int64_t ordinal);
// Where name() read it: the exporting file's name, or the definition file / library ("" when unknown).
py::str source(const py::str& module, int64_t ordinal);
// {ordinal: name} -- everything known for one module.
py::dict<int64_t, py::str> table(const py::str& module);
// The name as a C identifier, or None when it has none (unknown, or a C++ name).
std::optional<py::str> identifier(const py::str& module, int64_t ordinal);
// {(MODULE, ordinal): name or None} for every function a 16-bit module imports by number.
py::dict<std::tuple<py::str, int64_t>, std::optional<py::str>> imports16(const py::str& module_path);
// the same for a 32-bit module
py::dict<std::tuple<py::str, int64_t>, std::optional<py::str>> imports32(const py::str& module_path);

// {MODULE: {ordinal: name}} from a linker definition file (`name @ number [NONAME]`).
py::dict<py::str, py::dict<int64_t, py::str>> read_definition_file(const py::str& path);
// {MODULE: {ordinal: name}} of the functions an import library reaches by number (32-bit archive or 16-bit records).
py::dict<py::str, py::dict<int64_t, py::str>> read_import_library(const py::str& path);

// Rewrite ordinal_names.json from definition files and import libraries (files or folders; earlier ones win).
// `log` defaults to print.
py::dict<py::str, Shipped> collect(const py::list<py::str>& paths,
                                   const std::function<void(const py::str&)>& log = nullptr);

}  // namespace codebreaker::symbols::ordinals
