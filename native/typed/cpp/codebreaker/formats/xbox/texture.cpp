// codebreaker/formats/xbox/texture.cpp -- typed C++ of codebreaker.formats.xbox.texture (see texture.hpp).
//
// The layout and decoding code is that of native/formats/xbox/graphics.cc (its texture section), whose
// functions are private to that file; the exceptions it raised are the Python's.

#include "codebreaker/formats/xbox/texture.hpp"

#include <algorithm>
#include <cstring>

#include "../../../../../formats/core/bytes.h"
#include "../../../../../formats/core/text.h"
#include "../../../../../formats/xbox/pyfmt.h"

namespace codebreaker::formats::xbox::texture {

namespace {

namespace cb = ::cbf;

// the exception an answer carried: native._exception(kind, message)
[[noreturn]] void raise_py(const cb::PyError& e) {
    const std::string& k = e.kind;
    const std::string& m = e.message;
    if (k == "struct.error") throw py::StructError(m);
    if (k == "KeyError") {
        size_t a = m.find_first_not_of('\'');                // message.strip("'"), then str(KeyError(t)) is its repr
        size_t b = m.find_last_not_of('\'');
        throw py::KeyError(py::repr(py::str(a == std::string::npos ? std::string() : m.substr(a, b - a + 1))));
    }
    if (k == "ValueError") throw py::ValueError(m);
    if (k == "IndexError") throw py::IndexError(m);
    if (k == "MemoryError") throw py::MemoryError(m);
    if (k == "OverflowError") throw py::OverflowError(m);
    throw py::RuntimeError(m);
}

template <class F>
auto native(F&& f) -> decltype(f()) {
    try {
        return f();
    } catch (const cb::PyError& e) {
        raise_py(e);
    }
}

struct Fmt {
    int64_t block_bytes, log2bpp;
    const char* fourcc;
};

const Fmt& texture_format(const py::str& name_s) {
    static const Fmt dxt1{8, 3, "DXT1"}, dxt3{16, 4, "DXT3"}, dxt5{16, 4, "DXT5"};
    const std::string name = name_s.utf8();
    if (name == "dxt1") return dxt1;
    if (name == "dxt3") return dxt3;
    if (name == "dxt5") return dxt5;
    cb::raise("KeyError", "'" + name + "'");
}

int64_t fdiv(int64_t a, int64_t b) { return a / b - ((a % b != 0) && ((a < 0) != (b < 0))); }
int64_t blocks_of(int64_t v) { return fdiv(v + 3, 4); }

void check_alloc(int64_t n) {
    if (n < 0) cb::raise("ValueError", "negative count");
    if (n > (int64_t(1) << 34)) cb::raise("MemoryError", "");
}

// out[dst:dst+n] = data[src:src+n], as a bytearray does it (a short slice shortens the array)
void put_block(std::string& out, int64_t dst, cb::Bytes data, int64_t src, int64_t n) {
    cb::Bytes b = data.slice(src, src + n);
    if (b.size() == n && dst >= 0 && dst + n <= int64_t(out.size())) std::memcpy(&out[size_t(dst)], b.data(), size_t(n));
    else cb::xbox::slice_assign(out, dst, dst + n, b.str());
}

std::string swap16_of(cb::Bytes d) {
    if (d.size() % 2)
        cb::raise("ValueError", "attempt to assign bytes of size " + cb::dec(d.size() / 2) + " to extended slice of size " + cb::dec((d.size() + 1) / 2));
    std::string b = d.str();
    for (size_t i = 0; i + 1 < b.size(); i += 2) std::swap(b[i], b[i + 1]);
    return b;
}

struct Rgb {
    int r, g, b;
};
Rgb rgb565(int c) { return {((c >> 11) & 0x1F) * 255 / 31, ((c >> 5) & 0x3F) * 255 / 63, (c & 0x1F) * 255 / 31}; }

}  // namespace

// ------------------------------------------------------------------------------------------ constants
const py::dict<py::str, std::tuple<int64_t, int64_t, py::bytes>> FORMATS = {
    {py::str("dxt1"), std::make_tuple(int64_t(8), int64_t(3), py::bytes("DXT1"))},
    {py::str("dxt3"), std::make_tuple(int64_t(16), int64_t(4), py::bytes("DXT3"))},
    {py::str("dxt5"), std::make_tuple(int64_t(16), int64_t(4), py::bytes("DXT5"))},
};

const py::str DESCRIPTION = py::str("Xbox 360 texture untile, DDS export and DXT decoding (DXT1/3/5)");

// ------------------------------------------------------------------------------------------ the layout
int64_t tiled_offset_2d(int64_t x, int64_t y, int64_t width_blocks, int64_t log2bpp) {
    int64_t aligned_width = (width_blocks + 31) & ~int64_t(31);
    int64_t macro = py::shl((x >> 5) + (y >> 5) * (aligned_width >> 5), log2bpp + 7);
    int64_t micro = py::shl((x & 7) + ((y & 6) << 2), log2bpp);
    int64_t offset = macro + ((micro & ~int64_t(0xF)) << 1) + (micro & 0xF) + py::shl(y & 8, 3 + log2bpp) + ((y & 1) << 4);
    return ((offset & ~int64_t(0x1FF)) << 3) + ((y & 16) << 7) + ((offset & 0x1C0) << 2) + (((((y & 8) >> 2) + (x >> 3)) & 3) << 6) + (offset & 0x3F);
}

std::tuple<int64_t, int64_t> _blocks(int64_t w, int64_t h) {
    int64_t bw = fdiv(w + 3, 4);
    int64_t bh = fdiv(h + 3, 4);
    return std::make_tuple(bw, bh);
}

py::bytes untile(const py::bytes& data_b, int64_t w, int64_t h, const py::str& fmt) {
    return native([&] {
        const cb::Bytes data(data_b.raw());
        const Fmt& f = texture_format(fmt);
        int64_t bw = blocks_of(w), bh = blocks_of(h);
        check_alloc(bw * bh * f.block_bytes);
        std::string out(size_t(bw * bh * f.block_bytes), '\0');
        for (int64_t y = 0; y < bh; ++y)
            for (int64_t x = 0; x < bw; ++x) put_block(out, (y * bw + x) * f.block_bytes, data, tiled_offset_2d(x, y, bw, f.log2bpp), f.block_bytes);
        return py::bytes(out);
    });
}

py::bytes tile(const py::bytes& data_b, int64_t w, int64_t h, const py::str& fmt) {
    return native([&] {
        const cb::Bytes data(data_b.raw());
        const Fmt& f = texture_format(fmt);
        int64_t bw = blocks_of(w), bh = blocks_of(h), max_off = 0;
        for (int64_t y = 0; y < bh; ++y)
            for (int64_t x = 0; x < bw; ++x) max_off = std::max(max_off, tiled_offset_2d(x, y, bw, f.log2bpp));
        check_alloc(max_off + f.block_bytes);
        std::string out(size_t(max_off + f.block_bytes), '\0');
        for (int64_t y = 0; y < bh; ++y)
            for (int64_t x = 0; x < bw; ++x) put_block(out, tiled_offset_2d(x, y, bw, f.log2bpp), data, (y * bw + x) * f.block_bytes, f.block_bytes);
        return py::bytes(out);
    });
}

py::bytes swap16(const py::bytes& data) {
    return native([&] { return py::bytes(swap16_of(cb::Bytes(data.raw()))); });
}

py::bytes dds_header(int64_t w, int64_t h, const py::str& fmt) {
    return native([&] {
        const Fmt& f = texture_format(fmt);
        int64_t linear = blocks_of(w) * blocks_of(h) * f.block_bytes;
        std::string o = "DDS ";
        cb::xbox::pack_uint(o, 124, 4, false);
        cb::xbox::pack_uint(o, 0x1 | 0x2 | 0x4 | 0x1000 | 0x80000, 4, false);
        cb::xbox::pack_uint(o, h, 4, false);
        cb::xbox::pack_uint(o, w, 4, false);
        cb::xbox::pack_uint(o, linear, 4, false);
        cb::xbox::pack_uint(o, 0, 4, false);
        cb::xbox::pack_uint(o, 0, 4, false);
        o.append(44, '\0');
        cb::xbox::pack_uint(o, 32, 4, false);
        cb::xbox::pack_uint(o, 0x4, 4, false);
        o += f.fourcc;
        o.append(20, '\0');
        cb::xbox::pack_uint(o, 0x1000, 4, false);
        o.append(16, '\0');
        return py::bytes(o);
    });
}

py::bytes to_dds(const py::bytes& linear, int64_t w, int64_t h, const py::str& fmt) { return dds_header(w, h, fmt) + linear; }

Image decode_dxt(const py::bytes& data_b, int64_t w, int64_t h, const py::str& fmt, bool tiled, bool swapped, bool force_opaque) {
    int64_t width = 0, height = 0;
    std::string rgba;
    native([&] {
        const cb::Bytes data(data_b.raw());
        const Fmt& f = texture_format(fmt);
        const bool is_dxt1 = fmt == py::str("dxt1"), is_dxt5 = fmt == py::str("dxt5");
        int64_t bw = blocks_of(w), bh = blocks_of(h);
        check_alloc(bw * 4 * bh * 4 * 4);
        width = bw * 4;
        height = bh * 4;
        rgba.assign(size_t(bw * 4 * bh * 4 * 4), '\0');
        const int64_t stride = bw * 4 * 4;
        for (int64_t by = 0; by < bh; ++by)
            for (int64_t bx = 0; bx < bw; ++bx) {
                int64_t o = tiled ? tiled_offset_2d(bx, by, bw, f.log2bpp) : (by * bw + bx) * f.block_bytes;
                cb::Bytes raw = data.slice(o, o + f.block_bytes);
                if (raw.size() < f.block_bytes) continue;
                std::string blk = swapped ? swap16_of(raw) : raw.str();
                const uint8_t* b = reinterpret_cast<const uint8_t*>(blk.data());
                auto color_block = [&](const uint8_t* q, bool dxt1, Rgb out[16]) {
                    int c0 = q[0] | q[1] << 8, c1 = q[2] | q[3] << 8;
                    uint32_t idx = uint32_t(q[4]) | uint32_t(q[5]) << 8 | uint32_t(q[6]) << 16 | uint32_t(q[7]) << 24;
                    Rgb col[4] = {rgb565(c0), rgb565(c1), {0, 0, 0}, {0, 0, 0}};
                    if (!dxt1 || c0 > c1) {
                        col[2] = {(2 * col[0].r + col[1].r) / 3, (2 * col[0].g + col[1].g) / 3, (2 * col[0].b + col[1].b) / 3};
                        col[3] = {(col[0].r + 2 * col[1].r) / 3, (col[0].g + 2 * col[1].g) / 3, (col[0].b + 2 * col[1].b) / 3};
                    } else {
                        col[2] = {(col[0].r + col[1].r) / 2, (col[0].g + col[1].g) / 2, (col[0].b + col[1].b) / 2};
                    }
                    for (int i = 0; i < 16; ++i) out[i] = col[(idx >> (2 * i)) & 3];
                };
                Rgb cols[16];
                int alpha[16];
                if (is_dxt1) {
                    color_block(b, true, cols);
                    for (int i = 0; i < 16; ++i) alpha[i] = 255;
                    if ((b[0] | b[1] << 8) <= (b[2] | b[3] << 8)) {
                        uint32_t idx = uint32_t(b[4]) | uint32_t(b[5]) << 8 | uint32_t(b[6]) << 16 | uint32_t(b[7]) << 24;
                        for (int i = 0; i < 16; ++i) alpha[i] = ((idx >> (2 * i)) & 3) == 3 ? 0 : 255;
                    }
                } else {
                    color_block(b + 8, false, cols);
                    if (force_opaque) {
                        for (int i = 0; i < 16; ++i) alpha[i] = 255;
                    } else if (is_dxt5) {
                        int a0 = b[0], a1 = b[1], pal[8] = {a0, a1, 0, 0, 0, 0, 0, 0};
                        uint64_t bits = 0;
                        for (int k = 5; k >= 0; --k) bits = bits << 8 | b[2 + k];
                        if (a0 > a1) {
                            for (int k = 1; k < 7; ++k) pal[1 + k] = ((7 - k) * a0 + k * a1) / 7;
                        } else {
                            for (int k = 1; k < 5; ++k) pal[1 + k] = ((5 - k) * a0 + k * a1) / 5;
                            pal[6] = 0;
                            pal[7] = 255;
                        }
                        for (int i = 0; i < 16; ++i) alpha[i] = pal[(bits >> (3 * i)) & 7];
                    } else {
                        uint64_t bits = 0;
                        for (int k = 7; k >= 0; --k) bits = bits << 8 | b[k];
                        for (int i = 0; i < 16; ++i) alpha[i] = int((bits >> (4 * i)) & 0xF) * 17;
                    }
                }
                for (int i = 0; i < 16; ++i) {
                    int64_t p = (by * 4 + i / 4) * stride + (bx * 4 + i % 4) * 4;
                    rgba[size_t(p)] = char(cols[i].r);
                    rgba[size_t(p + 1)] = char(cols[i].g);
                    rgba[size_t(p + 2)] = char(cols[i].b);
                    rgba[size_t(p + 3)] = char(alpha[i]);
                }
            }
        return 0;
    });
    // Image.frombytes("RGBA", (width, height), rgba).crop((0, 0, w, h)) -- Pillow's checks, then the cut
    if (width < 0 || height < 0) throw py::ValueError("Width and height must be >= 0");
    if (w < 0) throw py::ValueError("Coordinate 'right' is less than 'left'");
    if (h < 0) throw py::ValueError("Coordinate 'lower' is less than 'upper'");
    Image im;
    im.width = w;
    im.height = h;
    std::string out(size_t(w * h * 4), '\0');
    for (int64_t y = 0; y < h; ++y) std::memcpy(&out[size_t(y * w * 4)], &rgba[size_t(y * width * 4)], size_t(w * 4));
    im.pixels = py::bytes(out);
    return im;
}

// ------------------------------------------------------------------------ CLI
void main(const py::list<py::str>& sys_argv) {
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
    py::file f = py::open_rb(inp);
    py::bytes data = f.read(-1);
    py::bytes linear = op == py::str("untile") ? untile(data, w, h, fmt) : data;
    if (swap) linear = swap16(linear);
    {
        py::file o = py::open(out, py::str("wb"));
        o.write(to_dds(linear, w, h, fmt));
        o.close();
    }
    py::print(py::percent(py::str("%s %dx%d %s -> %s (%d bytes)"), op, w, h, fmt, out, py::os::path::getsize(out)));
}

}  // namespace codebreaker::formats::xbox::texture
