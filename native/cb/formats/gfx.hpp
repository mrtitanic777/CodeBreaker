// cb/formats/gfx.hpp -- Scaleform GFx user-interface movies, and their fonts as TrueType.
//
//     for (const gfx::Movie& movie : gfx::find_movies(data))        its offset, "GFX" or "CFX", and the SWF body (inflated)
//         gfx::external_images(movie.body), gfx::fonts(movie.body), gfx::placements(movie.body)
//     gfx::font_to_ttf(data, font_id)                               a DefineFont3 font as a TrueType file
//
// GFx is Scaleform's SWF-derived format ("GFX" uncompressed, "CFX" zlib, then an SWF tag stream), which many console games draw their
// menus with. A file may hold several movies, each wrapped by its engine (Heavy Iron's GoodEngine puts an engine header, the movie's name
// and asset ids before the GFX body), so movies are found by their magic and kept only where the tag stream parses and ends in an End tag
// -- an incidental "GFX" in shader source is not a movie. Read: external images (DefineExternalImage 1001/1008/1009), embedded fonts
// (DefineFont2/3), placements (PlaceObject2/3).
//
// A DefineFont3 font is vector outlines (SWF shape records: moves, lines, quadratic curves on a 20480-unit em), a code table and layout;
// font_to_ttf rebuilds it as a TrueType file: 2048 units to the em, a glyph `g<N>` for each glyph, a cmap, the metrics, a name table.
//
//     gfx scan|images|fonts|placements <file>
//     gfx font-ttf <file> <font id> <out.ttf> [family]
#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/cli.hpp"
#include "cb/core/error.hpp"

namespace cb::formats::gfx {

// One movie of a file.
struct Movie {
    std::size_t offset = 0;   // where its magic is
    std::string magic;        // "GFX" or "CFX"
    Bytes body;               // the SWF body (a CFX movie inflated): the frame size, rate and count, then the tags
};

// Each valid GFx (uncompressed) / CFX (zlib) movie in `data`: the version byte, the declared length, and a tag stream that parses and
// ends in an End tag (so incidental "GFX"/"CFX" byte runs, e.g. in shader source, are not movies). In the order of their offsets.
std::vector<Movie> find_movies(ByteView data);

// The movie's name from the GoodEngine wrapper before the magic at `offset`: the last identifier-looking run of text there ("" when there
// is no clean one, e.g. for FONT.ho).
std::string wrapper_name(ByteView data, std::size_t offset);

struct Tag {
    unsigned code = 0;
    Bytes body;
};
// The tag stream of a movie's body, up to and including the End tag. TruncatedError when the body is cut short.
std::vector<Tag> parse_tags(ByteView body);

struct ExternalImage {
    unsigned character_id = 0;
    unsigned width = 0, height = 0;
    std::string name;
};
std::vector<ExternalImage> external_images(ByteView body);   // DefineExternalImage 1001 / 1008 / 1009

struct FontInfo {
    int version = 0;          // the SWF font version (2 or 3), not the tag code (48 / 75)
    unsigned id = 0;
    std::string name;
    unsigned glyphs = 0;
    bool bold = false, layout = false;
};
std::vector<FontInfo> fonts(ByteView body);                  // DefineFont2 / DefineFont3

struct Matrix {
    double scale_x = 1, scale_y = 1, translate_x = 0, translate_y = 0;   // translation in pixels
};
struct Placement {
    unsigned depth = 0;
    std::optional<unsigned> character_id;
    std::optional<Matrix> matrix;
};
// PlaceObject2 / PlaceObject3; a tag too short to read is left out.
std::vector<Placement> placements(ByteView body);

// One step of a glyph's outline: a move, a straight line, or a quadratic curve (control point, then the end), in SWF coordinates.
struct PathOp {
    enum class Kind { Move, Line, Curve } kind = Kind::Move;
    std::int64_t x = 0, y = 0;                 // the point (a curve's end)
    std::int64_t control_x = 0, control_y = 0; // a curve's control point
    friend bool operator==(const PathOp&, const PathOp&) = default;
};
using Contour = std::vector<PathOp>;
// The glyph shape whose records start at byte `start` of `data`, as contours.
std::vector<Contour> glyph_contours(ByteView data, std::size_t start);

// The result of font_to_ttf: the font's name and glyph count, and the TrueType file.
struct TrueType {
    std::string name;
    std::size_t glyphs = 0;
    Bytes file;
};
// The DefineFont3 font `font_id` of the first movie of `data` that has it, as TrueType (family: its name without '-' unless given).
// NotFoundError-like FormatError when no movie has the font.
TrueType font_to_ttf(ByteView data, unsigned font_id, const std::optional<std::string>& family = std::nullopt);

struct Environment {};

// `gfx scan|images|fonts|placements <file>` and `gfx font-ttf <file> <font id> <out.ttf> [family]`.
cli::Command command(const Environment& environment = {});

}  // namespace cb::formats::gfx
