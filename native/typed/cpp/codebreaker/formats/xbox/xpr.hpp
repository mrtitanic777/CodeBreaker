// codebreaker/formats/xbox/xpr.hpp -- typed C++ from codebreaker.formats.xbox.xpr.
//
// XPR resource-bundle reader -- pull textures out of Xbox `.xpr` packages.
//
// Two container generations, both handled here:
//
//   * XPR0 -- original Xbox.  Little-endian.  Header is magic 'XPR0', total size,
//     header size (where pixel data begins), followed by an Xbox D3DTexture
//     resource whose packed Format dword encodes the nv2a texture format and the
//     log2 width/height.  DXT surfaces are stored *linearly* (unswizzled), so we
//     can slice them straight out and emit a DDS.
//
//   * XPR2 -- Xbox 360.  Big-endian.  Magic 'XPR2', header size, data size, then a
//     resource directory.  The pixel data begins at header size and the 360 tiled
//     DXT layout / 16-bit endian is handled by our texture module.  We parse the
//     header + data region; per-resource dimensions come from the (optional)
//     resource table when present, else must be supplied.
//
// Format facts only -- no third-party code.  Verified by a synthetic XPR0
// round-trip in the selftest (build a bundle around a known DXT1 surface, read it
// back, confirm format/size/pixels and a valid DDS).
//
// Typing notes:
//   * parse() is formats.dll's xpr.parse (native/formats/xbox/graphics.cc, linked in and asked through cbf_call).  Its dict
//     has two shapes, by container: Xpr0 and Xpr2 (records of the dict's keys); `Info` is the variant of the two
//     (`info["container"]` is which one it holds).  info.get("width") of an XPR2 (no such key) is None: see to_dds().
//   * to_dds() is over the typed texture port (formats/xbox/texture.hpp).  XprError is its own exception class
//     (Python: Exception).
//   * Replaces the hook `xpr.parse` of the formats.xbox package.
//   * main() takes sys.argv[1:]; sys.exit(n) is py::SystemExit(n).
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>
#include <variant>

#include "py.hpp"

namespace codebreaker::formats::xbox::xpr {

extern const py::str DESCRIPTION;

struct XprError : py::Exception {
    using py::Exception::Exception;
    const char* type() const override { return "XprError"; }
};

// an XPR0 bundle: {container, xfmt, format_name, dds_fmt, width, height, mipmaps, cubemap, data_offset, pixels, total}
struct Xpr0 {
    py::str container;
    int64_t xfmt = 0;
    py::str format_name;
    std::optional<py::str> dds_fmt;     // "dxt1" / "dxt3" / "dxt5", or None for a surface with no DDS path
    int64_t width = 0;
    int64_t height = 0;
    int64_t mipmaps = 0;
    bool cubemap = false;
    int64_t data_offset = 0;
    py::bytes pixels;
    int64_t total = 0;
};
template <class F> void for_each_field_mut(Xpr0& x, F&& f) {
    f("container", x.container, true);
    f("xfmt", x.xfmt, true);
    f("format_name", x.format_name, true);
    f("dds_fmt", x.dds_fmt, true);
    f("width", x.width, true);
    f("height", x.height, true);
    f("mipmaps", x.mipmaps, true);
    f("cubemap", x.cubemap, true);
    f("data_offset", x.data_offset, true);
    f("pixels", x.pixels, true);
    f("total", x.total, true);
}

// an XPR2 bundle: {container, header_size, data_size, data_offset, pixels}
struct Xpr2 {
    py::str container;
    int64_t header_size = 0;
    int64_t data_size = 0;
    int64_t data_offset = 0;
    py::bytes pixels;
};
template <class F> void for_each_field_mut(Xpr2& x, F&& f) {
    f("container", x.container, true);
    f("header_size", x.header_size, true);
    f("data_size", x.data_size, true);
    f("data_offset", x.data_offset, true);
    f("pixels", x.pixels, true);
}

using Info = std::variant<Xpr0, Xpr2>;

// Assemble a minimal XPR0 bundle around a linear texture (for tests/tools).
py::bytes build_xpr0(const py::bytes& pixels, int64_t width, int64_t height, int64_t xfmt = 0x0C, int64_t mipmaps = 1);

// An XPR0 / XPR2 bundle -> {container, ... pixels}: XPR0 {xfmt, format_name, dds_fmt, width, height, mipmaps,
// cubemap, data_offset, pixels, total}, XPR2 {header_size, data_size, data_offset, pixels} (the 360 pixel region
// is tiled DXT: hand it to texture.untile + texture.to_dds with the surface's real dimensions).  XprError when it
// is neither (formats.dll's, native/formats/xbox/graphics.cc).
Info parse(const py::bytes& data);

// Turn a parsed XPR into DDS bytes. XPR0 DXT is linear -> direct. XPR2 is
// 360-tiled -> untile + endian-fix via the texture module (needs w/h).
py::bytes to_dds(const Info& info, std::optional<int64_t> width = std::nullopt, std::optional<int64_t> height = std::nullopt);

// self-verification: (passed, what)
std::tuple<bool, py::str> _selfcheck();

// the command line without the program name (sys.argv[1:]); prints to stdout
void main(const py::list<py::str>& argv);

}  // namespace codebreaker::formats::xbox::xpr
