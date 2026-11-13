// cb/formats/fonts.hpp -- raster fonts: the .FNT format, and the .FON files that hold several of them.
//
//     for (const fonts::Font& font : fonts::fonts_in(path))     every font of a .FON, or the one in a bare .FNT
//         font.glyph('A')                                       rows of '#' and ' ' for one character
//         font.render("Hello")                                  the same for a line of text
//     fonts::Font::parse(data, at)                              one .FNT image inside `data`
//
// A .FON is a 16-bit module with no code whose FONT resources are each a .FNT image. The .FNT header is the same in every version up
// to the character table; what follows differs:
//
//     1.x  (version 0x0100)  one bitmap for the whole font ("strike"): `height` rows of `row_bytes` bytes, the characters side by
//                            side. Fixed-pitch fonts have no table (character i starts at column i * width); proportional ones have
//                            a table of column offsets, one word per character plus one.
//     2.x  (version 0x0200)  a table of (width, offset:16) per character at 0x76; each glyph is its own bitmap, stored a
//                            byte-column at a time (`height` bytes per column).
//     3.x  (version 0x0300)  the same with (width, offset:32) at 0x94.
//
// Fonts whose type has bit 0 set are stroke ("vector") fonts -- ROMAN, MODERN, SCRIPT: they are listed with their metrics but have
// no bitmaps to draw.
//
// `fonts FILE [--index N] [--show TEXT]` lists the fonts of a file (a module name of the selected corpus will do) and draws TEXT in
// one of them.
#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/cli.hpp"
#include "cb/core/error.hpp"

namespace cb::formats::fonts {

enum class Kind { Vector, Fixed, Proportional };

// Where one character's bitmap is: its width in pixels, and its offset (a column of the strike in 1.x fonts, a byte offset into
// the image in 2.x and 3.x).
struct Place {
    int width = 0;
    std::uint32_t offset = 0;
};

class Font {
public:
    // The font whose .FNT image starts at `at` of `data`. FormatError when there is none.
    static Font parse(std::shared_ptr<const Bytes> data, std::size_t at = 0);
    static Font parse(Bytes data, std::size_t at = 0) { return parse(std::make_shared<const Bytes>(std::move(data)), at); }

    int version = 0;
    std::uint32_t size = 0;
    std::string copyright, name;     // name: the face name ("" when the font has none)
    bool vector = false, italic = false;
    int points = 0, ascent = 0, weight = 0, charset = 0;
    int width = 0;                   // the pixel width of a fixed-pitch font, 0 for a proportional one
    int height = 0, average_width = 0, max_width = 0;
    int first = 0, last = 0, default_char = 0;   // the character range, and the default character (relative to `first`)
    int row_bytes = 0;
    std::uint32_t bits = 0;          // where the 1.x strike starts
    std::map<int, Place> places;     // character code -> its bitmap (raster fonts)

    Kind kind() const { return vector ? Kind::Vector : width != 0 ? Kind::Fixed : Kind::Proportional; }
    // One character as `height` strings of '#' and ' '. A character the font does not have is drawn as its default character; a
    // vector font draws nothing.
    std::vector<std::string> glyph(char32_t code) const;
    // A line of text (UTF-8): each character's glyph and a blank column after it.
    std::vector<std::string> render(std::string_view utf8) const;
    // "Courier        10pt 13px  fixed        chars 32..255  version 2.00"
    std::string describe() const;

private:
    std::shared_ptr<const Bytes> data_;
    std::size_t at_ = 0;
    void read_strike(int count);
};

// Every font of a .FON (or of any 16-bit module): the FONT resources that parse; or a bare .FNT file's one font. A file that holds
// none gives an empty list. IoError for a file that cannot be read.
std::vector<Font> fonts_in(const std::filesystem::path& file);

struct Environment {
    // A corpus name -> the binary (corpus.retail); nullopt when there is none. A path that is a file is taken as it is.
    std::function<std::optional<std::filesystem::path>(std::string_view)> retail;
};

// `fonts FILE [--index N] [--show TEXT]`; exit 1 (with a message on stderr) when FILE is neither a file nor a corpus name.
cli::Command command(const Environment& environment);

}  // namespace cb::formats::fonts
