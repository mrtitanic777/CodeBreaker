#include "cb/formats/xbox/texture.hpp"

#include <algorithm>
#include <array>
#include <cstring>

#include "cb/core/error.hpp"
#include "cb/core/io.hpp"

namespace cb::formats::xbox::texture {

namespace {

constexpr std::string_view kReader = "xbox.texture";
constexpr std::uint64_t kMaxSurface = std::uint64_t{1} << 34;   // 16 GB: more than any surface

void check_size(std::uint64_t bytes) {
    if (bytes > kMaxSurface) throw UnsupportedError(kReader, 0, "a surface of more than 16 GB");
}

struct Rgb {
    int r, g, b;
};
Rgb rgb565(int c) { return {((c >> 11) & 0x1F) * 255 / 31, ((c >> 5) & 0x3F) * 255 / 63, (c & 0x1F) * 255 / 31}; }

Bytes swapped16(ByteView d) {
    if (d.size() % 2) throw FormatError(kReader, d.size(), "an odd number of bytes cannot be 16-bit words");
    Bytes b(d.begin(), d.end());
    for (std::size_t i = 0; i + 1 < b.size(); i += 2) std::swap(b[i], b[i + 1]);
    return b;
}

// Copy `n` bytes of `data` from `src` to `out` at `dst`, as far as the data goes (the rest of a block past its end stays 0).
void put_block(Bytes& out, std::uint64_t dst, ByteView data, std::uint64_t src, std::uint64_t n) {
    if (src >= data.size()) return;
    const std::uint64_t have = std::min<std::uint64_t>(n, data.size() - src);
    std::memcpy(out.data() + dst, data.data() + src, static_cast<std::size_t>(have));
}

}  // namespace

const FormatInfo& info(Format f) {
    static const FormatInfo dxt1{8, 3, "DXT1", "dxt1"}, dxt3{16, 4, "DXT3", "dxt3"}, dxt5{16, 4, "DXT5", "dxt5"};
    switch (f) {
        case Format::Dxt1: return dxt1;
        case Format::Dxt3: return dxt3;
        case Format::Dxt5: return dxt5;
    }
    return dxt1;
}

std::optional<Format> parse_format(std::string_view name) {
    if (name == "dxt1") return Format::Dxt1;
    if (name == "dxt3") return Format::Dxt3;
    if (name == "dxt5") return Format::Dxt5;
    return std::nullopt;
}

BlockSize blocks(std::uint32_t width, std::uint32_t height) {
    return {static_cast<std::uint32_t>((std::uint64_t{width} + 3) / 4), static_cast<std::uint32_t>((std::uint64_t{height} + 3) / 4)};
}

std::uint64_t tiled_offset_2d(std::uint32_t x, std::uint32_t y, std::uint32_t width_blocks, unsigned log2_block_bytes) {
    const std::uint64_t aligned_width = (std::uint64_t{width_blocks} + 31) & ~std::uint64_t{31};
    const std::uint64_t macro = ((x >> 5) + std::uint64_t{y >> 5} * (aligned_width >> 5)) << (log2_block_bytes + 7);
    const std::uint64_t micro = ((x & 7) + std::uint64_t{(y & 6) << 2}) << log2_block_bytes;
    const std::uint64_t offset = macro + ((micro & ~std::uint64_t{0xF}) << 1) + (micro & 0xF) +
                                 (std::uint64_t{y & 8} << (3 + log2_block_bytes)) + (std::uint64_t{y & 1} << 4);
    return ((offset & ~std::uint64_t{0x1FF}) << 3) + (std::uint64_t{y & 16} << 7) + ((offset & 0x1C0) << 2) +
           (((((y & 8) >> 2) + (x >> 3)) & 3) << 6) + (offset & 0x3F);
}

Bytes untile(ByteView data, std::uint32_t width, std::uint32_t height, Format format) {
    const FormatInfo& f = info(format);
    const BlockSize bs = blocks(width, height);
    check_size(std::uint64_t{bs.width} * bs.height * f.block_bytes);
    Bytes out(std::size_t{bs.width} * bs.height * f.block_bytes, 0);
    for (std::uint32_t y = 0; y < bs.height; ++y) {
        for (std::uint32_t x = 0; x < bs.width; ++x) {
            put_block(out, (std::uint64_t{y} * bs.width + x) * f.block_bytes, data,
                      tiled_offset_2d(x, y, bs.width, f.log2_block_bytes), f.block_bytes);
        }
    }
    return out;
}

Bytes tile(ByteView linear, std::uint32_t width, std::uint32_t height, Format format) {
    const FormatInfo& f = info(format);
    const BlockSize bs = blocks(width, height);
    std::uint64_t max_offset = 0;
    for (std::uint32_t y = 0; y < bs.height; ++y) {
        for (std::uint32_t x = 0; x < bs.width; ++x) max_offset = std::max(max_offset, tiled_offset_2d(x, y, bs.width, f.log2_block_bytes));
    }
    check_size(max_offset + f.block_bytes);
    Bytes out(static_cast<std::size_t>(max_offset + f.block_bytes), 0);
    for (std::uint32_t y = 0; y < bs.height; ++y) {
        for (std::uint32_t x = 0; x < bs.width; ++x) {
            put_block(out, tiled_offset_2d(x, y, bs.width, f.log2_block_bytes), linear, (std::uint64_t{y} * bs.width + x) * f.block_bytes,
                      f.block_bytes);
        }
    }
    return out;
}

Bytes swap16(ByteView data) { return swapped16(data); }

Bytes dds_header(std::uint32_t width, std::uint32_t height, Format format) {
    const FormatInfo& f = info(format);
    const BlockSize bs = blocks(width, height);
    const std::uint64_t linear_size = std::uint64_t{bs.width} * bs.height * f.block_bytes;
    if (linear_size > 0xFFFFFFFFull) throw UnsupportedError(kReader, 0, "a surface of more than 4 GB has no DDS header");
    io::ByteWriter w;
    w.bytes(as_bytes("DDS "));
    w.u32(124);                                        // header size
    w.u32(0x1 | 0x2 | 0x4 | 0x1000 | 0x80000);         // CAPS | HEIGHT | WIDTH | PIXELFORMAT | LINEARSIZE
    w.u32(height);
    w.u32(width);
    w.u32(static_cast<std::uint32_t>(linear_size));
    w.u32(0);                                          // depth
    w.u32(0);                                          // mip levels
    w.zeros(44);                                       // reserved
    w.u32(32);                                         // pixel format size
    w.u32(0x4);                                        // FOURCC
    w.bytes(as_bytes(f.fourcc));
    w.zeros(20);                                       // bit counts and masks
    w.u32(0x1000);                                     // caps: texture
    w.zeros(16);
    return w.take();
}

Bytes to_dds(ByteView linear, std::uint32_t width, std::uint32_t height, Format format) {
    Bytes out = dds_header(width, height, format);
    append(out, linear);
    return out;
}

Image decode_dxt(ByteView data, std::uint32_t width, std::uint32_t height, Format format, const DecodeOptions& options) {
    const FormatInfo& f = info(format);
    const BlockSize bs = blocks(width, height);
    const std::uint64_t padded_width = std::uint64_t{bs.width} * 4;
    const std::uint64_t padded_height = std::uint64_t{bs.height} * 4;
    check_size(padded_width * padded_height * 4);
    Bytes padded(static_cast<std::size_t>(padded_width * padded_height * 4), 0);
    const std::size_t stride = static_cast<std::size_t>(padded_width * 4);

    // 4 colours of a colour block from its two 565 endpoints; DXT1 with c0 <= c1 has a third at the midpoint and a clear fourth
    auto color_block = [](const std::uint8_t* q, bool dxt1, Rgb out[16]) {
        const int c0 = q[0] | q[1] << 8, c1 = q[2] | q[3] << 8;
        const std::uint32_t idx = std::uint32_t(q[4]) | std::uint32_t(q[5]) << 8 | std::uint32_t(q[6]) << 16 | std::uint32_t(q[7]) << 24;
        Rgb col[4] = {rgb565(c0), rgb565(c1), {0, 0, 0}, {0, 0, 0}};
        if (!dxt1 || c0 > c1) {
            col[2] = {(2 * col[0].r + col[1].r) / 3, (2 * col[0].g + col[1].g) / 3, (2 * col[0].b + col[1].b) / 3};
            col[3] = {(col[0].r + 2 * col[1].r) / 3, (col[0].g + 2 * col[1].g) / 3, (col[0].b + 2 * col[1].b) / 3};
        } else {
            col[2] = {(col[0].r + col[1].r) / 2, (col[0].g + col[1].g) / 2, (col[0].b + col[1].b) / 2};
        }
        for (int i = 0; i < 16; ++i) out[i] = col[(idx >> (2 * i)) & 3];
    };

    for (std::uint32_t by = 0; by < bs.height; ++by) {
        for (std::uint32_t bx = 0; bx < bs.width; ++bx) {
            const std::uint64_t at = options.tiled ? tiled_offset_2d(bx, by, bs.width, f.log2_block_bytes)
                                                   : (std::uint64_t{by} * bs.width + bx) * f.block_bytes;
            if (at + f.block_bytes > data.size()) continue;   // beyond the data: transparent black
            std::array<std::uint8_t, 16> blk{};
            std::memcpy(blk.data(), data.data() + at, f.block_bytes);
            if (options.swapped) {
                for (std::uint32_t i = 0; i + 1 < f.block_bytes; i += 2) std::swap(blk[i], blk[i + 1]);
            }
            const std::uint8_t* b = blk.data();
            Rgb cols[16];
            int alpha[16];
            if (format == Format::Dxt1) {
                color_block(b, true, cols);
                for (int& a : alpha) a = 255;
                if ((b[0] | b[1] << 8) <= (b[2] | b[3] << 8)) {   // c0 <= c1: index 3 is clear
                    const std::uint32_t idx = std::uint32_t(b[4]) | std::uint32_t(b[5]) << 8 | std::uint32_t(b[6]) << 16 | std::uint32_t(b[7]) << 24;
                    for (int i = 0; i < 16; ++i) alpha[i] = ((idx >> (2 * i)) & 3) == 3 ? 0 : 255;
                }
            } else {
                color_block(b + 8, false, cols);
                if (options.force_opaque) {
                    for (int& a : alpha) a = 255;
                } else if (format == Format::Dxt5) {
                    const int a0 = b[0], a1 = b[1];
                    int pal[8] = {a0, a1, 0, 0, 0, 0, 0, 0};
                    std::uint64_t bits = 0;
                    for (int k = 5; k >= 0; --k) bits = bits << 8 | b[2 + k];
                    if (a0 > a1) {
                        for (int k = 1; k < 7; ++k) pal[1 + k] = ((7 - k) * a0 + k * a1) / 7;
                    } else {
                        for (int k = 1; k < 5; ++k) pal[1 + k] = ((5 - k) * a0 + k * a1) / 5;
                        pal[6] = 0;
                        pal[7] = 255;
                    }
                    for (int i = 0; i < 16; ++i) alpha[i] = pal[(bits >> (3 * i)) & 7];
                } else {   // DXT3: 4 bits of alpha each
                    std::uint64_t bits = 0;
                    for (int k = 7; k >= 0; --k) bits = bits << 8 | b[k];
                    for (int i = 0; i < 16; ++i) alpha[i] = static_cast<int>((bits >> (4 * i)) & 0xF) * 17;
                }
            }
            for (int i = 0; i < 16; ++i) {
                const std::size_t p = (std::size_t{by} * 4 + static_cast<std::size_t>(i / 4)) * stride + (std::size_t{bx} * 4 + static_cast<std::size_t>(i % 4)) * 4;
                padded[p] = static_cast<std::uint8_t>(cols[i].r);
                padded[p + 1] = static_cast<std::uint8_t>(cols[i].g);
                padded[p + 2] = static_cast<std::uint8_t>(cols[i].b);
                padded[p + 3] = static_cast<std::uint8_t>(alpha[i]);
            }
        }
    }
    // the blocks cover up to three more columns and rows than the image: cut them off
    Image im;
    im.width = width;
    im.height = height;
    im.rgba.resize(std::size_t{width} * height * 4);
    for (std::uint32_t y = 0; y < height; ++y) {
        std::memcpy(im.rgba.data() + std::size_t{y} * width * 4, padded.data() + std::size_t{y} * stride, std::size_t{width} * 4);
    }
    return im;
}

}  // namespace cb::formats::xbox::texture
