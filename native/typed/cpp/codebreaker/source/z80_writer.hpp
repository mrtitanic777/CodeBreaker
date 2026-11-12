// codebreaker/source/z80_writer.hpp -- COMPAT SHIM (docs/CPP_DESIGN.md 6.3). The module now lives in native/cb/source/z80_writer.{hpp,cpp}; this
// header keeps the old py:: surface (reconstruct and main, with the tables) for the ports and the dynamic exe that still use it. Conversion and
// forwarding only: the result is the old image_writer::Result (codebreaker/source/image_writer.hpp, itself still the old port for the x86
// writers). Deleted with its last py:: consumer.
//
// Not carried over: the internals the module's own port exposed (walk).
#pragma once

#include <optional>

#include "cb/compat/source_writers.hpp"
#include "cb/source/z80_writer.hpp"
#include "codebreaker/source/image_writer.hpp"
#include "py.hpp"

namespace codebreaker::source::z80_writer {

struct Written : image_writer::Result {
    std::optional<py::bytes> built_image;   // r["built image"]
};

// `entries` {address: name}; `ports` {port: name}; `names` {address: name} for fixed addresses outside the block (system variables, ROM routines).
inline Written write(const py::bytes& image, int64_t origin, const py::dict<int64_t, py::str>& entries, const py::str& folder, const py::str& stem,
                     const py::dict<int64_t, py::str>& ports = py::dict<int64_t, py::str>(), const py::dict<int64_t, py::str>& names = py::dict<int64_t, py::str>(),
                     const py::list<py::str>& header = py::list<py::str>()) {
    namespace c = cb::compat::source;
    namespace n = cb::source;
    const auto table = [](const py::dict<int64_t, py::str>& d) {
        n::image_writer::Names out;
        for (const auto& kv : d.items()) out[static_cast<std::uint32_t>(std::get<0>(kv))] = std::get<1>(kv).utf8();
        return out;
    };
    return cb::compat::guard([&] {
        const cb::Bytes bytes = cb::compat::from_py(image);
        n::z80_writer::Block block{.image = bytes, .origin = static_cast<std::uint32_t>(origin), .entries = table(entries), .header = {}};
        for (const py::str& line : header) block.header.push_back(line.utf8());
        const n::image_writer::Written w = n::z80_writer::write(block, {.ports = table(ports), .names = table(names)}, c::path_of(folder), stem.utf8());
        Written out;
        static_cast<image_writer::Result&>(out) = c::old_result(w, "bytes");
        if (w.built_image) out.built_image = cb::compat::to_py(cb::ByteView(*w.built_image));
        return out;
    });
}

}  // namespace codebreaker::source::z80_writer