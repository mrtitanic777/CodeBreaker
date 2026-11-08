// codebreaker/formats/xbox/texture.hpp -- typed C++ of codebreaker.formats.xbox.texture
//
// Xbox 360 texture untiling + DDS export.
//
// Xbox 360 GPU textures are stored in a tiled (swizzled) memory layout for cache
// coherency, and DXT colour/index words are 16-bit byte-swapped versus PC. This
// converts a tiled Xbox 360 surface to a linear one and wraps it in a standard
// DDS so any PC tool can open it.
//
// The tiled-address math is the well-established Gildor/UModel
// `XGAddress2DTiledOffset` formula (also used by Noesis and UEViewer) --
// reimplemented here as original Python. decode_dxt() turns a DXT1/DXT3/DXT5 surface (tiled or
// linear) into an RGBA image for any program that does not read DDS.
//
// Commands:
//     texture untile <in> <w> <h> <dxt1|dxt5|dxt3> <out.dds> [--no-swap]
//     texture dds    <linear.bin> <w> <h> <fmt> <out.dds>   (already-linear data)
//
// Typing notes:
//   * The readers are the C++ of native/formats/xbox/graphics.cc (texture section): the same code, as typed
//     functions here (graphics.cc keeps them in an unnamed namespace behind its pickle interface).  An exception
//     it raised comes back as the py:: exception of the same Python type.
//   * decode_dxt returned a Pillow RGBA image: here the struct `Image` (width, height, and the pixels as
//     tobytes() gives them: 4 bytes per pixel, rows top to bottom).
//   * main(): Python reads sys.argv; here the caller hands sys.argv in (program name first).  sys.exit(2) is the
//     py::SystemExit it raises.
//   * tiled_offset_2d with a negative log2bpp raises ValueError (Python's negative shift count); the native
//     C++ shifted by a negative count, which is undefined.
#pragma once

#include <cstdint>
#include <tuple>

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::formats::xbox::texture {

// format -> (bytes per 4x4 block, log2 of that, dds fourcc)
extern const py::dict<py::str, std::tuple<int64_t, int64_t, py::bytes>> FORMATS;

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

extern const py::str DESCRIPTION;

// ------------------------------------------------------------------------ CLI
void main(const py::list<py::str>& sys_argv);

}  // namespace codebreaker::formats::xbox::texture
