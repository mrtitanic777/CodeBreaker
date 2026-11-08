// codebreaker/symbols/runtime_library.hpp -- interface of codebreaker.symbols.runtime_library (typed C++; implementation pending).
//
// runtime_library.py -- which of a module's functions came with the compiler, and which compiler.
//
//     identify(path)      {address: (name, library)}
//     compiler(path)      {"linker": "2.50", "compiler": "Microsoft Visual C++ 2.x", "evidence": [...]}
//
// Addresses are as the decompiler's descriptions use them: the address for a 32-bit module,
// describe.address(segment, offset) for a 16-bit one.
//
// Typing notes:
//   * compiler()'s dict with fixed keys is the record `Compiler`.
//   * signature() returns None or (size, mask text, digest); `mask` is the library function's byte mask (py::bytes:
//     truthy bytes are the places the linker fills in, as symbols.libraries.coff/omf Function.mask).
//   * match(bits, code): `bits` is an int (Python passes 16/32 or "16"/"32", compared as str()).
//   * _signatures() is {"32"|"16": {leading bytes: [Entry]}}.
//   * _flat_thunk_stubs takes the PE and its code sections as formats::pe gives them.
//   * learn()'s `log` defaults to print (nullptr here).
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

// (size, masked offsets, digest, name, library): one signature of the index
struct Entry {
    int64_t size = 0;
    py::list<int64_t> masked;
    py::str digest, name, library;
};
using Index = py::dict<py::str, py::dict<py::bytes, py::list<Entry>>>;

// (size, mask text, digest) of a function's bytes with what its byte mask leaves out left out, or None when too little
// is left to tell it from anything else.
std::optional<std::tuple<int64_t, py::str, py::str>> signature(const py::bytes& code, const py::bytes& mask);
// {"32"|"16": {leading bytes: [Entry]}}
Index& _signatures();
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

// How many instructions of a function do something of its own (not frame set-up, not `return 0`).
int64_t substance(int64_t bits, const py::bytes& code, int64_t size);
// Rewrite runtime_signatures.json from [(what the library is, file)]; -> {"32": n, "16": n}
py::dict<py::str, int64_t> learn(const py::list<std::tuple<py::str, py::str>>& sources,
                                 const std::function<void(const py::str&)>& log = nullptr);
// python -m codebreaker identify ...; returns the exit status
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::symbols::runtime_library
