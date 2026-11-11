// codebreaker/formats/gfx.cpp -- see gfx.hpp.

#include "codebreaker/formats/gfx.hpp"

#include <cmath>

#include "codebreaker/formats/native_ask.hpp"
#include "py_os.hpp"

namespace codebreaker::formats::gfx {

namespace na = native_ask;

std::function<void(const TtfFont&, const py::str& out_path)> save_ttf;

const py::str _USAGE(
    "gfx.py -- Scaleform GFx user-interface movies, and their fonts as TrueType.\n"
    "\n"
    "    python -m codebreaker gfx scan|images|fonts|placements <file>\n"
    "    python -m codebreaker gfx font-ttf <file> <font id> <out.ttf> [family]\n"
    "    from codebreaker.formats import gfx\n"
    "    for offset, magic, body in gfx.find_movies(data): gfx.ext_images(body), gfx.fonts(body), gfx.placements(body)\n"
    "    gfx.font_to_ttf(data, font_id, \"font.ttf\")");

// ---- Bits ----------------------------------------------------------------------------------------------------------
Bits::Bits(const py::bytes& d_, int64_t p_) : d(d_), p(p_), bit(0) {}

int64_t Bits::read(int64_t n) {
    // (a value past 62 bits is past int64: Python goes on with a big integer)
    if (n > 62) throw py::OverflowError("a bit field of more than 62 bits");
    int64_t v = 0;
    for (int64_t i = 0; i < n; ++i) {
        v = (v << 1) | ((d[p] >> (7 - bit)) & 1);
        bit += 1;
        if (bit == 8) {
            bit = 0;
            p += 1;
        }
    }
    return v;
}

int64_t Bits::reads(int64_t n) {
    int64_t v = read(n);
    return (n && (v >> (n - 1))) ? v - (int64_t(1) << n) : v;
}

void Bits::align() {
    if (bit) {
        bit = 0;
        p += 1;
    }
}

int64_t u16(const py::bytes& d, int64_t o) { return py::unpack_from1<int64_t>("<H", d, o); }
int64_t u32(const py::bytes& d, int64_t o) { return py::unpack_from1<int64_t>("<I", d, o); }
int64_t s16(const py::bytes& d, int64_t o) { return py::unpack_from1<int64_t>("<h", d, o); }

// ---- the reader's questions ----------------------------------------------------------------------------------------
py::list<std::tuple<int64_t, py::str, py::bytes>> _find_movies(const py::bytes& data) {
    return na::ask<py::list<std::tuple<int64_t, py::str, py::bytes>>>("gfx.find_movies", data, na::no_own);
}

py::list<std::tuple<int64_t, py::str, py::bytes>> find_movies(const py::bytes& data) { return _find_movies(data); }

py::str wrapper_name(const py::bytes& data, int64_t gfx_off) { return na::ask<py::str>("gfx.wrapper_name", data, na::no_own, gfx_off); }

py::list<std::tuple<int64_t, py::bytes>> parse_tags(const py::bytes& body) {
    return na::ask<py::list<std::tuple<int64_t, py::bytes>>>("gfx.parse_tags", body, na::no_own);
}

py::list<std::tuple<int64_t, int64_t, int64_t, py::str>> ext_images(const py::bytes& body) {
    return na::ask<py::list<std::tuple<int64_t, int64_t, int64_t, py::str>>>("gfx.ext_images", body, na::no_own);
}

py::list<std::tuple<int64_t, int64_t, py::str, int64_t, bool, bool>> fonts(const py::bytes& body) {
    return na::ask<py::list<std::tuple<int64_t, int64_t, py::str, int64_t, bool, bool>>>("gfx.fonts", body, na::no_own);
}

py::list<std::tuple<int64_t, std::optional<int64_t>, std::optional<std::tuple<double, double, double, double>>>> placements(const py::bytes& body) {
    return na::ask<py::list<std::tuple<int64_t, std::optional<int64_t>, std::optional<std::tuple<double, double, double, double>>>>>(
        "gfx.placements", body, na::no_own);
}

py::list<py::list<Op>> _glyph_contours(const py::bytes& data, int64_t start) {
    namespace nat = py::native;
    nat::Val v = na::ask_val("gfx.glyph_contours", data, na::no_own, start);
    py::list<py::list<Op>> out;
    for (const nat::Val& contour : v.items) {
        py::list<Op> ops;
        for (const nat::Val& t : contour.items) {                 // (letter, numbers...)
            Op op;
            op.op = py::str(t.items.at(0).s);
            for (size_t k = 1; k < t.items.size(); ++k) op.pts.append(t.items[k].i);
            ops.append(op);
        }
        out.append(ops);
    }
    return out;
}

std::optional<py::bytes> _find_font_tag(const py::bytes& data, int64_t font_id) {
    for (const auto& movie : find_movies(data))
        for (const auto& tag : parse_tags(std::get<2>(movie))) {
            const py::bytes& d = std::get<1>(tag);
            if (std::get<0>(tag) == 75 && u16(d, 0) == font_id) return d;
        }
    return std::nullopt;
}

std::tuple<py::str, int64_t> font_to_ttf(const py::bytes& data, int64_t font_id, const py::str& out_path, const std::optional<py::str>& family) {
    if (!save_ttf) throw py::ImportError("No module named 'fontTools'");
    std::optional<py::bytes> found = _find_font_tag(data, font_id);
    if (!found || found->empty()) throw py::KeyError(py::percent(py::str("DefineFont3 id=%d not found"), font_id));
    const py::bytes& fontdata = *found;
    int64_t flags = fontdata[2], nlen = fontdata[4];
    py::bytes raw_name = fontdata.slice(5, 5 + nlen);
    int64_t nul = raw_name.find(py::bytes(std::string(1, '\0')));
    py::str name = (nul < 0 ? raw_name : raw_name.slice(0, nul)).decode("latin1");
    int64_t p = 5 + nlen;
    int64_t n = u16(fontdata, p);
    p += 2;
    int64_t ot = p;
    bool wide = (flags & 0x08) != 0;
    py::list<int64_t> offs;
    if (wide) {
        for (int64_t k = 0; k <= n; ++k) offs.append(py::unpack_from1<int64_t>("<I", fontdata, ot + 4 * k));
    } else {
        for (int64_t k = 0; k <= n; ++k) offs.append(u16(fontdata, ot + 2 * k));
    }
    py::list<py::list<py::list<Op>>> shapes;
    for (int64_t g = 0; g < n; ++g) shapes.append(_glyph_contours(fontdata, ot + offs[g]));
    int64_t ctp = ot + offs[n];
    py::list<int64_t> codes;
    for (int64_t k = 0; k < n; ++k) codes.append(u16(fontdata, ctp + 2 * k));
    int64_t lp = ctp + 2 * n;
    int64_t ascent = s16(fontdata, lp), descent = s16(fontdata, lp + 2);
    lp += 6;
    py::list<int64_t> advances;
    for (int64_t k = 0; k < n; ++k) advances.append(s16(fontdata, lp + 2 * k));

    const int64_t EM = 20480, UPM = 2048;
    auto sc = [&](int64_t v) { return int64_t(std::nearbyint(double(v * UPM) / double(EM))); };   // round(v * UPM / EM): half to even
    TtfFont font;
    font.units_per_em = UPM;
    font.glyph_order.append(py::str(".notdef"));
    for (int64_t g = 0; g < n; ++g) font.glyph_order.append(py::percent(py::str("g%d"), g));
    font.glyphs.set(py::str(".notdef"), py::list<PenCall>{});
    // (an advance below zero, which a font's layout table can hold, is zero in TrueType's)
    font.metrics.set(py::str(".notdef"), std::make_tuple(advances.empty() ? int64_t(600) : std::max<int64_t>(0, sc(advances[0])), int64_t(0)));
    for (int64_t g = 0; g < n; ++g) {
        py::list<PenCall> pen;
        bool open_c = false;
        for (const py::list<Op>& c : shapes[g]) {
            for (const Op& op : c) {
                if (op.op == py::str("m")) {
                    if (open_c) pen.append(PenCall{py::str("closePath"), {}});
                    pen.append(PenCall{py::str("moveTo"), py::list<std::tuple<int64_t, int64_t>>{std::make_tuple(sc(op.pts[0]), -sc(op.pts[1]))}});
                    open_c = true;
                } else if (op.op == py::str("l")) {
                    pen.append(PenCall{py::str("lineTo"), py::list<std::tuple<int64_t, int64_t>>{std::make_tuple(sc(op.pts[0]), -sc(op.pts[1]))}});
                } else {
                    pen.append(PenCall{py::str("qCurveTo"),
                                       py::list<std::tuple<int64_t, int64_t>>{std::make_tuple(sc(op.pts[0]), -sc(op.pts[1])),
                                                                              std::make_tuple(sc(op.pts[2]), -sc(op.pts[3]))}});
                }
            }
            if (open_c) {
                pen.append(PenCall{py::str("closePath"), {}});
                open_c = false;
            }
        }
        py::str gname = py::percent(py::str("g%d"), g);
        font.glyphs.set(gname, pen);
        font.metrics.set(gname, std::make_tuple(std::max<int64_t>(0, sc(advances[g])), int64_t(0)));
        if (codes[g] && !font.cmap.contains(codes[g])) font.cmap.set(codes[g], gname);
    }
    int64_t asc = sc(ascent), dsc = -std::abs(sc(descent));
    font.ascent = asc;
    font.descent = dsc;
    font.os2_typo_ascender = asc;
    font.os2_typo_descender = dsc;
    font.os2_win_ascent = asc;
    font.os2_win_descent = -dsc;
    font.family = (family && !family->empty()) ? *family : name.replace(py::str("-"), py::str(""));
    save_ttf(font, out_path);
    return std::make_tuple(name, n);
}

int64_t main(const py::list<py::str>& argv) {
    auto S = [](const char* s) { return py::str(s); };
    if (argv.size() < 2) {
        py::print(_USAGE);
        return 2;
    }
    py::str command = argv[0], path = argv[1];
    py::bytes data = py::open_rb(path).read();
    if (command == S("font-ttf") && argv.size() > 3) {
        int64_t id = py::int_parse(argv[2]);
        auto r = font_to_ttf(data, id, argv[3], argv.size() > 4 ? std::optional<py::str>(argv[4]) : std::nullopt);
        py::print(py::percent(S("%r: %d glyphs -> %s"), std::get<0>(r), std::get<1>(r), argv[3]));
        return 0;
    }
    auto movies = find_movies(data);
    if (movies.empty()) py::print(py::percent(S("no GFx movies in %s"), path));
    for (const auto& movie : movies) {
        int64_t off = std::get<0>(movie);
        const py::str& magic = std::get<1>(movie);
        const py::bytes& body = std::get<2>(movie);
        py::str name = wrapper_name(data, off);
        if (command == S("scan")) {
            auto imgs = ext_images(body);
            auto fnts = fonts(body);
            py::print(py::percent(S("%s at $%X  %-28s %d images, %d fonts"), magic, off, name, imgs.size(), fnts.size()));
        } else if (command == S("images")) {
            for (const auto& im : ext_images(body))
                py::print(py::percent(S("%-28s character %-4d %4dx%-4d %s"), name, std::get<0>(im), std::get<1>(im), std::get<2>(im), std::get<3>(im)));
        } else if (command == S("fonts")) {
            for (const auto& f : fonts(body))
                py::print(py::percent(S("%-24s DefineFont%d id %d %-20r %d glyphs%s%s"), name, std::get<0>(f), std::get<1>(f), std::get<2>(f), std::get<3>(f),
                                      std::get<4>(f) ? S(" bold") : S(""), std::get<5>(f) ? S(" layout") : S("")));
        } else if (command == S("placements")) {
            py::dict<int64_t, py::str> images;
            for (const auto& im : ext_images(body)) images[std::get<0>(im)] = std::get<3>(im);
            for (const auto& pl : placements(body)) {
                const std::optional<int64_t>& cid = std::get<1>(pl);
                const auto& matrix = std::get<2>(pl);
                // (cid in images: a None cid is not a key)
                if (cid && images.contains(*cid) && matrix) {
                    double sx = std::get<0>(*matrix), sy = std::get<1>(*matrix), tx = std::get<2>(*matrix), ty = std::get<3>(*matrix);
                    py::print(py::percent(S("%-24s depth %-3d %-22s at (%.1f, %.1f) scale (%.3f, %.3f)"), name, std::get<0>(pl), images.at(*cid), tx, ty, sx, sy));
                }
            }
        } else {
            py::print(_USAGE);
            return 2;
        }
    }
    return 0;
}

}  // namespace codebreaker::formats::gfx
