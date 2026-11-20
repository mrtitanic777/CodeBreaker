#include "formats/gfx.hpp"

#include <zlib.h>

#include <algorithm>
#include <cmath>
#include <cstring>
#include <format>
#include <map>
#include <set>

#include "core/fs.hpp"
#include "core/io.hpp"
#include "core/log.hpp"
#include "core/text.hpp"

namespace cb::formats::gfx {

namespace {

constexpr std::size_t kMovieHeader = 8;                         // magic, version, length
constexpr unsigned kMinVersion = 1, kMaxVersion = 20;            // the GFx versions a movie states
constexpr unsigned kEndTag = 0, kShowFrameTag = 1;
constexpr unsigned kPlaceObject2 = 26, kPlaceObject3 = 70, kDefineFont2 = 48, kDefineFont3 = 75;
constexpr std::size_t kWrapperWindow = 512;                      // how far before the magic the engine's name is looked for
constexpr std::size_t kMaxWrapperName = 48;

// Bits most-significant-first from a buffer, the SWF way.
class BitReader {
public:
    BitReader(ByteView data, std::size_t at) : data_(data), at_(at) {}
    std::size_t position() const { return at_; }   // the byte the next bit is in
    std::uint64_t read(unsigned count) {
        std::uint64_t value = 0;
        for (unsigned k = 0; k < count; ++k) {
            if (at_ >= data_.size()) throw TruncatedError("swf bits", at_, 1, 0);
            value = (value << 1) | ((data_[at_] >> (7 - bit_)) & 1u);
            if (++bit_ == 8) {
                bit_ = 0;
                ++at_;
            }
        }
        return value;
    }
    std::int64_t read_signed(unsigned count) {
        const std::uint64_t value = read(count);
        return (count != 0 && ((value >> (count - 1)) & 1u) != 0) ? static_cast<std::int64_t>(value) - (std::int64_t{1} << count) : static_cast<std::int64_t>(value);
    }
    void align() {
        if (bit_ != 0) {
            bit_ = 0;
            ++at_;
        }
    }

private:
    ByteView data_;
    std::size_t at_;
    unsigned bit_ = 0;
};

// The bytes of a zlib stream, or nothing when it is damaged or cut short.
std::optional<Bytes> inflate_all(ByteView in) {
    z_stream stream;
    std::memset(&stream, 0, sizeof stream);
    if (inflateInit(&stream) != Z_OK) return std::nullopt;
    stream.next_in = const_cast<Bytef*>(in.data());
    stream.avail_in = static_cast<uInt>(in.size());
    Bytes out;
    Bytes chunk(1 << 16);
    int result;
    do {
        stream.next_out = chunk.data();
        stream.avail_out = static_cast<uInt>(chunk.size());
        result = inflate(&stream, Z_NO_FLUSH);
        if (result != Z_OK && result != Z_STREAM_END) {
            inflateEnd(&stream);
            return std::nullopt;
        }
        out.insert(out.end(), chunk.begin(), chunk.end() - stream.avail_out);
    } while (result != Z_STREAM_END && (stream.avail_in != 0 || stream.avail_out == 0));
    inflateEnd(&stream);
    if (result != Z_STREAM_END) return std::nullopt;
    return out;
}

std::string latin1_c_string(ByteView bytes) {
    const auto end = std::find(bytes.begin(), bytes.end(), std::uint8_t{0});
    return text::from_latin1(bytes.first(static_cast<std::size_t>(end - bytes.begin())));
}

ByteView clamped(ByteView data, std::size_t from, std::size_t size) {
    from = std::min(from, data.size());
    return data.subspan(from, std::min(size, data.size() - from));
}

// A font name as a quoted literal: quoted with ' (or " when the name has a ' and no "), backslash and control characters escaped.
std::string repr_text(std::string_view s) {
    const char quote = s.find('\'') != std::string_view::npos && s.find('"') == std::string_view::npos ? '"' : '\'';
    std::string out(1, quote);
    for (const char c : s) {
        if (c == quote || c == '\\') out += '\\';
        if (c == '\n') out += "\\n";
        else if (c == '\t') out += "\\t";
        else if (c == '\r') out += "\\r";
        else if (static_cast<unsigned char>(c) < 0x20 || c == 0x7F) out += std::format("\\x{:02x}", static_cast<unsigned>(static_cast<unsigned char>(c)));
        else out += c;
    }
    return out + quote;
}

std::size_t text_columns(std::string_view s) { return text::display_width(s); }

// `value` padded on the right to `width` columns (as %-Ns).
std::string pad_right(std::string_view value, std::size_t width) {
    std::string out(value);
    const std::size_t have = text_columns(value);
    if (have < width) out.append(width - have, ' ');
    return out;
}

}  // namespace

std::vector<Tag> parse_tags(ByteView body) {
    BitReader frame(body, 0);
    const unsigned bits = static_cast<unsigned>(frame.read(5));
    for (int k = 0; k < 4; ++k) frame.read(bits);   // the frame rectangle
    frame.align();
    std::size_t at = frame.position() + 4;         // the frame rate and count
    std::vector<Tag> tags;
    while (at + 2 <= body.size()) {
        io::ByteReader r(body, "swf tags");
        r.seek(at);
        const unsigned header = r.u16();
        std::uint32_t length = header & 0x3F;
        at += 2;
        if (length == 0x3F) {
            length = r.u32();
            at += 4;
        }
        const ByteView content = clamped(body, at, length);
        tags.push_back({header >> 6, Bytes(content.begin(), content.end())});
        at += length;
        if ((header >> 6) == kEndTag) break;
    }
    return tags;
}

std::vector<Movie> find_movies(ByteView data) {
    std::vector<Movie> movies;
    const auto find_magic = [&](std::string_view magic, std::size_t from) {
        const auto found = std::search(data.begin() + static_cast<std::ptrdiff_t>(std::min(from, data.size())), data.end(), magic.begin(), magic.end());
        return found == data.end() ? std::string::npos : static_cast<std::size_t>(found - data.begin());
    };
    std::size_t pos = 0;
    for (;;) {
        const std::size_t gfx = find_magic("GFX", pos), cfx = find_magic("CFX", pos);
        const std::size_t at = std::min(gfx, cfx);
        if (at == std::string::npos) break;
        pos = at + 3;
        if (at + kMovieHeader > data.size()) continue;
        const unsigned version = data[at + 3];
        if (version < kMinVersion || version > kMaxVersion) continue;
        io::ByteReader header(data, "gfx header");
        header.seek(at + 4);
        const std::uint32_t length = header.u32();
        if (length < kMovieHeader || length > data.size() - at) continue;
        const ByteView packed = clamped(data, at + kMovieHeader, length);
        Bytes body(packed.begin(), packed.end());
        const bool compressed = cfx == at;
        if (compressed) {
            auto inflated = inflate_all(packed);
            if (!inflated) continue;
            body = std::move(*inflated);
        }
        try {
            bool ends = false, shows = false;
            for (const Tag& tag : parse_tags(body)) {
                ends = ends || tag.code == kEndTag;
                shows = shows || tag.code == kShowFrameTag;
            }
            if (!ends || !shows) continue;
        } catch (const FormatError&) {
            continue;
        }
        movies.push_back({at, compressed ? "CFX" : "GFX", std::move(body)});
    }
    return movies;
}

std::string wrapper_name(ByteView data, std::size_t offset) {
    const ByteView window = data.subspan(offset > kWrapperWindow ? offset - kWrapperWindow : 0, offset - (offset > kWrapperWindow ? offset - kWrapperWindow : 0));
    std::vector<std::string> runs;
    std::string current;
    const auto flush = [&] {
        if (current.size() >= 2) runs.push_back(current);
        current.clear();
    };
    for (const std::uint8_t byte : window) {
        if (byte >= 0x20 && byte <= 0x7E) current += static_cast<char>(byte);
        else flush();
    }
    flush();
    for (auto run = runs.rbegin(); run != runs.rend(); ++run) {
        const std::string name(text::trim(*run, " "));
        if (name.size() < 2 || name.size() > kMaxWrapperName) continue;
        bool clean = true, alpha = false;
        std::set<char> distinct;
        for (const char c : name) {
            const bool letter = (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
            alpha = alpha || letter;
            clean = clean && (letter || (c >= '0' && c <= '9') || c == '_' || c == '.' || c == '-');
            distinct.insert(c);
        }
        if (clean && alpha && distinct.size() > 1) return name;
    }
    return "";
}

std::vector<ExternalImage> external_images(ByteView body) {
    std::vector<ExternalImage> out;
    for (const Tag& tag : parse_tags(body)) {
        if ((tag.code != 1001 && tag.code != 1008 && tag.code != 1009) || tag.body.size() < 10) continue;
        io::ByteReader r(tag.body, "DefineExternalImage");
        ExternalImage image;
        image.character_id = r.u16();
        r.skip(2);
        image.width = r.u16();
        image.height = r.u16();
        const unsigned name_length = r.u8();
        image.name = text::from_latin1(clamped(tag.body, 9, name_length));
        out.push_back(std::move(image));
    }
    return out;
}

std::vector<FontInfo> fonts(ByteView body) {
    std::vector<FontInfo> out;
    for (const Tag& tag : parse_tags(body)) {
        if ((tag.code != kDefineFont2 && tag.code != kDefineFont3) || tag.body.size() < 6) continue;
        io::ByteReader r(tag.body, "DefineFont");
        FontInfo font;
        font.version = tag.code == kDefineFont2 ? 2 : 3;
        font.id = r.u16();
        const unsigned flags = r.u8();
        r.skip(1);   // the language code
        const unsigned name_length = r.u8();
        font.name = latin1_c_string(clamped(tag.body, 5, name_length));
        r.seek(5 + name_length);
        font.glyphs = r.u16();
        font.bold = (flags & 1) != 0;
        font.layout = (flags & 0x80) != 0;
        out.push_back(std::move(font));
    }
    return out;
}

std::vector<Placement> placements(ByteView body) {
    std::vector<Placement> out;
    for (const Tag& tag : parse_tags(body)) {
        if (tag.code != kPlaceObject2 && tag.code != kPlaceObject3) continue;
        try {
            io::ByteReader r(tag.body, "PlaceObject");
            const unsigned flags = r.u8();
            if (tag.code == kPlaceObject3) r.skip(1);
            Placement placement;
            placement.depth = r.u16();
            if ((flags & 0x02) != 0) placement.character_id = r.u16();
            if ((flags & 0x04) != 0) {
                BitReader bits(tag.body, r.pos());
                Matrix matrix;
                if (bits.read(1) != 0) {   // a scale, in 16.16 fixed point
                    const unsigned n = static_cast<unsigned>(bits.read(5));
                    matrix.scale_x = static_cast<double>(bits.read_signed(n)) / 65536.0;
                    matrix.scale_y = static_cast<double>(bits.read_signed(n)) / 65536.0;
                }
                if (bits.read(1) != 0) {   // a rotation or skew: read past
                    const unsigned n = static_cast<unsigned>(bits.read(5));
                    bits.read(n);
                    bits.read(n);
                }
                const unsigned n = static_cast<unsigned>(bits.read(5));
                matrix.translate_x = static_cast<double>(bits.read_signed(n)) / 20.0;   // twips
                matrix.translate_y = static_cast<double>(bits.read_signed(n)) / 20.0;
                placement.matrix = matrix;
            }
            out.push_back(placement);
        } catch (const FormatError&) {
            // a tag too short to read is left out
        }
    }
    return out;
}

std::vector<Contour> glyph_contours(ByteView data, std::size_t start) {
    BitReader bits(data, start);
    const unsigned fill_bits = static_cast<unsigned>(bits.read(4)), line_bits = static_cast<unsigned>(bits.read(4));
    std::int64_t x = 0, y = 0;
    std::vector<Contour> contours;
    Contour current;
    bool have_current = false;
    const auto need_contour = [&] {
        if (!have_current) throw FormatError("glyph shape", bits.position(), "a line or curve before the first move");
    };
    for (;;) {
        if (bits.read(1) == 0) {   // not an edge: a style change, or the end
            const unsigned flags = static_cast<unsigned>(bits.read(5));
            if (flags == 0) break;
            if ((flags & 0x01) != 0) {   // a move
                const unsigned n = static_cast<unsigned>(bits.read(5));
                x = bits.read_signed(n);
                y = bits.read_signed(n);
                if (have_current && !current.empty()) contours.push_back(current);
                current = {PathOp{PathOp::Kind::Move, x, y, 0, 0}};
                have_current = true;
            }
            if ((flags & 0x02) != 0) bits.read(fill_bits);
            if ((flags & 0x04) != 0) bits.read(fill_bits);
            if ((flags & 0x08) != 0) bits.read(line_bits);
            if ((flags & 0x10) != 0) break;   // new styles: not in a glyph
        } else if (bits.read(1) != 0) {   // a straight edge
            const unsigned n = static_cast<unsigned>(bits.read(4)) + 2;
            std::int64_t dx, dy;
            if (bits.read(1) != 0) {
                dx = bits.read_signed(n);
                dy = bits.read_signed(n);
            } else if (bits.read(1) != 0) {
                dx = 0;
                dy = bits.read_signed(n);
            } else {
                dx = bits.read_signed(n);
                dy = 0;
            }
            x += dx;
            y += dy;
            need_contour();
            current.push_back(PathOp{PathOp::Kind::Line, x, y, 0, 0});
        } else {   // a curved edge
            const unsigned n = static_cast<unsigned>(bits.read(4)) + 2;
            const std::int64_t control_dx = bits.read_signed(n), control_dy = bits.read_signed(n), anchor_dx = bits.read_signed(n), anchor_dy = bits.read_signed(n);
            const std::int64_t control_x = x + control_dx, control_y = y + control_dy;
            x = control_x + anchor_dx;
            y = control_y + anchor_dy;
            need_contour();
            current.push_back(PathOp{PathOp::Kind::Curve, x, y, control_x, control_y});
        }
    }
    if (have_current && !current.empty()) contours.push_back(current);
    return contours;
}

cli::Command command(const Environment&) {
    const auto movie_command = [](std::string name, std::string summary, auto per_movie) {
        return cli::Command{std::move(name), std::move(summary), {cli::Positional{"file", "a file holding GFx movies"}},
                            [per_movie](const cli::Args& a) {
                                const std::string path = a.required("file");
                                const Bytes data = fs::read_file(fs::from_text(path));
                                const std::vector<Movie> movies = find_movies(data);
                                if (movies.empty()) out("no GFx movies in {}\n", path);
                                for (const Movie& movie : movies) per_movie(data, movie);
                                return kExitOk;
                            },
                            {}, {}, {}};
    };
    cli::Command scan = movie_command("scan", "list the movies of a file: where, their name, how many images and fonts", [](ByteView data, const Movie& movie) {
        out("{} at ${:X}  {} {} images, {} fonts\n", movie.magic, movie.offset, pad_right(wrapper_name(data, movie.offset), 28),
            external_images(movie.body).size(), fonts(movie.body).size());
    });
    cli::Command images = movie_command("images", "the external images each movie draws", [](ByteView data, const Movie& movie) {
        const std::string name = pad_right(wrapper_name(data, movie.offset), 28);
        for (const ExternalImage& image : external_images(movie.body))
            out("{} character {:<4} {:>4}x{:<4} {}\n", name, image.character_id, image.width, image.height, image.name);
    });
    cli::Command font_list = movie_command("fonts", "the fonts each movie embeds", [](ByteView data, const Movie& movie) {
        const std::string name = pad_right(wrapper_name(data, movie.offset), 24);
        for (const FontInfo& font : fonts(movie.body))
            out("{} DefineFont{} id {} {} {} glyphs{}{}\n", name, font.version, font.id, pad_right(repr_text(font.name), 20), font.glyphs, font.bold ? " bold" : "",
                font.layout ? " layout" : "");
    });
    cli::Command placed = movie_command("placements", "where each movie puts its external images", [](ByteView data, const Movie& movie) {
        const std::string name = pad_right(wrapper_name(data, movie.offset), 24);
        std::map<unsigned, std::string> names;
        for (const ExternalImage& image : external_images(movie.body)) names[image.character_id] = image.name;
        for (const Placement& p : placements(movie.body)) {
            if (!p.character_id || !p.matrix || !names.contains(*p.character_id)) continue;
            out("{} depth {:<3} {} at ({:.1f}, {:.1f}) scale ({:.3f}, {:.3f})\n", name, p.depth, pad_right(names.at(*p.character_id), 22), p.matrix->translate_x,
                p.matrix->translate_y, p.matrix->scale_x, p.matrix->scale_y);
        }
    });
    cli::Command ttf{"font-ttf",
                     "write a DefineFont3 font as a TrueType file",
                     {cli::Positional{"file", "a file holding GFx movies"}, cli::Positional{"font", "the font's character id"}, cli::Positional{"output", "the .ttf to write"},
                      cli::Positional{"family", "the family name (default: the font's name without dashes)", false}},
                     [](const cli::Args& a) {
                         const Bytes data = fs::read_file(fs::from_text(a.required("file")));
                         const auto id = a.integer<unsigned>("font");
                         if (!id) throw UsageError("the font's id is a number");
                         const TrueType font = font_to_ttf(data, *id, a.value("family"));
                         fs::write_file(fs::from_text(a.required("output")), font.file);
                         out("{}: {} glyphs -> {}\n", repr_text(font.name), font.glyphs, a.required("output"));
                         return kExitOk;
                     },
                     {}, {}, {}};
    return cli::Command{"gfx", "Scaleform GFx menus: movies, images, fonts, placements; a font as TTF", {}, {}, {scan, images, font_list, placed, ttf}, "Xbox 360", ""};
}

}  // namespace cb::formats::gfx
