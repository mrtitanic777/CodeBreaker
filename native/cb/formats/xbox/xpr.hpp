// cb/formats/xbox/xpr.hpp -- XPR resource bundles: the textures an Xbox or Xbox 360 game packs into one file, out as DDS.
//
//     xpr::Bundle b = xpr::parse(bytes);
//     Bytes dds = xpr::to_dds(b);                        XPR0 DXT surfaces directly; XPR2 with the surface's width and height
//
// Two container generations:
//
//   XPR0 -- the original Xbox, little-endian. A header: "XPR0", total size, header size (where the pixel data begins), then an Xbox
//           D3DTexture resource whose packed Format dword (at 0x18) holds the nv2a texture format (bits 8-15), mip levels (16-19), and the log2
//           width and height (20-23, 24-27); bit 2 marks a cube map. DXT surfaces are stored linearly, so they are sliced out and wrapped.
//   XPR2 -- the Xbox 360, big-endian: "XPR2", header size, data size, a resource directory. The pixel data begins at the header size and is
//           the 360's tiled, word-swapped DXT layout, handled by cb/formats/xbox/texture.hpp. The per-resource dimensions are not read, so
//           the caller supplies them.
//
// Errors: FormatError for bytes that are neither; UnsupportedError for an XPR0 format that is not DXT (no DDS path), UsageError for an
// XPR2 given no dimensions.
#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>

#include "cb/core/bytes.hpp"
#include "cb/core/cli.hpp"
#include "cb/formats/xbox/texture.hpp"

namespace cb::formats::xbox::xpr {

enum class Container { Xpr0, Xpr2 };
std::string_view container_name(Container c);   // "XPR0" "XPR2"

struct Bundle {
    Container container = Container::Xpr0;
    // XPR0: the texture
    std::uint32_t xfmt = 0;                           // the nv2a format number
    std::string format_name;                          // "DXT1", "A8R8G8B8", ... or "0x.." for one not named
    std::optional<texture::Format> dds_format;        // the DXT formats only
    std::uint32_t width = 0, height = 0, mipmaps = 0;
    bool cubemap = false;
    std::uint64_t total = 0;                          // the size the header declares (the file's when it declares none)
    // XPR2
    std::uint32_t header_size = 0, data_size = 0;
    // both
    std::uint64_t data_offset = 0;
    Bytes pixels;
};

// The nv2a format's name; "0x<hex>" for a number not named here.
std::string format_name(std::uint32_t xfmt);

// FormatError when neither magic is there or the header is cut short.
Bundle parse(ByteView file);

// A minimal XPR0 bundle around a linear surface (what the tools and tests make). `width` and `height` are powers of two.
Bytes build_xpr0(ByteView pixels, std::uint32_t width, std::uint32_t height, std::uint32_t xfmt = 0x0C, std::uint32_t mipmaps = 1);

// DDS bytes of the bundle's texture. An XPR2 needs `width` and `height` (it is untiled as DXT5, which is what its bundles hold).
Bytes to_dds(const Bundle& bundle, std::optional<std::uint32_t> width = std::nullopt, std::optional<std::uint32_t> height = std::nullopt);

// `xpr info|dds <file.xpr> ...`: the module's command line.
cli::Command command();

}  // namespace cb::formats::xbox::xpr
