// codebreaker/formats/gfx.hpp -- typed C++ from codebreaker.formats.gfx.
//
// gfx.py -- Scaleform GFx user-interface movies, and their fonts as TrueType.
//
//     python -m codebreaker gfx scan|images|fonts|placements <file>
//     python -m codebreaker gfx font-ttf <file> <font id> <out.ttf> [family]
//     from codebreaker.formats import gfx
//     for offset, magic, body in gfx.find_movies(data): gfx.ext_images(body), gfx.fonts(body), gfx.placements(body)
//     gfx.font_to_ttf(data, font_id, "font.ttf")
//
// GFx is Scaleform's SWF-derived format ("GFX" uncompressed, "CFX" zlib, then an SWF tag stream),
// which many console games draw their menus with. A file may hold several movies, each wrapped by
// its engine (Heavy Iron's GoodEngine puts an engine header, the movie's name and asset ids before
// the GFX body), so movies are found by their magic and kept only where the tag stream parses and
// ends in an End tag -- an incidental "GFX" in shader source is not a movie. Read: external images
// (DefineExternalImage 1001/1008/1009), embedded fonts (DefineFont2/3), placements (PlaceObject2/3).
//
// A DefineFont3 font is vector outlines (SWF shape records: moves, lines, quadratic curves on a
// 20480-unit em), a code table and layout; font_to_ttf rebuilds it as a TrueType file (fontTools).
//
// Typing notes:
//   * find_movies / wrapper_name / parse_tags / ext_images / fonts / placements / _glyph_contours are formats.dll's questions
//     (native/formats/xbox/graphics.cc, linked in and asked through cbf_call).  find_movies is a generator in Python
//     (`yield from` the reader's list): here the list.  A contour's op is `Op` (the Python's ("m", x, y) / ("l", x, y) /
//     ("q", cx, cy, x, y) tuple: the letter and its numbers).
//   * font_to_ttf: the Python builds the TrueType file with fontTools (FontBuilder, TTGlyphPen), which the C++ runtime does
//     not have.  What it does of its own -- finding the font, reading its glyph shapes, codes and layout, scaling them to
//     2048 units, the glyph order, the metrics, the character map, the names -- is ported (it is `TtfFont`, in the order
//     and with the arguments the Python hands fontTools); the hand-off to fontTools is the hook `save_ttf` (TtfFont, the
//     output path), which the caller sets (the pen calls TTGlyphPen would get are `PenCall`s of each glyph).  Not faithful:
//     without the hook set font_to_ttf raises py::ImportError ("No module named 'fontTools'", as Python does without it).
//   * main() returns the exit status.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::formats::gfx {

// Reads bits most-significant-first from a bytes buffer, SWF convention.
class Bits {
  public:
    py::bytes d;
    int64_t p = 0;
    int64_t bit = 0;

    explicit Bits(const py::bytes& d, int64_t p = 0);

    // Read n bits as an unsigned integer.
    int64_t read(int64_t n);
    // Read n bits as a signed (two's-complement) integer.
    int64_t reads(int64_t n);
    // Advance to the next whole byte boundary.
    void align();
};

int64_t u16(const py::bytes& d, int64_t o);
int64_t u32(const py::bytes& d, int64_t o);
int64_t s16(const py::bytes& d, int64_t o);

// [(offset, magic, body)] of each *valid* GFx (uncompressed) / CFX (zlib) movie in data: the version byte, the
// declared length, and a tag stream that parses and ends in an End tag (so incidental "GFX"/"CFX" byte runs, e.g. in
// shader source, are not movies).
py::list<std::tuple<int64_t, py::str, py::bytes>> _find_movies(const py::bytes& data);

// Yield (offset, magic, body) for each *valid* GFx (uncompressed) / CFX (zlib) movie in data.
py::list<std::tuple<int64_t, py::str, py::bytes>> find_movies(const py::bytes& data);

// Best-effort movie name from the GoodEngine wrapper preceding the GFX magic.
// Returns "" instead of junk when there's no clean identifier (e.g. for FONT.ho).
py::str wrapper_name(const py::bytes& data, int64_t gfx_off);

// The SWF tag stream of a movie's body: [(code, bytes)].
py::list<std::tuple<int64_t, py::bytes>> parse_tags(const py::bytes& body);

// DefineExternalImage (1001/1008/1009): (charId, w, h, name).
py::list<std::tuple<int64_t, int64_t, int64_t, py::str>> ext_images(const py::bytes& body);

// DefineFont2/3: (version, fontId, name, numGlyphs, bold, hasLayout).
// `version` is the SWF font version (2 or 3), not the raw tag code (48/75).
py::list<std::tuple<int64_t, int64_t, py::str, int64_t, bool, bool>> fonts(const py::bytes& body);

// PlaceObject2(26)/PlaceObject3(70): (depth, charId, matrix); matrix is (scaleX, scaleY, translateX_px, translateY_px).
py::list<std::tuple<int64_t, std::optional<int64_t>, std::optional<std::tuple<double, double, double, double>>>> placements(const py::bytes& body);

// one segment of a glyph's outline: ("m", x, y), ("l", x, y) or ("q", cx, cy, x, y)
struct Op {
    py::str op;
    py::list<int64_t> pts;
};

// Parse one SWF glyph SHAPE into [[(op, *pts)...]] contours (m/l/q, SWF coords).
py::list<py::list<Op>> _glyph_contours(const py::bytes& data, int64_t start);

std::optional<py::bytes> _find_font_tag(const py::bytes& data, int64_t font_id);

// what TTGlyphPen is called with for one glyph: moveTo(pt) / lineTo(pt) / qCurveTo(*pts) / closePath()
struct PenCall {
    py::str op;
    py::list<std::tuple<int64_t, int64_t>> pts;
};

// The font as the Python hands it to fontTools' FontBuilder (see the notes above).
struct TtfFont {
    int64_t units_per_em = 0;                                          // FontBuilder(UPM, isTTF=True)
    py::list<py::str> glyph_order;                                     // setupGlyphOrder: ".notdef", "g0" ...
    py::dict<py::str, py::list<PenCall>> glyphs;                       // setupGlyf: the pen calls of each glyph (".notdef": none)
    py::dict<int64_t, py::str> cmap;                                   // setupCharacterMap
    py::dict<py::str, std::tuple<int64_t, int64_t>> metrics;           // setupHorizontalMetrics: (advance, left side bearing)
    int64_t ascent = 0, descent = 0;                                   // setupHorizontalHeader(ascent=, descent=)
    py::str family;                                                    // setupNameTable familyName / psName (styleName "Regular")
    int64_t os2_typo_ascender = 0, os2_typo_descender = 0, os2_win_ascent = 0, os2_win_descent = 0;   // setupOS2
};

// the hand-off to fontTools: writes the TrueType file (FontBuilder.setupGlyf ... setupPost, save): set by the caller
extern std::function<void(const TtfFont&, const py::str& out_path)> save_ttf;

// Parse DefineFont3 `font_id` from `data` and write a TTF to out_path. Returns (name, glyphs).
std::tuple<py::str, int64_t> font_to_ttf(const py::bytes& data, int64_t font_id, const py::str& out_path, const std::optional<py::str>& family = std::nullopt);

extern const py::str _USAGE;

int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::formats::gfx
