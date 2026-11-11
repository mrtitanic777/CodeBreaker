// codebreaker/symbols/libraries/omf.hpp -- compat shim for cb/symbols/libraries/omf.hpp (docs/CPP_DESIGN.md 6.3). The old
// records in, the new reader called, the old records out; header-only. Deleted with its last old-layer consumer
// (symbols/runtime_library, verify/gate16/runtime16).
//
//     omf::functions(data)         [Function] of every object module in a .LIB (or one .OBJ)
//     omf::fields_only(mask)       the mask without the opcodes of far calls
//
//   * The Function here is codebreaker.symbols.libraries.Function as omf.py makes it: its mask is a bytearray.
//   * The reader's helpers (_WIDTH, _number, _mask) have no callers and are not carried over.
#pragma once

#include <cstdint>

#include "cb/compat/py_bridge.hpp"
#include "cb/symbols/libraries/omf.hpp"
#include "py.hpp"

namespace codebreaker::symbols::libraries::omf {

// One function of a library: its symbol, its bytes, its mask and its offset in its section.
struct Function {
    py::str name;
    py::bytes code;
    py::bytearray mask;
    int64_t at = 0;
};

namespace detail {
inline py::list<Function> to_old(const std::vector<cb::symbols::libraries::Function>& found) {
    py::list<Function> out;
    for (const auto& f : found) {
        out.append(Function{cb::compat::to_py(f.name), cb::compat::to_py(cb::ByteView(f.code)),
                            py::bytearray(cb::compat::to_py(cb::ByteView(f.mask))), static_cast<int64_t>(f.at)});
    }
    return out;
}
}  // namespace detail

// The mask without the opcodes of far calls: only what a fix-up's field covers.
inline py::bytes fields_only(const py::bytes& mask) {
    const cb::Bytes raw = cb::compat::from_py(mask);
    return cb::compat::to_py(cb::ByteView(cb::symbols::libraries::omf::fields_only(raw)));
}

// [Function] for every function in a library (a dictionary-paged .LIB) or one object module. A module the reader cannot
// follow is left out.
inline py::list<Function> functions(const py::bytes& data) {
    return cb::compat::guard([&] { return detail::to_old(cb::symbols::libraries::omf::functions(cb::compat::from_py(data))); });
}

// [Function] for the public functions of one object module (its bytes, MODEND included).
inline py::list<Function> module_functions(const py::bytes& module) {
    return cb::compat::guard([&] { return detail::to_old(cb::symbols::libraries::omf::module_functions(cb::compat::from_py(module))); });
}

}  // namespace codebreaker::symbols::libraries::omf
