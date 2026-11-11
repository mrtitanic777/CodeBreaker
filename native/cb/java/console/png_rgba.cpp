#include "cb/java/console/png_rgba.hpp"

#include <algorithm>
#include <array>
#include <cstdlib>
#include <cstring>

#include <zlib.h>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/io.hpp"

namespace cb::java::console::png {

namespace {

constexpr std::array<std::uint8_t, 8> kSignature = {0x89, 'P', 'N', 'G', '\r', '\n', 0x1a, '\n'};
constexpr std::string_view kReader = "png";

[[noreturn]] void unidentified() { throw FormatError(kReader, 0, "cannot identify image file"); }
[[noreturn]] void damaged(std::uint64_t at, const char* what) { throw FormatError(kReader, at, what); }

std::uint32_t be32(ByteView d, std::size_t at) {
    return (static_cast<std::uint32_t>(d[at]) << 24) | (static_cast<std::uint32_t>(d[at + 1]) << 16) |
           (static_cast<std::uint32_t>(d[at + 2]) << 8) | d[at + 3];
}

bool has_signature(ByteView d) { return d.size() >= 8 && std::memcmp(d.data(), kSignature.data(), 8) == 0; }

void put_be32(Bytes& out, std::uint32_t v) {
    for (int k = 3; k >= 0; --k) out.push_back(static_cast<std::uint8_t>((v >> (8 * k)) & 0xFF));
}

void put_chunk(Bytes& out, std::string_view type, ByteView body) {
    put_be32(out, static_cast<std::uint32_t>(body.size()));
    Bytes typed(type.begin(), type.end());
    append(typed, body);
    append(out, typed);
    put_be32(out, crypto::crc32(typed));
}

// The value of sample number `idx` of an unfiltered row: a 1/2/4-bit sample, a byte, or a 16-bit word.
unsigned sample_at(ByteView row, int depth, std::size_t idx) {
    if (depth == 8) return row[idx];
    if (depth == 16) return (static_cast<unsigned>(row[2 * idx]) << 8) | row[2 * idx + 1];
    const std::size_t bit = idx * static_cast<std::size_t>(depth);
    return (static_cast<unsigned>(row[bit / 8]) >> (8 - depth - bit % 8)) & ((1u << depth) - 1);
}

int paeth(int a, int b, int c) {
    const int p = a + b - c, pa = std::abs(p - a), pb = std::abs(p - b), pc = std::abs(p - c);
    if (pa <= pb && pa <= pc) return a;
    return pb <= pc ? b : c;
}

struct Chunk {
    std::string_view type;
    std::size_t data = 0;    // offset of the body
    std::uint32_t length = 0;
    bool crc_ok = false;
};

// Reads the chunks of a PNG one after the other.
class Chunks {
public:
    explicit Chunks(ByteView d) : d_(d) {}
    std::size_t pos() const { return pos_; }
    void rewind(std::size_t pos) { pos_ = pos; }
    // false: not even a chunk header is left. `cut` is set when the body runs past the end of the data.
    bool next(Chunk& c, bool& cut) {
        cut = false;
        if (pos_ + 8 > d_.size()) return false;
        c.length = be32(d_, pos_);
        c.type = std::string_view(reinterpret_cast<const char*>(d_.data() + pos_ + 4), 4);
        c.data = pos_ + 8;
        if (c.length > 0x7fffffffu || c.data + c.length + 4 > d_.size()) {
            cut = true;
            return true;
        }
        c.crc_ok = crypto::crc32(d_.subspan(pos_ + 4, 4 + static_cast<std::size_t>(c.length))) == be32(d_, c.data + c.length);
        pos_ = c.data + c.length + 4;
        return true;
    }

private:
    ByteView d_;
    std::size_t pos_ = 8;
};

Bytes inflate_exact(ByteView stream, std::size_t size) {
    Bytes raw(size);
    z_stream zs;
    std::memset(&zs, 0, sizeof zs);
    if (inflateInit(&zs) != Z_OK) damaged(0, "zlib error");
    zs.next_in = const_cast<Bytef*>(stream.data());
    zs.avail_in = static_cast<uInt>(stream.size());
    zs.next_out = raw.data();
    zs.avail_out = static_cast<uInt>(raw.size());
    const int rc = raw.empty() ? Z_STREAM_END : inflate(&zs, Z_FINISH);
    const std::size_t got = raw.size() - zs.avail_out;
    inflateEnd(&zs);
    // Z_BUF_ERROR with a full buffer: more data than the image needs, which is ignored
    if (got != raw.size() || (rc != Z_STREAM_END && rc != Z_BUF_ERROR && rc != Z_OK)) {
        damaged(0, got != raw.size() && (rc == Z_BUF_ERROR || rc == Z_OK || rc == Z_STREAM_END)
                       ? "image file is truncated"
                       : "broken data stream when reading image file");
    }
    return raw;
}

Bytes deflate_all(ByteView scan) {
    z_stream zs;
    std::memset(&zs, 0, sizeof zs);
    if (deflateInit(&zs, Z_DEFAULT_COMPRESSION) != Z_OK) damaged(0, "zlib error");
    Bytes z(deflateBound(&zs, static_cast<uLong>(scan.size())));
    zs.next_in = const_cast<Bytef*>(scan.data());
    zs.avail_in = static_cast<uInt>(scan.size());
    zs.next_out = z.data();
    zs.avail_out = static_cast<uInt>(z.size());
    const int rc = deflate(&zs, Z_FINISH);
    z.resize(z.size() - zs.avail_out);
    deflateEnd(&zs);
    if (rc != Z_STREAM_END) damaged(0, "zlib error");
    return z;
}

struct Pass {
    std::uint32_t x0, y0, dx, dy;
};

Image decode(ByteView d) {
    if (!has_signature(d)) unidentified();
    Chunks chunks(d);
    Chunk c;
    bool cut = false;
    if (!chunks.next(c, cut) || cut || c.type != "IHDR" || c.length != 13 || !c.crc_ok) unidentified();
    const std::uint32_t w = be32(d, c.data), h = be32(d, c.data + 4);
    const int depth = d[c.data + 8], ct = d[c.data + 9];
    const int filter_method = d[c.data + 11];
    const int interlace = d[c.data + 12];
    {
        static constexpr int kModes[][2] = {{0, 1}, {0, 2}, {0, 4}, {0, 8}, {0, 16}, {2, 8}, {2, 16}, {3, 1},
                                            {3, 2}, {3, 4}, {3, 8}, {4, 8}, {4, 16}, {6, 8}, {6, 16}};
        bool ok = false;
        for (const auto& m : kModes) ok = ok || (m[0] == ct && m[1] == depth);
        // (the compression method byte is not looked at)
        if (!ok || w == 0 || h == 0 || w > 0x7fffffffu || h > 0x7fffffffu || filter_method != 0) unidentified();
    }
    Bytes plte, trns;
    bool has_trns = false, saw_idat = false;
    for (;;) {   // the chunks up to the first IDAT
        const std::size_t before = chunks.pos();
        if (!chunks.next(c, cut)) unidentified();
        if (cut) damaged(before, "Truncated File Read");
        if (c.type == "IDAT") {
            chunks.rewind(before);
            saw_idat = true;
            break;
        }
        if (c.type == "IEND") break;
        if (c.type == "PLTE" || c.type == "tRNS") {
            if (!c.crc_ok) unidentified();
            Bytes body(d.begin() + static_cast<std::ptrdiff_t>(c.data), d.begin() + static_cast<std::ptrdiff_t>(c.data + c.length));
            if (c.type == "PLTE") plte = std::move(body);
            else {
                trns = std::move(body);
                has_trns = true;
            }
        }
    }
    if (!saw_idat) damaged(chunks.pos(), "cannot load this image");
    if (has_trns && ((ct == 0 && trns.size() < 2) || (ct == 2 && trns.size() < 6))) unidentified();
    Bytes idat;
    for (;;) {   // the IDAT chunks, one after the other
        const std::size_t before = chunks.pos();
        if (!chunks.next(c, cut) || c.type != "IDAT") {
            chunks.rewind(before);
            break;
        }
        if (cut || !c.crc_ok) damaged(before, "broken data stream when reading image file");
        append(idat, d.subspan(c.data, c.length));
    }

    // the passes: (x0, y0, dx, dy), or the whole image
    std::vector<Pass> passes;
    if (interlace != 0) passes = {{0, 0, 8, 8}, {4, 0, 8, 8}, {0, 4, 4, 8}, {2, 0, 4, 4}, {0, 2, 2, 4}, {1, 0, 2, 2}, {0, 1, 1, 2}};
    else passes = {{0, 0, 1, 1}};
    const int channels = ct == 0 ? 1 : ct == 2 ? 3 : ct == 3 ? 1 : ct == 4 ? 2 : 4;
    const std::size_t bytes_per_pixel = std::max<std::size_t>(1, static_cast<std::size_t>(channels * depth / 8));
    auto row_bytes = [&](std::uint32_t pass_width) {
        return (static_cast<std::size_t>(pass_width) * static_cast<std::size_t>(channels) * static_cast<std::size_t>(depth) + 7) / 8;
    };
    std::size_t total = 0;
    for (const Pass& p : passes) {
        if (w <= p.x0 || h <= p.y0) continue;
        const std::uint32_t pw = (w + p.dx - 1 - p.x0) / p.dx, ph = (h + p.dy - 1 - p.y0) / p.dy;
        total += static_cast<std::size_t>(ph) * (1 + row_bytes(pw));
    }
    const Bytes raw = inflate_exact(idat, total);

    Bytes pixels(static_cast<std::size_t>(w) * h * 4);
    std::size_t at = 0;
    for (const Pass& p : passes) {
        if (w <= p.x0 || h <= p.y0) continue;
        const std::uint32_t pw = (w + p.dx - 1 - p.x0) / p.dx, ph = (h + p.dy - 1 - p.y0) / p.dy;
        const std::size_t rb = row_bytes(pw);
        Bytes prev(rb, 0), row(rb, 0);
        for (std::uint32_t y = 0; y < ph; ++y) {
            const int filter = raw[at++];
            if (filter > 4) damaged(at, "unrecognized data stream contents when reading image file");
            for (std::size_t x = 0; x < rb; ++x) {
                const int rv = raw[at + x];
                const int a = x >= bytes_per_pixel ? row[x - bytes_per_pixel] : 0;
                const int b = prev[x];
                const int cc = x >= bytes_per_pixel ? prev[x - bytes_per_pixel] : 0;
                const int v = filter == 0 ? rv : filter == 1 ? rv + a : filter == 2 ? rv + b : filter == 3 ? rv + ((a + b) >> 1) : rv + paeth(a, b, cc);
                row[x] = static_cast<std::uint8_t>(v & 0xFF);
            }
            at += rb;
            for (std::uint32_t i = 0; i < pw; ++i) {
                unsigned r, g, bl, a = 255;
                const std::size_t s = static_cast<std::size_t>(i) * static_cast<std::size_t>(channels);
                if (ct == 0) {
                    const unsigned v = sample_at(row, depth, s);
                    const unsigned l = depth == 1 ? (v ? 255u : 0u) : depth == 2 ? v * 85 : depth == 4 ? v * 17 : depth == 8 ? v : std::min(v, 255u);
                    if (has_trns) {
                        const unsigned t16 = (static_cast<unsigned>(trns[0]) << 8) | trns[1];
                        const unsigned tl = depth == 1 ? (t16 ? 255u : 0u) : (t16 & 0xFF);
                        if (l == tl) a = 0;
                    }
                    r = g = bl = l;
                } else if (ct == 2) {
                    unsigned rr = sample_at(row, depth, s), gg = sample_at(row, depth, s + 1), bb = sample_at(row, depth, s + 2);
                    if (depth == 16) {
                        rr >>= 8;
                        gg >>= 8;
                        bb >>= 8;
                    }
                    if (has_trns && rr == unsigned{trns[1]} && gg == unsigned{trns[3]} && bb == unsigned{trns[5]}) a = 0;
                    r = rr;
                    g = gg;
                    bl = bb;
                } else if (ct == 3) {
                    const unsigned idx = sample_at(row, depth, s);
                    if ((idx + 1) * 3 <= plte.size()) {
                        r = plte[idx * 3];
                        g = plte[idx * 3 + 1];
                        bl = plte[idx * 3 + 2];
                    } else {
                        r = g = bl = 0;   // an index past the palette is black
                    }
                    if (has_trns && idx < trns.size()) a = trns[idx];
                } else if (ct == 4) {
                    unsigned l = sample_at(row, depth, s), al = sample_at(row, depth, s + 1);
                    if (depth == 16) {
                        l >>= 8;
                        al >>= 8;
                    }
                    r = g = bl = l;
                    a = al;
                } else {   // 6: RGBA (only the 16-bit form reaches here; the 8-bit form is never converted)
                    const int shift = depth == 16 ? 8 : 0;
                    r = sample_at(row, depth, s) >> shift;
                    g = sample_at(row, depth, s + 1) >> shift;
                    bl = sample_at(row, depth, s + 2) >> shift;
                    a = sample_at(row, depth, s + 3) >> shift;
                }
                const std::size_t o = (static_cast<std::size_t>(p.y0 + y * p.dy) * w + (p.x0 + i * p.dx)) * 4;
                pixels[o] = static_cast<std::uint8_t>(r);
                pixels[o + 1] = static_cast<std::uint8_t>(g);
                pixels[o + 2] = static_cast<std::uint8_t>(bl);
                pixels[o + 3] = static_cast<std::uint8_t>(a);
            }
            prev = row;
        }
    }

    return Image{w, h, std::move(pixels)};
}

}  // namespace

Bytes encode_rgba(const Image& image) {
    const std::uint32_t w = image.width, h = image.height;
    if (image.rgba.size() != static_cast<std::size_t>(w) * h * 4) throw InternalError("png: pixel data does not match the size");
    Bytes scan;
    scan.reserve(image.rgba.size() + h);
    for (std::uint32_t y = 0; y < h; ++y) {
        scan.push_back(0);   // filter type 0: each row as it is
        append(scan, ByteView(image.rgba).subspan(static_cast<std::size_t>(y) * w * 4, static_cast<std::size_t>(w) * 4));
    }
    Bytes out(kSignature.begin(), kSignature.end());
    Bytes ihdr;
    put_be32(ihdr, w);
    put_be32(ihdr, h);
    for (int b : {8, 6, 0, 0, 0}) ihdr.push_back(static_cast<std::uint8_t>(b));   // 8-bit, RGBA, deflate, filter 0, no interlace
    put_chunk(out, "IHDR", ihdr);
    put_chunk(out, "IDAT", deflate_all(scan));
    put_chunk(out, "IEND", {});
    return out;
}

Image decode_rgba(ByteView png) { return decode(png); }

std::optional<std::uint8_t> color_type(ByteView d) {
    if (d.size() < 26 || !has_signature(d)) return std::nullopt;
    return d[25];
}

bool needs_rgba(ByteView d) {
    const std::optional<std::uint8_t> ct = color_type(d);
    return ct && *ct != 6;
}

Converted normalize_to_rgba(ByteView d) {
    if (!needs_rgba(d)) return {Bytes(d.begin(), d.end()), false};
    return {encode_rgba(decode(d)), true};
}

}  // namespace cb::java::console::png
