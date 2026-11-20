#include "formats/xbox/xpr.hpp"

#include <algorithm>
#include <bit>
#include <format>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/io.hpp"
#include "core/log.hpp"

namespace cb::formats::xbox::xpr {

namespace {

constexpr std::string_view kXprReader = "xpr";
constexpr std::size_t kXpr0MinSize = 0x1C;
constexpr std::size_t kXpr2MinSize = 0x10;
constexpr std::uint32_t kXprFormatAt = 0x18;   // the D3DTexture's packed Format dword

struct XprFormatName {
    std::uint32_t xfmt;
    std::string_view name;
};
constexpr XprFormatName kXprFormats[] = {{0x00, "L8"},       {0x01, "AL8"},  {0x02, "A1R5G5B5"}, {0x05, "R5G6B5"},   {0x06, "A8R8G8B8"},
                                         {0x07, "X8R8G8B8"}, {0x0B, "P8"},   {0x0C, "DXT1"},     {0x0E, "DXT3"},     {0x0F, "DXT5"},
                                         {0x19, "A8"},       {0x1A, "A8L8"}, {0x3F, "LIN_A8R8G8B8"}};

std::optional<texture::Format> xpr_dds_format(std::uint32_t xfmt) {
    switch (xfmt) {
        case 0x0C: return texture::Format::Dxt1;
        case 0x0E: return texture::Format::Dxt3;
        case 0x0F: return texture::Format::Dxt5;
        default: return std::nullopt;
    }
}

// The pixels of [offset, end) of the file; a bundle the file cuts short is as long as the file holds.
Bytes xpr_pixels(ByteView file, std::uint64_t offset, std::uint64_t end) {
    if (offset > file.size()) throw FormatError(kXprReader, offset, "the pixel data starts past the end of the file");
    end = std::clamp<std::uint64_t>(end, offset, file.size());
    return Bytes(file.begin() + static_cast<std::ptrdiff_t>(offset), file.begin() + static_cast<std::ptrdiff_t>(end));
}

}  // namespace

std::string_view container_name(Container c) { return c == Container::Xpr0 ? "XPR0" : "XPR2"; }

std::string format_name(std::uint32_t xfmt) {
    for (const XprFormatName& f : kXprFormats) {
        if (f.xfmt == xfmt) return std::string(f.name);
    }
    return std::format("0x{:02X}", xfmt);
}

Bundle parse(ByteView file) {
    io::ByteReader r(file, kXprReader);
    Bundle b;
    const std::string_view magic = as_text(r.bytes(4));
    if (magic == "XPR0") {
        if (file.size() < kXpr0MinSize) throw TruncatedError(kXprReader, 0, kXpr0MinSize, file.size());
        b.container = Container::Xpr0;
        const std::uint32_t total = r.u32(), header_size = r.u32();
        r.seek(kXprFormatAt);
        const std::uint32_t format = r.u32();
        b.xfmt = (format >> 8) & 0xFF;
        b.mipmaps = (format >> 16) & 0xF;
        b.width = 1u << ((format >> 20) & 0xF);
        b.height = 1u << ((format >> 24) & 0xF);
        b.cubemap = (format & 0x4) != 0;
        b.format_name = format_name(b.xfmt);
        b.dds_format = xpr_dds_format(b.xfmt);
        b.data_offset = header_size ? header_size : kXpr0MinSize;
        // a total that is zero or larger than the file means "to the end"
        b.pixels = xpr_pixels(file, b.data_offset, (total && total <= file.size()) ? total : file.size());
        b.total = total ? total : file.size();
    } else if (magic == "XPR2") {
        if (file.size() < kXpr2MinSize) throw TruncatedError(kXprReader, 0, kXpr2MinSize, file.size());
        b.container = Container::Xpr2;
        b.header_size = r.u32_be();
        const std::uint32_t data_size = r.u32_be();
        b.data_offset = b.header_size;
        b.pixels = xpr_pixels(file, b.data_offset, data_size ? b.data_offset + data_size : file.size());
        b.data_size = data_size ? data_size : static_cast<std::uint32_t>(b.pixels.size());
    } else {
        throw FormatError(kXprReader, 0, "unknown magic (expected XPR0 or XPR2)");
    }
    return b;
}

Bytes build_xpr0(ByteView pixels, std::uint32_t width, std::uint32_t height, std::uint32_t xfmt, std::uint32_t mipmaps) {
    if (!std::has_single_bit(width) || !std::has_single_bit(height)) throw UsageError("an XPR0 texture's width and height are powers of two");
    constexpr std::uint32_t header_size = 0x1C;
    const std::uint32_t format = ((xfmt & 0xFF) << 8) | (2u << 4) | ((mipmaps & 0xF) << 16) |
                                 (static_cast<std::uint32_t>(std::countr_zero(width) & 0xF) << 20) |
                                 (static_cast<std::uint32_t>(std::countr_zero(height) & 0xF) << 24);
    io::ByteWriter w;
    w.bytes(as_bytes("XPR0"));
    w.u32(static_cast<std::uint32_t>(header_size + pixels.size()));
    w.u32(header_size);
    w.u32(0);              // the D3DTexture resource: common, data, lock, format
    w.u32(header_size);
    w.u32(0);
    w.u32(format);
    w.bytes(pixels);
    return w.take();
}

Bytes to_dds(const Bundle& bundle, std::optional<std::uint32_t> width, std::optional<std::uint32_t> height) {
    if (bundle.container == Container::Xpr0) {
        if (!bundle.dds_format) throw UnsupportedError(kXprReader, 0, std::format("XPR0 format {} is not a DXT surface (no DDS path yet)", bundle.format_name));
        return texture::to_dds(bundle.pixels, bundle.width, bundle.height, *bundle.dds_format);
    }
    // an XPR2 is the 360's tiled DXT: it needs the surface's dimensions, and is read as DXT5
    if (!width || !height || !*width || !*height) throw UsageError("an XPR2 needs a width and height to untile (the resource dimensions are not read)");
    const Bytes linear = texture::untile(texture::swap16(bundle.pixels), *width, *height, texture::Format::Dxt5);
    return texture::to_dds(linear, *width, *height, texture::Format::Dxt5);
}

// ---- the command line ----------------------------------------------------------------------------------------------------------

namespace {

Bundle xpr_open(const cli::Args& a) { return parse(fs::read_file(fs::from_text(a.required("file")))); }

}  // namespace

cli::Command command() {
    cli::Command c;
    c.name = "xpr";
    c.summary = "extract textures from Xbox .xpr resource bundles (XPR0 original / XPR2 360) as DDS";
    c.area = "Xbox";
    const cli::Positional file{"file", "the .xpr bundle"};
    c.subcommands.push_back(cli::Command{"info", "the container and its texture", {file}, [](const cli::Args& a) {
        const Bundle b = xpr_open(a);
        out("  {:<12} {}\n", "container", container_name(b.container));
        if (b.container == Container::Xpr0) {
            out("  {:<12} {}\n", "format_name", b.format_name);
            out("  {:<12} {}\n", "dds_fmt", b.dds_format ? texture::info(*b.dds_format).name : std::string_view("none"));
            out("  {:<12} {}\n", "width", b.width);
            out("  {:<12} {}\n", "height", b.height);
            out("  {:<12} {}\n", "mipmaps", b.mipmaps);
            out("  {:<12} {}\n", "cubemap", b.cubemap ? "true" : "false");
        }
        out("  {:<12} {}\n", "data_offset", b.data_offset);
        if (b.container == Container::Xpr2) {
            out("  {:<12} {}\n", "header_size", b.header_size);
            out("  {:<12} {}\n", "data_size", b.data_size);
        }
        return kExitOk;
    }});
    c.subcommands.push_back(cli::Command{"dds", "write the texture as a DDS",
                                         {file, cli::Positional{"out", "the .dds to write"}, cli::Positional{"width", "the surface width (an XPR2 needs it)", false},
                                          cli::Positional{"height", "the surface height (an XPR2 needs it)", false}},
                                         [](const cli::Args& a) {
        const Bundle b = xpr_open(a);
        const auto width = a.integer<std::uint32_t>("width"), height = a.integer<std::uint32_t>("height");
        const Bytes dds = to_dds(b, width, height);
        fs::write_file(fs::from_text(a.required("out")), dds);
        out("wrote {} ({} bytes, {} {}x{})\n", a.required("out"), dds.size(), b.container == Container::Xpr0 ? b.format_name : std::string("?"),
            width.value_or(b.width), height.value_or(b.height));
        return kExitOk;
    }});
    return c;
}

}  // namespace cb::formats::xbox::xpr
