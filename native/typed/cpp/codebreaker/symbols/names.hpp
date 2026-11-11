// codebreaker/symbols/names.hpp -- interface of codebreaker.symbols.names (typed C++; implementation pending).
//
// names.py -- the best name for every function and import of a module, and where it came from.
//
//     names::of_module(path)        {"functions": [(address, key, name, source)], "imports": [(module, ordinal, name, source)]}
//     names::bodies(path)           {address: digest of the body}
//     names::index(path)            record the module's bodies, and the names it gives them, in the store
//     names::set_name(path, address, "Name")     name a body: it holds in every module with that body
//
// A function's name is taken from the first of: export, run-time types (PowerPC), debug symbols, ordinal table,
// named body, runtime library, same body; else sub_<address>.
//
// Typing notes:
//   * of_module()'s dict {"functions", "imports"} is the record `ModuleNames`; its tuples are records `FunctionName`
//     and `ImportName` (`ordinal` is None for an import by name).
//   * `fs` (the function store) defaults to store::functions::open(): nullptr here means "open it".
//   * An address is int64_t (a 32-bit address, or describe::address(seg, off) for a 16-bit module).
//   * own_names() answers {address: (name, source)}.
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>

#include "codebreaker/engine/native_engine.hpp"
#include "py.hpp"

namespace codebreaker::symbols::names {

using FunctionStorePtr = std::shared_ptr<engine::native_engine::FunctionStore>;

// one function of a module: (address, the function store's key, name, where the name is from)
struct FunctionName {
    int64_t address = 0;
    py::str key, name, source;
};
// one import: (module, ordinal or None, name, where the name is from)
struct ImportName {
    py::str module;
    std::optional<int64_t> ordinal;
    py::str name, source;
};
struct ModuleNames {
    py::list<FunctionName> functions;
    py::list<ImportName> imports;
};

// {address: (name, source)}
using Named = py::dict<int64_t, std::tuple<py::str, py::str>>;

// The function store's text for an address (see verify/lanes/win32.py and win16.py).
py::str key(const py::str& module_path, int64_t address);

// {address: digest} for the functions of a module that have a body worth telling apart.
py::dict<int64_t, py::str> bodies(const py::str& module_path);

// {(segment, offset): name} from CodeView symbols appended to an executable (the NB09 form), or {}.
py::dict<std::tuple<int64_t, int64_t>, py::str> codeview(const py::bytes& data);
// [(segment number, {offset: name})] from a .SYM file.
py::list<std::tuple<int64_t, py::dict<int64_t, py::str>>> read_sym(const py::str& path);
// {segment number: name} from a .SYM file ({} when it cannot be read).
py::dict<int64_t, py::str> sym_objects(const py::str& path);
// {address: name of the object file the function was linked from}.
py::dict<int64_t, py::str> source_files(const py::str& module_path);
// {address: (name, where it is from)} from debug symbols shown to be for this very build.
py::dict<int64_t, std::tuple<py::str, py::str>> debug_names(const py::str& module_path);

// {address: (name, source)} from what the module and its symbol files say themselves.
Named own_names(const py::str& module_path);
// {address: (name, source)} for a PowerPC image (run-time type information and the entry point).
Named powerpc_names(const py::str& module_path);

// Record a module's bodies in the store, each with the name the module itself gives it. -> number of bodies
int64_t index(const py::str& module_path, const FunctionStorePtr& fs = nullptr, const std::optional<Named>& own = std::nullopt);
// Name the body of the function at `address`; -> the body's digest, or None.
std::optional<py::str> set_name(const py::str& module_path, int64_t address, const py::str& name,
                                const FunctionStorePtr& fs = nullptr);
// Every function and import of a module with its best name and where the name is from.
ModuleNames of_module(const py::str& module_path, const FunctionStorePtr& fs = nullptr);

// python -m codebreaker names <module> [--set ADDRESS NAME] | --shared | --ordinals FILE...
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::symbols::names
