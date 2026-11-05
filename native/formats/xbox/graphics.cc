// graphics.cc -- the Xbox 360 texture layout and DDS (texture), XPR bundles (xpr), Scaleform GFx movies (gfx)
// and XNA content (xnb):  codebreaker/formats/xbox/{texture,xpr}.py, codebreaker/formats/{gfx,xnb}.py.
// What needs Pillow, fontTools, ffmpeg or the file system stays in those modules; the bytes they are
// made of are here.
#include <algorithm>
#include <array>
#include <cstring>
#include <map>
#include <optional>
#include <set>

#include "../core/build.h"
#include "../core/questions.h"
#include "../core/text.h"
#include "../lzx/lzx.h"
#include "pyfmt.h"
#include "zlib.h"

namespace cbf::xbox {
namespace {

// ================================================================================ texture
struct Fmt { int64_t block_bytes, log2bpp; const char *fourcc; };

const Fmt &texture_format(const std::string &name) {
    static const Fmt dxt1{8, 3, "DXT1"}, dxt3{16, 4, "DXT3"}, dxt5{16, 4, "DXT5"};
    if (name == "dxt1") return dxt1;
    if (name == "dxt3") return dxt3;
    if (name == "dxt5") return dxt5;
    raise("KeyError", "'" + name + "'");
}

int64_t tiled_offset(int64_t x, int64_t y, int64_t width_blocks, int64_t log2bpp) {
    int64_t aligned_width = (width_blocks + 31) & ~int64_t(31);
    int64_t macro = ((x >> 5) + (y >> 5) * (aligned_width >> 5)) << (log2bpp + 7);
    int64_t micro = ((x & 7) + ((y & 6) << 2)) << log2bpp;
    int64_t offset = macro + ((micro & ~int64_t(0xF)) << 1) + (micro & 0xF) + ((y & 8) << (3 + log2bpp)) + ((y & 1) << 4);
    return ((offset & ~int64_t(0x1FF)) << 3) + ((y & 16) << 7) + ((offset & 0x1C0) << 2) +
           (((((y & 8) >> 2) + (x >> 3)) & 3) << 6) + (offset & 0x3F);
}

int64_t fdiv(int64_t a, int64_t b) { return a / b - ((a % b != 0) && ((a < 0) != (b < 0))); }
int64_t blocks_of(int64_t v) { return fdiv(v + 3, 4); }

void check_alloc(int64_t n) {
    if (n < 0) raise("ValueError", "negative count");
    if (n > (int64_t(1) << 34)) raise("MemoryError", "");
}

// out[dst:dst+n] = data[src:src+n], as a bytearray does it (a short slice shortens the array)
void put_block(std::string &out, int64_t dst, Bytes data, int64_t src, int64_t n) {
    Bytes b = data.slice(src, src + n);
    if (b.size() == n && dst >= 0 && dst + n <= int64_t(out.size())) std::memcpy(&out[size_t(dst)], b.data(), size_t(n));
    else slice_assign(out, dst, dst + n, b.str());
}

std::string untile(Bytes data, int64_t w, int64_t h, const std::string &fmt) {
    const Fmt &f = texture_format(fmt);
    int64_t bw = blocks_of(w), bh = blocks_of(h);
    check_alloc(bw * bh * f.block_bytes);
    std::string out(size_t(bw * bh * f.block_bytes), '\0');
    for (int64_t y = 0; y < bh; ++y)
        for (int64_t x = 0; x < bw; ++x)
            put_block(out, (y * bw + x) * f.block_bytes, data, tiled_offset(x, y, bw, f.log2bpp), f.block_bytes);
    return out;
}

std::string tile(Bytes data, int64_t w, int64_t h, const std::string &fmt) {
    const Fmt &f = texture_format(fmt);
    int64_t bw = blocks_of(w), bh = blocks_of(h), max_off = 0;
    for (int64_t y = 0; y < bh; ++y)
        for (int64_t x = 0; x < bw; ++x) max_off = std::max(max_off, tiled_offset(x, y, bw, f.log2bpp));
    check_alloc(max_off + f.block_bytes);
    std::string out(size_t(max_off + f.block_bytes), '\0');
    for (int64_t y = 0; y < bh; ++y)
        for (int64_t x = 0; x < bw; ++x)
            put_block(out, tiled_offset(x, y, bw, f.log2bpp), data, (y * bw + x) * f.block_bytes, f.block_bytes);
    return out;
}

std::string swap16(Bytes d) {
    if (d.size() % 2) raise("ValueError", "attempt to assign bytes of size " + dec(d.size() / 2) + " to extended slice of size " + dec((d.size() + 1) / 2));
    std::string b = d.str();
    for (size_t i = 0; i + 1 < b.size(); i += 2) std::swap(b[i], b[i + 1]);
    return b;
}

std::string dds_header(int64_t w, int64_t h, const std::string &fmt) {
    const Fmt &f = texture_format(fmt);
    int64_t linear = blocks_of(w) * blocks_of(h) * f.block_bytes;
    std::string o = "DDS ";
    pack_uint(o, 124, 4, false);
    pack_uint(o, 0x1 | 0x2 | 0x4 | 0x1000 | 0x80000, 4, false);
    pack_uint(o, h, 4, false);
    pack_uint(o, w, 4, false);
    pack_uint(o, linear, 4, false);
    pack_uint(o, 0, 4, false);
    pack_uint(o, 0, 4, false);
    o.append(44, '\0');
    pack_uint(o, 32, 4, false);
    pack_uint(o, 0x4, 4, false);
    o += f.fourcc;
    o.append(20, '\0');
    pack_uint(o, 0x1000, 4, false);
    o.append(16, '\0');
    return o;
}

void texture_tiled_offset(Call &c) {
    c.out.integer(tiled_offset(c.int_arg(0), c.int_arg(1), c.int_arg(2), c.int_arg(3)));
}
void texture_untile(Call &c) { c.out.bytes(Bytes(untile(c.data, c.int_arg(0), c.int_arg(1), c.str_arg(2)))); }
void texture_tile(Call &c) { c.out.bytes(Bytes(tile(c.data, c.int_arg(0), c.int_arg(1), c.str_arg(2)))); }
void texture_swap16(Call &c) { c.out.bytes(Bytes(swap16(c.data))); }
void texture_dds_header(Call &c) { c.out.bytes(Bytes(dds_header(c.int_arg(0), c.int_arg(1), c.str_arg(2)))); }

struct Rgb { int r, g, b; };
Rgb rgb565(int c) { return {((c >> 11) & 0x1F) * 255 / 31, ((c >> 5) & 0x3F) * 255 / 63, (c & 0x1F) * 255 / 31}; }

// texture.decode_rgba(data, w, h, fmt, tiled, swapped, force_opaque) -> (width, height, RGBA bytes of the whole blocks)
void texture_decode_rgba(Call &c) {
    Bytes data = c.data;
    int64_t w = c.int_arg(0), h = c.int_arg(1);
    std::string fmt = c.str_arg(2);
    bool tiled = c.bool_arg(3, true), swapped = c.bool_arg(4, true), force_opaque = c.bool_arg(5, false);
    const Fmt &f = texture_format(fmt);
    int64_t bw = blocks_of(w), bh = blocks_of(h);
    check_alloc(bw * 4 * bh * 4 * 4);
    std::string rgba(size_t(bw * 4 * bh * 4 * 4), '\0');
    const int64_t stride = bw * 4 * 4;
    for (int64_t by = 0; by < bh; ++by)
        for (int64_t bx = 0; bx < bw; ++bx) {
            int64_t o = tiled ? tiled_offset(bx, by, bw, f.log2bpp) : (by * bw + bx) * f.block_bytes;
            Bytes raw = data.slice(o, o + f.block_bytes);
            if (raw.size() < f.block_bytes) continue;
            std::string blk = swapped ? swap16(raw) : raw.str();
            const uint8_t *b = reinterpret_cast<const uint8_t *>(blk.data());
            auto color_block = [&](const uint8_t *q, bool dxt1, Rgb out[16]) {
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
            if (fmt == "dxt1") {
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
                } else if (fmt == "dxt5") {
                    int a0 = b[0], a1 = b[1], pal[8] = {a0, a1, 0, 0, 0, 0, 0, 0};
                    uint64_t bits = 0;
                    for (int k = 5; k >= 0; --k) bits = bits << 8 | b[2 + k];
                    if (a0 > a1) for (int k = 1; k < 7; ++k) pal[1 + k] = ((7 - k) * a0 + k * a1) / 7;
                    else {
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
    Out &o = c.out;
    o.begin_tuple();
    o.integer(bw * 4);
    o.integer(bh * 4);
    o.bytes(Bytes(rgba));
    o.end_tuple();
}

// ================================================================================ xpr
// xpr.parse(data) -> the dict, or XprError
void xpr_parse(Call &c) {
    Bytes d = c.data;
    Value r = vdict();
    if (d.slice_is(0, 4, "XPR0", 4)) {
        if (d.size() < 0x1C) raise("XprError", "not an XPR0 bundle");
        int64_t total = d.u32(4), header_size = d.u32(8), fmtdw = d.u32(0x0C + 12);
        d.u32(0x0C); d.u32(0x10); d.u32(0x14);
        int64_t xfmt = (fmtdw >> 8) & 0xFF, mips = (fmtdw >> 16) & 0xF, log2w = (fmtdw >> 20) & 0xF, log2h = (fmtdw >> 24) & 0xF;
        bool cubemap = (fmtdw & 0x4) != 0;
        int64_t off = header_size ? header_size : 0x1C;
        Bytes pixels = (total && total <= d.size()) ? d.slice(off, total) : d.slice(off);
        static const std::map<int64_t, const char *> names = {{0x00, "L8"}, {0x01, "AL8"}, {0x02, "A1R5G5B5"}, {0x05, "R5G6B5"},
            {0x06, "A8R8G8B8"}, {0x07, "X8R8G8B8"}, {0x0B, "P8"}, {0x0C, "DXT1"}, {0x0E, "DXT3"}, {0x0F, "DXT5"},
            {0x19, "A8"}, {0x1A, "A8L8"}, {0x3F, "LIN_A8R8G8B8"}};
        auto it = names.find(xfmt);
        std::string name = it != names.end() ? it->second : std::string("0x") + (xfmt < 16 ? "0" : "") + hex(uint64_t(xfmt), true);
        put(r, "container", vstr("XPR0"));
        put(r, "xfmt", vint(xfmt));
        put(r, "format_name", vstr(name));
        put(r, "dds_fmt", xfmt == 0x0C ? vstr("dxt1") : xfmt == 0x0E ? vstr("dxt3") : xfmt == 0x0F ? vstr("dxt5") : vnone());
        put(r, "width", vint(int64_t(1) << log2w));
        put(r, "height", vint(int64_t(1) << log2h));
        put(r, "mipmaps", vint(mips));
        put(r, "cubemap", vbool(cubemap));
        put(r, "data_offset", vint(off));
        put(r, "pixels", vbytes(pixels));
        put(r, "total", vint(total ? total : d.size()));
    } else if (d.slice_is(0, 4, "XPR2", 4)) {
        if (d.size() < 0x10) raise("XprError", "not an XPR2 bundle");
        int64_t header_size = d.be32(4), data_size = d.be32(8), off = header_size;
        Bytes pixels = data_size ? d.slice(off, off + data_size) : d.slice(off);
        put(r, "container", vstr("XPR2"));
        put(r, "header_size", vint(header_size));
        put(r, "data_size", vint(data_size ? data_size : pixels.size()));
        put(r, "data_offset", vint(off));
        put(r, "pixels", vbytes(pixels));
    } else {
        raise("XprError", "unknown magic " + bytes_repr(d.slice(0, 4)) + " (expected XPR0 or XPR2)");
    }
    emit(c.out, r);
}

// ================================================================================ gfx
struct Bits {
    Bytes d;
    int64_t p;
    int bit = 0;
    Bits(Bytes data, int64_t at) : d(data), p(at) {}
    uint64_t read(int64_t n) {
        uint64_t v = 0;
        for (int64_t k = 0; k < n; ++k) {
            v = (v << 1) | ((d.at(p) >> (7 - bit)) & 1);
            if (++bit == 8) { bit = 0; ++p; }
        }
        return v;
    }
    int64_t reads(int64_t n) {
        uint64_t v = read(n);
        return (n && (v >> (n - 1))) ? int64_t(v) - (int64_t(1) << n) : int64_t(v);
    }
    void align() { if (bit) { bit = 0; ++p; } }
};

struct Tag { int64_t code; Bytes body; };

std::vector<Tag> read_tags(Bytes body, int64_t p) {
    std::vector<Tag> tags;
    while (p + 2 <= body.size()) {
        int64_t tc = body.u16(p);
        p += 2;
        int64_t code = tc >> 6, length = tc & 0x3F;
        if (length == 0x3F) { length = body.u32(p); p += 4; }
        tags.push_back({code, body.slice(p, p + length)});
        p += length;
        if (code == 0) break;
    }
    return tags;
}

std::vector<Tag> parse_tags(Bytes body) {
    Bits br(body, 0);
    int64_t nb = int64_t(br.read(5));
    for (int k = 0; k < 4; ++k) br.read(nb);
    br.align();
    return read_tags(body, br.p + 4);
}

bool inflate_all(Bytes in, std::string &out) {
    z_stream zs;
    std::memset(&zs, 0, sizeof zs);
    if (inflateInit(&zs) != Z_OK) return false;
    zs.next_in = const_cast<Bytef *>(in.data());
    zs.avail_in = uInt(in.size());
    char buf[1 << 16];
    int rc;
    do {
        zs.next_out = reinterpret_cast<Bytef *>(buf);
        zs.avail_out = sizeof buf;
        rc = inflate(&zs, Z_NO_FLUSH);
        if (rc != Z_OK && rc != Z_STREAM_END) { inflateEnd(&zs); return false; }
        out.append(buf, sizeof buf - zs.avail_out);
    } while (rc != Z_STREAM_END && (zs.avail_in || zs.avail_out == 0));
    inflateEnd(&zs);
    return rc == Z_STREAM_END;
}

struct Movie { int64_t offset; std::string magic, body; };

std::vector<Movie> find_movies(Bytes data) {
    std::vector<Movie> out;
    const int64_t n = data.size();
    int64_t pos = 0;
    for (;;) {
        int64_t a = data.find("GFX", 3, pos), b = data.find("CFX", 3, pos);
        int64_t i = a < 0 ? b : b < 0 ? a : std::min(a, b);
        if (i < 0) break;
        pos = i + 3;
        if (i + 8 > n) continue;
        int64_t ver = data.at(i + 3);
        if (!(1 <= ver && ver <= 20)) continue;
        int64_t flen = data.u32(i + 4);
        if (!(8 <= flen && flen <= n - i)) continue;
        std::string body = data.slice(i + 8, i + 8 + flen).str();
        if (data.slice_is(i, i + 3, "CFX", 3)) {
            std::string un;
            if (!inflate_all(Bytes(body), un)) continue;
            body = un;
        }
        try {
            bool z = false, one = false;
            for (auto &t : parse_tags(Bytes(body))) { if (t.code == 0) z = true; if (t.code == 1) one = true; }
            if (!z || !one) continue;
        } catch (const PyError &) {
            continue;
        }
        out.push_back({i, data.slice(i, i + 3).str(), body});
    }
    return out;
}

void gfx_find_movies(Call &c) {
    Out &o = c.out;
    o.begin_list();
    for (auto &m : find_movies(c.data)) {
        o.begin_tuple();
        o.integer(m.offset);
        o.text(m.magic);
        o.bytes(Bytes(m.body));
        o.end_tuple();
    }
    o.end_list();
}

// gfx.wrapper_name(data, off)
void gfx_wrapper_name(Call &c) {
    int64_t off = c.int_arg(0);
    Bytes chunk = c.data.slice(std::max<int64_t>(0, off - 512), off);
    std::vector<std::string> runs;
    std::string cur;
    for (const uint8_t *p = chunk.begin(); p != chunk.end(); ++p) {
        if (*p >= 0x20 && *p <= 0x7e) cur += char(*p);
        else { if (cur.size() >= 2) runs.push_back(cur); cur.clear(); }
    }
    if (cur.size() >= 2) runs.push_back(cur);
    for (auto it = runs.rbegin(); it != runs.rend(); ++it) {
        std::string t = *it;
        size_t a = t.find_first_not_of(' '), z = t.find_last_not_of(' ');
        t = a == std::string::npos ? "" : t.substr(a, z - a + 1);
        if (t.size() < 2 || t.size() > 48) continue;
        bool ok = true, alpha = false;
        std::set<char> uniq;
        for (char ch : t) {
            bool l = (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z');
            if (l) alpha = true;
            if (!(l || (ch >= '0' && ch <= '9') || ch == '_' || ch == '.' || ch == '-')) ok = false;
            uniq.insert(ch);
        }
        if (ok && alpha && uniq.size() > 1) { c.out.text(t); return; }
    }
    c.out.text("");
}

void gfx_parse_tags(Call &c) {
    Out &o = c.out;
    o.begin_list();
    for (auto &t : parse_tags(c.data)) {
        o.begin_tuple();
        o.integer(t.code);
        o.bytes(t.body);
        o.end_tuple();
    }
    o.end_list();
}

void gfx_ext_images(Call &c) {
    Out &o = c.out;
    o.begin_list();
    for (auto &t : parse_tags(c.data)) {
        if (!(t.code == 1001 || t.code == 1008 || t.code == 1009) || t.body.size() < 10) continue;
        const Bytes &d = t.body;
        int64_t nlen = d.at(8);
        o.begin_tuple();
        o.integer(d.u16(0));
        o.integer(d.u16(4));
        o.integer(d.u16(6));
        o.text(latin1(d.slice(9, 9 + nlen)));
        o.end_tuple();
    }
    o.end_list();
}

void gfx_fonts(Call &c) {
    Out &o = c.out;
    o.begin_list();
    for (auto &t : parse_tags(c.data)) {
        if (!(t.code == 48 || t.code == 75) || t.body.size() < 6) continue;
        const Bytes &d = t.body;
        int64_t fid = d.u16(0), flags = d.at(2), nlen = d.at(4);
        Bytes nm = d.slice(5, 5 + nlen);
        int64_t z = nm.find_byte(0);
        if (z >= 0) nm = nm.slice(0, z);
        int64_t ng = d.u16(5 + nlen);
        o.begin_tuple();
        o.integer(t.code == 48 ? 2 : 3);
        o.integer(fid);
        o.text(latin1(nm));
        o.integer(ng);
        o.boolean((flags & 1) != 0);
        o.boolean((flags & 0x80) != 0);
        o.end_tuple();
    }
    o.end_list();
}

void gfx_placements(Call &c) {
    Out &o = c.out;
    o.begin_list();
    for (auto &t : parse_tags(c.data)) {
        if (t.code != 26 && t.code != 70) continue;
        try {
            const Bytes &d = t.body;
            int64_t flags = d.at(0), p = t.code == 70 ? 2 : 1;
            int64_t depth = d.u16(p);
            p += 2;
            std::optional<int64_t> cid;
            if (flags & 0x02) { cid = d.u16(p); p += 2; }
            double sx = 1.0, sy = 1.0, tx = 0, ty = 0;
            bool mat = (flags & 0x04) != 0;
            if (mat) {
                Bits br(d, p);
                if (br.read(1)) { int64_t n = int64_t(br.read(5)); sx = double(br.read(n)) / 65536.0; sy = double(br.read(n)) / 65536.0; }
                if (br.read(1)) { int64_t n = int64_t(br.read(5)); br.read(n); br.read(n); }
                int64_t n = int64_t(br.read(5));
                tx = double(br.reads(n)) / 20.0;
                ty = double(br.reads(n)) / 20.0;
            }
            o.begin_tuple();
            o.integer(depth);
            if (cid) o.integer(*cid); else o.none();
            if (mat) { o.begin_tuple(); o.real(sx); o.real(sy); o.real(tx); o.real(ty); o.end_tuple(); }
            else o.none();
            o.end_tuple();
        } catch (const PyError &) {
        }
    }
    o.end_list();
}

// gfx.glyph_contours(data, start) -> [[("m", x, y) | ("l", x, y) | ("q", cx, cy, x, y)]]
void gfx_glyph_contours(Call &c) {
    Bits br(c.data, c.int_arg(0));
    int64_t nfill = int64_t(br.read(4)), nline = int64_t(br.read(4)), x = 0, y = 0;
    struct Op { char op; int64_t v[4]; };
    std::vector<std::vector<Op>> contours;
    std::vector<Op> cur;
    bool have = false;
    for (;;) {
        if (br.read(1) == 0) {
            int64_t f = int64_t(br.read(5));
            if (f == 0) break;
            if (f & 0x01) {
                int64_t mb = int64_t(br.read(5));
                x = br.reads(mb);
                y = br.reads(mb);
                if (have && !cur.empty()) contours.push_back(cur);
                cur = {Op{'m', {x, y, 0, 0}}};
                have = true;
            }
            if (f & 0x02) br.read(nfill);
            if (f & 0x04) br.read(nfill);
            if (f & 0x08) br.read(nline);
            if (f & 0x10) break;
        } else if (br.read(1)) {
            int64_t nb = int64_t(br.read(4)) + 2, dx, dy;
            if (br.read(1)) { dx = br.reads(nb); dy = br.reads(nb); }
            else if (br.read(1)) { dx = 0; dy = br.reads(nb); }
            else { dx = br.reads(nb); dy = 0; }
            x += dx;
            y += dy;
            if (!have) raise("AttributeError", "'NoneType' object has no attribute 'append'");
            cur.push_back(Op{'l', {x, y, 0, 0}});
        } else {
            int64_t nb = int64_t(br.read(4)) + 2;
            int64_t cdx = br.reads(nb), cdy = br.reads(nb), adx = br.reads(nb), ady = br.reads(nb);
            int64_t cx = x + cdx, cy = y + cdy;
            x = cx + adx;
            y = cy + ady;
            if (!have) raise("AttributeError", "'NoneType' object has no attribute 'append'");
            cur.push_back(Op{'q', {cx, cy, x, y}});
        }
    }
    if (have && !cur.empty()) contours.push_back(cur);
    Out &o = c.out;
    o.begin_list();
    for (auto &ct : contours) {
        o.begin_list();
        for (auto &op : ct) {
            o.begin_tuple();
            o.text(std::string(1, op.op));
            int n = op.op == 'q' ? 4 : 2;
            for (int k = 0; k < n; ++k) o.integer(op.v[k]);
            o.end_tuple();
        }
        o.end_list();
    }
    o.end_list();
}

// ================================================================================ xnb
// a 7-bit encoded int, Python's own (unbounded): `value` is saturated for use as a size, `exact` its bytes (little-endian)
struct Seven { uint64_t value; int64_t o; std::vector<uint8_t> exact; };

Seven sevenbit_exact(Bytes b, int64_t o) {
    Seven r{0, o, {}};
    int shift = 0;
    for (;;) {
        uint8_t x = b.at(r.o++);
        size_t byte = size_t(shift / 8), sh = size_t(shift % 8);
        if (r.exact.size() < byte + 2) r.exact.resize(byte + 2, 0);
        r.exact[byte] |= uint8_t((x & 0x7F) << sh);
        r.exact[byte + 1] |= uint8_t((x & 0x7F) >> (8 - sh));
        if (shift >= 56) r.value = uint64_t(1) << 60;       // (past this it is a size no file holds)
        else r.value |= uint64_t(x & 0x7F) << shift;
        if (!(x & 0x80)) return r;
        shift += 7;
    }
}

std::pair<uint64_t, int64_t> sevenbit(Bytes b, int64_t o) {
    Seven r = sevenbit_exact(b, o);
    return {r.value, r.o};
}

std::pair<std::string, int64_t> xstring(Bytes b, int64_t o) {
    auto n = sevenbit(b, o);
    o = n.second;
    return {utf8_replace(b.slice(o, o + int64_t(n.first))), o + int64_t(n.first)};
}

std::string deframe(Bytes raw, int64_t size) {
    int64_t pos = 14;
    std::string chunks;
    while (pos < size) {
        int64_t hi = raw.at(pos), lo = raw.at(pos + 1);
        pos += 2;
        int64_t block = hi << 8 | lo, frame = 0x8000;
        if (hi == 0xFF) {
            frame = lo << 8 | raw.at(pos);
            block = int64_t(raw.at(pos + 1)) << 8 | raw.at(pos + 2);
            pos += 3;
        }
        if (block == 0 || frame == 0) break;
        chunks += raw.slice(pos, pos + block).str();
        pos += block;
    }
    return chunks;
}

// xnb.read(raw) -> (platform, version, flags, data, [reader names], shared, offset)
void xnb_read(Call &c) {
    Bytes raw = c.data;
    if (!raw.slice_is(0, 3, "XNB", 3)) raise("ValueError", "not an XNB file");
    int64_t platform = raw.at(3), version = raw.at(4), flags = raw.at(5), size = raw.u32(6);
    std::string data;
    if (flags & 0x80) {
        int64_t out_len = raw.u32(10);
        std::string stream = deframe(raw, size);
        auto v = lzx::lzxd_decompress(Bytes(stream), 16, out_len);
        data.assign(reinterpret_cast<const char *>(v.data()), v.size());
    } else {
        data = raw.slice(10, size).str();
    }
    Bytes d(data);
    auto cnt = sevenbit(d, 0);
    int64_t o = cnt.second;
    std::vector<std::string> readers;
    for (uint64_t k = 0; k < cnt.first; ++k) {
        auto s = xstring(d, o);
        o = s.second + 4;
        readers.push_back(s.first.substr(0, s.first.find(',')));
    }
    Seven sh = sevenbit_exact(d, o);
    Out &out = c.out;
    out.begin_tuple();
    std::string pl;
    put_codepoint(pl, uint32_t(platform));
    out.text(pl);
    out.integer(version);
    out.integer(flags);
    out.bytes(d);
    out.begin_list();
    for (auto &r : readers) out.text(r);
    out.end_list();
    out.bigint(sh.exact);
    out.integer(sh.o);
    out.end_tuple();
}

struct Texture { int64_t format, width, height, levels; std::vector<Bytes> mips; int64_t o; };

Texture xnb_texture(Bytes data, int64_t o) {
    o = sevenbit(data, o).second;
    Texture t;
    data.need(o, 16);
    t.format = data.i32(o);
    t.width = data.u32(o + 4);
    t.height = data.u32(o + 8);
    t.levels = data.u32(o + 12);
    o += 16;
    for (int64_t k = 0; k < t.levels; ++k) {
        int64_t size = data.u32(o);
        o += 4;
        t.mips.push_back(data.slice(o, o + size));
        o += size;
    }
    t.o = o;
    return t;
}

void emit_texture(Out &out, const Texture &t) {
    out.begin_dict();
    out.item("format", t.format);
    out.item("width", t.width);
    out.item("height", t.height);
    out.item("levels", t.levels);
    out.key("mips");
    out.begin_list();
    for (auto &m : t.mips) out.bytes(m);
    out.end_list();
    out.end_dict();
}

float f32(Bytes b, int64_t o) {
    uint32_t v = b.u32(o);
    float f;
    std::memcpy(&f, &v, 4);
    return f;
}

// xnb.texture(data, o) -> ({format, width, height, levels, mips}, o)
void xnb_texture_q(Call &c) {
    Texture t = xnb_texture(c.data, c.int_arg(0));
    Out &out = c.out;
    out.begin_tuple();
    emit_texture(out, t);
    out.integer(t.o);
    out.end_tuple();
}

// xnb.sprite_font(data, o)
void xnb_sprite_font(Call &c) {
    Bytes data = c.data;
    int64_t o = sevenbit(data, c.int_arg(0)).second;
    Texture tex = xnb_texture(data, o);
    o = tex.o;
    auto rects = [&](std::vector<std::array<int64_t, 4>> &out) {
        o = sevenbit(data, o).second;
        int64_t n = data.u32(o);
        o += 4;
        for (int64_t i = 0; i < n; ++i) {
            std::array<int64_t, 4> r;
            data.need(o + 16 * i, 16);
            for (int k = 0; k < 4; ++k) r[size_t(k)] = data.i32(o + 16 * i + 4 * k);
            out.push_back(r);
        }
        o += 16 * n;
    };
    std::vector<std::array<int64_t, 4>> glyphs, cropping;
    rects(glyphs);
    rects(cropping);
    o = sevenbit(data, o).second;
    int64_t n = data.u32(o);
    o += 4;
    if (!(int64_t(glyphs.size()) == int64_t(cropping.size()) && int64_t(cropping.size()) == n))
        raise("ValueError", "sprite font lists disagree: " + dec(int64_t(glyphs.size())) + " glyphs, " + dec(int64_t(cropping.size())) +
                                " cropping, " + dec(n) + " characters");
    std::vector<std::string> chars;
    for (int64_t k = 0; k < n; ++k) {
        int64_t b0 = data.at(o), size = b0 < 0x80 ? 1 : b0 < 0xE0 ? 2 : b0 < 0xF0 ? 3 : 4;
        std::vector<uint32_t> cps = codepoints(utf8_replace(data.slice(o, o + size)));
        std::string ch;
        if (!cps.empty()) put_codepoint(ch, cps[0]);
        chars.push_back(ch);
        o += size;
    }
    data.need(o, 8);
    int64_t line_spacing = data.i32(o);
    double spacing = f32(data, o + 4);
    o += 8;
    o = sevenbit(data, o).second;
    int64_t k = data.u32(o);
    o += 4;
    std::vector<std::array<double, 3>> kerning;
    for (int64_t i = 0; i < k; ++i) data.need(o + 12 * i, 12), kerning.push_back({f32(data, o + 12 * i), f32(data, o + 12 * i + 4), f32(data, o + 12 * i + 8)});
    Out &out = c.out;
    out.begin_dict();
    out.key("texture");
    emit_texture(out, tex);
    for (int pass = 0; pass < 2; ++pass) {
        out.key(pass ? "cropping" : "glyphs");
        out.begin_list();
        for (auto &r : pass ? cropping : glyphs) {
            out.begin_list();
            for (int64_t v : r) out.integer(v);
            out.end_list();
        }
        out.end_list();
    }
    out.key("characters");
    out.begin_list();
    for (auto &ch : chars) out.text(ch);
    out.end_list();
    out.item("line_spacing", line_spacing);
    out.key("spacing");
    out.real(spacing);
    out.key("kerning");
    out.begin_list();
    for (auto &kk : kerning) {
        out.begin_list();
        for (double v : kk) out.real(v);
        out.end_list();
    }
    out.end_list();
    out.end_dict();
}

// xnb.sound_effect(data, o, platform)
void xnb_sound_effect(Call &c) {
    Bytes data = c.data;
    int64_t o = sevenbit(data, c.int_arg(0)).second;
    std::string platform = c.str_arg(1);
    int64_t n = data.u32(o);
    std::string fmt = data.slice(o + 4, o + 4 + n).str();
    o += 4 + n;
    int64_t size = data.u32(o);
    Bytes samples_b = data.slice(o + 4, o + 4 + size);
    std::string samples = samples_b.str();
    o += 4 + size;
    data.need(o, 12);
    int64_t loop_start = data.i32(o), loop_length = data.i32(o + 4), duration = data.i32(o + 8);
    if (platform == "x") {
        std::vector<int> widths = {2, 2, 4, 4, 2, 2, 2};
        if (Bytes(fmt).be16(0) == 0x166) {
            for (int w : {2, 4, 4, 4, 4, 4, 4, 4, 1, 1, 2}) widths.push_back(w);
        }
        int64_t size_of = 0;
        for (int w : widths) size_of += w;
        int64_t take = int64_t(fmt.size()) >= size_of ? size_of : 18;
        if (take == 18) widths.resize(7);
        Bytes f(fmt);
        f.need(0, int(take));
        std::string swapped;
        int64_t at = 0;
        for (int w : widths) {
            f.be(at, w);    // struct.error when the format is too short
            for (int k = w - 1; k >= 0; --k) swapped += fmt[size_t(at + k)];
            at += w;
        }
        fmt.replace(0, size_t(take), swapped);
    }
    Bytes f(fmt);
    f.need(0, 16);
    int64_t tag = f.u16(0), channels = f.u16(2), rate = f.u32(4);
    f.u32(8);
    f.u16(12);
    int64_t bits = f.u16(14);
    if (platform == "x" && tag == 1 && bits == 16) {
        std::string sw;
        for (size_t i = 0; i + 1 < samples.size(); i += 2) { sw += samples[i + 1]; sw += samples[i]; }
        samples = sw;
    }
    Out &out = c.out;
    out.begin_dict();
    out.item("tag", tag);
    out.item("channels", channels);
    out.item("rate", rate);
    out.item("bits", bits);
    out.key("fmt");
    out.bytes(Bytes(fmt));
    out.key("samples");
    out.bytes(Bytes(samples));
    out.key("loop");
    out.begin_list();
    out.integer(loop_start);
    out.integer(loop_length);
    out.end_list();
    out.item("duration_ms", duration);
    out.end_dict();
}

void xnb_effect_code(Call &c) {
    Bytes data = c.data;
    int64_t o = sevenbit(data, c.int_arg(0)).second;
    int64_t n = data.u32(o);
    c.out.bytes(data.slice(o + 4, o + 4 + n));
}

}  // namespace

CBF_QUESTIONS(graphics) {
    add("texture.tiled_offset_2d", texture_tiled_offset);
    add("texture.untile", texture_untile);
    add("texture.tile", texture_tile);
    add("texture.swap16", texture_swap16);
    add("texture.dds_header", texture_dds_header);
    add("texture.decode_rgba", texture_decode_rgba);
    add("xpr.parse", xpr_parse);
    add("gfx.find_movies", gfx_find_movies);
    add("gfx.wrapper_name", gfx_wrapper_name);
    add("gfx.parse_tags", gfx_parse_tags);
    add("gfx.ext_images", gfx_ext_images);
    add("gfx.fonts", gfx_fonts);
    add("gfx.placements", gfx_placements);
    add("gfx.glyph_contours", gfx_glyph_contours);
    add("xnb.read", xnb_read);
    add("xnb.texture", xnb_texture_q);
    add("xnb.sprite_font", xnb_sprite_font);
    add("xnb.sound_effect", xnb_sound_effect);
    add("xnb.effect_code", xnb_effect_code);
}

}  // namespace cbf::xbox
