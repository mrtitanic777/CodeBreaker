// codebreaker/formats/xbox/texture.hpp -- compat shim over cb/formats/xbox/texture.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module itself is native/cb/formats/xbox/texture.{hpp,cpp}; this header keeps the old py:: surface for the ports that
// have not been refactored yet (xnb, xpr, gfx, effect, the XNA lane) and is deleted with the last of them. Conversion and
// forwarding only; `main` is the old command-line wrapper over the same functions.
#pragma once

#include <cstdint>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/formats/xbox/texture.hpp"

namespace codebreaker::formats::xbox::texture {

// format -> (bytes per 4x4 block, log2 of that, dds fourcc)
inline const py::dict<py::str, std::tuple<int64_t, int64_t, py::bytes>> FORMATS = {
    {py::str("dxt1"), std::make_tuple(int64_t(8), int64_t(3), py::bytes("DXT1"))},
    {py::str("dxt3"), std::make_tuple(int64_t(16), int64_t(4), py::bytes("DXT3"))},
    {py::str("dxt5"), std::make_tuple(int64_t(16), int64_t(4), py::bytes("DXT5"))},
};

// Byte offset of tiled block (x, y) -- Gildor/UModel XGAddress2DTiledOffset. x, y and width
// are in blocks; log2bpp = log2(bytes per block), which the result is already scaled by.
int64_t tiled_offset_2d(int64_t x, int64_t y, int64_t width_blocks, int64_t log2bpp);

std::tuple<int64_t, int64_t> _blocks(int64_t w, int64_t h);

// Tiled Xbox 360 surface -> linear block layout.
py::bytes untile(const py::bytes& data, int64_t w, int64_t h, const py::str& fmt);

// Linear block layout -> tiled Xbox 360 surface (inverse of untile).
py::bytes tile(const py::bytes& data, int64_t w, int64_t h, const py::str& fmt);

// Byte-swap every 16-bit word (Xbox 360 DXT endian fix).
py::bytes swap16(const py::bytes& data);

// Standard 128-byte DDS header for a DXT-compressed surface.
py::bytes dds_header(int64_t w, int64_t h, const py::str& fmt);

py::bytes to_dds(const py::bytes& linear, int64_t w, int64_t h, const py::str& fmt);

// (the Pillow RGBA image decode_dxt made)
struct Image {
    int64_t width = 0, height = 0;
    py::bytes pixels;                 // width * height * 4: R, G, B, A of each pixel
};

// A DXT1/DXT3/DXT5 surface as an RGBA image: un-tiled and its 16-bit words swapped as
// the Xbox 360 keeps them (tiled / swapped False for a linear PC surface). force_opaque ignores
// alpha (an opaque mask stored as DXT5).
Image decode_dxt(const py::bytes& data, int64_t w, int64_t h, const py::str& fmt, bool tiled = true, bool swapped = true,
                 bool force_opaque = false);

inline const py::str DESCRIPTION = py::str("Xbox 360 texture untile, DDS export and DXT decoding (DXT1/3/5)");

// ------------------------------------------------------------------------ CLI
void main(const py::list<py::str>& sys_argv);

// ---- the bodies: all conversion --------------------------------------------------------------------------

namespace shim_ {

namespace nt = cb::formats::xbox::texture;

// the format the old code named; a name that is not one is the KeyError the old reader raised
inline nt::Format format_of(const py::str& name) {
    if (const auto f = nt::parse_format(cb::compat::from_py(name))) return *f;
    throw py::KeyError(py::repr(name));
}
inline std::uint32_t dim(int64_t v) { return cb::compat::int_from_py<std::uint32_t>(v); }

}  // namespace shim_

inline int64_t tiled_offset_2d(int64_t x, int64_t y, int64_t width_blocks, int64_t log2bpp) {
    using shim_::dim;
    return cb::compat::to_py(cb::formats::xbox::texture::tiled_offset_2d(dim(x), dim(y), dim(width_blocks), static_cast<unsigned>(log2bpp)));
}

inline std::tuple<int64_t, int64_t> _blocks(int64_t w, int64_t h) {
    const cb::formats::xbox::texture::BlockSize b = cb::formats::xbox::texture::blocks(shim_::dim(w), shim_::dim(h));
    return std::make_tuple(int64_t(b.width), int64_t(b.height));
}

inline py::bytes untile(const py::bytes& data, int64_t w, int64_t h, const py::str& fmt) {
    return cb::compat::guard([&] {
        return cb::compat::to_py(cb::ByteView(cb::formats::xbox::texture::untile(cb::compat::from_py(data), shim_::dim(w), shim_::dim(h), shim_::format_of(fmt))));
    });
}
inline py::bytes tile(const py::bytes& data, int64_t w, int64_t h, const py::str& fmt) {
    return cb::compat::guard([&] {
        return cb::compat::to_py(cb::ByteView(cb::formats::xbox::texture::tile(cb::compat::from_py(data), shim_::dim(w), shim_::dim(h), shim_::format_of(fmt))));
    });
}
inline py::bytes swap16(const py::bytes& data) {
    return cb::compat::guard([&] { return cb::compat::to_py(cb::ByteView(cb::formats::xbox::texture::swap16(cb::compat::from_py(data)))); });
}
inline py::bytes dds_header(int64_t w, int64_t h, const py::str& fmt) {
    return cb::compat::guard([&] {
        return cb::compat::to_py(cb::ByteView(cb::formats::xbox::texture::dds_header(shim_::dim(w), shim_::dim(h), shim_::format_of(fmt))));
    });
}
inline py::bytes to_dds(const py::bytes& linear, int64_t w, int64_t h, const py::str& fmt) {
    return cb::compat::guard([&] {
        return cb::compat::to_py(cb::ByteView(cb::formats::xbox::texture::to_dds(cb::compat::from_py(linear), shim_::dim(w), shim_::dim(h), shim_::format_of(fmt))));
    });
}

inline Image decode_dxt(const py::bytes& data, int64_t w, int64_t h, const py::str& fmt, bool tiled, bool swapped, bool force_opaque) {
    return cb::compat::guard([&] {
        const cb::formats::xbox::texture::Image im = cb::formats::xbox::texture::decode_dxt(
            cb::compat::from_py(data), shim_::dim(w), shim_::dim(h), shim_::format_of(fmt), {.tiled = tiled, .swapped = swapped, .force_opaque = force_opaque});
        Image out;
        out.width = im.width;
        out.height = im.height;
        out.pixels = cb::compat::to_py(cb::ByteView(im.rgba));
        return out;
    });
}

// the old command-line wrapper over the functions above
inline void main(const py::list<py::str>& sys_argv) {
    py::list<py::str> a = sys_argv.slice_from(1);
    bool swap = true;
    if (a.contains(py::str("--no-swap"))) {
        swap = false;
        a.remove(py::str("--no-swap"));
    }
    if (a.empty() || a[0] == py::str("-h") || a[0] == py::str("--help") || !(a[0] == py::str("untile") || a[0] == py::str("dds"))) {
        py::print(py::str("usage: texture untile <in> <w> <h> <dxt1|dxt3|dxt5> <out.dds> [--no-swap]\n"
                          "       texture dds    <linear.bin> <w> <h> <fmt> <out.dds>"));
        return;
    }
    py::str op = a[0], inp = a[1];
    int64_t w = py::int_parse(a[2]);
    int64_t h = py::int_parse(a[3]);
    py::str fmt = a[4].lower(), out = a[5];
    if (!FORMATS.contains(fmt)) {
        py::print(py::percent(py::str("unknown format %r (dxt1/dxt3/dxt5)"), fmt));
        throw py::SystemExit(2);
    }
    const py::bytes data = cb::compat::guard([&] {
        const cb::Bytes file = cb::fs::read_file(cb::fs::from_text(cb::compat::from_py(inp)));
        return cb::compat::to_py(cb::ByteView(file));
    });
    py::bytes linear = op == py::str("untile") ? untile(data, w, h, fmt) : data;
    if (swap) linear = swap16(linear);
    const py::bytes dds = to_dds(linear, w, h, fmt);
    cb::compat::guard([&] { cb::fs::write_file(cb::fs::from_text(cb::compat::from_py(out)), cb::compat::from_py(dds)); });
    py::print(py::percent(py::str("%s %dx%d %s -> %s (%d bytes)"), op, w, h, fmt, out, dds.size()));
}

}  // namespace codebreaker::formats::xbox::texture
