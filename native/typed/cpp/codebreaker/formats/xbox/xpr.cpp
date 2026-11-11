// codebreaker/formats/xbox/xpr.cpp -- see xpr.hpp.

#include "codebreaker/formats/xbox/xpr.hpp"

#include <cmath>

#include "codebreaker/formats/native_ask.hpp"
#include "codebreaker/formats/xbox/texture.hpp"
#include "py_os.hpp"

namespace codebreaker::formats::xbox::xpr {

namespace {

namespace na = native_ask;
namespace tex = codebreaker::formats::xbox::texture;

// math.log2(x) as int(...) takes it: ValueError for x <= 0
int64_t int_log2(int64_t x) {
    if (x <= 0) throw py::ValueError("math domain error");
    return int64_t(std::log2(double(x)));
}

// "%s" of an info value (None, a number)
py::str shown(const std::optional<int64_t>& v) { return v ? py::to_str(*v) : py::str("None"); }

}  // namespace

const py::str DESCRIPTION("extract textures from Xbox .xpr resource bundles (XPR0 original / XPR2 360) -> DDS");

py::bytes build_xpr0(const py::bytes& pixels, int64_t width, int64_t height, int64_t xfmt, int64_t mipmaps) {
    int64_t log2w = int_log2(width), log2h = int_log2(height);
    int64_t fmtdw = ((xfmt & 0xFF) << 8) | (2 << 4) | ((mipmaps & 0xF) << 16) | ((log2w & 0xF) << 20) | ((log2h & 0xF) << 24);
    int64_t header_size = 0x1C;
    py::bytes hdr = py::bytes("XPR0") + py::pack("<II", header_size + py::len(pixels), header_size);
    hdr = hdr + py::pack("<IIII", int64_t(0), header_size, int64_t(0), fmtdw);       // D3DTexture resource
    hdr = hdr + py::bytes::zeros(header_size - py::len(hdr));
    return hdr + pixels;
}

Info parse(const py::bytes& data) {
    namespace nat = py::native;
    nat::Val v = na::ask_val("xpr.parse", data, na::raises<XprError>("XprError"));
    const nat::Val* container = v.get("container");
    if (container && container->s == "XPR0") return nat::read<Xpr0>(v);
    return nat::read<Xpr2>(v);
}

py::bytes to_dds(const Info& info, std::optional<int64_t> width, std::optional<int64_t> height) {
    if (std::holds_alternative<Xpr0>(info)) {
        const Xpr0& x = std::get<Xpr0>(info);
        if (!x.dds_fmt || x.dds_fmt->empty())
            throw XprError(py::percent(py::str("XPR0 format %s is not a DXT surface (no DDS path yet)"), x.format_name));
        return tex::to_dds(x.pixels, x.width, x.height, *x.dds_fmt);
    }
    // XPR2: caller supplies dimensions + assumed dxt5 unless told otherwise
    const Xpr2& x = std::get<Xpr2>(info);
    std::optional<int64_t> w = (width && *width) ? width : std::nullopt;       // width or info.get("width"): an XPR2 has none
    std::optional<int64_t> h = (height && *height) ? height : std::nullopt;
    if (!(w && h)) throw XprError("XPR2 needs --width/--height to untile (no resource dims)");
    py::bytes linear = tex::untile(tex::swap16(x.pixels), *w, *h, py::str("dxt5"));
    return tex::to_dds(linear, *w, *h, py::str("dxt5"));
}

// ── self-verification ──────────────────────────────────────────────────────
std::tuple<bool, py::str> _selfcheck() {
    // a 64x32 DXT1 surface = 64*32/2 = 1024 bytes; fill with a recognisable ramp
    int64_t w = 64, h = 32;
    std::string ramp;
    for (int64_t i = 0; i < py::floordiv(w * h, 2); ++i) ramp.push_back(char((i * 7) & 0xFF));
    py::bytes pixels(ramp);
    py::bytes blob = build_xpr0(pixels, w, h, 0x0C);
    Info info = parse(blob);
    const Xpr0* x = std::get_if<Xpr0>(&info);
    if (!x || !x->dds_fmt || *x->dds_fmt != py::str("dxt1")) {
        py::str shown_info = x ? py::repr(*x) : py::repr(std::get<Xpr2>(info));
        return std::make_tuple(false, py::percent(py::str("format decode wrong: %s"), shown_info));
    }
    if (x->width != w || x->height != h) return std::make_tuple(false, py::percent(py::str("dims wrong: %dx%d"), x->width, x->height));
    if (x->pixels != pixels) return std::make_tuple(false, py::str("pixel round-trip mismatch"));
    py::bytes dds = to_dds(info);
    if (dds.slice(0, 4) != py::bytes("DDS ") || dds.slice(-py::len(pixels), py::len(dds)) != pixels)
        return std::make_tuple(false, py::str("DDS wrap wrong"));
    // XPR2 header round-trip (magic + 2 sizes + 8 pad = 0x14 before pixels)
    py::bytes x2 = py::bytes("XPR2") + py::pack(">II", int64_t(0x14), py::len(pixels)) + py::bytes::zeros(8) + pixels;
    Info i2 = parse(x2);
    const Xpr2* y = std::get_if<Xpr2>(&i2);
    if (!y || y->data_offset != 0x14 || y->pixels != pixels) return std::make_tuple(false, py::str("XPR2 header parse wrong"));
    return std::make_tuple(true, py::str("XPR0 build/parse/DDS round-trip (64x32 DXT1) + XPR2 header"));
}

void main(const py::list<py::str>& a) {
    auto S = [](const char* s) { return py::str(s); };
    if (a.empty() || a[0] == S("-h") || a[0] == S("--help")) {
        py::print(S("usage:\n"
                    "  xpr info <file.xpr>              show container + texture info\n"
                    "  xpr dds  <file.xpr> <out.dds> [W H]   extract texture to DDS\n"
                    "  xpr --selftest"));
        return;
    }
    if (a[0] == S("--selftest")) {
        auto [ok, msg] = _selfcheck();
        py::print((ok ? S("PASS: ") : S("FAIL: ")) + msg);
        throw py::SystemExit(ok ? 0 : 1);
    }
    py::str op = a[0];
    py::bytes data = py::open_rb(a[1]).read();
    Info info = parse(data);
    if (op == S("info")) {
        // for k in (container, format_name, dds_fmt, width, height, mipmaps, cubemap, data_offset, header_size, data_size): if k in info
        auto row = [&](const char* k, const py::str& value) { py::print(py::percent(S("  %-12s %s"), S(k), value)); };
        if (const Xpr0* x = std::get_if<Xpr0>(&info)) {
            row("container", x->container);
            row("format_name", x->format_name);
            row("dds_fmt", x->dds_fmt ? *x->dds_fmt : S("None"));
            row("width", py::to_str(x->width));
            row("height", py::to_str(x->height));
            row("mipmaps", py::to_str(x->mipmaps));
            row("cubemap", py::to_str(x->cubemap));
            row("data_offset", py::to_str(x->data_offset));
        } else {
            const Xpr2& y = std::get<Xpr2>(info);
            row("container", y.container);
            row("data_offset", py::to_str(y.data_offset));
            row("header_size", py::to_str(y.header_size));
            row("data_size", py::to_str(y.data_size));
        }
        return;
    }
    if (op == S("dds")) {
        py::str out = a[2];
        std::optional<int64_t> w = py::len(a) > 3 ? std::optional<int64_t>(py::int_parse(a[3])) : std::nullopt;
        std::optional<int64_t> h = py::len(a) > 4 ? std::optional<int64_t>(py::int_parse(a[4])) : std::nullopt;
        py::bytes dds = to_dds(info, w, h);
        {
            py::file f = py::open(out, S("wb"));
            f.write(dds);
            f.close();
        }
        // info.get("format_name", "?"), w or info.get("width"), h or info.get("height")
        py::str name = S("?");
        std::optional<int64_t> iw, ih;
        if (const Xpr0* x = std::get_if<Xpr0>(&info)) {
            name = x->format_name;
            iw = x->width;
            ih = x->height;
        }
        py::print(py::percent(S("wrote %s (%d bytes, %s %sx%s)"), out, py::len(dds), name, shown((w && *w) ? w : iw),
                              shown((h && *h) ? h : ih)));
        return;
    }
    py::print(py::percent(S("unknown op: %s"), op));
    throw py::SystemExit(2);
}

}  // namespace codebreaker::formats::xbox::xpr
