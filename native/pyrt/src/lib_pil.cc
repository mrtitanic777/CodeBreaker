// lib_pil.cc -- PIL (Pillow) as far as CodeBreaker uses it, in C++: PIL.Image with the modes L, LA, P, RGB
// and RGBA; Image.new / open / frombytes / merge; convert (Pillow's integer luma, palette and
// transparency rules), crop, resize (Pillow's own NEAREST and its two-pass fixed-point resampling filters),
// paste, split, getpixel / putpixel / load() access, getdata, tobytes, putpalette, save. PNG is read and
// written (every colour type and bit depth, Adam7; adaptive filtering as Pillow's encoder, zlib from
// lib_extra.cc's copy); PPM / PGM (P6, P5) is read. What is not here raises, not guesses.
//
// The compressed bytes of a saved PNG are zlib's (this runtime's 1.3.1); the Pillow wheel carries zlib-ng,
// whose deflate output differs for all but small images: the decoded pixels and every other byte match.
#include <algorithm>
#include <cmath>
#include <cstring>

#include "rt.hpp"
#include "zlib.h"

namespace py {

TypeObj* T(const var& v);

namespace {

constexpr double M_PI = 3.14159265358979323846;
var S(const std::string& s) { return new_str(s); }

struct Img : DataObj {
    std::string mode;
    int w = 0, h = 0, nb = 1;
    std::string px;                 // w*h*nb bytes, row by row
    std::string pal;                // P: the palette as RGB bytes (as put / read; not padded)
    var info, format;
    Img* self_() { return this; }
};
TypeObj *T_img = nullptr, *T_pixels = nullptr;
var g_unidentified;

Img* I(const var& v) {
    if (!v.is_ref() || v.ref()->type != T_img) raise(E_AttributeError, "expected a PIL Image, not " + tname(v));
    return static_cast<Img*>(v.ref());
}
int bands_of(const std::string& mode) {
    if (mode == "L" || mode == "P" || mode == "1") return 1;
    if (mode == "LA") return 2;
    if (mode == "RGB") return 3;
    if (mode == "RGBA") return 4;
    raise(E_ValueError, "unrecognized image mode");
}
var make_img(const std::string& mode, int w, int h) {
    if (w < 0 || h < 0) raise(E_ValueError, "Width and height must be >= 0");
    auto* m = new Img();
    m->type = T_img;
    m->mode = mode;
    m->nb = bands_of(mode);
    m->w = w;
    m->h = h;
    m->px.assign((size_t)w * h * m->nb, '\0');
    var v(static_cast<Obj*>(m));
    m->info = new_dict_obj();
    m->format = None;
    return v;
}
void size_arg(const var& s, int& w, int& h) {
    if (!(is_tuple(s) || is_list(s)) || len_(s) != 2) raise(E_ValueError, "Size must be a list or tuple of two integers");
    w = (int)index_(s.getitem(var(0)));
    h = (int)index_(s.getitem(var(1)));
}

// a colour argument as the mode's pixel bytes
void ink_of(const std::string& mode, const var& c, uint8_t out[4]) {
    int nb = bands_of(mode);
    for (int i = 0; i < 4; i++) out[i] = 0;
    if (is_int(c)) {
        uint32_t v = (uint32_t)(index_(c) & 0xFFFFFFFF);
        if (mode == "1") { out[0] = index_(c) ? 255 : 0; return; }
        if (mode == "L" || mode == "P") { int64_t x = index_(c); out[0] = (uint8_t)std::max<int64_t>(0, std::min<int64_t>(255, x)); return; }
        uint8_t b[4] = {(uint8_t)v, (uint8_t)(v >> 8), (uint8_t)(v >> 16), (uint8_t)(v >> 24)};
        if (mode == "LA") { out[0] = b[0]; out[1] = b[3]; return; }
        out[0] = b[0]; out[1] = b[1]; out[2] = b[2]; out[3] = nb == 4 ? b[3] : 0;
        return;
    }
    if (c.is_float()) { out[0] = (uint8_t)c.flt(); return; }
    if (is_str(c)) {
        const std::string& s = str_utf8(c);
        if (!s.empty() && s[0] == '#' && (s.size() == 7 || s.size() == 4)) {
            auto hx = [&](size_t i, size_t n) { return (uint8_t)(n == 1 ? strtol(std::string(2, s[i]).c_str(), nullptr, 16) : strtol(s.substr(i, 2).c_str(), nullptr, 16)); };
            size_t n = s.size() == 4 ? 1 : 2;
            uint8_t r = hx(1, n), g = hx(1 + n, n), b = hx(1 + 2 * n, n);
            if (mode == "L") out[0] = (uint8_t)((r * 19595 + g * 38470 + b * 7471 + 0x8000) >> 16);
            else { out[0] = r; out[1] = g; out[2] = b; out[3] = 255; }
            return;
        }
        raise(E_ValueError, "unknown color specifier: " + repr_(c));
    }
    if (is_tuple(c) || is_list(c)) {
        auto it = to_vector(c);
        size_t n = it.size();
        auto b = [&](size_t i) { int64_t x = index_(it[i]); return (uint8_t)std::max<int64_t>(0, std::min<int64_t>(255, x)); };
        if (mode == "1") { if (n != 1) raise(E_TypeError, "color must be int or single-element tuple"); out[0] = b(0) ? 255 : 0; return; }
        if (mode == "L" || mode == "P") { if (n != 1) raise(E_TypeError, "color must be int or single-element tuple"); out[0] = b(0); return; }
        if (mode == "LA") {
            if (n == 1) { out[0] = b(0); out[1] = 255; return; }
            if (n == 2) { out[0] = b(0); out[1] = b(1); return; }
            raise(E_TypeError, "color must be int or tuple");
        }
        if (n == 3 || n == 4) { out[0] = b(0); out[1] = b(1); out[2] = b(2); out[3] = n == 4 ? b(3) : 255; return; }
        raise(E_TypeError, "color must be int or tuple");
    }
    raise(E_TypeError, "color must be int or tuple");
}
var pixel_var(const Img* im, size_t i) {
    const uint8_t* p = (const uint8_t*)im->px.data() + i * im->nb;
    if (im->nb == 1) return var((int64_t)p[0]);
    std::vector<var> t;
    for (int k = 0; k < im->nb; k++) t.push_back(var((int64_t)p[k]));
    return new_tuple(t);
}
inline int luma(int r, int g, int b) { return (r * 19595 + g * 38470 + b * 7471 + 0x8000) >> 16; }

// ------------------------------------------------------------------------------------------ convert
var convert_to(const var& src, const std::string& mode) {
    const Img* a = I(src);
    if (mode == "1" && a->mode != "1") raise(E_ValueError, "conversion to mode 1 is not supported by this runtime");
    if (mode == a->mode) {
        var c = make_img(mode, a->w, a->h);
        Img* d = I(c);
        d->px = a->px;
        d->pal = a->pal;
        d->info = a->info.call("copy");
        d->format = None;
        return c;
    }
    bands_of(mode);
    if (mode == "P") raise(E_ValueError, "conversion from " + a->mode + " to P is not supported by this runtime");
    var c = make_img(mode, a->w, a->h);
    Img* d = I(c);
    d->info = a->info.call("copy");
    size_t n = (size_t)a->w * a->h;
    const uint8_t* s = (const uint8_t*)a->px.data();
    uint8_t* o = (uint8_t*)d->px.data();
    // transparency: a P image's index (int) or per-entry alphas (bytes), an L / RGB image's key colour
    var tr = a->info.call("get", S("transparency"), None);
    int tr_index = -1;
    std::string tr_bytes;
    int tr_key[3] = {-1, -1, -1};
    bool want_alpha = mode == "RGBA" || mode == "LA";
    bool used_tr = false;
    if (!tr.is_none()) {
        if (a->mode == "P") {
            if (is_bytes(tr)) tr_bytes = raw_of(tr);
            else if (is_int(tr)) tr_index = (int)index_(tr);
        } else if (a->mode == "L" && is_int(tr)) tr_key[0] = (int)index_(tr);
        else if (a->mode == "RGB" && (is_tuple(tr) || is_list(tr)) && len_(tr) == 3) {
            for (int k = 0; k < 3; k++) tr_key[k] = (int)index_(tr.getitem(var(k)));
        }
    }
    for (size_t i = 0; i < n; i++) {
        int r, g, b, al = 255;
        if (a->mode == "L" || a->mode == "1") { r = g = b = s[i]; if (tr_key[0] >= 0 && s[i] == tr_key[0]) al = 0; }
        else if (a->mode == "LA") { r = g = b = s[i * 2]; al = s[i * 2 + 1]; }
        else if (a->mode == "RGB") { r = s[i * 3]; g = s[i * 3 + 1]; b = s[i * 3 + 2]; if (tr_key[0] >= 0 && r == tr_key[0] && g == tr_key[1] && b == tr_key[2]) al = 0; }
        else if (a->mode == "RGBA") { r = s[i * 4]; g = s[i * 4 + 1]; b = s[i * 4 + 2]; al = s[i * 4 + 3]; }
        else {   // P
            size_t k = s[i];
            if (k * 3 + 2 < a->pal.size()) { r = (uint8_t)a->pal[k * 3]; g = (uint8_t)a->pal[k * 3 + 1]; b = (uint8_t)a->pal[k * 3 + 2]; }
            else r = g = b = 0;
            if (tr_index >= 0 && (int)k == tr_index) al = 0;
            else if (!tr_bytes.empty()) al = k < tr_bytes.size() ? (uint8_t)tr_bytes[k] : 255;
        }
        if (al != 255 || tr_index >= 0 || !tr_bytes.empty() || tr_key[0] >= 0) used_tr = used_tr || want_alpha;
        if (mode == "L") o[i] = (uint8_t)luma(r, g, b);
        else if (mode == "LA") { o[i * 2] = (uint8_t)luma(r, g, b); o[i * 2 + 1] = (uint8_t)al; }
        else if (mode == "RGB") { o[i * 3] = (uint8_t)r; o[i * 3 + 1] = (uint8_t)g; o[i * 3 + 2] = (uint8_t)b; }
        else { o[i * 4] = (uint8_t)r; o[i * 4 + 1] = (uint8_t)g; o[i * 4 + 2] = (uint8_t)b; o[i * 4 + 3] = (uint8_t)al; }
    }
    if (want_alpha && !tr.is_none()) { try { d->info.call("pop", S("transparency"), None); } catch (error&) {} }
    return c;
}

// ------------------------------------------------------------------------------------------ resize
struct Filter { double support; double (*f)(double); };
double f_box(double x) { return (x > -0.5 && x <= 0.5) ? 1.0 : 0.0; }
double f_bilinear(double x) { x = std::fabs(x); return x < 1.0 ? 1.0 - x : 0.0; }
double f_hamming(double x) {
    x = std::fabs(x);
    if (x == 0.0) return 1.0;
    if (x >= 1.0) return 0.0;
    x *= M_PI;
    return std::sin(x) / x * (0.54 + 0.46 * std::cos(x));
}
double f_bicubic(double x) {
    const double a = -0.5;
    x = std::fabs(x);
    if (x < 1.0) return ((a + 2.0) * x - (a + 3.0)) * x * x + 1;
    if (x < 2.0) return (((x - 5) * x + 8) * x - 4) * a;
    return 0.0;
}
double sinc(double x) { if (x == 0.0) return 1.0; x *= M_PI; return std::sin(x) / x; }
double f_lanczos(double x) { return (x > -3.0 && x < 3.0) ? sinc(x) * sinc(x / 3.0) : 0.0; }
const Filter* filter_of(int id) {
    static const Filter box{0.5, f_box}, bil{1.0, f_bilinear}, ham{1.0, f_hamming}, bic{2.0, f_bicubic}, lan{3.0, f_lanczos};
    switch (id) { case 4: return &box; case 2: return &bil; case 5: return &ham; case 3: return &bic; case 1: return &lan; }
    raise(E_ValueError, "unknown resampling filter");
}
// Resample.c: precompute_coeffs + normalize_coeffs_8bpc
struct Coeffs { int ksize; std::vector<int> bounds, kk; };
Coeffs precompute(int in_size, double in0, double in1, int out_size, const Filter* flt) {
    double scale = (in1 - in0) / out_size;
    double filterscale = std::max(scale, 1.0);
    double support = flt->support * filterscale;
    Coeffs c;
    c.ksize = (int)std::ceil(support) * 2 + 1;
    c.bounds.assign((size_t)out_size * 2, 0);
    std::vector<double> k((size_t)out_size * c.ksize, 0.0);
    for (int xx = 0; xx < out_size; xx++) {
        double center = in0 + (xx + 0.5) * scale, ww = 0.0, ss = 1.0 / filterscale;
        int xmin = (int)(center - support + 0.5);
        if (xmin < 0) xmin = 0;
        int xmax = (int)(center + support + 0.5);
        if (xmax > in_size) xmax = in_size;
        xmax -= xmin;
        double* kp = &k[(size_t)xx * c.ksize];
        for (int x = 0; x < xmax; x++) { double w = flt->f((x + xmin - center + 0.5) * ss); kp[x] = w; ww += w; }
        for (int x = 0; x < xmax; x++) if (ww != 0.0) kp[x] /= ww;
        c.bounds[(size_t)xx * 2] = xmin;
        c.bounds[(size_t)xx * 2 + 1] = xmax;
    }
    c.kk.resize(k.size());
    for (size_t i = 0; i < k.size(); i++) c.kk[i] = (int)(k[i] < 0 ? -0.5 + k[i] * (1 << 22) : 0.5 + k[i] * (1 << 22));
    return c;
}
inline uint8_t clip8(int v) { return (uint8_t)(v < 0 ? 0 : v > 255 ? 255 : v); }
// src: sw x sh with nb bands; one horizontal pass over rows [y0, y0+rows), then vertical
std::string do_resample(const std::string& src, int sw, int sh, int nb, int ow, int oh, double bx0, double by0, double bx1, double by1, const Filter* flt) {
    bool need_h = ow != sw || bx0 != 0 || bx1 != sw;
    bool need_v = oh != sh || by0 != 0 || by1 != sh;
    std::string cur = src;
    int cw = sw, ch = sh;
    Coeffs ch_x, ch_y;
    int ybox_first = 0, ybox_last = sh;
    if (need_v) {
        ch_y = precompute(sh, by0, by1, oh, flt);
        ybox_first = ch_y.bounds[0];
        ybox_last = ch_y.bounds[(size_t)oh * 2 - 2] + ch_y.bounds[(size_t)oh * 2 - 1];
    }
    if (need_h) {
        ch_x = precompute(sw, bx0, bx1, ow, flt);
        int rows = ybox_last - ybox_first;
        std::string tmp((size_t)ow * rows * nb, '\0');
        for (int yy = 0; yy < rows; yy++) {
            const uint8_t* srow = (const uint8_t*)cur.data() + (size_t)(yy + ybox_first) * sw * nb;
            uint8_t* drow = (uint8_t*)tmp.data() + (size_t)yy * ow * nb;
            for (int xx = 0; xx < ow; xx++) {
                int xmin = ch_x.bounds[(size_t)xx * 2], xmax = ch_x.bounds[(size_t)xx * 2 + 1];
                const int* k = &ch_x.kk[(size_t)xx * ch_x.ksize];
                for (int b = 0; b < nb; b++) {
                    int ss = 1 << 21;
                    for (int x = 0; x < xmax; x++) ss += srow[(size_t)(x + xmin) * nb + b] * k[x];
                    drow[(size_t)xx * nb + b] = clip8(ss >> 22);
                }
            }
        }
        cur.swap(tmp);
        cw = ow;
        ch = rows;
        if (need_v) { /* the vertical pass reads rows relative to ybox_first */ }
    }
    if (need_v) {
        std::string out((size_t)cw * oh * nb, '\0');
        for (int yy = 0; yy < oh; yy++) {
            int ymin = ch_y.bounds[(size_t)yy * 2], ymax = ch_y.bounds[(size_t)yy * 2 + 1];
            const int* k = &ch_y.kk[(size_t)yy * ch_y.ksize];
            int base = need_h ? ymin - ybox_first : ymin;
            for (int xx = 0; xx < cw; xx++)
                for (int b = 0; b < nb; b++) {
                    int ss = 1 << 21;
                    for (int y = 0; y < ymax; y++) ss += (uint8_t)cur[((size_t)(y + base) * cw + xx) * nb + b] * k[y];
                    out[((size_t)yy * cw + xx) * nb + b] = (char)clip8(ss >> 22);
                }
        }
        return out;
    }
    (void)ch;
    return cur;
}

// ------------------------------------------------------------------------------------------ PNG
uint32_t be32(const uint8_t* p) { return ((uint32_t)p[0] << 24) | (p[1] << 16) | (p[2] << 8) | p[3]; }
void put32(std::string& s, uint32_t v) { s += (char)(v >> 24); s += (char)(v >> 16); s += (char)(v >> 8); s += (char)v; }
void put_chunk(std::string& out, const char* id, const std::string& data) {
    put32(out, (uint32_t)data.size());
    std::string body = std::string(id, 4) + data;
    out += body;
    put32(out, (uint32_t)crc32_z(crc32_z(0, nullptr, 0), (const Bytef*)body.data(), body.size()));
}
[[noreturn]] void bad_image(const std::string& what) { raise(T(g_unidentified), what); }

std::string inflate_all(const std::string& in) {
    z_stream z{};
    if (inflateInit(&z) != Z_OK) raise(E_OSError, "broken PNG file");
    z.next_in = (Bytef*)in.data();
    z.avail_in = (uInt)in.size();
    std::string out;
    char buf[65536];
    int rc;
    do {
        z.next_out = (Bytef*)buf;
        z.avail_out = sizeof buf;
        rc = inflate(&z, Z_NO_FLUSH);
        out.append(buf, sizeof buf - z.avail_out);
    } while (rc == Z_OK);
    inflateEnd(&z);
    if (rc != Z_STREAM_END && rc != Z_BUF_ERROR) raise(E_OSError, "broken data stream when reading image file");
    return out;
}
inline int paeth(int a, int b, int c) {
    int p = a + b - c, pa = std::abs(p - a), pb = std::abs(p - b), pc = std::abs(p - c);
    return (pa <= pb && pa <= pc) ? a : (pb <= pc ? b : c);
}
// unfilter one pass of w x h samples; returns rows packed (stride bytes each); pos advanced
void unfilter(const std::string& raw, size_t& pos, int h, size_t stride, int bpp, std::string& out) {
    out.assign(stride * h, '\0');
    for (int y = 0; y < h; y++) {
        if (pos + 1 + stride > raw.size()) raise(E_OSError, "image file is truncated");
        int ft = (uint8_t)raw[pos++];
        uint8_t* row = (uint8_t*)out.data() + y * stride;
        const uint8_t* prev = y ? row - stride : nullptr;
        const uint8_t* src = (const uint8_t*)raw.data() + pos;
        for (size_t i = 0; i < stride; i++) {
            int a = i >= (size_t)bpp ? row[i - bpp] : 0, b = prev ? prev[i] : 0, c = (prev && i >= (size_t)bpp) ? prev[i - bpp] : 0;
            int v = src[i];
            switch (ft) {
                case 0: break;
                case 1: v += a; break;
                case 2: v += b; break;
                case 3: v += (a + b) / 2; break;
                case 4: v += paeth(a, b, c); break;
                default: raise(E_OSError, "unknown filter category");
            }
            row[i] = (uint8_t)v;
        }
        pos += stride;
    }
}
var decode_png(const std::string& d) {
    if (d.size() < 33 || memcmp(d.data(), "\x89PNG\r\n\x1a\n", 8) != 0) bad_image("not a PNG");
    size_t p = 8;
    uint32_t w = 0, h = 0;
    int depth = 0, ct = 0, interlace = 0;
    std::string plte, trns, idat;
    bool have_ihdr = false;
    while (p + 8 <= d.size()) {
        uint32_t len = be32((const uint8_t*)d.data() + p);
        std::string id = d.substr(p + 4, 4);
        if (p + 12 + (size_t)len > d.size()) raise(E_OSError, "image file is truncated");
        const uint8_t* body = (const uint8_t*)d.data() + p + 8;
        if (id == "IHDR") {
            if (len < 13) raise(E_SyntaxError, "broken PNG file (bad header checksum in IHDR)");
            w = be32(body); h = be32(body + 4); depth = body[8]; ct = body[9]; interlace = body[12];
            have_ihdr = true;
        } else if (id == "PLTE") plte.assign((const char*)body, len);
        else if (id == "tRNS") trns.assign((const char*)body, len);
        else if (id == "IDAT") idat.append((const char*)body, len);
        else if (id == "IEND") break;
        p += 12 + len;
    }
    if (!have_ihdr) raise(E_SyntaxError, "Truncated File Read");
    int chans = ct == 0 ? 1 : ct == 2 ? 3 : ct == 3 ? 1 : ct == 4 ? 2 : ct == 6 ? 4 : 0;
    bool ok = (ct == 0 && (depth == 1 || depth == 2 || depth == 4 || depth == 8 || depth == 16)) ||
              ((ct == 2 || ct == 4 || ct == 6) && (depth == 8 || depth == 16)) || (ct == 3 && (depth == 1 || depth == 2 || depth == 4 || depth == 8));
    if (!ok) raise(E_OSError, "unsupported PNG image mode (" + std::to_string(depth) + "," + std::to_string(ct) + ")");
    std::string mode = ct == 0 ? (depth == 1 ? "1" : "L") : ct == 2 ? "RGB" : ct == 3 ? "P" : ct == 4 ? "LA" : "RGBA";
    if (ct == 0 && depth == 16) raise(E_OSError, "16-bit grayscale PNG (mode I;16) is not supported by this runtime");
    int nb = chans;
    bool la16 = ct == 4 && depth == 16;   // (Pillow reads LA;16B as RGBA: converted after decoding)
    std::string raw = inflate_all(idat);
    var im = make_img(mode, (int)w, (int)h);
    Img* m = I(im);
    int bpp = std::max(1, depth * chans / 8);
    auto sample_row = [&](const uint8_t* row, int count, uint8_t* out) {   // one row of `count` pixels -> 8-bit samples
        if (depth == 8) { memcpy(out, row, (size_t)count * chans); return; }
        if (depth == 16) { for (int i = 0; i < count * chans; i++) out[i] = row[i * 2]; return; }
        int per = 8 / depth, mask = (1 << depth) - 1;
        for (int i = 0; i < count; i++) {
            int v = (row[i / per] >> (8 - depth * (i % per + 1))) & mask;
            out[i] = ct == 3 ? (uint8_t)v : (uint8_t)(v * 255 / mask);
        }
    };
    if (!interlace) {
        size_t stride = ((size_t)w * depth * chans + 7) / 8, pos = 0;
        std::string un;
        unfilter(raw, pos, (int)h, stride, bpp, un);
        for (uint32_t y = 0; y < h; y++) sample_row((const uint8_t*)un.data() + y * stride, (int)w, (uint8_t*)m->px.data() + (size_t)y * w * nb);
    } else {
        static const int x0[7] = {0, 4, 0, 2, 0, 1, 0}, y0[7] = {0, 0, 4, 0, 2, 0, 1}, dx[7] = {8, 8, 4, 4, 2, 2, 1}, dy[7] = {8, 8, 8, 4, 4, 2, 2};
        size_t pos = 0;
        for (int ps = 0; ps < 7; ps++) {
            int pw = ((int)w - x0[ps] + dx[ps] - 1) / dx[ps], ph = ((int)h - y0[ps] + dy[ps] - 1) / dy[ps];
            if (pw <= 0 || ph <= 0) continue;
            size_t stride = ((size_t)pw * depth * chans + 7) / 8;
            std::string un;
            unfilter(raw, pos, ph, stride, bpp, un);
            std::string row((size_t)pw * nb, '\0');
            for (int y = 0; y < ph; y++) {
                sample_row((const uint8_t*)un.data() + y * stride, pw, (uint8_t*)row.data());
                for (int x = 0; x < pw; x++)
                    memcpy(&m->px[((size_t)(y0[ps] + y * dy[ps]) * w + x0[ps] + x * dx[ps]) * nb], &row[(size_t)x * nb], nb);
            }
        }
    }
    if (ct == 3) m->pal = plte;
    // transparency, as PngImagePlugin keeps it in info
    if (!trns.empty()) {
        if (mode == "P") {
            bool simple = true;
            int zeros = 0;
            for (size_t i = 0; i < trns.size(); i++) { if (trns[i] == 0) zeros++; else if ((uint8_t)trns[i] != 255) simple = false; }
            if (simple && zeros == 1) m->info.setitem(S("transparency"), var((int64_t)trns.find('\0')));
            else m->info.setitem(S("transparency"), new_bytes(trns));
        } else if ((mode == "L" || mode == "1") && trns.size() >= 2) m->info.setitem(S("transparency"), var((int64_t)(((uint8_t)trns[0] << 8) | (uint8_t)trns[1])));
        else if (mode == "RGB" && trns.size() >= 6)
            m->info.setitem(S("transparency"), new_tuple({var((int64_t)(((uint8_t)trns[0] << 8) | (uint8_t)trns[1])), var((int64_t)(((uint8_t)trns[2] << 8) | (uint8_t)trns[3])),
                                                         var((int64_t)(((uint8_t)trns[4] << 8) | (uint8_t)trns[5]))}));
    }
    if (interlace) m->info.setitem(S("interlace"), var(1));
    m->format = S("PNG");
    if (la16) {
        var rgba = convert_to(im, "RGBA");
        I(rgba)->info = m->info;
        I(rgba)->format = m->format;
        return rgba;
    }
    return im;
}

std::string deflate_idat(const std::string& raw, size_t bufsize, std::string& out_chunks) {
    z_stream z{};
    deflateInit2(&z, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 15, 9, Z_DEFAULT_STRATEGY);
    z.next_in = (Bytef*)raw.data();
    z.avail_in = (uInt)raw.size();
    std::string all;
    std::vector<char> buf(65536);
    int rc;
    do {
        z.next_out = (Bytef*)buf.data();
        z.avail_out = (uInt)buf.size();
        rc = deflate(&z, Z_FINISH);
        all.append(buf.data(), buf.size() - z.avail_out);
    } while (rc == Z_OK || rc == Z_BUF_ERROR);
    deflateEnd(&z);
    for (size_t o = 0; o < all.size(); o += bufsize) put_chunk(out_chunks, "IDAT", all.substr(o, bufsize));
    if (all.empty()) put_chunk(out_chunks, "IDAT", "");
    return all;
}

std::string encode_png(const Img* im, const CallArgs& kwargs) {
    int colors = 0, bits = 8, ct = 0;
    std::string mode = im->mode;
    size_t nb = im->nb;
    if (mode == "P") {
        colors = std::max(1, std::min((int)(im->pal.size() / 3), 256));
        var b = kwarg(kwargs, "bits");
        if (!b.is_unbound()) colors = std::min(1 << (int)index_(b), 256);
        bits = 8;
        if (colors <= 16) bits = colors <= 2 ? 1 : colors <= 4 ? 2 : 4;
        ct = 3;
    } else if (mode == "1") { ct = 0; bits = 1; }
    else if (mode == "L") ct = 0;
    else if (mode == "LA") ct = 4;
    else if (mode == "RGB") ct = 2;
    else if (mode == "RGBA") ct = 6;
    std::string out("\x89PNG\r\n\x1a\n", 8), hdr;
    put32(hdr, (uint32_t)im->w);
    put32(hdr, (uint32_t)im->h);
    hdr += (char)bits; hdr += (char)ct; hdr += '\0'; hdr += '\0'; hdr += '\0';
    put_chunk(out, "IHDR", hdr);
    if (mode == "P") {
        std::string pb = im->pal.substr(0, (size_t)colors * 3);
        pb.resize((size_t)colors * 3, '\0');
        put_chunk(out, "PLTE", pb);
    }
    var tr = kwarg(kwargs, "transparency");
    if (tr.is_unbound()) tr = im->info.call("get", S("transparency"), None);
    bool tr_set = !tr.is_none() && (tr.truthy() || (is_int(tr) && index_(tr) == 0));
    if (tr_set && !tr.is_none()) {
        if (mode == "P") {
            if (is_bytes(tr)) put_chunk(out, "tRNS", raw_of(tr).substr(0, (size_t)colors));
            else {
                int64_t t = std::max<int64_t>(0, std::min<int64_t>(255, index_(tr)));
                std::string alpha((size_t)t, '\xff');
                alpha += '\0';
                put_chunk(out, "tRNS", alpha.substr(0, (size_t)colors));
            }
        } else if (mode == "L") {
            int64_t t = std::max<int64_t>(0, std::min<int64_t>(65535, index_(tr)));
            std::string s; s += (char)(t >> 8); s += (char)t;
            put_chunk(out, "tRNS", s);
        } else if (mode == "RGB") {
            std::string s;
            for (int k = 0; k < 3; k++) { int64_t t = index_(tr.getitem(var(k))); s += (char)(t >> 8); s += (char)t; }
            put_chunk(out, "tRNS", s);
        } else if (!kwarg(kwargs, "transparency").is_unbound()) raise(E_OSError, "cannot use transparency for this mode");
    }
    // the image data: rows packed to `bits`, filtered (adaptive for 8-bit rows, none below), deflated
    size_t stride = ((size_t)im->w * bits * nb + 7) / 8;
    int bpp = std::max<size_t>(1, bits * nb / 8);
    std::string raw;
    raw.reserve((stride + 1) * im->h);
    std::string prev(stride, '\0'), row(stride, '\0');
    bool adaptive = !(mode == "P" && bits == 8);   // (Pillow filters rows adaptively, except 8-bit palette rows)
    for (int y = 0; y < im->h; y++) {
        const uint8_t* src = (const uint8_t*)im->px.data() + (size_t)y * im->w * nb;
        if (bits == 8) memcpy(&row[0], src, stride);
        else {
            std::fill(row.begin(), row.end(), '\0');
            int mask = (1 << bits) - 1;
            for (int x = 0; x < im->w; x++) row[(size_t)x * bits / 8] |= (char)((src[x] & mask) << (8 - bits - (x * bits) % 8));
        }
        int best = 0;
        std::string cand[5];
        long sums[5];
        auto sum_of = [&](const std::string& f) { long s = 0; for (unsigned char c : f) s += std::abs((int)(signed char)c); return s; };
        cand[0] = row;
        sums[0] = adaptive ? sum_of(row) : 0;
        if (adaptive) {
            for (int ft = 1; ft < 5; ft++) {
                if (ft == 3) continue;   // (Pillow's encoder never tries Average)
                std::string f(stride, '\0');
                for (size_t i = 0; i < stride; i++) {
                    int a = i >= (size_t)bpp ? (uint8_t)row[i - bpp] : 0, b = (uint8_t)prev[i], c = i >= (size_t)bpp ? (uint8_t)prev[i - bpp] : 0, x = (uint8_t)row[i];
                    int pred = ft == 1 ? a : ft == 2 ? b : ft == 3 ? (a + b) / 2 : paeth(a, b, c);
                    f[i] = (char)(x - pred);
                }
                cand[ft] = f;
                sums[ft] = sum_of(f);
                if (sums[ft] < sums[best]) best = ft;
            }
        }
        raw += (char)best;
        raw += cand[best];
        prev = row;
    }
    deflate_idat(raw, std::max<size_t>(65536, (size_t)im->w * 4), out);
    put_chunk(out, "IEND", "");
    return out;
}

// ------------------------------------------------------------------------------------------ PPM
var decode_pnm(const std::string& d) {
    if (d.size() < 3 || d[0] != 'P' || (d[1] != '6' && d[1] != '5')) bad_image("not a PNM");
    size_t p = 2;
    auto token = [&]() -> long {
        for (;;) {
            while (p < d.size() && isspace((unsigned char)d[p])) p++;
            if (p < d.size() && d[p] == '#') { while (p < d.size() && d[p] != '\n') p++; continue; }
            break;
        }
        long v = 0;
        bool any = false;
        while (p < d.size() && isdigit((unsigned char)d[p])) { v = v * 10 + (d[p] - '0'); p++; any = true; }
        if (!any) raise(E_ValueError, "Reached EOF while reading header");
        return v;
    };
    long w = token(), h = token(), maxv = token();
    p++;   // the one whitespace byte
    if (maxv > 255) raise(E_OSError, "unsupported PPM maxval");
    bool rgb = d[1] == '6';
    var im = make_img(rgb ? "RGB" : "L", (int)w, (int)h);
    Img* m = I(im);
    size_t need = m->px.size();
    if (d.size() - p < need) raise(E_OSError, "image file is truncated (" + std::to_string(need - (d.size() - p)) + " bytes not processed)");
    memcpy(&m->px[0], d.data() + p, need);
    if (maxv != 255) for (auto& c : m->px) c = (char)((uint8_t)c * 255 / maxv);
    m->format = S("PPM");
    return im;
}

std::string encode_ppm(const Img* im) {
    if (im->mode != "RGB" && im->mode != "L") raise(E_OSError, "cannot write mode " + im->mode + " as PPM");
    return std::string(im->mode == "RGB" ? "P6" : "P5") + "\n" + std::to_string(im->w) + " " + std::to_string(im->h) + "\n255\n" + im->px;
}

var open_bytes(const std::string& d) {
    if (d.size() >= 8 && memcmp(d.data(), "\x89PNG\r\n\x1a\n", 8) == 0) return decode_png(d);
    if (d.size() >= 3 && d[0] == 'P' && (d[1] == '6' || d[1] == '5') && isspace((unsigned char)d[2])) return decode_pnm(d);
    bad_image("cannot identify image file");
}

// ------------------------------------------------------------------------------------------ module
void box_args(const Img* im, const var& box, int& x0, int& y0, int& x1, int& y1) {
    if (box.is_none()) { x0 = y0 = 0; x1 = im->w; y1 = im->h; return; }
    if (!(is_tuple(box) || is_list(box)) || len_(box) != 4) raise(E_TypeError, "box must be a 4-tuple");
    x0 = (int)index_(box.getitem(var(0))); y0 = (int)index_(box.getitem(var(1)));
    x1 = (int)index_(box.getitem(var(2))); y1 = (int)index_(box.getitem(var(3)));
}
void check_xy(const Img* im, const var& xy, int& x, int& y) {
    if (!(is_tuple(xy) || is_list(xy)) || len_(xy) != 2) raise(E_TypeError, "argument must be a sequence of length 2");
    x = (int)index_(xy.getitem(var(0)));
    y = (int)index_(xy.getitem(var(1)));
    if (x < 0) x += im->w;
    if (y < 0) y += im->h;
    if (x < 0 || y < 0 || x >= im->w || y >= im->h) raise(E_IndexError, "image index out of range");
}

// raw pixel data into an image (mode 1 arrives eight pixels to a byte)
void load_raw(Img* d, const std::string& data) {
    if (d->mode == "1") {
        size_t stride = ((size_t)d->w + 7) / 8;
        if (data.size() < stride * d->h) raise(E_ValueError, "not enough image data");
        for (int y = 0; y < d->h; y++)
            for (int x = 0; x < d->w; x++) d->px[(size_t)y * d->w + x] = (data[y * stride + x / 8] & (0x80 >> (x % 8))) ? '\xff' : '\0';
        return;
    }
    if (data.size() < d->px.size()) raise(E_ValueError, "not enough image data");
    memcpy(&d->px[0], data.data(), d->px.size());
}

var open_any(const var& fp) {
    std::string data;
    if (is_str(fp) || (fp.is_ref() && fp.hasattr("__fspath__"))) {
        std::string path = fs_path(fp);
        var f = import("builtins").call("open", S(path), S("rb"));
        data = raw_of(f.call("read"));
        f.call("close");
    } else {
        data = raw_of(fp.call("read"));
    }
    var im = open_bytes(data);
    return im;
}

void init_image(const var& m) {
    mod_set(m, "__name__", S("PIL.Image"));
    T_img = new_type("Image", {}, Layout::NativeData, false, "PIL.Image");
    T_pixels = new_type("PixelAccess", {}, Layout::NativeData, false, "PIL.Image");
    mod_set(m, "Image", tv(T_img));
    struct { const char* n; int v; } consts[] = {{"NEAREST", 0}, {"LANCZOS", 1}, {"ANTIALIAS", 1}, {"BILINEAR", 2}, {"LINEAR", 2}, {"BICUBIC", 3}, {"CUBIC", 3}, {"BOX", 4}, {"HAMMING", 5}};
    for (auto& c : consts) mod_set(m, c.n, var((int64_t)c.v));
    mod_set(m, "UnidentifiedImageError", g_unidentified);
    mod_set(m, "MODES", new_list({S("L"), S("LA"), S("P"), S("RGB"), S("RGBA")}));

    mod_def(m, "new", [](const CallArgs& a) {
        std::string mode = str_utf8(arg(a, 0, "mode"));
        int w, h;
        size_arg(arg(a, 1, "size"), w, h);
        var color = arg(a, 2, "color", var(0));
        var im = make_img(mode, w, h);
        Img* d = I(im);
        uint8_t ink[4];
        if (is_int(color) && index_(color) == 0 && !color.is_bool()) return im;
        if (color.is_none()) return im;
        ink_of(mode, color, ink);
        for (size_t i = 0; i < (size_t)w * h; i++) memcpy(&d->px[i * d->nb], ink, d->nb);
        return im;
    });
    mod_def(m, "open", [](const CallArgs& a) {
        var fp = arg(a, 0, "fp");
        return open_any(fp);
    });
    mod_def(m, "frombytes", [](const CallArgs& a) {
        std::string mode = str_utf8(arg(a, 0, "mode"));
        int w, h;
        size_arg(arg(a, 1, "size"), w, h);
        const std::string& data = raw_of(arg(a, 2, "data"));
        var im = make_img(mode, w, h);
        Img* d = I(im);
        load_raw(d, data);
        return im;
    });
    mod_def(m, "merge", [](const CallArgs& a) {
        std::string mode = str_utf8(arg(a, 0, "mode"));
        auto bands = to_vector(arg(a, 1, "bands"));
        int nb = bands_of(mode);
        if ((int)bands.size() != nb) raise(E_ValueError, "wrong number of bands");
        const Img* f = I(bands[0]);
        var im = make_img(mode, f->w, f->h);
        Img* d = I(im);
        for (int b = 0; b < nb; b++) {
            const Img* s = I(bands[b]);
            if (s->mode != "L") raise(E_ValueError, "mode mismatch");
            if (s->w != f->w || s->h != f->h) raise(E_ValueError, "size mismatch");
            for (size_t i = 0; i < (size_t)f->w * f->h; i++) d->px[i * nb + b] = s->px[i];
        }
        return im;
    });

    add_getter(T_img, "mode", [](const var& v) { return S(I(v)->mode); });
    add_getter(T_img, "size", [](const var& v) { return new_tuple({var((int64_t)I(v)->w), var((int64_t)I(v)->h)}); });
    add_getter(T_img, "width", [](const var& v) { return var((int64_t)I(v)->w); });
    add_getter(T_img, "height", [](const var& v) { return var((int64_t)I(v)->h); });
    add_getter(T_img, "info", [](const var& v) { return I(v)->info; });
    add_getter(T_img, "format", [](const var& v) { return I(v)->format; });
    add_getter(T_img, "palette", [](const var&) { return var(None); });
    add_method(T_img, "getbands", [](const CallArgs& a) {
        const std::string& m = I(a.pos.at(0))->mode;
        std::vector<var> t;
        if (m == "P") t = {S("P")};
        else for (char c : m) t.push_back(S(std::string(1, c)));
        return new_tuple(t);
    });
    add_method(T_img, "load", [](const CallArgs& a) {
        auto* p = new Img();   // (the accessor keeps its image in `info`)
        p->type = T_pixels;
        p->info = a.pos.at(0);
        return var(static_cast<Obj*>(p));
    });
    add_method(T_img, "copy", [](const CallArgs& a) {
        const Img* s = I(a.pos.at(0));
        var c = make_img(s->mode, s->w, s->h);
        Img* d = I(c);
        d->px = s->px; d->pal = s->pal;
        d->info = s->info.call("copy");
        return c;
    });
    add_method(T_img, "close", [](const CallArgs&) { return var(None); });
    add_method(T_img, "__enter__", [](const CallArgs& a) { return a.pos.at(0); });
    add_method(T_img, "__exit__", [](const CallArgs&) { return var(None); });
    // (a function, not a native: rich comparison looks past natives; __ne__ follows from __eq__)
    T_img->dict->set(interned("__eq__"), def("__eq__", "self, other", {}, [](var self, var other) -> var {
        if (!other.is_ref() || other.ref()->type != T_img) return NotImplemented;
        const Img *x = I(self), *y = I(other);
        return var(x->mode == y->mode && x->w == y->w && x->h == y->h && x->px == y->px && x->pal == y->pal && eq_(x->info, y->info));
    }));
    T_img->dict->set(interned("__hash__"), var(None));
    add_method(T_img, "tobytes", [](const CallArgs& a) {
        const Img* s = I(a.pos.at(0));
        if (s->mode != "1") return new_bytes(s->px);
        std::string out;   // mode 1: eight pixels to a byte, most significant first, rows padded
        size_t stride = ((size_t)s->w + 7) / 8;
        out.assign(stride * s->h, '\0');
        for (int y = 0; y < s->h; y++)
            for (int x = 0; x < s->w; x++)
                if (s->px[(size_t)y * s->w + x]) out[y * stride + x / 8] |= (char)(0x80 >> (x % 8));
        return new_bytes(out);
    });
    add_method(T_img, "frombytes", [](const CallArgs& a) {
        Img* d = I(a.pos.at(0));
        const std::string& data = raw_of(a.pos.at(1));
        load_raw(d, data);
        return var(None);
    });
    add_method(T_img, "getdata", [](const CallArgs& a) {
        const Img* s = I(a.pos.at(0));
        std::vector<var> out;
        size_t n = (size_t)s->w * s->h;
        out.reserve(n);
        for (size_t i = 0; i < n; i++) out.push_back(pixel_var(s, i));
        return new_list(out);
    });
    add_method(T_img, "getpixel", [](const CallArgs& a) {
        const Img* s = I(a.pos.at(0));
        int x, y;
        check_xy(s, a.pos.at(1), x, y);
        return pixel_var(s, (size_t)y * s->w + x);
    });
    add_method(T_img, "putpixel", [](const CallArgs& a) {
        Img* s = I(a.pos.at(0));
        int x, y;
        check_xy(s, a.pos.at(1), x, y);
        uint8_t ink[4];
        ink_of(s->mode, a.pos.at(2), ink);
        memcpy(&s->px[((size_t)y * s->w + x) * s->nb], ink, s->nb);
        return var(None);
    });
    add_method(T_img, "putpalette", [](const CallArgs& a) {
        Img* s = I(a.pos.at(0));
        if (s->mode != "P" && s->mode != "L") raise(E_ValueError, "illegal image mode");
        var data = a.pos.at(1);
        std::string rawmode = a.pos.size() > 2 ? str_utf8(a.pos[2]) : "RGB";
        std::string bytes;
        if (is_bytes(data)) bytes = raw_of(data);
        else for (const var& v : to_vector(data)) bytes += (char)index_(v);
        if (rawmode != "RGB") raise(E_ValueError, "putpalette: only the RGB rawmode is supported");
        if (bytes.size() > 768) bytes.resize(768);
        s->pal = bytes;
        return var(None);
    });
    add_method(T_img, "getpalette", [](const CallArgs& a) -> var {
        const Img* s = I(a.pos.at(0));
        if (s->mode != "P") return None;
        std::vector<var> out;
        for (unsigned char c : s->pal) out.push_back(var((int64_t)c));
        return new_list(out);
    });
    add_method(T_img, "convert", [](const CallArgs& a) {
        var mode = arg(a, 1, "mode", None);
        if (mode.is_none()) return a.pos.at(0).call("copy");
        return convert_to(a.pos.at(0), str_utf8(mode));
    });
    add_method(T_img, "split", [](const CallArgs& a) {
        const Img* s = I(a.pos.at(0));
        std::vector<var> out;
        if (s->mode == "P") { out.push_back(a.pos[0].call("copy")); return new_tuple(out); }
        for (int b = 0; b < s->nb; b++) {
            var band = make_img("L", s->w, s->h);
            Img* d = I(band);
            for (size_t i = 0; i < (size_t)s->w * s->h; i++) d->px[i] = s->px[i * s->nb + b];
            out.push_back(band);
        }
        return new_tuple(out);
    });
    add_method(T_img, "crop", [](const CallArgs& a) {
        const Img* s = I(a.pos.at(0));
        int x0, y0, x1, y1;
        box_args(s, arg(a, 1, "box", None), x0, y0, x1, y1);
        if (x1 < x0 || y1 < y0) raise(E_ValueError, "Coordinate 'right' is less than 'left'");
        var c = make_img(s->mode, x1 - x0, y1 - y0);
        Img* d = I(c);
        d->pal = s->pal;
        d->info = s->info.call("copy");
        for (int y = y0; y < y1; y++) {
            if (y < 0 || y >= s->h) continue;
            int xa = std::max(x0, 0), xb = std::min(x1, s->w);
            if (xb <= xa) continue;
            memcpy(&d->px[((size_t)(y - y0) * (x1 - x0) + (xa - x0)) * s->nb], &s->px[((size_t)y * s->w + xa) * s->nb], (size_t)(xb - xa) * s->nb);
        }
        return c;
    });
    add_method(T_img, "resize", [](const CallArgs& a) {
        const Img* s = I(a.pos.at(0));
        int ow, oh;
        size_arg(arg(a, 1, "size"), ow, oh);
        var rs = arg(a, 2, "resample", None);
        var boxv = arg(a, 3, "box", None);
        int resample = rs.is_none() ? 3 : (int)index_(rs);
        if (s->mode == "P") resample = 0;
        double bx0 = 0, by0 = 0, bx1 = s->w, by1 = s->h;
        if (!boxv.is_none()) {
            bx0 = boxv.getitem(var(0)).as_double(); by0 = boxv.getitem(var(1)).as_double();
            bx1 = boxv.getitem(var(2)).as_double(); by1 = boxv.getitem(var(3)).as_double();
            if (bx0 < 0 || by0 < 0 || bx1 > s->w || by1 > s->h || bx1 <= bx0 || by1 <= by0) raise(E_ValueError, "box can't exceed original image size");
        }
        if (ow <= 0 || oh <= 0) raise(E_ValueError, "height and width must be > 0");
        var out = make_img(s->mode, ow, oh);
        Img* d = I(out);
        d->pal = s->pal;
        d->info = s->info.call("copy");
        if (resample == 0) {
            // Geometry.c ImagingScaleAffine: the source coordinate starts at box + scale/2 and accumulates the scale
            double sx = (bx1 - bx0) / ow, sy = (by1 - by0) / oh;
            std::vector<int> xt(ow);
            double xx = bx0 + sx * 0.5;
            for (int x = 0; x < ow; x++) { xt[x] = xx < 0 ? -1 : std::min(s->w - 1, (int)xx); xx += sx; }
            double yy = by0 + sy * 0.5;
            for (int y = 0; y < oh; y++) {
                int yi = yy < 0 ? -1 : std::min(s->h - 1, (int)yy);
                yy += sy;
                if (yi < 0) continue;
                for (int x = 0; x < ow; x++)
                    if (xt[x] >= 0) memcpy(&d->px[((size_t)y * ow + x) * s->nb], &s->px[((size_t)yi * s->w + xt[x]) * s->nb], s->nb);
            }
            return out;
        }
        if (ow == s->w && oh == s->h && bx0 == 0 && by0 == 0 && bx1 == s->w && by1 == s->h) { d->px = s->px; return out; }
        std::string src = s->px;
        bool alpha = s->mode == "RGBA" || s->mode == "LA";
        int ac = s->nb - 1;
        if (alpha) {   // premultiplied (RGBa / La) around the resample
            for (size_t i = 0; i < src.size(); i += s->nb) {
                int al = (uint8_t)src[i + ac];
                for (int c = 0; c < ac; c++) { int t = (uint8_t)src[i + c] * al + 128; src[i + c] = (char)(((t >> 8) + t) >> 8); }
            }
        }
        std::string r = do_resample(src, s->w, s->h, s->nb, ow, oh, bx0, by0, bx1, by1, filter_of(resample));
        if (alpha) {
            for (size_t i = 0; i < r.size(); i += s->nb) {
                int al = (uint8_t)r[i + ac];
                for (int c = 0; c < ac; c++) if (al != 0 && al != 255) r[i + c] = (char)clip8(255 * (uint8_t)r[i + c] / al);
            }
        }
        d->px = r;
        return out;
    });
    add_method(T_img, "paste", [](const CallArgs& a) {
        Img* s = I(a.pos.at(0));
        var im = a.pos.at(1), box = arg(a, 2, "box", None), mask = arg(a, 3, "mask", None);
        if (!mask.is_none()) raise(E_NotImplementedError, "Image.paste with a mask is not supported by this runtime");
        bool is_image = im.is_ref() && im.ref()->type == T_img;
        if (box.is_none()) box = new_tuple({var(0), var(0)});
        int x0, y0, x1, y1;
        if (len_(box) == 2) {
            if (!is_image) raise(E_ValueError, "cannot determine region size; use 4-item box");
            x0 = (int)index_(box.getitem(var(0))); y0 = (int)index_(box.getitem(var(1)));
            x1 = x0 + I(im)->w; y1 = y0 + I(im)->h;
        } else box_args(s, box, x0, y0, x1, y1);
        if (is_image) {
            var conv = I(im)->mode == s->mode ? im : convert_to(im, s->mode);
            const Img* p = I(conv);
            if (p->w != x1 - x0 || p->h != y1 - y0) raise(E_ValueError, "images do not match");
            for (int y = y0; y < y1; y++) {
                if (y < 0 || y >= s->h) continue;
                int xa = std::max(x0, 0), xb = std::min(x1, s->w);
                if (xb <= xa) continue;
                memcpy(&s->px[((size_t)y * s->w + xa) * s->nb], &p->px[((size_t)(y - y0) * p->w + (xa - x0)) * s->nb], (size_t)(xb - xa) * s->nb);
            }
        } else {
            uint8_t ink[4];
            ink_of(s->mode, im, ink);
            for (int y = std::max(y0, 0); y < std::min(y1, s->h); y++)
                for (int x = std::max(x0, 0); x < std::min(x1, s->w); x++) memcpy(&s->px[((size_t)y * s->w + x) * s->nb], ink, s->nb);
        }
        return var(None);
    });
    add_method(T_img, "save", [](const CallArgs& a) {
        const Img* s = I(a.pos.at(0));
        var fp = arg(a, 1, "fp");
        var fmt = arg(a, 2, "format", None);
        std::string f;
        std::string path;
        bool is_path = is_str(fp) || (fp.is_ref() && fp.hasattr("__fspath__") && !fp.hasattr("write"));
        if (is_path) path = fs_path(fp);
        if (!fmt.is_none()) f = str_utf8(fmt);
        else if (is_path) {
            size_t dot = path.find_last_of('.');
            std::string ext = dot == std::string::npos ? "" : path.substr(dot);
            for (auto& c : ext) c = (char)tolower((unsigned char)c);
            if (ext == ".png") f = "PNG";
            else if (ext == ".ppm" || ext == ".pgm" || ext == ".pnm") f = "PPM";
            else raise(E_ValueError, "unknown file extension: " + ext);
        } else {
            std::string name = fp.hasattr("name") && is_str(fp.getattr("name")) ? str_utf8(fp.getattr("name")) : "";
            size_t dot = name.find_last_of('.');
            std::string ext = dot == std::string::npos ? "" : name.substr(dot);
            for (auto& c : ext) c = (char)tolower((unsigned char)c);
            if (ext == ".png") f = "PNG";
            else raise(E_ValueError, "unknown file extension: " + ext);
        }
        for (auto& c : f) c = (char)toupper((unsigned char)c);
        CallArgs kw;
        kw.kw = a.kw;
        std::string data;
        if (f == "PNG") data = encode_png(s, kw);
        else if (f == "PPM") data = encode_ppm(s);
        else raise(E_KeyError, f);
        if (is_path) {
            var out = import("builtins").call("open", S(path), S("wb"));
            out.call("write", new_bytes(data));
            out.call("close");
        } else fp.call("write", new_bytes(data));
        return var(None);
    });

    // load(): px[x, y] reads and writes
    add_method(T_pixels, "__getitem__", [](const CallArgs& a) {
        const Img* s = I(static_cast<Img*>(a.pos.at(0).ref())->info);
        int x, y;
        check_xy(s, a.pos.at(1), x, y);
        return pixel_var(s, (size_t)y * s->w + x);
    });
    add_method(T_pixels, "__setitem__", [](const CallArgs& a) {
        Img* s = I(static_cast<Img*>(a.pos.at(0).ref())->info);
        int x, y;
        check_xy(s, a.pos.at(1), x, y);
        uint8_t ink[4];
        ink_of(s->mode, a.pos.at(2), ink);
        memcpy(&s->px[((size_t)y * s->w + x) * s->nb], ink, s->nb);
        return var(None);
    });
}

}  // namespace

void init_pil_modules() {
    register_native_module("PIL", [](const var& m) {
        mod_set(m, "__version__", S("12.2.0"));
        g_unidentified = make_class("UnidentifiedImageError", "UnidentifiedImageError", {t::OSError}, new_dict(), {}, "PIL");
        mod_set(m, "UnidentifiedImageError", g_unidentified);
        mod_set(m, "Image", import("PIL.Image"));
    });
    register_native_module("PIL.Image", init_image);
}

}  // namespace py
