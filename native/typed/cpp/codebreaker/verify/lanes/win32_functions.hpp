// codebreaker/verify/lanes/win32_functions.hpp -- COMPAT SHIM: the one function of the old 32-bit lane that the old decompile headers ask for. A header of its
// own, so that they need not take in the lane's (win32.hpp brings in the decompiler and the reconstruction path). No logic: forwarding to the carve of
// cb/formats/pe_functions, which is what Win32Lane::functions answers from.
#pragma once

#include <cstdint>

#include "cb/compat/legacy_settings.hpp"
#include "cb/compat/py_bridge.hpp"
#include "cb/formats/pe_functions.hpp"
#include "py.hpp"

namespace codebreaker::verify::lanes::win32 {

// The starts of a 32-bit module's functions, ascending (sorted(pe_functions.carve(path).funcs)).
inline py::list<int64_t> functions(const py::str& lane, const py::str& path) {
    (void)lane;   // ("pe")
    const ::cb::formats::pe_functions::PeCarve carve = ::cb::compat::guard([&] {
        return ::cb::formats::pe_functions::PeCarve::open(::cb::compat::legacy::path_of(path));
    });
    py::list<int64_t> out;
    for (const auto& [start, function] : carve.functions()) out.append(static_cast<int64_t>(start));   // (the map's order: ascending)
    return out;
}

}  // namespace codebreaker::verify::lanes::win32
