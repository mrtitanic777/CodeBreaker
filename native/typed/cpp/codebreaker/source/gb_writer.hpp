// codebreaker/source/gb_writer.hpp -- COMPAT SHIM (docs/CPP_DESIGN.md 6.3). The module now lives in native/cb/source/gb_writer.{hpp,cpp}; this
// header keeps the old py:: surface (reconstruct and main, with the tables) for the ports and the dynamic exe that still use it. Conversion and
// forwarding only: the result is the old image_writer::Result (codebreaker/source/image_writer.hpp, itself still the old port for the x86
// writers). Deleted with its last py:: consumer.
//
// Not carried over: the internals the module's own port exposed (_HEADER, _HEADER_AT, _walk).
#pragma once

#include "cb/compat/source_writers.hpp"
#include "cb/source/gb_writer.hpp"
#include "codebreaker/source/image_writer.hpp"
#include "py.hpp"

namespace codebreaker::source::gb_writer {

inline const py::str DOC("gb_writer.py -- a Game Boy program as assembly source that builds back into it.");

namespace shim_ {
inline py::dict<int64_t, py::str> table(const std::map<std::uint32_t, std::string>& t) {
    py::dict<int64_t, py::str> d;
    for (const auto& [address, name] : t) d.set(static_cast<int64_t>(address), py::str(name));
    return d;
}
}  // namespace shim_

// the I/O registers, as hardware.inc names them; and the bank controller's (writes into ROM space)
inline const py::dict<int64_t, py::str> REGISTERS = shim_::table(cb::source::gb_writer::registers());
inline const py::dict<int64_t, py::str> MBC_REGISTERS = shim_::table(cb::source::gb_writer::mbc_registers());

inline image_writer::Result reconstruct(const py::str& path, const py::str& folder) {
    namespace c = cb::compat::source;
    return cb::compat::guard([&] { return c::old_result(cb::source::gb_writer::reconstruct(c::path_of(path), c::path_of(folder)), "rom bytes"); });
}

inline int64_t main(const py::list<py::str>& argv) { return image_writer::main(argv, reconstruct, DOC); }

}  // namespace codebreaker::source::gb_writer