// codebreaker/source/msx_writer.hpp -- COMPAT SHIM (docs/CPP_DESIGN.md 6.3). The module now lives in native/cb/source/msx_writer.{hpp,cpp}; this
// header keeps the old py:: surface (reconstruct and main, with the tables) for the ports and the dynamic exe that still use it. Conversion and
// forwarding only. Deleted with its last py:: consumer.
#pragma once

#include <optional>

#include "cb/compat/source_writers.hpp"
#include "cb/source/msx_writer.hpp"
#include "codebreaker/source/image_writer.hpp"
#include "codebreaker/source/z80_writer.hpp"
#include "py.hpp"

namespace codebreaker::source::msx_writer {

inline const py::str DOC(R"DOC(msx_writer.py -- an MSX cartridge as assembly source that builds back into it.

    python -m codebreaker.source.msx_writer <game.rom> [<out folder>]
    from codebreaker.source import msx_writer
    r = msx_writer.reconstruct(path, folder)    # as source/z80_writer.py's write()

The cartridge (formats/msx.py) is written through source/z80_writer.py from its header's entry
points, with the MSX's names: the VDP, PSG and PPI ports and the BIOS's documented entry points
and system variables, as the MSX2 Technical Handbook names them.
)DOC");

namespace shim_ {
inline py::dict<int64_t, py::str> table(const std::map<std::uint32_t, std::string>& t) {
    py::dict<int64_t, py::str> d;
    for (const auto& [address, name] : t) d.set(static_cast<int64_t>(address), py::str(name));
    return d;
}
}  // namespace shim_

inline const py::dict<int64_t, py::str> PORTS = shim_::table(cb::source::msx_writer::machine().ports);
inline const py::dict<int64_t, py::str> NAMES = shim_::table(cb::source::msx_writer::machine().names);

inline z80_writer::Written reconstruct(const py::str& path, const py::str& folder) {
    namespace c = cb::compat::source;
    return cb::compat::guard([&] {
        const cb::source::image_writer::Written w = cb::source::msx_writer::reconstruct(c::path_of(path), c::path_of(folder));
        z80_writer::Written out;
        static_cast<image_writer::Result&>(out) = c::old_result(w, "bytes");
        if (w.built_image) out.built_image = cb::compat::to_py(cb::ByteView(*w.built_image));
        return out;
    });
}

inline int64_t main(const py::list<py::str>& argv) {
    return image_writer::main(
        argv, [](const py::str& path, const py::str& folder) -> image_writer::Result { return reconstruct(path, folder); }, DOC, py::str("bytes"),
        py::str("bytes"));
}

}  // namespace codebreaker::source::msx_writer
