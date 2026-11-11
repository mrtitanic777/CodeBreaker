// codebreaker/formats/fonts.cpp -- see fonts.hpp.

#include "codebreaker/formats/fonts.hpp"

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/formats/detect.hpp"
#include "codebreaker/formats/native_ask.hpp"
#include "codebreaker/formats/ne.hpp"
#include "py_os.hpp"

namespace codebreaker::formats::fonts {

namespace na = native_ask;

Font::Font(const py::bytes& data_, int64_t at_) : data(data_), at(at_) {
    // (the reader's answer, the Font's attributes)
    FontAttrs a = na::ask<FontAttrs>("fonts.font", data_, na::no_own, at_);
    version = a.version;
    size = a.size;
    copyright = a.copyright;
    vector = a.vector;
    points = a.points;
    ascent = a.ascent;
    italic = a.italic;
    weight = a.weight;
    charset = a.charset;
    width = a.width;
    height = a.height;
    average_width = a.average_width;
    max_width = a.max_width;
    first = a.first;
    last = a.last;
    default_ = a.default_;
    row_bytes = a.row_bytes;
    name = a.name;
    bits = a.bits;
    _where = a._where;
    widths = a.widths;
}

py::str Font::kind() const { return vector ? py::str("vector") : width ? py::str("fixed") : py::str("proportional"); }

py::list<py::str> Font::glyph(int64_t code) const { return na::ask<py::list<py::str>>("fonts.glyph", data, na::no_own, at, code); }

py::list<py::str> Font::glyph(const py::str& ch) const {
    // ord(ch)
    if (ch.size() != 1)
        throw py::TypeError(py::percent(py::str("ord() expected a character, but string of length %d found"), ch.size()));
    return glyph(int64_t(ch.u32()[0]));
}

py::list<py::str> Font::render(const py::str& text) const { return na::ask<py::list<py::str>>("fonts.render", data, na::no_own, at, text); }

py::str Font::describe() const {
    return py::percent(py::str("%-14s %2dpt %2dpx  %-12s chars %d..%d  version %x.%02x"), name.empty() ? py::str("?") : name, points, height, kind(),
                       first, last, version >> 8, version & 0xFF);
}

Font parse(const py::bytes& data, int64_t at) {
    if (at + 0x76 > py::len(data) ||
        !([&] {
            int64_t v = py::unpack_from1<int64_t>("<H", data, at);
            return v == 0x0100 || v == 0x0200 || v == 0x0300;
        })())
        throw py::ValueError(py::percent(py::str("no font at %#x"), at));
    return Font(data, at);
}

py::list<Font> fonts_in(const py::str& path) {
    py::list<Font> out;
    auto kind = detect::format_of(path);
    if (kind && *kind == py::str("NE")) {
        ne::NEFile nef(path);
        for (const ne::Resource& r : nef.resources()) {
            if (r.type == py::str("FONT")) {
                try {
                    out.append(parse(nef.data, r.off));
                } catch (const py::ValueError&) {
                } catch (const py::StructError&) {
                }
            }
        }
        return out;
    }
    py::bytes data = py::open_rb(path).read();
    try {
        out.append(parse(data));
    } catch (const py::ValueError&) {
    } catch (const py::StructError&) {
    }
    return out;
}

int64_t main(const py::list<py::str>& argv) {
    auto S = [](const char* s) { return py::str(s); };
    if (argv.empty()) {
        // (main.__doc__: the docstring with its common indentation removed, as CPython 3.13+ compiles it)
        py::print(S("python -m codebreaker fonts FILE [--index N] [--show TEXT]\n\n"
                    "List the fonts in a .FON or .FNT file (a module name of the selected corpus will do), and\n"
                    "draw TEXT in one of them."));
        return 2;
    }
    std::optional<py::str> path = py::os::path::isfile(argv[0]) ? std::optional<py::str>(argv[0]) : base::corpus::retail(argv[0]);
    if (!path || path->empty()) throw py::SystemExit(py::percent(S("no file %r"), argv[0]));
    auto found = fonts_in(*path);
    py::print(py::percent(S("%s: %d font(s)"), py::os::path::basename(*path), found.size()));
    int64_t i = 0;
    for (const Font& f : found) {
        py::print(py::percent(S("  [%d] %s"), i, f.describe()));
        i += 1;
    }
    bool has_show = argv.contains(S("--show"));
    if (has_show && !found.empty()) {
        // argv.index(...) + 1
        auto index_of = [&](const char* what) { return argv.index(S(what)); };
        const Font& f = found[argv.contains(S("--index")) ? py::int_parse(argv[index_of("--index") + 1]) : 0];
        if (f.vector) {
            py::print(S("  a vector font has no bitmaps to draw"));
            return 0;
        }
        for (const py::str& line : f.render(argv[index_of("--show") + 1])) py::print(S("  ") + line);
    }
    return 0;
}

}  // namespace codebreaker::formats::fonts
