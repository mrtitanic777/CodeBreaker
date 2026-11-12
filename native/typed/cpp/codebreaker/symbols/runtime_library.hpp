// codebreaker/symbols/runtime_library.hpp -- compat shim for cb/symbols/runtime_library.hpp (docs/CPP_DESIGN.md 6.3).
//
// runtime_library.py -- which of a module's functions came with the compiler, and which compiler.
//
//     identify(path)      {address: (name, library)}
//     compiler(path)      {"linker": "2.50", "compiler": "Microsoft Visual C++ 2.x", "evidence": [...]}
//
// Addresses are as the decompiler's descriptions use them: the address for a 32-bit module,
// describe.address(segment, offset) for a 16-bit one.
//
// The signatures, the matching, the naming of what is found and the guess at the compiler are cb/symbols/runtime_library;
// runtime_library.cpp (this shim's translation unit) gives it the module the old readers and the corpus make of a path, the
// signature rows the knowledge base holds, and converts types. Deleted with its last old-layer consumer.
//
// Typing notes:
//   * compiler()'s dict with fixed keys is the record `Compiler`.
//   * signature() returns None or (size, mask text, digest); `mask` is the library function's byte mask (py::bytes:
//     truthy bytes are the places the linker fills in).
//   * match(bits, code): `bits` is 16 or 32.
//   * _flat_thunk_stubs takes the PE and its code sections as formats::pe gives them.
//   * learn()'s `log` defaults to print (nullptr here).
//   * `_signatures()` and its Entry/Index types are gone: the index is cb::symbols::runtime_library::SignatureTable.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>

#include "codebreaker/formats/pe.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::symbols::runtime_library {

// modules that are a runtime library themselves: stem -> what their exports are
extern const py::dict<py::str, py::str> ITSELF;
extern const py::str THUNK_COMPILER;   // "Microsoft thunk compiler: a 32-to-16-bit flat thunk"
// used by: symbols.names
extern const py::re::Pattern _FRAME;   // the instructions every function's frame is made of

// (size, mask text, digest) of a function's bytes with what its byte mask leaves out left out, or None when too little
// is left to tell it from anything else.
std::optional<std::tuple<int64_t, py::str, py::str>> signature(const py::bytes& code, const py::bytes& mask);
// (name, library) of the library function `code` starts with, or None.
std::optional<std::tuple<py::str, py::str>> match(int64_t bits, const py::bytes& code);

// A linker symbol as the C name: `_memcpy` -> memcpy, `__allmul` -> _allmul, `_f@8` -> f.
py::str _c_name(const py::str& symbol);
// The functions of a flat-thunk module that the thunk compiler generated. (used by verify.lanes.win32)
py::list<int64_t> _flat_thunk_stubs(const py::str& module_path, const formats::pe::PEFile& pe,
                                    const py::list<std::tuple<uint64_t, py::bytes, py::str>>& sections);
// {address: (name, library)} for the functions of a module that belong to a runtime library.
py::dict<int64_t, std::tuple<py::str, py::str>> identify(const py::str& module_path);

// What built a module: the linker's version, the guessed compiler, the evidence.
struct Compiler {
    py::str linker;
    py::str compiler;
    py::list<py::str> evidence;
};
// `identified` defaults to identify(module_path).
Compiler compiler(const py::str& module_path,
                  const std::optional<py::dict<int64_t, std::tuple<py::str, py::str>>>& identified = std::nullopt);

// How many instructions of a function do something of their own (not frame set-up, not `return 0`).
int64_t substance(int64_t bits, const py::bytes& code, int64_t size);
// Rewrite runtime_signatures.json from [(what the library is, file)]; -> {"32": n, "16": n}
py::dict<py::str, int64_t> learn(const py::list<std::tuple<py::str, py::str>>& sources,
                                 const std::function<void(const py::str&)>& log = nullptr);
// python -m codebreaker identify ...; returns the exit status
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::symbols::runtime_library
