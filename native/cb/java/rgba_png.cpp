#include "cb/java/rgba_png.hpp"

#include <algorithm>
#include <array>
#include <cstdlib>
#include <cstring>
#include <format>
#include <optional>
#include <vector>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "zlib.h"

namespace cb::java::rgba_png {

namespace {

constexpr std::string_view kReader = "png";
constexpr std::array<std::uint8_t, 8> kSignature = {0x89, 'P', 'N', 'G', '\r', '\n', 0x1a, '\n'};

enum ColorType : std::uint8_t { kGrey = 0, kRgb = 2, kPalette = 3, kGreyAlpha = 4, kRgba = 6 };

struct Header {
    std::uint32_t width = 0;
    std::uint32_t height = 0;
    std::uint8_t depth = 0;
    std::uint8_t color = 0;
    bool interlaced = false;

    std::size_t channels() const {
        switch (color) {
            case kRgb: return 3;
            case kGreyAlpha: return 2;
            case kRgba: return 4;
            default: return 1;
        }
    }
    std::size_t bits_per_pixel() const { return channels() * depth; }
    std::size_t bytes_per_pixel_for_filter() const { return std::max<std::size_t>(1, bits_per_pixel() / 8); }
    // The bytes of one row of `pixels` pixels (without its filter byte).
    std::size_t stride(std::size_t pixels) const { return (pixels * bits_per_pixel() + 7) / 8; }
};

std::uint32_t be32(ByteView v, std::size_t at) {
    return (std::uint32_t{v[at]} << 24) | (std::uint32_t{v[at + 1]} << 16) | (std::uint32_t{v[at + 2]} << 8) | v[at + 3];
}

void put_be32(Bytes& out, std::uint32_t v) {
    for (int shift = 24; shift >= 0; shift -= 8) out.push_back(static_cast<std::uint8_t>(v >> shift));
}

void put_chunk(Bytes& out, std::string_view type, ByteView data) {
    put_be32(out, static_cast<std::uint32_t>(data.size()));
    const std::size_t crc_from = out.size();
    append(out, as_bytes(type));
    append(out, data);
    put_be32(out, crypto::crc32(ByteView(out).subspan(crc_from)));
}

// Whether the (colour type, depth) pair is one the PNG specification allows.
bool valid_combination(std::uint8_t color, std::uint8_t depth) {
    switch (color) {
        case kGrey: return depth == 1 || depth == 2 || depth == 4 || depth == 8 || depth == 16;
        case kRgb:
        case kGreyAlpha:
        case kRgba: return depth == 8 || depth == 16;
        case kPalette: return depth == 1 || depth == 2 || depth == 4 || depth == 8;
        default: return false;
    }
}

Header read_header(ByteView data) {
    if (data.size() != 13) throw FormatError(kReader, 8, "the IHDR chunk is not 13 bytes");
    Header h;
    h.width = be32(data, 0);
    h.height = be32(data, 4);
    h.depth = data[8];
    h.color = data[9];
    if (h.width == 0 || h.height == 0 || h.width > 0x7FFFFFFFu || h.height > 0x7FFFFFFFu) {
        throw FormatError(kReader, 8, std::format("bad picture size {}x{}", h.width, h.height));
    }
    if (std::uint64_t{h.width} * h.height > kMaxPixels) {
        throw FormatError(kReader, 8, std::format("{}x{} pixels is more than the {} this reader takes", h.width, h.height, kMaxPixels));
    }
    if (!valid_combination(h.color, h.depth)) {
        throw FormatError(kReader, 16, std::format("colour type {} cannot have {} bits per sample", h.color, h.depth));
    }
    if (data[10] != 0) throw UnsupportedError(kReader, 18, std::format("compression method {}", data[10]));
    if (data[11] != 0) throw UnsupportedError(kReader, 19, std::format("filter method {}", data[11]));
    if (data[12] > 1) throw UnsupportedError(kReader, 20, std::format("interlace method {}", data[12]));
    h.interlaced = data[12] == 1;
    return h;
}

// The Adam7 passes: first column, first row, column step, row step.
struct Pass {
    std::uint32_t x0, y0, dx, dy;
};
constexpr std::array<Pass, 7> kAdam7 = {{{0, 0, 8, 8}, {4, 0, 8, 8}, {0, 4, 4, 8}, {2, 0, 4, 4}, {0, 2, 2, 4}, {1, 0, 2, 2}, {0, 1, 1, 2}}};

// The size of a pass's picture (0 when the pass is empty for a small image).
std::uint32_t pass_size(std::uint32_t total, std::uint32_t first, std::uint32_t step) {
    return total > first ? (total - first + step - 1) / step : 0;
}

std::size_t inflated_size(const Header& h) {
    if (!h.interlaced) return std::size_t{h.height} * (h.stride(h.width) + 1);
    std::size_t total = 0;
    for (const Pass& p : kAdam7) {
        const std::uint32_t pw = pass_size(h.width, p.x0, p.dx), ph = pass_size(h.height, p.y0, p.dy);
        if (pw != 0 && ph != 0) total += std::size_t{ph} * (h.stride(pw) + 1);
    }
    return total;
}

// Inflate the zlib stream into exactly `expected` bytes (more data than that is ignored).
Bytes inflate_exact(ByteView z, std::size_t expected) {
    Bytes out(expected);
    z_stream zs;
    std::memset(&zs, 0, sizeof zs);
    if (inflateInit(&zs) != Z_OK) throw InternalError("zlib: inflateInit failed");
    zs.next_in = const_cast<Bytef*>(z.data());
    zs.avail_in = static_cast<uInt>(z.size());
    zs.next_out = out.data();
    zs.avail_out = static_cast<uInt>(out.size());
    const int rc = inflate(&zs, Z_FINISH);
    const std::size_t produced = out.size() - zs.avail_out;
    inflateEnd(&zs);
    if (rc == Z_DATA_ERROR || rc == Z_MEM_ERROR || rc == Z_NEED_DICT) {
        throw FormatError(kReader, 0, "the image data is not a valid zlib stream");
    }
    if (produced < expected) throw FormatError(kReader, 0, "the image data ends early");
    return out;
}

int paeth(int a, int b, int c) {
    const int p = a + b - c;
    const int pa = std::abs(p - a), pb = std::abs(p - b), pc = std::abs(p - c);
    if (pa <= pb && pa <= pc) return a;
    return pb <= pc ? b : c;
}

// Undo the row filters of `rows` scanlines of `stride` bytes (each led by its filter byte), read from `raw` at
// `*offset` (which moves on); the plain rows are appended to `out`.
void unfilter(ByteView raw, std::size_t* offset, std::uint32_t rows, std::size_t stride, std::size_t bpp, Bytes& out) {
    Bytes prev(stride, 0);
    Bytes row(stride);
    for (std::uint32_t y = 0; y < rows; ++y) {
        const std::uint8_t type = raw[*offset];
        std::memcpy(row.data(), &raw[*offset + 1], stride);
        *offset += stride + 1;
        for (std::size_t i = 0; i < stride; ++i) {
            const int a = i >= bpp ? row[i - bpp] : 0;
            const int b = prev[i];
            const int c = i >= bpp ? prev[i - bpp] : 0;
            int x = row[i];
            switch (type) {
                case 0: break;
                case 1: x += a; break;
                case 2: x += b; break;
                case 3: x += (a + b) / 2; break;
                case 4: x += paeth(a, b, c); break;
                default: throw FormatError(kReader, 0, std::format("unknown row filter type {}", type));
            }
            row[i] = static_cast<std::uint8_t>(x & 0xFF);
        }
        append(out, row);
        prev = row;
    }
}

// What the palette and tRNS chunks said.
struct Colors {
    Bytes palette;      // R, G, B per entry
    Bytes palette_alpha;   // alpha per entry (shorter than the palette: the rest are opaque)
    bool has_key = false;  // a colour key for grey / RGB images: that colour is transparent
    std::uint16_t key[3] = {0, 0, 0};
};

// Sample number `index` of a row: 1 to 16 bits wide.
std::uint16_t sample_at(const std::uint8_t* row, std::size_t index, unsigned depth) {
    switch (depth) {
        case 8: return row[index];
        case 16: return static_cast<std::uint16_t>((row[2 * index] << 8) | row[2 * index + 1]);
        default: {
            const std::size_t bit = index * depth;
            const unsigned shift = 8 - depth - static_cast<unsigned>(bit % 8);
            return static_cast<std::uint16_t>((row[bit / 8] >> shift) & ((1u << depth) - 1));
        }
    }
}

// A sample as 8 bits: 16-bit samples keep their high byte, narrow grey samples are spread over 0..255.
std::uint8_t to_8bit(std::uint16_t v, unsigned depth) {
    if (depth == 8) return static_cast<std::uint8_t>(v);
    if (depth == 16) return static_cast<std::uint8_t>(v >> 8);
    return static_cast<std::uint8_t>(v * 255 / ((1u << depth) - 1));
}

void convert_pixel(const Header& h, const Colors& colors, const std::uint8_t* row, std::size_t x, std::uint8_t* out) {
    const unsigned depth = h.depth;
    switch (h.color) {
        case kGrey: {
            const std::uint16_t g = sample_at(row, x, depth);
            out[0] = out[1] = out[2] = to_8bit(g, depth);
            out[3] = colors.has_key && g == colors.key[0] ? 0 : 255;
            break;
        }
        case kRgb: {
            const std::uint16_t r = sample_at(row, 3 * x, depth), g = sample_at(row, 3 * x + 1, depth), b = sample_at(row, 3 * x + 2, depth);
            out[0] = to_8bit(r, depth);
            out[1] = to_8bit(g, depth);
            out[2] = to_8bit(b, depth);
            out[3] = colors.has_key && r == colors.key[0] && g == colors.key[1] && b == colors.key[2] ? 0 : 255;
            break;
        }
        case kPalette: {
            const std::size_t index = sample_at(row, x, depth);
            if (index * 3 + 2 >= colors.palette.size()) {
                throw FormatError(kReader, 0, std::format("palette index {} is outside the {}-entry palette", index, colors.palette.size() / 3));
            }
            std::memcpy(out, &colors.palette[index * 3], 3);
            out[3] = index < colors.palette_alpha.size() ? colors.palette_alpha[index] : 255;
            break;
        }
        case kGreyAlpha:
            out[0] = out[1] = out[2] = to_8bit(sample_at(row, 2 * x, depth), depth);
            out[3] = to_8bit(sample_at(row, 2 * x + 1, depth), depth);
            break;
        default:   // kRgba
            for (std::size_t k = 0; k < 4; ++k) out[k] = to_8bit(sample_at(row, 4 * x + k, depth), depth);
            break;
    }
}

}  // namespace

Image blank(std::uint32_t width, std::uint32_t height) {
    Image img;
    img.width = width;
    img.height = height;
    img.rgba.assign(std::size_t{width} * height * 4, 0);
    return img;
}

Image decode(ByteView png) {
    if (png.size() < kSignature.size() || !std::equal(kSignature.begin(), kSignature.end(), png.begin())) {
        throw FormatError(kReader, 0, "not a PNG file (bad signature)");
    }
    std::optional<Header> header;
    Colors colors;
    Bytes idat;
    std::size_t pos = kSignature.size();
    while (pos + 12 <= png.size()) {
        const std::uint32_t length = be32(png, pos);
        if (length > png.size() - pos - 12) throw FormatError(kReader, pos, "a chunk runs past the end of the file");
        const std::string_view type = as_text(png.subspan(pos + 4, 4));
        const ByteView data = png.subspan(pos + 8, length);
        if (crypto::crc32(png.subspan(pos + 4, 4 + std::size_t{length})) != be32(png, pos + 8 + length)) {
            throw FormatError(kReader, pos, std::format("the CRC of the {} chunk does not match", type));
        }
        if (type == "IHDR") {
            header = read_header(data);
        } else if (type == "PLTE") {
            colors.palette.assign(data.begin(), data.end() - data.size() % 3);
        } else if (type == "tRNS" && header) {
            if (header->color == kPalette) {
                colors.palette_alpha.assign(data.begin(), data.end());
            } else if (header->color == kGrey && data.size() >= 2) {
                colors.has_key = true;
                colors.key[0] = static_cast<std::uint16_t>((data[0] << 8) | data[1]);
            } else if (header->color == kRgb && data.size() >= 6) {
                colors.has_key = true;
                for (std::size_t k = 0; k < 3; ++k) colors.key[k] = static_cast<std::uint16_t>((data[2 * k] << 8) | data[2 * k + 1]);
            }
        } else if (type == "IDAT") {
            append(idat, data);
        } else if (type == "IEND") {
            break;
        }
        pos += 12 + std::size_t{length};
    }
    if (!header) throw FormatError(kReader, 8, "no IHDR chunk");
    if (idat.empty()) throw FormatError(kReader, png.size(), "no image data");

    const Header& h = *header;
    const Bytes raw = inflate_exact(idat, inflated_size(h));
    const std::size_t bpp = h.bytes_per_pixel_for_filter();
    Image img = blank(h.width, h.height);

    auto place = [&](const Bytes& plain, std::uint32_t pw, std::uint32_t ph, const Pass& p) {
        const std::size_t stride = h.stride(pw);
        for (std::uint32_t y = 0; y < ph; ++y) {
            const std::uint8_t* row = &plain[y * stride];
            for (std::uint32_t x = 0; x < pw; ++x) {
                convert_pixel(h, colors, row, x, img.pixel(p.x0 + x * p.dx, p.y0 + y * p.dy));
            }
        }
    };
    std::size_t offset = 0;
    if (!h.interlaced) {
        Bytes plain;
        plain.reserve(std::size_t{h.height} * h.stride(h.width));
        unfilter(raw, &offset, h.height, h.stride(h.width), bpp, plain);
        place(plain, h.width, h.height, Pass{0, 0, 1, 1});
    } else {
        for (const Pass& p : kAdam7) {
            const std::uint32_t pw = pass_size(h.width, p.x0, p.dx), ph = pass_size(h.height, p.y0, p.dy);
            if (pw == 0 || ph == 0) continue;
            Bytes plain;
            unfilter(raw, &offset, ph, h.stride(pw), bpp, plain);
            place(plain, pw, ph, p);
        }
    }
    return img;
}

Bytes encode(const Image& image) {
    CB_ASSERT(image.rgba.size() == std::size_t{image.width} * image.height * 4, "the pixel buffer does not match the size");
    Bytes scan;
    scan.reserve(image.rgba.size() + image.height);
    for (std::uint32_t y = 0; y < image.height; ++y) {
        scan.push_back(0);   // filter type 0: the row as it is
        if (image.width != 0) {
            const std::uint8_t* row = image.pixel(0, y);
            scan.insert(scan.end(), row, row + std::size_t{image.width} * 4);
        }
    }
    z_stream zs;
    std::memset(&zs, 0, sizeof zs);
    if (deflateInit(&zs, Z_DEFAULT_COMPRESSION) != Z_OK) throw InternalError("zlib: deflateInit failed");
    Bytes z(deflateBound(&zs, static_cast<uLong>(scan.size())));
    zs.next_in = scan.data();
    zs.avail_in = static_cast<uInt>(scan.size());
    zs.next_out = z.data();
    zs.avail_out = static_cast<uInt>(z.size());
    const int rc = deflate(&zs, Z_FINISH);
    z.resize(z.size() - zs.avail_out);
    deflateEnd(&zs);
    if (rc != Z_STREAM_END) throw InternalError("zlib: deflate did not finish");

    Bytes out(kSignature.begin(), kSignature.end());
    Bytes ihdr;
    put_be32(ihdr, image.width);
    put_be32(ihdr, image.height);
    for (const std::uint8_t b : std::array<std::uint8_t, 5>{8, 6, 0, 0, 0}) ihdr.push_back(b);   // 8 bits, RGBA, deflate, filter 0, no interlace
    put_chunk(out, "IHDR", ihdr);
    put_chunk(out, "IDAT", z);
    put_chunk(out, "IEND", {});
    return out;
}

Image crop(const Image& image, std::int64_t left, std::int64_t top, std::int64_t right, std::int64_t bottom) {
    if (right <= left || bottom <= top) return blank(0, 0);
    Image out = blank(static_cast<std::uint32_t>(right - left), static_cast<std::uint32_t>(bottom - top));
    for (std::uint32_t y = 0; y < out.height; ++y) {
        for (std::uint32_t x = 0; x < out.width; ++x) {
            const std::int64_t sx = left + x, sy = top + y;
            if (sx >= 0 && sx < image.width && sy >= 0 && sy < image.height) {
                std::memcpy(out.pixel(x, y), image.pixel(static_cast<std::uint32_t>(sx), static_cast<std::uint32_t>(sy)), 4);
            }
        }
    }
    return out;
}

Image resize_nearest(const Image& image, std::uint32_t size) {
    if (size == 0) throw UsageError("resize_nearest: the size must be at least 1");
    Image out = blank(size, size);
    if (image.width == 0 || image.height == 0) return out;
    const double sx = static_cast<double>(image.width) / size, sy = static_cast<double>(image.height) / size;
    // The source position of an output pixel is accumulated from half a step, not computed as (i + 0.5) * step:
    // the roundings differ, and this is how Pillow's nearest scale does it.
    std::vector<std::uint32_t> xin(size), yin(size);
    double xo = sx * 0.5, yo = sy * 0.5;
    for (std::uint32_t i = 0; i < size; ++i) {
        xin[i] = std::min(static_cast<std::uint32_t>(xo), image.width - 1);
        yin[i] = std::min(static_cast<std::uint32_t>(yo), image.height - 1);
        xo += sx;
        yo += sy;
    }
    for (std::uint32_t y = 0; y < size; ++y) {
        for (std::uint32_t x = 0; x < size; ++x) std::memcpy(out.pixel(x, y), image.pixel(xin[x], yin[y]), 4);
    }
    return out;
}

}  // namespace cb::java::rgba_png
