// codebreaker/formats/fonts.hpp -- typed C++ from codebreaker.formats.fonts.
//
// fonts.py -- raster fonts: the .FNT format, and the .FON files that hold several of them.
//
//     fonts::fonts_in(path)       [Font] -- every font in a .FON, or the one in a bare .FNT
//     fonts::parse(data, at)      Font   -- one .FNT image inside `data`
//     font.glyph("A")             rows of '#' and ' ' for one character (raster fonts)
//     font.render("Hello")        the same for a line of text
//
//     python -m codebreaker fonts FILE [--index N] [--show TEXT]
//
// A .FON is a 16-bit module with no code whose FONT resources are each a .FNT image. The .FNT
// header is the same in every version up to the character table; what follows differs:
//
//     1.x  (version 0x0100)  one bitmap for the whole font ("strike"): `height` rows of
//                            `row_bytes` bytes, the characters side by side. Fixed-pitch fonts
//                            have no table (character i starts at column i * width); proportional
//                            ones have a table of column offsets, one word per character plus one.
//     2.x  (version 0x0200)  a table of (width, offset:16) per character at 0x76; each glyph is its
//                            own bitmap, stored a byte-column at a time (`height` bytes per column).
//     3.x  (version 0x0300)  the same with (width, offset:32) at 0x94.
//
// Fonts whose type has bit 0 set are stroke ("vector") fonts -- ROMAN, MODERN, SCRIPT: they are
// listed with their metrics but have no bitmaps to draw.
//
// Typing notes:
//   * The font reader and the glyph / render drawing are formats.dll's (native/formats/win/fonts.cc, linked in and asked
//     through cbf_call).  The Python's Font copies the answer's dict into its attributes: here they are the fields of the
//     class (`default` is `default_`: a C++ keyword).  `widths` / `_where` are {code: pixels} / {code: (width, offset)}.
//   * glyph(ch) takes a str (ord(ch): TypeError unless it is one character) or an int: two overloads.
//   * fonts_in() reads an NE module with the typed NE reader (formats/ne.hpp) and detect (formats/detect.hpp).
//   * main() returns the exit status; `raise SystemExit("no file ...")` is py::SystemExit(str).
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::formats::fonts {

// what formats.dll answers for a font: the Font's attributes
struct FontAttrs {
    int64_t version = 0;
    int64_t size = 0;
    py::str copyright;
    bool vector = false;
    int64_t points = 0;
    int64_t ascent = 0;
    bool italic = false;
    int64_t weight = 0;
    int64_t charset = 0;
    int64_t width = 0;
    int64_t height = 0;
    int64_t average_width = 0;
    int64_t max_width = 0;
    int64_t first = 0;
    int64_t last = 0;
    int64_t default_ = 0;
    int64_t row_bytes = 0;
    py::str name;
    int64_t bits = 0;
    py::dict<int64_t, std::tuple<int64_t, int64_t>> _where;
    py::dict<int64_t, int64_t> widths;
};
template <class F> void for_each_field_mut(FontAttrs& a, F&& f) {
    f("version", a.version, true);
    f("size", a.size, true);
    f("copyright", a.copyright, true);
    f("vector", a.vector, true);
    f("points", a.points, true);
    f("ascent", a.ascent, true);
    f("italic", a.italic, true);
    f("weight", a.weight, true);
    f("charset", a.charset, true);
    f("width", a.width, true);
    f("height", a.height, true);
    f("average_width", a.average_width, true);
    f("max_width", a.max_width, true);
    f("first", a.first, true);
    f("last", a.last, true);
    f("default", a.default_, true);
    f("row_bytes", a.row_bytes, true);
    f("name", a.name, true);
    f("bits", a.bits, true);
    f("_where", a._where, true);
    f("widths", a.widths, true);
}

// One font. `widths` is {character code: pixels} for raster fonts. Read by formats.dll
// (native/formats/win/fonts.cc), which also draws glyph() and render().
class Font {
  public:
    py::bytes data;
    int64_t at = 0;
    int64_t version = 0, size = 0;
    py::str copyright;
    bool vector = false;
    int64_t points = 0, ascent = 0;
    bool italic = false;
    int64_t weight = 0, charset = 0, width = 0, height = 0, average_width = 0, max_width = 0, first = 0, last = 0, default_ = 0, row_bytes = 0;
    py::str name;
    int64_t bits = 0;
    py::dict<int64_t, std::tuple<int64_t, int64_t>> _where;
    py::dict<int64_t, int64_t> widths;

    Font(const py::bytes& data, int64_t at);

    py::str kind() const;                      // "vector" / "fixed" / "proportional"

    // One character as `height` strings of '#' and ' '. A character the font does not have
    // is drawn as its default character; a vector font draws nothing.
    py::list<py::str> glyph(const py::str& ch) const;
    py::list<py::str> glyph(int64_t code) const;
    py::list<py::str> render(const py::str& text) const;
    py::str describe() const;
};

// The font whose .FNT image starts at `at`.
Font parse(const py::bytes& data, int64_t at = 0);

// [Font] -- the FONT resources of a .FON (or of any 16-bit module), or a bare .FNT file's one
// font. A file that holds none gives an empty list.
py::list<Font> fonts_in(const py::str& path);

// python -m codebreaker fonts FILE [--index N] [--show TEXT]
//
// List the fonts in a .FON or .FNT file (a module name of the selected corpus will do), and
// draw TEXT in one of them.
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::formats::fonts
